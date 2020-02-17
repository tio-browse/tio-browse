//
// test/TIOMaterial_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOMaterial.h"

#include <gtest/gtest.h>
#include <typhonio.h>

#include <map>
#include <string>
#include <vector>

#include "src/TIOException.h"
#include "src/TIOTreeItem.h"
#include "test/TIOTreeItemTestFixture.h"

class TIOMaterialTestFixture : public TIOTreeItemTestFixture {};

TEST_F(TIOMaterialTestFixture, initialization) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  EXPECT_EQ("Material", material.data(0).toString().toStdString());
}

TEST_F(TIOMaterialTestFixture, childCount) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  EXPECT_EQ(0, material.childCount());
}

TEST_F(TIOMaterialTestFixture, canFetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  EXPECT_EQ(true, material.canFetchMore());
}

TEST_F(TIOMaterialTestFixture, fetchMore) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  EXPECT_EQ(0, material.childCount());
  material.fetchMore();
  EXPECT_EQ(7, material.childCount());
}

TEST_F(TIOMaterialTestFixture, fetchMoreNonexistentMaterial) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("nonexistent material", m_mockTIOTreeItem);
  try {
    material.fetchMore();
    FAIL() << "Exception not thrown as expected";
  } catch (const TIOException &e) {
    EXPECT_STREQ("Object does not exist", e.what());
  }
}

TEST_F(TIOMaterialTestFixture, childNamesColinearMaterial) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  std::vector<std::string> childNames = {"Material information",
                                         "Material  :  <Open item for data>",
                                         "Mixed mat  :  <Open item for data>",
                                         "N comp  :  <Open item for data>",
                                         "PTR  :  <Open item for data>",
                                         "Vol frac  :  <Open item for data>",
                                         "Chunks"};
  EXPECT_EQ(childNames.size(), material.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, material.childCount());
    EXPECT_EQ(childNames[i],
              material.child(i)->data(0).toString().toStdString());
  }
}

// TODO(jim): Create childValuesUnstructuredMaterial test

// TODO(jim): Create childValuesPointMaterial test

TEST_F(TIOMaterialTestFixture, childNamesPointMaterial) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  std::vector<std::string> childNames = {
      "Material information", "Material  :  <Open item for data>", "Chunks"};
  EXPECT_EQ(childNames.size(), material.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, material.childCount());
    EXPECT_EQ(childNames[i],
              material.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialTestFixture, childNamesVariablesAndVargroups) {
  SetUp("../data/ex_vargroup.h5", "state000", "mesh");
  TIOMaterial material("material", m_mockTIOTreeItem);
  material.fetchMore();
  std::vector<std::string> childNames = {"Material information",
                                         "Material  :  <Open item for data>",
                                         "Mixed mat  :  Not in file",
                                         "N comp  :  Not in file",
                                         "PTR  :  Not in file",
                                         "Vol frac  :  Not in file",
                                         "Chunks",
                                         "the answer",
                                         "unit prefixes"};
  EXPECT_EQ(childNames.size(), material.childCount());
  for (int i = 0; i < childNames.size(); ++i) {
    ASSERT_LT(i, material.childCount());
    EXPECT_EQ(childNames[i],
              material.child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialTestFixture, infoValuesColinearMaterial) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  ASSERT_LE(1, material.childCount());  // info group
  TIOTreeItem *infoGroup = material.child(0);
  EXPECT_EQ("Material information",
            infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "N mat  :  5",
      "N ghosts  :  1",
      "Is mixed  :  1",
      "Material numbers  :  <Open item for data>",
      "Material names  :  <Open item for data>",
      "Data type  :  TIO_INT",
      "NC data type  :  TIO_INT",
      "PTR data type  :  TIO_INT",
      "VF data type  :  TIO_DOUBLE",
      "N dims  :  3",
      "N I  :  10",
      "N J  :  7",
      "N K  :  4",
      "N mix cell  :  726",
      "N mix comp  :  1452",
      "N chunks  :  8"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialTestFixture, infoValuesNoncolinearMaterial) {
  // TODO(jim): Create infoValuesNoncolinearMaterial test
}

TEST_F(TIOMaterialTestFixture, infoValuesUnstructuredMaterial) {
  // TODO(jim): Create infoValuesUnstructuredMaterial test
}

TEST_F(TIOMaterialTestFixture, infoValuesPointMaterial) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  ASSERT_LE(1, material.childCount());
  TIOTreeItem *infoGroup = material.child(0);
  EXPECT_EQ("Material information",
            infoGroup->data(0).toString().toStdString());
  std::vector<std::string> infoValues = {
      "N mat  :  5",
      "N ghosts  :  TIO_GHOSTS_NONE",
      "Is mixed  :  0",
      "Material numbers  :  <Open item for data>",
      "Material names  :  <Open item for data>",
      "Data type  :  TIO_INT",
      "NC data type  :  TIO_DATATYPE_NULL",
      "PTR data type  :  TIO_DATATYPE_NULL",
      "VF data type  :  TIO_DATATYPE_NULL",
      "N nodes  :  440",
      "N chunks  :  1"};
  EXPECT_EQ(infoValues.size(), infoGroup->childCount());
  for (int i = 0; i < infoValues.size(); ++i) {
    ASSERT_LT(i, infoGroup->childCount());
    EXPECT_EQ(infoValues[i],
              infoGroup->child(i)->data(0).toString().toStdString());
  }
}

TEST_F(TIOMaterialTestFixture, dataValuesMaterialNumbersAndNames) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();

  // Material numbers
  DataArray *dataArray = material.getArrayData("Material numbers");
  ASSERT_NE(nullptr, dataArray);
  std::vector<int> materialNumbers = {1, 2, 3, 4, 5};
  EXPECT_EQ(materialNumbers.size(), dataArray->getArrayLength());
  for (int i = 0; i < materialNumbers.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(materialNumbers[i], dataArray->toQVariant(i).toInt());
  }

  // Material names
  dataArray = material.getArrayData("Material names");
  ASSERT_NE(nullptr, dataArray);
  std::vector<std::string> materialNames = {"Inner Shell", "Special", "Middle",
                                            "Special", "Outer"};
  EXPECT_EQ(materialNames.size(), dataArray->getArrayLength());
  for (int i = 0; i < materialNames.size(); ++i) {
    ASSERT_LT(i, dataArray->getArrayLength());
    EXPECT_EQ(materialNames[i],
              dataArray->toQVariant(i).toString().toStdString());
  }
}

