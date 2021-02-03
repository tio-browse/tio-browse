//
// src/ConvTIOtoPyTypes.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_CONVTIOTOPYTYPES_H_
#define SRC_CONVTIOTOPYTYPES_H_

#include <typhonio.h>

#include <QString>

// #include <map>
//  #include <string>

class ConvTIOtoPyTypes {
 public:
  // int TIOtoPyType(std::string TIO_type);
  static int TIOtoPyType(TIO_Data_t TIO_type);
  static QString NametoPy(QString name);
};

#endif  // SRC_TIOTYPENAMES_H_
