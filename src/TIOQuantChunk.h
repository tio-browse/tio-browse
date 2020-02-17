//
// src/TIOQuantChunk.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOQUANTCHUNK_H_
#define SRC_TIOQUANTCHUNK_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO quant chunk tree item
class TIOQuantChunk : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOQuantChunk(const std::string &itemName, const int &chunkIndex,
                         TIOTreeItem *parent = 0);

  ~TIOQuantChunk() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;

 private:
  //! Get a DataArray object for a point quant chunk
  //! \param variable[in] variable name -- "Pure"
  //! \param fileID[in] TyphonIO fileID
  //! \param quantID[in] TyphonIO quantID
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param nGhosts[in] Number of ghosts
  //! \return a DataArray object with the requested data
  DataArray *GetPointQuantData(const std::string &variable,
                               const TIO_File_t &fileID,
                               const TIO_Object_t &quantID,
                               const TIO_Data_t &dataType,
                               const TIO_Size_t &nGhosts);

  //! Get a DataArray object for a quad quant chunk -- colinear or noncolinear
  //! \param variable[in] variable name -- "Pure" or "Mixed"
  //! \param fileID[in] TyphonIO fileID
  //! \param quantID[in] TyphonIO quantID
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param nGhosts[in] Number of ghosts
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetQuadQuantData(const std::string &variable,
                              const TIO_File_t &fileID,
                              const TIO_Object_t &quantID,
                              const TIO_Data_t &dataType,
                              const TIO_Size_t &nGhosts,
                              const TIO_Bool_t &isMixed);

  //! Get a DataArray object for a unstructured quant chunk
  //! \param variable[in] variable name -- "Pure" or "Mixed"
  //! \param fileID[in] TyphonIO fileID
  //! \param quantID[in] TyphonIO quantID
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param nGhosts[in] Number of ghosts
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetUnstrQuantData(const std::string &variable,
                               const TIO_File_t &fileID,
                               const TIO_Object_t &quantID,
                               const TIO_Data_t &dataType,
                               const TIO_Size_t &nGhosts,
                               const TIO_Bool_t &isMixed);

  int m_chunkIndex;
};

#endif  // SRC_TIOQUANTCHUNK_H_
