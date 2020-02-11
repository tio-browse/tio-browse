//
// test/TIOVargroup_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOVargroup.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "src/TIOException.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOVargroupTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOVargroupTestFixture, initialization) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("version", m_mockTIOTreeItem);
  EXPECT_EQ(0, vargroup.childCount());
}

TEST_F(TIOVargroupTestFixture, childCount) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("version", m_mockTIOTreeItem);
  EXPECT_EQ(0, vargroup.childCount());
}

TEST_F(TIOVargroupTestFixture, canFetchMore) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("version", m_mockTIOTreeItem);
  EXPECT_EQ(true, vargroup.canFetchMore());
}

TEST_F(TIOVargroupTestFixture, fetchMore) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("version", m_mockTIOTreeItem);
  EXPECT_EQ(0, vargroup.childCount());
  vargroup.fetchMore();
  EXPECT_EQ(3, vargroup.childCount());
}

TEST_F(TIOVargroupTestFixture, fetchMoreNonexistentVargroup) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("nonexistent vargroup", m_mockTIOTreeItem);
  try {
    vargroup.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOVargroupTestFixture, itemNamesFileChild) {
  SetUp("../data/ex_vargroup.h5");
  TIOVargroup vargroup("version", m_mockTIOTreeItem);
  vargroup.fetchMore();
  std::vector<std::string> childNames = {"major version", "minor version",
                                         "patch version"};
  EXPECT_EQ(childNames.size(), vargroup.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, vargroup.childCount());
    EXPECT_EQ(childNames[i],
              vargroup.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVargroupTestFixture, itemNamesStateChild) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOVargroup vargroup("number sequences", m_mockTIOTreeItem);
  vargroup.fetchMore();
  std::vector<std::string> childNames = {"primes", "squares", "fibonacci"};
  EXPECT_EQ(childNames.size(), vargroup.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, vargroup.childCount());
    EXPECT_EQ(childNames[i],
              vargroup.child(i)->data(0).toString().toStdString());
  }
}
