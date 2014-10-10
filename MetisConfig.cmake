#  Try to find Metis
#  Once done this will define:
#
#    METIS_FOUND            (BOOL)       Flag indicating ifMETIS was found
#    METIS_INCLUDE_DIR      (PATH)       Path to the METIS include file
#    METIS_LIBRARY          (FILE)       METIS library
#
# Copyright (c) 2013 Klimov Max, <maxy.klimov@gmail.com>
#
# Redistribution and use of this file is allowed according to the terms of the MIT license.
#
# #############################################################################

if( METIS_LIBRARY AND METIS_INCLUDE_DIR )
    # In cache already
  set(METIS_FOUND TRUE)
else( METIS_LIBRARY AND METIS_INCLUDE_DIR )


  if(WIN32)
    set(INCLUDE_SEARCH_PATHS $ENV{PROGRAMFILES}/METIS ${GNUWIN32_DIR} $ENV{METIS_INCLUDE_PATH} $ENV{METIS_INSTALL_DIR} ${METIS_ROOT_DIR})
    set(INCLUDE_SEARCH_SUFFIXES include)
    set(LIBRARY_SEARCH_PATHS $ENV{PROGRAMFILES}/METIS $ENV{PROGRAMFILES}/METIS ${GNUWIN32_DIR} $ENV{METIS_LIB_PATH} $ENV{METIS_INSTALL_DIR} ${METIS_ROOT_DIR})
    set(LIBRARIES_SEARCH_SUFFIXES lib)
  else(WIN32)
    set(INCLUDE_SEARCH_PATHS $ENV{METIS_INCLUDE_PATH}  ${METIS_ROOT_DIR})
    set(INCLUDE_SEARCH_SUFFIXES include)
    set(LIBRARY_SEARCH_PATHS $ENV{METIS_LIBRARY_PATH}  ${METIS_ROOT_DIR})
    set(LIBRARIES_SEARCH_SUFFIXES lib)  
  endif(WIN32)

 set(INCLUDE_NAMES "metis.h")
  if(NOT METIS_INCLUDE_DIR)
    find_path(METIS_INCLUDE_DIR
              NAMES ${INCLUDE_NAMES}
              HINTS ${INCLUDE_SEARCH_PATHS}
              PATH_SUFFIXES ${INCLUDE_SEARCH_SUFFIXES}
              DOC "Path to METIS include files")
  endif(NOT METIS_INCLUDE_DIR)


  set(LIBRARY_NAMES "metis")
  if(NOT METIS_LIBRARY)
    find_library(METIS_LIBRARY
                 NAMES ${LIBRARY_NAMES}
                 PATHS ${LIBRARY_SEARCH_PATHS}
                 PATH_SUFFIXES ${LIBRARIES_SEARCH_SUFFIXES}
                 DOC "Path to METIS library file")
  endif(NOT METIS_LIBRARY)

  if( METIS_LIBRARY AND METIS_INCLUDE_DIR )
      set(METIS_FOUND TRUE)
  else( METIS_LIBRARY AND METIS_INCLUDE_DIR )
      set(METIS_FOUND FALSE)
  endif( METIS_LIBRARY AND METIS_INCLUDE_DIR )    

  if(METIS_FOUND)
    if(NOT METIS_FIND_QUIETLY)
      message(STATUS "Found METIS library: ${METIS_LIBRARY}")
      message(STATUS "Found METIS include: ${METIS_INCLUDE_DIR}")
    endif(NOT METIS_FIND_QUIETLY)
  else(METIS_FOUND)
    if(METIS_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find METIS")
    else(METIS_FIND_REQUIRED)
      message(WARNING "Could NOT find METIS")
    endif(METIS_FIND_REQUIRED)
  endif(METIS_FOUND)
   
endif(METIS_LIBRARY AND METIS_INCLUDE_DIR )
