//
// test/TIOState_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOState.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "src/TIOException.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOStateTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOStateTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  EXPECT_EQ("State000", state->data(0).toString().toStdString());
}

TEST_F(TIOStateTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  EXPECT_EQ(0, state->childCount());
}

TEST_F(TIOStateTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  EXPECT_EQ(true, state->canFetchMore());
}

TEST_F(TIOStateTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  EXPECT_EQ(0, state->childCount());
  state->fetchMore();
  EXPECT_EQ(2, state->childCount());
}

TEST_F(TIOStateTestFixture, fetchMoreNonexistentState) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("nonexistent state", m_mockTIOTreeItem);
  EXPECT_EQ(0, state->childCount());
  try {
    state->fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (const TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOStateTestFixture, childNames) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  state->fetchMore();
  std::vector<std::string> childNames = {"State information", "mesh"};
  EXPECT_EQ(childNames.size(), state->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, state->childCount());
    EXPECT_EQ(childNames[i], state->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOStateTestFixture, childNamesVariablesAndVargroups) {
  SetUp("../data/ex_vargroup.h5");
  TIOState *state = new TIOState("state000", m_mockTIOTreeItem);
  state->fetchMore();
  std::vector<std::string> childNames = {"State information", "mesh",
                                         "magic square", "number sequences"};
  EXPECT_EQ(childNames.size(), state->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, state->childCount());
    EXPECT_EQ(childNames[i], state->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOStateTestFixture, infoValues) {
  SetUp("../data/3d_chunk_v1.h5");
  TIOState *state = new TIOState("State000", m_mockTIOTreeItem);
  state->fetchMore();
  ASSERT_EQ(2, state->childCount());
  // "State Information"
  TIOTreeItem *stateInfoGroupItem = state->child(0);
  std::vector<std::string> infoValues = {"Step  :  0", "Time  :  0",
                                         "Units  :  us"};

  for (int i = 0; i < infoValues.size(); ++i) {
    EXPECT_EQ(infoValues[i],
              stateInfoGroupItem->child(i)->data(0).toString().toStdString());
  }
}

#ifdef TYPHONIO_INTERFACES_ENABLED
TEST_F(TIOStateTestFixture, interfaces) {
  SetUp("../data/Test_Interface.h5");
  TIOState *state = new TIOState("State_01", m_mockTIOTreeItem);
  state->fetchMore();
  ASSERT_EQ(2, state->childCount());
  std::vector<std::string> stateChildNames = {"State information",
                                              "Interfaces"};

  for (int i = 0; i < stateChildNames.size(); ++i) {
    EXPECT_EQ(stateChildNames[i],
              state->child(i)->data(0).toString().toStdString());
  }
}
#endif  // TYPHONIO_INTERFACES_ENABLED
