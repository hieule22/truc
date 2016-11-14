// Unit tests for Semantic Analyzer.
// Copyright 2016 Hieu Le.

#include "src/parser.h"

#include <memory>
#include <sstream>

#include "gtest/gtest.h"
#include "util/ptr_util.h"

class SemanticAnalyzerTest : public testing::Test {
 protected:
  // Create a parser from given input string.
  std::unique_ptr<Parser> CreateParser(const std::string& input) {
    ss = util::make_unique<std::istringstream>(input);
    return util::make_unique<Parser>(new Scanner(new Buffer(ss.get())));
  }

 private:
  std::unique_ptr<std::istringstream> ss;
};

TEST_F(SemanticAnalyzerTest, ParseValidProgram) {
  EXPECT_TRUE(CreateParser(
      "program foo0; "
      "begin "
        "print 10; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo1; "
        "i: int; "
      "begin "
        "print i; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo2; "
        "procedure add(a: int; b: int) "
        "begin "
          "print(a + b); "
        "end; "
      "begin "
        "add(1, 2); "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo3; "
        "a, b: int; "
      "begin "
        "a := 1; "
        "b := a + 1; "
        "print(a + b); "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo4; "
        "a, b: int; "

        "procedure max(a, b: int) "
        "begin "
          "if (a > b) then begin "
            "print a; "
          "end "
          "else begin "
            "print b; "
          "end; "
        "end; "

      "begin "
        "a := 1; "
        "b := 2; "
        "max(a, b); "
      "end; ")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo5; "
        "a, c: int; "

        "procedure increment(a: int) "
          "c: int; "
        "begin "
          "c := 1; "
          "a := a + c; "
          "print a; "
        "end; "

      "begin "
        "a := 0; "
        "increment(a); "
        "c := 1; "
        "increment(c); "
      "end; ")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo6; "
        "a: bool; "
        "b: int; "
      "begin "
        "while (a) loop begin "
          "print a; "
        "end; "

        "while (b = 1) loop begin "
          "print b; "
        "end; "

        "if (not a) then begin "
          "print a; "
        "end; "

        "if (b < 1) then begin "
          "print b; "
        "end; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo7; "
        "a: bool; "
        "b: int; "
      "begin "
        "while (a or (b > 1)) loop begin "
          "if not a then begin "
            "print(b > 1); "
          "end "
          "else begin "
            "if b = 0 then begin "
              "print a; "
            "end; "
          "end; "
        "end; "
      "end; ")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo8; "
        "a, b, c, d: int; "
        "m, n, p, q: bool; "

        "procedure bar() "
          "a, b, c, d: int; "
          "m, n, p, q: bool; "
        "begin "
          "print(a + b + c + d); "
          "print(m and n and not p or q); "
        "end; "

      "begin "
        "bar(); "
      "end; ")->parse_program());
}

TEST_F(SemanticAnalyzerTest, ParseInvalidProgram) {
  ASSERT_EXIT(CreateParser(
      "program foo; "
        "a: int; "
        "a: bool; "
      "begin "
        "print(a); "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "The identifier a has already been declared.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
      "begin "
        "print(a); "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "The identifier a has not been declared.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
      "begin "
        "if 1 then begin print(1); end; "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Type error: expected BOOL_T found INT_T.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
      "begin "
        "while 1 loop begin print(1); end; "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Type error: expected BOOL_T found INT_T.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
      "begin "
        "print((1 + (1 = 1))); end; "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Type error: expected INT_T found BOOL_T.");

  // TODO(hieule): Fix failing test.
  // ASSERT_EXIT(CreateParser(
  //     "program foo; "
  //     "begin "
  //       "print(((1 = 1) and 1)); end; "
  //     "end;")->parse_program(),
  //             ::testing::ExitedWithCode(EXIT_FAILURE),
  //             "Type error: expected BOOL_T found INT_T.");

  // ASSERT_EXIT(CreateParser(
  //     "program foo; "
  //     "begin "
  //       "if (1 = 1) and 2 then begin "
  //         "print(1); "
  //       "end; "
  //     "end;")->parse_program(),
  //             ::testing::ExitedWithCode(EXIT_FAILURE),
  //             "Type error: expected BOOL_T found INT_T.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
        "procedure increment(a: int) "
        "begin "
          "print(a + 1); "
        "end; "
      "begin "
        "increment(1 = 2); "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Type error: expected INT_T found BOOL_T.");

  ASSERT_EXIT(CreateParser(
      "program foo; "
        "procedure bar() "
        "begin "
          "print 1; "
        "end; "
      "begin "
        "print(bar); "
      "end;")->parse_program(),
              ::testing::ExitedWithCode(EXIT_FAILURE),
              "Type error: expected INT_T or BOOL_T, found PROCEDURE_T.");
}
