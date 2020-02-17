//
// test/TIOFile_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOFile.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "src/TIOException.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOFileTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOFileTestFixture, initialization) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  EXPECT_EQ("ex_colinear_mesh.h5", file.data(0).toString().toStdString());
  EXPECT_EQ(TIO_NULL, file.getFileID());
}

TEST_F(TIOFileTestFixture, canFetchMore) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  EXPECT_EQ(true, file.canFetchMore());
}

TEST_F(TIOFileTestFixture, fetchMore) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  EXPECT_EQ(0, file.childCount());
  file.fetchMore();
  EXPECT_EQ(2, file.childCount());
}

TEST_F(TIOFileTestFixture, fetchMoreNonexistentFile) {
  TIOFile file("nonexistent_file.h5", "../data/nonexistent_file.h5",
               m_mockTIOTreeItem);
  try {
    file.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (const TIOException &e) {
    EXPECT_STREQ("Failed to open file - Check filename & permissions",
                 e.what());
  }
}

TEST_F(TIOFileTestFixture, childNames) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  file.fetchMore();
  std::cout << "here 3" << std::endl;
  ASSERT_LE(1, file.childCount());
  std::cout << "here 4" << std::endl;
  std::vector<std::string> childNames = {"File information", "state000"};
  EXPECT_EQ(childNames.size(), file.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, file.childCount());
    EXPECT_EQ(childNames[i], file.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOFileTestFixture, childNameVariablesAndVargroups) {
  TIOFile file("ex_vargroup.h5", "../data/ex_vargroup.h5", m_mockTIOTreeItem);
  file.fetchMore();
  ASSERT_LE(1, file.childCount());
  std::vector<std::string> childNames = {"File information", "state000",
                                         "colors", "version"};
  EXPECT_EQ(childNames.size(), file.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, file.childCount());
    EXPECT_EQ(childNames[i], file.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOFileTestFixture, getFileID) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  EXPECT_EQ(TIO_NULL, file.getFileID());
  file.fetchMore();
  EXPECT_NE(TIO_NULL, file.getFileID());
}

TEST_F(TIOFileTestFixture, infoValues) {
  TIOFile file("ex_colinear_mesh.h5", "../data/ex_colinear_mesh.h5",
               m_mockTIOTreeItem);
  file.fetchMore();
  ASSERT_LE(1, file.childCount());
  TIOTreeItem *infoGroup = file.child(0);
  EXPECT_EQ("File information", infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "Code  :  ex_colinear_mesh", "Version  :  0.0.1",
      "Date  :  2016/11/03",       "Title  :  TyphonIO colinear mesh example",
      "Library  :  1.6.1",
  };
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}
