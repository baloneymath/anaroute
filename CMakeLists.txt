cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME "anaroute")
project(${PROJECT_NAME})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_INSTALL_PREFIX})
else()
  set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
unset(CMAKE_INSTALL_PREFIX)

# Find LIMBO
if (LIMBO_DIR)
  set(LIMBO_ROOT_DIR ${LIMBO_DIR})
else()
  set(LIMBO_ROOT_DIR $ENV{LIMBO_DIR})
endif()
message(STATUS "Find Limbo: ${LIMBO_ROOT_DIR}")
unset(LIMBO_DIR CACHE)

# Find SparseHash
if (SPARSE_HASH_DIR)
  set(SPARSEHASH_ROOT_DIR ${SPARSE_HASH_DIR})
else()
  set(SPARSEHASH_ROOT_DIR $ENV{SPARSE_HASH_DIR})
endif()
message(STATUS "Find SparseHash: ${SPARSEHASH_ROOT_DIR}")

# Find Boost
find_package(Boost 1.6 REQUIRED)

# Find Zlib
find_package(ZLIB REQUIRED)

#set(CMAKE_CXX_FLAGS "-g -pg -std=c++14 -Wall -fPIC")
set(CMAKE_CXX_FLAGS "-O3 -std=c++14 -Wall -fPIC")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR/include})

add_subdirectory(src)


