cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob3 /DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

project(skylua VERSION 0.1.0 LANGUAGES CXX)
include(GNUInstallDirs)

find_package(mimalloc CONFIG REQUIRED)
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LuaJIT REQUIRED IMPORTED_TARGET luajit)
find_path(SOL2_INCLUDE_DIRS "sol/abort.hpp")

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES src/plugin.cpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOL2_INCLUDE_DIRS} vendor)
target_link_libraries(${PROJECT_NAME} PRIVATE mimalloc-static PkgConfig::LuaJIT)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
