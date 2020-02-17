//
// src/TIOInfoItem.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOINFOITEM_H_
#define SRC_TIOINFOITEM_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO info tree item
class TIOInfoItem : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as standard string
  //! \param[in] parent is the Qt parent item
  TIOInfoItem(const std::string &itemName, const std::string &itemValue,
              TIOTreeItem *parent = 0);

  // Destructor
  ~TIOInfoItem() override;

  //! Can this instance fetch more data (from file)
  //! Overrides method in TIOTreeItem
  //! \return boolean if this instance can fetch more data
  bool canFetchMore() override;

  //! Get the data for a specific column
  //! Overrides method in TIOTreeItem
  //! \return QVariant (can be QString etc.)
  QVariant data(int column) const;

  //! Does this instance have children
  //! Overrides method in TIOTreeItem
  //! \return boolean if this item has children
  bool hasChildren() override;

 private:
  std::string m_itemValue;
};

#endif  // SRC_TIOINFOITEM_H_
