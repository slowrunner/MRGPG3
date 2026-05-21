cmake_minimum_required(VERSION 3.16)
project(robot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Locate the already-built gopigo3_cpp library ──────────────────────────────
# GOPIGO3_INSTALL_DIR is set by the build script to  <project_root>/build
find_library(GOPIGO3_LIB
    NAMES gopigo3_cpp
    PATHS "${GOPIGO3_INSTALL_DIR}/lib"
    NO_DEFAULT_PATH
    REQUIRED
)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(myrobot src/myrobot.cpp)

target_include_directories(myrobot SYSTEM PRIVATE "${GOPIGO3_INSTALL_DIR}/include")

target_link_libraries(myrobot PRIVATE "${GOPIGO3_LIB}")

target_compile_options(myrobot PRIVATE -Wall -Wextra -Wpedantic)

# ── Install ───────────────────────────────────────────────────────────────────
install(TARGETS myrobot
    RUNTIME DESTINATION bin
)

