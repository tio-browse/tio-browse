//
// src/TIOQuantChunk.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOQuantChunk.h"
#include <memory>
#include <string>
#include <utility>
#include "src/TIODataItem.h"
#include "src/TIOException.h"
#include "src/TIOInfoGroup.h"
#include "src/TIOTypeNames.h"

TIOQuantChunk::TIOQuantChunk(const std::string &itemName, const int &chunkIndex,
                             TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent), m_chunkIndex(chunkIndex) {}

TIOQuantChunk::~TIOQuantChunk() {}

void TIOQuantChunk::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t quantID = m_parentItem->getObjectID();
  TIO_Object_t meshID = m_parentItem->getMeshID();

  TIO_t tErr;
  TIO_Mesh_t meshType;
  tErr = TIO_Get_Mesh_Info(fileID, meshID, nullptr, &meshType, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Data_t dataType;
  TIO_Size_t nGhosts;
  tErr = TIO_Get_Quant_Info(fileID, quantID, nullptr, &dataType, nullptr,
                            &nGhosts, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Dims_t nDims;
  TIO_Size_t nI, nJ, nK, nMixComp;
  TIO_Size_t nN;
  TIO_Size_t nNodes;

  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Quant chunk information", this));
  infoGroup->AddTIOInfoItem("Data type", TIOTypeNames::TIO_DATA[dataType]);

  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      tErr = TIO_Read_QuadQuant_Chunk(
          fileID, quantID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType,
          nGhosts, &nDims, &nI, &nJ, &nK, nullptr, &nMixComp, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N dims", std::to_string(nDims));
      infoGroup->AddTIOInfoItem("N I", std::to_string(nI));
      infoGroup->AddTIOInfoItem("N J", std::to_string(nJ));
      infoGroup->AddTIOInfoItem("N K", std::to_string(nK));
      infoGroup->AddTIOInfoItem("N ghosts", std::to_string(nGhosts));
      infoGroup->AddTIOInfoItem("N mix comp", std::to_string(nMixComp));
      break;
    case TIO_MESH_UNSTRUCT:
      tErr = TIO_Read_UnstrQuant_Chunk(fileID, quantID, m_chunkIndex,
                                       TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                       &nN, nullptr, &nMixComp, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N N", std::to_string(nN));
      infoGroup->AddTIOInfoItem("N ghosts", std::to_string(nGhosts));
      infoGroup->AddTIOInfoItem("N mix comp", std::to_string(nMixComp));
      break;
    case TIO_MESH_POINT:
      tErr = TIO_Read_PointQuant_Chunk(fileID, quantID, m_chunkIndex,
                                       TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                       &nNodes, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      TIO_Size_t nL, nH;
      tErr = TIO_Get_Point_Chunk(fileID, meshID, m_chunkIndex, nullptr, &nL,
                                 &nH, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N nodes", std::to_string(nNodes));
      infoGroup->AddTIOInfoItem("N L", std::to_string(nL));
      infoGroup->AddTIOInfoItem("N H", std::to_string(nH));
      infoGroup->AddTIOInfoItem("N ghosts", std::to_string(nGhosts));
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }

  m_childItems.push_back(std::move(infoGroup));

  // Add data items
  m_childItems.push_back(std::unique_ptr<TIODataItem>(
      new TIODataItem("Pure", "<Open item for data>", this)));
  if (meshType != TIO_MESH_POINT) {
    std::string mixDataString = nMixComp > 0 ? "<Open item for data>" : "None";
    m_childItems.push_back(std::unique_ptr<TIODataItem>(
        new TIODataItem("Mixed", mixDataString, this)));
  }
}

DataArray *TIOQuantChunk::getArrayData(const std::string &variable) {
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_File_t quantID = m_parentItem->getObjectID();

  TIO_t tErr;
  TIO_Data_t dataType;
  TIO_Centre_t centring;
  TIO_Size_t nGhosts;
  TIO_Bool_t isMixed;
  char units[TIO_STRLEN];
  tErr = TIO_Get_Quant_Info(fileID, quantID, nullptr, &dataType, &centring,
                            &nGhosts, &isMixed, units);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Get the mesh type
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
      return GetQuadQuantData(variable, fileID, quantID, dataType, nGhosts,
                              isMixed);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrQuantData(variable, fileID, quantID, dataType, nGhosts,
                               isMixed);
      break;
    case TIO_MESH_POINT:
      return GetPointQuantData(variable, fileID, quantID, dataType, nGhosts);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

DataArray *TIOQuantChunk::GetPointQuantData(const std::string &variable,
                                            const TIO_File_t &fileID,
                                            const TIO_Object_t &quantID,
                                            const TIO_Data_t &dataType,
                                            const TIO_Size_t &nGhosts) {
  // Check the variable name is valid
  if (variable != "Pure") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_t tErr;
  TIO_Dims_t nDims = TIO_1D;
  TIO_Size_t dims[1];
  tErr = TIO_Read_PointQuant_Chunk(fileID, quantID, m_chunkIndex,
                                   TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                   &dims[0], nullptr);
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
  tErr = TIO_Read_PointQuant_Chunk(fileID, quantID, m_chunkIndex,
                                   TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                   nullptr, dataArray->getVoidPointer());
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOQuantChunk::GetQuadQuantData(const std::string &variable,
                                           const TIO_File_t &fileID,
                                           const TIO_Object_t &quantID,
                                           const TIO_Data_t &dataType,
                                           const TIO_Size_t &nGhosts,
                                           const TIO_Bool_t &isMixed) {
  // Check the variable name is valid
  if (variable != "Pure" && variable != "Mixed") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_t tErr;
  TIO_Dims_t nDims;
  TIO_Size_t dims[3];
  TIO_Size_t nMixComp;
  tErr = TIO_Read_QuadQuant_Chunk(
      fileID, quantID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      &nDims, &dims[0], &dims[1], &dims[2], nullptr, &nMixComp, nullptr);
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
  tErr = TIO_Read_QuadQuant_Chunk(
      fileID, quantID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      nullptr, nullptr, nullptr, nullptr,
      variable == "Pure" ? dataArray->getVoidPointer() : nullptr, nullptr,
      variable == "Mixed" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOQuantChunk::GetUnstrQuantData(const std::string &variable,
                                            const TIO_File_t &fileID,
                                            const TIO_Object_t &quantID,
                                            const TIO_Data_t &dataType,
                                            const TIO_Size_t &nGhosts,
                                            const TIO_Bool_t &isMixed) {
  // Check the variable name is valid
  if (variable != "Pure" && variable != "Mixed") {
    throw TIOVariableNameException(variable);
  }

  // Get quant dimensions
  TIO_t tErr;
  TIO_Dims_t nDims = TIO_1D;
  TIO_Size_t dims[1];
  TIO_Size_t nMixComp;
  tErr = TIO_Read_UnstrQuant_Chunk(fileID, quantID, m_chunkIndex,
                                   TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                   &dims[0], nullptr, &nMixComp, nullptr);
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
  tErr = TIO_Read_UnstrQuant_Chunk(
      fileID, quantID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      nullptr, variable == "Pure" ? dataArray->getVoidPointer() : nullptr,
      nullptr, variable == "Mixed" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}
