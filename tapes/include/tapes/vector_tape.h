#include "itape.h"

#include <vector>

namespace tapes {

class vector_tape final : public itape {
public:
  using int_type = itape::int_type;

  vector_tape() noexcept = default;
  ~vector_tape() override = default;

  vector_tape(std::initializer_list<int_type> list);

  template <typename Vector>
    requires std::is_same_v<std::remove_cvref_t<Vector>, std::vector<int_type>>
  vector_tape(Vector&& vector) : _vector(std::forward<Vector>(vector)) {}

  vector_tape(const vector_tape&) = default;
  vector_tape& operator=(const vector_tape&) = default;

  vector_tape(vector_tape&&) = default;
  vector_tape& operator=(vector_tape&&) = default;

  void write(int_type value) override;
  std::optional<int_type> read() noexcept override;
  void move_left() noexcept override;
  void move_right() noexcept override;
  void rewind() noexcept override;
  void clear() noexcept override;

  const std::vector<int_type>& get_vector() const noexcept;

private:
  std::vector<int_type> _vector;
  std::size_t _position = 0;
};

} // namespace tapes
