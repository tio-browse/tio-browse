//
// src/TIOQuantGroup.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOQUANTGROUP_H_
#define SRC_TIOQUANTGROUP_H_

#include <typhonio.h>

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO quant group tree item
class TIOQuantGroup : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOQuantGroup(const std::string &itemName, TIOTreeItem *parent = 0);

  ~TIOQuantGroup() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Overloads getObjectID to pass request to parent
  //! \return TyphonIO object ID of parent
  TIO_Object_t getObjectID() override;
};

#endif  // SRC_TIOQUANTGROUP_H_
