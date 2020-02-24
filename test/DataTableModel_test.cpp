//
// test/DataTableModel_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "DataTableModel.h"

#include <gtest/gtest.h>

TEST(DataTableModelTest, initialization) {
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  double *dataPointer = static_cast<double *>(dataArray->getVoidPointer());
  ASSERT_EQ(6, dataArray->getArrayLength());
  dataPointer[0] = 1.1;
  dataPointer[1] = 2.2;
  dataPointer[2] = 3.3;
  dataPointer[3] = 4.4;
  dataPointer[4] = 5.5;
  dataPointer[5] = 6.6;
  DataTableModel dataTableModel;
  dataTableModel.setDataArray(dataArray);
  EXPECT_EQ(3, dataTableModel.columnCount());
  EXPECT_EQ(2, dataTableModel.rowCount());

  EXPECT_DOUBLE_EQ(1.1,
                   dataTableModel.data(dataTableModel.index(0, 0)).toDouble());
  EXPECT_DOUBLE_EQ(2.2,
                   dataTableModel.data(dataTableModel.index(0, 1)).toDouble());
  EXPECT_DOUBLE_EQ(3.3,
                   dataTableModel.data(dataTableModel.index(0, 2)).toDouble());
  EXPECT_DOUBLE_EQ(4.4,
                   dataTableModel.data(dataTableModel.index(1, 0)).toDouble());
  EXPECT_DOUBLE_EQ(5.5,
                   dataTableModel.data(dataTableModel.index(1, 1)).toDouble());
  EXPECT_DOUBLE_EQ(6.6,
                   dataTableModel.data(dataTableModel.index(1, 2)).toDouble());
  delete dataArray;
}
