//
// src/TIOQuantChunkGroup.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOQUANTCHUNKGROUP_H_
#define SRC_TIOQUANTCHUNKGROUP_H_

#include <typhonio.h>

#include <QVariant>
#include <string>

#include "TIOTreeItem.h"

//! TyphonIO quant chunk group tree item
class TIOQuantChunkGroup : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as a std string
  //! \param[in] parent is the TIOTreeItem that is the parent of this item
  explicit TIOQuantChunkGroup(const std::string &itemName,
                              const TIO_Size_t &nChunks,
                              TIOTreeItem *parent = 0);

  ~TIOQuantChunkGroup() override;

  //! Used by the tree model to fetch child data (from file)
  void fetchMore() override;

  //! Overloads getObjectID to pass request to parent
  //! \return TyphonIO object ID of parent
  TIO_Object_t getObjectID() override;

  //! Overloads getObjectID to pass request to parent
  //! \return TIOTreeItem pointer to parent item
  TIOTreeItem *getParentItem() override;

 private:
  TIO_Size_t m_nChunks;
};

#endif  // SRC_TIOQUANTCHUNKGROUP_H_
