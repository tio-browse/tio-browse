//
// src/TIOFile.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/TIOFile.h"
#include <sstream>
#include <string>
#include <utility>
#include "src/TIOException.h"
#include "src/TIOInfoGroup.h"
#include "src/TIOInfoItem.h"
#include "src/TIOState.h"

TIOFile::TIOFile(const std::string& itemName, const std::string& filename,
                 TIOTreeItem* parent)
    : TIOTreeItem(itemName, parent), m_fileID(TIO_NULL), m_filename(filename) {}

TIOFile::~TIOFile() {
  m_childItems.clear();

  if (m_fileID != TIO_NULL) {
    TIO_Close(m_fileID);
    m_fileID = TIO_NULL;
  }
}

void TIOFile::fetchMore() {
  TIO_t tErr;
  char codename[TIO_STRLEN];
  char version[TIO_STRLEN];
  char date[TIO_STRLEN];
  char title[TIO_STRLEN];
  int majorVersion, minorVersion, releaseVersion;
  std::stringstream ssFileVersion;
  TIO_Size_t nStates;

  TIOTreeItem::fetchMore();

  tErr = TIO_Open(const_cast<char*>(m_filename.c_str()), &m_fileID,
                  TIO_ACC_READONLY, codename, version, date, title,
                  MPI_COMM_NULL, MPI_INFO_NULL, MPI_PROC_NULL);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  tErr = TIO_Get_FileVersion(m_filename.c_str(), &majorVersion, &minorVersion,
                             &releaseVersion);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }

  ssFileVersion << majorVersion << '.' << minorVersion << '.' << releaseVersion;

  // Add file info
  auto infoGroup =
      std::unique_ptr<TIOInfoGroup>(new TIOInfoGroup("File information", this));
  infoGroup->AddTIOInfoItem("Code", codename);
  infoGroup->AddTIOInfoItem("Version", version);
  infoGroup->AddTIOInfoItem("Date", date);
  infoGroup->AddTIOInfoItem("Title", title);
  infoGroup->AddTIOInfoItem("Library", ssFileVersion.str());
  m_childItems.push_back(std::move(infoGroup));

  // Add states
  tErr = TIO_List_States(m_fileID, &nStates, nullptr);
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  auto stateNames =
      std::unique_ptr<char[][TIO_STRLEN]>(new char[nStates][TIO_STRLEN]);
  tErr = TIO_List_States(m_fileID, nullptr, stateNames.get());
  if (tErr != TIO_SUCCESS) {
    throw TIOException(tErr);
  }
  for (TIO_Size_t i = 0; i < nStates; ++i) {
    m_childItems.push_back(
        std::unique_ptr<TIOState>(new TIOState(stateNames[i], this)));
  }

  // Add variables and vargroups
  AddTIOVariables(m_fileID, TIO_NULL);
  AddTIOVargroups(m_fileID, TIO_NULL);
}

TIO_File_t TIOFile::getFileID() { return m_fileID; }
