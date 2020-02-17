//
// ex_interface_mesh.cpp
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

const int nCells0 = 10;
const int nCells1 = 16;
const int nCells2 = 8;
const int nCells3 = 8;

double points0[] = {-2, 0, 0, -2, 1, 0, 0,  0, 0, 0,  1, 0, -2, 1, 0,
                    -2, 2, 0, 0,  1, 0, 0,  2, 0, -2, 2, 0, -2, 3, 0,
                    0,  2, 0, 0,  3, 0, -2, 3, 0, -2, 4, 0, 0,  3, 0,
                    0,  4, 0, -2, 4, 0, -2, 5, 0, 0,  4, 0, 0,  5, 0};

double points1[] = {0,       0,       0, 0,      1,      0, 0,       1,       0,
                    0,       2,       0, 0,      2,      0, 0,       3,       0,
                    0,       3,       0, 0,      4,      0, 0,       4,       0,
                    0,       5,       0, 2.5,    1,      0, 2.5,     0,       0,
                    2.64286, 2,       0, 2.5,    1,      0, 2.83333, 2.44444, 0,
                    2.64286, 2,       0, 3,      2.5,    0, 2.83333, 2.44444, 0,
                    3.42857, 2.42857, 0, 3,      2.5,    0, 2.83333, 2.44444, 0,
                    2.6875,  2.6875,  0, 2.6875, 2.6875, 0, 2.64286, 3,       0,
                    2.64286, 3,       0, 2.5,    4,      0, 4,       2.5,     0,
                    3.42857, 2.42857, 0, 5,      2.5,    0, 4,       2.5,     0,
                    2.5,     4,       0, 2.5,    5,      0};

double points2[] = {-2, 3, 0, -2, 4,  0, 0, 3,  0,  0, 4, 0,  -2, 4, 0, -2,
                    5,  0, 0, 4,  0,  0, 5, 0,  -2, 5, 0, -2, 6,  0, 0, 5,
                    0,  0, 6, 0,  -2, 6, 0, -2, 7,  0, 0, 6,  0,  0, 7, 0};

double points3[] = {0,   3, 0, 0,   4, 0, 0,   4, 0, 0,   5, 0,
                    0,   5, 0, 0,   6, 0, 0,   6, 0, 0,   7, 0,
                    2.5, 3, 0, 2.5, 4, 0, 2.5, 4, 0, 2.5, 5, 0,
                    2.5, 5, 0, 2.5, 6, 0, 2.5, 6, 0, 2.5, 7, 0};

int mats0[] = {4, 3, 3, 2, 4, 3, 3, 2, 4, 3, 3, 2, 4, 3, 3, 2, 4, 3, 3, 2};

int mats1[] = {3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 2, 1, 2, 1, 2, 1,
               3, 1, 3, 1, 2, 3, 2, 3, 2, 3, 3, 1, 3, 1, 2, 3};

int mats2[] = {4, 3, 3, 2, 4, 3, 3, 2, 4, 3, 3, 2, 4, 3, 3, 2};

int mats3[] = {3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3};

void PrintTyphonIOErrorAndExit(const TIO_t tioError,
                               const std::string &errorMessage) {
  std::cout << errorMessage << std::endl;
  std::cout << "Error code: " << tioError << std::endl;
  char tioErrorMessage[TIO_STRLEN];
  TIO_Get_Error(tioError, tioErrorMessage);
  std::cout << tioErrorMessage << std::endl;
  exit(-1);
}

void CreatePolygon(const TIO_File_t fileID, const TIO_Object_t &interfaceID,
                   const std::string polygonName, TIO_Size_t nCells,
                   double *points, int *materials) {
  double *x1 = new double[nCells];
  double *y1 = new double[nCells];
  double *x2 = new double[nCells];
  double *y2 = new double[nCells];
  int *matL = new int[nCells];
  int *matR = new int[nCells];

  for (int i = 0; i < nCells; ++i) {
    x1[i] = points[i * 6];
    y1[i] = points[i * 6 + 1];
    x2[i] = points[i * 6 + 3];
    y2[i] = points[i * 6 + 4];
    matL[i] = materials[i * 2];
    matR[i] = materials[i * 2 + 1];
  }

  TIO_t tioError;
  TIO_Object_t polygonID;
  tioError = TIO_Create_Polygon(fileID, interfaceID, polygonName.c_str(),
                                nCells, &polygonID, "metres");
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Polygon");
  }

  tioError =
      TIO_Write_Polygon(fileID, polygonID, nCells, x1, y1, x2, y2, matL, matR);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Polygon");
  }

  tioError = TIO_Close_Polygon(fileID, polygonID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close_Polygon");
  }

  delete[] x1;
  delete[] y1;
  delete[] x2;
  delete[] y2;
  delete[] matL;
  delete[] matR;
}

int main(int argc, char *argv[]) {
  TIO_t tioError;
  TIO_File_t fileID;
  std::string fileName = "ex_interface_mesh.h5";
  std::string codeName = "ex_interface_mesh";
  std::string version = "0.0.1";
  std::string date = "2016/11/03";
  std::string title = "TyphonIO interface mesh example";
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

  TIO_Object_t interfaceID;
  tioError = TIO_Create_Interface(fileID, stateID, "material interfaces",
                                  &interfaceID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Interface");
  }

  CreatePolygon(fileID, interfaceID, "polygon0", nCells0, points0, mats0);
  CreatePolygon(fileID, interfaceID, "polygon1", nCells1, points1, mats1);
  CreatePolygon(fileID, interfaceID, "polygon2", nCells2, points2, mats2);
  CreatePolygon(fileID, interfaceID, "polygon3", nCells3, points3, mats3);

  tioError = TIO_Close_Interface(fileID, interfaceID);
  if (tioError != TIO_SUCCESS) {
    PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Close_Interface");
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
