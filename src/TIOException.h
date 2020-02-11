//
// src/TIOException.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOEXCEPTION_H_
#define SRC_TIOEXCEPTION_H_

#include <typhonio.h>
#include <exception>
#include <string>

class TIOException : public std::exception {
 public:
  explicit TIOException(const TIO_t &tErr);
  const char *what() const throw() override;

 private:
  std::string m_message;
};

class TIOMeshtypeException : public std::exception {
 public:
  explicit TIOMeshtypeException(const TIO_Mesh_t &meshType);
  const char *what() const throw() override;

 private:
  std::string m_message;
};

class TIOVariableNameException : public std::exception {
 public:
  explicit TIOVariableNameException(const std::string &variableName);
  const char *what() const throw() override;

 private:
  std::string m_message;
};

class TIODataArrayException : public std::exception {
  const char *what() const throw() override;
};

class TIOGenericException : public std::exception {
 public:
  explicit TIOGenericException(const std::string &message);
  const char *what() const throw() override;

 private:
  std::string m_message;
};

#endif  // SRC_TIOEXCEPTION_H_
