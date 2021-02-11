//
// src/ConvTIOtoPyTypes.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
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
  switch (TIO_type) {
    case TIO_DATATYPE_NULL:
      return -1;  // Need to decide what to do with Null type.
    case TIO_SHORT:
      return NPY_SHORT;
    case TIO_USHORT:
      return NPY_USHORT;
    case TIO_INT:
      return NPY_INT;
    case TIO_UINT:
      return NPY_UINT;
    case TIO_LONG:
      return NPY_LONG;
    case TIO_ULONG:
      return NPY_ULONG;
    case TIO_LLONG:
      return NPY_LONGLONG;
    case TIO_ULLONG:
      return NPY_ULONGLONG;
    case TIO_FLOAT:
      return NPY_FLOAT;
    case TIO_DOUBLE:
      return NPY_DOUBLE;
    case TIO_LDOUBLE:
      return NPY_LONGDOUBLE;
    case TIO_LOGICAL:
      return NPY_BOOL;
    case TIO_CHAR:
      return -1;  // return NPY_CHAR; //Deprecciated don't use.
    case TIO_UCHAR:
      return -1;  // can't find an equivalent
    case TIO_STRING:
      return NPY_STRING;
    default:
      return -1;
  }
}

QString ConvTIOtoPyTypes::NametoPy(QString name) {
  name.remove(" ");
  name[0] = name[0].toLower();
  return name;
}