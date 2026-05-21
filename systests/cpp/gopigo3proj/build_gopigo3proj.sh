#!/usr/bin/env bash
# build_gopigo3proj.sh
# Run from the gopigo3proj/ directory.
# Builds the GoPiGo3 library, 
#        then builds the myrobot and GoPiGo3 Example executables.
# Final layout:
#   build/
#     lib/   – libgopigo3_cpp.a  (+ installed headers under build/include/)
#     bin/   – myrobot and Example executables
#     build/ – all CMake-generated temp/cache files

# NOTE:  This utility was originally written by Claude Code per my prompting

# Prevent sourcing – running in the current shell means a build error kills the session
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    : # running normally, continue
else
    echo "ERROR: do not source this script. " 
    echo "Run it as: bash build_gopigo3proj.sh or ./build_gopigo3proj.sh" >&2
    return 1
fi

# -- Check for cmake, install if missing ---------------------------------------
if ! command -v cmake &> /dev/null; then
    echo "cmake not found, installing..."
    sudo apt-get update -qq
    sudo apt-get install -y cmake
else
    echo "cmake found: $(cmake --version | head -1)"
fi

set -euo pipefail

# ── Paths ─────────────────────────────────────────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_ROOT="${SCRIPT_DIR}/build"
BUILD_TEMP="${BUILD_ROOT}/build"   # CMake binary dir (temp files live here)
INSTALL_DIR="${BUILD_ROOT}"        # cmake --install prefix  (lib/ bin/ go here)

echo "========================================================"
echo "  GoPiGo3 Project Builder"
echo "  Project root : ${SCRIPT_DIR}"
echo "  Install root : ${INSTALL_DIR}"
echo "  Temp/cache   : ${BUILD_TEMP}"
echo "========================================================"

# --- Clean previous temp/cache files
echo "Cleaning previous build temp files..."
rm -rf "${BUILD_TEMP}"
mkdir -p "${BUILD_TEMP}"


# ── Step 1: build_gopigo3cpp ──────────────────────────────────────────────────
echo ""
echo "[ Step 1/3 ]  build_gopigo3cpp  –  building the GoPiGo3 library"
echo "----------------------------------------------------------------"

GOPIGO3_TEMP="${BUILD_TEMP}/gopigo3_cpp"
mkdir -p "${GOPIGO3_TEMP}"

cmake -S "${SCRIPT_DIR}/gopigo3_cpp" \
      -B "${GOPIGO3_TEMP}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"

cmake --build  "${GOPIGO3_TEMP}" --parallel
cmake --install "${GOPIGO3_TEMP}"

echo "  ✓  libgopigo3_cpp.a installed to ${INSTALL_DIR}/lib/"

# ── Step 2: build_robot ───────────────────────────────────────────────────────
echo ""
echo "[ Step 2/3 ]  build_robot  –  building the robot executable"
echo "----------------------------------------------------------------"

ROBOT_TEMP="${BUILD_TEMP}/robot"
mkdir -p "${ROBOT_TEMP}"

cmake -S "${SCRIPT_DIR}/robot" \
      -B "${ROBOT_TEMP}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
      -DGOPIGO3_INSTALL_DIR="${INSTALL_DIR}"

cmake --build  "${ROBOT_TEMP}" --parallel
cmake --install "${ROBOT_TEMP}"

echo "  ✓  myrobot installed to ${INSTALL_DIR}/bin/"

# ── Step 3: build_Examples  ───────────────────────────────────────────────────────
echo ""
echo "[ Step 3/3 ]  build_Examples  –  building the GoPiGo3 Example executables"
echo "----------------------------------------------------------------"

EXAMPLES_TEMP="${BUILD_TEMP}/Examples"
mkdir -p "${EXAMPLES_TEMP}"

cmake -S "${SCRIPT_DIR}/Examples" \
      -B "${EXAMPLES_TEMP}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
      -DGOPIGO3_INSTALL_DIR="${INSTALL_DIR}"

cmake --build  "${EXAMPLES_TEMP}" --parallel
cmake --install "${EXAMPLES_TEMP}"

echo "  ✓  GoPiGo3 Examples installed to ${INSTALL_DIR}/bin/"

# ── Summary ───────────────────────────────────────────────────────────────────
echo ""
echo "========================================================"
echo "  Build complete!"
echo ""
echo "  build/"
echo "    lib/    libgopigo3_cpp.a"
echo "    bin/    myrobot Example executables"
echo "    build/  (CMake temp files -  safe to delete)"
echo "========================================================"


