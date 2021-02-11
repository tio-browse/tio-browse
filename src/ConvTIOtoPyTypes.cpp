//
// src/ConvTIOtoPyTypes.cpp
//
// (c) British Crown Owned Copyright 2021/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//
#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION

#include "ConvTIOtoPyTypes.h"

#include <Python.h>
#include <numpy/ndarraytypes.h>

#include <map>
#include <string>

int ConvTIOtoPyTypes::TIOtoPyType(TIO_Data_t TIO_type) {
  int py_type;

  switch (TIO_type) {
    case TIO_DATATYPE_NULL:
      // TODO(t-cann): Handle NULL type correctly
      py_type = -1;
      break;
    case TIO_SHORT:
      py_type = NPY_SHORT;
      break;
    case TIO_USHORT:
      py_type = NPY_USHORT;
      break;
    case TIO_INT:
      py_type = NPY_INT;
      break;
    case TIO_UINT:
      py_type = NPY_UINT;
      break;
    case TIO_LONG:
      py_type = NPY_LONG;
      break;
    case TIO_ULONG:
      py_type = NPY_ULONG;
      break;
    case TIO_LLONG:
      py_type = NPY_LONGLONG;
      break;
    case TIO_ULLONG:
      py_type = NPY_ULONGLONG;
      break;
    case TIO_FLOAT:
      py_type = NPY_FLOAT;
      break;
    case TIO_DOUBLE:
      py_type = NPY_DOUBLE;
      break;
    case TIO_LDOUBLE:
      py_type = NPY_LONGDOUBLE;
      break;
    case TIO_LOGICAL:
      py_type = NPY_BOOL;
      break;
    case TIO_CHAR:
      // TODO(t-cann): Handle TIO_CHAR correctly
      py_type = -1;
      break;
    case TIO_UCHAR:
      // TODO(t-cann): Handle TIO_UCHAR correctly
      py_type = -1;
      break;
    case TIO_STRING:
      py_type = NPY_STRING;
      break;
    default:
      py_type = -1;
  }

  return py_type;
}

QString ConvTIOtoPyTypes::NametoPy(QString name) {
  name.remove(" ");
  name[0] = name[0].toLower();
  return name;
}
