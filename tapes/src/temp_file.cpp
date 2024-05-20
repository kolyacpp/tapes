#include <tapes/temp_file.h>

namespace fs = std::filesystem;

namespace tapes {

std::filesystem::path temp_file::temp_directory{};

temp_file::temp_file(const std::string& prefix) {
  if (temp_directory.empty()) {
    temp_directory = std::filesystem::temp_directory_path() / "tapes";
    std::filesystem::create_directory(temp_directory);
  }

  if (!std::filesystem::is_directory(temp_directory)) {
    throw tape_exception("Temp directory is not a directory");
  }

  if (!std::filesystem::exists(temp_directory)) {
    std::filesystem::create_directories(temp_directory);
  }

  size_t suffix = 0;
  do {
    // TODO: replace with propper name generation
    _filepath = temp_directory / (prefix + "-" + std::to_string(suffix++));
  } while (suffix < 128 && std::filesystem::exists(_filepath));

  if (suffix == 128) {
    throw tape_exception("Failed to find name for temp file");
  }

  std::ofstream file(_filepath);
  if(!file.is_open()) {
    throw tape_exception("Failed to create temp file: " + _filepath.string());
  }
}

temp_file::~temp_file() {
  std::error_code ec;
  // noexcept overload of remove
  std::filesystem::remove(_filepath, ec);
}

void temp_file::set_temp_directory(const std::filesystem::path& path) {
  temp_directory = path;
}

const std::filesystem::path& temp_file::get_path() const {
  return _filepath;
}

} // namespace tapes