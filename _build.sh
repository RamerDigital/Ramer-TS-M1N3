#!/bin/bash

# ==============================================================================
# Ramer-TS-M1N3 — macOS Build Script (AU, VST3 & Standalone)
# Ramer Digital (2026)
# ==============================================================================

set -e

# Ensure relative paths resolve relative to the script directory
cd "$(dirname "$0")"

BUILD_CONFIG="Release"

show_help() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -h, --help        Show this help message"
    echo "  --debug           Build in Debug configuration"
    echo "  --release         Build in Release configuration (default)"
    exit 0
}

for arg in "$@"; do
    case $arg in
        -h|--help)
            show_help
            ;;
        --debug)
            BUILD_CONFIG="Debug"
            ;;
        --release)
            BUILD_CONFIG="Release"
            ;;
        *)
            echo "Unknown option: $arg"
            show_help
            ;;
    esac
done

echo "Cleaning old build artifacts..."
xcodebuild -shutdown >/dev/null 2>&1 || true

if [ -d .build-mac ]; then
    OLD_BUILD=".build-mac_old_$(date +%s)"
    mv .build-mac "$OLD_BUILD"
    rm -Rf "$OLD_BUILD" || true
fi
rm -Rf .build-mac_old_* || true
rm -Rf /tmp/TS-M1N3-build-mac || true

echo "Configuring macOS project (Apple Silicon arm64, Config: $BUILD_CONFIG)..."
cmake -B.build-mac -GXcode \
    -D"CMAKE_OSX_ARCHITECTURES=arm64" \
    -D"CMAKE_OSX_DEPLOYMENT_TARGET=12.0" \
    -DCMAKE_XCODE_ATTRIBUTE_OBJROOT=/tmp/TS-M1N3-build-mac/obj \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="-" \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE="Automatic"

echo "=========================================================="
echo "Xcode project generation completed successfully!"
echo "Xcode project: .build-mac/ramer-ts-m1n3.xcodeproj"
echo "=========================================================="

echo "Building the macOS product configuration: $BUILD_CONFIG..."
cmake --build .build-mac --config "$BUILD_CONFIG"

echo "=========================================================="
echo "macOS Build completed successfully!"
echo "Artifacts generated in .build-mac/ramer-ts-m1n3_artefacts/$BUILD_CONFIG/"
echo "=========================================================="
