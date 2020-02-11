//
// ex_vargroup.cpp
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

void PrintTyphonIOErrorAndExit(const TIO_t tErr,
        const std::string& errorMessage) {
    std::cout << errorMessage << std::endl;
    std::cout << "Error code: " << tErr << std::endl;
    char tErrMessage[TIO_STRLEN];
    TIO_Get_Error(tErr, tErrMessage);
    std::cout << tErrMessage << std::endl;
    exit(-1);
}

void CreateVariable(const TIO_File_t &fileID, const TIO_Object_t &objectID,
        const std::string &name, const TIO_Data_t &dataType,
        const TIO_Dims_t &nDims, const TIO_Size_t *dims,
        const std::string &units, const void *data) {
    TIO_Object_t varID;
    TIO_t tErr = TIO_Create_Variable(fileID, objectID, name.c_str(), &varID,
            dataType, nDims, dims, units.c_str());
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Variable");
    }
    tErr = TIO_Write_Variable(fileID, varID, dataType, data);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Write_Variable");
    }
    tErr = TIO_Close_Variable(fileID, varID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Variable");
    }
}

void CreateVersionNumberVargroup(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    TIO_Object_t vargroupID;
    TIO_t tErr = TIO_Create_Vargroup(fileID, objectID, "version", &vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    CreateVariable(fileID, vargroupID, "major version", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){1});
    CreateVariable(fileID, vargroupID, "minor version", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){2});
    CreateVariable(fileID, vargroupID, "patch version", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){3});
    tErr = TIO_Close_Vargroup(fileID, vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }
}

void CreateColorsVariable(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    char colors[7][TIO_STRLEN] = {"red", "orange", "yellow", "green", "blue",
            "indigo", "violet"};
    CreateVariable(fileID, objectID, "colors", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){7}, "", colors);
}

void CreateMagicSquareVariable(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    int magicSquare[3][3] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    CreateVariable(fileID, objectID, "magic square", TIO_INT, TIO_2D,
            (const TIO_Size_t[] ) { 3, 3 }, "", magicSquare);
}

void CreateNumberSequencesVargroup(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    TIO_Object_t vargroupID;
    TIO_t tErr = TIO_Create_Vargroup(fileID, objectID, "number sequences",
            &vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
    CreateVariable(fileID, vargroupID, "primes", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){16}, "", primes);
    int squares[] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169,
            196, 225 };
    CreateVariable(fileID, vargroupID, "squares", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){16}, "", squares);
    int fibonacci[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377,
            610};
    CreateVariable(fileID, vargroupID, "fibonacci", TIO_INT, TIO_1D,
        (const TIO_Size_t[]){16}, "", fibonacci);
    tErr = TIO_Close_Vargroup(fileID, vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }
}

void CreateCalendarInfoVargroup(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    TIO_Object_t vargroupID;
    TIO_t tErr = TIO_Create_Vargroup(fileID, objectID, "calendar info",
            &vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    CreateVariable(fileID, vargroupID, "months in the year", TIO_INT, TIO_1D,
            (const TIO_Size_t[]) {1}, "", (const int[]) {12});

    // Create a child vargroup
    TIO_Object_t subVargroupID;
    tErr = TIO_Create_Vargroup(fileID, vargroupID, "month lengths",
            &subVargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    CreateVariable(fileID, subVargroupID, "January", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "February", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){28});
    CreateVariable(fileID, subVargroupID, "March", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "April", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){30});
    CreateVariable(fileID, subVargroupID, "May", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "June", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){30});
    CreateVariable(fileID, subVargroupID, "July", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "August", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "September", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){30});
    CreateVariable(fileID, subVargroupID, "October", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    CreateVariable(fileID, subVargroupID, "November", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){30});
    CreateVariable(fileID, subVargroupID, "December", TIO_UINT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const unsigned int[]){31});
    tErr = TIO_Close_Vargroup(fileID, subVargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }

    tErr = TIO_Close_Vargroup(fileID, vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }
}

