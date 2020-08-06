// test/TIOMeshChunk_test.cpp

// (c) British Crown Owned Copyright 2020/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt

#include "TIOMeshChunk.h"

#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

#include "TIOTreeItemTestFixture.h"

class TIOMeshChunkTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOMeshChunkTestFixture, initialization) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ("Chunk0", meshChunk->data(0).toString().toStdString());
}

TEST_F(TIOMeshChunkTestFixture, childCount) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, meshChunk->childCount());
}

TEST_F(TIOMeshChunkTestFixture, canFetchMore) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(true, meshChunk->canFetchMore());
}

TEST_F(TIOMeshChunkTestFixture, fetchMore) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  EXPECT_EQ(0, meshChunk->childCount());
  meshChunk->fetchMore();
  EXPECT_EQ(4, meshChunk->childCount());
}

TEST_F(TIOMeshChunkTestFixture, childValuesColinearChunk) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  std::vector<std::string> meshChildNames = {
      "Quad mesh chunk information",
      "I coord  :  <Open item for data>",
      "J coord  :  <Open item for data>",
      "K coord  :  <Open item for data>",
  };
  EXPECT_EQ(meshChildNames.size(), meshChunk->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, meshChunk->childCount());
    EXPECT_EQ(meshChildNames[i],
              meshChunk->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMeshChunkTestFixture, childValuesUnstructuredChunk) {
  SetUp("data/ex_unstructured_mesh.h5", "state000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  std::vector<std::string> meshChildNames = {
      "Unstr mesh chunk information",
      "I coord  :  <Open item for data>",
      "J coord  :  <Open item for data>",
      "K coord  :  <Open item for data>",
      "Node IDs  :  <Open item for data>",
      "Cell IDs  :  <Open item for data>",
      "Shapes  :  <Open item for data>",
      "N nodes per shape  :  <Open item for data>",
      "N cells per shape  :  <Open item for data>",
      "Connectivity  :  <Open item for data>",
  };
  EXPECT_EQ(meshChildNames.size(), meshChunk->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, meshChunk->childCount());
    EXPECT_EQ(meshChildNames[i],
              meshChunk->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMeshChunkTestFixture, childValuesPointChunk) {
  SetUp("data/3d_point_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  std::vector<std::string> meshChildNames = {
      "Point mesh chunk information",
      "I coord  :  <Open item for data>",
      "J coord  :  <Open item for data>",
      "K coord  :  <Open item for data>",
  };
  EXPECT_EQ(meshChildNames.size(), meshChunk->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, meshChunk->childCount());
    EXPECT_EQ(meshChildNames[i],
              meshChunk->child(i)->data(0).toString().toStdString());
  }
}

// TODO(jim): Add test to check that 2D mesh has no K coord item

