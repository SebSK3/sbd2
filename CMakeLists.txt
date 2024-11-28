cmake_minimum_required(VERSION 3.30.0)
project(sbd1 VERSION 0.1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG  "-g -DDEBUG")

file(GLOB source_files CONFIGURE_DEPENDS "src/*.hpp" "src/*.cpp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

add_executable(sbd1 ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
