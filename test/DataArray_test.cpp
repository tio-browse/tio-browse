//
// test/DataArray_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "src/DataArray.h"

#include <gtest/gtest.h>
#include <typhonio.h>

TEST(DataArrayTest, initialization) {
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  EXPECT_EQ(3, dataArray->getDim(0));
  EXPECT_EQ(1, dataArray->getDim(1));
  EXPECT_EQ(2, dataArray->getDim(2));
  EXPECT_EQ(6, dataArray->getArrayLength());
  EXPECT_EQ(TIO_DOUBLE, dataArray->getTIODataType());
  delete dataArray;
}

TEST(DataArrayTest, getVoidPointer) {
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  ASSERT_EQ(6, dataArray->getArrayLength());
  ASSERT_EQ(TIO_DOUBLE, dataArray->getTIODataType());
  double *ptr = static_cast<double *>(dataArray->getVoidPointer());
  EXPECT_NE(nullptr, dataArray->getVoidPointer());
}

TEST(DataArrayTest, toQVariant) {
  TIO_Size_t dims[3] = {3, 1, 2};
  DataArray *dataArray = new DataArray("test name", TIO_3D, dims,
                                       DataArray::TYPE_DATA, TIO_DOUBLE);
  ASSERT_EQ(6, dataArray->getArrayLength());
  ASSERT_EQ(TIO_DOUBLE, dataArray->getTIODataType());
  double *ptr = static_cast<double *>(dataArray->getVoidPointer());
  ASSERT_NE(nullptr, ptr);
  ptr[0] = 1.1;
  ptr[1] = 2.2;
  ptr[2] = 3.3;
  ptr[3] = 4.4;
  ptr[4] = 5.5;
  ptr[5] = 6.6;
  EXPECT_DOUBLE_EQ(1.1, dataArray->toQVariant(0).toDouble());
  EXPECT_DOUBLE_EQ(2.2, dataArray->toQVariant(1).toDouble());
  EXPECT_DOUBLE_EQ(3.3, dataArray->toQVariant(2).toDouble());
  EXPECT_DOUBLE_EQ(4.4, dataArray->toQVariant(3).toDouble());
  EXPECT_DOUBLE_EQ(5.5, dataArray->toQVariant(4).toDouble());
  EXPECT_DOUBLE_EQ(6.6, dataArray->toQVariant(5).toDouble());
  delete dataArray;
}
