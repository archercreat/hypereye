cmake_minimum_required(VERSION 3.15)

project(driver LANGUAGES CXX ASM_MASM)

# Include FindWDK
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindWDK/cmake")
find_package(WDK REQUIRED)

# Disable exceptions
string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS heye/lib/*.cpp heye/lib/*.asm heye/src/*.cpp heye/src/*.hpp)
file(GLOB_RECURSE INCLUDES CONFIGURE_DEPENDS heye/lib/*.hpp)

wdk_add_driver(driver KMDF 1.15 ${SOURCES} ${INCLUDES})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${INCLUDES})

# Enable std23
target_compile_features(driver PRIVATE cxx_std_23)

target_include_directories(driver PRIVATE heye/lib)

target_compile_options(driver PRIVATE
    "/W4"
    "/WX"
    "/wd4005"
    "/wd4201"
    "/Wv:18"
    "/Zc:strictStrings-"
)

target_compile_definitions(driver PRIVATE _VCRUNTIME_DISABLED_WARNINGS)
