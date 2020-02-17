//
// src/TIOVariable.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOVARIABLE_H_
#define SRC_TIOVARIABLE_H_

#include <string>

#include "TIOTreeItem.h"

class TIOVariable : public TIOTreeItem {
 public:
  explicit TIOVariable(const std::string &itemName, TIOTreeItem *parent = 0);
  ~TIOVariable() override;

  static std::string DimsToString(const TIO_Dims_t nDims,
                                  const TIO_Size_t *dims);

  //! Used by the tree model to populate child data (from file)
  void fetchMore() override;

  //! Used to get the array data associated with the named variable
  //! \return a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;
};

#endif  // SRC_TIOVARIABLE_H_
