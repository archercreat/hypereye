cmake_minimum_required(VERSION 3.15)

project(hypereye LANGUAGES CXX ASM_MASM)

# Include FindWDK
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindWDK/cmake")
find_package(WDK REQUIRED)

file(GLOB_RECURSE HEYE_SOURCES  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm")
file(GLOB_RECURSE HEYE_INCLUDES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

wdk_add_library(hypereye KMDF 1.15 ${HEYE_SOURCES} ${HEYE_INCLUDES})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${HEYE_SOURCES} ${HEYE_INCLUDES})

# Enable std23
target_compile_features(hypereye PRIVATE
    "cxx_std_23"
)

target_include_directories(hypereye PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

target_compile_options(hypereye PRIVATE
    "/GR-"      # Disable RTTI
    "/EHs-c-"   # Disable exceptions
    "/W4"
    "/WX"
    "/wd4005"
    "/wd4201"
    "/Wv:18"
    "/Zc:strictStrings-"
)

target_compile_definitions(hypereye PRIVATE
    "_VCRUNTIME_DISABLED_WARNINGS"
    "-D_HAS_EXCEPTIONS=0"
)
