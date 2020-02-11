//
// src/TIOPolygon.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOPolygon.h"
#include <memory>
#include <string>
#include <utility>
#include "src/TIODataItem.h"
#include "src/TIOException.h"
#include "src/TIOInfoGroup.h"
#include "src/TIOInfoItem.h"

TIOPolygon::TIOPolygon(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOPolygon::~TIOPolygon() { m_childItems.clear(); }

void TIOPolygon::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_Object_t fileID = m_parentItem->getFileID();
  TIO_Object_t interfaceID = m_parentItem->getObjectID();
  TIO_Size_t nVertices;
  char units[TIO_STRLEN];
  TIO_t tioError = TIO_Open_Polygon(
      fileID, interfaceID, m_itemData[0].toString().toStdString().c_str(),
      &m_objectID, &nVertices, units);
  if (tioError != TIO_SUCCESS) {
    throw TIOException(tioError);
  }

  // Add polygon info
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Polygon information", this));
  infoGroup->AddTIOInfoItem("N vertices", std::to_string(nVertices));
  infoGroup->AddTIOInfoItem("Data type", "TIO_DOUBLE");
  m_childItems.push_back(std::move(infoGroup));

  // Add data items
  AddTIODataItem("X 1", "<Open item for data>");
  AddTIODataItem("Y 1", "<Open item for data>");
  AddTIODataItem("X 2", "<Open item for data>");
  AddTIODataItem("Y 2", "<Open item for data>");
  AddTIODataItem("Left material", "<Open item for data>");
  AddTIODataItem("Right material", "<Open item for data>");

  // Add vargroups and variables
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}

DataArray *TIOPolygon::getArrayData(const std::string &variable) {
  // Check for valid variable
  if (variable != "X 1" && variable != "Y 1" && variable != "X 2" &&
      variable != "Y 2" && variable != "Left material" &&
      variable != "Right material") {
    throw TIOVariableNameException(variable);
  }

  // Get array sizes
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Size_t nCells;
  TIO_t tErr =
      TIO_Get_Polygon_Info(fileID, m_objectID, nullptr, &nCells, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  TIO_Data_t dataType = TIO_DATATYPE_NULL;
  if (variable == "X 1" || variable == "Y 1" || variable == "X 2" ||
      variable == "Y 2") {
    dataType = TIO_DOUBLE;
  } else if (variable == "Left material" || variable == "Right material") {
    dataType = TIO_INT;
  }

  DataArray *dataArray =
      new DataArray(variable, TIO_1D, &nCells, DataArray::TYPE_DATA, dataType);

  // TODO(jim): Remove these allocates when TIO_Read_Polygon has been fixed
  // TODO(jim): ... to accept null arguments
  double *x1 = new double[nCells];
  double *y1 = new double[nCells];
  double *x2 = new double[nCells];
  double *y2 = new double[nCells];
  int *matL = new int[nCells];
  int *matR = new int[nCells];

  // TODO(jim): Replace x1, y2, x2 etc. with nullptr when TIO_Read_Polygon has
  // TODO(jim): ... been fixed to accept null arguments
  tErr = TIO_Read_Polygon(
      fileID, m_objectID, nCells,
      variable == "X 1" ? static_cast<double *>(dataArray->getVoidPointer())
                        : x1,
      variable == "Y 1" ? static_cast<double *>(dataArray->getVoidPointer())
                        : y1,
      variable == "X 2" ? static_cast<double *>(dataArray->getVoidPointer())
                        : x2,
      variable == "Y 2" ? static_cast<double *>(dataArray->getVoidPointer())
                        : y2,
      variable == "Left material"
          ? static_cast<int *>(dataArray->getVoidPointer())
          : matL,
      variable == "Right material"
          ? static_cast<int *>(dataArray->getVoidPointer())
          : matR);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    dataArray = nullptr;
  }

  // TODO(jim): Remove these deallocates when TIO_Read_Polygon has been fixed
  // TODO(jim): ... to accept null arguments
  delete[] x1;
  delete[] y1;
  delete[] x2;
  delete[] y2;
  delete[] matL;
  delete[] matR;

  return dataArray;
}
