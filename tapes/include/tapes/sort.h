#pragma once

#include "tape.h"

#include <tapes/file_tape.h>
#include <tapes/temp_file.h>

#include <queue>

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>

namespace tapes {

template <typename Comp = std::less<>>
class multiway_merge_sort {
public:
  multiway_merge_sort(size_t buffer_size, size_t tape_count, Comp comp = {}) noexcept(
      std::is_nothrow_default_constructible_v<Comp> && std::is_nothrow_move_constructible_v<Comp>)
      : _comp(std::move(comp)),
        _tape_count(tape_count),
        _buffer_size(buffer_size) {
    assert(buffer_size >= 2);
    assert(tape_count >= 2);

    // required because of using heap of size (tape_count) for merging tapes in phase2()
    assert(buffer_size >= tape_count);
  }

  void sort(itape& input, itape& output) {
    std::vector<temp_file> temp_files;
    std::vector<file_tape> tapes;
    for (size_t i = 0; i < _tape_count; i++) {
      temp_files.emplace_back("sort_temp" + std::to_string(i));
      tapes.emplace_back(temp_files.back().get_path());
    }

    sort_blocks(input, output);

    uint64_t vals_per_tape = _buffer_size;
    while (true) {
      fill_tapes(tapes, output, vals_per_tape);
      if (merge(tapes, output, vals_per_tape)) {
        return;
      }
      vals_per_tape *= _tape_count;
    }
  }

private:
  Comp _comp;
  size_t _tape_count;
  size_t _buffer_size;

private:
  // sorts blocks of size _buffer_size and writes them to output tape
  void sort_blocks(itape& input, itape& output) {
    std::vector<itape::int_type> buffer;
    buffer.reserve(_buffer_size);

    do {
      std::optional<itape::int_type> val;
      buffer.clear();
      while ((val = input.read()) && buffer.size() < _buffer_size) {
        buffer.push_back(*val);
        input.move_right();
      }

      std::sort(buffer.begin(), buffer.end(), _comp);

      for (auto v : buffer) {
        output.write(v);
        output.move_right();
      }
    } while (!buffer.empty());
  }

  // fills tapes with blocks of values from output tape
  void fill_tapes(std::vector<file_tape>& tapes, itape& output, uint64_t values_per_tape) {
    for (auto& tape : tapes) {
      tape.clear();
    }
    output.rewind();

    for (auto tape = tapes.begin(); auto val = output.read(); ++tape) {
      if (tape == tapes.end()) {
        tape = tapes.begin();
      }

      for (uint64_t i = 0; i < values_per_tape && val; ++i) {
        tape->write(*val);
        tape->move_right();
        output.move_right();
        val = output.read();
      }
    }
  }

  struct heap_element {
    itape::int_type value;
    file_tape* current_tape;
    uint64_t tape_reads;

    heap_element(itape::int_type value, file_tape* current_tape, uint64_t tape_reads)
        : value(value),
          current_tape(current_tape),
          tape_reads(tape_reads) {}
  };

  struct heap_comparator {
    Comp& comp;

    bool operator()(const heap_element& lhs, const heap_element& rhs) const {
      return comp(rhs.value, lhs.value);
    }
  };

  using heap_min = std::priority_queue<heap_element, std::vector<heap_element>, heap_comparator>;

  // merges blocks from tapes into output tape using min-heap
  // block sizes increase
  bool merge(std::vector<file_tape>& tapes, itape& output, uint64_t values_per_tape) {
    heap_comparator comp{_comp};
    heap_min pq(comp);
    std::vector<heap_element> next_iteration;

    for (auto& tape : tapes) {
      tape.rewind();
      if (auto val = tape.read()) {
        next_iteration.emplace_back(*val, std::addressof(tape), 1);
        tape.move_right();
      }
    }

    output.rewind();

    uint64_t tape_reads_limit = 0;

    while (!next_iteration.empty()) {
      for (auto& el : next_iteration) {
        pq.push(el);
      }
      next_iteration.clear();
      tape_reads_limit += values_per_tape;

      while (!pq.empty()) {
        auto el = pq.top();
        pq.pop();

        output.write(el.value);
        output.move_right();

        if (auto val = el.current_tape->read()) {
          el.value = *val;
          el.current_tape->move_right();
          ++el.tape_reads;

          if (el.tape_reads > tape_reads_limit) {
            next_iteration.push_back(el);
          } else {
            pq.emplace(el);
          }
        }
      }
    }

    return tape_reads_limit == values_per_tape;
  }
};

extern template void multiway_merge_sort<std::less<>>::sort(itape&, itape&);
extern template void multiway_merge_sort<std::greater<>>::sort(itape&, itape&);

} // namespace tapes
