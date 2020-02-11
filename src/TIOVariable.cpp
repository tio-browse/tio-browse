//
// src/TIOVariable.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOVariable.h"
#include <memory>
#include <string>
#include <utility>
#include "src/TIOException.h"
#include "src/TIOInfoGroup.h"
#include "src/TIOTypeNames.h"

TIOVariable::TIOVariable(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOVariable::~TIOVariable() {
  m_childItems.clear();

  TIO_File_t fileID = m_parentItem->getFileID();
  if (m_objectID != TIO_NULL && fileID != TIO_NULL) {
    TIO_Close_Variable(fileID, m_objectID);
  }
  m_objectID = TIO_NULL;
}

std::string TIOVariable::DimsToString(const TIO_Dims_t nDims,
                                      const TIO_Size_t *dims) {
  std::string dimsString;
  dimsString.append("[");

  for (int i = 0; i < nDims; ++i) {
    dimsString.append(std::to_string(dims[i]));
    if (i < nDims - 1) {
      dimsString.append(", ");
    }
  }
  dimsString.append("]");
  return dimsString;
}

void TIOVariable::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t objectID = m_parentItem->getObjectID();

  TIO_Data_t dataType;
  TIO_Dims_t nDims;
  TIO_Size_t dims[TIO_MAXDIMS];
  char units[TIO_STRLEN];
  TIO_t tErr = TIO_Open_Variable(fileID, objectID,
                                 m_itemData[0].toString().toStdString().c_str(),
                                 &m_objectID, &dataType, &nDims, dims, units);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add info group
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Variable information", this));
  infoGroup->AddTIOInfoItem("Data type", TIOTypeNames::TIO_DATA[dataType]);
  infoGroup->AddTIOInfoItem("N dims", std::to_string(nDims));
  infoGroup->AddTIOInfoItem("Dims", DimsToString(nDims, dims));
  infoGroup->AddTIOInfoItem("Units", units);
  m_childItems.push_back(std::move(infoGroup));

  // Add data item
  AddTIODataItem("Value", "<Open item for data>");
}

DataArray *TIOVariable::getArrayData(const std::string &variable) {
  if (variable != "Value") {
    throw TIOVariableNameException(variable);
  }
  TIO_File_t fileID = m_parentItem->getFileID();

  TIO_t tErr;
  TIO_Data_t dataType;
  TIO_Dims_t nDims;
  TIO_Size_t dims[TIO_MAXDIMS];
  tErr = TIO_Get_Variable_Info(fileID, m_objectID, nullptr, &dataType, &nDims,
                               dims, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, nDims, dims, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read data
  tErr = TIO_Read_Variable(fileID, m_objectID, dataType,
                           dataArray->getVoidPointer());
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}
