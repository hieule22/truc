// Unit tests for Scanner class.
// Copyright 2016 Hieu Le.

#include "src/scanner.h"

#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "src/streambuffer.h"

// Keywords.
#define PROGRAM   KeywordToken(keyword_attr_type::KW_PROGRAM)
#define PROCEDURE KeywordToken(keyword_attr_type::KW_PROCEDURE)
#define INT       KeywordToken(keyword_attr_type::KW_INT)
#define BOOL      KeywordToken(keyword_attr_type::KW_BOOL)
#define BEGIN     KeywordToken(keyword_attr_type::KW_BEGIN)
#define END       KeywordToken(keyword_attr_type::KW_END)
#define IF        KeywordToken(keyword_attr_type::KW_IF)
#define THEN      KeywordToken(keyword_attr_type::KW_THEN)
#define ELSE      KeywordToken(keyword_attr_type::KW_ELSE)
#define WHILE     KeywordToken(keyword_attr_type::KW_WHILE)
#define LOOP      KeywordToken(keyword_attr_type::KW_LOOP)
#define PRINT     KeywordToken(keyword_attr_type::KW_PRINT)
#define NOT       KeywordToken(keyword_attr_type::KW_NOT)

// Punctuations.
#define SEMICOLON    PuncToken(punc_attr_type::PUNC_SEMI)
#define COLON        PuncToken(punc_attr_type::PUNC_COLON)
#define COMMA        PuncToken(punc_attr_type::PUNC_COMMA)
#define ASSIGNMENT   PuncToken(punc_attr_type::PUNC_ASSIGN)
#define OPENBRACKET  PuncToken(punc_attr_type::PUNC_OPEN)
#define CLOSEBRACKET PuncToken(punc_attr_type::PUNC_CLOSE)

// Relational operators.
#define EQUAL          RelopToken(relop_attr_type::RELOP_EQ)
#define NOTEQUAL       RelopToken(relop_attr_type::RELOP_NE)
#define GREATERTHAN    RelopToken(relop_attr_type::RELOP_GT)
#define GREATEROREQUAL RelopToken(relop_attr_type::RELOP_GE)
#define LESSTHAN       RelopToken(relop_attr_type::RELOP_LT)
#define LESSOREQUAL    RelopToken(relop_attr_type::RELOP_LE)

// Additive operators.
#define ADD      AddopToken(addop_attr_type::ADDOP_ADD)
#define SUBTRACT AddopToken(addop_attr_type::ADDOP_SUB)
#define OR       AddopToken(addop_attr_type::ADDOP_OR)

// Multiplicative operators.
#define MULTIPLY MulopToken(mulop_attr_type::MULOP_MUL)
#define DIVIDE   MulopToken(mulop_attr_type::MULOP_DIV)
#define AND      MulopToken(mulop_attr_type::MULOP_AND)

// Identifiers and numbers.
#define IDENTIFIER(id) IdToken(id)
#define NUMBER(num)    NumToken(num)

// EOF
#define ENDOFFILE EofToken()

namespace {

class ScannerTest : public testing::Test {
 protected:
  // Creates a character buffer from given input string.
  std::unique_ptr<Buffer> CreateBuffer(const std::string& input) {
    ss = std::make_unique<std::istringstream>(input);
    return std::make_unique<StreamBuffer>(ss.get());
  }

  // Checks if the token represented in input string matches expected token.
  void MatchSingleToken(const std::string& input, const Token& expected) {
    Scanner scanner(CreateBuffer(input));
    std::unique_ptr<Token> actual(scanner.next_token());
    EXPECT_EQ(*actual->to_string(), *expected.to_string());
    EXPECT_EQ(*scanner.next_token()->to_string(), *ENDOFFILE.to_string());
  }

  // Tests if the tokens represented in input string matches a list of
  // expected tokens.
  void MatchTokens(const std::string& input, const std::vector<Token*>& token) {
    Scanner scanner(CreateBuffer(input));
    for (const auto& expected : token) {
      std::unique_ptr<Token> actual(scanner.next_token());
      EXPECT_EQ(*actual->to_string(), *expected->to_string());
    }
  }

