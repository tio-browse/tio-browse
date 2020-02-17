//
// test/TIOTreeModel_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOTreeModel.h"

#include <gtest/gtest.h>

#include <QStringList>

TEST(TIOTreeModel, canFetchMore) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ(true, treeModel->canFetchMore(index));
}

TEST(TIOTreeModel, columnCount) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ(1, treeModel->columnCount(index));
}

TEST(TIOTreeModel, data) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ("dummy_filename.h5",
            treeModel->data(index, Qt::DisplayRole).toString().toStdString());
}

TEST(TIOTreeModel, fetchMore) {
  TIOTreeModel *treeModel = new TIOTreeModel("../data/3d_chunk_v1.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ(0, treeModel->rowCount(index));
  treeModel->fetchMore(index);
  EXPECT_EQ(2, treeModel->rowCount(index));
}

TEST(TIOTreeModel, hasChildren) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ(true, treeModel->hasChildren(index));
}

TEST(TIOTreeModel, index) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);

  EXPECT_EQ(0, index.column());
  EXPECT_EQ(0, index.row());
}

TEST(TIOTreeModel, parent) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  QModelIndex parentIndex = treeModel->parent(index);
  EXPECT_EQ(-1, parentIndex.row());
  EXPECT_EQ(-1, parentIndex.column());
}

TEST(TIOTreeModel, rowCount) {
  TIOTreeModel *treeModel = new TIOTreeModel("dummy_filename.h5");
  QModelIndex index = treeModel->index(0, 0);
  EXPECT_EQ(0, treeModel->rowCount(index));
}
