//
// test/TIOTreeItem_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOTreeItem.h"
#include <gtest/gtest.h>
#include "test/TIOTreeItemTestFixture.h"

TEST(TIOTreeItem, initialization) {
  TIOTreeItem treeItem("tree item");
  EXPECT_EQ(true, treeItem.canFetchMore());
}

TEST_F(TIOTreeItemTestFixture, addTIOVariables) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOTreeItem treeItem("tree item", m_mockTIOTreeItem);
  treeItem.AddTIOVariables(m_fileID, m_stateID);
  ASSERT_EQ(treeItem.childCount(), 1);
  EXPECT_EQ("magic square",
            treeItem.child(0)->data(0).toString().toStdString());
}

TEST_F(TIOTreeItemTestFixture, addTIOVargroups) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOTreeItem treeItem("tree item", m_mockTIOTreeItem);
  treeItem.AddTIOVargroups(m_fileID, m_stateID);
  ASSERT_EQ(treeItem.childCount(), 1);
  EXPECT_EQ("number sequences",
            treeItem.child(0)->data(0).toString().toStdString());
}
