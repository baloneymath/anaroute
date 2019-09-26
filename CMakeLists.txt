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
#if (BOOST_DIR)
  #set(BOOST_ROOT ${BOOST_DIR})
  #set(BOOST_INCLUDEDIR ${BOOST_DIR}/boost)
  #set(BOOST_LIBRARYDIR ${BOOST_DIR}/libs)
#else()
  #set(BOOST_ROOT $ENV{BOOST_DIR})
  #set(BOOST_INCLUDEDIR $ENV{BOOST_DIR}/boost)
  #set(BOOST_LIBRARYDIR $ENV{BOOST_DIR}/libs)
#endif()

find_package(Boost 1.6 REQUIRED COMPONENTS system graph iostreams)

# Find Zlib
find_package(ZLIB REQUIRED)

#set(CMAKE_CXX_FLAGS "-g -pg -std=c++14 -Wall -fPIC")
set(CMAKE_CXX_FLAGS "-O3 -std=c++14 -Wall -fPIC")


##################################################
#        Source Code                             #
##################################################
add_executable(${PROJECT_NAME}
  src/main/main.cpp
  src/main/anaroute.cpp
  src/util/util.cpp
  src/db/lef/layer/lefCutLayer.cpp
  src/db/lef/layer/lefRoutingLayer.cpp
  src/db/lef/lefSite.cpp
  src/db/lef/lefVia.cpp
  src/db/dbLef.cpp
  src/db/dbTechfile.cpp
  src/db/dbPin.cpp
  src/db/dbCir.cpp
  src/db/net/dr/drNet.cpp
  src/parser/lefreader.cpp
  src/parser/techfileReader.cpp
  src/parser/ispd08reader.cpp
  src/parser/gdsReader.cpp
)

##################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${ZLIB_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${SPARSEHASH_ROOT_DIR}/include
  ${LIMBO_ROOT_DIR}/include
)

file(GLOB LIMBO_LIB 
  #lefdef
  ${LIMBO_ROOT_DIR}/lib/libcdef.a 
  ${LIMBO_ROOT_DIR}/lib/libcdefzlib.a 
  ${LIMBO_ROOT_DIR}/lib/libdef.a 
  ${LIMBO_ROOT_DIR}/lib/libdefzlib.a 
	${LIMBO_ROOT_DIR}/lib/libdefparser.a
	${LIMBO_ROOT_DIR}/lib/libdefparseradapt.a
  ${LIMBO_ROOT_DIR}/lib/libclef.a 
  ${LIMBO_ROOT_DIR}/lib/libclefzlib.a 
  ${LIMBO_ROOT_DIR}/lib/liblef.a 
  ${LIMBO_ROOT_DIR}/lib/liblefzlib.a
	${LIMBO_ROOT_DIR}/lib/liblefparseradapt.a
  #limbo gdsparser
  ${LIMBO_ROOT_DIR}/lib/libgdsdb.a 
  ${LIMBO_ROOT_DIR}/lib/libgdsparser.a
)

target_link_libraries(${PROJECT_NAME} ${LIMBO_LIB} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})

#Install
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX_DIR}/bin)

