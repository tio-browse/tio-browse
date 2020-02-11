//
// src/TIOInterface.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOINTERFACE_H_
#define SRC_TIOINTERFACE_H_

#include <typhonio.h>
#include <string>
#include "src/TIOTreeItem.h"

//! TyphonIO interface tree item
class TIOInterface : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOInterface(const std::string& itemName, TIOTreeItem* parent = 0);

  ~TIOInterface() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;
};

#endif  // SRC_TIOINTERFACE_H_
