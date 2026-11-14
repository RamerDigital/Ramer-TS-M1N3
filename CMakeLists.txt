cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# Ramer-TS-M1N3 — Unified CMake Root Configuration
# Ramer Digital (2026)
# ==============================================================================

# Versioning
set(PROJECT_VERSION_STRING "1.0.0")
set(PROJECT_VERSION_CLEAN "1.0.0")

if(NOT APPLE)
    message(FATAL_ERROR "Ramer-TS-M1N3 targets macOS (Apple Silicon arm64) exclusively.")
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS deployment target")
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Target architecture on macOS")

if(NOT CMAKE_OSX_SYSROOT)
    execute_process(COMMAND xcrun --sdk macosx --show-sdk-path
                    OUTPUT_VARIABLE CMAKE_OSX_SYSROOT_DETECTED
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT_DETECTED}" CACHE PATH "macOS SDK path" FORCE)
endif()

set(CMAKE_XCODE_GENERATE_SCHEME ON)
set(CMAKE_XCODE_SCHEME_LAUNCH_CONFIGURATION "Release")

project(ramer-ts-m1n3 VERSION ${PROJECT_VERSION_CLEAN})

set(CMAKE_CXX_STANDARD 20)

# ==============================================================================
# Third-Party Dependencies (JUCE, RTNeural, AudioDSPTools, JSON)
# ==============================================================================
add_subdirectory(third-party/Ramer-JUCE)

include(third-party/cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

set(RTNEURAL_XSIMD ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(third-party/Ramer-RTNeural)

file(GLOB_RECURSE AUDIO_DSP_TOOLS_SOURCES
    "third-party/AudioDSPTools/dsp/*.cpp"
    "third-party/AudioDSPTools/dsp/*.h"
)
add_library(audio_dsp_tools STATIC ${AUDIO_DSP_TOOLS_SOURCES})
target_include_directories(audio_dsp_tools PUBLIC
    "third-party/AudioDSPTools"
    "third-party/AudioDSPTools/dsp"
    "third-party/Ramer-RTNeural/third-party/Eigen"
)
target_compile_features(audio_dsp_tools PUBLIC cxx_std_20)
target_compile_options(audio_dsp_tools PRIVATE -O3 -mcpu=apple-m1 -flto=thin -w)
target_link_options(audio_dsp_tools PUBLIC -flto=thin)

include(third-party/cmake/WarningFlags.cmake)
add_subdirectory(third-party/Ramer-RTNeural/third-party/json)

# ==============================================================================
# Binary Data Assets
# ==============================================================================
juce_add_binary_data(BinaryData SOURCES
    resources/footswitch_down.png
    resources/footswitch_up.png
    resources/knob2.png
    resources/led_red_off.png
    resources/led_red_on.png
    resources/logo.png
    resources/ts_background_black.jpg
    models/model_ts9_48k_cond2.msgpack
)
set_target_properties(BinaryData PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# ==============================================================================
# Shared JUCE Plugin Modules Library
# ==============================================================================
add_library(juce_plugin_modules STATIC)

target_link_libraries(juce_plugin_modules
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_dsp
        nlohmann_json::nlohmann_json
        RTNeural
        audio_dsp_tools
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        warning_flags
)

target_compile_definitions(juce_plugin_modules
    PUBLIC
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JucePlugin_Manufacturer="Ramer Digital"
        JucePlugin_VersionString="${CMAKE_PROJECT_VERSION}"
        JucePlugin_Name="Ramer-TS-M1N3"
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,COMPILE_DEFINITIONS>
)

target_include_directories(juce_plugin_modules SYSTEM
    PUBLIC
        third-party/Ramer-RTNeural
        third-party/Ramer-RTNeural/third-party/xsimd/include
        third-party/Ramer-RTNeural/third-party/Eigen
        third-party/AudioDSPTools
        third-party/AudioDSPTools/dsp
        Source
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

# ==============================================================================
# macOS Desktop Targets (AU, VST3 & Standalone)
# ==============================================================================
include(CMakeLists-mac.txt)
