//
// src/TIOMaterialChunk.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOMATERIALCHUNK_H_
#define SRC_TIOMATERIALCHUNK_H_

#include <string>
#include "src/TIOTreeItem.h"

//! TyphonIO material chunk tree item
class TIOMaterialChunk : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOMaterialChunk(const std::string &itemName, const int &chunkIndex,
                            TIOTreeItem *parent = 0);

  ~TIOMaterialChunk() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;

 private:
  //! Get a DataArray object for a point quant
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \return a DataArray object with the requested data
  DataArray *GetPointMaterialChunkData(const std::string &variable,
                                       const TIO_Data_t &dataType,
                                       const TIO_Size_t &nGhosts);

  //! Get a DataArray object for a quad quant -- colinear or noncolinear
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param ncompDataType[in] TyphonIO TIO_Data_t item
  //! \param ptrDataType[in] TyphonIO TIO_Data_t item
  //! \param vfDataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetQuadMaterialChunkData(const std::string &variable,
                                      const TIO_Data_t &dataType,
                                      const TIO_Data_t &ncompDataType,
                                      const TIO_Data_t &ptrDataType,
                                      const TIO_Data_t &vfDataType,
                                      const TIO_Size_t &nGhosts,
                                      const TIO_Bool_t &isMixed);

  //! Get a DataArray object for a unstructured quant
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param ncompDataType[in] TyphonIO TIO_Data_t item
  //! \param ptrDataType[in] TyphonIO TIO_Data_t item
  //! \param vfDataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetUnstrMaterialChunkData(const std::string &variable,
                                       const TIO_Data_t &dataType,
                                       const TIO_Data_t &ncompDataType,
                                       const TIO_Data_t &ptrDataType,
                                       const TIO_Data_t &vfDataType,
                                       const TIO_Size_t &nGhosts,
                                       const TIO_Bool_t &isMixed);
  int m_chunkIndex;
};

#endif  // SRC_TIOMATERIALCHUNK_H_
