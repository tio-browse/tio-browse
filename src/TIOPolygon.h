//
// src/TIOPolygon.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOPOLYGON_H_
#define SRC_TIOPOLYGON_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO polygon item
class TIOPolygon : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOPolygon(const std::string &itemName, TIOTreeItem *parent = 0);

  ~TIOPolygon() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Override getArrayData(const std::string&) in TreeItem
  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;
};

#endif  // SRC_TIOPOLYGON_H_
