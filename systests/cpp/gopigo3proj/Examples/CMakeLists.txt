cmake_minimum_required(VERSION 3.16)
project(Examples VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -- Locate the already-built gopigo3_cpp library ------------------------------
find_library(GOPIGO3_LIB
    NAMES gopigo3_cpp
    PATHS "${GOPIGO3_INSTALL_DIR}/lib"
    NO_DEFAULT_PATH
    REQUIRED
)

# -- One executable per .cpp file in src/ --------------------------------------
file(GLOB EXAMPLE_SOURCES "src/*.cpp")

foreach(SOURCE ${EXAMPLE_SOURCES})
    # Derive executable name from filename, e.g. src/drive.cpp -> drive
    get_filename_component(EXE_NAME ${SOURCE} NAME_WE)

    add_executable(${EXE_NAME} ${SOURCE})

    target_include_directories(${EXE_NAME} SYSTEM PRIVATE "${GOPIGO3_INSTALL_DIR}/include")

    target_link_libraries(${EXE_NAME} PRIVATE "${GOPIGO3_LIB}")

    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -Wpedantic)

    install(TARGETS ${EXE_NAME}
        RUNTIME DESTINATION bin
    )
endforeach()
