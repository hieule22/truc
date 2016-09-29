// Unit tests for FileBuffer class.
// @author Hieu Le
// @version 09/29/2016

#include "src/filebuffer.h"

#include "gtest/gtest.h"

namespace {

TEST(FileBufferDeathTest, ConstructWithInvalidFilename) {
  ASSERT_EXIT( { FileBuffer buffer("Foo"); },
               ::testing::ExitedWithCode(EXIT_FAILURE),
               "c*Can't open source file Fooc*");
}

}  // namespace