TEST_F(TIOMeshChunkTestFixture, infoValuesColinearChunk) {
  SetUp("data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  ASSERT_LE(1, meshChunk->childCount());
  TIOTreeItem *chunkInfoGroup = meshChunk->child(0);  // info group
  EXPECT_EQ("Quad mesh chunk information",
            chunkInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "Data type  :  TIO_DOUBLE",
      "N dims  :  3",
      "IL  :  0",
      "IH  :  5",
      "JL  :  0",
      "JH  :  4",
      "KL  :  0",
      "KH  :  2",
  };
  EXPECT_EQ(infoValues.size(), chunkInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, chunkInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              chunkInfoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMeshChunkTestFixture, infoValuesUnstructuredChunk) {
  SetUp("data/ex_unstructured_mesh.h5", "state000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  ASSERT_LE(1, meshChunk->childCount());
  TIOTreeItem *chunkInfoGroup = meshChunk->child(0);  // info group
  ASSERT_EQ("Unstr mesh chunk information",
            chunkInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "Data type  :  TIO_DOUBLE",
      "Graph data type  :  TIO_INT",
      "N dims  :  3",
      "N nodes  :  32",
      "N cells  :  7",
      "N shapes  :  1",
      "N connectivity  :  56",
      "N ghost nodes  :  0",
      "N ghost cells  :  0",
      "N ghost shapes  :  0",
      "N ghost connectivity  :  0",
  };
  EXPECT_EQ(infoValues.size(), chunkInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, chunkInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              chunkInfoGroup->child(i)->data(0).toString().toStdString());
  }
}

// TODO(jim): Add test for unstructured mesh with ghosts

TEST_F(TIOMeshChunkTestFixture, infoValuesPointChunk) {
  SetUp("data/3d_point_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  ASSERT_LE(1, meshChunk->childCount());
  TIOTreeItem *chunkInfoGroup = meshChunk->child(0);  // info group
  EXPECT_EQ("Point mesh chunk information",
            chunkInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Data type  :  TIO_DOUBLE",
                                         "N dims  :  3", "NL  :  0",
                                         "NH  :  439", "N ghosts  :  0"};
  EXPECT_EQ(infoValues.size(), chunkInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, chunkInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              chunkInfoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMeshChunkTestFixture, dataValuesColinearChunk) {
  SetUp("data/ex_colinear_mesh.h5", "state000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = meshChunk->getArrayData("I coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> iCoords = {0.0, 1.0, 2.0};
  EXPECT_EQ(iCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < iCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(iCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // J coords
  dataArray = meshChunk->getArrayData("J coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> jCoords = {0.0, 1.0, 2.0, 3.0};
  EXPECT_EQ(jCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < jCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(jCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // K coords
  dataArray = meshChunk->getArrayData("K coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> kCoords = {0.0, 1.0, 2.0, 3.0, 4.0};
  EXPECT_EQ(kCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < kCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(kCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;
}

TEST_F(TIOMeshChunkTestFixture, dataValuesUnstructuredChunk) {
  SetUp("data/ex_unstructured_mesh.h5", "state000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = meshChunk->getArrayData("I coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> iCoords = {0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0,
                                 2.0, 2.0, 2.0, 2.0, 0.0, 1.0, 1.0, 0.0,
                                 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};
  EXPECT_EQ(iCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < iCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(iCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // J coords
  dataArray = meshChunk->getArrayData("J coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> jCoords = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
                                 0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0,
                                 3.0, 3.0, 3.0, 3.0, 0.0, 0.0, 1.0, 1.0,
                                 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0};
  EXPECT_EQ(jCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < jCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(jCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // K coords
  dataArray = meshChunk->getArrayData("K coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> kCoords = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                                 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                                 0.0, 0.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0,
                                 3.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0};
  EXPECT_EQ(kCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < kCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(kCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // Nodes IDs
  dataArray = meshChunk->getArrayData("Node IDs");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> nodeIDs = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                              23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
  EXPECT_EQ(nodeIDs.size(), dataArray->getArrayLength());
  for (int i = 0; i < nodeIDs.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(nodeIDs[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // Cell IDs
  dataArray = meshChunk->getArrayData("Cell IDs");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> cellIDs = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(cellIDs.size(), dataArray->getArrayLength());
  for (int i = 0; i < cellIDs.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(cellIDs[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // Shapes
  dataArray = meshChunk->getArrayData("Shapes");
  ASSERT_NE(nullptr, dataArray);
  std::vector<std::string> shapes = {"TIO_SHAPE_HEX8"};
  EXPECT_EQ(shapes.size(), dataArray->getArrayLength());
  for (int i = 0; i < shapes.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(shapes[i], dataArray->toQVariant(i).toString().toStdString());
  }
  delete dataArray;

  // N nodes per shape
  dataArray = meshChunk->getArrayData("N nodes per shape");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> nodesPerShape = {8};
  EXPECT_EQ(nodesPerShape.size(), dataArray->getArrayLength());
  for (int i = 0; i < nodesPerShape.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(nodesPerShape[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // N cells per shape
  dataArray = meshChunk->getArrayData("N cells per shape");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> cellsPerShape = {7};
  EXPECT_EQ(cellsPerShape.size(), dataArray->getArrayLength());
  for (int i = 0; i < cellsPerShape.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(cellsPerShape[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // Connectivity
  dataArray = meshChunk->getArrayData("Connectivity");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> connectivity = {
      1,  2,  3,  4,  5,  6,  7,  8,  2,  9,  10, 3,  6,  11, 12, 7,  5,  6, 7,
      8,  13, 14, 15, 16, 13, 14, 15, 16, 17, 18, 19, 20, 3,  21, 22, 4,  7, 23,
      24, 8,  21, 25, 26, 22, 23, 27, 28, 24, 25, 29, 30, 26, 27, 31, 32, 28};
  EXPECT_EQ(connectivity.size(), dataArray->getArrayLength());
  for (int i = 0; i < connectivity.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(connectivity[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;
}

TEST_F(TIOMeshChunkTestFixture, dataValuesPointChunk) {
  SetUp("data/3d_point_v1.h5", "State000", "mesh");
  TIOMeshChunk *meshChunk = new TIOMeshChunk("Chunk0", 0, m_mockTIOTreeItem);
  meshChunk->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = meshChunk->getArrayData("I coord");
  ASSERT_NE(nullptr, dataArray);
  ASSERT_EQ(440, dataArray->getArrayLength());
  std::map<int, double> iCoords = {
      {0, -5.0},  {1, -4.0},   {2, -3.0},   {3, -2.0},  {5, 0.0},
      {8, 3.0},   {13, -3.0},  {21, 5.0},   {34, -4.0}, {55, -5.0},
      {89, -4.0}, {144, -4.0}, {233, -3.0}, {377, -2.0}};
  for (std::map<int, double>::iterator it = iCoords.begin();
       it != iCoords.end(); ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(it->second, dataArray->toQVariant(it->first).toDouble());
  }
  delete dataArray;

  // J coords
  dataArray = meshChunk->getArrayData("J coord");
  ASSERT_NE(nullptr, dataArray);
  ASSERT_EQ(440, dataArray->getArrayLength());
  std::map<int, double> jCoords = {{0, 0.0},   {1, 0.0},  {2, 0.0},  {3, 0.0},
                                   {5, 0.0},   {8, 0.0},  {13, 1.0}, {21, 1.0},
                                   {34, 3.0},  {55, 5.0}, {89, 0.0}, {144, 5.0},
                                   {233, 5.0}, {377, 2.0}};
  for (std::map<int, double>::iterator it = jCoords.begin();
       it != jCoords.end(); ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(it->second, dataArray->toQVariant(it->first).toDouble());
  }
  delete dataArray;

  // K coords
  dataArray = meshChunk->getArrayData("K coord");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(440, dataArray->getArrayLength());
  std::map<int, double> kCoords = {{0, 0.0},   {1, 0.0},  {2, 0.0},  {3, 0.0},
                                   {5, 0.0},   {8, 0.0},  {13, 0.0}, {21, 0.0},
                                   {34, 0.0},  {55, 0.0}, {89, 1.0}, {144, 1.0},
                                   {233, 2.0}, {377, 4.0}};
  for (std::map<int, double>::iterator it = kCoords.begin();
       it != kCoords.end(); ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(it->second, dataArray->toQVariant(it->first).toDouble());
  }
  delete dataArray;
}
