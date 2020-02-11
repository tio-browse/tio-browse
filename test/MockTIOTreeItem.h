//
// test/MockTIOTreeItem.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef TEST_MOCKTIOTREEITEM_H_
#define TEST_MOCKTIOTREEITEM_H_

#include <typhonio.h>
#include <string>
#include "src/TIOTreeItem.h"

class MockTIOTreeItem : public TIOTreeItem {
 public:
  MockTIOTreeItem(std::string itemName, TIO_File_t fileID,
                  TIO_Object_t objectID)
      : TIOTreeItem(itemName), m_fileID(fileID), m_meshID(TIO_NULL) {
    m_objectID = objectID;
  }
  MockTIOTreeItem(std::string itemName, TIO_File_t fileID, TIO_Object_t meshID,
                  TIO_Object_t objectID)
      : TIOTreeItem(itemName), m_fileID(fileID), m_meshID(meshID) {
    m_objectID = objectID;
  }
  TIO_File_t getFileID() override { return m_fileID; }
  TIO_Object_t getMeshID() override { return m_meshID; }

 private:
  TIO_File_t m_fileID;
  TIO_Object_t m_meshID;
};

#endif  // TEST_MOCKTIOTREEITEM_H_
