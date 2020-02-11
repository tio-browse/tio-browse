//
// src/TIOMeshChunk.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOMESHCHUNK_H_
#define SRC_TIOMESHCHUNK_H_

#include <typhonio.h>
#include <QVariant>
#include <string>
#include "src/TIOTreeItem.h"

//! TyphonIO chunk tree item
class TIOMeshChunk : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOMeshChunk(const std::string &itemName, const int &chunkIndex,
                        TIOTreeItem *parent = 0);

  ~TIOMeshChunk() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;

 private:
  //! Get a DataArray object for a point mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetPointMeshChunkData(const std::string &variable,
                                   const TIO_Data_t &coordDataType);

  //! Get a DataArray object for a quad mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetQuadMeshChunkData(const std::string &variable,
                                  const TIO_Data_t &coordDataType);

  //! Get a DataArray object for a unstructured mesh
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param variable[in] variable name
  DataArray *GetUnstrMeshChunkData(const std::string &variable,
                                   const TIO_Data_t &graphDataType,
                                   const TIO_Data_t &coordDataType);

  int m_chunkIndex;
};

#endif  // SRC_TIOMESHCHUNK_H_
