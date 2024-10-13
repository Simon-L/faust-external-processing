cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

set(FAUST_VERSION 2.75.7)

project(
    faust-base-cpp
    VERSION 0.1.0
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)


# ---- Faust

include(FetchContent)
FetchContent_Declare(
  faust
  URL  https://github.com/grame-cncm/faust/archive/refs/tags/${FAUST_VERSION}.tar.gz
)
FetchContent_MakeAvailable(faust)

# ---- Declare library ----

add_library(
    faust-base-cpp_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    faust-base-cpp_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
    "\$<BUILD_INTERFACE:${faust_SOURCE_DIR}/architecture>"
)

target_compile_features(faust-base-cpp_lib PUBLIC cxx_std_17)
target_link_libraries(faust-base-cpp_lib PRIVATE sndfile)

# ---- Declare faust-base-cpp ----

add_executable(faust-base-cpp_exe source/main.cpp)
add_executable(faust-base-cpp::exe ALIAS faust-base-cpp_exe)

set_property(TARGET faust-base-cpp_exe PROPERTY OUTPUT_NAME faust-base-cpp)

target_compile_features(faust-base-cpp_exe PRIVATE cxx_std_17)

target_link_libraries(faust-base-cpp_exe PRIVATE faust-base-cpp_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

