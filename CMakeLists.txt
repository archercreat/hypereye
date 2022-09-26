cmake_minimum_required(VERSION 3.15)

project(driver LANGUAGES CXX ASM_MASM)

# Include FindWDK
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindWDK/cmake")
find_package(WDK REQUIRED)

# Disable exceptions
string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.asm)
file(GLOB_RECURSE INCLUDES CONFIGURE_DEPENDS include/*.hpp)

wdk_add_driver(driver ${SOURCES} ${INCLUDES})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${INCLUDES})

# Enable std23
target_compile_features(driver PRIVATE cxx_std_23)

target_include_directories(driver PRIVATE src)

target_compile_options(driver PRIVATE
    "/W4"
    "/WX"
    "/wd4005"
    "/wd4201"
    "/Wv:18"
)

target_compile_definitions(driver PRIVATE _VCRUNTIME_DISABLED_WARNINGS)
