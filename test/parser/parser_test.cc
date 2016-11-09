// Unit tests for Parser class.
// Copyright 2016 Hieu Le.

#include "src/parser.h"

#include <memory>
#include <sstream>

#include "gtest/gtest.h"
#include "util/ptr_util.h"

class ParserTest : public testing::Test {
 protected:
  // Creates a parser from the given input string.
  std::unique_ptr<Parser> CreateParser(const std::string& input) {
    ss = util::make_unique<std::istringstream>(input);
    return util::make_unique<Parser>(new Scanner(new Buffer(ss.get())));
  }

 private:
  std::unique_ptr<std::istringstream> ss;
};

TEST_F(ParserTest, ParseExpression) {
  EXPECT_TRUE(CreateParser("1")->parse_expr());
  EXPECT_TRUE(CreateParser("1 * 2")->parse_expr());
  EXPECT_TRUE(CreateParser("foo")->parse_expr());
  EXPECT_TRUE(CreateParser("(2 + 3) * a")->parse_expr());
  EXPECT_TRUE(CreateParser("a = 1")->parse_expr());
  EXPECT_TRUE(CreateParser("((((((1))))))")->parse_expr());
  EXPECT_TRUE(CreateParser("-3")->parse_expr());
  EXPECT_TRUE(CreateParser("-a")->parse_expr());
  EXPECT_TRUE(CreateParser("+(a + b)")->parse_expr());
  EXPECT_TRUE(CreateParser("-(a + 1)")->parse_expr());
  EXPECT_TRUE(CreateParser("not (a = 1)")->parse_expr());
  EXPECT_TRUE(CreateParser("(not foo and quoz)")->parse_expr());
  EXPECT_FALSE(CreateParser("+")->parse_expr());
  EXPECT_FALSE(CreateParser("program")->parse_expr());
  EXPECT_FALSE(CreateParser("+")->parse_expr());
  EXPECT_FALSE(CreateParser("(())")->parse_expr());
}

TEST_F(ParserTest, ParseFactor) {
  EXPECT_TRUE(CreateParser("foo")->parse_factor());
  EXPECT_TRUE(CreateParser("100")->parse_factor());
  EXPECT_TRUE(CreateParser("(foo)")->parse_factor());
  EXPECT_TRUE(CreateParser("(1 + 2)")->parse_factor());
  EXPECT_TRUE(CreateParser("-(1 + 2)")->parse_factor());
  EXPECT_TRUE(CreateParser("not (foo and bar)")->parse_factor());
  EXPECT_FALSE(CreateParser("not")->parse_factor());
  EXPECT_FALSE(CreateParser("program")->parse_factor());
}

TEST_F(ParserTest, ParseSign) {
  EXPECT_TRUE(CreateParser("-")->parse_sign());
  EXPECT_TRUE(CreateParser("+")->parse_sign());
  EXPECT_TRUE(CreateParser("not")->parse_sign());
  EXPECT_FALSE(CreateParser("foo")->parse_sign());
  EXPECT_FALSE(CreateParser("123")->parse_sign());
  EXPECT_FALSE(CreateParser("=")->parse_sign());
  EXPECT_FALSE(CreateParser("")->parse_sign());
}

TEST_F(ParserTest, ParseProgramBasic) {
  EXPECT_TRUE(CreateParser(
      "program a; "
      "begin "
        "print 10; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program a; "
        "i: int; "
      "begin "
        "i := 1; "
        "while (i <= 100) loop begin "
          "i := i * 2; "
        "end; "
        "print i; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program a; "
        "a, b: int; "
      "begin "
        "a := 2; "
        "b := 3; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program a; "
        "a, b: int; "
      "begin "
        "if (a < b) then begin "
          "print a; "
        "end "
        "else begin "
          "print b; "
        "end; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program looper; "
        "a: int; "
        "b: bool; "
      "begin "
        "a := 0; "
        "b := 0 = 0; "
        "while b loop begin "
           "if a <= 2 then begin "
             "a := (a + 1) * 3; "
           "end "
           "else begin "
             "a := (a - 1) * 2; "
           "end; "
           "b := a > 1; "
           "print a; "
         "end; "
      "end;")->parse_program());
}
