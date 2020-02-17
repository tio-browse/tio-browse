//
// src/TIOException.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "TIOException.h"

#include <string>

TIOException::TIOException(const TIO_t &tErr) {
  char description[TIO_STRLEN];
  TIO_Get_Error(tErr, description);
  m_message.assign(description, TIO_STRLEN);
}

const char *TIOException::what() const throw() { return m_message.c_str(); }

TIOMeshtypeException::TIOMeshtypeException(const TIO_Mesh_t &meshType) {
  m_message = "Invalid mesh type : " + std::to_string(meshType);
}

const char *TIOMeshtypeException::what() const throw() {
  return "Invalid TyphonIO mesh type";
}

TIOVariableNameException::TIOVariableNameException(
    const std::string &variableName) {
  m_message = "Invalid variable name : " + variableName;
}

const char *TIOVariableNameException::what() const throw() {
  return m_message.c_str();
}

const char *TIODataArrayException::what() const throw() {
  return "Data array failed";
}

TIOGenericException::TIOGenericException(const std::string &message) {
  m_message = message;
}

const char *TIOGenericException::what() const throw() {
  return m_message.c_str();
}
