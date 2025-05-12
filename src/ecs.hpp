#pragma once

#include "entt.hpp"
#include "fs.hpp"
#include "lua.hpp"

namespace ecs {
typedef fs::LuaFile Fd;
typedef std::unique_ptr<lua::ScopedState> State;
typedef sol::protected_function Tick;

inline entt::registry registry;

template <glue::GameEvent>
struct EventHandler {
  sol::protected_function handler;
};

[[noreturn]] static void LatentTick() {
  static const auto tick_rate = std::chrono::milliseconds(cfg::LoadOrDefault()->tick_rate);

  for (;;) {
    std::this_thread::sleep_for(tick_rate);

    SKSE::GetTaskInterface()->AddTask([] {
      auto view = registry.view<const Tick>();
      for (auto [_, tick] : view.each()) tick();
    });
  }
}

inline auto EnableLatentTick() { std::jthread(&LatentTick).detach(); }

inline auto BindEventHandlersToEntity(const entt::entity& ent,
                                      std::unique_ptr<lua::ScopedState>& state) {
  magic_enum::enum_for_each<glue::GameEvent>([&](auto val) {
    constexpr auto func_name = glue::GetLuaFuncName<val>();
    state->logger->trace("Checking handler: {}", func_name);
    sol::protected_function func = state->skylua[func_name];
    if (func.valid()) {
      state->logger->info("Bound {}", func_name);
      registry.emplace<EventHandler<val>>(ent, std::move(func));
    }
  });
}

inline auto AddLuaFileToRegistry(const fs::LuaFile& fd) {
  const auto e = registry.create();
  auto state = lua::ScopedState::FromPath(fd);
  if (auto maybe_tick = state->MaybeGetTick()) {
    state->logger->trace("Tick registered for: {}", state->name);
    registry.emplace<Tick>(e, maybe_tick.value());
  }
  BindEventHandlersToEntity(e, state);
  registry.emplace<State>(e, std::move(state));
  registry.emplace<Fd>(e, fd);
}

inline auto UnbindAllEventHandlersForEntity(const entt::entity& e) {
  magic_enum::enum_for_each<glue::GameEvent>([&](auto val) {
    if (auto maybe_handler = registry.try_get<EventHandler<val>>(e)) {
      maybe_handler->handler.abandon();
    }
  });

  if (auto maybe_tick = registry.try_get<Tick>(e)) {
    maybe_tick->abandon();
  }
}

inline auto RebuildEntityForPath(const fs::LuaFile& fd) {
  for (auto e : registry.view<const Fd>()) {
    auto ent_fd = registry.get<const Fd>(e);
    if (ent_fd.relative_path_str == fd.relative_path_str) {
      UnbindAllEventHandlersForEntity(e);
      registry.destroy(e);
      AddLuaFileToRegistry(ent_fd);
    }
  }
}
}