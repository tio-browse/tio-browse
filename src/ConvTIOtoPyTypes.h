//
// src/ConvTIOtoPyTypes.h
//
// (c) British Crown Owned Copyright 2021/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_CONVTIOTOPYTYPES_H_
#define SRC_CONVTIOTOPYTYPES_H_

#include <typhonio.h>

#include <QString>

//! Collection of functions for converting types from TIO to Python compatible.
class ConvTIOtoPyTypes {
 public:
  //! Get corresponding numpy array type
  //! \param[in] TIO_type a TIO type
  //! \return int corresponding to equivalent type in numpy/ndarraytypes.h
  static int TIOtoPyType(TIO_Data_t TIO_type);

  //! Make QString PEP compliant
  //! \param[in] name Default TIO data name
  //! \return Converted QString to be PEP compliant
  static QString NametoPy(QString name);
};

#endif  // SRC_CONVTIOTOPYTYPES_H_
