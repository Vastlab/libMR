cmake_minimum_required(VERSION 2.8)
project(libMR C CXX)

set(LIBMR_MAJOR_VERSION 0)
set(LIBMR_MINOR_VERSION 1)
set(LIBMR_PATCH_VERSION 0)
set(LIBMR_VERSION
  ${LIBMR_MAJOR_VERSION}.${LIBMR_MINOR_VERSION}.${LIBMR_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/libMR)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find libmr/libmr.h
  "${PROJECT_BINARY_DIR}")  # to find bin/config.h

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -I /usr/include/sys")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


include(UseDoxygen OPTIONAL)

# Add sub-directories
add_subdirectory(libMR)
add_subdirectory(examples)


# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(TARGETS MR 
  FILE "${PROJECT_BINARY_DIR}/libMRLibraryDepends.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE libMR)

# Create a libMRBuildTreeSettings.cmake file for the use from the build tree
configure_file(libMRBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/libMRBuildTreeSettings.cmake" @ONLY)

# Create the libMRConfig.cmake and libMRConfigVersion files
file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
configure_file(libMRConfig.cmake.in
  "${PROJECT_BINARY_DIR}/libMRConfig.cmake" @ONLY)
configure_file(libMRConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/libMRConfigVersion.cmake" @ONLY)

# Install the export set for use with the install-tree
install(EXPORT libMRLibraryDepends DESTINATION
  "${INSTALL_DATA_DIR}/libMR/CMake"
  COMPONENT dev)
