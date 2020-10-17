# Copyright 2019 Želimir Marojević
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt
# or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# For more information, see http://www.boost.org

cmake_minimum_required(VERSION 3.5)

project (xodr2bgl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#enable_testing()

#set(Boost_NO_BOOST_CMAKE ON)
#set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_VERBOSE ON)
set(Boost_PATH /home/zeli/local/opt/boost-1.74.0)
find_package(Boost 1.72 REQUIRED COMPONENTS graph filesystem PATHS ${Boost_PATH})

include_directories( ${Boost_INCLUDE_DIRS} )

add_subdirectory(src)
