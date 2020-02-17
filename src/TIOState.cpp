//
// src/TIOState.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOState.h"

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOInfoItem.h"
#include "TIOInterfaceGroup.h"
#include "TIOMesh.h"
#include "TIOTreeItem.h"

TIOState::TIOState(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOState::~TIOState() {
  m_childItems.clear();

  // TODO(jim): Replace this with a call to 'getParentFileID' which would
  // TODO(jim): ... check that m_parentItem is a valid pointer first
  TIO_File_t fileID = m_parentItem->getFileID();
  if (fileID != TIO_NULL && m_objectID != TIO_NULL) {
    TIO_Close_State(fileID, m_objectID);
  }
}

void TIOState::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_t tErr;
  TIO_Step_t stateStep;
  TIO_Time_t stateTime;
  char stateUnits[TIO_STRLEN];
  std::stringstream ssStateStep, ssStateTime;
  TIO_Size_t nMeshes;

  // Get fileID from parent
  TIO_File_t fileID = m_parentItem->getFileID();

  tErr = TIO_Open_State(fileID, m_itemData[0].toString().toStdString().c_str(),
                        &m_objectID, &stateStep, &stateTime, stateUnits);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  ssStateStep << stateStep;
  ssStateTime << stateTime;

  // Add state info
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("State information", this));
  infoGroup->AddTIOInfoItem("Step", ssStateStep.str());
  infoGroup->AddTIOInfoItem("Time", ssStateTime.str());
  infoGroup->AddTIOInfoItem("Units", stateUnits);
  m_childItems.push_back(std::move(infoGroup));

  // Create info items

#ifdef TYPHONIO_INTERFACES_ENABLED
  // Add interfaces to state
  TIO_Size_t nInterfaces;
  tErr = TIO_List_Interfaces(fileID, m_objectID, &nInterfaces, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  if (nInterfaces > 0) {
    m_childItems.push_back(std::unique_ptr<TIOInterfaceGroup>(
        new TIOInterfaceGroup("Interfaces", this)));
  }
#endif

  // Add meshes to state
  tErr = TIO_List_Meshes(fileID, m_objectID, &nMeshes, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  auto meshNames =
      std::unique_ptr<char[][TIO_STRLEN]>(new char[nMeshes][TIO_STRLEN]);
  tErr = TIO_List_Meshes(fileID, m_objectID, nullptr, meshNames.get());
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  for (TIO_Size_t i = 0; i < nMeshes; ++i) {
    m_childItems.push_back(
        std::unique_ptr<TIOMesh>(new TIOMesh(meshNames[i], this)));
  }

  // Add variables and vargroups
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}
