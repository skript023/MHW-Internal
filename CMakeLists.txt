cmake_minimum_required(VERSION 3.20)

project(MHW CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Fetch modules
message("\nFetching modules")
include(scripts/minhook.cmake)
include(scripts/json.cmake)
include(scripts/g3log.cmake)

message("\nFetching custom modules")
include(scripts/imgui.cmake)


# MHW
message(STATUS "MHW")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

#if (MSVC)
 # add_compile_options(/bigobj)
#else ()
#  add_compile_options(-Wa,-mbig-obj)
#endif ()

add_library(MHW MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET MHW PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(MHW PRIVATE 
    "${SRC_DIR}"
    "${json_SOURCE_DIR}/single_include"
    "${imgui_SOURCE_DIR}"
)

target_precompile_headers(MHW PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(MHW PRIVATE minhook imgui g3log)

# Warnings as errors
set_property(TARGET MHW PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(MHW 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

add_subdirectory(injector)