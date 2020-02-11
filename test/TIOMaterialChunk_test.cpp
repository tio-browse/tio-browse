//
// test/TIOMaterialChunk_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOMaterialChunk.h"
#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "test/TIOTreeItemTestFixture.h"

class TIOMaterialChunkTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOMaterialChunkTestFixture, initialization) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* chunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ("Chunk0", chunk->data(0).toString().toStdString());
}

TEST_F(TIOMaterialChunkTestFixture, childCount) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, materialChunk->childCount());
}

TEST_F(TIOMaterialChunkTestFixture, canFetchMore) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(true, materialChunk->canFetchMore());
}

TEST_F(TIOMaterialChunkTestFixture, fetchMore) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, materialChunk->childCount());
  materialChunk->fetchMore();
  EXPECT_EQ(6, materialChunk->childCount());
}

TEST_F(TIOMaterialChunkTestFixture, childValuesColinearChunk) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  std::vector<std::string> childNames = {"Material chunk information",
                                         "Material  :  <Open item for data>",
                                         "Mixed mat  :  <Open item for data>",
                                         "N comp  :  <Open item for data>",
                                         "PTR  :  <Open item for data>",
                                         "Vol frac  :  <Open item for data>"};
  EXPECT_EQ(childNames.size(), materialChunk->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, materialChunk->childCount());
    EXPECT_EQ(childNames[i],
              materialChunk->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialChunkTestFixture, childValuesPointChunk) {
  SetUpMaterial("../data/3d_point_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  std::vector<std::string> childNames = {"Material chunk information",
                                         "Material  :  <Open item for data>"};
  EXPECT_EQ(childNames.size(), materialChunk->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, materialChunk->childCount());
    EXPECT_EQ(childNames[i],
              materialChunk->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialChunkTestFixture, infoValuesColinearChunk) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  ASSERT_LE(1, materialChunk->childCount());
  TIOTreeItem* chunkInfoGroup = materialChunk->child(0);  // info group
  EXPECT_EQ("Material chunk information",
            chunkInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Data type  :  TIO_INT",
                                         "N dims  :  3",
                                         "N I  :  7",
                                         "N J  :  6",
                                         "N K  :  4",
                                         "N ghosts  :  1",
                                         "N mix cell  :  79",
                                         "N mix comp  :  158",
                                         "NC data type  :  TIO_INT",
                                         "PTR data type  :  TIO_INT",
                                         "VF data type  :  TIO_DOUBLE"};
  EXPECT_EQ(infoValues.size(), chunkInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, chunkInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              chunkInfoGroup->child(i)->data(0).toString().toStdString());
  }
}

// TODO(jim): Add test for unstructured material chunk info values

TEST_F(TIOMaterialChunkTestFixture, infoValuesPointChunk) {
  SetUpMaterial("../data/3d_point_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Chunk0", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  ASSERT_LE(1, materialChunk->childCount());
  TIOTreeItem* chunkInfoGroup = materialChunk->child(0);  // info group
  EXPECT_EQ("Material chunk information",
            chunkInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "Data type  :  TIO_INT", "N nodes  :  440", "N L  :  0", "N H  :  439",
      "N ghosts  :  TIO_GHOSTS_NONE"};
  EXPECT_EQ(infoValues.size(), chunkInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, chunkInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              chunkInfoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialChunkTestFixture, dataValuesColinearChunk) {
  SetUpMaterial("../data/3d_chunk_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Material", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  DataArray* dataArray;

  // Material
  dataArray = materialChunk->getArrayData("Material");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> matData = {{0, 1},    {1, 1},    {2, 1},    {3, 1},
                                {5, 2},    {8, 1},    {13, -24}, {21, 1},
                                {34, -36}, {55, -25}, {89, 2},   {144, -76}};
  EXPECT_EQ(168, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = matData.begin(); it != matData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // Mixed materialChunk
  dataArray = materialChunk->getArrayData("Mixed mat");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> mixData = {{0, 3},  {1, 4},  {2, 3},  {3, 4},
                                {5, 3},  {8, 2},  {13, 3}, {21, 3},
                                {34, 2}, {55, 3}, {89, 2}, {144, 1}};
  EXPECT_EQ(158, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = mixData.begin(); it != mixData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // N comp
  dataArray = materialChunk->getArrayData("N comp");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> nCompData = {{0, 2}, {1, 2},  {2, 2},  {3, 2},  {5, 2},
                                  {8, 2}, {13, 2}, {21, 2}, {34, 2}, {55, 2}};
  EXPECT_EQ(79, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = nCompData.begin();
       it != nCompData.end(); ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // PTR
  dataArray = materialChunk->getArrayData("PTR");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> ptrData = {{0, 1},   {1, 3},   {2, 5},   {3, 7},
                                {5, 11},  {8, 17},  {13, 27}, {21, 43},
                                {34, 69}, {55, 111}};
  EXPECT_EQ(79, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = ptrData.begin(); it != ptrData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // Vol frac
  dataArray = materialChunk->getArrayData("Vol frac");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, double> vfData = {
      {0, 0.960634},  {1, 0.039366},  {2, 0.778943},  {3, 0.221057},
      {5, 0.103939},  {8, 0.663882},  {13, 0.309120}, {21, 0.103939},
      {34, 0.192564}, {55, 0.287624}, {89, 0.195262}, {144, 0.591405}};
  EXPECT_EQ(158, dataArray->getArrayLength());
  for (std::map<int, double>::iterator it = vfData.begin(); it != vfData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(it->second, dataArray->toQVariant(it->first).toDouble());
  }
  delete dataArray;
}

// TODO(jim): Add test for unstructured material chunk data values

TEST_F(TIOMaterialChunkTestFixture, dataValuesPointChunk) {
  SetUpMaterial("../data/3d_point_v1.h5", "State000", "mesh", "Material");
  TIOMaterialChunk* materialChunk =
      new TIOMaterialChunk("Material", 0, m_mockTIOTreeItem);
  materialChunk->fetchMore();
  DataArray* dataArray;

  // Material
  dataArray = materialChunk->getArrayData("Material");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> matData = {{0, 1},  {1, 1},   {2, 1},   {3, 2},  {5, 3},
                                {8, 4},  {13, 1},  {21, 5},  {34, 2}, {55, 3},
                                {89, 1}, {144, 3}, {233, 3}, {377, 3}};
  EXPECT_EQ(440, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = matData.begin(); it != matData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;
}
