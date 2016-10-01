// Unit tests for Scanner class.
// Copyright 2016 Hieu Le.

#include "src/scanner.h"

#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "src/buffer.h"

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
  Buffer* CreateBuffer(const std::string& input) {
    ss = std::make_unique<std::istringstream>(input);
    return new Buffer(ss.get());
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
  MatchSingleToken("p", IDENTIFIER("p"));
  MatchSingleToken("pr", IDENTIFIER("pr"));
  MatchSingleToken("pro", IDENTIFIER("pro"));
  MatchSingleToken("prog", IDENTIFIER("prog"));
  MatchSingleToken("progr", IDENTIFIER("progr"));
  MatchSingleToken("progra", IDENTIFIER("progra"));
  MatchSingleToken("program", PROGRAM);
  MatchSingleToken("programs", IDENTIFIER("programs"));

  MatchSingleToken("proc", IDENTIFIER("proc"));
  MatchSingleToken("proce", IDENTIFIER("proce"));
  MatchSingleToken("proced", IDENTIFIER("proced"));
  MatchSingleToken("procedu", IDENTIFIER("procedu"));
  MatchSingleToken("procedur", IDENTIFIER("procedur"));
  MatchSingleToken("procedure", PROCEDURE);
  MatchSingleToken("procedures", IDENTIFIER("procedures"));


  MatchSingleToken("i", IDENTIFIER("i"));
  MatchSingleToken("in", IDENTIFIER("in"));
  MatchSingleToken("int", INT);
  MatchSingleToken("ints", IDENTIFIER("ints"));

  MatchSingleToken("b", IDENTIFIER("b"));
  MatchSingleToken("bo", IDENTIFIER("bo"));
  MatchSingleToken("boo", IDENTIFIER("boo"));
  MatchSingleToken("bool", BOOL);
  MatchSingleToken("boolean", IDENTIFIER("boolean"));

  MatchSingleToken("be", IDENTIFIER("be"));
  MatchSingleToken("beg", IDENTIFIER("beg"));
  MatchSingleToken("begi", IDENTIFIER("begi"));
  MatchSingleToken("begin", BEGIN);
  MatchSingleToken("beginning", IDENTIFIER("beginning"));

  MatchSingleToken("e", IDENTIFIER("e"));
  MatchSingleToken("en", IDENTIFIER("en"));
  MatchSingleToken("end", END);
  MatchSingleToken("end123", IDENTIFIER("end123"));
  
  MatchSingleToken("if", IF);
  MatchSingleToken("iffy", IDENTIFIER("iffy"));

  MatchSingleToken("t", IDENTIFIER("t"));
  MatchSingleToken("th", IDENTIFIER("th"));
  MatchSingleToken("the", IDENTIFIER("the"));
  MatchSingleToken("then", THEN);
  MatchSingleToken("thenprogram", IDENTIFIER("thenprogram"));

  MatchSingleToken("e", IDENTIFIER("e"));
  MatchSingleToken("el", IDENTIFIER("el"));
  MatchSingleToken("els", IDENTIFIER("els"));
  MatchSingleToken("else", ELSE);
  MatchSingleToken("elseif", IDENTIFIER("elseif"));

  MatchSingleToken("w", IDENTIFIER("w"));
  MatchSingleToken("wh", IDENTIFIER("wh"));
  MatchSingleToken("whi", IDENTIFIER("whi"));
  MatchSingleToken("whil", IDENTIFIER("whil"));
  MatchSingleToken("while", WHILE);
  MatchSingleToken("whilelse", IDENTIFIER("whilelse"));

  MatchSingleToken("l", IDENTIFIER("l"));
  MatchSingleToken("lo", IDENTIFIER("lo"));
  MatchSingleToken("loo", IDENTIFIER("loo"));
  MatchSingleToken("loop", LOOP);
  MatchSingleToken("looprint", IDENTIFIER("looprint"));

  MatchSingleToken("pri", IDENTIFIER("pri"));
  MatchSingleToken("prin", IDENTIFIER("prin"));
  MatchSingleToken("print", PRINT);
  MatchSingleToken("printend", IDENTIFIER("printend"));

  MatchSingleToken("n", IDENTIFIER("n"));  
  MatchSingleToken("no", IDENTIFIER("no"));
  MatchSingleToken("not", NOT);
  MatchSingleToken("not123", IDENTIFIER("not123"));

  MatchSingleToken("o", IDENTIFIER("o"));
  MatchSingleToken("or", OR);
  MatchSingleToken("orr", IDENTIFIER("orr"));

  MatchSingleToken("a", IDENTIFIER("a"));
  MatchSingleToken("an", IDENTIFIER("an"));
  MatchSingleToken("and", AND);
  MatchSingleToken("andd", IDENTIFIER("andd"));

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

  MatchSingleToken("*", MULTIPLY);
  MatchSingleToken("/", DIVIDE);

  MatchSingleToken("foobarquoz", IDENTIFIER("foobarquoz"));
  MatchSingleToken("a", IDENTIFIER("a"));
  MatchSingleToken("z", IDENTIFIER("z"));
  MatchSingleToken("ints", IDENTIFIER("ints"));
  MatchSingleToken("loo", IDENTIFIER("loo"));
  MatchSingleToken("elseif", IDENTIFIER("elseif"));
  MatchSingleToken("myprocedure", IDENTIFIER("myprocedure"));
  MatchSingleToken("procedures", IDENTIFIER("procedures"));
  MatchSingleToken("andnota23", IDENTIFIER("andnota23"));
  MatchSingleToken("a1b2c3d4e5f6g7h8i9", IDENTIFIER("a1b2c3d4e5f6g7h8i9"));
  MatchSingleToken("a123456789", IDENTIFIER("a123456789"));

  MatchSingleToken("1", NUMBER("1"));
  MatchSingleToken("9999999999999", NUMBER("9999999999999"));
  MatchSingleToken("000000000000", NUMBER("000000000000"));
  MatchSingleToken("123456789", NUMBER("123456789"));

  MatchSingleToken("", ENDOFFILE);
}

