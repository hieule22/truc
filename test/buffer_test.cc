// Unit tests for Buffer interface.
// @author Hieu Le
// @version 09/29/2016

#include "src/buffer.h"

#include "gtest/gtest.h"

namespace {

class BufferTest : public Buffer, public testing::Test {
 public:
  BufferTest() {}

  ~BufferTest() {}

  char next_char() override { return '\0'; }

  void unread_char(const char c) override {}
};

TEST_F(BufferTest, BufferFatalError) {
  ASSERT_EXIT(this->buffer_fatal_error(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Exiting on BUFFER FATAL ERROR\n");
 
}

TEST_F(BufferTest, IsWhitespace) {
  EXPECT_TRUE(this->is_whitespace(SPACE));
  EXPECT_TRUE(this->is_whitespace(TAB));
  EXPECT_TRUE(this->is_whitespace(NEW_LINE));

  EXPECT_FALSE(this->is_whitespace('/'));
  EXPECT_FALSE(this->is_whitespace('a'));
  EXPECT_FALSE(this->is_whitespace(','));
}

TEST_F(BufferTest, Validate) {
  EXPECT_TRUE(this->validate(COMMENT_MARKER));
  EXPECT_TRUE(this->validate(NEW_LINE));
  EXPECT_TRUE(this->validate('+'));
  EXPECT_TRUE(this->validate('-'));
  for (int c = 'a'; c <= 'z'; ++c) {
    EXPECT_TRUE(this->validate(static_cast<char>(c)));
  }
  for (int c = '0'; c <= '9'; ++c) {
    EXPECT_TRUE(this->validate(static_cast<char>(c)));
  }

  for (int c = 'A'; c <= 'Z'; ++c) {
    EXPECT_FALSE(this->validate(static_cast<char>(c)));
  }
  EXPECT_FALSE(this->validate(EOF_MARKER));
}

}
