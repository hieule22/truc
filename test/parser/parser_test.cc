// Unit tests for Parser class.
// Copyright 2016 Hieu Le.

#include "src/parser.h"

#include <memory>
#include <sstream>

#include "gtest/gtest.h"
#include "util/ptr_util.h"

namespace {

class ParserTest : public testing::Test {
 protected:
  // Creates a parser from the given input string.
  Parser CreateParser(const std::string& input) {
    ss = util::make_unique<std::istringstream>(input);
    return Parser(new Scanner(new Buffer(ss.get())));
  }

 private:
  std::unique_ptr<std::istringstream> ss;
};

TEST_F(ParserTest, ParseProgramBasic) {
  {
    const std::string program =
        "program a; "
        "begin "
          "print 10; "
        "end;";
      
    Parser parser = CreateParser(program);
    EXPECT_TRUE(parser.parse_program());
  }

  // TODO(hieule): Fix failing tests.
  {
    const std::string program =
      "program a; "
         "i: int; "
       "begin "
         "i := 1; "
         "while (i <= 100) loop begin "
           "i := i * 2; "
         "end; "
         "print i; "
       "end;";

    Parser parser = CreateParser(program);
    EXPECT_TRUE(parser.parse_program());
  }
}

}  // namespace
