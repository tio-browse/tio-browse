//
// src/TIOInterface.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOInterface.h"
#include <memory>
#include <string>
#include <utility>
#include "src/TIOException.h"
#include "src/TIOInfoGroup.h"
#include "src/TIOInfoItem.h"
#include "src/TIOPolygon.h"

TIOInterface::TIOInterface(const std::string& itemName, TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent) {}

TIOInterface::~TIOInterface() {
  m_childItems.clear();

  TIO_File_t fileID = m_parentItem->getFileID();
  if (fileID != TIO_NULL && m_objectID != TIO_NULL) {
    TIO_Close_Interface(fileID, m_objectID);
  }
  m_objectID = TIO_NULL;
}

void TIOInterface::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t stateID = m_parentItem->getObjectID();
  TIO_t tErr;
  tErr = TIO_Open_Interface(fileID, stateID,
                            m_itemData[0].toString().toStdString().c_str(),
                            &m_objectID);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add interface info
  TIO_Size_t nPolygons;
  tErr = TIO_List_Polygons(fileID, m_objectID, &nPolygons, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Interface information", this));
  infoGroup->AddTIOInfoItem("N polygons", std::to_string(nPolygons));
  m_childItems.push_back(std::move(infoGroup));

  // Add polygons to interface item
  if (nPolygons > 0) {
    auto polygonNames =
        std::unique_ptr<char[][TIO_STRLEN]>(new char[nPolygons][TIO_STRLEN]);
    tErr = TIO_List_Polygons(fileID, m_objectID, nullptr, polygonNames.get());
    if (tErr != TIO_SUCCESS) {
      throw TIOException(tErr);
    }
    for (TIO_Size_t i = 0; i < nPolygons; ++i) {
      m_childItems.push_back(
          std::unique_ptr<TIOPolygon>(new TIOPolygon(polygonNames[i], this)));
    }
  }

  // Add vargroups and variables
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}
