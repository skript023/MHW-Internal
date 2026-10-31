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
include(cmake/ellohim_hook.cmake)
include(cmake/json.cmake)
include(cmake/g3log.cmake)

message("\nFetching custom modules")
include(cmake/imgui.cmake)
include(cmake/asio.cmake)
include(cmake/websocket.cmake)
include(cmake/quickjs.cmake)
include(cmake/stbi.cmake)

set(ASTRA_SOURCE_DIR "L:/Coding/Experiment/astra" CACHE PATH "Local Astra source directory")
set(ASTRA_BACKENDS "" CACHE STRING "Use the host's existing ImGui backends")
add_subdirectory("${ASTRA_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/astra")
target_include_directories(astra PUBLIC "${imgui_SOURCE_DIR}")

# MHW
message(STATUS "MHW")
file(GLOB_RECURSE SRC_MAIN CONFIGURE_DEPENDS
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if (MSVC)
  add_compile_options(/bigobj)
endif()

add_library(MHW MODULE "${SRC_MAIN}")

target_sources(MHW PRIVATE
    "${ASTRA_SOURCE_DIR}/host/canvas.cpp"
    "${ASTRA_SOURCE_DIR}/host/input_service.cpp"
    "${ASTRA_SOURCE_DIR}/host/notification.cpp"
    "${ASTRA_SOURCE_DIR}/host/settings.cpp"
    "${ASTRA_SOURCE_DIR}/host/hotkeys.cpp"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET MHW PROPERTY CXX_STANDARD 23)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

# Layer contract: app -> core | game -> core. Included roots mirror the layout so
# headers can be included as "core/..." / "game/...".
target_include_directories(MHW PRIVATE 
    "${SRC_DIR}"
    "${SRC_DIR}/core"
    "${SRC_DIR}/core/ui"
    "${SRC_DIR}/core/config"
    "${SRC_DIR}/core/threading"
    "${SRC_DIR}/core/compat"
    "${SRC_DIR}/game"
    "${ellohim_hook_SOURCE_DIR}/src/hooking/core/hde"
    "${ellohim_hook_SOURCE_DIR}/include"
    "${json_SOURCE_DIR}/single_include"
    "${imgui_SOURCE_DIR}"
    "${websocket_SOURCE_DIR}"
    "${asio_SOURCE_DIR}/asio/include"
    "${ASTRA_SOURCE_DIR}/include"
    "${stbi_SOURCE_DIR}"
)

target_precompile_headers(MHW PRIVATE "${SRC_DIR}/core/common.hpp")
target_link_libraries(MHW PRIVATE quickjs EllohimHook imgui astra::ui astra::banners AsyncLogger dbghelp websocket winhttp iphlpapi d3d11 d3d12 dxgi ${ZLIB_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto Crypt32)

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
