// Unit tests for pointer utilities.
// Copyright 2016 Hieu Le.

#include "util/ptr_util.h"

#include <string>

#include "gtest/gtest.h"

namespace util {
namespace {

TEST(PtrUtilTest, MakeUnique) {
  auto string_ptr = util::make_unique<std::string>("Foo");
  EXPECT_EQ(*string_ptr, "Foo");
}

}  // namespace
}  // namespace util
