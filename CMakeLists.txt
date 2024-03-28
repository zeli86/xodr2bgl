# Copyright 2024 Želimir Marojević
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# For more information, see http://www.boost.org

cmake_minimum_required(VERSION 3.24)

project (xodr2bg LANGUAGES CXX VERSION 0.1.0)

set(ENABLE_TESTS ON CACHE BOOL "Enable unit tests")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_FIND_DEBUG_MODE OFF)

set(Boost_NO_SYSTEM_PATHS ON)
find_package(Boost REQUIRED COMPONENTS filesystem graph log log_setup CONFIG)
find_package(pugixml REQUIRED CONFIG)#

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/xodr2bg-config-version.cmake
  COMPATIBILITY AnyNewerVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xodr2bg-config-version.cmake DESTINATION lib)

add_subdirectory(src)
#add_subdirectory(trash)

message(STATUS "ENABLE_TESTS := ${ENABLE_TESTS}")
if(ENABLE_TESTS)
  enable_testing()
  #find_package(FakeIt REQUIRED)
  find_package(Catch2 3 REQUIRED)
  include(CTest)
  include(Catch)
  add_subdirectory(tests)
endif()