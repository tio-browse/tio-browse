//
// src/TIODataItem.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIODATAITEM_H_
#define SRC_TIODATAITEM_H_

#include <string>
#include "src/TIOTreeItem.h"

//! TyphonIO data tree item
class TIODataItem : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as standard string
  //! \param[in] parent is the Qt parent item
  explicit TIODataItem(const std::string& itemName,
                       const std::string& itemValue, TIOTreeItem* parent = 0);

  //! Destructor
  ~TIODataItem() override;

  //! Override canFetchMore from TIOTreeItem
  //! TIODataItem doesn't have children
  //! \return false (TreeData has no children)
  bool canFetchMore() override;

  //! Get the data for a specific column
  //! \return QVariant (can be QString etc.)
  QVariant data(int column) const override;

  //! Override getArrayData in TreeItem
  //! Used to get the array data associated with this variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray* getArrayData() override;

  //! Override hasArrayData in TreeItem
  //! Does this tree item have array data
  //! \return true (TreeData has array data)
  bool hasArrayData() override;

  //! Override hasChildren in TIOTreeItem
  //! TIODataItem doesn't have children
  //! \return false (data item has no children)
  bool hasChildren() override;
};

#endif  // SRC_TIODATAITEM_H_
