//
// ex_noncolinear_mesh.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include <typhonio.h>
#include <iostream>
#include <string>

void PrintTyphonIOErrorAndExit(const TIO_t tioError,
        const std::string& errorMessage) {
    std::cout << errorMessage << std::endl;
    std::cout << "Error code: " << tioError << std::endl;
    char tioErrorMessage[TIO_STRLEN];
    TIO_Get_Error(tioError, tioErrorMessage);
    std::cout << tioErrorMessage << std::endl;
    exit(-1);
}

int main(int argc, char* argv[]) {
    TIO_t tioError;
    TIO_File_t fileID;
    std::string fileName = "ex_noncolinear_mesh.h5";
    std::string codeName = "ex_noncolinear_mesh";
    std::string version = "0.0.1";
    std::string date = "2016/11/03";
    std::string title = "TyphonIO non-colinear mesh example";
    tioError = TIO_Create(fileName.c_str(), &fileID, TIO_ACC_REPLACE,
            codeName.c_str(), version.c_str(), date.c_str(), title.c_str(),
            MPI_COMM_NULL, MPI_INFO_NULL, MPI_PROC_NULL);
    if (tioError != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create");
    }

    TIO_Object_t stateID;
    tioError = TIO_Create_State(fileID, "state000", &stateID, 0, 0.0,
            "seconds");
    if (tioError != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_State");
    }

    TIO_Object_t meshID;
    tioError = TIO_Create_Mesh(fileID, stateID, "mesh", &meshID,
        TIO_MESH_QUAD_NONCOLINEAR, TIO_COORD_CARTESIAN, TIO_FALSE, "mesh", 1,
        TIO_DATATYPE_NULL, TIO_DOUBLE, TIO_3D, 3, 4, 5, 0, 1, "metres",
        "metres", "metres", "x", "y", "z");
    if (tioError != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Create_Mesh");
    }

    tioError = TIO_Set_Quad_Chunk(fileID, meshID, 0, TIO_3D, 0, 2, 0, 3, 0, 4,
        0, 0);
    if (tioError != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tioError, "Error in TIO_Set_Quad_Chunk");
    }

    double iCoords[] = {0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0,
        2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0,
        1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0,
        0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0,
        2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0};
    double jCoords[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0,
            3.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0,
            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 0.0,
            0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 0.0, 0.0,
            0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0};
    double kCoords[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0,
            3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 4.0, 4.0,
            4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0};

    tioError = TIO_Write_QuadMesh_Chunk(fileID, meshID, 0, TIO_XFER_INDEPENDENT,
        TIO_DOUBLE, iCoords, jCoords, kCoords);
    if (tioError != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tioError,
                "Error in TIO_Write_QuadMesh_Chunk");
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