// TODO(hieule22): Fix memory leaks.
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

// TODO(hieule22): Fix memory leaks.
TEST_F(ScannerTest, StressTest) {
  std::string input;
  std::vector<Token*> expected;
  Token* tokens[] = {new IDENTIFIER("foo"), new EQUAL, new NUMBER("1"), new AND,
                     new IDENTIFIER("bar"), new NOTEQUAL, new NUMBER("2")};
  for (int i = 0; i < 20000; ++i) {
    input.append("foo = 1 and bar <> 2");
    for (auto iter = std::begin(tokens); iter != std::end(tokens); ++iter) {
      expected.push_back(*iter);
    }
  }
  expected.push_back(new ENDOFFILE);
  MatchTokens(input, expected);
}

// TODO(hieule22): Fix memory leaks.
TEST_F(ScannerTest, EndToEnd) {
  MatchTokens("\n\n##This is a comment.\n"
              "int a=2; #This is another comment\t\t$$$%%%!!!\n"
              "procedures \n \t progra \n \t ints a99999"
              "#One last comment \t \t # Hello world $$$ #### \n\n\n\t",
              { new INT, new IDENTIFIER("a"), new EQUAL, new NUMBER("2"),
                    new SEMICOLON, new IDENTIFIER("procedures"),
                    new IDENTIFIER("progra"), new IDENTIFIER("ints"),
                    new IDENTIFIER("a99999"), new ENDOFFILE });
  MatchTokens("#Helloworld\t\t\t\t\t\t\t#$%^&*&^^^\n\n\n\n\n 123abc",
	      { new NUMBER("123"), new IDENTIFIER("abc"), new ENDOFFILE });
}

}  // namespace
