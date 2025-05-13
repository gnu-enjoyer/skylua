#pragma once

#include "cfg.hpp"
#include "fs.hpp"
#include "glue.hpp"
#include "misc.hpp"

namespace lua {
struct ScopedState {
  sol::state lua;
  sol::table skylua;
  std::string name;
  std::shared_ptr<spdlog::logger> logger;

  void BindUsertypes() {
    //@lua class TESObjectREFR
    lua.new_usertype<RE::TESObjectREFR>(
        "TESObjectREFR", sol::no_constructor,
        //@lua method get_name || -> string
        "get_name", [](RE::TESObjectREFR* ptr) { return ptr->GetName(); },
        //@lua method as_actor || -> Actor
        "as_actor",
        [](RE::TESObjectREFR* ptr) -> std::optional<RE::Actor*> {
          return ptr->As<RE::Actor>() ? std::optional(ptr->As<RE::Actor>()) : std::nullopt;
        });

    //@lua class Actor
    lua.new_usertype<RE::Actor>("Actor", sol::no_constructor, "as_actor_owner",
                                //@lua method as_actor_owner || -> ActorValueOwner
                                [](RE::Actor* ptr) { return ptr->AsActorValueOwner(); });

    //@lua class PlayerCharacter
    lua.new_usertype<RE::PlayerCharacter>("PlayerCharacter", sol::no_constructor);

    //@lua class ActorValueOwner
    lua.new_usertype<RE::ActorValueOwner>(
        //@lua method get_actor_value | ActorValue | -> float
        "ActorValueOwner", sol::no_constructor, "get_actor_value",
        [](RE::ActorValueOwner* ptr, RE::ActorValue k) { return ptr->GetActorValue(k); },
        //@lua method set_actor_value | ActorValue, float |
        "set_actor_value",
        [](RE::ActorValueOwner* ptr, RE::ActorValue k, float v) { ptr->SetActorValue(k, v); });

    //@lua class ActionEvent
    lua.new_usertype<SKSE::ActionEvent>(
        "ActionEvent", sol::no_constructor,
        //@lua method get_type || -> integer
        "get_type",
        [](const SKSE::ActionEvent* ptr) -> std::uint32_t { return ptr->type.underlying(); },
        //@lua method get_source || -> integer
        "get_source",
        [](const SKSE::ActionEvent* ptr) -> std::uint32_t { return ptr->slot.underlying(); });

    //@lua class CrosshairRefEvent
    lua.new_usertype<SKSE::CrosshairRefEvent>(
        "CrosshairRefEvent", sol::no_constructor,
        //@lua method get_crosshair_ref || -> TESObjectREFR
        "get_crosshair_ref", [](const SKSE::CrosshairRefEvent* ptr) -> RE::TESObjectREFR* {
          return ptr->crosshairRef.get();
        });

    //@lua class ModCallbackEvent
    lua.new_usertype<SKSE::ModCallbackEvent>(
        "ModCallbackEvent", sol::no_constructor,
        //@lua method get_event_name || -> string
        "get_event_name", [](const SKSE::ModCallbackEvent* ptr) { return ptr->eventName; });

    //@lua class MenuOpenCloseEvent
    lua.new_usertype<RE::MenuOpenCloseEvent>(
        "MenuOpenCloseEvent", sol::no_constructor,
        //@lua method get_menu_name || -> string
        "get_menu_name", [](const RE::MenuOpenCloseEvent* ptr) { return ptr->menuName; });

    //@lua class TESActivateEvent
    lua.new_usertype<RE::TESActivateEvent>(
        "TESActivateEvent", sol::no_constructor,
        //@lua method get_activated || -> TESObjectREFR
        "get_activated",
        [](const RE::TESActivateEvent* ptr) -> RE::TESObjectREFR* {
          return ptr->objectActivated.get();
        },
        //@lua method get_activator || -> TESObjectREFR
        "get_activator",
        [](const RE::TESActivateEvent* ptr) -> RE::TESObjectREFR* { return ptr->actionRef.get(); });

    //@lua class TESHitEvent
    lua.new_usertype<RE::TESHitEvent>(
        "TESHitEvent", sol::no_constructor,
        //@lua method get_cause || -> TESObjectREFR
        "get_cause",
        [](const RE::TESHitEvent* ptr) -> RE::TESObjectREFR* { return ptr->cause.get(); },
        //@lua method get_target || -> TESObjectREFR
        "get_target",
        [](const RE::TESHitEvent* ptr) -> RE::TESObjectREFR* { return ptr->target.get(); },
        //@lua method get_source || -> integer
        "get_source", [](const RE::TESHitEvent* ptr) -> std::uint32_t { return ptr->source; },
        //@lua method get_projectile || -> integer
        "get_projectile",
        [](const RE::TESHitEvent* ptr) -> std::uint32_t { return ptr->projectile; },
        //@lua method get_flags || -> integer
        "get_flags",
        [](const RE::TESHitEvent* ptr) -> std::uint8_t { return ptr->flags.underlying(); });
  }

  auto DefaultCfg() {
    cfg::LoadLuaLibs(lua);
    lua.set_function("lua_error_handler",
                     [logger = logger](std::string_view err) { logger->error(err); });
    sol::protected_function::set_default_handler(lua["lua_error_handler"]);
    BindUsertypes();

    skylua = lua["SkyLua"].get_or_create<sol::table>();
    skylua.set_function("log", [logger = logger](sol::variadic_args args) {
      for (auto i : args) {
        logger->info(i.as<std::string_view>());
      }
    });

    glue::BindEnums(lua);
    glue::BindHelperFuncs(skylua);
    logger->info("DefaultCfg loaded");
  }

 public:
  ScopedState(const ScopedState&) = delete;
  ScopedState& operator=(const ScopedState&) = delete;
  ScopedState(ScopedState&&) = delete;
  ScopedState& operator=(ScopedState&&) = delete;

  std::optional<sol::protected_function> MaybeGetTick() {
    if (skylua["OnTick"]) {
      return skylua["OnTick"];
    }
    return {};
  }

  explicit ScopedState(const fs::LuaFile& fd)
      : name(fd.relative_path_str),
        logger(std::make_shared<spdlog::logger>(misc::NamedLogger(fd.relative_path_str))) {
    logger->info("Initialising {}", name);
    DefaultCfg();
  }

  static auto FromPath(const fs::LuaFile& fd) {
    auto new_state_ptr = std::make_unique<ScopedState>(fd);
    new_state_ptr->lua.safe_script_file(fd.AbsolutePath().string());
    return new_state_ptr;
  }
};
}