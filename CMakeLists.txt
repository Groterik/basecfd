cmake_minimum_required(VERSION 2.8)

#Project name
set(PROJECT basecfd)

enable_testing()

project(${PROJECT})
set(${PROJECT}_VERSION "0.1")
string(TOUPPER ${PROJECT} UPROJECT)

# Options
option(${UPROJECT}_AS_SHARED "Build ${PROJECT} as shared library" ON)
option(${UPROJECT}_WITH_UNIT_TESTS "Build ${PROJECT} with unit tests" ON)
option(${UPROJECT}_WITH_PLUGINS "Build ${PROJECT} with plugins" ON)

set(CXX11_FLAG "-std=c++11")
if(CMAKE_COMPILER_IS_GNUCC)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
      set(CXX11_FLAG "-std=c++0x")
    endif()
endif()

list(APPEND CMAKE_CXX_FLAGS ${CXX11_FLAG})

if(${UPROJECT}_AS_SHARED)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()

if(${UPROJECT}_WITH_UNIT_TESTS)
  add_definitions(-DBASECFD_WITH_UNIT_TESTS)
endif()

# Dependencies

# Project files
set(SOURCES
        src/adjoiner.cpp
        src/basecfd.cpp
        src/format.cpp
        src/grid.cpp
        src/partitioner.cpp
        src/plugin.cpp
        src/unstructured_grid.cpp
    )

set(PUBLIC_HEADERS
        include/basecfd/adjacency_list.hpp
        include/basecfd/adjoiner.hpp
        include/basecfd/algorithm.hpp
        include/basecfd/cell.hpp
        include/basecfd/cell_iterator.hpp
        include/basecfd/definitions.hpp
        include/basecfd/element.hpp
        include/basecfd/filtered_iterator.hpp
        include/basecfd/format.hpp
        include/basecfd/grid.hpp
        include/basecfd/grid_info.hpp
        include/basecfd/partitioner.hpp
        include/basecfd/plugin.hpp
        include/basecfd/range.hpp
        include/basecfd/unstructured_grid.hpp
        include/basecfd/vector.hpp
    )

set(PRIVATE_HEADERS
        thirdparty/dynfload/dynfload.hpp
    )

set(OTHER_FILES
        .astylerc
   )

# Build settings
include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT} ${LIB_TYPE} ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${OTHER_FILES})
set_property(TARGET ${PROJECT} PROPERTY PUBLIC_HEADER ${PUBLIC_HEADERS})
set_property(TARGET ${PROJECT} PROPERTY VERSION ${${PROJECT}_VERSION})

# Dependencies
target_link_libraries(${PROJECT} dl)


# Installation
set(INSTALL_INCLUDE_DIR include)
set(INSTALL_LIBRARY_DIR lib)
set(INSTALL_BINARY_DIR bin)

install(TARGETS ${PROJECT}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}/${PROJECT})

# CMake package config
include(CMakePackageConfigHelpers)
set(PACKAGE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}-config.cmake")
set(PACKAGE_INSTALL_DIR share/${PROJECT}/cmake)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/package.cmake.in ${PACKAGE_OUTPUT}
                              INSTALL_DESTINATION ${PACKAGE_INSTALL_DIR}
                              PATH_VARS INSTALL_INCLUDE_DIR INSTALL_LIBRARY_DIR INSTALL_BINARY_DIR
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)


install(FILES ${PACKAGE_OUTPUT}
        DESTINATION ${PACKAGE_INSTALL_DIR})

if(${UPROJECT}_WITH_PLUGINS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)
endif()

if(${UPROJECT}_WITH_UNIT_TESTS)
  add_executable(${PROJECT}_units test/units/tests_runner.cpp)
  target_link_libraries(${PROJECT}_units ${PROJECT})
  add_test(unit-tests ${PROJECT}_units)
endif()
