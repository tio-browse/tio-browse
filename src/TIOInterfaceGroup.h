//
// src/TIOInterfaceGroup.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOINTERFACEGROUP_H_
#define SRC_TIOINTERFACEGROUP_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO interface group tree item
class TIOInterfaceGroup : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOInterfaceGroup(const std::string &itemName,
                             TIOTreeItem *parent = 0);

  ~TIOInterfaceGroup() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Overloads getObjectID to pass request to parent
  //! \return TyphonIO object ID of parent
  TIO_Object_t getObjectID() override;
};

#endif  // SRC_TIOINTERFACEGROUP_H_
