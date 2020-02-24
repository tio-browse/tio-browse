//
// test/TIOMesh_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOMesh.h"

#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

#include "TIOException.h"
#include "TIOTreeItemTestFixture.h"

class TIOMeshTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOMeshTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  EXPECT_EQ("mesh", mesh->data(0).toString().toStdString());
  delete mesh;
}

TEST_F(TIOMeshTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  EXPECT_EQ(0, mesh->childCount());
  delete mesh;
}

TEST_F(TIOMeshTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  EXPECT_EQ(true, mesh->canFetchMore());
  delete mesh;
}

TEST_F(TIOMeshTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  EXPECT_EQ(0, mesh->childCount());
  mesh->fetchMore();
  EXPECT_EQ(7, mesh->childCount());
  delete mesh;
}

TEST_F(TIOMeshTestFixture, fetchMoreNonexistentMesh) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh mesh("nonexistent mesh", m_mockTIOTreeItem);
  try {
    mesh.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (const TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOMeshTestFixture, childValuesColinearMesh) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  std::vector<std::string> meshChildNames = {"Mesh information",
                                             "I coord  :  <Open item for data>",
                                             "J coord  :  <Open item for data>",
                                             "K coord  :  <Open item for data>",
                                             "Chunks",
                                             "Material",
                                             "Quantities"};
  EXPECT_EQ(meshChildNames.size(), mesh->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, mesh->childCount());
    EXPECT_EQ(meshChildNames[i],
              mesh->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, childValuesUnstructuredMesh) {
  SetUp("../data/ex_unstructured_mesh.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  std::vector<std::string> meshChildNames = {
      "Mesh information",
      "I coord  :  <Open item for data>",
      "J coord  :  <Open item for data>",
      "K coord  :  <Open item for data>",
      "Node IDs  :  <Open item for data>",
      "Cell IDs  :  <Open item for data>",
      "Shapes  :  <Open item for data>",
      "N nodes per shape  :  <Open item for data>",
      "N cells per shape  :  <Open item for data>",
      "Connectivity  :  <Open item for data>",
      "Chunks",
      "Quantities"};
  EXPECT_EQ(meshChildNames.size(), mesh->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, mesh->childCount());
    EXPECT_EQ(meshChildNames[i],
              mesh->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, childValuesPointMesh) {
  SetUp("../data/3d_point_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  std::vector<std::string> meshChildNames = {"Mesh information",
                                             "I coord  :  <Open item for data>",
                                             "J coord  :  <Open item for data>",
                                             "K coord  :  <Open item for data>",
                                             "Chunks",
                                             "Material",
                                             "Quantities"};
  EXPECT_EQ(meshChildNames.size(), mesh->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, mesh->childCount());
    EXPECT_EQ(meshChildNames[i],
              mesh->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, childValuesNoMaterial) {
  SetUp("../data/ex_colinear_mesh.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  std::vector<std::string> meshChildNames = {"Mesh information",
                                             "I coord  :  <Open item for data>",
                                             "J coord  :  <Open item for data>",
                                             "K coord  :  <Open item for data>",
                                             "Chunks",
                                             "Quantities"};
  EXPECT_EQ(meshChildNames.size(), mesh->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, mesh->childCount());
    EXPECT_EQ(meshChildNames[i],
              mesh->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, childValuesVariablesAndVargroups) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  std::vector<std::string> meshChildNames = {"Mesh information",
                                             "I coord  :  <Open item for data>",
                                             "J coord  :  <Open item for data>",
                                             "K coord  :  <Open item for data>",
                                             "Chunks",
                                             "material",
                                             "Quantities",
                                             "year",
                                             "calendar info"};
  EXPECT_EQ(meshChildNames.size(), mesh->childCount());
  for (int i = 0; i < meshChildNames.size(); ++i) {
    ASSERT_LT(i, mesh->childCount());
    EXPECT_EQ(meshChildNames[i],
              mesh->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

// TODO(jim): Add test to check that mesh with no quantities does not have a
// TODO(jim): ... quant group

// TODO(jim): Add test to check that 2D mesh has no K coord item

TEST_F(TIOMeshTestFixture, infoValuesColinearMesh) {
  SetUp("../data/3d_chunk_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  ASSERT_LE(1, mesh->childCount());
  TIOTreeItem *meshInfoGroup = mesh->child(0);  // "Mesh Information"
  EXPECT_EQ("Mesh information",
            meshInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Mesh type  :  TIO_MESH_QUAD_COLINEAR",
                                         "Coord type  :  TIO_COORD_CARTESIAN",
                                         "Is AMR  :  TIO_FALSE",
                                         "Group  :  mesh",
                                         "Order  :  1",
                                         "Data type  :  TIO_DOUBLE",
                                         "Graph data type  :  TIO_INT",
                                         "N dims  :  3",
                                         "N1  :  11",
                                         "N2  :  8",
                                         "N3  :  5",
                                         "I label  :  x axis",
                                         "J label  :  y axis",
                                         "K label  :  z axis",
                                         "I units  :  xunits",
                                         "J units  :  yunits",
                                         "K units  :  zunits",
                                         "IL  :  Not in file",
                                         "IH  :  Not in file",
                                         "JL  :  Not in file",
                                         "JH  :  Not in file",
                                         "KL  :  Not in file",
                                         "KH  :  Not in file",
                                         "N chunks  :  8"};
  EXPECT_EQ(infoValues.size(), meshInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, meshInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              meshInfoGroup->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, infoValuesUnstructuredMesh) {
  SetUp("../data/ex_unstructured_mesh.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  EXPECT_EQ(12, mesh->childCount());
  ASSERT_LE(1, mesh->childCount());
  TIOTreeItem *meshInfoGroup = mesh->child(0);  // "Mesh Information"
  EXPECT_EQ("Mesh information",
            meshInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Mesh type  :  TIO_MESH_UNSTRUCT",
                                         "Coord type  :  TIO_COORD_CARTESIAN",
                                         "Is AMR  :  TIO_FALSE",
                                         "Group  :  mesh",
                                         "Order  :  1",
                                         "Data type  :  TIO_DOUBLE",
                                         "Graph data type  :  TIO_INT",
                                         "N dims  :  3",
                                         "N1  :  32",
                                         "N2  :  7",
                                         "N3  :  1",
                                         "N4  :  56",
                                         "I label  :  x",
                                         "J label  :  y",
                                         "K label  :  z",
                                         "I units  :  metres",
                                         "J units  :  metres",
                                         "K units  :  metres",
                                         "IL  :  Not in file",
                                         "IH  :  Not in file",
                                         "JL  :  Not in file",
                                         "JH  :  Not in file",
                                         "KL  :  Not in file",
                                         "KH  :  Not in file",
                                         "N chunks  :  1"};
  EXPECT_EQ(infoValues.size(), meshInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, meshInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              meshInfoGroup->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, infoValuesPointMesh) {
  SetUp("../data/3d_point_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  EXPECT_EQ(7, mesh->childCount());
  ASSERT_LE(1, mesh->childCount());
  TIOTreeItem *meshInfoGroup = mesh->child(0);  // "Mesh Information"
  EXPECT_EQ("Mesh information",
            meshInfoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Mesh type  :  TIO_MESH_POINT",
                                         "Coord type  :  TIO_COORD_CARTESIAN",
                                         "Is AMR  :  TIO_FALSE",
                                         "Group  :  mesh",
                                         "Order  :  1",
                                         "Data type  :  TIO_DOUBLE",
                                         "Graph data type  :  TIO_INT",
                                         "N dims  :  3",
                                         "N1  :  440",
                                         "I label  :  x axis",
                                         "J label  :  y axis",
                                         "K label  :  z axis",
                                         "I units  :  xunits",
                                         "J units  :  yunits",
                                         "K units  :  zunits",
                                         "IL  :  Not in file",
                                         "IH  :  Not in file",
                                         "JL  :  Not in file",
                                         "JH  :  Not in file",
                                         "KL  :  Not in file",
                                         "KH  :  Not in file",
                                         "N chunks  :  1"};
  EXPECT_EQ(infoValues.size(), meshInfoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, meshInfoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              meshInfoGroup->child(i)->data(0).toString().toStdString());
  }
  delete mesh;
}

TEST_F(TIOMeshTestFixture, dataValuesColinearMesh) {
  SetUp("../data/ex_colinear_mesh.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = mesh->getArrayData("I coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> iCoords = {0.0, 1.0, 2.0};
  EXPECT_EQ(iCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < iCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(iCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // J coords
  dataArray = mesh->getArrayData("J coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> jCoords = {0.0, 1.0, 2.0, 3.0};
  EXPECT_EQ(jCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < jCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(jCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  // K coords
  dataArray = mesh->getArrayData("K coord");
  ASSERT_NE(nullptr, dataArray);
  std::vector<double> kCoords = {0.0, 1.0, 2.0, 3.0, 4.0};
  EXPECT_EQ(kCoords.size(), dataArray->getArrayLength());
  for (int i = 0; i < kCoords.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(kCoords[i], dataArray->toQVariant(i).toDouble());
  }
  delete dataArray;

  delete mesh;
}

TEST_F(TIOMeshTestFixture, dataValuesUnstructuredMesh) {
  SetUp("../data/ex_unstructured_mesh.h5", "state000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = mesh->getArrayData("I coord");
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
  dataArray = mesh->getArrayData("J coord");
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
  dataArray = mesh->getArrayData("K coord");
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
  dataArray = mesh->getArrayData("Node IDs");
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
  dataArray = mesh->getArrayData("Cell IDs");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> cellIDs = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(cellIDs.size(), dataArray->getArrayLength());
  for (int i = 0; i < cellIDs.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(cellIDs[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // Shapes
  dataArray = mesh->getArrayData("Shapes");
  ASSERT_NE(nullptr, dataArray);
  std::vector<std::string> shapes = {"TIO_SHAPE_HEX8"};
  EXPECT_EQ(shapes.size(), dataArray->getArrayLength());
  for (int i = 0; i < shapes.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(shapes[i], dataArray->toQVariant(i).toString().toStdString());
  }
  delete dataArray;

  // N nodes per shape
  dataArray = mesh->getArrayData("N nodes per shape");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> nodesPerShape = {8};
  EXPECT_EQ(nodesPerShape.size(), dataArray->getArrayLength());
  for (int i = 0; i < nodesPerShape.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(nodesPerShape[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // N cells per shape
  dataArray = mesh->getArrayData("N cells per shape");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> cellsPerShape = {7};
  EXPECT_EQ(cellsPerShape.size(), dataArray->getArrayLength());
  for (int i = 0; i < cellsPerShape.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(cellsPerShape[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;

  // Connectivity
  dataArray = mesh->getArrayData("Connectivity");
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

  delete mesh;
}

TEST_F(TIOMeshTestFixture, dataValuesPointMesh) {
  SetUp("../data/3d_point_v1.h5", "State000");
  TIOMesh *mesh = new TIOMesh("mesh", m_mockTIOTreeItem);
  mesh->fetchMore();
  DataArray *dataArray;

  // I coords
  dataArray = mesh->getArrayData("I coord");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(440, dataArray->getArrayLength());
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
  dataArray = mesh->getArrayData("J coord");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(440, dataArray->getArrayLength());
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
  dataArray = mesh->getArrayData("K coord");
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

  delete mesh;
}
