//
// src/TIOQuant.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOQuant.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "TIODataItem.h"
#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOInfoItem.h"
#include "TIOQuantChunkGroup.h"
#include "TIOTypeNames.h"

namespace {
std::string TIODataValueToString(TIO_Data_t dataType, void *value) {
  switch (dataType) {
    case TIO_DATATYPE_NULL:
      return "";
      break;
    case TIO_SHORT:
      return "";
      break;
    case TIO_USHORT:
      return "";
      break;
    case TIO_INT:
      return "";
      break;
    case TIO_UINT:
      return "";
      break;
    case TIO_LONG:
      return "";
      break;
    case TIO_ULONG:
      return "";
      break;
    case TIO_LLONG:
      return "";
      break;
    case TIO_ULLONG:
      return "";
      break;
    case TIO_FLOAT:
      return "";
      break;
    case TIO_DOUBLE:
      return std::to_string(*static_cast<double *>(value));
      break;
    case TIO_LDOUBLE:
      return "";
      break;
    case TIO_LOGICAL:
      return "";
      break;
    case TIO_CHAR:
      return "";
      break;
    case TIO_UCHAR:
      return "";
      break;
    case TIO_ENUM_T:
      return "";
      break;
    case TIO_SIZE_T:
      return "";
      break;
    case TIO_STEP_T:
      return "";
      break;
    case TIO_TIME_T:
      return "";
      break;
    case TIO_STRING:
      return "";
      break;
    default:
      return std::string();
  }
}
}  // namespace