 private:
  std::unique_ptr<std::istringstream> ss;
};

TEST_F(ScannerTest, NextTokenBasic) {
  MatchSingleToken("program", PROGRAM);
  MatchSingleToken("procedure", PROCEDURE);
  MatchSingleToken("int", INT);
  MatchSingleToken("bool", BOOL);
  MatchSingleToken("begin", BEGIN);
  MatchSingleToken("end", END);
  MatchSingleToken("if", IF);
  MatchSingleToken("then", THEN);
  MatchSingleToken("else", ELSE);
  MatchSingleToken("while", WHILE);
  MatchSingleToken("loop", LOOP);
  MatchSingleToken("print", PRINT);
  MatchSingleToken("not", NOT);

  MatchSingleToken(";", SEMICOLON);
  MatchSingleToken(":", COLON);
  MatchSingleToken(",", COMMA);
  MatchSingleToken(":=", ASSIGNMENT);
  MatchSingleToken("(", OPENBRACKET);
  MatchSingleToken(")", CLOSEBRACKET);

  MatchSingleToken("=", EQUAL);
  MatchSingleToken("<>", NOTEQUAL);
  MatchSingleToken(">", GREATERTHAN);
  MatchSingleToken(">=", GREATEROREQUAL);
  MatchSingleToken("<", LESSTHAN);
  MatchSingleToken("<=", LESSOREQUAL);

  MatchSingleToken("+", ADD);
  MatchSingleToken("-", SUBTRACT);
  MatchSingleToken("or", OR);

  MatchSingleToken("*", MULTIPLY);
  MatchSingleToken("/", DIVIDE);
  MatchSingleToken("and", AND);

  MatchSingleToken("foobarquoz", IDENTIFIER("foobarquoz"));
  MatchSingleToken("a", IDENTIFIER("a"));
  MatchSingleToken("z", IDENTIFIER("z"));
  MatchSingleToken("ints", IDENTIFIER("ints"));
  MatchSingleToken("loo", IDENTIFIER("loo"));
  MatchSingleToken("elseif", IDENTIFIER("elseif"));
  MatchSingleToken("myprocedure", IDENTIFIER("myprocedure"));
  MatchSingleToken("andnota23", IDENTIFIER("andnota23"));
  MatchSingleToken("a1b2c3d4e5f6g7h8i9", IDENTIFIER("a1b2c3d4e5f6g7h8i9"));
  MatchSingleToken("a123456789", IDENTIFIER("a123456789"));

  MatchSingleToken("1", NUMBER("1"));
  MatchSingleToken("9999999999999", NUMBER("9999999999999"));
  MatchSingleToken("000000000000", NUMBER("000000000000"));
  MatchSingleToken("123456789", NUMBER("123456789"));

  MatchSingleToken("", ENDOFFILE);
}

// TODO(hieutle): Fix memory leaks.
TEST_F(ScannerTest, MultipleNextTokens) {
  MatchTokens("int a = 1;", { new INT, new IDENTIFIER("a"), new EQUAL,
          new NUMBER("1"), new SEMICOLON, new ENDOFFILE });
  MatchTokens("int a = 1 + 1 + 1;", { new INT, new IDENTIFIER("a"),
          new EQUAL, new NUMBER("1"), new ADD, new NUMBER("1"), new ADD,
          new NUMBER("1"), new SEMICOLON, new ENDOFFILE });
  MatchTokens("while else int", { new WHILE, new ELSE, new INT,
          new ENDOFFILE });
  MatchTokens("foo bar baz", { new IDENTIFIER("foo"),
          new IDENTIFIER("bar"), new IDENTIFIER("baz"), new ENDOFFILE });
  MatchTokens("and nota2", { new AND, new IDENTIFIER("nota2"), new ENDOFFILE});
  MatchTokens("and not a23", { new AND, new NOT, new IDENTIFIER("a23"),
          new ENDOFFILE });
  MatchTokens("123abc", { new NUMBER("123"), new IDENTIFIER("abc"),
          new ENDOFFILE });
  MatchTokens("integer >= 2", { new IDENTIFIER("integer"), new GREATEROREQUAL,
          new NUMBER("2"), new ENDOFFILE });
  MatchTokens("integer > = 2", { new IDENTIFIER("integer"), new GREATERTHAN,
          new EQUAL, new NUMBER("2"), new ENDOFFILE });
  MatchTokens("if(a+1)*2=2then", { new IF, new OPENBRACKET, new IDENTIFIER("a"),
          new ADD, new NUMBER("1"), new CLOSEBRACKET, new MULTIPLY,
          new NUMBER("2"), new EQUAL, new NUMBER("2"), new THEN,
          new ENDOFFILE });
  MatchTokens("a1+b2<>c3", { new IDENTIFIER("a1"), new ADD,
          new IDENTIFIER("b2"), new NOTEQUAL, new IDENTIFIER("c3"),
          new ENDOFFILE });
}

}  // namespace

