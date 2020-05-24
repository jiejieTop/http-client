cmake_minimum_required(VERSION 2.8)

project(http-client)

set(TARGETS "http-client")
set(SUBDIRS "test" "platform" "network" "common" "libhttp" "httpclient")
set(INCDIRS  "test" "platform/linux" "common" "common/log" "network/mbedtls/include" "network" "libhttp" "httpclient")
set(OUTDIRS "build")
set(LIBNAMES "platform" "network" "common" "salof" "mbedtls" "libhttp" "httpclient")
set(PROJECT_ROOT_PATH "${PROJECT_SOURCE_DIR}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_ROOT_PATH}/${OUTDIRS}/lib/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_ROOT_PATH}/${OUTDIRS}/bin/")

set(CMAKE_BUILD_TYPE "Debug")   # Debug Release
set(CMAKE_LIB_TYPE "SHARED")   # SHARED STATIC

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++" )

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS "-lpthread")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11")
    set(CMAKE_CXX_FLAGS "-lpthread")
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3") 
endif(CMAKE_COMPILER_IS_GNUCXX)

foreach(incdir ${INCDIRS})
    include_directories(${incdir})
endforeach()

foreach(subdir ${SUBDIRS})
    add_subdirectory(${PROJECT_ROOT_PATH}/${subdir})
endforeach()

link_directories(${LIBRARY_OUTPUT_PATH})

