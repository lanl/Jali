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
# Build TPL: NetCDF-fortran
# 

# --- Define all the directories and common external project flags
define_external_project_args(NetCDF_Fortran TARGET netcdf-fortran
	DEPENDS ${MPI_PROJECT} NetCDF)

# add version version to the autogenerated tpl_versions.h file
Jali_tpl_version_write(FILENAME ${TPL_VERSIONS_INCLUDE_FILE}
  PREFIX NetCDF_Fortran
  VERSION ${NetCDF_Fortran_VERSION_MAJOR} ${NetCDF_Fortran_VERSION_MINOR} ${NetCDF_Fortran_VERSION_PATCH})
  
#set(cpp_flags_list -I${NetCDF_prefix_dir}/include)

set(cpp_flags_list -I${TPL_INSTALL_PREFIX}/include)
set(ld_flags_list -L${TPL_INSTALL_PREFIX}/lib)
list(REMOVE_DUPLICATES cpp_flags_list)
list(REMOVE_DUPLICATES ld_flags_list)
build_whitespace_string(netcdf_fortran_cppflags ${cpp_flags_list})
build_whitespace_string(netcdf_fortran_ldflags ${ld_flags_list})

# --- Add external project build and tie to the ZLIB build target
ExternalProject_Add(${NetCDF_Fortran_BUILD_TARGET}

	# Package dependency target
	DEPENDS ${NetCDF_Fortran_PACKAGE_DEPENDS}

	# Temporary files directory
	TMP_DIR ${NetCDF_Fortran_tmp_dir}

	# Timestamp and log directory
	STAMP_DIR ${NetCDF_Fortran_stamp_dir}

	# -- Downloads

	# Download directory
	DOWNLOAD_DIR ${TPL_DOWNLOAD_DIR}

	# URL may be a web site OR a local file
	URL ${NetCDF_Fortran_URL}

	# md5sum of the archive file
	URL_MD5 ${NetCDF_Fortran_MD5_SUM}

	# -- Configure

	# Source directory
	SOURCE_DIR ${NetCDF_Fortran_source_dir}

	CONFIGURE_COMMAND
		<SOURCE_DIR>/configure
		--prefix=<INSTALL_DIR>
		--disable-shared
		FC=${CMAKE_Fortran_COMPILER}
		FCFLAGS=${Jali_COMMON_FCFLAGS}
		CPPFLAGS=${netcdf_fortran_cppflags}
		LDFLAGS=${netcdf_fortran_ldflags}

	# -- Build

	# Build directory 
	BINARY_DIR ${NetCDF_Fortran_build_dir}

	# $(MAKE) enables parallel builds through make
	BUILD_COMMAND $(MAKE)

	# Flag for in source builds
	BUILD_IN_SOURCE ${NetCDF_Fortran_BUILD_IN_SOURCE}

	# -- Install

	# Install directory
	INSTALL_DIR ${TPL_INSTALL_PREFIX}

	# -- Output control
	${NetCDF_Fortran_logging_args})
