//
// src/TIOMaterialChunk.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOMaterialChunk.h"

#include <memory>
#include <string>
#include <utility>

#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOTypeNames.h"

TIOMaterialChunk::TIOMaterialChunk(const std::string &itemName,
                                   const int &chunkIndex, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent), m_chunkIndex(chunkIndex) {}

TIOMaterialChunk::~TIOMaterialChunk() { m_childItems.clear(); }

void TIOMaterialChunk::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t materialID = m_parentItem->getObjectID();

  // Get material info
  TIO_t tErr;
  TIO_Data_t dataType;
  TIO_Size_t nGhosts;
  TIO_Bool_t isMixed;
  TIO_Data_t ncompDataType, ptrDataType, vfDataType;
  tErr = TIO_Get_Material_Info(fileID, materialID, nullptr, &dataType, nullptr,
                               &nGhosts, &isMixed, &ncompDataType, &ptrDataType,
                               &vfDataType);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add material chunk info
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Material chunk information", this));
  infoGroup->AddTIOInfoItem("Data type", TIOTypeNames::TIO_DATA[dataType]);

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
    // no break;
    case TIO_MESH_QUAD_NONCOLINEAR: {
      TIO_Dims_t nDims;
      TIO_Size_t nI, nJ, nK, nMixCell, nMixComp;
      tErr = TIO_Read_QuadMaterial_Chunk(
          fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType,
          nGhosts, &nDims, &nI, &nJ, &nK, nullptr, ncompDataType, ptrDataType,
          vfDataType, &nMixCell, &nMixComp, nullptr, nullptr, nullptr, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N dims", std::to_string(nDims));
      infoGroup->AddTIODataItem("N I", std::to_string(nI));
      infoGroup->AddTIODataItem("N J", std::to_string(nJ));
      infoGroup->AddTIODataItem("N K", std::to_string(nK));
      infoGroup->AddTIODataItem("N ghosts",
                                TIOTypeNames::TIOGhostToString(nGhosts));
      infoGroup->AddTIODataItem("N mix cell", std::to_string(nMixCell));
      infoGroup->AddTIODataItem("N mix comp", std::to_string(nMixComp));
    } break;
    case TIO_MESH_UNSTRUCT: {
      TIO_Size_t nCells, nMixCell, nMixComp;
      tErr = TIO_Read_UnstrMaterial_Chunk(
          fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType,
          nGhosts, &nCells, nullptr, ncompDataType, ptrDataType, vfDataType,
          &nMixCell, &nMixComp, nullptr, nullptr, nullptr, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N cells", std::to_string(nCells));
      infoGroup->AddTIODataItem("N ghosts",
                                TIOTypeNames::TIOGhostToString(nGhosts));
      infoGroup->AddTIOInfoItem("N mix cell", std::to_string(nMixCell));
      infoGroup->AddTIOInfoItem("N mix comp", std::to_string(nMixComp));
    } break;
    case TIO_MESH_POINT: {
      TIO_Size_t nNodes;
      tErr = TIO_Read_PointMaterial_Chunk(fileID, materialID, m_chunkIndex,
                                          TIO_XFER_INDEPENDENT, dataType,
                                          nGhosts, &nNodes, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N nodes", std::to_string(nNodes));
      TIO_Size_t nL, nH;
      tErr = TIO_Get_Point_Chunk(fileID, meshID, m_chunkIndex, nullptr, &nL,
                                 &nH, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIOInfoItem("N L", std::to_string(nL));
      infoGroup->AddTIOInfoItem("N H", std::to_string(nH));
      infoGroup->AddTIODataItem("N ghosts",
                                TIOTypeNames::TIOGhostToString(nGhosts));
    } break;
    default:
      throw TIOMeshtypeException(meshType);
  }

  if (meshType == TIO_MESH_QUAD_COLINEAR ||
      meshType == TIO_MESH_QUAD_NONCOLINEAR || meshType == TIO_MESH_UNSTRUCT) {
    infoGroup->AddTIOInfoItem("NC data type",
                              TIOTypeNames::TIO_DATA[ncompDataType]);
    infoGroup->AddTIOInfoItem("PTR data type",
                              TIOTypeNames::TIO_DATA[ptrDataType]);
    infoGroup->AddTIOInfoItem("VF data type",
                              TIOTypeNames::TIO_DATA[vfDataType]);
  }

  m_childItems.push_back(std::move(infoGroup));

  // Add data items
  AddTIODataItem("Material", "<Open item for data>");
  if (meshType == TIO_MESH_QUAD_COLINEAR ||
      meshType == TIO_MESH_QUAD_NONCOLINEAR || meshType == TIO_MESH_UNSTRUCT) {
    AddTIODataItem("Mixed mat", isMixed == TIO_TRUE ? "<Open item for data>"
                                                    : "<Not in file>");
    AddTIODataItem("N comp", isMixed == TIO_TRUE ? "<Open item for data>"
                                                 : "<Not in file>");
    AddTIODataItem(
        "PTR", isMixed == TIO_TRUE ? "<Open item for data>" : "<Not in file>");
    AddTIODataItem("Vol frac", isMixed == TIO_TRUE ? "<Open item for data>"
                                                   : "<Not in file>");
  }
}

DataArray *TIOMaterialChunk::getArrayData(const std::string &variable) {
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();
  TIO_Object_t materialID = m_parentItem->getObjectID();

  // Get mesh type
  TIO_Mesh_t meshType;
  TIO_t tErr = TIO_Get_Mesh_Info(
      fileID, meshID, nullptr, &meshType, nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Get material data types
  TIO_Data_t dataType, ncompDataType, ptrDataType, vfDataType;
  TIO_Size_t nGhosts;
  TIO_Bool_t isMixed;
  tErr = TIO_Get_Material_Info(fileID, materialID, nullptr, &dataType, nullptr,
                               &nGhosts, &isMixed, &ncompDataType, &ptrDataType,
                               &vfDataType);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Check for mixed status
  if (variable != "Material" && !isMixed) {
    return nullptr;
  }

  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      return GetQuadMaterialChunkData(variable, dataType, ncompDataType,
                                      ptrDataType, vfDataType, nGhosts,
                                      isMixed);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrMaterialChunkData(variable, dataType, ncompDataType,
                                       ptrDataType, vfDataType, nGhosts,
                                       isMixed);
      break;
    case TIO_MESH_POINT:
      return GetPointMaterialChunkData(variable, dataType, nGhosts);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

DataArray *TIOMaterialChunk::GetPointMaterialChunkData(
    const std::string &variable, const TIO_Data_t &dataType,
    const TIO_Size_t &nGhosts) {
  if (variable != "Material") {
    throw TIOVariableNameException(variable);
  }

  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t materialID = m_parentItem->getObjectID();

  // Get array size
  TIO_Size_t size[1];
  tErr = TIO_Read_PointMaterial_Chunk(fileID, materialID, m_chunkIndex,
                                      TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                      &size[0], nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, TIO_1D, size, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read material data
  tErr = TIO_Read_PointMaterial_Chunk(fileID, materialID, m_chunkIndex,
                                      TIO_XFER_INDEPENDENT, dataType, nGhosts,
                                      nullptr, dataArray->getVoidPointer());
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOMaterialChunk::GetQuadMaterialChunkData(
    const std::string &variable, const TIO_Data_t &dataType,
    const TIO_Data_t &ncompDataType, const TIO_Data_t &ptrDataType,
    const TIO_Data_t &vfDataType, const TIO_Size_t &nGhosts,
    const TIO_Bool_t &isMixed) {
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t materialID = m_parentItem->getObjectID();

  // Get array size
  TIO_Dims_t nDims;
  TIO_Size_t nI, nJ, nK, nMixCell, nMixComp;
  TIO_t tErr = TIO_Read_QuadMaterial_Chunk(
      fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      &nDims, &nI, &nJ, &nK, nullptr, ncompDataType, ptrDataType, vfDataType,
      &nMixCell, &nMixComp, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Calculate array size
  TIO_Size_t size[3];
  TIO_Data_t arrayDataType;
  if (variable == "Material") {
    size[0] = nI;
    size[1] = nJ;
    size[2] = nK;
    arrayDataType = dataType;
  } else if (variable == "Mixed mat") {
    size[0] = nMixComp;
    arrayDataType = dataType;
  } else if (variable == "N comp") {
    size[0] = nMixCell;
    arrayDataType = ncompDataType;
  } else if (variable == "PTR") {
    size[0] = nMixCell;
    arrayDataType = ptrDataType;
  } else if (variable == "Vol frac") {
    size[0] = nMixComp;
    arrayDataType = vfDataType;
  } else {
    throw TIOVariableNameException(variable);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, variable == "Material" ? nDims : TIO_1D, size,
                    DataArray::TYPE_DATA, arrayDataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read material data
  tErr = TIO_Read_QuadMaterial_Chunk(
      fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      nullptr, nullptr, nullptr, nullptr,
      variable == "Material" ? dataArray->getVoidPointer() : nullptr,
      ncompDataType, ptrDataType, vfDataType, nullptr, nullptr,
      variable == "N comp" ? dataArray->getVoidPointer() : nullptr,
      variable == "PTR" ? dataArray->getVoidPointer() : nullptr,
      variable == "Mixed mat" ? dataArray->getVoidPointer() : nullptr,
      variable == "Vol frac" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}

DataArray *TIOMaterialChunk::GetUnstrMaterialChunkData(
    const std::string &variable, const TIO_Data_t &dataType,
    const TIO_Data_t &ncompDataType, const TIO_Data_t &ptrDataType,
    const TIO_Data_t &vfDataType, const TIO_Size_t &nGhosts,
    const TIO_Bool_t &isMixed) {
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t materialID = m_parentItem->getObjectID();

  // Get array size
  TIO_Size_t nCells, nMixCell, nMixComp;
  TIO_t tErr = TIO_Read_UnstrMaterial_Chunk(
      fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      &nCells, nullptr, ncompDataType, ptrDataType, vfDataType, &nMixCell,
      &nMixComp, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Calculate array size
  TIO_Size_t size[1];
  TIO_Data_t arrayDataType;
  if (variable == "Material") {
    size[0] = nCells;
    arrayDataType = dataType;
  } else if (variable == "Mixed mat") {
    size[0] = nMixComp;
    arrayDataType = dataType;
  } else if (variable == "N comp") {
    size[0] = nMixCell;
    arrayDataType = ncompDataType;
  } else if (variable == "PTR") {
    size[0] = nMixCell;
    arrayDataType = ptrDataType;
  } else if (variable == "Vol frac") {
    size[0] = nMixComp;
    arrayDataType = vfDataType;
  } else {
    throw TIOVariableNameException(variable);
  }

  // Create data array
  DataArray *dataArray = new DataArray(variable, TIO_1D, size,
                                       DataArray::TYPE_DATA, arrayDataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read material data
  tErr = TIO_Read_UnstrMaterial_Chunk(
      fileID, materialID, m_chunkIndex, TIO_XFER_INDEPENDENT, dataType, nGhosts,
      nullptr, variable == "Material" ? dataArray->getVoidPointer() : nullptr,
      ncompDataType, ptrDataType, vfDataType, nullptr, nullptr,
      variable == "N comp" ? dataArray->getVoidPointer() : nullptr,
      variable == "PTR" ? dataArray->getVoidPointer() : nullptr,
      variable == "Mixed mat" ? dataArray->getVoidPointer() : nullptr,
      variable == "Vol frac" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
    return nullptr;
  }
  return dataArray;
}
