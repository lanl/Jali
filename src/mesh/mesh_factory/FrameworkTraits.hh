/* -*-  mode: c++; c-default-style: "google"; indent-tabs-mode: nil -*- */
// -------------------------------------------------------------
/**
 * @file   FrameworkTraits.hh
 * @author William A. Perkins
 * @date Mon Aug  1 13:47:02 2011
 *
 * @brief
 *
 *
 */
// -------------------------------------------------------------
// -------------------------------------------------------------
// Created March 10, 2011 by William A. Perkins
// Last Change: Mon Aug  1 13:47:02 2011 by William A. Perkins <d3g096@PE10900.pnl.gov>
// -------------------------------------------------------------
#ifndef _FrameworkTraits_hh_
#define _FrameworkTraits_hh_

#include <mpi.h>
#include <vector>

#include "MeshFramework.hh"
#include "MeshFileType.hh"
#include "Mesh.hh"

#include "GeometricModel.hh"

#include "errors.hh"
#include "exceptions.hh"

namespace Jali {

/// Is the specified framework available
extern bool framework_available(const Framework& f);

/// General routine to see if a format can be read by particular framework
extern bool framework_reads(const Framework& f, const Format& fmt,
                            const bool& parallel);

/// Read a mesh
extern Mesh *
framework_read(const MPI_Comm& comm, const Framework& f,
               const std::string& fname,
               const JaliGeometry::GeometricModelPtr& gm =
               (JaliGeometry::GeometricModelPtr) NULL,
               const bool request_faces = true,
               const bool request_edges = false,
               const bool request_wedges = false,
               const bool request_corners = false,
               const int num_tiles=0);

/// General routine to see if a mesh can be generated by a particular framework
extern bool framework_generates(const Framework& f, const bool& parallel,
                                const unsigned int& dimension);

/// General routine to see if a mesh can be extracted from another mesh by a particular framework
extern bool framework_extracts(const Framework& f, const bool& parallel,
                               const unsigned int& dimension);

/// Generate a hexahedral mesh
extern Mesh *
framework_generate(const MPI_Comm& comm, const Framework& f,
                   const double& x0, const double& y0, const double& z0,
                   const double& x1, const double& y1, const double& z1,
                   const unsigned int& nx, const unsigned int& ny,
                   const unsigned int& nz,
                   const JaliGeometry::GeometricModelPtr& gm =
                   (JaliGeometry::GeometricModelPtr) NULL,
                   const bool request_faces = true,
                   const bool request_edges = false,
                   const bool request_wedges = false,
                   const bool request_corners = false,
                   const int num_tiles = 0);

/// Generate a quadrilateral mesh
extern Mesh *
framework_generate(const MPI_Comm& comm, const Framework& f,
                   const double& x0, const double& y0,
                   const double& x1, const double& y1,
                   const unsigned int& nx, const unsigned int& ny,
                   const JaliGeometry::GeometricModelPtr& gm =
                   (JaliGeometry::GeometricModelPtr) NULL,
                   const bool request_faces = true,
                   const bool request_edges = false,
                   const bool request_wedges = false,
                   const bool request_corners = false,
                   const int num_tiles = 0);

extern Mesh *
framework_extract(const MPI_Comm& comm, const Framework& f,
                  const Mesh *inmesh,
                  const std::vector<std::string>& setnames,
                  const Entity_kind setkind,
                  const bool flatten = false,
                  const bool extrude = false,
                  const bool request_faces = true,
                  const bool request_edges = false,
                  const bool request_wedges = false,
                  const bool request_corners = false,
                  const int num_tiles = 0);

extern Mesh *
framework_extract(const MPI_Comm& comm, const Framework& f,
                  const Mesh& inmesh,
                  const std::vector<std::string>& setnames,
                  const Entity_kind setkind,
                  const bool flatten = false,
                  const bool extrude = false,
                  const bool request_faces = true,
                  const bool request_edges = false,
                  const bool request_wedges = false,
                  const bool request_corners = false,
                  const int num_tiles = 0);

Mesh *
framework_extract(const MPI_Comm& comm, const Framework& f,
                  const Mesh& inmesh,
                  const std::vector<int>& entity_id_list,
                  const Entity_kind entity_kind,
                  const bool request_faces = true,
                  const bool request_edges = false,
                  const bool request_wedges = false,
                  const bool request_corners = false,
                  const int num_tiles = 0,
                  const bool flatten = false,
                  const bool extrude = false);

}  // namespace Jali

#endif
