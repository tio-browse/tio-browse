//
// src/TIOTreeModel.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOTREEMODEL_H_
#define SRC_TIOTREEMODEL_H_

#include <QAbstractItemModel>
#include <memory>
#include <string>
#include "src/MainWindowMediator.h"
#include "src/TIORootItem.h"

class TIOTreeModel : public QAbstractItemModel {
 public:
  explicit TIOTreeModel(const std::string& filename, QObject* parent = 0);
  ~TIOTreeModel() override;
  bool canFetchMore(const QModelIndex& parent) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  void fetchMore(const QModelIndex& parent) override;
  bool hasArrayData(const QModelIndex& index) const;
  bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
  DataArray* getArrayData(const QModelIndex& index) const;
  QModelIndex index(int row, int column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

 private:
  std::unique_ptr<TIORootItem> m_rootItem;
  std::string m_filename;
};

#endif  // SRC_TIOTREEMODEL_H_
