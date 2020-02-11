//
// src/TIOTreeItem.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOTREEITEM_H_
#define SRC_TIOTREEITEM_H_

#include <typhonio.h>
#include <QVariant>
#include <memory>
#include <string>
#include <vector>
#include "src/DataArray.h"

//! Class that forms basis of tree model
class TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as standard string
  //! \param[in] parent is the Qt parent item
  explicit TIOTreeItem(const std::string& itemName, TIOTreeItem* parent = 0);

  virtual ~TIOTreeItem();

  //! Create a TIODataItem child and append to this
  virtual void AddTIODataItem(const std::string& itemName,
                              const std::string& itemValue);

  //! Add the vargroup children
  //! \param[in] TyphonIO fileID
  //! \param[in] TyphonIO objectID
  virtual void AddTIOVargroups(const TIO_File_t& fileID,
                               const TIO_Object_t& objectID);

  virtual void AddTIOVariables(const TIO_File_t& fileID,
                               const TIO_Object_t& objectID);

  //! Can this instance fetch more data (from file)
  //! \return boolean if this instance can fetch more data
  virtual bool canFetchMore();

  //! Returns the child item at a given row
  //! \param[in] row is the requested row
  TIOTreeItem* child(int row);

  //! Get the child count
  //! \return the number of children as integer
  int childCount() const;

  //! Get the column count
  //! \return the number of columns as integer
  int columnCount() const;

  //! Get the data for a specific column
  //! \return QVariant (can be QString etc.)
  virtual QVariant data(int column) const;

  //! Used to ask the class instance to fetch more data
  //! Used by the model when more data is required (i.e. before a tree node is
  //! expanded). Here it only sets an internal flag to show that the node has
  //! been initialized but is intended to be overridden in child classes.
  virtual void fetchMore();

  //! Get the array data for this tree item
  //!
  //! \return is a pointer to a newly-allocated DataArray or nullptr
  virtual DataArray* getArrayData();

  //! Get the named array data for this tree item
  //!
  //! \return is a pointer to a newly-allocated DataArray or nullptr
  virtual DataArray* getArrayData(const std::string& variable);

  //! Get the TyphonIO file ID
  //! The routine just asks its parent for the file ID but is intended to be
  //! overridden in a child class
  //! \return TyphonIO fileID associated with this tree item
  virtual TIO_File_t getFileID();

  //! Get the TyphonIO object ID
  //! The objectID will be specific to to child class e.g. stateID, meshID,
  //! etc.
  //! \return TyphonIO objectID associated with this tree item
  virtual TIO_Object_t getObjectID();

  // TODO(jim): Consider whether this could be replaced with a
  // TODO(jim): ... getParentObjectID routine, then we wouldn't have to return
  // TODO(jim): ... a pointer
  //! Get pointer to item's parent
  //! The routine returns the m_parentItem pointer that it uses internally
  //! \return TIOTreeItem pointer to some TIOTreeItem child class instance
  virtual TIOTreeItem* getParentItem();

  //! Get the TyphonIO object ID of the mesh object
  //! The routine just asks its parent for the meshID but is intended to be
  //! overridden in a derived class
  //! \return TyphonIO mesh ID
  virtual TIO_Object_t getMeshID();

  //! Does this instance have array data
  //! Does this tree item have data, for example mesh coordinates, or density
  //! data that should be displayed in a table
  //! Can be called before calling getArrayData()
  //! return true if this tree item has array data, false otherwise
  virtual bool hasArrayData();

  //! Does this instance have children
  //! \return boolean true -- assumes children -- can be overridden in child
  //! class
  virtual bool hasChildren();

  TIOTreeItem* parent();
  int row() const;

 protected:
  QList<QVariant> m_itemData;
  TIOTreeItem* m_parentItem;
  TIO_Object_t m_objectID;
  std::vector<std::unique_ptr<TIOTreeItem> > m_childItems;

 private:
  bool m_isInitialized;
};

#endif  // SRC_TIOTREEITEM_H_
