//
// src/AboutInfo.h
//
// (c) British Crown Owned Copyright 2019/AWE
//
// This file is part of TIO tool
// Released under the BSD 3-clause license.
// For more details see license.txt
//

#ifndef SRC_ABOUTINFO_H_
#define SRC_ABOUTINFO_H_

#include <string>

//! Class for passing app information between classes
//!
class AboutInfo {
 public:
  //! Constructor
  //!
  AboutInfo();

  //! appDescription getter
  //!
  const std::string& getAppDescription() const;

  //! appDescription setter
  //!
  void setAppDescription(const std::string& appDescription);

  //! appName getter
  //!
  const std::string& getAppName() const;

  //! appName setter
  //!
  void setAppName(const std::string& appName);

  //! appVersion getter
  //!
  const std::string& getAppVersion() const;

  //! appVersion setter
  //!
  void setAppVersion(const std::string& appVersion);

  //! custodianEmail getter
  //!
  const std::string& getCustodianEmail() const;

  //! custodianEmail setter
  //!
  void setCustodianEmail(const std::string& custodianEmail);

  //! custodianExtension getter
  //!
  const std::string& getCustodianExtension() const;

  //! custodianExtension setter
  //!
  void setCustodianExtension(const std::string& custodianExtension);

  //! custodianName getter
  //!
  const std::string& getCustodianName() const;

  //! custodianName setter
  //!
  void setCustodianName(const std::string& custodianName);

 private:
  std::string appName;
  std::string appVersion;
  std::string appDescription;
  std::string custodianName;
  std::string custodianExtension;
  std::string custodianEmail;
};

#endif  // SRC_ABOUTINFO_H_
