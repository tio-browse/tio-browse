//
// test/TIOQuant_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuant.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "src/TIOException.h"
#include "src/TIOTreeItem.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOQuantTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOQuantTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  EXPECT_EQ("Density", quant.data(0).toString().toStdString());
}

TEST_F(TIOQuantTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  EXPECT_EQ(0, quant.childCount());
}

TEST_F(TIOQuantTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  EXPECT_EQ(true, quant.canFetchMore());
}

TEST_F(TIOQuantTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  EXPECT_EQ(0, quant.childCount());
  quant.fetchMore();
  EXPECT_EQ(4, quant.childCount());
}

TEST_F(TIOQuantTestFixture, fetchMoreNonexistentQuant) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("nonexistent quant", m_mockTIOTreeItem);
  try {
    quant.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (const TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOQuantTestFixture, childNamesQuadQuant) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  quant.fetchMore();
  std::vector<std::string> childNames = {"Quant information",
                                         "Pure  :  <Open item for data>",
                                         "Mixed  :  None", "Chunks"};
  EXPECT_EQ(childNames.size(), quant.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, quant.childCount());
    EXPECT_EQ(childNames[i], quant.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, childNamesPointQuant) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOQuant quant("X-Velocity", m_mockTIOTreeItem);
  quant.fetchMore();
  std::vector<std::string> childNames = {
      "Quant information", "Pure  :  <Open item for data>", "Chunks"};
  EXPECT_EQ(childNames.size(), quant.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, quant.childCount());
    EXPECT_EQ(childNames[i], quant.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, childNamesVariablesAndVargroups) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOQuant quant("distance", m_mockTIOTreeItem);
  quant.fetchMore();
  std::vector<std::string> childNames = {
      "Quant information", "Pure  :  <Open item for data>",
      "Mixed  :  None",    "Chunks",
      "directions",        "skill"};
  EXPECT_EQ(childNames.size(), quant.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, quant.childCount());
    EXPECT_EQ(childNames[i], quant.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, quadQuantInformation) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  quant.fetchMore();
  ASSERT_LE(1, quant.childCount());
  TIOTreeItem *infoGroup = quant.child(0);
  EXPECT_EQ("Quant information", infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoData = {"Centring  :  TIO_CENTRE_CELL",
                                       "Units  :  ",
                                       "Data type  :  TIO_DOUBLE",
                                       "N ghosts  :  1",
                                       "Is mixed  :  0",
                                       "Minimum  :  Not in file",
                                       "Maximum  :  Not in file",
                                       "N dims  :  3",
                                       "N I  :  10",
                                       "N J  :  7",
                                       "N K  :  4",
                                       "N mix comp  :  0",
                                       "N chunks  :  8"};
  EXPECT_EQ(infoData.size(), infoGroup->childCount());
  for (int i = 0; i < infoData.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoData[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, unstrQuantInformation) {
  SetUp("../data/ex_unstructured_mesh.h5", "state000", "mesh");
  TIOQuant quant("N neighbours", m_mockTIOTreeItem);
  quant.fetchMore();
  ASSERT_LE(1, quant.childCount());
  TIOTreeItem *infoGroup = quant.child(0);
  EXPECT_EQ("Quant information", infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoData = {"Centring  :  TIO_CENTRE_CELL",
                                       "Units  :  unitless",
                                       "Data type  :  TIO_INT",
                                       "N ghosts  :  0",
                                       "Is mixed  :  0",
                                       "Minimum  :  Not in file",
                                       "Maximum  :  Not in file",
                                       "N N  :  7",
                                       "N mix comp  :  0",
                                       "N chunks  :  1"};
  EXPECT_EQ(infoData.size(), infoGroup->childCount());
  for (int i = 0; i < infoData.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoData[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, pointQuantInformation) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOQuant quant("X-Velocity", m_mockTIOTreeItem);
  quant.fetchMore();
  EXPECT_EQ(3, quant.childCount());
  ASSERT_LE(1, quant.childCount());
  TIOTreeItem *infoGroup = quant.child(0);
  EXPECT_EQ("Quant information", infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {"Centring  :  TIO_CENTRE_NODE",
                                         "Units  :  ",
                                         "Data type  :  TIO_DOUBLE",
                                         "N ghosts  :  0",
                                         "Is mixed  :  0",
                                         "Minimum  :  Not in file",
                                         "Maximum  :  Not in file",
                                         "N N  :  440",
                                         "N chunks  :  1"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, quantChunks) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuant quant("Density", m_mockTIOTreeItem);
  quant.fetchMore();
  ASSERT_EQ(4, quant.childCount());
  TIOTreeItem *chunkGroup = quant.child(3);
  EXPECT_EQ("Chunks", chunkGroup->data(0).toString().toStdString());
  chunkGroup->fetchMore();
  ASSERT_EQ(8, chunkGroup->childCount());
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ("Chunk" + std::to_string(i),
              chunkGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOQuantTestFixture, quadQuantPureData) {
  SetUp("../data/ex_colinear_mesh.h5", "state000", "mesh");
  TIOQuant quant("sum_x_y_z", m_mockTIOTreeItem);
  quant.fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_LE(2, quant.childCount());
  TIOTreeItem *pureDataItem = quant.child(1);
  EXPECT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = quant.getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with std::map and loop
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

TEST_F(TIOQuantTestFixture, unstrQuantPureData) {
  SetUp("../data/ex_unstructured_mesh.h5", "state000", "mesh");
  TIOQuant quant("N neighbours", m_mockTIOTreeItem);
  quant.fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_LE(2, quant.childCount());
  TIOTreeItem *pureDataItem = quant.child(1);
  EXPECT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = quant.getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with std::map and loop
  ASSERT_EQ(7, dataArray->getArrayLength());
  EXPECT_EQ(3, dataArray->toQVariant(0).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(1).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(2).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(3).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(4).toInt());
  EXPECT_EQ(2, dataArray->toQVariant(5).toInt());
  EXPECT_EQ(1, dataArray->toQVariant(6).toInt());
}

TEST_F(TIOQuantTestFixture, pointQuantPureData) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOQuant quant("X-Velocity", m_mockTIOTreeItem);
  quant.fetchMore();
  // Check that the 'Pure' data item exists
  ASSERT_LE(2, quant.childCount());
  TIOTreeItem *pureDataItem = quant.child(1);
  EXPECT_EQ("Pure  :  <Open item for data>",
            pureDataItem->data(0).toString().toStdString());
  // Get Pure data
  DataArray *dataArray = quant.getArrayData("Pure");
  ASSERT_NE(nullptr, dataArray);
  // TODO(jim): Replace this with std::map and loop
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

// TODO(jim): Add tests for mixed quant data
