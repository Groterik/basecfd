cmake_minimum_required(VERSION 2.8)

#Project name
set(PROJECT basecfd_plugin_vtk_legacy)

enable_testing()

project(${PROJECT})
set(${PROJECT}_VERSION "0.1")
string(TOUPPER ${PROJECT} UPROJECT)

# Options

if(${UPROJECT}_AS_SHARED)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()

if(${UPROJECT}_WITH_UNIT_TESTS)
  add_definitions(-DBASECFD_WITH_UNIT_TESTS)
endif()

# Dependencies
find_package(Metis REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/)

# Project files
set(SOURCES
        vtk_legacy.cpp
    )

set(PUBLIC_HEADERS
    )

set(PRIVATE_HEADERS
    )

set(OTHER_FILES
   )

# Build settings
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT} SHARED ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${OTHER_FILES})
set_property(TARGET ${PROJECT} PROPERTY PUBLIC_HEADER ${PUBLIC_HEADERS})
set_property(TARGET ${PROJECT} PROPERTY VERSION ${${PROJECT}_VERSION})

target_link_libraries(${PROJECT} basecfd)


# Installation
set(INSTALL_INCLUDE_DIR include)
set(INSTALL_LIBRARY_DIR lib)
set(INSTALL_BINARY_DIR bin)

install(TARGETS ${PROJECT}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}/${PROJECT})

