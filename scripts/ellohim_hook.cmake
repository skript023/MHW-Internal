include(FetchContent)

set(ELLOHIM_BUILD_EXAMPLES OFF CACHE BOOL "Build Ellohim-Hook examples" FORCE)

FetchContent_Declare(
    ellohim_hook
    URL https://github.com/skript023/Ellohim-Hook/archive/b691998e6876edfd9e14fadca391f9adbc28f4c3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
message("Ellohim-Hook")
FetchContent_MakeAvailable(ellohim_hook)
