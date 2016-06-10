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
#include "Geometry.hh"

#include "errors.hh"
#include "exceptions.hh"

namespace Jali {

/// Is the specified framework available
extern bool framework_available(const Framework& f);

/// General routine to see if a format can be read by particular framework
extern bool framework_reads(const Framework& f, const Format& fmt,
                            const bool& parallel);

/// Read a mesh
extern std::shared_ptr<Mesh>
framework_read(const MPI_Comm& comm, const Framework& f, 
               const std::string& fname,
               const JaliGeometry::GeometricModelPtr& gm,
               const bool request_faces,
               const bool request_edges,
               const bool request_sides,
               const bool request_wedges,
               const bool request_corners,
               const int num_tiles,
               const int num_ghost_layers_tile,
               const int num_ghost_layers_distmesh,
               const bool boundary_ghosts_requested,
               const Partitioner_type partitioner,
               const JaliGeometry::Geom_type geom_type);

/// General routine to see if a mesh can be generated by a particular framework
extern bool framework_generates(const Framework& f, const bool& parallel,
                                const unsigned int& dimension);

/// General routine to see if a mesh can be extracted from another
/// mesh by a particular framework
extern bool framework_extracts(const Framework& f, const bool& parallel,
                               const unsigned int& dimension);

/// Generate a hexahedral mesh
extern std::shared_ptr<Mesh>
framework_generate(const MPI_Comm& comm, const Framework& f, 
                   const double& x0, const double& y0, const double& z0,
                   const double& x1, const double& y1, const double& z1,
                   const unsigned int& nx, const unsigned int& ny,
                   const unsigned int& nz,
                   const JaliGeometry::GeometricModelPtr& gm,
                   const bool request_faces,
                   const bool request_edges,
                   const bool request_sides,
                   const bool request_wedges,
                   const bool request_corners,
                   const int num_tiles,
                   const int num_ghost_layers_tile,
                   const int num_ghost_layers_distmesh,
                   const bool boundary_ghosts_requested,
                   const Partitioner_type partitioner);

/// Generate a quadrilateral mesh
extern std::shared_ptr<Mesh>
framework_generate(const MPI_Comm& comm, const Framework& f, 
                   const double& x0, const double& y0,
                   const double& x1, const double& y1,
                   const unsigned int& nx, const unsigned int& ny,
                   const JaliGeometry::GeometricModelPtr& gm,
                   const bool request_faces,
                   const bool request_edges,
                   const bool request_sides,
                   const bool request_wedges,
                   const bool request_corners,
                   const int num_tiles,
                   const int num_ghost_layers_tile,
                   const int num_ghost_layers_distmesh,
                   const bool boundary_ghosts_requested,
                   const Partitioner_type partitioner,
                   const JaliGeometry::Geom_type geom_type);

/// Generate a 1d mesh
extern std::shared_ptr<Mesh>
framework_generate(const MPI_Comm& comm, const Framework& f,
                   const std::vector<double>& x,
                   const JaliGeometry::GeometricModelPtr& gm,
                   const bool request_faces,
                   const bool request_edges,
                   const bool request_sides,
                   const bool request_wedges,
                   const bool request_corners,
                   const int num_tiles,
                   const int num_ghost_layers_tile,
                   const int num_ghost_layers_distmesh,
                   const bool boundary_ghosts_requested,
                   const Partitioner_type partitioner,
                   const JaliGeometry::Geom_type geom_type);

extern std::shared_ptr<Mesh>
framework_extract(const MPI_Comm& comm, const Framework& f,
                  const std::shared_ptr<Mesh> inmesh,
                  const std::vector<std::string>& setnames,
                  const Entity_kind setkind,
                  const bool flatten,
                  const bool extrude,
                  const bool request_faces,
                  const bool request_edges,
                  const bool request_sides,
                  const bool request_wedges,
                  const bool request_corners,
                  const int num_tiles,
                  const int num_ghost_layers_tile,
                  const int num_ghost_layers_distmesh,
                  const bool boundary_ghosts_requested,
                  const Partitioner_type partitioner,
                  const JaliGeometry::Geom_type geom_type);

extern std::shared_ptr<Mesh>
framework_extract(const MPI_Comm& comm, const Framework& f,
                  const Mesh& inmesh,
                  const std::vector<std::string>& setnames,
                  const Entity_kind setkind,
                  const bool flatten,
                  const bool extrude,
                  const bool request_faces,
                  const bool request_edges,
                  const bool request_sides,
                  const bool request_wedges,
                  const bool request_corners,
                  const int num_tiles,
                  const int num_ghost_layers_tile,
                  const int num_ghost_layers_distmesh,
                  const bool boundary_ghosts_requested,
                  const Partitioner_type partitioner,
                  const JaliGeometry::Geom_type geom_type);

std::shared_ptr<Mesh>
framework_extract(const MPI_Comm& comm, const Framework& f, 
                  const Mesh& inmesh, 
                  const std::vector<int>& entity_id_list,
                  const Entity_kind entity_kind,
                  const bool request_faces,
                  const bool request_edges,
                  const bool request_sides,
                  const bool request_wedges,
                  const bool request_corners,
                  const int num_tiles,
                  const bool flatten,
                  const bool extrude,
                  const int num_ghost_layers_tile,
                  const int num_ghost_layers_distmesh,
                  const bool boundary_ghosts_requested,
                  const Partitioner_type partitioner,
                  const JaliGeometry::Geom_type geom_type);

}  // namespace Jali

#endif
