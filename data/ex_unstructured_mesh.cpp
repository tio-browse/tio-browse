//
// ex_unstructured_mesh.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include <typhonio.h>

#include <iostream>
#include <string>

void PrintTyphonIOErrorAndExit(const TIO_t tioError,
                               const std::string &errorMessage) {
  std::cout << errorMessage << std::endl;
  std::cout << "Error code: " << tioError << std::endl;
  char tioErrorMessage[TIO_STRLEN];
  TIO_Get_Error(tioError, tioErrorMessage);
  std::cout << tioErrorMessage << std::endl;
  exit(-1);
}

int main(int argc, char *argv[]) {
  TIO_t tioError;
  TIO_File_t fileID;
  std::string fileName = "ex_unstructured_mesh.h5";
  std::string codeName = "ex_unstructured_mesh";
  std::string version = "0.0.1";
  std::string date = "2017/01/05";
  std::string title = "TyphonIO unstructured example";
  tioError =
      TIO_Create(fileName.c_str(), &fileID, TIO_ACC_REPLACE, codeName.c_str(),
                 version.c_str(), date.c_str(), title.c_str(), MPI_COMM_NULL,
                 MPI_INFO_NULL, MPI_PROC_NULL);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create");
  }

  TIO_Object_t stateID;
  tioError = TIO_Create_State(fileID, "state000", &stateID, 0, 0.0, "seconds");
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_State");
  }

  TIO_Object_t meshID;
  TIO_Size_t nNodes = 32;
  TIO_Size_t nCells = 7;
  TIO_Size_t nShapes = 1;
  TIO_Size_t nConnectivity = 56;
  TIO_Size_t nChunks = 1;
  tioError = TIO_Create_Mesh(fileID, stateID, "mesh", &meshID,
                             TIO_MESH_UNSTRUCT, TIO_COORD_CARTESIAN, TIO_FALSE,
                             "mesh", 1, TIO_INT, TIO_DOUBLE, TIO_3D, nNodes,
                             nCells, nShapes, nConnectivity, nChunks, "metres",
                             "metres", "metres", "x", "y", "z");
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Mesh");
  }

  tioError = TIO_Set_Unstr_Chunk(fileID, meshID, 0, TIO_3D, nNodes, nCells,
                                 nShapes, nConnectivity, 0, 0, 0, 0, 0, 0);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Set_Quad_Chunk");
  }

  int nodeIDs[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                   12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                   23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
  int cellIDs[] = {1, 2, 3, 4, 5, 6, 7};
  TIO_Shape_t shapes[] = {TIO_SHAPE_HEX8};
  TIO_Size_t nCellsPerShape[] = {7};
  int connectivity[] = {1,  2,  3,  4,  5,  6,  7,  8,  2,  9,  10, 3,  6,  11,
                        12, 7,  5,  6,  7,  8,  13, 14, 15, 16, 13, 14, 15, 16,
                        17, 18, 19, 20, 3,  21, 22, 4,  7,  23, 24, 8,  21, 25,
                        26, 22, 23, 27, 28, 24, 25, 29, 30, 26, 27, 31, 32, 28};
  double iCoords[] = {0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 2.0, 2.0, 2.0,
                      2.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0,
                      1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};
  double jCoords[] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0,
                      1.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 3.0, 0.0, 0.0,
                      1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0};
  double kCoords[] = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0,
                      1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0,
                      2.0, 2.0, 3.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 4.0};

  tioError = TIO_Write_UnstrMesh_Chunk(
      fileID, meshID, 0, TIO_XFER_INDEPENDENT, TIO_INT, TIO_DOUBLE, nodeIDs,
      cellIDs, shapes, nCellsPerShape, connectivity, iCoords, jCoords, kCoords);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Write_QuadMesh_Chunk");
  }

  TIO_Object_t quantID;
  tioError = TIO_Create_Quant(fileID, meshID, "N neighbours", &quantID, TIO_INT,
                              TIO_CENTRE_CELL, 0, TIO_FALSE, "unitless");
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Quant");
  }

  int nNeighbours[] = {3, 1, 2, 1, 2, 2, 1};
  tioError = TIO_Write_UnstrQuant_Chunk(
      fileID, quantID, 0, TIO_XFER_INDEPENDENT, TIO_INT, nNeighbours, nullptr);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Write_UnstrQuant_Chunk");
  }

  tioError = TIO_Close_Quant(fileID, quantID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close_Quant");
  }

  tioError = TIO_Close_Mesh(fileID, meshID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close_Mesh");
  }

  tioError = TIO_Close_State(fileID, stateID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close_State");
  }

  tioError = TIO_Close(fileID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close");
  }
  return 0;
}
