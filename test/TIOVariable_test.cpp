//
// test/TIOVariable_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOVariable.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "TIOTreeItemTestFixture.h"

class TIOVariableTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOVariableTestFixture, initialization) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  EXPECT_EQ(0, variable.childCount());
}

TEST_F(TIOVariableTestFixture, childCount) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  EXPECT_EQ(0, variable.childCount());
}

TEST_F(TIOVariableTestFixture, canFetchMore) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  EXPECT_EQ(true, variable.canFetchMore());
}

TEST_F(TIOVariableTestFixture, fetchMore) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  EXPECT_EQ(0, variable.childCount());
  variable.fetchMore();
  EXPECT_EQ(2, variable.childCount());
}

TEST_F(TIOVariableTestFixture, itemNamesFileChild) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  variable.fetchMore();
  std::vector<std::string> itemNames = {"Variable information",
                                        "Value  :  <Open item for data>"};
  EXPECT_EQ(itemNames.size(), variable.childCount());
  for (int i = 0; i < itemNames.size(); ++i) {
    ASSERT_LT(i, variable.childCount());
    EXPECT_EQ(itemNames[i],
              variable.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, infoValuesFileChild) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  variable.fetchMore();
  ASSERT_LE(1, variable.childCount());
  TIOTreeItem *infoGroup = variable.child(0);
  std::vector<std::string> childNames = {
      "Data type  :  TIO_STRING", "N dims  :  1", "Dims  :  [7]", "Units  :  "};
  EXPECT_EQ(childNames.size(), infoGroup->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(childNames[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, dataValuesFileChild) {
  SetUp("../data/ex_vargroup.h5");
  TIOVariable variable("colors", m_mockTIOTreeItem);
  variable.fetchMore();
  DataArray *dataArray = variable.getArrayData("Value");
  ASSERT_NE(nullptr, dataArray);
  std::vector<std::string> colors = {"red",  "orange", "yellow", "green",
                                     "blue", "indigo", "violet"};
  EXPECT_EQ(colors.size(), dataArray->getArrayLength());
  for (int i = 0; i < colors.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(colors[i], dataArray->toQVariant(i).toString().toStdString());
  }
  delete dataArray;
}

TEST_F(TIOVariableTestFixture, itemNamesStateChild) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOVariable variable("magic square", m_mockTIOTreeItem);
  variable.fetchMore();
  std::vector<std::string> itemNames = {"Variable information",
                                        "Value  :  <Open item for data>"};
  EXPECT_EQ(itemNames.size(), variable.childCount());
  for (int i = 0; i < itemNames.size(); ++i) {
    ASSERT_LT(i, variable.childCount());
    EXPECT_EQ(itemNames[i],
              variable.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, infoValuesStateChild) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOVariable variable("magic square", m_mockTIOTreeItem);
  variable.fetchMore();
  ASSERT_LE(1, variable.childCount());
  TIOTreeItem *infoGroup = variable.child(0);
  std::vector<std::string> childNames = {
      "Data type  :  TIO_INT", "N dims  :  2", "Dims  :  [3, 3]", "Units  :  "};
  EXPECT_EQ(childNames.size(), infoGroup->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(childNames[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, dataValuesStateChild) {
  SetUp("../data/ex_vargroup.h5", "state000");
  TIOVariable variable("magic square", m_mockTIOTreeItem);
  variable.fetchMore();
  DataArray *dataArray = variable.getArrayData("Value");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> magicNumbers = {4, 9, 2, 3, 5, 7, 8, 1, 6};
  EXPECT_EQ(magicNumbers.size(), dataArray->getArrayLength());
  for (int i = 0; i < magicNumbers.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(magicNumbers[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;
}

TEST_F(TIOVariableTestFixture, itemNamesMeshChild) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOVariable variable("year", m_mockTIOTreeItem);
  variable.fetchMore();
  std::vector<std::string> itemNames = {"Variable information",
                                        "Value  :  <Open item for data>"};
  EXPECT_EQ(itemNames.size(), variable.childCount());
  for (int i = 0; i < itemNames.size(); ++i) {
    ASSERT_LT(i, variable.childCount());
    EXPECT_EQ(itemNames[i],
              variable.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, infoValuesMeshChild) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOVariable variable("year", m_mockTIOTreeItem);
  variable.fetchMore();
  ASSERT_LE(1, variable.childCount());
  TIOTreeItem *infoGroup = variable.child(0);
  std::vector<std::string> childNames = {
      "Data type  :  TIO_INT", "N dims  :  1", "Dims  :  [1]", "Units  :  "};
  EXPECT_EQ(childNames.size(), infoGroup->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(childNames[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, dataValuesMeshChild) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOVariable variable("year", m_mockTIOTreeItem);
  variable.fetchMore();
  DataArray *dataArray = variable.getArrayData("Value");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> year = {2017};
  EXPECT_EQ(year.size(), dataArray->getArrayLength());
  for (int i = 0; i < year.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(year[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;
}

TEST_F(TIOVariableTestFixture, itemNamesMaterialChild) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOVariable variable("year", m_mockTIOTreeItem);
  variable.fetchMore();
  std::vector<std::string> itemNames = {"Variable information",
                                        "Value  :  <Open item for data>"};
  EXPECT_EQ(itemNames.size(), variable.childCount());
  for (int i = 0; i < itemNames.size(); ++i) {
    ASSERT_LT(i, variable.childCount());
    EXPECT_EQ(itemNames[i],
              variable.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, infoValuesMaterialChild) {
  SetUpMaterial("../data/ex_vargroup.h5", "state000", "mesh", "material");
  TIOVariable variable("the answer", m_mockTIOTreeItem);
  variable.fetchMore();
  ASSERT_LE(1, variable.childCount());
  TIOTreeItem *infoGroup = variable.child(0);
  std::vector<std::string> childNames = {
      "Data type  :  TIO_INT", "N dims  :  1", "Dims  :  [1]", "Units  :  "};
  EXPECT_EQ(childNames.size(), infoGroup->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(childNames[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, dataValuesMaterialChild) {
  SetUpMaterial("../data/ex_vargroup.h5", "state000", "mesh", "material");
  TIOVariable variable("the answer", m_mockTIOTreeItem);
  variable.fetchMore();
  DataArray *dataArray = variable.getArrayData("Value");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> theAnswer = {42};
  EXPECT_EQ(theAnswer.size(), dataArray->getArrayLength());
  for (int i = 0; i < theAnswer.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(theAnswer[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;
}

TEST_F(TIOVariableTestFixture, itemNamesQuantChild) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOVariable variable("year", m_mockTIOTreeItem);
  variable.fetchMore();
  std::vector<std::string> itemNames = {"Variable information",
                                        "Value  :  <Open item for data>"};
  EXPECT_EQ(itemNames.size(), variable.childCount());
  for (int i = 0; i < itemNames.size(); ++i) {
    ASSERT_LT(i, variable.childCount());
    EXPECT_EQ(itemNames[i],
              variable.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, infoValuesQuantChild) {
  SetUpQuant("../data/ex_vargroup.h5", "state000", "mesh", "distance");
  TIOVariable variable("directions", m_mockTIOTreeItem);
  variable.fetchMore();
  ASSERT_LE(1, variable.childCount());
  TIOTreeItem *infoGroup = variable.child(0);
  std::vector<std::string> childNames = {
      "Data type  :  TIO_UCHAR", "N dims  :  1", "Dims  :  [4]", "Units  :  "};
  EXPECT_EQ(childNames.size(), infoGroup->childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(childNames[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOVariableTestFixture, dataValuesQuantChild) {
  SetUpQuant("../data/ex_vargroup.h5", "state000", "mesh", "distance");
  TIOVariable variable("directions", m_mockTIOTreeItem);
  variable.fetchMore();
  DataArray *dataArray = variable.getArrayData("Value");
  ASSERT_NE(nullptr, dataArray);
  std::vector<char> directions = {'n', 'e', 's', 'w'};
  EXPECT_EQ(directions.size(), dataArray->getArrayLength());
  for (int i = 0; i < directions.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(directions[i], dataArray->toQVariant(i).toInt());
  }
  delete dataArray;
}
