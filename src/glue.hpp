#pragma once

#define GAME_EVENTS_RE \
  X(TESHitEvent)       \
  X(TESActivateEvent)  \
  X(MenuOpenCloseEvent)

#define GAME_EVENTS_SKSE \
  X(CrosshairRefEvent)   \
  X(ModCallbackEvent)    \
  X(ActionEvent)

#define GAME_EVENTS \
  GAME_EVENTS_RE    \
  GAME_EVENTS_SKSE

#define INHERIT_EVENT_RE(e) RE::BSTEventSink<RE::e>

#define INHERIT_EVENT_SKSE(e) RE::BSTEventSink<SKSE::e>

namespace glue {
enum class GameEventHandler {
#define X(name) Handle_##name,
  GAME_EVENTS
#undef X
};

enum class GameEvent {
#define X(name) name,
  GAME_EVENTS
#undef X
};

template <GameEvent E>
constexpr auto GetLuaFuncName() {
  return magic_enum::enum_name(
      magic_enum::enum_value<GameEventHandler>(magic_enum::enum_index<E>()));
}

template <typename E>
auto ToLua(const char* name, sol::state& lua) {
  sol::table t = lua.create_table();
  for (auto& e : magic_enum::enum_values<E>()) {
    t[magic_enum::enum_name(e)] = e;
  }
  lua["SkyLua"][name] = t;
}

inline auto BindEnums(sol::state& lua) {
  ToLua<RE::ActorValue>("ActorValue", lua);
  ToLua<RE::ACTOR_BASE_DATA::Flag>("ActorBaseDataFlag", lua);
  ToLua<RE::Actor::BOOL_FLAGS>("ActorBoolFlags", lua);

  ToLua<RE::TESHitEvent::Flag>("HitEventFlag", lua);
  ToLua<RE::QuestFlag>("QuestFlag", lua);
  ToLua<RE::QUEST_DATA::Type>("QuestType", lua);

  ToLua<SKSE::ActionEvent::Slot>("ActionEventSlot", lua);
  ToLua<SKSE::ActionEvent::Type>("ActionEventType", lua);
}

template <typename... Args>
void ConsoleLog(Args&&... args) {
  RE::ConsoleLog::GetSingleton()->Print(std::forward<Args>(args)...);
}

inline void BindHelperFuncs(sol::table& lua) {
  lua.set_function("get_player", [] { return RE::PlayerCharacter::GetSingleton(); });
  lua.set_function("print", [](std::string_view sv) { ConsoleLog(sv.data()); });
  lua.set_function("debug", [](std::string_view sv) { return RE::DebugMessageBox(sv.data()); });
  lua.set_function("notify", [](std::string_view sv) { return RE::DebugNotification(sv.data()); });
}
}
