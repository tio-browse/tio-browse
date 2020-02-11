//
// src/TIOVargroup.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOVARGROUP_H_
#define SRC_TIOVARGROUP_H_

#include <string>
#include "src/TIOTreeItem.h"

class TIOVargroup : public TIOTreeItem {
 public:
  explicit TIOVargroup(const std::string &itemName, TIOTreeItem *parent = 0);
  ~TIOVargroup() override;

  //! Used by the tree model to populate child data (from file)
  void fetchMore() override;
};

#endif  // SRC_TIOVARGROUP_H_
