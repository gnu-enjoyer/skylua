#include "cfg.hpp"
#include "ecs.hpp"
#include "fs.hpp"
#include "mimalloc-new-delete.h"
#include "mimalloc-override.h"
#include "misc.hpp"
#include "sink.hpp"

auto BootstrapLua() {
  auto dir = fs::FindOrCreateDirectory<fs::Destination::RUNTIME>();
  auto lua_files = fs::LuaFile::GetLuaFilesOnDisk(dir, cfg::LoadOrDefault()->ignored_filenames);

  for (auto& i : lua_files) {
    ecs::AddLuaFileToRegistry(i);
  }

  ecs::EnableLatentTick();
}

auto SetupSinks() {
  auto sink = sink::LuaSink::GetSingleton();
  RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESHitEvent>(sink);
  RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESActivateEvent>(sink);
  RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(sink);
  SKSE::GetCrosshairRefEventSource()->AddEventSink(sink);
  SKSE::GetActionEventSource()->AddEventSink(sink);
}

static void CallBack(fs::HotReloadCallback cb) {
  // tbd synchronise instead of copying over entire set
  for (auto& i : cb) {
    ecs::RebuildEntityForPath(i);
  }
};

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
  SKSE::Init(skse);
  misc::EnableDefaultLogger();
  SetupSinks();

  SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
      BootstrapLua();
      cfg::LoadOrDefault()->enable_hot_reload
          ? fs::HotReloader<fs::RetryMethod::LOOP>(cfg::LoadOrDefault()->ignored_filenames)
                .DetachWithCallback(CallBack)
          : fs::HotReloader<fs::RetryMethod::NONE>(cfg::LoadOrDefault()->ignored_filenames)
                .await(CallBack);
    }
  });

  return true;
}
