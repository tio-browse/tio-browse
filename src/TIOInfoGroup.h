//
// src/TIOInfoGroup.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_TIOINFOGROUP_H_
#define SRC_TIOINFOGROUP_H_

#include <string>

#include "TIOTreeItem.h"

//! TyphonIO info group tree item
class TIOInfoGroup : public TIOTreeItem {
 public:
  //! Constructor
  //! \param[in] itemName as standard string
  //! \param[in] parent is the Qt parent item
  explicit TIOInfoGroup(const std::string &itemName, TIOTreeItem *parent = 0);

  // Destructor
  ~TIOInfoGroup() override;

  //! Add an info item to this info group
  //! \param[in] itemName is the name of the info item
  //! \param[in] itemValue is the value of the info item
  void AddTIOInfoItem(std::string itemName, std::string itemValue);

  //! Get the array data for this tree item
  //! \param[in] variable is the name of the data variable to return
  //! \return is a pointer to a newly-allocated DataArray or nullptr
  DataArray *getArrayData(const std::string &variable) override;
};

#endif  // SRC_TIOINFOGROUP_H_
