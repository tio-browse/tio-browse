//
// src/TIOMaterial.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOMaterial.h"

#include <typhonio.h>

#include <memory>
#include <string>
#include <utility>

#include "TIOException.h"
#include "TIOInfoGroup.h"
#include "TIOInfoItem.h"
#include "TIOMaterialChunkGroup.h"
#include "TIOTypeNames.h"

TIOMaterial::TIOMaterial(const std::string &itemName, TIOTreeItem *parent)
    : TIOTreeItem(itemName, parent) {}

TIOMaterial::~TIOMaterial() {
  m_childItems.clear();

  if (m_objectID != TIO_NULL) {
    TIO_Close_Material(m_parentItem->getFileID(), m_objectID);
  }
}

void TIOMaterial::fetchMore() {
  TIOTreeItem::fetchMore();

  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getObjectID();
  TIO_Data_t datatype;
  TIO_Size_t nmat;
  TIO_Size_t nghosts;
  TIO_Bool_t ismixed;
  TIO_Data_t ncomp_datatype;
  TIO_Data_t ptr_datatype;
  TIO_Data_t vf_datatype;

  tErr = TIO_Open_Material(fileID, meshID,
                           m_itemData[0].toString().toStdString().c_str(),
                           &m_objectID, &datatype, &nmat, &nghosts, &ismixed,
                           &ncomp_datatype, &ptr_datatype, &vf_datatype);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Add material info
  auto infoGroup = std::unique_ptr<TIOInfoGroup>(
      new TIOInfoGroup("Material information", this));
  infoGroup->AddTIOInfoItem("N mat", std::to_string(nmat));
  infoGroup->AddTIOInfoItem("N ghosts",
                            TIOTypeNames::TIOGhostToString(nghosts));
  infoGroup->AddTIOInfoItem("Is mixed", std::to_string(ismixed));
  infoGroup->AddTIODataItem("Material numbers", "<Open item for data>");
  infoGroup->AddTIODataItem("Material names", "<Open item for data>");
  infoGroup->AddTIOInfoItem("Data type", TIOTypeNames::TIO_DATA[datatype]);
  infoGroup->AddTIOInfoItem("NC data type",
                            TIOTypeNames::TIO_DATA[ncomp_datatype]);
  infoGroup->AddTIOInfoItem("PTR data type",
                            TIOTypeNames::TIO_DATA[ptr_datatype]);
  infoGroup->AddTIOInfoItem("VF data type",
                            TIOTypeNames::TIO_DATA[vf_datatype]);

  // Get mesh type and n chunks from mesh
  TIO_Mesh_t meshType;
  TIO_Size_t nChunks;
  tErr = TIO_Get_Mesh_Info(fileID, meshID, nullptr, &meshType, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, &nChunks, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Get info from TIO_Read_XXXMaterial_All
  TIO_Dims_t nDims;
  TIO_Size_t nI, nJ, nK, nMixCell, nMixComp, nCells, nNodes;
  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // deliberate fall through
    case TIO_MESH_QUAD_NONCOLINEAR: {
      tErr = TIO_Read_QuadMaterial_All(
          fileID, m_objectID, TIO_DATATYPE_NULL, &nDims, &nI, &nJ, &nK, nullptr,
          TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, &nMixCell,
          &nMixComp, nullptr, nullptr, nullptr, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIODataItem("N dims", std::to_string(nDims));
      infoGroup->AddTIODataItem("N I", std::to_string(nI));
      infoGroup->AddTIODataItem("N J", std::to_string(nJ));
      infoGroup->AddTIODataItem("N K", std::to_string(nK));
      infoGroup->AddTIODataItem("N mix cell", std::to_string(nMixCell));
      infoGroup->AddTIODataItem("N mix comp", std::to_string(nMixComp));
    } break;
    case TIO_MESH_UNSTRUCT: {
      tErr = TIO_Read_UnstrMaterial_All(
          fileID, m_objectID, TIO_DATATYPE_NULL, &nCells, nullptr,
          TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, &nMixCell,
          &nMixComp, nullptr, nullptr, nullptr, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIODataItem("N cells", std::to_string(nCells));
      infoGroup->AddTIODataItem("N mix cell", std::to_string(nMixCell));
      infoGroup->AddTIODataItem("N mix comp", std::to_string(nMixComp));
    } break;
    case TIO_MESH_POINT: {
      tErr = TIO_Read_PointMaterial_All(fileID, m_objectID, TIO_DATATYPE_NULL,
                                        &nNodes, nullptr);
      if (tErr != TIO_SUCCESS) {
        throw TIOException(tErr);
      }
      infoGroup->AddTIODataItem("N nodes", std::to_string(nNodes));
    } break;
    default:
      throw TIOMeshtypeException(meshType);
  }

  infoGroup->AddTIODataItem("N chunks", std::to_string(nChunks));
  m_childItems.push_back(std::move(infoGroup));

  // Add data items
  AddTIODataItem("Material", "<Open item for data>");
  if (meshType == TIO_MESH_QUAD_COLINEAR ||
      meshType == TIO_MESH_QUAD_NONCOLINEAR || meshType == TIO_MESH_UNSTRUCT) {
    AddTIODataItem("Mixed mat",
                   ismixed ? "<Open item for data>" : "Not in file");
    AddTIODataItem("N comp", ismixed ? "<Open item for data>" : "Not in file");
    AddTIODataItem("PTR", ismixed ? "<Open item for data>" : "Not in file");
    AddTIODataItem("Vol frac",
                   ismixed ? "<Open item for data>" : "Not in file");
  }

  // Add chunks
  m_childItems.push_back(std::unique_ptr<TIOMaterialChunkGroup>(
      new TIOMaterialChunkGroup("Chunks", nChunks, this)));

  // Add vargroups and variables
  AddTIOVariables(fileID, m_objectID);
  AddTIOVargroups(fileID, m_objectID);
}

DataArray *TIOMaterial::getArrayData(const std::string &variable) {
  TIO_File_t fileID = m_parentItem->getFileID();
  TIO_Object_t meshID = m_parentItem->getMeshID();

  // Get mesh info
  TIO_t tErr;
  TIO_Mesh_t meshType;
  TIO_Dims_t nDims;
  tErr = TIO_Get_Mesh_Info(fileID, meshID, nullptr, &meshType, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr, &nDims, nullptr,
                           nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Get material info
  TIO_Data_t dataType;
  TIO_Size_t nMaterials;
  TIO_Bool_t isMixed;
  TIO_Data_t ncompDataType, ptrDataType, vfDataType;
  tErr = TIO_Get_Material_Info(fileID, m_objectID, nullptr, &dataType,
                               &nMaterials, nullptr, &isMixed, &ncompDataType,
                               &ptrDataType, &vfDataType);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Material numbers and names
  if (variable == "Material numbers" || variable == "Material names") {
    TIO_Size_t size[1] = {nMaterials};
    DataArray *dataArray =
        new DataArray(variable, TIO_1D, size, DataArray::TYPE_DATA,
                      variable == "Material numbers" ? dataType : TIO_STRING);
    if (dataArray == nullptr) {
      throw TIODataArrayException();
    }
    tErr = TIO_Get_Material_Range(
        fileID, m_objectID, dataType, nullptr,
        variable == "Material numbers" ? dataArray->getVoidPointer() : nullptr,
        variable == "Material names"
            ? static_cast<char(*)[TIO_STRLEN]>(dataArray->getVoidPointer())
            : nullptr);
    if (tErr != TIO_SUCCESS) {
      delete dataArray;
      throw TIOException(tErr);
    }
    return dataArray;
  }

  // Check for mixed status
  if (variable != "Material" && !isMixed) {
    return nullptr;
  }

  // Material data
  switch (meshType) {
    case TIO_MESH_QUAD_COLINEAR:
    // no break
    case TIO_MESH_QUAD_NONCOLINEAR:
      return GetQuadMaterialData(variable, dataType, ncompDataType, ptrDataType,
                                 vfDataType, isMixed);
      break;
    case TIO_MESH_UNSTRUCT:
      return GetUnstrMaterialData(variable, dataType, ncompDataType,
                                  ptrDataType, vfDataType, isMixed);
      break;
    case TIO_MESH_POINT:
      return GetPointMaterialData(variable, dataType);
      break;
    default:
      throw TIOMeshtypeException(meshType);
  }
}

DataArray *TIOMaterial::GetPointMaterialData(const std::string &variable,
                                             const TIO_Data_t &dataType) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Size_t nNodes[1];
  tErr = TIO_Read_PointMaterial_All(fileID, m_objectID, dataType, &nNodes[0],
                                    nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  // Create data array
  DataArray *dataArray =
      new DataArray(variable, TIO_1D, nNodes, DataArray::TYPE_DATA, dataType);
  if (dataArray == nullptr) {
    throw TIODataArrayException();
  }

  // Read material data
  tErr = TIO_Read_PointMaterial_All(fileID, m_objectID, dataType, nullptr,
                                    dataArray->getVoidPointer());
  if (tErr != TIO_SUCCESS) {
    delete dataArray;
    throw TIOException(tErr);
  }

  return dataArray;
}

DataArray *TIOMaterial::GetQuadMaterialData(const std::string &variable,
                                            const TIO_Data_t &dataType,
                                            const TIO_Data_t &ncompDataType,
                                            const TIO_Data_t &ptrDataType,
                                            const TIO_Data_t &vfDataType,
                                            const TIO_Bool_t &isMixed) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Dims_t nDims;
  TIO_Size_t nI, nJ, nK, nMixCell, nMixComp;
  tErr = TIO_Read_QuadMaterial_All(
      fileID, m_objectID, dataType, &nDims, &nI, &nJ, &nK, nullptr,
      ncompDataType, ptrDataType, vfDataType, &nMixCell, &nMixComp, nullptr,
      nullptr, nullptr, nullptr);
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
  tErr = TIO_Read_QuadMaterial_All(
      fileID, m_objectID, dataType, nullptr, nullptr, nullptr, nullptr,
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

DataArray *TIOMaterial::GetUnstrMaterialData(const std::string &variable,
                                             const TIO_Data_t &dataType,
                                             const TIO_Data_t &ncompDataType,
                                             const TIO_Data_t &ptrDataType,
                                             const TIO_Data_t &vfDataType,
                                             const TIO_Bool_t &isMixed) {
  TIO_t tErr;
  TIO_File_t fileID = m_parentItem->getFileID();

  // Get array size
  TIO_Size_t nCells, nMixCell, nMixComp;
  tErr = TIO_Read_UnstrMaterial_All(fileID, m_objectID, dataType, &nCells,
                                    nullptr, ncompDataType, ptrDataType,
                                    vfDataType, &nMixCell, &nMixComp, nullptr,
                                    nullptr, nullptr, nullptr);
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
  tErr = TIO_Read_UnstrMaterial_All(
      fileID, m_objectID, dataType, nullptr,
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
