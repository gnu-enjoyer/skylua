#pragma once

#include "ecs.hpp"
#include "glue.hpp"

#define RE_EVENT_HANDLER(e)                                                                        \
  RE::BSEventNotifyControl ProcessEvent(const RE::e* event, RE::BSTEventSource<RE::e>*) override { \
    ecs::registry.view<const ecs::EventHandler<glue::GameEvent::e>>().each(                        \
        [&event](auto& f) { f.handler(event); });                                                  \
    return RE::BSEventNotifyControl::kContinue;                                                    \
  }

#define SKSE_EVENT_HANDLER(e)                                                               \
  RE::BSEventNotifyControl ProcessEvent(const SKSE::e* event, RE::BSTEventSource<SKSE::e>*) \
      override {                                                                            \
    ecs::registry.view<const ecs::EventHandler<glue::GameEvent::e>>().each(                 \
        [&event](auto& f) { f.handler(event); });                                           \
    return RE::BSEventNotifyControl::kContinue;                                             \
  }

namespace sink {
struct Empty {};

struct LuaSink final :
#define X(x) INHERIT_EVENT_RE(x),
    GAME_EVENTS_RE
#undef X
#define X(x) INHERIT_EVENT_SKSE(x),
        GAME_EVENTS_SKSE
#undef X
            Empty {
#define X(e) RE_EVENT_HANDLER(e)
  GAME_EVENTS_RE
#undef X

#define X(e) SKSE_EVENT_HANDLER(e)
  GAME_EVENTS_SKSE
#undef X

  LuaSink(const LuaSink&) = delete;
  LuaSink(LuaSink&&) = delete;
  LuaSink& operator=(const LuaSink&) = delete;
  LuaSink& operator=(LuaSink&&) = delete;

  static LuaSink* GetSingleton() {
    static LuaSink sinkleton;
    return &sinkleton;
  }

 private:
  LuaSink() = default;
};
}