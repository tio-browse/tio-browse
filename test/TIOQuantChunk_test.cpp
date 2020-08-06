// test/TIOQuantChunk_test.cpp

// (c) British Crown Owned Copyright 2020/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt

#include "TIOQuantChunk.h"

#include <string>
#include <vector>

#include "TIOTreeItemTestFixture.h"

class TIOQuantChunkTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOQuantChunkTestFixture, initialization) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ("Chunk0", chunk->data(0).toString().toStdString());
}

TEST_F(TIOQuantChunkTestFixture, childCount) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunk->childCount());
}

TEST_F(TIOQuantChunkTestFixture, canFetchMore) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(true, chunk->canFetchMore());
}

TEST_F(TIOQuantChunkTestFixture, quadQuantChunkFetchMore) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunk->childCount());
  chunk->fetchMore();
  EXPECT_EQ(3, chunk->childCount());
}

TEST_F(TIOQuantChunkTestFixture, pointQuantChunkFetchMore) {
  SetUpQuant("data/3d_point_v1.h5", "State000", "mesh", "X-Velocity");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, chunk->childCount());
  chunk->fetchMore();
  EXPECT_EQ(2, chunk->childCount());
}

TEST_F(TIOQuantChunkTestFixture, quadQuantChunkChildNames) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  ASSERT_EQ(3, chunk->childCount());
  EXPECT_EQ("Quant chunk information",
            chunk->child(0)->data(0).toString().toStdString());
  EXPECT_EQ("Pure  :  <Open item for data>",
            chunk->child(1)->data(0).toString().toStdString());
  EXPECT_EQ("Mixed  :  None",
            chunk->child(2)->data(0).toString().toStdString());
}

TEST_F(TIOQuantChunkTestFixture, pointQuantChunkChildNames) {
  SetUpQuant("data/3d_point_v1.h5", "State000", "mesh", "X-Velocity");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  ASSERT_EQ(2, chunk->childCount());
  EXPECT_EQ("Quant chunk information",
            chunk->child(0)->data(0).toString().toStdString());
  EXPECT_EQ("Pure  :  <Open item for data>",
            chunk->child(1)->data(0).toString().toStdString());
}

