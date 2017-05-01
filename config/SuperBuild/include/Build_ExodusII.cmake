# Copyright (c) 2017, Los Alamos National Security, LLC
# All rights reserved.

# Copyright 2017. Los Alamos National Security, LLC. This software was
# produced under U.S. Government contract DE-AC52-06NA25396 for Los
# Alamos National Laboratory (LANL), which is operated by Los Alamos
# National Security, LLC for the U.S. Department of Energy. The
# U.S. Government has rights to use, reproduce, and distribute this
# software.  NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY,
# LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR ASSUMES ANY LIABILITY
# FOR THE USE OF THIS SOFTWARE.  If software is modified to produce
# derivative works, such modified software should be clearly marked, so
# as not to confuse it with the version available from LANL.
 
# Additionally, redistribution and use in source and binary forms, with
# or without modification, are permitted provided that the following
# conditions are met:

# 1.  Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 3.  Neither the name of Los Alamos National Security, LLC, Los Alamos
# National Laboratory, LANL, the U.S. Government, nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
 
# THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS
# ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



#
# Build TPL: ExodusII 
#   

# --- Define all the directories and common external project flags
define_external_project_args(ExodusII 
                             TARGET exodusii
                             DEPENDS ${MPI_PROJECT} NetCDF)

# add version to the autogenerated tpl_versions.h file
Jali_tpl_version_write(FILENAME ${TPL_VERSIONS_INCLUDE_FILE}
  PREFIX ExodusII
  VERSION ${ExodusII_VERSION_MAJOR} ${ExodusII_VERSION_MINOR} ${ExodusII_VERSION_PATCH})


# --- Define the configure command

# Need to define variables used in the configure script
include(BuildWhitespaceString)
build_whitespace_string(common_cmake_args
                       ${Jali_CMAKE_C_COMPILER_ARGS}
                       -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER_USE}
                       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

# Build the configure script
set(ExodusII_sh_configure ${ExodusII_prefix_dir}/exodusii-configure-step.sh)
configure_file(${SuperBuild_TEMPLATE_FILES_DIR}/exodusii-configure-step.sh.in
               ${ExodusII_sh_configure}
               @ONLY)

# Configure the CMake command file
set(ExodusII_cmake_configure ${ExodusII_prefix_dir}/exodusii-configure-step.cmake)
configure_file(${SuperBuild_TEMPLATE_FILES_DIR}/exodusii-configure-step.cmake.in
               ${ExodusII_cmake_configure}
               @ONLY)
set(ExodusII_CONFIGURE_COMMAND ${CMAKE_COMMAND} -P ${ExodusII_cmake_configure})  

# --- Add external project build and tie to the ZLIB build target
ExternalProject_Add(${ExodusII_BUILD_TARGET}
                    DEPENDS   ${ExodusII_PACKAGE_DEPENDS}             # Package dependency target
                    TMP_DIR   ${ExodusII_tmp_dir}                     # Temporary files directory
                    STAMP_DIR ${ExodusII_stamp_dir}                   # Timestamp and log directory
                    # -- Download and URL definitions
                    DOWNLOAD_DIR ${TPL_DOWNLOAD_DIR}                  # Download directory
                    URL          ${ExodusII_URL}                      # URL may be a web site OR a local file
                    URL_MD5      ${ExodusII_MD5_SUM}                  # md5sum of the archive file
                    # -- Configure
                    SOURCE_DIR         ${ExodusII_source_dir}         # Source directory
                    CONFIGURE_COMMAND  ${ExodusII_CONFIGURE_COMMAND}
                    # -- Build
                    BINARY_DIR        ${ExodusII_build_dir}           # Build directory 
                    BUILD_COMMAND     $(MAKE)
                    BUILD_IN_SOURCE   ${ExodusII_BUILD_IN_SOURCE}     # Flag for in source builds
                    # -- Install
                    INSTALL_DIR      ${TPL_INSTALL_PREFIX}            # Install directory
                    # -- Output control
                    ${ExodusII_logging_args})

# --- Add the nemsis build step  

# Configure the build script
#EXODUS-5.xset(NEMESIS_sh_build ${ExodusII_prefix_dir}/nemesis-build.sh)
#EXODUS-5.xconfigure_file(${SuperBuild_TEMPLATE_FILES_DIR}/nemesis-build.sh.in
#EXODUS-5.x               ${NEMESIS_sh_build}
#EXODUS-5.x               @ONLY)

# Configure the CMake command file
#EXODUS-5.xset(NEMESIS_cmake_build ${ExodusII_prefix_dir}/nemesis-build.cmake)
#EXODUS-5.xconfigure_file(${SuperBuild_TEMPLATE_FILES_DIR}/nemesis-build.cmake.in
#EXODUS-5.x               ${NEMESIS_cmake_build}
#EXODUS-5.x               @ONLY)
#EXODUS-5.xset(NEMESIS_BUILD_COMMAND ${CMAKE_COMMAND} -P ${NEMESIS_cmake_build}) 
#EXODUS-5.x
#EXODUS-5.xExternalProject_Add_Step(${ExodusII_BUILD_TARGET} nemesis
#EXODUS-5.x                         COMMAND ${NEMESIS_BUILD_COMMAND}
#EXODUS-5.x                         COMMENT "Building nemesis (ExodusII extension)"
#EXODUS-5.x                         DEPENDEES install
#EXODUS-5.x                         WORKING_DIRECTORY ${ExodusII_prefix}
#EXODUS-5.x                         LOG TRUE)
