#pragma once

#include "fs.hpp"

namespace cfg {
enum class LuaLibs { DEFAULT, FULL };

static constexpr auto cfg_file_name = "skylua.json"sv;
static inline const auto cfg_path =
    fs::FindOrCreateDirectory<fs::Destination::RUNTIME>() / cfg_file_name;

struct Config {
  spdlog::level::level_enum log_level{spdlog::level::trace};
  std::uint64_t tick_rate{1000};
  bool enable_hot_reload{true};
  LuaLibs lua_libs{LuaLibs::DEFAULT};
};

static const Config* LoadOrDefault(bool force_reload = false) {
  static Config config{};
  static bool first_load{true};

  if (first_load || force_reload) {
    auto _res = std::filesystem::exists(cfg_path)
                    ? glz::read_file_json(config, cfg_path.string(), std::string{})
                    : glz::write_file_json(config, cfg_path.string(), std::string{});
    first_load = false;
  }
  return &config;
}

inline auto LoadLuaLibs(sol::state& lua) {
  LoadOrDefault()->lua_libs == LuaLibs::DEFAULT
      ? lua.open_libraries(sol::lib::bit32, sol::lib::table)
      : lua.open_libraries();
}
}

template <>
struct glz::meta<cfg::LuaLibs> {
  using enum cfg::LuaLibs;
  static constexpr auto value = enumerate(DEFAULT, FULL);
};