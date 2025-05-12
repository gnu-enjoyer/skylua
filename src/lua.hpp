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
    // Events
    lua.new_usertype<SKSE::ActionEvent>(
        "ActionEvent", sol::no_constructor, "get_type",
        [](const SKSE::ActionEvent* ptr) -> std::uint32_t { return ptr->type.underlying(); },
        "get_source",
        [](const SKSE::ActionEvent* ptr) -> std::uint32_t { return ptr->slot.underlying(); });

    lua.new_usertype<SKSE::CrosshairRefEvent>(
        "CrosshairRefEvent", sol::no_constructor, "get_crosshair_ref",
        [](const SKSE::CrosshairRefEvent* ptr) -> RE::TESObjectREFR* {
          return ptr->crosshairRef.get();
        });

    lua.new_usertype<SKSE::ModCallbackEvent>(
        "ModCallbackEvent", sol::no_constructor, "get_event_name",
        [](const SKSE::ModCallbackEvent* ptr) { return ptr->eventName; });

    lua.new_usertype<RE::MenuOpenCloseEvent>(
        "TESActivateEvent", sol::no_constructor, "get_menu_name",
        [](const RE::MenuOpenCloseEvent* ptr) { return ptr->menuName; }, "get_activator",
        [](const RE::TESActivateEvent* ptr) -> RE::TESObjectREFR* { return ptr->actionRef.get(); });

    lua.new_usertype<RE::TESActivateEvent>(
        "TESActivateEvent", sol::no_constructor, "get_activated",
        [](const RE::TESActivateEvent* ptr) -> RE::TESObjectREFR* {
          return ptr->objectActivated.get();
        },
        "get_activator",
        [](const RE::TESActivateEvent* ptr) -> RE::TESObjectREFR* { return ptr->actionRef.get(); });

    lua.new_usertype<RE::TESHitEvent>(
        "TESHitEvent", sol::no_constructor, "get_cause",
        [](const RE::TESHitEvent* ptr) -> RE::TESObjectREFR* { return ptr->cause.get(); },
        "get_target",
        [](const RE::TESHitEvent* ptr) -> RE::TESObjectREFR* { return ptr->target.get(); },
        "get_source", [](const RE::TESHitEvent* ptr) -> std::uint32_t { return ptr->source; },
        "get_projectile",
        [](const RE::TESHitEvent* ptr) -> std::uint32_t { return ptr->projectile; }, "get_flags",
        [](const RE::TESHitEvent* ptr) -> std::uint8_t { return ptr->flags.underlying(); });

    // Core
    lua.new_usertype<RE::TESObjectREFR>(
        "TESObjectREFR", sol::no_constructor, "get_name",
        [](RE::TESObjectREFR* ptr) { return ptr->GetName(); }, "as_actor",
        [](RE::TESObjectREFR* ptr) -> std::optional<RE::Actor*> {
          return ptr->As<RE::Actor>() ? std::optional(ptr->As<RE::Actor>()) : std::nullopt;
        });

    lua.new_usertype<RE::Actor>("Actor", sol::no_constructor, "as_actor_owner",
                                [](RE::Actor* ptr) { return ptr->AsActorValueOwner(); });

    lua.new_usertype<RE::PlayerCharacter>("PlayerCharacter", sol::no_constructor);

    lua.new_usertype<RE::ActorValueOwner>(
        "ActorValueOwner", sol::no_constructor, "get_actor_value",
        [](RE::ActorValueOwner* ptr, RE::ActorValue k) { return ptr->GetActorValue(k); },
        "set_actor_value",
        [](RE::ActorValueOwner* ptr, RE::ActorValue k, float v) {
          return ptr->SetActorValue(k, v);
        });
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