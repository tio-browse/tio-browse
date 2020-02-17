//
// src/TIOState.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOSTATE_H_
#define SRC_TIOSTATE_H_

#include <typhonio.h>

#include <string>

#include "TIOTreeItem.h"

class TIOState : public TIOTreeItem {
 public:
  explicit TIOState(const std::string &itemName, TIOTreeItem *parent = 0);

  // Destructor
  ~TIOState() override;

  //! Used to ask the class instance to fetch more data
  //! Used by the model to request that data is fetched from a TyphonIO file
  void fetchMore() override;
};

#endif  // SRC_TIOSTATE_H_
