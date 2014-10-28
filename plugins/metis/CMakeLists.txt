cmake_minimum_required(VERSION 2.8)

enable_testing()

find_package(Metis REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${METIS_INCLUDE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/adjoiner)

