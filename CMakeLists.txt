cmake_minimum_required(VERSION 3.5)
project (hashTable VERSION 1.0.0 LANGUAGES CXX)

# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall -g" )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}" )

#Include dir
include_directories(include)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(hashTests ../src/hashTests.cpp)
target_link_libraries(hashTests ${GTEST_LIBRARIES} pthread)