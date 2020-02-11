//
// test/TIOTreeItemTestFixture.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef TEST_TIOTREEITEMTESTFIXTURE_H_
#define TEST_TIOTREEITEMTESTFIXTURE_H_

#include <gtest/gtest.h>
#include <typhonio.h>
#include <string>
#include "test/MockTIOTreeItem.h"

//! This is a test fixture for TIOTreeItem children
//!
//! The class contains a tree item that returns a fileID and objectID.
//! Tests can define their own fixtures inherited from this class.
class TIOTreeItemTestFixture : public ::testing::Test {
 public:
  TIOTreeItemTestFixture();

  ~TIOTreeItemTestFixture() {}

  void SetUp() {}

  void SetUp(const std::string fileName);

  void SetUp(std::string fileName, std::string stateName);

  void SetUp(std::string fileName, std::string stateName, std::string meshName);

  void SetUpInterface(const std::string& fileName, const std::string& stateName,
                      const std::string& interfaceName);

  void SetUpMaterial(const std::string& fileName, const std::string& stateName,
                     const std::string& meshName,
                     const std::string& materialName);

  void SetUpQuant(const std::string& fileName, const std::string& stateName,
                  const std::string& meshName, const std::string& quantName);

  void TearDown();

  MockTIOTreeItem* m_mockTIOTreeItem;
  TIO_File_t m_fileID;
  TIO_Object_t m_stateID, m_meshID, m_materialID, m_quantID, m_interfaceID;

 private:
  void OpenFile(const std::string& fileName);

  void OpenInterface(const std::string& interfaceName);

  void OpenMaterial(const std::string& materialName);

  void OpenMesh(const std::string& meshName);

  void OpenQuant(const std::string& quantName);

  void OpenState(const std::string& stateName);
};

#endif  // TEST_TIOTREEITEMTESTFIXTURE_H_
