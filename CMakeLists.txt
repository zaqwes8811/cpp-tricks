cmake_minimum_required(VERSION 3.25)

set(project_name cpp-tricks)
project(project_name)

set(generation_version 0)
set(major_version 0)
set(minor_version 3)
set(branch_version 0)
option(BUILD_CLANG_TIDY "Build with clang-tidy" OFF)
option(BUILD_TESTS "Build tests" ON)

option(BUILD_WITH_ASAN "Build with ASan" OFF)
option(BUILD_WITH_TSAN "Build with TSan" OFF)
option(BUILD_WITH_FUZZ "Build with AFL" OFF)

set(CMAKE_VERBOSE_MAKEFILE true)

include_directories(${CMAKE_SOURCE_DIR})

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (BUILD_WITH_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

if (BUILD_WITH_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
endif()

#if (PLATFORM_TYPE STREQUAL "X86_64_TORCH_LEGACY")
#    set(CMAKE_CXX_STANDARD 14)
#else ()
set(CMAKE_CXX_STANDARD 17)
#endif ()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
endif ()

####################################################################################
####################################################################################
####################################################################################

find_package(GTest REQUIRED)

add_subdirectory(projects/backtrace_and_heap)
add_subdirectory(projects/analysers_templs/asan_check_templ)
add_subdirectory(projects/analysers_templs/tsan_check_templ)
add_subdirectory(projects/cpp_lang)
add_subdirectory(projects/high_perf)
add_subdirectory(edu/books)
add_subdirectory(projects/safe)

add_subdirectory(projects/cpp26)

add_subdirectory(edu)
