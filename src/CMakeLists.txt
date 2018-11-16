#
# Define a macro to add sources.
# This will also accumulate include directories if
# a header file is spotted
# WARNING: this automagically prepends the correct
# path (relative to the file invoking the macro)
# to the source file.
#

set ( INCFLO_INCLUDE_PATHS  ${PROJECT_SOURCE_DIR}/src/include )

macro ( add_sources )
  foreach ( item IN ITEMS ${ARGN} )
    set ( source_name  ${CMAKE_CURRENT_LIST_DIR}/${item} )
    string ( REPLACE "//" "/" source_name ${source_name})
    target_sources ( ${INCFLO_LIBNAME} PRIVATE ${source_name} )

    # Add folder to list of includes if necessary
    get_filename_component ( filetype ${source_name} EXT )
    if ( (${filetype} MATCHES ".h") OR (${filetype} MATCHES ".H" ) )
      list (APPEND INCFLO_INCLUDE_PATHS ${CMAKE_CURRENT_LIST_DIR})
    endif()
  endforeach()
endmacro()

# Fist, define the library we want to add
add_library ( ${INCFLO_LIBNAME} "" )

# Now, one by one, we add all the sources from all the subdirs
# to the library defined above

add_sources ( incflo.cpp  )
add_sources ( main.cpp )
add_sources ( incflo.H )

include ( advance/CMakeLists.txt )
include ( boundary_conditions/CMakeLists.txt )
include ( convection/CMakeLists.txt )
include ( derive/CMakeLists.txt )
include ( diffusion/CMakeLists.txt )
include ( divergence/CMakeLists.txt )
include ( embedded_boundaries/CMakeLists.txt )
include ( include/CMakeLists.txt )
include ( projection/CMakeLists.txt )
include ( rheology/CMakeLists.txt )
include ( setup/CMakeLists.txt )
include ( utilities/CMakeLists.txt )


# Remove duplicates from include lists
list ( REMOVE_DUPLICATES INCFLO_INCLUDE_PATHS )
target_include_directories ( ${INCFLO_LIBNAME} PUBLIC ${INCFLO_INCLUDE_PATHS} )

#
# Link to imported AMReX
#
target_link_libraries ( ${INCFLO_LIBNAME} AMReX::amrex )

#
# Set the default flags 
# 
if ( NOT CMAKE_Fortran_FLAGS )
   target_compile_options ( ${INCFLO_LIBNAME}
      PUBLIC
      # GNU Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:Fortran>:
      -O0 -ggdb -fbounds-check -fbacktrace -Wuninitialized -Wunused -finit-real=snan -finit-integer=2147483647>>>>
      # GNU Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:Fortran>:
      >>>>
      # Intel Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:Intel>:$<$<COMPILE_LANGUAGE:Fortran>:
      -O0 -traceback -check bounds,uninit,pointers>>>>
      # Intel Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:Intel>:$<$<COMPILE_LANGUAGE:Fortran>:
      -ip -qopt-report=5 -qopt-report-phase=vec>>>>
      # Cray Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:Cray>:$<$<COMPILE_LANGUAGE:Fortran>:
      -O0 -e i>>>>
      # Cray Release 
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:Cray>:$<$<COMPILE_LANGUAGE:Fortran>:
      >>>>
      # PGI Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:PGI>:$<$<COMPILE_LANGUAGE:Fortran>:
      -O0 -Mbounds -Mchkptr>>>>
      # PGI Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:PGI>:$<$<COMPILE_LANGUAGE:Fortran>:
      -gopt -fast>>>>
      )	  
endif ()

if ( NOT CMAKE_CXX_FLAGS )
   target_compile_options ( ${INCFLO_LIBNAME}
      PUBLIC
      # GNU Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:
      -O0 -fno-inline -ggdb -Wall -Wno-sign-compare>>>>
      # GNU Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:
      >>>>
      # Intel Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:Intel>:$<$<COMPILE_LANGUAGE:CXX>:
      -O0 -traceback -Wcheck>>>>
      # Intel Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:Intel>:$<$<COMPILE_LANGUAGE:CXX>:
      -ip -qopt-report=5 -qopt-report-phase=vec>>>>
      # Cray Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:Cray>:$<$<COMPILE_LANGUAGE:CXX>:
      -O0>>>>
      # Cray Release 
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:Cray>:$<$<COMPILE_LANGUAGE:CXX>:
      >>>>
      # PGI Debug
      $<BUILD_INTERFACE:$<$<CONFIG:Debug>:$<$<C_COMPILER_ID:PGI>:$<$<COMPILE_LANGUAGE:CXX>:
      -O0 -Mbounds>>>>
      # PGI Release
      $<BUILD_INTERFACE:$<$<CONFIG:Release>:$<$<C_COMPILER_ID:PGI>:$<$<COMPILE_LANGUAGE:CXX>:
      -gopt -fast>>>>
      )	  
endif ()
