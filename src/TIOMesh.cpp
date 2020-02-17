//
// src/TIOMesh.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOMesh.h"

#include <typhonio.h>

#include <memory>
#include <string>
#include <utility>

#include "TIODataItem.h"
#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOInfoItem.h"
#include "TIOMaterial.h"
#include "TIOMeshChunkGroup.h"
#include "TIOQuantGroup.h"
#include "TIOTypeNames.h"

TIOMesh::TIOMesh(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent), m_meshType(TIO_MESH_NULL) {}

TIOMesh::~TIOMesh() {
  m_childItems.clear();

  // TODO(jim): Replace this with a call to 'getParentFileID' which would
  // TODO(jim): ... check that m_parentItem is a valid pointer first
  TIO_File_t fileID = m_parentItem->getFileID();
  if (fileID != TIO_NULL && m_objectID != TIO_NULL) {
    TIO_Close_Mesh(fileID, m_objectID);
    m_objectID = TIO_NULL;
  }
}

void TIOMesh::fetchMore() {
  TIO_t tErr;
  TIO_Coord_t coordtype;
  TIO_Bool_t isAMR;
  char group[TIO_STRLEN];
  TIO_Size_t order;
  TIO_Data_t graph_datatype;
  TIO_Data_t coord_datatype;
  TIO_Dims_t ndims;
  TIO_Size_t n1;
  TIO_Size_t n2;
  TIO_Size_t n3;
  TIO_Size_t n4;
  TIO_Size_t nchunks;
  char iunits[TIO_STRLEN];
  char junits[TIO_STRLEN];
  char kunits[TIO_STRLEN];
  char ilabel[TIO_STRLEN];
  char jlabel[TIO_STRLEN];
  char klabel[TIO_STRLEN];

  TIOTreeItem::fetchMore();

  // Get file ID and state ID from parent
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_File_t stateID = m_parentItem->getObjectID();

  tErr = TIO_Open_Mesh(
      fileID, stateID, m_itemData[0].toString().toStdString().c_str(),
      &m_objectID, &m_meshType, &coordtype, &isAMR, group, &order,
      &graph_datatype, &coord_datatype, &ndims, &n1, &n2, &n3, &n4, &nchunks,
      iunits, junits, kunits, ilabel, jlabel, klabel);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add mesh info
  auto infoGroup =
      std::unique_ptr<TIOInfoGroup>(new TIOInfoGroup("Mesh information", this));
  infoGroup->AddTIOInfoItem("Mesh type", TIOTypeNames::TIO_MESH[m_meshType]);
  infoGroup->AddTIOInfoItem("Coord type", TIOTypeNames::TIO_COORD[coordtype]);
  infoGroup->AddTIOInfoItem("Is AMR", TIOTypeNames::TIO_BOOL[isAMR]);
  infoGroup->AddTIOInfoItem("Group", group);
  infoGroup->AddTIOInfoItem("Order", std::to_string(order));
  infoGroup->AddTIOInfoItem("Data type",
                            TIOTypeNames::TIO_DATA[coord_datatype]);
  infoGroup->AddTIOInfoItem("Graph data type",
                            TIOTypeNames::TIO_DATA[graph_datatype]);
  infoGroup->AddTIOInfoItem("N dims", std::to_string(ndims));
  infoGroup->AddTIOInfoItem("N1", std::to_string(n1));

  if (m_meshType == TIO_MESH_QUAD_COLINEAR ||
      m_meshType == TIO_MESH_QUAD_NONCOLINEAR ||
      m_meshType == TIO_MESH_UNSTRUCT) {
    infoGroup->AddTIOInfoItem("N2", std::to_string(n2));
    infoGroup->AddTIOInfoItem("N3", std::to_string(n3));
  }

  if (m_meshType == TIO_MESH_QUAD_NONCOLINEAR ||
      m_meshType == TIO_MESH_UNSTRUCT) {
    infoGroup->AddTIOInfoItem("N4", std::to_string(n4));
  }

  infoGroup->AddTIOInfoItem("I label", ilabel);
  infoGroup->AddTIOInfoItem("J label", jlabel);
  infoGroup->AddTIOInfoItem("K label", klabel);
  infoGroup->AddTIOInfoItem("I units", iunits);
  infoGroup->AddTIOInfoItem("J units", junits);
  infoGroup->AddTIOInfoItem("K units", kunits);

  // Get the mesh range information
  long double xl, xh, yl, yh, zl, zh;
  std::string xlString = "Not in file";
  std::string xhString = "Not in file";
  std::string ylString = "Not in file";
  std::string yhString = "Not in file";
  std::string zlString = "Not in file";
  std::string zhString = "Not in file";
  tErr = TIO_Get_Mesh_Range(fileID, m_objectID, TIO_LDOUBLE, nullptr, &xl, &xh,
                            &yl, &yh, &zl, &zh);
  if (tErr == TIO_SUCCESS) {
    if (ndims >= 1) {
      xlString = std::to_string(xl);
      xhString = std::to_string(xh);
    }
    if (ndims >= 2) {
      ylString = std::to_string(yl);
      yhString = std::to_string(yh);
    }
    if (ndims >= 3) {
      zlString = std::to_string(zl);
      zhString = std::to_string(zh);
    }
  } else if (tErr == TIO_ERR_INVALID_RANGE) {
    // TIO_Get_Mesh_Range will return TIO_ERR_INVALID_RANGE if there is no
    // range information in the file
  } else {
    throw TIOException(tErr);
  }

  infoGroup->AddTIOInfoItem("IL", xlString);
  infoGroup->AddTIOInfoItem("IH", xhString);
  infoGroup->AddTIOInfoItem("JL", ylString);
  infoGroup->AddTIOInfoItem("JH", yhString);
  infoGroup->AddTIOInfoItem("KL", zlString);
  infoGroup->AddTIOInfoItem("KH", zhString);
  infoGroup->AddTIOInfoItem("N chunks", std::to_string(nchunks));
  m_childItems.push_back(std::move(infoGroup));

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

  if (m_meshType == TIO_MESH_UNSTRUCT) {
    AddTIODataItem("Node IDs", "<Open item for data>");
    AddTIODataItem("Cell IDs", "<Open item for data>");
    AddTIODataItem("Shapes", "<Open item for data>");
    AddTIODataItem("N nodes per shape", "<Open item for data>");
    AddTIODataItem("N cells per shape", "<Open item for data>");
    AddTIODataItem("Connectivity", "<Open item for data>");
  }

  // Add chunks
  m_childItems.push_back(std::unique_ptr<TIOMeshChunkGroup>(
      new TIOMeshChunkGroup("Chunks", nchunks, this)));

  // Add materials
  char materialName[TIO_STRLEN];
  tErr = TIO_List_Material(fileID, m_objectID, materialName);
  if (tErr == TIO_SUCCESS) {
    m_childItems.push_back(
        std::unique_ptr<TIOMaterial>(new TIOMaterial(materialName, this)));
  } else if (tErr == TIO_FAIL) {
    // TIO_List_Material will return TIO_FAIL if no material exists
  } else {
    throw TIOException(tErr);
  }

  // Add Quants
  TIO_Size_t nQuants;
  tErr = TIO_List_Quants(fileID, m_objectID, &nQuants, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  if (nQuants > 0) {
    m_childItems.push_back(
        std::unique_ptr<TIOQuantGroup>(new TIOQuantGroup("Quantities", this)));
  }

  // Add variables and vargroups
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}

DataArray *TIOMesh::getArrayData(const std::string &variable) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Mesh_t meshType;
  TIO_Data_t graphDataType, coordDataType;
  tErr = TIO_Get_Mesh_Info(fileID, m_objectID, nullptr, &meshType, nullptr,
                           nullptr, nullptr, nullptr, &graphDataType,
                           &coordDataType, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      return GetQuadMeshData(variable, coordDataType);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrMeshData(variable, graphDataType, coordDataType);
      break;
    case TIO_MESH_POINT:
      return GetPointMeshData(variable, coordDataType);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

TIO_Object_t TIOMesh::getMeshID() { return m_objectID; }

DataArray *TIOMesh::GetPointMeshData(const std::string &variable,
                                     const TIO_Data_t &coordDataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Size_t dims[1];
  tErr = TIO_Read_PointMesh_All(fileID, m_objectID, coordDataType, nullptr,
                                &dims[0], nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray = new DataArray(variable, TIO_1D, dims,
                                       DataArray::TYPE_DATA, coordDataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read mesh data
  tErr = TIO_Read_PointMesh_All(
      fileID, m_objectID, coordDataType, nullptr, nullptr,
      variable == "I coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "J coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "K coord" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}

DataArray *TIOMesh::GetQuadMeshData(const std::string &variable,
                                    const TIO_Data_t &coordDataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Size_t dims[3];
  tErr = TIO_Read_QuadMesh_All(fileID, m_objectID, coordDataType, nullptr,
                               &dims[0], &dims[1], &dims[2], nullptr, nullptr,
                               nullptr);
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

  // Read mesh data
  tErr = TIO_Read_QuadMesh_All(
      fileID, m_objectID, coordDataType, nullptr, nullptr, nullptr, nullptr,
      variable == "I coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "J coord" ? dataArray->getVoidPointer() : nullptr,
      variable == "K coord" ? dataArray->getVoidPointer() : nullptr);
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }
  return dataArray;
}

DataArray *TIOMesh::GetUnstrMeshData(const std::string &variable,
                                     const TIO_Data_t &graphDataType,
                                     const TIO_Data_t &coordDataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Size_t nNodes, nCells, nShapes, nConnectivity;
  tErr = TIO_Read_UnstrMesh_All(
      fileID, m_objectID, graphDataType, coordDataType, nullptr, &nNodes,
      &nCells, &nShapes, &nConnectivity, nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Calculate array size
  TIO_Size_t size[1];
  TIO_Data_t dataType;
  DataArray::ArrayType arrayType;
  if (variable == "I coord" || variable == "J coord" || variable == "K coord") {
    size[0] = nNodes;
    arrayType = DataArray::TYPE_DATA;
    dataType = coordDataType;
  } else if (variable == "Node IDs") {
    size[0] = nNodes;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Cell IDs") {
    size[0] = nCells;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Shapes") {
    size[0] = nShapes;
    arrayType = DataArray::TYPE_SHAPE;
    dataType = TIO_DATATYPE_NULL;
  } else if (variable == "N nodes per shape") {
    size[0] = nShapes;
    arrayType = DataArray::TYPE_NODES_PER_CELL;
    dataType = TIO_DATATYPE_NULL;
  } else if (variable == "N cells per shape") {
    size[0] = nShapes;
    arrayType = DataArray::TYPE_DATA;
    dataType = graphDataType;
  } else if (variable == "Connectivity") {
    size[0] = nConnectivity;
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

  // Read mesh data
  tErr = TIO_Read_UnstrMesh_All(
      fileID, m_objectID, graphDataType, coordDataType, nullptr, nullptr,
      nullptr, nullptr, nullptr,
      variable == "Node IDs" ? dataArray->getVoidPointer() : nullptr,
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
