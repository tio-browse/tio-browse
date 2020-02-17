//
// test/TIOInterfaceGroupItem_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInterfaceGroup.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "test/TIOTreeItemTestFixture.h"

class TIOInterfaceGroupTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOInterfaceGroupTestFixture, initialization) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterfaceGroup group("Interfaces", m_mockTIOTreeItem);
  EXPECT_EQ("Interfaces", group.data(0).toString().toStdString());
}

TEST_F(TIOInterfaceGroupTestFixture, childCount) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterfaceGroup group("Interfaces", m_mockTIOTreeItem);
  EXPECT_EQ(0, group.childCount());
}

TEST_F(TIOInterfaceGroupTestFixture, canFetchMore) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterfaceGroup group("Interfaces", m_mockTIOTreeItem);
  EXPECT_EQ(true, group.canFetchMore());
}

TEST_F(TIOInterfaceGroupTestFixture, fetchMore) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterfaceGroup group("Interfaces", m_mockTIOTreeItem);
  EXPECT_EQ(0, group.childCount());
  group.fetchMore();
  EXPECT_EQ(3, group.childCount());
}

TEST_F(TIOInterfaceGroupTestFixture, interfaceNames) {
  SetUp("../data/Test_Interface.h5", "State_01");
  TIOInterfaceGroup group("Interfaces", m_mockTIOTreeItem);
  group.fetchMore();
  std::vector<std::string> interfaceNames = {
      "interface_container1", "interface_container2", "interface_container3"};
  EXPECT_EQ(interfaceNames.size(), group.childCount());
  for (int i = 0; i < interfaceNames.size(); ++i) {
    ASSERT_LT(i, group.childCount());
    EXPECT_EQ(interfaceNames[i],
              group.child(i)->data(0).toString().toStdString());
  }
}
