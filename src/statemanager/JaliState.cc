/*---------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *---------------------------------------------------------------------------~*/

#include <memory>
#include "JaliState.h"
#include "JaliStateVector.h"

namespace Jali {

// Add new material to the state

void State::add_material(std::string const& matname,
                         std::vector<int> const& matcells) {
  for (auto const& mset : material_cellsets_)
    if (matname == mset->name()) {
      std::cerr << "Material name \"" << matname << "\" already used\n";
      return;
    }

  // Find a cell set with this name in the mesh. If it doesn't
  // exist, create it
  std::shared_ptr<MeshSet> matset =
      mymesh_->find_meshset(matname, Entity_kind::CELL);
  if (!matset) {
    Entity_ID_List owned_entities, ghost_entities;
    bool with_reverse_map = true;
    matset = make_meshset(matname, *mymesh_, Jali::Entity_kind::CELL,
                          owned_entities, ghost_entities, with_reverse_map);
  }

  matset->add_entities(matcells);

  material_cellsets_.push_back(matset);

  int matid = num_materials()-1;
  if (cell_materials_.size() == 0)
    cell_materials_.resize(mymesh_->num_entities(Jali::Entity_kind::CELL,
                                                 Jali::Entity_type::ALL));
  for (auto const& c : matcells)
    cell_materials_[c].push_back(matid);

  // GO TO EACH MMStateVector AND ADD ENTRIES FOR THIS MATERIAL
  for (auto &sv : state_vectors_) {
    if (sv->type() == StateVector_type::MULTIVAL) {
      auto mv = std::dynamic_pointer_cast<MultiStateVectorBase<Mesh>>(sv);
      if (mv) mv->add_material(matcells.size());
    }
  }
}

// Remove material from the state -- EXPENSIVE (CAN JUST MAKE SET EMPTY)
void State::rem_material(int m) {
  if (m >= material_cellsets_.size())
    return;

  std::shared_ptr<MeshSet> matset = material_cellsets_[m];
  if (!matset) return;

  for (auto const& c : material_cellsets_[m]->entities()) {
    int nmats = cell_materials_[c].size();
    for (int i = 0; i < nmats; i++)
      if (m == cell_materials_[c][i])
        cell_materials_[c].erase(cell_materials_[c].begin()+i);
  }

  material_cellsets_.erase(material_cellsets_.begin()+m);

  // GO TO EACH MMStateVector AND REMOVE ENTRIES FOR THIS MATERIAL
  for (auto &sv : state_vectors_) {
    if (sv->type() == StateVector_type::MULTIVAL) {
      auto mv = std::dynamic_pointer_cast<MultiStateVectorBase<Mesh>>(sv);
      if (mv) mv->rem_material(m);
    }
  }
}

/// Add cells to a material

void State::add_cells_to_material(int m, std::vector<int> const& cells) {
  assert(m < material_cellsets_.size());
  material_cellsets_[m]->add_entities(cells);

  for (auto const& c : cells)
    cell_materials_[c].push_back(m);

  for (auto & sv : state_vectors_) {
    if (sv->type() == StateVector_type::MULTIVAL) {
      auto mv = std::dynamic_pointer_cast<MultiStateVectorBase<Mesh>>(sv);
      int size = mv->size(m);
      mv->resize(m, size+cells.size());
    }
  }
}

/// Remove cells from a material

void State::rem_cells_from_material(int m, std::vector<int> const& cells) {

  // NOT IMPLEMENTED - THERE ARE MAJOR CONSIDERATIONS OF EFFICIENCY
  // IN HOW WE DO THIS - IF WE REMOVE THESE ENTRIES FROM THE LIST
  // THEN WE HAVE TO REMOVE ENTRIES FROM ALL STATE VECTORS FROM
  // CORRESPONDING TO THIS MATERIAL AS WELL. BESIDES MESHSETS DO
  // FUNKY THINGS WITH REMOVING CELLS AND THAT HAS TO BE TAKEN
  // INTO ACCOUNT (See MeshSet::rem_entities)

  throw std::runtime_error("rem_cells_from_material NOT IMPLEMENTED");

}



//! \brief Add a state vectors from the mesh
//! Initialize a state vectors in the statemanager from mesh field data

void State::init_from_mesh() {

  int num;
  std::vector<std::string> varnames, vartypes;

  for (int ikind = 0; ikind < NUM_ENTITY_KINDS; ikind++) {
    Entity_kind kind = (Entity_kind) ikind;
    if (kind != Entity_kind::NODE && kind != Entity_kind::FACE &&
        kind != Entity_kind::CELL) continue;

    mymesh_->get_field_info(kind, &num, &varnames, &vartypes);
    if (!num) continue;

    int spacedim = mymesh_->space_dimension();

    int nent = mymesh_->num_entities(kind, Entity_type::ALL);

    for (int i = 0; i < num; i++) {
      if (vartypes[i] == "INT") {
        int *data = new int[nent];
        mymesh_->get_field(varnames[i], kind, data);
        UniStateVector<int, Mesh> & sv = add(varnames[i], mymesh_,
                                             kind,
                                             Entity_type::ALL, data);
      } else if (vartypes[i] == "DOUBLE") {
        double *data = new double[nent];
        mymesh_->get_field(varnames[i], kind, data);
        UniStateVector<double, Mesh> & sv = add(varnames[i], mymesh_,
                                                kind,
                                                Entity_type::ALL,
                                                data);
      } else if (vartypes[i] == "VECTOR") {
        if (spacedim == 2) {
          std::array<double, 2> *data = new std::array<double, 2>[nent];
          mymesh_->get_field(varnames[i], kind, data);
          UniStateVector<std::array<double, 2>, Mesh> & sv =
              add(varnames[i], mymesh_, kind, Entity_type::ALL, data);
        } else if (spacedim == 3) {
          std::array<double, 3> *data = new std::array<double, 3>[nent];
          mymesh_->get_field(varnames[i], kind, data);
          UniStateVector<std::array<double, 3>, Mesh> & sv =
              add(varnames[i], mymesh_, kind, Entity_type::ALL, data);
        }
      } else if (vartypes[i] == "TENSOR") {  // assumes symmetric tensors
        if (spacedim == 2) {  // lower half & diagonal of 2x2 tensor
          std::array<double, 3> *data = new std::array<double, 3>[nent];
          mymesh_->get_field(varnames[i], kind, data);
          UniStateVector<std::array<double, 3>, Mesh> & sv =
              add(varnames[i], mymesh_, kind, Entity_type::ALL, data);
        } else if (spacedim == 3) {  // lower half & diagonal of 3x3 tensor
          std::array<double, 6> *data = new std::array<double, 6>[nent];
          mymesh_->get_field(varnames[i], kind, data);
          UniStateVector<std::array<double, 6>, Mesh> & sv =
              add(varnames[i], mymesh_, kind, Entity_type::ALL, data);
        }
      }  // TENSOR
    }  // for each field on entity kind
  }  // for each entity kind

}  // init_from_mesh


//! \brief Export field data to mesh
//! Export data from state vectors to mesh fields - Since the statevector is
//! templated, we have to go through case by case to see if the type matches
//! any of the types that the mesh can receive

void State::export_to_mesh() {

  State::const_iterator it = cbegin();

  while (it != cend()) {
    const std::shared_ptr<StateVectorBase> vec = *it;
    std::string name = vec->name();
    Entity_kind entity_kind = vec->entity_kind();
    bool status = false;

    // We only know how to store univalued data with the mesh
    if (vec->type() == StateVector_type::UNIVAL) {
      if (vec->data_type() == typeid(double)) {
        auto svec = std::dynamic_pointer_cast<UniStateVector<double>>(vec);
        status = mymesh_->store_field(name, entity_kind, svec->get_raw_data());
      } else if (vec->data_type() == typeid(int)) {
        auto svec = std::dynamic_pointer_cast<UniStateVector<int>>(vec);
        status = mymesh_->store_field(name, entity_kind, svec->get_raw_data());
      } else if (vec->data_type() == typeid(std::array<double, 2>)) {
        auto svec =
            std::dynamic_pointer_cast<UniStateVector<std::array<double, 2>>>(vec);
        status = mymesh_->store_field(name, entity_kind, svec->get_raw_data());
      } else if (vec->data_type() == typeid(std::array<double, 3>)) {
        auto svec =
            std::dynamic_pointer_cast<UniStateVector<std::array<double, 3>>>(vec);
        status = mymesh_->store_field(name, entity_kind, svec->get_raw_data());
      } else if (vec->data_type() == typeid(std::array<double, 6>)) {
        auto svec =
            std::dynamic_pointer_cast<UniStateVector<std::array<double, 6>>>(vec);
        status = mymesh_->store_field(name, entity_kind, svec->get_raw_data());
      }
    }

    if (!status)
      std::cerr << "Could not export vector " << name << " to mesh file\n";

    ++it;
  }
}


//! Print all state vectors

std::ostream & operator<<(std::ostream & os, State const & s) {
  State::const_iterator it = s.cbegin();
  while (it != s.cend()) {
    const std::shared_ptr<StateVectorBase> vec = *it;
    vec->print(os);
    ++it;
  }
}

}  // namespace Jali