TEST_F(TIOMaterialTestFixture, dataValuesColinearMaterial) {
  SetUp("../data/3d_chunk_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  DataArray *dataArray;

  // Material
  dataArray = material.getArrayData("Material");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> matData = {{0, 1},     {1, 1},     {2, 1},    {3, -70},
                                {5, -152},  {8, 4},     {13, -74}, {21, 1},
                                {34, -15},  {55, -303}, {89, -95}, {144, 2},
                                {233, -367}};
  EXPECT_EQ(280, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = matData.begin(); it != matData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // Mixed material
  dataArray = material.getArrayData("Mixed mat");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> mixData = {{0, 3},   {1, 4},   {2, 3},   {3, 4},
                                {5, 3},   {8, 2},   {13, 3},  {21, 3},
                                {34, 2},  {55, 3},  {89, 2},  {144, 1},
                                {233, 4}, {377, 4}, {610, 3}, {987, 4}};
  EXPECT_EQ(1452, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = mixData.begin(); it != mixData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // N comp
  dataArray = material.getArrayData("N comp");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> nCompData = {
      {0, 2},  {1, 2},  {2, 2},  {3, 2},   {5, 2},   {8, 2},   {13, 2}, {21, 2},
      {34, 2}, {55, 2}, {89, 2}, {144, 2}, {233, 2}, {377, 2}, {610, 2}};
  EXPECT_EQ(726, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = nCompData.begin();
       it != nCompData.end(); ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // PTR
  dataArray = material.getArrayData("PTR");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> ptrData = {{0, 1},     {1, 3},     {2, 5},     {3, 7},
                                {5, 11},    {8, 17},    {13, 27},   {21, 43},
                                {34, 69},   {55, 111},  {89, 179},  {144, 289},
                                {233, 467}, {377, 755}, {610, 1221}};
  EXPECT_EQ(726, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = ptrData.begin(); it != ptrData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;

  // Vol frac
  dataArray = material.getArrayData("Vol frac");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, double> vfData = {
      {0, 0.960634},   {1, 0.039366},   {2, 0.778943},   {3, 0.221057},
      {5, 0.103939},   {8, 0.663882},   {13, 0.309120},  {21, 0.103939},
      {34, 0.192564},  {55, 0.287624},  {89, 0.195262},  {144, 0.591405},
      {233, 0.536049}, {377, 0.747567}, {610, 0.093637}, {987, 0.375907}};
  EXPECT_EQ(1452, dataArray->getArrayLength());
  for (std::map<int, double>::iterator it = vfData.begin(); it != vfData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_DOUBLE_EQ(it->second, dataArray->toQVariant(it->first).toDouble());
  }
  delete dataArray;
}

// TODO(jim): Create dataValuesUnstructuredMaterial test

TEST_F(TIOMaterialTestFixture, dataValuesPointMaterial) {
  SetUp("../data/3d_point_v1.h5", "State000", "mesh");
  TIOMaterial material("Material", m_mockTIOTreeItem);
  material.fetchMore();
  DataArray *dataArray;

  // Material
  dataArray = material.getArrayData("Material");
  ASSERT_NE(nullptr, dataArray);
  std::map<int, int> matData = {{0, 1},  {1, 1},   {2, 1},   {3, 2},  {5, 3},
                                {8, 4},  {13, 1},  {21, 5},  {34, 2}, {55, 3},
                                {89, 1}, {144, 3}, {233, 3}, {377, 3}};
  EXPECT_EQ(440, dataArray->getArrayLength());
  for (std::map<int, int>::iterator it = matData.begin(); it != matData.end();
       ++it) {
    ASSERT_LT(it->first, dataArray->getArrayLength());
    EXPECT_EQ(it->second, dataArray->toQVariant(it->first).toInt());
  }
  delete dataArray;
}
