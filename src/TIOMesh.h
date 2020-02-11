//
// src/TIOMesh.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOMESH_H_
#define SRC_TIOMESH_H_

#include <string>
#include "src/TIOTreeItem.h"

class TIOMesh : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOMesh(const std::string &itemName, TIOTreeItem *parent = 0);

  //! Destructor
  ~TIOMesh() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;

  //! Get the TyphonIO mesh ID associated with this mesh item
  //! \return TyphonIO mesh ID
  TIO_Object_t getMeshID() override;

 private:
  //! Get a DataArray object for a point mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetPointMeshData(const std::string &variable,
                              const TIO_Data_t &coordDataType);

  //! Get a DataArray object for a quad mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetQuadMeshData(const std::string &variable,
                             const TIO_Data_t &coordDataType);

  //! Get a DataArray object for a unstructured mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetUnstrMeshData(const std::string &variable,
                              const TIO_Data_t &graphDataType,
                              const TIO_Data_t &coordDataType);

  TIO_Mesh_t m_meshType;
};

#endif  // SRC_TIOMESH_H_
