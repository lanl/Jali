//
// Copyright Los Alamos National Security, LLC 2009-2015
// All rights reserved. See Copyright notice in main directory
//

#ifndef _JALI_MESHTILE_H_
#define _JALI_MESHTILE_H_

#include <vector>
#include <algorithm>

#include "mpi.h"

#include "MeshDefs.hh"

namespace Jali {

/*! 
  @class MeshTile "MeshTile.hh"
  @brief Encapsulates notion of a meshtile (a group of cells)
  
  Mesh tiles are small groupings of mesh cells on a compute node and
  are typically obtained by partitioning the full mesh on a compute
  node. They are lightweight structures that are merely lists of cells
  that are in a partition. Mesh tiles include ghost nodes at the
  compute node level (i.e. MPI level) BUT TYPICALLY ARE NOT EXPECTED
  TO INCLUDE GHOST CELLS (although they can).
  
  Meshtiles are a way for the work on a mesh to be broken up into
  manageable, parallely executable chunks.
  
  **** IMPORTANT NOTE ABOUT CONSTANTNESS OF THIS CLASS ****
  Instantiating a const version of this class only guarantees that
  the underlying mesh topology and geometry does not change (the
  public interfaces conforms strictly to this definition). However,
  for purposes of memory savings we use lazy initialization and
  caching of face data, edge data, geometry quantities, columns etc.,
  which means that these data may still change. We also cannot
  initialize the cached quantities in the constructor since they
  depend on initialization of data structures in the derived class -
  however, the base class gets constructed before the derived class
  gets constructed so it is not possible without more obscure
  acrobatics. This is why some of the caching data declarations are
  declared with the keyword 'mutable' and routines that modify the
  mutable data are declared with a constant qualifier.
  
*/

// forward declaration of the mesh class

class Mesh;

class MeshTile {

 public:

  /// @brief Constructor

  MeshTile(Mesh const & parent_mesh, std::vector<Entity_ID> const meshcells,
           bool const request_faces = true, bool const request_edges = false,
           bool const request_wedges = false,
           bool const request_corners = false);

  /// @brief Copy Constructor - deleted

  MeshTile(MeshTile const &meshtile_in) = delete;

  /// @brief Assignment operator - deleted

  MeshTile & operator=(MeshTile const &meshtile_in) = delete;

  /// @brief Destructor

  ~MeshTile() {}

  /// @brief The mesh that this tile belongs to

  Mesh const & mesh() {
    return mesh_;
  }

  //
  // General mesh tile information
  // -------------------------
  //

  /*! 
    @brief Number of entities of a particular kind and parallel type
    @param kind Entity_kind of the entities (CELL, NODE, WEDGE etc)
    @param parallel_type Parallel_type of entities (OWNED, GHOST, ALL)
  */

  unsigned int num_entities(Entity_kind kind, Parallel_type parallel_type) const;

  /*! 
    @brief Number of nodes of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_nodes() const;

  /*! 
    @brief Number of edges of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_edges() const;

  /*! 
    @brief Number of faces of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_faces() const;

  /*! 
    @brief Number of wedges of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_wedges() const;

  /*! 
    @brief Number of corners of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_corners() const;

  /*! 
    @brief Number of nodes of parallel type
    @tparam ptype Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  unsigned int num_cells() const;

  /*! 
    @brief Node list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL>
  std::vector<Entity_ID> const & nodes() const;

  /*! 
    @brief Edge list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL> std::vector<Entity_ID>
  const & edges() const;

  /*! 
    @brief Face list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL> std::vector<Entity_ID>
  const & faces() const;

  /*! 
    @brief Wedge list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL> std::vector<Entity_ID>
  const & wedges() const;

  /*! 
    @brief Corner list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL> std::vector<Entity_ID>
  const & corners() const;

  /*! 
    @brief Cell list
    @tparam ptype   Parallel type (Parallel_type::OWNED, Parallel_type::GHOST, Parallel_type::ALL)
  */
  template<Parallel_type ptype = Parallel_type::ALL> std::vector<Entity_ID>
  const & cells() const;

 private:

  // Data

  Mesh const & mesh_;

