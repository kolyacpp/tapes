#pragma once

#include <tapes/tape_exception.h>

#include <filesystem>
#include <fstream>

namespace tapes {

class temp_file {
public:
  temp_file(const std::string& prefix);
  ~temp_file();

  temp_file(const temp_file&) = delete;
  temp_file& operator=(const temp_file&) = delete;

  temp_file(temp_file&&) = default;
  temp_file& operator=(temp_file&&) = default;

  static void set_temp_directory(const std::filesystem::path& path);
  const std::filesystem::path& get_path() const;

private:
  static std::filesystem::path temp_directory;
  std::filesystem::path _filepath;
};

} // namespace tapes
