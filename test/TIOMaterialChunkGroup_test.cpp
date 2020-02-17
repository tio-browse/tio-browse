//
// test/TIOMaterialChunkGroup_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOMaterialChunkGroup.h"

#include <gtest/gtest.h>

#include "test/TIOTreeItemTestFixture.h"

class TIOMaterialChunkGroupTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOMaterialChunkGroupTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterialChunkGroup chunkGroup("Chunks", 8, m_mockTIOTreeItem);
  EXPECT_EQ("Chunks", chunkGroup.data(0).toString().toStdString());
}

TEST_F(TIOMaterialChunkGroupTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterialChunkGroup chunkGroup("Chunks", 8, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunkGroup.childCount());
}

TEST_F(TIOMaterialChunkGroupTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterialChunkGroup chunkGroup("Chunks", 8, m_mockTIOTreeItem);
  EXPECT_EQ(true, chunkGroup.canFetchMore());
}

TEST_F(TIOMaterialChunkGroupTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterialChunkGroup chunkGroup("Chunks", 8, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunkGroup.childCount());
  chunkGroup.fetchMore();
  EXPECT_EQ(8, chunkGroup.childCount());
}

TEST_F(TIOMaterialChunkGroupTestFixture, childValues) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterialChunkGroup chunkGroup("Chunks", 8, m_mockTIOTreeItem);
  chunkGroup.fetchMore();
  EXPECT_EQ(8, chunkGroup.childCount());
  for (int i = 0; i < 8; ++i) {
    ASSERT_LT(i, chunkGroup.childCount());
    EXPECT_EQ("Chunk" + std::to_string(i),
              chunkGroup.child(i)->data(0).toString().toStdString());
  }
}
