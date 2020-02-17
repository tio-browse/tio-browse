//
// test/TIOQuantChunkGroup.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuantChunkGroup.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "test/TIOTreeItemTestFixture.h"

class TIOQuantChunkGroupTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOQuantChunkGroupTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantChunkGroup *chunkGroup =
      new TIOQuantChunkGroup("Density", 8, m_mockTIOTreeItem);
  EXPECT_EQ("Density", chunkGroup->data(0).toString().toStdString());
}

TEST_F(TIOQuantChunkGroupTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantChunkGroup *chunkGroup =
      new TIOQuantChunkGroup("Density", 8, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunkGroup->childCount());
}

TEST_F(TIOQuantChunkGroupTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantChunkGroup *chunkGroup =
      new TIOQuantChunkGroup("Density", 8, m_mockTIOTreeItem);
  EXPECT_EQ(true, chunkGroup->canFetchMore());
}

TEST_F(TIOQuantChunkGroupTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantChunkGroup *chunkGroup =
      new TIOQuantChunkGroup("Density", 8, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunkGroup->childCount());
  chunkGroup->fetchMore();
  EXPECT_EQ(8, chunkGroup->childCount());
}

TEST_F(TIOQuantChunkGroupTestFixture, childNames) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantChunkGroup *chunkGroup =
      new TIOQuantChunkGroup("Density", 8, m_mockTIOTreeItem);
  chunkGroup->fetchMore();
  ASSERT_EQ(8, chunkGroup->childCount());

  for (auto i = 0; i < 8; ++i) {
    EXPECT_EQ("Chunk" + std::to_string(i),
              chunkGroup->child(i)->data(0).toString().toStdString());
  }
}
