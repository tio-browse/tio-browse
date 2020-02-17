//
// src/TIOMeshChunk.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOMeshChunk.h"

#include <typhonio.h>

#include <memory>
#include <string>
#include <utility>

#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOInfoItem.h"
#include "TIOTypeNames.h"

TIOMeshChunk::TIOMeshChunk(const std::string &itemName, const int &chunkIndex,
                           TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent), m_chunkIndex(chunkIndex) {}

TIOMeshChunk::~TIOMeshChunk() { m_childItems.clear(); }

void TIOMeshChunk::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getObjectID();

  // Get mesh info
  TIO_t tErr;
  TIO_Mesh_t meshType;
  TIO_Data_t graphDataType, coordDataType;
  tErr = TIO_Get_Mesh_Info(
      fileID, meshID, nullptr, &meshType, nullptr, nullptr, nullptr, nullptr,
      &graphDataType, &coordDataType, nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Dims_t ndims;
  TIO_Size_t nmixcell, nmixcomp;
  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // deliberate fall through
    case TIO_MESH_QUAD_NONCOLINEAR: {
      TIO_Size_t il, ih, jl, jh, kl, kh;
      tErr = TIO_Get_Quad_Chunk(fileID, meshID, m_chunkIndex, &ndims, &il, &ih,
                                &jl, &jh, &kl, &kh, &nmixcell, &nmixcomp);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      auto infoGroup = std::unique_ptr<TIOInfoGroup>(
          new TIOInfoGroup("Quad mesh chunk information", this));
      infoGroup->AddTIOInfoItem("Data type",
                                TIOTypeNames::TIO_DATA[coordDataType]);
      infoGroup->AddTIOInfoItem("N dims", std::to_string(ndims));
      infoGroup->AddTIOInfoItem("IL", std::to_string(il));
      infoGroup->AddTIOInfoItem("IH", std::to_string(ih));
      infoGroup->AddTIOInfoItem("JL", std::to_string(jl));
      infoGroup->AddTIOInfoItem("JH", std::to_string(jh));
      infoGroup->AddTIOInfoItem("KL", std::to_string(kl));
      infoGroup->AddTIOInfoItem("KH", std::to_string(kh));
      m_childItems.push_back(std::move(infoGroup));
    } break;
    case TIO_MESH_UNSTRUCT: {
      TIO_Size_t nnodes, ncells, nshapes, nconnectivity;
      TIO_Size_t nghost_nodes, nghost_cells, nghost_shapes;
      TIO_Size_t nghost_connectivity;
      tErr = TIO_Get_Unstr_Chunk(fileID, meshID, m_chunkIndex, &ndims, &nnodes,
                                 &ncells, &nshapes, &nconnectivity,
                                 &nghost_nodes, &nghost_cells, &nghost_shapes,
                                 &nghost_connectivity, &nmixcell, &nmixcomp);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      auto infoGroup = std::unique_ptr<TIOInfoGroup>(
          new TIOInfoGroup("Unstr mesh chunk information", this));
      infoGroup->AddTIOInfoItem("Data type",
                                TIOTypeNames::TIO_DATA[coordDataType]);
      infoGroup->AddTIOInfoItem("Graph data type",
                                TIOTypeNames::TIO_DATA[graphDataType]);
      infoGroup->AddTIOInfoItem("N dims", std::to_string(ndims));
      infoGroup->AddTIOInfoItem("N nodes", std::to_string(nnodes));
      infoGroup->AddTIOInfoItem("N cells", std::to_string(ncells));
      infoGroup->AddTIOInfoItem("N shapes", std::to_string(nshapes));
      infoGroup->AddTIOInfoItem("N connectivity",
                                std::to_string(nconnectivity));
      infoGroup->AddTIOInfoItem("N ghost nodes", std::to_string(nghost_nodes));
      infoGroup->AddTIOInfoItem("N ghost cells", std::to_string(nghost_cells));
      infoGroup->AddTIOInfoItem("N ghost shapes",
                                std::to_string(nghost_shapes));
      infoGroup->AddTIOInfoItem("N ghost connectivity",
                                std::to_string(nghost_connectivity));
      m_childItems.push_back(std::move(infoGroup));
    } break;
    case TIO_MESH_POINT: {
      TIO_Size_t nl, nh;
      TIO_Size_t nghost_nodes;
      tErr = TIO_Get_Point_Chunk(fileID, meshID, m_chunkIndex, &ndims, &nl, &nh,
                                 &nghost_nodes);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      auto infoGroup = std::unique_ptr<TIOInfoGroup>(
          new TIOInfoGroup("Point mesh chunk information", this));
      infoGroup->AddTIOInfoItem("Data type",
                                TIOTypeNames::TIO_DATA[coordDataType]);
      infoGroup->AddTIOInfoItem("N dims", std::to_string(ndims));
      infoGroup->AddTIOInfoItem("NL", std::to_string(nl));
      infoGroup->AddTIOInfoItem("NH", std::to_string(nh));
      infoGroup->AddTIOInfoItem("N ghosts", std::to_string(nghost_nodes));
      m_childItems.push_back(std::move(infoGroup));
    } break;
    default:
      throw TIOMeshtypeException(meshType);
  }

  // Add mesh data items
  if (ndims == TIO_1D || ndims == TIO_2D || ndims == TIO_3D) {
    AddTIODataItem("I coord", "<Open item for data>");
  }
  if (ndims == TIO_2D || ndims == TIO_3D) {
    AddTIODataItem("J coord", "<Open item for data>");
  }
  if (ndims == TIO_3D) {
    AddTIODataItem("K coord", "<Open item for data>");
  }

  if (meshType == TIO_MESH_UNSTRUCT) {
    AddTIODataItem("Node IDs", "<Open item for data>");
    AddTIODataItem("Cell IDs", "<Open item for data>");
    AddTIODataItem("Shapes", "<Open item for data>");
    AddTIODataItem("N nodes per shape", "<Open item for data>");
    AddTIODataItem("N cells per shape", "<Open item for data>");
    AddTIODataItem("Connectivity", "<Open item for data>");
  }
}

