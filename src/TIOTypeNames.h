//
// src/TIOTypeNames.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOTYPENAMES_H_
#define SRC_TIOTYPENAMES_H_

#include <typhonio.h>
#include <map>
#include <string>

class TIOTypeNames {
 public:
  static std::map<TIO_Mesh_t, std::string> TIO_MESH;
  static std::map<TIO_Coord_t, std::string> TIO_COORD;
  static std::map<TIO_Bool_t, std::string> TIO_BOOL;
  static std::map<TIO_Data_t, std::string> TIO_DATA;
  static std::map<TIO_Centre_t, std::string> TIO_CENTRE;
  static std::map<TIO_Shape_t, std::string> TIO_SHAPE;
  static std::map<TIO_Shape_t, int> TIO_SHAPE_NODE_COUNTS;
  static std::string TIOGhostToString(const TIO_Size_t &ghosts);
};

#endif  // SRC_TIOTYPENAMES_H_
