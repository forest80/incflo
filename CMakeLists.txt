cmake_minimum_required(VERSION 3.10)

# Set the search path for cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/tools/CMake)

# Location of CMake-configured AMReX 
set(AMREX_INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/../amrex_install_dir)

project(incflo LANGUAGES CXX Fortran)

# Include problem-specific options
include("${CMAKE_BINARY_DIR}/problem.cmake")

if(DEBUG)
    set(INCFLO_BUILD_TYPE "DEBUG")
else()
    set(INCFLO_BUILD_TYPE "RELEASE")
endif()

include(incflo_utils)
set_default_config_flags()

# Set some cache variable for the whole project
set (INCFLO_LIBNAME "incflocore" CACHE INTERNAL "Name of INCFLO core library")
set (INCFLO_EXENAME "incflo"     CACHE INTERNAL "Name of INCFLO executable")

# Import AMReX. Do it here so it is visibile to all targets
find_package (AMReX REQUIRED HINTS ${AMREX_INSTALL_DIR})

if ( NOT (AMREX_ENABLE_AMRDATA AND AMREX_ENABLE_EB) )
   message ( FATAL_ERROR "AMReX installation missing required components!")
endif ()

if ( NOT ( "${INCFLO_BUILD_TYPE}" STREQUAL "${AMREX_BUILD_TYPE}" ) )
   message (WARNING "INCFLO build type (${INCFLO_BUILD_TYPE}) type does not match AMReX build type (${AMREX_BUILD_TYPE})")
endif ()

# This is the main file
set(MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Core incflo library
add_subdirectory(src)

# We should check what this line does
file (GLOB USR_OVERRIDES ${CMAKE_CURRENT_BINARY_DIR}/*.f90)
add_executable (${INCFLO_EXENAME} ${MAIN} ${USR_OVERRIDES} )
target_link_libraries ( ${INCFLO_EXENAME} ${INCFLO_LIBNAME} )

# Copy compile_commands.json from the build directory to the project root directory
if ( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
endif ()