TEST_F(TIOQuantChunkTestFixture, infoValuesColinearMesh) {
  SetUpQuant("data/3d_chunk_v1.h5", "State000", "mesh", "Density");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  ASSERT_EQ(3, chunk->childCount());
  // Quant chunk info
  TIOTreeItem *chunkInfo = chunk->child(0);
  std::vector<std::string> infoValues = {"Data type  :  TIO_DOUBLE",
                                         "N dims  :  3",
                                         "N I  :  7",
                                         "N J  :  6",
                                         "N K  :  4",
                                         "N ghosts  :  1",
                                         "N mix comp  :  0"};
  ASSERT_EQ(infoValues.size(), chunkInfo->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    EXPECT_EQ(infoValues[i],
              chunkInfo->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantChunkTestFixture, infoValuesUnstrMesh) {
  SetUpQuant("data/ex_unstructured_mesh.h5", "state000", "mesh",
             "N neighbours");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  ASSERT_EQ(3, chunk->childCount());
  // Quant chunk info
  TIOTreeItem *chunkInfo = chunk->child(0);
  std::vector<std::string> infoValues = {"Data type  :  TIO_INT", "N N  :  7",
                                         "N ghosts  :  0", "N mix comp  :  0"};
  ASSERT_EQ(infoValues.size(), chunkInfo->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    EXPECT_EQ(infoValues[i],
              chunkInfo->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantChunkTestFixture, infoValuesPointMesh) {
  SetUpQuant("data/3d_point_v1.h5", "State000", "mesh", "X-Velocity");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  ASSERT_EQ(2, chunk->childCount());
  // Quant chunk info
  TIOTreeItem *chunkInfo = chunk->child(0);
  std::vector<std::string> infoValues = {"Data type  :  TIO_DOUBLE",
                                         "N nodes  :  440", "N L  :  0",
                                         "N H  :  439", "N ghosts  :  0"};
  ASSERT_EQ(infoValues.size(), chunkInfo->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    EXPECT_EQ(infoValues[i],
              chunkInfo->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantChunkTestFixture, quadQuantPureData) {
  SetUpQuant("data/ex_colinear_mesh.h5", "state000", "mesh", "sum_x_y_z");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_EQ(3, chunk->childCount());
  TIOTreeItem *pureDataItem = chunk->child(1);
  ASSERT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = chunk->getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with a std::map and loop
  ASSERT_EQ(24, dataArray->getArrayLength());
  EXPECT_EQ(0, dataArray->toQVariant(0).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(1).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(2).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(3).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(4).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(5).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(6).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(7).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(8).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(9).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(10).toInt());
  EXPECT_EQ(4, dataArray->toQVariant(11).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(12).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(13).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(14).toInt());
  EXPECT_EQ(4, dataArray->toQVariant(15).toInt());
  EXPECT_EQ(4, dataArray->toQVariant(16).toInt());
  EXPECT_EQ(5, dataArray->toQVariant(17).toInt());
  EXPECT_EQ(3, dataArray->toQVariant(18).toInt());
  EXPECT_EQ(4, dataArray->toQVariant(19).toInt());
  EXPECT_EQ(4, dataArray->toQVariant(20).toInt());
  EXPECT_EQ(5, dataArray->toQVariant(21).toInt());
  EXPECT_EQ(5, dataArray->toQVariant(22).toInt());
  EXPECT_EQ(6, dataArray->toQVariant(23).toInt());
}

TEST_F(TIOQuantChunkTestFixture, unstrQuantPureData) {
  SetUpQuant("data/ex_unstructured_mesh.h5", "state000", "mesh",
             "N neighbours");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_EQ(3, chunk->childCount());
  TIOTreeItem *pureDataItem = chunk->child(1);
  ASSERT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = chunk->getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with a std::map and loop
  ASSERT_EQ(7, dataArray->getArrayLength());
  EXPECT_EQ(3, dataArray->toQVariant(0).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(1).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(2).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(3).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(4).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(5).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(6).toInt());
}

TEST_F(TIOQuantChunkTestFixture, pointQuantPureData) {
  SetUpQuant("data/3d_point_v1.h5", "State000", "mesh", "X-Velocity");
  TIOQuantChunk *chunk = new TIOQuantChunk("Chunk0", 0, m_mockTIOTreeItem);
  chunk->fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_EQ(2, chunk->childCount());
  TIOTreeItem *pureDataItem = chunk->child(1);
  ASSERT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = chunk->getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with a std::map and loop
  ASSERT_EQ(440, dataArray->getArrayLength());
  EXPECT_DOUBLE_EQ(0.0, dataArray->toQVariant(0).toDouble());
  EXPECT_DOUBLE_EQ(1.0, dataArray->toQVariant(1).toDouble());
  EXPECT_DOUBLE_EQ(2.0, dataArray->toQVariant(2).toDouble());
  EXPECT_DOUBLE_EQ(3.0, dataArray->toQVariant(3).toDouble());
  EXPECT_DOUBLE_EQ(5.0, dataArray->toQVariant(5).toDouble());
  EXPECT_DOUBLE_EQ(8.0, dataArray->toQVariant(8).toDouble());
  EXPECT_DOUBLE_EQ(2.236068, dataArray->toQVariant(13).toDouble());
  EXPECT_DOUBLE_EQ(10.049875, dataArray->toQVariant(21).toDouble());
  EXPECT_DOUBLE_EQ(3.162278, dataArray->toQVariant(34).toDouble());
  EXPECT_DOUBLE_EQ(5.000000, dataArray->toQVariant(55).toDouble());
  EXPECT_DOUBLE_EQ(1.414214, dataArray->toQVariant(89).toDouble());
  EXPECT_DOUBLE_EQ(5.196152, dataArray->toQVariant(144).toDouble());
  EXPECT_DOUBLE_EQ(5.744563, dataArray->toQVariant(233).toDouble());
  EXPECT_DOUBLE_EQ(5.385165, dataArray->toQVariant(377).toDouble());
}

// TODO(jim): Add test for file with mixed cell data
