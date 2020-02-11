//
// test/TIOQuantGroup_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuantGroup.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "test/TIOTreeItemTestFixture.h"

class TIOQuantGroupTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOQuantGroupTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantGroup* quantGroup =
      new TIOQuantGroup("Quantities", m_mockTIOTreeItem);
  EXPECT_EQ("Quantities", quantGroup->data(0).toString().toStdString());
}

TEST_F(TIOQuantGroupTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantGroup* quantGroup =
      new TIOQuantGroup("Quantities", m_mockTIOTreeItem);
  EXPECT_EQ(0, quantGroup->childCount());
}

TEST_F(TIOQuantGroupTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantGroup* quantGroup =
      new TIOQuantGroup("Quantities", m_mockTIOTreeItem);
  EXPECT_EQ(true, quantGroup->canFetchMore());
}

TEST_F(TIOQuantGroupTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantGroup* quantGroup =
      new TIOQuantGroup("Quantities", m_mockTIOTreeItem);
  EXPECT_EQ(0, quantGroup->childCount());
  quantGroup->fetchMore();
  EXPECT_EQ(5, quantGroup->childCount());
}

TEST_F(TIOQuantGroupTestFixture, quantNames) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOQuantGroup* quantGroup =
      new TIOQuantGroup("Quantities", m_mockTIOTreeItem);
  quantGroup->fetchMore();
  ASSERT_EQ(5, quantGroup->childCount());
  std::vector<std::string> quantNames = {"Density", "X-Velocity", "XFlux",
                                         "YFlux", "ZFlux"};

  for (int i = 0; i < quantNames.size(); ++i) {
    EXPECT_EQ(quantNames[i],
              quantGroup->child(i)->data(0).toString().toStdString());
  }
}