void CreateUnitPrefixesVargroup(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    TIO_Object_t vargroupID;
    TIO_t tErr = TIO_Create_Vargroup(fileID, objectID, "unit prefixes",
            &vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    CreateVariable(fileID, vargroupID, "kilo", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^3");
    CreateVariable(fileID, vargroupID, "mega", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^6");
    CreateVariable(fileID, vargroupID, "giga", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^9");
    CreateVariable(fileID, vargroupID, "tera", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^12");
    CreateVariable(fileID, vargroupID, "peta", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^15");
    CreateVariable(fileID, vargroupID, "exa", TIO_STRING, TIO_1D,
            (const TIO_Size_t[]){1}, "", "10^18");
    tErr = TIO_Close_Vargroup(fileID, vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }
}

void CreateSkillVargroup(const TIO_File_t &fileID,
        const TIO_Object_t &objectID) {
    TIO_Object_t vargroupID;
    TIO_t tErr = TIO_Create_Vargroup(fileID, objectID, "skill",
            &vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Vargroup");
    }
    CreateVariable(fileID, vargroupID, "level", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){9});
    CreateVariable(fileID, vargroupID, "high", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){5});
    tErr = TIO_Close_Vargroup(fileID, vargroupID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Vargroup");
    }
}

int main(int argc, char* argv[]) {
    TIO_t tErr;
    TIO_File_t fileID;
    std::string fileName = "ex_vargroup.h5";
    std::string codeName = "ex_vargroup";
    std::string version = "0.0.1";
    std::string date = "2016/11/03";
    std::string title = "TyphonIO vargroup example";
    tErr = TIO_Create(fileName.c_str(), &fileID, TIO_ACC_REPLACE,
            codeName.c_str(), version.c_str(), date.c_str(), title.c_str(),
            MPI_COMM_NULL, MPI_INFO_NULL, MPI_PROC_NULL);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create");
    }

    // Variables and vargroups on file
    CreateColorsVariable(fileID, TIO_NULL);
    CreateVersionNumberVargroup(fileID, TIO_NULL);

    TIO_Object_t stateID;
    tErr = TIO_Create_State(fileID, "state000", &stateID, 0, 0.0,
            "seconds");
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_State");
    }

    // Variables and vargroups on state
    CreateMagicSquareVariable(fileID, stateID);
    CreateNumberSequencesVargroup(fileID, stateID);

    // Create mesh
    TIO_Object_t meshID;
    tErr = TIO_Create_Mesh(fileID, stateID, "mesh", &meshID,
            TIO_MESH_QUAD_COLINEAR, TIO_COORD_CARTESIAN, TIO_FALSE, "group", 1,
            TIO_DATATYPE_NULL, TIO_DOUBLE, TIO_3D, 2, 3, 4, 0, 1, "", "",
            "", "", "", "");
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Mesh");
    }

    // Variables and vargroups on mesh
    CreateVariable(fileID, meshID, "year", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){2017});
    CreateCalendarInfoVargroup(fileID, meshID);

    tErr = TIO_Set_Quad_Chunk(fileID, meshID, 0, TIO_3D, 0, 1, 0, 2, 0, 3, 0,
            0);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Set_Quad_Chunk");
    }
    tErr = TIO_Write_QuadMesh_All(fileID, meshID, TIO_DOUBLE,
            (const double[]){0.0, 1.0},
            (const double[]){0.0, 1.0, 2.0},
            (const double[]){0.0, 1.0, 2.0, 3.0});
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Write_QuadMesh_Chunk");
    }

    // Create material
    TIO_Object_t materialID;
    tErr = TIO_Create_Material(fileID, meshID, "material", &materialID,
            TIO_INT, 3, TIO_GHOSTS_NONE, TIO_FALSE, TIO_DATATYPE_NULL,
            TIO_DATATYPE_NULL, TIO_DATATYPE_NULL);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Material");
    }

    // Variables and vargroups on material
    CreateVariable(fileID, materialID, "the answer", TIO_INT, TIO_1D,
            (const TIO_Size_t[]){1}, "", (const int[]){42});
    CreateUnitPrefixesVargroup(fileID, materialID);

    tErr = TIO_Write_QuadMaterial_Chunk(fileID, materialID, 0,
            TIO_XFER_INDEPENDENT, TIO_INT, (const int[]){1, 1, 2, 2, 3, 3},
            TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, nullptr,
            nullptr, nullptr, nullptr);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr,
                "Error in TIO_Write_QuadMaterial_Chunk");
    }

    tErr = TIO_Close_Material(fileID, materialID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Material");
    }

    // Create quant
    TIO_Object_t quantID;
    tErr = TIO_Create_Quant(fileID, meshID, "distance", &quantID, TIO_FLOAT,
            TIO_CENTRE_NODE, TIO_GHOSTS_NONE, TIO_FALSE, "metres");
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Create_Quant");
    }

    // Variables and vargroups on quant
    CreateVariable(fileID, quantID, "directions", TIO_UCHAR, TIO_1D,
            (const TIO_Size_t[]){4}, "",
            (const unsigned char[]){'n', 'e', 's', 'w'});
    CreateSkillVargroup(fileID, quantID);

    tErr = TIO_Write_QuadQuant_Chunk(fileID, quantID, 0, TIO_XFER_INDEPENDENT,
            TIO_FLOAT, (const float[]){0.0, 1.0, 1.0, 1.4142, 1.0, 1.4142,
        1.4142, 1.7321, 2.0, 2.2361, 2.2361, 2.4495}, nullptr);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Write_QuadQuant_Chunk");
    }

    tErr = TIO_Close_Quant(fileID, quantID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Quant");
    }

    tErr = TIO_Close_Mesh(fileID, meshID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_Mesh");
    }

    tErr = TIO_Close_State(fileID, stateID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close_State");
    }

    tErr = TIO_Close(fileID);
    if (tErr != TIO_SUCCESS) {
        PrintTyphonIOErrorAndExit(tErr, "Error in TIO_Close");
    }
    return 0;
}
