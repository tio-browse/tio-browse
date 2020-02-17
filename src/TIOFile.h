//
// src/TIOFile.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOFILE_H_
#define SRC_TIOFILE_H_

#include <memory>
#include <string>

#include "TIOTreeItem.h"

class TIOFile : public TIOTreeItem {
 public:
  explicit TIOFile(const std::string &itemName, const std::string &filename,
                   TIOTreeItem *parent = 0);

  ~TIOFile() override;

  //! Used to ask the class instance to fetch more data
  //! Used by the model to request that data is fetched from a TyphonIO file
  void fetchMore() override;

  TIO_File_t getFileID() override;

 private:
  TIO_File_t m_fileID;
  std::string m_filename;
};

#endif  // SRC_TIOFILE_H_
