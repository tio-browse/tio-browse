//
// test/TIOTreeItemTestFixture.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "test/TIOTreeItemTestFixture.h"

#include <typhonio.h>

#include <string>

TIOTreeItemTestFixture::TIOTreeItemTestFixture()
    : m_mockTIOTreeItem(nullptr),
      m_fileID(TIO_NULL),
      m_stateID(TIO_NULL),
      m_meshID(TIO_NULL),
      m_materialID(TIO_NULL),
      m_quantID(TIO_NULL),
      m_interfaceID(TIO_NULL) {}

void TIOTreeItemTestFixture::OpenFile(const std::string &fileName) {
  TIO_t tioError =
      TIO_Open(fileName.c_str(), &m_fileID, TIO_ACC_READONLY, nullptr, nullptr,
               nullptr, nullptr, MPI_COMM_NULL, MPI_INFO_NULL, MPI_PROC_NULL);
  if (tioError != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open, perhaps file is missing";
  }
}

#ifdef TYPHONIO_INTERFACES_ENABLED
void TIOTreeItemTestFixture::OpenInterface(const std::string &interfaceName) {
  TIO_t tioError = TIO_Open_Interface(m_fileID, m_stateID,
                                      interfaceName.c_str(), &m_interfaceID);
  if (tioError != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open_Interface";
  }
}
#endif

void TIOTreeItemTestFixture::OpenMaterial(const std::string &materialName) {
  TIO_t tErr = TIO_Open_Material(m_fileID, m_meshID, materialName.c_str(),
                                 &m_materialID, nullptr, nullptr, nullptr,
                                 nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open_Material";
  }
}

void TIOTreeItemTestFixture::OpenMesh(const std::string &meshName) {
  TIO_t tioError =
      TIO_Open_Mesh(m_fileID, m_stateID, meshName.c_str(), &m_meshID, nullptr,
                    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tioError != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open_Mesh";
  }
}

void TIOTreeItemTestFixture::OpenQuant(const std::string &quantName) {
  TIO_t tioError =
      TIO_Open_Quant(m_fileID, m_meshID, quantName.c_str(), &m_quantID, nullptr,
                     nullptr, nullptr, nullptr, nullptr);
  if (tioError != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open_Quant";
  }
}

void TIOTreeItemTestFixture::OpenState(const std::string &stateName) {
  TIO_t tioError = TIO_Open_State(m_fileID, stateName.c_str(), &m_stateID,
                                  nullptr, nullptr, nullptr);
  if (tioError != TIO_SUCCESS) {
    FAIL() << "Error in TIO_Open_State";
  }
}

void TIOTreeItemTestFixture::SetUp(const std::string fileName) {
  OpenFile(fileName);
  m_mockTIOTreeItem = new MockTIOTreeItem("tree item", m_fileID, TIO_NULL);
}

void TIOTreeItemTestFixture::SetUp(std::string fileName,
                                   std::string stateName) {
  OpenFile(fileName);
  OpenState(stateName);
  m_mockTIOTreeItem = new MockTIOTreeItem("tree item", m_fileID, m_stateID);
}

void TIOTreeItemTestFixture::SetUp(std::string fileName, std::string stateName,
                                   std::string meshName) {
  OpenFile(fileName);
  OpenState(stateName);
  OpenMesh(meshName);
  m_mockTIOTreeItem =
      new MockTIOTreeItem("tree item", m_fileID, m_meshID, m_meshID);
}

void TIOTreeItemTestFixture::SetUpInterface(const std::string &fileName,
                                            const std::string &stateName,
                                            const std::string &interfaceName) {
  OpenFile(fileName);
  OpenState(stateName);
#ifdef TYPHONIO_INTERFACES_ENABLED
  OpenInterface(interfaceName);
#endif
  m_mockTIOTreeItem = new MockTIOTreeItem("tree item", m_fileID, m_interfaceID);
}

void TIOTreeItemTestFixture::SetUpMaterial(const std::string &fileName,
                                           const std::string &stateName,
                                           const std::string &meshName,
                                           const std::string &materialName) {
  OpenFile(fileName);
  OpenState(stateName);
  OpenMesh(meshName);
  OpenMaterial(materialName);
  m_mockTIOTreeItem =
      new MockTIOTreeItem("Material", m_fileID, m_meshID, m_materialID);
}

void TIOTreeItemTestFixture::SetUpQuant(const std::string &fileName,
                                        const std::string &stateName,
                                        const std::string &meshName,
                                        const std::string &quantName) {
  OpenFile(fileName);
  OpenState(stateName);
  OpenMesh(meshName);
  OpenQuant(quantName);
  m_mockTIOTreeItem =
      new MockTIOTreeItem("quant item", m_fileID, m_meshID, m_quantID);
}

void TIOTreeItemTestFixture::TearDown() {
  TIO_t tioError;
  if (m_fileID != TIO_NULL) {
#ifdef TYPHONIO_INTERFACES_ENABLED
    if (m_interfaceID != TIO_NULL) {
      tioError = TIO_Close_Interface(m_fileID, m_interfaceID);
    }
#endif
    if (m_quantID != TIO_NULL) {
      tioError = TIO_Close_Quant(m_fileID, m_quantID);
    }
    if (m_materialID != TIO_NULL) {
      tioError = TIO_Close_Material(m_fileID, m_materialID);
    }
    if (m_meshID != TIO_NULL) {
      tioError = TIO_Close_Mesh(m_fileID, m_meshID);
    }
    if (m_stateID != TIO_NULL) {
      tioError = TIO_Close_State(m_fileID, m_stateID);
    }
    tioError = TIO_Close(m_fileID);
  }
  delete m_mockTIOTreeItem;
}
