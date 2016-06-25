configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

cmake_minimum_required(VERSION 2.8)

SET(libMR_SRCS weibull.c MetaRecognition.cpp)
IF(WIN32)               
  SET(libMR_SRCS ${libMR_SRCS} weibull.h MetaRecognition.h)
ENDIF(WIN32)

add_library(MR SHARED  ${libMR_SRCS} config.h.in)

set_target_properties(MR PROPERTIES
  PUBLIC_HEADER "MetaRecognition.h;${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS MR
  # IMPORTANT: Add the libmr library to the "export-set"
  EXPORT libMRLibraryDepends
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
    COMPONENT dev)
