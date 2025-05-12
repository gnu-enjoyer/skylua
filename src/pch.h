#pragma once

#define SOL_NO_EXCEPTIONS 1
#define SOL_LUAJIT 1
#define SOL_USING_CXX_LUAJIT 1
#define SOL_LUA_BIT32_LIB 1

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>
#include <ranges>
#include <sol/sol.hpp>
#include <string>
#include <string_view>
#include <unordered_set>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "glaze/glaze.hpp"
#include "magic_enum/magic_enum.hpp"
#include "magic_enum/magic_enum_utility.hpp"

using namespace std::literals;
