include(FetchContent)

set(ELLOHIM_BUILD_EXAMPLES OFF CACHE BOOL "Build Ellohim-Hook examples" FORCE)

FetchContent_Declare(
    ellohim_hook
    URL https://github.com/skript023/Ellohim-Hook/archive/c8a1b0aaa073ea6466f5e4d3235b94b382537b79.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
message("Ellohim-Hook")
FetchContent_MakeAvailable(ellohim_hook)

# Apply patch for protected/access-denied threads (e.g. MHW DRM/watchdog threads)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/patches/thread_freezer.hpp")
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/patches/thread_freezer.hpp"
         DESTINATION "${ellohim_hook_SOURCE_DIR}/src/hooking/core")
endif()
