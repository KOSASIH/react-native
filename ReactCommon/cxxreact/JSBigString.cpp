// Copyright 2004-present Facebook. All Rights Reserved.

#include "JSBigString.h"

#include <fcntl.h>
#include <sys/stat.h>

#include <folly/Memory.h>
#include <folly/ScopeGuard.h>

#if defined(_MSC_VER)
#define open _open
#endif

namespace facebook {
namespace react {

std::unique_ptr<const JSBigFileString> JSBigFileString::fromPath(const std::string& sourceURL) {
  int fd = ::open(sourceURL.c_str(), O_RDONLY);
  folly::checkUnixError(fd, "Could not open file", sourceURL);
  SCOPE_EXIT { CHECK(::close(fd) == 0); };

  struct stat fileInfo;
  folly::checkUnixError(::fstat(fd, &fileInfo), "fstat on bundle failed.");

  return folly::make_unique<const JSBigFileString>(fd, fileInfo.st_size);
}

}  // namespace react
}  // namespace facebook
