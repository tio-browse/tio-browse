//
// src/TIORootItem.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//
//

#ifndef SRC_TIOROOTITEM_H_
#define SRC_TIOROOTITEM_H_

#include <string>

#include "TIOTreeItem.h"

class TIORootItem : public TIOTreeItem {
 public:
  explicit TIORootItem(const std::string &itemName, TIOTreeItem *parent = 0);
  ~TIORootItem() override;
  void OpenFile(const std::string &fileName);

  TIOTreeItem *child(int row);
};

#endif  // SRC_TIOROOTITEM_H_
