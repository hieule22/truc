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
      "program foo; "
      "begin "
        "print 10; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program bar; "
        "i: int; "
      "begin "
        "print i; "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program quoz; "
        "procedure add(a: int; b: int) "
        "begin "
          "print (a + b); "
        "end; "
      "begin "
        "add(1, 2); "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo; "
        "a, b: int; "
      "begin "
        "a := 1; "
        "b := a + 1; "
        "print(a + b); "
      "end;")->parse_program());
}
