//
// test/TIOPolygon_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOPolygon.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "src/TIOException.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOPolygonTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOPolygonTestFixture, initialization) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  EXPECT_EQ("polygon_1", polygon.data(0).toString().toStdString());
}

TEST_F(TIOPolygonTestFixture, childCount) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  EXPECT_EQ(0, polygon.childCount());
}

TEST_F(TIOPolygonTestFixture, canFetchMore) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  EXPECT_TRUE(polygon.canFetchMore());
}

TEST_F(TIOPolygonTestFixture, fetchMore) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  EXPECT_EQ(0, polygon.childCount());
  polygon.fetchMore();
  EXPECT_EQ(7, polygon.childCount());
}

TEST_F(TIOPolygonTestFixture, fetchMoreNonexistentPolygon) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("nonexistent polygon", m_mockTIOTreeItem);
  try {
    polygon.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOPolygonTestFixture, interfaceNames) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  polygon.fetchMore();
  std::vector<std::string> interfaceNames = {
      "Polygon information",
      "X 1  :  <Open item for data>",
      "Y 1  :  <Open item for data>",
      "X 2  :  <Open item for data>",
      "Y 2  :  <Open item for data>",
      "Left material  :  <Open item for data>",
      "Right material  :  <Open item for data>"};
  EXPECT_EQ(interfaceNames.size(), polygon.childCount());
  for (int i = 0; i < interfaceNames.size(); ++i) {
    ASSERT_LT(i, polygon.childCount());
    EXPECT_EQ(interfaceNames[i],
              polygon.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOPolygonTestFixture, infoValues) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  polygon.fetchMore();
  ASSERT_LE(1, polygon.childCount());
  TIOTreeItem *infoGroup = polygon.child(0);
  std::vector<std::string> infoValues = {"N vertices  :  20",
                                         "Data type  :  TIO_DOUBLE"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOPolygonTestFixture, dataValues) {
  SetUpInterface("../data/Test_Interface.h5", "State_01",
                 "interface_container3");
  TIOPolygon polygon("polygon_1", m_mockTIOTreeItem);
  polygon.fetchMore();
  EXPECT_EQ(7, polygon.childCount());
  DataArray *dataArray;

  dataArray = polygon.getArrayData("X 1");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "X 1");
  EXPECT_EQ(dataArray->getDim(0), 20);
  std::vector<double> x1Values = {
      1.01,  2.01,  3.01,  4.01,  5.01,  6.01,  7.01,  8.01,  9.01,  10.01,
      11.01, 12.01, 13.01, 14.01, 15.01, 16.01, 17.01, 18.01, 19.01, 20.01};
  EXPECT_EQ(x1Values.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), x1Values[i]);
  }
  delete dataArray;

  dataArray = polygon.getArrayData("Y 1");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "Y 1");
  EXPECT_EQ(dataArray->getDim(0), 20);
  std::vector<double> y1Values = {
      1.51,  2.51,  3.51,  4.51,  5.51,  6.51,  7.51,  8.51,  9.51,  10.51,
      11.51, 12.51, 13.51, 14.51, 15.51, 16.51, 17.51, 18.51, 19.51, 20.51};
  EXPECT_EQ(y1Values.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), y1Values[i]);
  }
  delete dataArray;

  dataArray = polygon.getArrayData("X 2");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "X 2");
  EXPECT_EQ(dataArray->getDim(0), 20);
  std::vector<double> x2Values = {1.1,  2.1,  3.1,  4.1,  5.1,  6.1,  7.1,
                                  8.1,  9.1,  10.1, 11.1, 12.1, 13.1, 14.1,
                                  15.1, 16.1, 17.1, 18.1, 19.1, 20.1};
  EXPECT_EQ(x2Values.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), x2Values[i]);
  }
  delete dataArray;

  dataArray = polygon.getArrayData("Y 2");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "Y 2");
  EXPECT_EQ(dataArray->getDim(0), 20);
  ;
  std::vector<double> y2Values = {1.2,  2.2,  3.2,  4.2,  5.2,  6.2,  7.2,
                                  8.2,  9.2,  10.2, 11.2, 12.2, 13.2, 14.2,
                                  15.2, 16.2, 17.2, 18.2, 19.2, 20.2};
  EXPECT_EQ(y2Values.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), y2Values[i]);
  }
  delete dataArray;

  dataArray = polygon.getArrayData("Left material");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "Left material");
  EXPECT_EQ(dataArray->getDim(0), 20);
  std::vector<double> matLValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  EXPECT_EQ(matLValues.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), matLValues[i]);
  }
  delete dataArray;

  dataArray = polygon.getArrayData("Right material");
  ASSERT_NE(nullptr, dataArray);
  EXPECT_EQ(dataArray->getName(), "Right material");
  EXPECT_EQ(dataArray->getDim(0), 20);
  std::vector<double> matRValues = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                    2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  EXPECT_EQ(matRValues.size(), dataArray->getArrayLength());
  for (int i = 0; i < 20; ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(dataArray->toQVariant(i).toDouble(), matRValues[i]);
  }
  delete dataArray;
}
