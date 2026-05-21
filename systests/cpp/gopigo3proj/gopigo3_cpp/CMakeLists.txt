cmake_minimum_required(VERSION 3.16)
project(gopigo3_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Sources ────────────────────────────────────────────────────────────────────
add_library(gopigo3_cpp STATIC
    src/GoPiGo3.cpp
    src/EasyGoPiGo3.cpp
)

target_include_directories(gopigo3_cpp
    SYSTEM PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(gopigo3_cpp PRIVATE -Wall -Wextra -Wpedantic)

# ── Install rules (relative to CMAKE_INSTALL_PREFIX) ──────────────────────────
install(TARGETS gopigo3_cpp
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
    DESTINATION include
)

