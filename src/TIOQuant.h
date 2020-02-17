//
// src/TIOQuant.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOQUANT_H_
#define SRC_TIOQUANT_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO quant tree item
class TIOQuant : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as standard string
  //! \param[in] parent is the Qt parent item
  explicit TIOQuant(const std::string &itemName, TIOTreeItem *parent = 0);

  ~TIOQuant() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;

 private:
  //! Get a DataArray object for a point quant
  //! \param variable[in] variable name -- "Pure"
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \return a DataArray object with the requested data
  DataArray *GetPointQuantData(const std::string &variable,
                               const TIO_Data_t &dataType);

  //! Get a DataArray object for a quad quant -- colinear or noncolinear
  //! \param variable[in] variable name -- "Pure" or "Mixed"
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetQuadQuantData(const std::string &variable,
                              const TIO_Data_t &dataType,
                              const TIO_Bool_t &isMixed);

  //! Get a DataArray object for a unstructured quant
  //! \param variable[in] variable name -- "Pure" or "Mixed"
  //! \param dataType[in] TyphonIO TIO_Data_t item
  //! \param isMixed[in] Whether the variable has mix data
  //! \return a DataArray object with the requested data
  DataArray *GetUnstrQuantData(const std::string &variable,
                               const TIO_Data_t &dataType,
                               const TIO_Bool_t &isMixed);
};

#endif  // SRC_TIOQUANT_H_
