//
// src/TIOMaterial.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOMATERIAL_H_
#define SRC_TIOMATERIAL_H_

#include <typhonio.h>

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO material tree item
class TIOMaterial : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOMaterial(const std::string &itemName, TIOTreeItem *parent = 0);

  ~TIOMaterial() override;

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
  DataArray *GetPointMaterialData(const std::string &variable,
                                  const TIO_Data_t &dataType);

  //! Get a DataArray object for a quad quant -- colinear or noncolinear
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param ncompDataType[in] TyphonIO TIO_Data_t item
  //! \param ptrDataType[in] TyphonIO TIO_Data_t item
  //! \param vfDataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetQuadMaterialData(const std::string &variable,
                                 const TIO_Data_t &dataType,
                                 const TIO_Data_t &ncompDataType,
                                 const TIO_Data_t &ptrDataType,
                                 const TIO_Data_t &vfDataType,
                                 const TIO_Bool_t &isMixed);

  //! Get a DataArray object for a unstructured quant
  //! \param variable[in] variable name
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param ncompDataType[in] TyphonIO TIO_Data_t item
  //! \param ptrDataType[in] TyphonIO TIO_Data_t item
  //! \param vfDataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetUnstrMaterialData(const std::string &variable,
                                  const TIO_Data_t &dataType,
                                  const TIO_Data_t &ncompDataType,
                                  const TIO_Data_t &ptrDataType,
                                  const TIO_Data_t &vfDataType,
                                  const TIO_Bool_t &isMixed);
};

#endif  // SRC_TIOMATERIAL_H_