  std::vector<Entity_ID> nodeids_owned_, nodeids_ghost_, nodeids_all_;
  std::vector<Entity_ID> edgeids_owned_, edgeids_ghost_, edgeids_all_;
  std::vector<Entity_ID> faceids_owned_, faceids_ghost_, faceids_all_;
  std::vector<Entity_ID> wedgeids_owned_, wedgeids_ghost_, wedgeids_all_;
  std::vector<Entity_ID> cornerids_owned_, cornerids_ghost_, cornerids_all_;
  std::vector<Entity_ID> cellids_owned_, cellids_ghost_, cellids_all_;
  std::vector<Entity_ID> dummy_list_;

  // Make the State class a friend so that it can access protected
  // methods for retrieving and storing mesh fields

  friend class State;

}; // End class MeshTile



// Default implementation of num_nodes prints an error and returns
// 0 while the meaningful implementation is in specialized routines
template<Parallel_type ptype> inline
unsigned int MeshTile::num_nodes() const {
  std::cerr << "MeshTile::num_nodes() - " <<
      "Meaningless to query for list of nodes of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_nodes<Parallel_type::OWNED>() const {
  return nodeids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_nodes<Parallel_type::GHOST>() const {
  return nodeids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_nodes<Parallel_type::ALL>() const {
  return (num_nodes<Parallel_type::OWNED>() +
          num_nodes<Parallel_type::GHOST>());
}

template<Parallel_type ptype> inline
unsigned int MeshTile::num_edges() const {
  std::cerr << "MeshTile::num_edges() - " <<
      "Meaningless to query for list of edges of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_edges<Parallel_type::OWNED>() const {
  return edgeids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_edges<Parallel_type::GHOST>() const {
  return edgeids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_edges<Parallel_type::ALL>() const {
  return (num_edges<Parallel_type::OWNED>() +
          num_edges<Parallel_type::GHOST>());
}

template<Parallel_type ptype> inline
unsigned int MeshTile::num_faces() const {
  std::cerr << "MeshTile::num_faces() - " <<
      "Meaningless to query for list of faces of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_faces<Parallel_type::OWNED>() const {
  return faceids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_faces<Parallel_type::GHOST>() const {
  return faceids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_faces<Parallel_type::ALL>() const {
  return (num_faces<Parallel_type::OWNED>() +
          num_faces<Parallel_type::GHOST>());
}

template<Parallel_type ptype> inline
unsigned int MeshTile::num_wedges() const {
  std::cerr << "MeshTile::num_wedges() - " <<
      "Meaningless to query for list of wedges of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_wedges<Parallel_type::OWNED>() const {
return wedgeids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_wedges<Parallel_type::GHOST>() const {
  return wedgeids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_wedges<Parallel_type::ALL>() const {
  return (num_wedges<Parallel_type::OWNED>() +
          num_wedges<Parallel_type::GHOST>());
}

template<Parallel_type ptype> inline
unsigned int MeshTile::num_corners() const {
  std::cerr << "MeshTile::num_corners() - " <<
      "Meaningless to query for list of corners of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_corners<Parallel_type::OWNED>() const {
  return cornerids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_corners<Parallel_type::GHOST>() const {
  return cornerids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_corners<Parallel_type::ALL>() const {
  return (num_corners<Parallel_type::OWNED>() +
          num_corners<Parallel_type::GHOST>());
}

template<Parallel_type ptype> inline
unsigned int MeshTile::num_cells() const {
  std::cerr << "MeshTile::num_cells() - " <<
      "Meaningless to query for list of cells of kind parallel type " <<
      ptype << "\n";
  return 0;
}
template<> inline
unsigned int MeshTile::num_cells<Parallel_type::OWNED>() const {
  return cellids_owned_.size();
}
template<> inline
unsigned int MeshTile::num_cells<Parallel_type::GHOST>() const {
  return cellids_ghost_.size();
}
template<> inline
unsigned int MeshTile::num_cells<Parallel_type::ALL>() const {
  return (num_cells<Parallel_type::OWNED>() +
          num_cells<Parallel_type::GHOST>());
}



inline
unsigned int MeshTile::num_entities(const Entity_kind kind,
                                   const Parallel_type ptype) const {
  switch (kind) {
    case Entity_kind::NODE:
      switch (ptype) {
        case Parallel_type::OWNED: return num_nodes<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_nodes<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_nodes<Parallel_type::ALL>();
        default: return 0;
      }
    case Entity_kind::EDGE:
      switch (ptype) {
        case Parallel_type::OWNED: return num_edges<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_edges<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_edges<Parallel_type::ALL>();
        default: return 0;
      }
    case Entity_kind::FACE:
      switch (ptype) {
        case Parallel_type::OWNED: return num_faces<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_faces<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_faces<Parallel_type::ALL>();
        default: return 0;
      }
    case Entity_kind::WEDGE:
      switch (ptype) {
        case Parallel_type::OWNED: return num_wedges<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_wedges<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_wedges<Parallel_type::ALL>();
        default: return 0;
      }
    case Entity_kind::CORNER:
      switch (ptype) {
        case Parallel_type::OWNED: return num_corners<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_corners<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_corners<Parallel_type::ALL>();
        default: return 0;
      }
    case Entity_kind::CELL:
      switch (ptype) {
        case Parallel_type::OWNED: return num_cells<Parallel_type::OWNED>();
        case Parallel_type::GHOST: return num_cells<Parallel_type::GHOST>();
        case Parallel_type::ALL: return num_cells<Parallel_type::ALL>();
        default: return 0;
      }
    default:
      return 0;
  }
}




// entity lists (default implementation prints error message -
// meaningful values returned through template specialization)

template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::nodes() const {
  std::cerr << "MeshTile::nodes() - " <<
      "Meaningless to query for list of nodes of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::nodes<Parallel_type::OWNED>() const {
  return nodeids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::nodes<Parallel_type::GHOST>() const {
  return nodeids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::nodes<Parallel_type::ALL>() const {
  return nodeids_all_;
}

template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::edges() const {
  std::cerr << "MeshTile::edges() - " <<
      "Meaningless to query for list of edges of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::edges<Parallel_type::OWNED>() const {
  return edgeids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::edges<Parallel_type::GHOST>() const {
  return edgeids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::edges<Parallel_type::ALL>() const {
  return edgeids_all_;
}

template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::faces() const {
  std::cerr << "MeshTile::faces() - " <<
      "Meaningless to query for list of faces of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::faces<Parallel_type::OWNED>() const {
  return faceids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::faces<Parallel_type::GHOST>() const {
  return faceids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::faces<Parallel_type::ALL>() const {
  return faceids_all_;
}


template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::wedges() const {
  std::cerr << "MeshTile::wedges() - " <<
      "Meaningless to query for list of wedges of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::wedges<Parallel_type::OWNED>() const {
  return wedgeids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::wedges<Parallel_type::GHOST>() const {
  return wedgeids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::wedges<Parallel_type::ALL>() const {
  return wedgeids_all_;
}


template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::corners() const {
  std::cerr << "MeshTile::corners() - " <<
      "Meaningless to query for list of corners of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::corners<Parallel_type::OWNED>() const {
  return cornerids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::corners<Parallel_type::GHOST>() const {
  return cornerids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::corners<Parallel_type::ALL>() const {
  return cornerids_all_;
}


template<Parallel_type ptype> inline
const std::vector<Entity_ID> & MeshTile::cells() const {
  std::cerr << "MeshTile::cells() - " <<
      "Meaningless to query for list of cells of parallel type " <<
      ptype << "\n";
  return dummy_list_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::cells<Parallel_type::OWNED>() const {
  return cellids_owned_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::cells<Parallel_type::GHOST>() const {
  return cellids_ghost_;
}
template<> inline
const std::vector<Entity_ID> & MeshTile::cells<Parallel_type::ALL>() const {
  return cellids_all_;
}

// @brief MeshTile factory
//
// Standalone function to make a tile and return a pointer to it so
// that Mesh.hh can use a forward declaration of MeshTile and this
// function to create new tiles

std::shared_ptr<MeshTile> make_meshtile(Mesh const & parent_mesh,
                                        const std::vector<Entity_ID> & cells,
                                        const bool request_faces,
                                        const bool request_edges,
                                        const bool request_wedges,
                                        const bool request_corners);


} // end namespace Jali





#endif /* _JALI_MESHTILE_H_ */