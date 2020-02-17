//
// src/DataTableModel.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_DATATABLEMODEL_H_
#define SRC_DATATABLEMODEL_H_

#include <QAbstractTableModel>

#include "DataArray.h"

//! Model to serve TyphonIO array data to a TableView widget
//!
//! The DataArray object is set with a call to the setDataArray method. The
//! size of the table can then be obtained using calls to the columnCount and
//! rowCount methods. The data and formatting for a particular cell is obtained
//! using the data method.
class DataTableModel : public QAbstractTableModel {
 public:
  //! Constructor
  //! \param[in] parent is a pointer to the Qt parent widget
  explicit DataTableModel(QObject *parent = 0);

  //! Fetch the column count
  //! \param[in] index A QModelIndex instance (not used)
  //! \return The column count
  int columnCount(const QModelIndex &index = QModelIndex()) const;

  //! Get the data, or formatting information for a specific cell
  //! \param[in] index The index of the cell
  //! \param[in] role An ItemDataRole enum specifying the type of data
  //! required -- data, formatting, etc.
  //! \return The data or formatting as required
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  //! Get the headerData for a specific section
  //! \param[in] section is the column or row
  //! \param[in] orientation is horizontal or vertical for columns or rows
  //! \param[in] role is the data role that is used to request data,
  //!            formatting data, etc.
  //! \return The data or formatting info as required
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  //! Fetch the row count
  //! \param[in] index A QModelIndex instance (not used)
  //! \return The row count
  int rowCount(const QModelIndex &index = QModelIndex()) const;

  //! Setter for the DataArray
  void setDataArray(DataArray *dataArray);

  //! Set the origin for the header labels
  //!
  //! Used for setting the origin to 0 for C++ fans and 1 for Fortran fans
  void setHeaderOrigin(const int &origin);

 private:
  DataArray *m_dataArray;
  int m_headerOrigin;
};

#endif  // SRC_DATATABLEMODEL_H_
