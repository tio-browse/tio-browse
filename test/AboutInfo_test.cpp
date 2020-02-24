//
// test/AboutInfo_test.cpp
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO browse
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#include "AboutInfo.h"

#include <gtest/gtest.h>

TEST(AboutInfo, CheckGettersAndSetters) {
  AboutInfo aboutInfo;
  aboutInfo.setAppName("app name");
  aboutInfo.setAppVersion("app version");
  aboutInfo.setAppDescription("app description");
  aboutInfo.setCustodianName("custodian name");
  aboutInfo.setCustodianEmail("custodian email");
  aboutInfo.setCustodianExtension("custodian extension");

  EXPECT_EQ("app name", aboutInfo.getAppName());
  EXPECT_EQ("app version", aboutInfo.getAppVersion());
  EXPECT_EQ("app description", aboutInfo.getAppDescription());
  EXPECT_EQ("custodian name", aboutInfo.getCustodianName());
  EXPECT_EQ("custodian email", aboutInfo.getCustodianEmail());
  EXPECT_EQ("custodian extension", aboutInfo.getCustodianExtension());
}
