// End-to-end tests for lexical analyzer.
// Copyright 2016 Hieu Le.

#include "src/scanner.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

namespace {

// MockScanner reads tokens from expected output file.
class MockScanner : public Scanner {
 public:
  explicit MockScanner(char *filename) : Scanner(filename) {
    infile_.open(filename);
    stream_ = &infile_;
  }

  ~MockScanner() {
    infile_.close();
  }

  Token *next_token() {
    std::string token;
    (*stream_) >> token;

    if (token == "PROGRAM") {
      return new KeywordToken(keyword_attr_type::KW_PROGRAM);
    } else if (token == "PROCEDURE") {
      return new KeywordToken(keyword_attr_type::KW_PROCEDURE);
    } else if (token == "INT") {
      return new KeywordToken(keyword_attr_type::KW_INT);
    } else if (token == "BOOL") {
      return new KeywordToken(keyword_attr_type::KW_BOOL);
    } else if (token == "BEGIN") {
      return new KeywordToken(keyword_attr_type::KW_BEGIN);
    } else if (token == "END") {
      return new KeywordToken(keyword_attr_type::KW_END);
    } else if (token == "IF") {
      return new KeywordToken(keyword_attr_type::KW_IF);
    } else if (token == "THEN") {
      return new KeywordToken(keyword_attr_type::KW_THEN);
    } else if (token == "ELSE") {
      return new KeywordToken(keyword_attr_type::KW_ELSE);
    } else if (token == "WHILE") {
      return new KeywordToken(keyword_attr_type::KW_WHILE);
    } else if (token == "LOOP") {
      return new KeywordToken(keyword_attr_type::KW_LOOP);
    } else if (token == "PRINT") {
      return new KeywordToken(keyword_attr_type::KW_PRINT);
    } else if (token == "NOT") {
      return new KeywordToken(keyword_attr_type::KW_NOT);
    } else if (token == "SEMICOLON") {
      return new PuncToken(punc_attr_type::PUNC_SEMI);
    } else if (token == "COLON") {
      return new PuncToken(punc_attr_type::PUNC_COLON);
    } else if (token == "COMMA") {
      return new PuncToken(punc_attr_type::PUNC_COMMA);
    } else if (token == "ASSIGNMENT") {
      return new PuncToken(punc_attr_type::PUNC_ASSIGN);
    } else if (token == "OPENBRACKET") {
      return new PuncToken(punc_attr_type::PUNC_OPEN);
    } else if (token == "CLOSEBRACKET") {
      return new PuncToken(punc_attr_type::PUNC_CLOSE);
    } else if (token == "EQUAL") {
      return new RelopToken(relop_attr_type::RELOP_EQ);
    } else if (token == "NOTEQUAL") {
      return new RelopToken(relop_attr_type::RELOP_NE);
    } else if (token == "GREATERTHAN") {
      return new RelopToken(relop_attr_type::RELOP_GT);
    } else if (token == "GREATEROREQUAL") {
      return new RelopToken(relop_attr_type::RELOP_GE);
    } else if (token == "LESSTHAN") {
      return new RelopToken(relop_attr_type::RELOP_LT);
    } else if (token == "LESSOREQUAL") {
      return new RelopToken(relop_attr_type::RELOP_LE);
    } else if (token == "ADD") {
      return new AddopToken(addop_attr_type::ADDOP_ADD);
    } else if (token == "SUBTRACT") {
      return new AddopToken(addop_attr_type::ADDOP_SUB);
    } else if (token == "OR") {
      return new AddopToken(addop_attr_type::ADDOP_OR);
    } else if (token == "MULTIPLY") {
      return new MulopToken(mulop_attr_type::MULOP_MUL);
    } else if (token == "DIVIDE") {
      return new MulopToken(mulop_attr_type::MULOP_DIV);
    } else if (token == "AND") {
      return new MulopToken(mulop_attr_type::MULOP_AND);
    } else if (token == "IDENTIFIER") {
      std::string attribute;
      (*stream_) >> attribute;
      return new IdToken(attribute);
    } else if (token == "NUMBER") {
      std::string attribute;
      (*stream_) >> attribute;
      return new NumToken(attribute);
    } else if (token == "ENDOFFILE") {
      return new EofToken();
    }

    std::cerr << "Unrecognized token: " << token << std::endl;
    std::exit(EXIT_FAILURE);

    return nullptr;
  }

 private:
  ifstream infile_;
  istream *stream_;
};

// TODO(hieule): Implement all test cases.
TEST(ScannerTest, EndToEnd) {
  const int N_TESTS = 5;
  for (int i = 0; i < N_TESTS; ++i) {
    char filename[100];
    std::snprintf(filename, sizeof(filename),
                  "test/scanner/data/test%d.in", i);
    Scanner scanner(filename);

    std::snprintf(filename, sizeof(filename),
                  "test/scanner/data/test%d.out", i);
    MockScanner mock(filename);

    std::unique_ptr<Token> actual;
    std::unique_ptr<Token> expected;

    do {
      actual.reset(scanner.next_token());
      expected.reset(mock.next_token());
      EXPECT_EQ(*actual->to_string(), *expected->to_string());
    } while (typeid(*expected) != typeid(EofToken));
  }
}

}  // namespace
