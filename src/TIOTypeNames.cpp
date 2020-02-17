//
// src/TIOTypeNames.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOTypeNames.h"

#include <map>
#include <string>

std::map<TIO_Mesh_t, std::string> TIOTypeNames::TIO_MESH = {
    {TIO_MESH_NULL, "TIO_MESH_NULL"},
    {TIO_MESH_STATIC, "TIO_MESH_STATIC"},
    {TIO_MESH_QUAD_COLINEAR, "TIO_MESH_QUAD_COLINEAR"},
    {TIO_MESH_QUAD_NONCOLINEAR, "TIO_MESH_QUAD_NONCOLINEAR"},
    {TIO_MESH_UNSTRUCT, "TIO_MESH_UNSTRUCT"},
    {TIO_MESH_POINT, "TIO_MESH_POINT"}};

std::map<TIO_Coord_t, std::string> TIOTypeNames::TIO_COORD = {
    {TIO_COORD_NULL, "TIO_COORD_NULL"},
    {TIO_COORD_CARTESIAN, "TIO_COORD_CARTESIAN"},
    {TIO_COORD_SPHERICAL, "TIO_COORD_SPHERICAL"},
    {TIO_COORD_CYLINDRICAL, "TIO_COORD_CYLINDRICAL"},
    {TIO_COORD_CYLINDRICAL_AXISYMMETRIC, "TIO_COORD_CYLINDRICAL_AXISYMMETRIC"}};

std::map<TIO_Bool_t, std::string> TIOTypeNames::TIO_BOOL = {
    {TIO_FALSE, "TIO_FALSE"}, {TIO_TRUE, "TIO_TRUE"}};

std::map<TIO_Data_t, std::string> TIOTypeNames::TIO_DATA = {
    {TIO_DATATYPE_NULL, "TIO_DATATYPE_NULL"},
    {TIO_SHORT, "TIO_SHORT"},
    {TIO_USHORT, "TIO_USHORT"},
    {TIO_INT, "TIO_INT"},
    {TIO_UINT, "TIO_UINT"},
    {TIO_LONG, "TIO_LONG"},
    {TIO_ULONG, "TIO_ULONG"},
    {TIO_LLONG, "TIO_LLONG"},
    {TIO_ULLONG, "TIO_ULLONG"},
    {TIO_FLOAT, "TIO_FLOAT"},
    {TIO_DOUBLE, "TIO_DOUBLE"},
    {TIO_LDOUBLE, "TIO_LDOUBLE"},
    {TIO_LOGICAL, "TIO_LOGICAL"},
    {TIO_CHAR, "TIO_CHAR"},
    {TIO_UCHAR, "TIO_UCHAR"},
    {TIO_STRING, "TIO_STRING"}};

std::map<TIO_Centre_t, std::string> TIOTypeNames::TIO_CENTRE = {
    {TIO_CENTRE_NULL, "TIO_CENTRE_NULL"},
    {TIO_CENTRE_CELL, "TIO_CENTRE_CELL"},
    {TIO_CENTRE_EDGE_I, "TIO_CENTRE_EDGE_I"},
    {TIO_CENTRE_EDGE_J, "TIO_CENTRE_EDGE_J"},
    {TIO_CENTRE_EDGE_K, "TIO_CENTRE_EDGE_K"},
    {TIO_CENTRE_FACE_I, "TIO_CENTRE_FACE_I"},
    {TIO_CENTRE_FACE_J, "TIO_CENTRE_FACE_J"},
    {TIO_CENTRE_FACE_K, "TIO_CENTRE_FACEK"},
    {TIO_CENTRE_NODE, "TIO_CENTRE_NODE"}};

std::map<TIO_Shape_t, std::string> TIOTypeNames::TIO_SHAPE = {
    {TIO_SHAPE_NULL, "TIO_SHAPE_NULL"},
    {TIO_SHAPE_POINT1, "TIO_SHAPE_POINT1"},
    {TIO_SHAPE_BAR2, "TIO_SHAPE_BAR2"},
    {TIO_SHAPE_TRI3, "TIO_SHAPE_TRI3"},
    {TIO_SHAPE_QUAD4, "TIO_SHAPE_QUAD4"},
    {TIO_SHAPE_TET4, "TIO_SHAPE_TET4"},
    {TIO_SHAPE_WEDGE6, "TIO_SHAPE_WEDGE6"},
    {TIO_SHAPE_HEX8, "TIO_SHAPE_HEX8"},
    {TIO_SHAPE_PYR5, "TIO_SHAPE_PYR5"},
    {TIO_SHAPE_MIN, "TIO_SHAPE_MIN"},
    {TIO_SHAPE_MAX, "TIO_SHAPE_MAX"}};

std::map<TIO_Shape_t, int> TIOTypeNames::TIO_SHAPE_NODE_COUNTS = {
    {TIO_SHAPE_POINT1, 1}, {TIO_SHAPE_BAR2, 2}, {TIO_SHAPE_TRI3, 3},
    {TIO_SHAPE_QUAD4, 4},  {TIO_SHAPE_TET4, 4}, {TIO_SHAPE_WEDGE6, 6},
    {TIO_SHAPE_HEX8, 8},   {TIO_SHAPE_PYR5, 5}};

std::string TIOTypeNames::TIOGhostToString(const TIO_Size_t &ghosts) {
  switch (ghosts) {
    case TIO_GHOSTS_NONE:
      return "TIO_GHOSTS_NONE";
    case TIO_GHOSTS_ALL:
      return "TIO_GHOSTS_ALL";
    case TIO_GHOSTS_NULL:
      return "TIO_GHOSTS_NULL";
    default:
      return std::to_string(ghosts);
  }
}
