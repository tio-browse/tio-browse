//
// test/DataDialog_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/DataDialog.h"
#include <QTest>

class TestDataDialog : public QObject {
  Q_OBJECT
 private slots:
  void initialization();
  void defaultRowAndColumnNames();
  void oneOriginRowAndColumnNames();
};

void TestDataDialog::initialization() {
  // Create the DataArray
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  double *ptr = static_cast<double *>(dataArray->getVoidPointer());
  ptr[0] = 1.1;
  ptr[1] = 2.2;
  ptr[2] = 3.3;
  ptr[3] = 4.4;
  ptr[4] = 5.5;
  ptr[5] = 6.6;

  DataDialog *dataDialog = new DataDialog(dataArray);

  // Verify that the one-origin check box initialized correctly
  QCheckBox *oneOriginCheckBox =
      dataDialog->findChild<QCheckBox *>("oneOriginCheckBox");
  QVERIFY(Qt::Unchecked == oneOriginCheckBox->checkState());

  // Check that the TableView initialized correctly
  QTableView *tableView = dataDialog->findChild<QTableView *>("tableView");
  QVERIFY(1.1 == tableView->model()->index(0, 0).data().toDouble());
  QVERIFY(2.2 == tableView->model()->index(0, 1).data().toDouble());
  QVERIFY(3.3 == tableView->model()->index(0, 2).data().toDouble());
  QVERIFY(4.4 == tableView->model()->index(1, 0).data().toDouble());
  QVERIFY(5.5 == tableView->model()->index(1, 1).data().toDouble());
  QVERIFY(6.6 == tableView->model()->index(1, 2).data().toDouble());
  delete dataDialog;
}

void TestDataDialog::defaultRowAndColumnNames() {
  // Create the DataArray
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  double *ptr = static_cast<double *>(dataArray->getVoidPointer());
  ptr[0] = 1.1;
  ptr[1] = 2.2;
  ptr[2] = 3.3;
  ptr[3] = 4.4;
  ptr[4] = 5.5;
  ptr[5] = 6.6;

  DataDialog *dataDialog = new DataDialog(dataArray);
  QTableView *tableView = dataDialog->findChild<QTableView *>("tableView");
  // Column names
  QVERIFY(0 == tableView->model()->headerData(0, Qt::Horizontal));
  QVERIFY(1 == tableView->model()->headerData(1, Qt::Horizontal));
  QVERIFY(2 == tableView->model()->headerData(2, Qt::Horizontal));

  // Row names
  QVERIFY(0 == tableView->model()->headerData(0, Qt::Vertical));
  QVERIFY(1 == tableView->model()->headerData(1, Qt::Vertical));
  delete dataDialog;
}

void TestDataDialog::oneOriginRowAndColumnNames() {
  // Create the DataArray
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  double *ptr = static_cast<double *>(dataArray->getVoidPointer());
  ptr[0] = 1.1;
  ptr[1] = 2.2;
  ptr[2] = 3.3;
  ptr[3] = 4.4;
  ptr[4] = 5.5;
  ptr[5] = 6.6;

  DataDialog *dataDialog = new DataDialog(dataArray);

  // Check the one-origin check box
  QCheckBox *oneOriginCheckBox =
      dataDialog->findChild<QCheckBox *>("oneOriginCheckBox");
  oneOriginCheckBox->setCheckState(Qt::Checked);
  Q_ASSERT(Qt::Checked == oneOriginCheckBox->checkState());

  QTableView *tableView = dataDialog->findChild<QTableView *>("tableView");
  // Column names
  QVERIFY(1 == tableView->model()->headerData(0, Qt::Horizontal));
  QVERIFY(2 == tableView->model()->headerData(1, Qt::Horizontal));
  QVERIFY(3 == tableView->model()->headerData(2, Qt::Horizontal));

  // Row names
  QVERIFY(1 == tableView->model()->headerData(0, Qt::Vertical));
  QVERIFY(2 == tableView->model()->headerData(1, Qt::Vertical));
  delete dataDialog;
}

QTEST_MAIN(TestDataDialog)
#include "DataDialog_test.moc"
