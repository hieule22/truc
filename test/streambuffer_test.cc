// Unit tests for StreamBuffer class.
// @author Hieu Le
// @version 09/28/2016

#include "src/streambuffer.h"

#include <sstream>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

// Test if StreamBuffer generates on specified input an expected sequence of
// characters.
void TestNextChar(const std::string& input,
                  const std::vector<char>& expected) {
  std::istringstream ss(input);
  StreamBuffer buffer(&ss);
  for (const char c : expected) {
    EXPECT_EQ(buffer.next_char(), c);
  }
}

TEST(StreamBufferTest, NextCharBasic) {
  TestNextChar("bool", {'b', 'o', 'o', 'l', EOF_MARKER});
  TestNextChar("int a;", {'i', 'n', 't', SPACE, 'a', ';', EOF_MARKER});
  TestNextChar("a = 3;", {'a', SPACE, '=', SPACE, '3', ';', EOF_MARKER});
  TestNextChar("if(a )", {'i', 'f', '(', 'a', SPACE, ')', EOF_MARKER});
  TestNextChar(";:(),=<>+-",
               {';', ':', '(', ')', ',', '=', '<', '>', '+', '-', EOF_MARKER});
  TestNextChar("", {EOF_MARKER});
  TestNextChar("a", {'a', EOF_MARKER, EOF_MARKER, EOF_MARKER});
}

TEST(StreamBufferTest, NextCharWithWhitespace) {
  TestNextChar("a b", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("a\n\nb", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("\n\n\t a", {'a', EOF_MARKER});
  TestNextChar("a \n\t ", {'a', SPACE, EOF_MARKER});
  TestNextChar("  \n\n\t\t  ", {EOF_MARKER});
  TestNextChar("a\n", {'a', SPACE, EOF_MARKER});
}

TEST(StreamBufferTest, NextCharWithComments) {
  TestNextChar("f#abc def ghi\nb", {'f', SPACE, 'b', EOF_MARKER});
  TestNextChar("a#foo quoz bar \n#bar\nb", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("#this #is #comment\na#abcxyz", {'a', SPACE, EOF_MARKER});
  TestNextChar("a#this\t\tis a\t\tcomment\nb", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("#this is a comment\na", {'a', EOF_MARKER});
  TestNextChar("a#this is a comment", {'a', SPACE, EOF_MARKER});
  TestNextChar("#this is a comment", {EOF_MARKER});
  TestNextChar("abc #!@#$%^&*\n", {'a', 'b', 'c', SPACE, EOF_MARKER});
  TestNextChar("a#$$$\nb", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("#$$$", {EOF_MARKER});
}


TEST(StreamBufferTest, NextCharWithMixingWhitespaceAndComments) {
  TestNextChar("a #foo bar\n b", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar("#foo bar\n\n\t  a b", {'a', SPACE, 'b', EOF_MARKER});
  TestNextChar(" a #foo bar \n \t   b #foo\n\t c#foo bar",
               {'a', SPACE, 'b', SPACE, 'c', SPACE, EOF_MARKER});
  TestNextChar("  a #$$$foo$$$\t\t\t  ^^^bar^^^\n\n\n\n b",
               {'a', SPACE, 'b', EOF_MARKER});
}

TEST(StreamBufferTest, NextCharWithLongInputStream) {
  {
    std::string input;
    for (int i = 0; i < 20000; ++i) {
      input.push_back('a');
    }
    std::vector<char> expected(input.begin(), input.end());
    expected.push_back(EOF_MARKER);
    TestNextChar(input, expected);
  }
  {
    std::string input = "a";
    for (int i = 0; i < 20000; ++i) {
      input.append(" \n\t");
    }
    TestNextChar(input, {'a', SPACE, EOF_MARKER});
  }
  {
    std::string input = "#";
    for (int i = 0; i < 20000; ++i){
      input.append("#$%^&");
    }
    TestNextChar(input, {EOF_MARKER});
    input.append("\na");
    TestNextChar(input, {'a', EOF_MARKER});
  }
  {
    std::string input;
    std::vector<char> expected;
    for (int i = 0; i < 20000; ++i) {
      input.append("\n\t#abc$$$$@@@\n a #^&*0099** #0\t\t\t123\n");
      expected.push_back('a');
      expected.push_back(SPACE);
    }
    expected.push_back(EOF_MARKER);
    TestNextChar(input, expected);
  }
}

TEST(StreamBufferTest, UnreadCharBasic) {
  std::istringstream ss("a");
  StreamBuffer buffer(&ss);
  const char result = buffer.next_char();
  EXPECT_EQ(result, 'a');
  buffer.unread_char(result);
  EXPECT_EQ(buffer.next_char(), result);
  EXPECT_EQ(buffer.next_char(), EOF_MARKER);
  buffer.unread_char(EOF_MARKER);
  EXPECT_EQ(buffer.next_char(), EOF_MARKER);
}

TEST(StreamBufferDeathTest, NextCharIllegalInput) {
  {
    std::istringstream ss("FOO");
    StreamBuffer buffer(&ss);
    ASSERT_EXIT(buffer.next_char(),
                 ::testing::ExitedWithCode(EXIT_FAILURE),
                 "c*Invalid character: Fc*");
  }
  {
    std::istringstream ss("%^&");
    StreamBuffer buffer(&ss);
    ASSERT_EXIT(buffer.next_char(),
                ::testing::ExitedWithCode(EXIT_FAILURE),
                "c*Invalid character: %c*");
  }
  {
    std::istringstream ss("$");
    StreamBuffer buffer(&ss);
    ASSERT_EXIT(buffer.next_char(),
                ::testing::ExitedWithCode(EXIT_FAILURE),
                "c*Invalid character: \\$c*");
  }
  {
    std::istringstream ss("#$$$$$\nabc#%%%%%%\n%$");
    StreamBuffer buffer(&ss);
    ASSERT_EXIT( { while (true) buffer.next_char(); },
		::testing::ExitedWithCode(EXIT_FAILURE),
		"c*Invalid character: \\%c*");
  }
}

}  // namespace
