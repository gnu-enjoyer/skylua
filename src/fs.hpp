#pragma once

#include <shlobj.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace fs {
enum class Destination { RUNTIME, LOGS };
enum class RetryMethod { NONE, LOOP };

inline std::optional<std::filesystem::path> TryGetMyDocuments() {
  TCHAR szPath[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath);
  return SUCCEEDED(result) ? std::optional(szPath) : std::nullopt;
}

template <Destination D>
std::filesystem::path FindOrCreateDirectory() {
  auto dir = (D == Destination::LOGS)
                 ? TryGetMyDocuments()
                       ? TryGetMyDocuments().value() /
                             std::filesystem::path("My Games/Skyrim Special Edition/SKSE/Logs")
                       : std::filesystem::current_path()
                 : std::filesystem::current_path() / "SkyLua";

  if (std::filesystem::exists(dir)) {
    return dir;
  }
  return std::filesystem::create_directories(dir) ? dir : std::filesystem::current_path();
}

static inline auto log_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
    (FindOrCreateDirectory<Destination::LOGS>() / "SkyLua.log").string(), true);

class LuaFile {
  static inline auto root_dir = FindOrCreateDirectory<Destination::RUNTIME>();

  static std::vector<std::filesystem::path> GetFilesOnDisk(const std::filesystem::path& root) {
    if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
      return {};
    }
    return std::filesystem::recursive_directory_iterator(root) |
           std::views::filter([](const auto& p) {
             return p.is_regular_file() && p.path().extension() == ".lua";
           }) |
           std::views::transform(
               [&root](const auto& e) { return std::filesystem::relative(e.path(), root); }) |
           std::ranges::to<std::vector>();
  }

 public:
  std::string relative_path_str;

  [[nodiscard]] auto AbsolutePath() const {
    return std::filesystem::path(root_dir / relative_path_str);
  }

  friend bool operator==(const LuaFile&, const LuaFile&) = default;

  static std::vector<LuaFile> GetLuaFilesOnDisk(
      const std::filesystem::path& root, const std::unordered_set<std::string>& ignore_list = {}) {
    return GetFilesOnDisk(root) |
           std::views::filter([&ignore_list](const std::filesystem::path& path) {
             return !ignore_list.contains(path.filename().string()) && path.has_relative_path();
           }) |
           std::views::transform(
               [](const std::filesystem::path& path) { return LuaFile{path.string()}; }) |
           std::ranges::to<std::vector>();
  }
};

struct LuaFileHash {
  std::size_t operator()(const LuaFile& lf) const noexcept {
    return std::hash<std::string>{}(lf.relative_path_str);
  }
};

typedef std::unordered_set<LuaFile, LuaFileHash> HotReloadCallback;

template <RetryMethod M>
class HotReloader {
  std::unordered_set<std::string> ignore_list;
  HotReloadCallback paths_buffer;
  HANDLE fd = nullptr;

 public:
  void await(std::function<void(HotReloadCallback)> f) {
    if (fd != INVALID_HANDLE_VALUE) {
      DWORD buffer[1024]{};
      DWORD bytes_returned;

      if (ReadDirectoryChangesW(fd, &buffer, sizeof(buffer), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE,
                                &bytes_returned, NULL, NULL)) {
        auto p_notify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
        std::filesystem::path updated_path(p_notify->FileName);

        if (updated_path.extension() == ".lua" &&
            !ignore_list.contains(updated_path.filename().string())) {
          auto [_, success] = paths_buffer.emplace(LuaFile{updated_path.string()});
          if (success) {
            SKSE::GetTaskInterface()->AddTask([&f, cb = paths_buffer] {
              // tbd synchronise instead of copying over
              f(cb);
            });
          }
        }

        if constexpr (M == RetryMethod::LOOP) {
          std::this_thread::yield();
          paths_buffer.clear();
          await(f);
        }
      }
    }
  }

  ~HotReloader() {
    if (fd) {
      CloseHandle(fd);
    }
  }

  explicit HotReloader(const std::unordered_set<std::string>& ignored = {}) : ignore_list(ignored) {
    fd = CreateFile(FindOrCreateDirectory<Destination::RUNTIME>().c_str(), FILE_LIST_DIRECTORY,
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS, NULL);
  }

  template <typename F>
  static void DetachWithCallback(F&& cb) {
    std::jthread([&cb] {
      auto reloader = HotReloader();
      reloader.await(cb);
    }).detach();
  }
};
}
