//
// test/TIOInterface_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInterface.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test/TIOTreeItemTestFixture.h"

class TIOInterfaceTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOInterfaceTestFixture, initialization) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  EXPECT_EQ("interface_container3", interface.data(0).toString().toStdString());
}

TEST_F(TIOInterfaceTestFixture, childCount) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  EXPECT_EQ(0, interface.childCount());
}

TEST_F(TIOInterfaceTestFixture, canFetchMore) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  EXPECT_EQ(true, interface.canFetchMore());
}

TEST_F(TIOInterfaceTestFixture, fetchMore) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  EXPECT_EQ(0, interface.childCount());
  interface.fetchMore();
  EXPECT_EQ(3, interface.childCount());
}

TEST_F(TIOInterfaceTestFixture, fetchMoreNoPolygons) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container1", m_mockTIOTreeItem);
  EXPECT_EQ(0, interface.childCount());
  interface.fetchMore();
  EXPECT_EQ(1, interface.childCount());
}

TEST_F(TIOInterfaceTestFixture, childNames) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  interface.fetchMore();
  std::vector<std::string> childNames = {"Interface information", "polygon_1",
                                         "polygon_2"};
  EXPECT_EQ(childNames.size(), interface.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, interface.childCount());
    EXPECT_EQ(childNames[i],
              interface.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOInterfaceTestFixture, infoValues) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container3", m_mockTIOTreeItem);
  interface.fetchMore();
  ASSERT_LE(1, interface.childCount());
  TIOTreeItem* infoGroup = interface.child(0);
  std::vector<std::string> infoValues = {"N polygons  :  2"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOInterfaceTestFixture, infoValuesNoPolygons) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterface interface("interface_container1", m_mockTIOTreeItem);
  interface.fetchMore();
  ASSERT_LE(1, interface.childCount());
  TIOTreeItem* infoGroup = interface.child(0);
  std::vector<std::string> infoValues = {"N polygons  :  0"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}
