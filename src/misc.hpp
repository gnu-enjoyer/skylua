#pragma once

namespace misc {
inline auto NamedLogger(const std::string& name) {
  auto cfg = cfg::LoadOrDefault();
  spdlog::logger logger(name, fs::log_sink);
  logger.enable_backtrace(32);
  logger.set_level(cfg->log_level);
  logger.flush_on(cfg->log_level);
  return logger;
}

inline auto EnableDefaultLogger() {
  auto logger = NamedLogger("SkyLua");
  spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
}
}
