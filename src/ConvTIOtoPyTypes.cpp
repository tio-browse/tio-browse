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
      // Need to Decide / TODO
      return -1;
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
      return -1;
      // return NPY_CHAR; //Deprecciated don't use.
    case TIO_UCHAR:
      return -1;  // can't find
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
// enum NPY_TYPES {    NPY_BOOL=0,
//                     NPY_BYTE, NPY_UBYTE,
//                     NPY_SHORT, NPY_USHORT,
//                     NPY_INT, NPY_UINT,
//                     NPY_LONG, NPY_ULONG,
//                     NPY_LONGLONG, NPY_ULONGLONG,
//                     NPY_FLOAT, NPY_DOUBLE, NPY_LONGDOUBLE,
//                     NPY_CFLOAT, NPY_CDOUBLE, NPY_CLONGDOUBLE,
//                     NPY_OBJECT=17,
//                     NPY_STRING, NPY_UNICODE,
//                     NPY_VOID,
//                     /*
//                      * New 1.6 types appended, may be integrated
//                      * into the above in 2.0.
//                      */
//                     NPY_DATETIME, NPY_TIMEDELTA, NPY_HALF,

//                     NPY_NTYPES,
//                     NPY_NOTYPE,
//                     NPY_CHAR NPY_ATTR_DEPRECATE("Use NPY_STRING"),
//                     NPY_USERDEF=256,  /* leave room for characters */

//                     /* The number of types not including the new 1.6 types */
//                     NPY_NTYPES_ABI_COMPATIBLE=21
// };

// std::map<TIO_Data_t, std::string> TIOTypeNames::TIO_DATA = {
//     {TIO_DATATYPE_NULL, "TIO_DATATYPE_NULL"},
//     {TIO_SHORT, "TIO_SHORT"},
//     {TIO_USHORT, "TIO_USHORT"},
//     {TIO_INT, "TIO_INT"},
//     {TIO_UINT, "TIO_UINT"},
//     {TIO_LONG, "TIO_LONG"},
//     {TIO_ULONG, "TIO_ULONG"},
//     {TIO_LLONG, "TIO_LLONG"},
//     {TIO_ULLONG, "TIO_ULLONG"},
//     {TIO_FLOAT, "TIO_FLOAT"},
//     {TIO_DOUBLE, "TIO_DOUBLE"},
//     {TIO_LDOUBLE, "TIO_LDOUBLE"},
//     {TIO_LOGICAL, "TIO_LOGICAL"},
//     {TIO_CHAR, "TIO_CHAR"},
//     {TIO_UCHAR, "TIO_UCHAR"},
//     {TIO_STRING, "TIO_STRING"}};
// }
