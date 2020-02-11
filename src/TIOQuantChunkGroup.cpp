//
// src/TIOQuantChunkGroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuantChunkGroup.h"
#include <memory>
#include <string>
#include "src/TIOQuantChunk.h"

TIOQuantChunkGroup::TIOQuantChunkGroup(const std::string& itemName,
                                       const TIO_Size_t& nChunks,
                                       TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent), m_nChunks(nChunks) {}

TIOQuantChunkGroup::~TIOQuantChunkGroup() { m_childItems.clear(); }

void TIOQuantChunkGroup::fetchMore() {
  TIOTreeItem::fetchMore();

  // Add chunk items to tree structure
  for (TIO_Size_t i = 0; i < m_nChunks; ++i) {
    m_childItems.push_back(std::unique_ptr<TIOQuantChunk>(
        new TIOQuantChunk("Chunk" + std::to_string(i), i, this)));
  }
}

TIO_Object_t TIOQuantChunkGroup::getObjectID() {
  return m_parentItem->getObjectID();
}

TIOTreeItem* TIOQuantChunkGroup::getParentItem() {
  return m_parentItem->getParentItem();
}
