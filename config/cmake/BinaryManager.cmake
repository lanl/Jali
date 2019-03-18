# Copyright (c) 2019, Triad National Security, LLC
# All rights reserved.

# Copyright 2019. Triad National Security, LLC. This software was
# produced under U.S. Government contract 89233218CNA000001 for Los
# Alamos National Laboratory (LANL), which is operated by Triad
# National Security, LLC for the U.S. Department of Energy. 
# All rights in the program are reserved by Triad National Security,
# LLC, and the U.S. Department of Energy/National Nuclear Security
# Administration. The Government is granted for itself and others acting
# on its behalf a nonexclusive, paid-up, irrevocable worldwide license
# in this material to reproduce, prepare derivative works, distribute
# copies to the public, perform publicly and display publicly, and to
# permit others to do so
 
# 
# This is open source software distributed under the 3-clause BSD license.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of Triad National Security, LLC, Los Alamos
#    National Laboratory, LANL, the U.S. Government, nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.

 
# THIS SOFTWARE IS PROVIDED BY TRIAD NATIONAL SECURITY, LLC AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# TRIAD NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Functions for building and managing binaries.
#

include(CMakeParseArguments)
include(PrintVariable)
include(InstallManager)

#
# Usage:
#
# ADD_Jali_EXECUTABLE(<target name>
#                       SOURCE file1 file2 ....
#                       LINK_LIBS lib1 lib2 .....
#                       OUPUT_NAME name
#                       OUTPUT_DIRECTORY dir
#                       NO_INSTALL)
# 
#
# Arguments:
#
#   target CMake target name defined for this binary
#
#   SOURCE List of source files to compile 
#
#   LINK_LIBS Defines the list of link libraries required to build and link target
#
#   OUTPUT_NAME (Optional) Set the output name to OUTPUT_NAME, will use the CMake defaults
#   if this is not set.
#
#   OUTPUT_DIRECTORY (Optional) Set the output directory, default is CMAKE_CURRENT_BINARY_DIR
#
#   NO_INSTALL (Optional) All binaries defined by this function will be added to the install
#   target. Use this option to not install the binary.
#
# 
function(ADD_Jali_EXECUTABLE target)

  # --- Parse the input
  set(options NO_INSTALL)
  set(oneValueArgs OUTPUT_NAME OUTPUT_DIRECTORY)
  set(multiValueArgs SOURCE LINK_LIBS)
  cmake_parse_arguments(Jali_BIN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # --- Check the input
  if ( NOT Jali_BIN_SOURCE )
    message(FATAL "Can not define binary ${target} without source files.")
  endif()

  # --- Define the executable
  add_executable(${target} ${Jali_BIN_SOURCE})

  # --- Add link libraries
  if ( Jali_BIN_LINK_LIBS )
    target_link_libraries(${target} ${Jali_BIN_LINK_LIBS})
  endif()

  # --- Add target to the install target
  if ( NOT "${Jali_BIN_NO_INSTALL}" )
    add_install_binary(${target})
  endif()

  # --- Change the output name and directory if requested
  if ( Jali_BIN_OUTPUT_NAME )
    set_target_properties( ${target} PROPERTIES OUTPUT_NAME ${Jali_BIN_OUTPUT_NAME})
  endif()

  if ( Jali_BIN_OUTPUT_DIRECTORY )
    set_target_properties(${target} PROPERTIES OUTPUT_DIRECTORY ${Jali_BIN_OUTPUT_NAME})
  endif()



endfunction(ADD_Jali_EXECUTABLE)

