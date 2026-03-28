#define NOB_IMPLEMENTATION
#include "thirdparty/nob.h"


#define SRC_DIR "src/"
#define BUILD_DIR "build/"

#define RAYLIB_SRC "thirdparty/raylib-5.5/src/"

#define APP_NAME "Calc"

static bool build_raylib(void) {
    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "make", "-C", RAYLIB_SRC);
    return nob_cmd_run_sync(cmd);
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!build_raylib()) return 1;

    if (!mkdir_if_not_exists(BUILD_DIR)) return 1;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc", SRC_DIR"cal.c", "-o", BUILD_DIR APP_NAME);

#if defined(__linux__)
    nob_cmd_append(&cmd, "-L"RAYLIB_SRC, "-lraylib", "-lm", "-lpthread", "-lGL", "-lrt", "-lX11");
#elif defined(_WIN32)
    nob_cmd_append(&cmd, "-L"RAYLIB_SRC, "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm");
elif defined(__APPLE__)
    nob_cmd_append(&cmd, "-L"RAYLIB_SRC, "-lraylib",
        "-framework", "OpenGL",
        "-framework", "Cocoa",
        "-framework", "IOKit",
        "-framework", "CoreVideo"
    );
#endif

    if (!nob_cmd_run_sync(cmd)) return 1;

    return 0;
}