DataArray *TIOMeshChunk::getArrayData(const std::string &variable) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();
  TIO_Mesh_t meshType;
  TIO_Data_t graphDataType, coordDataType;
  tErr = TIO_Get_Mesh_Info(
      fileID, meshID, nullptr, &meshType, nullptr, nullptr, nullptr, nullptr,
      &graphDataType, &coordDataType, nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      return GetQuadMeshChunkData(variable, coordDataType);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrMeshChunkData(variable, graphDataType, coordDataType);
      break;
    case TIO_MESH_POINT:
      return GetPointMeshChunkData(variable, coordDataType);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

DataArray *TIOMeshChunk::GetPointMeshChunkData(
    const std::string &variable, const TIO_Data_t &coordDataType) {
  if (variable != "I coord" && variable != "J coord" && variable != "K coord") {
    throw TIOVariableNameException(variable);
  }
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();

  // Get array size
  TIO_Size_t nL, nH, nGhosts;
  tErr = TIO_Get_Point_Chunk(fileID, meshID, m_chunkIndex, nullptr, &nL, &nH,
                             &nGhosts);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Size_t size[1];
  size[0] = nH - nL + 1;
  if (size[0] < 1) {
    throw TIOGenericException("Invalid chunk extents");
  }

  // Create data array
  DataArray *dataArray = new DataArray(variable, TIO_1D, size,
                                       DataArray::TYPE_DATA, coordDataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  tErr = TIO_Read_PointMesh_Chunk(
      fileID, meshID, m_chunkIndex, TIO_XFER_INDEPENDENT, coordDataType,
      TIO_GHOSTS_ALL, nullptr, nullptr,
      variable == "I coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "J coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "K coord" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  return dataArray;
}

DataArray *TIOMeshChunk::GetQuadMeshChunkData(const std::string &variable,
                                              const TIO_Data_t &coordDataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();

  // Get array size
  TIO_Size_t dims[3];
  tErr = TIO_Read_QuadMesh_Chunk(fileID, meshID, m_chunkIndex,
                                 TIO_XFER_INDEPENDENT, coordDataType,
                                 TIO_GHOSTS_NONE, nullptr, &dims[0], &dims[1],
                                 &dims[2], nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Calculate array size
  TIO_Size_t arraySize[1];
  if (variable == "I coord") {
    arraySize[0] = dims[0];
  } else if (variable == "J coord") {
    arraySize[0] = dims[1];
  } else if (variable == "K coord") {
    arraySize[0] = dims[2];
  } else {
    throw TIOVariableNameException(variable);
  }

  // Create data array
  DataArray *dataArray = new DataArray(variable, TIO_1D, arraySize,
                                       DataArray::TYPE_DATA, coordDataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read mesh dat
  tErr = TIO_Read_QuadMesh_Chunk(
      fileID, meshID, m_chunkIndex, TIO_XFER_INDEPENDENT, coordDataType,
      TIO_GHOSTS_NONE, nullptr, nullptr, nullptr, nullptr,
      variable == "I coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "J coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "K coord" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}

DataArray *TIOMeshChunk::GetUnstrMeshChunkData(
    const std::string &variable, const TIO_Data_t &graphDataType,
    const TIO_Data_t &coordDataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();

  // Get array size
  TIO_Size_t nNodes, nCells, nShapes, nConnectivity;
  TIO_Size_t nGhostNodes, nGhostCells, nGhostShapes, nGhostConnectivity;
  TIO_Size_t nMixCell, nMixComp;
  tErr = TIO_Get_Unstr_Chunk(fileID, meshID, m_chunkIndex, nullptr, &nNodes,
                             &nCells, &nShapes, &nConnectivity, &nGhostNodes,
                             &nGhostCells, &nGhostShapes, &nGhostConnectivity,
                             &nMixCell, &nMixComp);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  TIO_Size_t size[1];
  TIO_Data_t dataType;
  DataArray::ArrayType arrayType;
  if (variable == "I coord" || variable == "J coord" || variable == "K coord") {
    size[0] = nNodes + nGhostNodes;
    arrayType = DataArray::TYPE_DATA;
    dataType = coordDataType;
  } else if (variable == "Node IDs") {
    size[0] = nNodes + nGhostNodes;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Cell IDs") {
    size[0] = nCells + nGhostCells;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Shapes") {
    size[0] = nShapes + nGhostShapes;
    arrayType = DataArray::TYPE_SHAPE;
    dataType = TIO_DATATYPE_NULL;
  } else if (variable == "N nodes per shape") {
    size[0] = nShapes + nGhostShapes;
    arrayType = DataArray::TYPE_NODES_PER_CELL;
    dataType = TIO_DATATYPE_NULL;
  } else if (variable == "N cells per shape") {
    size[0] = nShapes + nGhostShapes;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Connectivity") {
    size[0] = nConnectivity + nGhostConnectivity;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else {
    throw TIOVariableNameException(variable);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, TIO_1D, size, arrayType, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  tErr = TIO_Read_UnstrMesh_Chunk(
      fileID, meshID, m_chunkIndex, TIO_XFER_INDEPENDENT, graphDataType,
      coordDataType, TIO_GHOSTS_ALL, nullptr, nullptr, nullptr, nullptr,
      nullptr, variable == "Node IDs" ? dataArray->getVoidPointer() : nullptr,
      variable == "Cell IDs" ? dataArray->getVoidPointer() : nullptr,
      (variable == "Shapes" || variable == "N nodes per shape")
          ? static_cast<TIO_Shape_t *>(dataArray->getVoidPointer())
          : nullptr,
      variable == "N cells per shape" ? dataArray->getVoidPointer() : nullptr,
      variable == "Connectivity" ? dataArray->getVoidPointer() : nullptr,
      variable == "I coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "J coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "K coord" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}
