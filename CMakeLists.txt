cmake_minimum_required(VERSION 3.20)

project(MHW CXX ASM_MASM)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# OpenSSL configuration
set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/openssl")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")   # opsional
set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib")         # opsional
set(ZLIB_LIBRARIES "${OPENSSL_LIBRARIES}/zlib.lib")

find_package(OpenSSL REQUIRED)

# Fetch modules
message("\nFetching modules")
include(scripts/minhook.cmake)
include(scripts/json.cmake)
include(scripts/g3log.cmake)

message("\nFetching custom modules")
include(scripts/imgui.cmake)
include(scripts/asio.cmake)
include(scripts/websocket.cmake)


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

add_library(MHW MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET MHW PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(MHW PRIVATE 
    "${SRC_DIR}"
    "${SRC_DIR}/hooking"
    "${SRC_DIR}/services"
    "${SRC_DIR}/server"
    "${json_SOURCE_DIR}/single_include"
    "${imgui_SOURCE_DIR}"
    "${websocket_SOURCE_DIR}"
    "${asio_SOURCE_DIR}/asio/include"
)

target_precompile_headers(MHW PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(MHW PRIVATE minhook imgui g3log websocket ${ZLIB_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto Crypt32)

# Warnings as errors
set_property(TARGET MHW PROPERTY COMPILE_WARNING_AS_ERROR OFF)

add_compile_definitions(MHW 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

if(MSVC)
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /LTCG /OPT:REF,ICF /GUARD:NO")
    string(REPLACE "/Ob1" "/Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
endif()

add_subdirectory(injector)