TIOQuant::TIOQuant(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOQuant::~TIOQuant() {
  m_childItems.clear();

  if (m_objectID != TIO_NULL) {
    TIO_Close_Quant(m_parentItem->getFileID(), m_objectID);
  }
}

void TIOQuant::fetchMore() {
  TIOTreeItem::fetchMore();

  // Create a node for the quant info
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Quant information", this));

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getObjectID();
  TIO_Data_t dataType;
  TIO_Centre_t centring;
  TIO_Size_t nGhosts;
  TIO_Bool_t isMixed;
  char units[TIO_STRLEN];
  TIO_t tErr = TIO_Open_Quant(
      fileID, meshID, m_itemData[0].toString().toStdString().c_str(),
      &m_objectID, &dataType, &centring, &nGhosts, &isMixed, units);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add quant information
  std::vector<std::pair<std::string, std::string> > infoData = {
      {"Centring", TIOTypeNames::TIO_CENTRE[centring]},
      {"Units", units},
      {"Data type", TIOTypeNames::TIO_DATA[dataType]},
      {"N ghosts", std::to_string(nGhosts)},
      {"Is mixed", std::to_string(isMixed)},
  };

  for (auto &infoItem : infoData) {
    infoGroup->AddTIOInfoItem(infoItem.first, infoItem.second);
  }

  // Add quant range data
  std::map<TIO_Data_t, int> typeSizes = {
      {TIO_DOUBLE, sizeof(double)},
      {TIO_DATATYPE_NULL, sizeof(nullptr)},
      {TIO_SHORT, sizeof(short)},
      {TIO_USHORT, sizeof(unsigned short)},
      {TIO_INT, sizeof(int)},
      {TIO_UINT, sizeof(unsigned int)},
      {TIO_LONG, sizeof(long)},
      {TIO_ULONG, sizeof(unsigned long)},
      {TIO_LLONG, sizeof(long long)},
      {TIO_ULLONG, sizeof(unsigned long long)},
      {TIO_FLOAT, sizeof(float)},
      {TIO_DOUBLE, sizeof(double)},
      {TIO_LDOUBLE, sizeof(long double)},
      {TIO_LOGICAL, sizeof(bool)},
      {TIO_CHAR, sizeof(char)},
      {TIO_UCHAR, sizeof(unsigned char)},
      //  {TIO_ENUM_T, sizeof(enum)},
      //  {TIO_SIZE_T, sizeof()},
      //  {TIO_STEP_T, sizeof()},
      //  {TIO_TIME_T, sizeof()},
      {TIO_STRING, sizeof(char) * TIO_STRLEN}};
  void *minimum;
  void *maximum;
  int sizeOfMinValue = typeSizes[dataType];
  int sizeOfMaxValue = typeSizes[dataType];
  minimum = ::operator new(sizeOfMinValue);
  maximum = ::operator new(sizeOfMaxValue);
  tErr = TIO_Get_Quant_Range(fileID, m_objectID, dataType, minimum, maximum);
  std::string minValueString, maxValueString;

  if (tErr == TIO_SUCCESS) {
    minValueString = TIODataValueToString(dataType, minimum);
    maxValueString = TIODataValueToString(dataType, maximum);
  } else if (tErr == TIO_ERR_INVALID_RANGE) {
    minValueString = "Not in file";
    maxValueString = "Not in file";
  } else {
    throw TIOException(tErr);
  }

  infoGroup->AddTIOInfoItem("Minimum", minValueString);
  infoGroup->AddTIOInfoItem("Maximum", maxValueString);

  // Add quant read-all data nodes
  TIO_Mesh_t meshType;
  TIO_Size_t nChunks;
  tErr = TIO_Get_Mesh_Info(fileID, meshID, nullptr, &meshType, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, &nChunks, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Size_t nMixComp;
  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR: {
      TIO_Dims_t nDims;
      TIO_Size_t nI, nJ, nK;
      tErr = TIO_Read_QuadQuant_All(fileID, m_objectID, dataType, &nDims, &nI,
                                    &nJ, &nK, nullptr, &nMixComp, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      std::vector<std::pair<std::string, std::string> > quadMeshData = {
          {"N dims", std::to_string(nDims)},
          {"N I", std::to_string(nI)},
          {"N J", std::to_string(nJ)},
          {"N K", std::to_string(nK)},
          {"N mix comp", std::to_string(nMixComp)},
      };
      for (auto &infoItem : quadMeshData) {
        infoGroup->AddTIOInfoItem(infoItem.first, infoItem.second);
      }
    } break;
    case TIO_MESH_UNSTRUCT: {
      TIO_Size_t nN;
      tErr = TIO_Read_UnstrQuant_All(fileID, m_objectID, dataType, &nN, nullptr,
                                     &nMixComp, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      std::vector<std::pair<std::string, std::string> > unstrMeshData = {
          {"N N", std::to_string(nN)},
          {"N mix comp", std::to_string(nMixComp)},
      };
      for (auto &infoItem : unstrMeshData) {
        infoGroup->AddTIOInfoItem(infoItem.first, infoItem.second);
      }
    } break;
    case TIO_MESH_POINT: {
      TIO_Size_t nNodes;
      tErr = TIO_Read_PointQuant_All(fileID, m_objectID, dataType, &nNodes,
                                     nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      std::vector<std::pair<std::string, std::string> > unstrMeshData = {
          {"N N", std::to_string(nNodes)},
      };
      for (auto &infoItem : unstrMeshData) {
        infoGroup->AddTIOInfoItem(infoItem.first, infoItem.second);
      }
    } break;
    default:
      throw TIOMeshtypeException(meshType);
  }

  // N chunks
  infoGroup->AddTIOInfoItem("N chunks", std::to_string(nChunks));
  m_childItems.push_back(std::move(infoGroup));

  // Add data items
  m_childItems.push_back(std::unique_ptr<TIODataItem>(
      new TIODataItem("Pure", "<Open item for data>", this)));
  if (meshType != TIO_MESH_POINT) {
    std::string mixDataString = nMixComp > 0 ? "<Open item for data>" : "None";
    m_childItems.push_back(std::unique_ptr<TIODataItem>(
        new TIODataItem("Mixed", mixDataString, this)));
  }

  // Add quant chunks group
  m_childItems.push_back(std::unique_ptr<TIOQuantChunkGroup>(
      new TIOQuantChunkGroup("Chunks", nChunks, this)));

  // Add variables and vargroups
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}

DataArray *TIOQuant::getArrayData(const std::string &variable) {
  if (variable != "Pure" && variable != "Mixed") {
    // Invalid variable
    return nullptr;
  }

  // Get quant info
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_t tErr;
  TIO_Data_t dataType;
  TIO_Centre_t centring;
  TIO_Size_t nGhosts;
  TIO_Bool_t isMixed;
  char units[TIO_STRLEN];
  tErr = TIO_Get_Quant_Info(fileID, m_objectID, nullptr, &dataType, &centring,
                            &nGhosts, &isMixed, units);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  if (isMixed == false && variable == "Mixed") {
    return nullptr;
  }

  // Get mesh type
  TIO_Object_t meshID = m_parentItem->getMeshID();
  TIO_Mesh_t meshType;
  tErr = TIO_Get_Mesh_Info(fileID, meshID, nullptr, &meshType, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      return GetQuadQuantData(variable, dataType, isMixed);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrQuantData(variable, dataType, isMixed);
      break;
    case TIO_MESH_POINT:
      return GetPointQuantData(variable, dataType);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

DataArray *TIOQuant::GetPointQuantData(const std::string &variable,
                                       const TIO_Data_t &dataType) {
  // Check the variable name is valid
  if (variable != "Pure") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_t tErr;
  TIO_Dims_t nDims = TIO_1D;
  TIO_Size_t dims[1];
  tErr =
      TIO_Read_PointQuant_All(fileID, m_objectID, dataType, &dims[0], nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, nDims, dims, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read data from TyphonIO
  tErr = TIO_Read_PointQuant_All(fileID, m_objectID, dataType, nullptr,
                                 dataArray->getVoidPointer());
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOQuant::GetQuadQuantData(const std::string &variable,
                                      const TIO_Data_t &dataType,
                                      const TIO_Bool_t &isMixed) {
  // Check the variable name is valid
  if (variable != "Pure" && variable != "Mixed") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_t tErr;
  TIO_Dims_t nDims;
  TIO_Size_t dims[3];
  TIO_Size_t nMixComp;
  tErr =
      TIO_Read_QuadQuant_All(fileID, m_objectID, dataType, &nDims, &dims[0],
                             &dims[1], &dims[2], nullptr, &nMixComp, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, nDims, dims, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read data from TyphonIO
  tErr = TIO_Read_QuadQuant_All(
      fileID, m_objectID, dataType, nullptr, nullptr, nullptr, nullptr,
      variable == "Pure" ? dataArray->getVoidPointer() : nullptr, nullptr,
      variable == "Mixed" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOQuant::GetUnstrQuantData(const std::string &variable,
                                       const TIO_Data_t &dataType,
                                       const TIO_Bool_t &isMixed) {
  // Check the variable name is valid
  if (variable != "Pure" && variable != "Mixed") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_t tErr;
  TIO_Dims_t nDims = TIO_1D;
  TIO_Size_t dims[1];
  TIO_Size_t nMixComp;
  tErr = TIO_Read_UnstrQuant_All(fileID, m_objectID, dataType, &dims[0],
                                 nullptr, &nMixComp, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, nDims, dims, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read data from TyphonIO
  tErr = TIO_Read_UnstrQuant_All(
      fileID, m_objectID, dataType, nullptr,
      variable == "Pure" ? dataArray->getVoidPointer() : nullptr, nullptr,
      variable == "Mixed" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}
