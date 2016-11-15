// Unit tests for Parser class.
// NOTE: PARSER_TEST_MODE flag must be enabled before testing.
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

  EXPECT_TRUE(CreateParser(
      "program prog; "
        "a, b, c: int; "
        "d, e, f: bool; "

        "procedure add(p,q: int; r: bool) "
          "m, n: int; "
        "begin print(p + q + m + n); end; "

      "begin "
        "a := 1; "
        "add(b, c, d); "
        "while d loop begin print(a); end; "
        "if e then begin print(b); end; "
        "if a > 0 then begin "
          "print(b); "
        "end else begin "
          "print(c); "
        "end; "
        "foo(a - b, b * c); "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo; "
        "procedure bar(a, b: int; c, d: bool) "
          "e: int; "
        "begin "
          "c := c and d; "
          "e := a + b; "
        "end; "
      "begin "
        "bar(1, 2, true, false); "
      "end;")->parse_program());

  EXPECT_TRUE(CreateParser(
      "program foo; "
        "procedure add(a, b: int) "
        "begin "
          "print(a + b); "
        "end; "

        "procedure subtract(a, b: int) "
        "begin "
          "print(a - b); "
        "end; "
      "begin "
        "add(1, 2); "
        "subtract(1, 2); "
      "end;")->parse_program());

  EXPECT_TRUE(
      CreateParser("program foo; begin print(1); end;")->parse_program());
  EXPECT_FALSE(CreateParser("program foo; begin end;")->parse_program());
}

TEST_F(ParserTest, ParseBlock) {
  EXPECT_TRUE(CreateParser("begin a := 1; end")->parse_block());
  EXPECT_TRUE(CreateParser("begin a := 1; b := 2; end")->parse_block());
  EXPECT_TRUE(CreateParser(
      "begin "
        "a := 1; "
        "while a loop begin print(a); end; "
        "if a = 1 then begin print(a); end; "
        "print(10); foo(10); "
      "end")->parse_block());

  EXPECT_FALSE(CreateParser("begin a := 1;")->parse_block());
  EXPECT_FALSE(CreateParser("a := 1; end")->parse_block());
  EXPECT_FALSE(CreateParser("begin foo end")->parse_block());
  EXPECT_FALSE(CreateParser("begin end")->parse_block());
}

TEST_F(ParserTest, ParseVariableDecl) {
  EXPECT_TRUE(CreateParser("a: int")->parse_variable_decl());
  EXPECT_TRUE(CreateParser("a: bool")->parse_variable_decl());
  EXPECT_TRUE(CreateParser("a, b: int")->parse_variable_decl());
  EXPECT_TRUE(CreateParser("a, b: bool")->parse_variable_decl());
  EXPECT_FALSE(CreateParser("1: int")->parse_variable_decl());
}

TEST_F(ParserTest, ParseProcedureDecl) {
  EXPECT_TRUE(CreateParser(
      "procedure multiply(a, b: int) "
      "begin "
        "print(a * b); "
      "end")->parse_procedure_decl());

  EXPECT_TRUE(CreateParser(
      "procedure bar(a, b: int; condition: bool) "
      "begin "
        "if condition then begin print(a + b); end "
        "else begin print(a - b); end; "
      "end")->parse_procedure_decl());

  EXPECT_FALSE(CreateParser(
      "procedure bar(a: int begin print(a); end")->parse_procedure_decl());

  EXPECT_FALSE(CreateParser(
      "procedure 123(a: int) begin print(a); end")->parse_procedure_decl());
}

TEST_F(ParserTest, ParseStmt) {
  EXPECT_TRUE(CreateParser("a := b + 1")->parse_stmt());
  EXPECT_TRUE(CreateParser("foo(1)")->parse_stmt());
  EXPECT_TRUE(CreateParser("if (a) then begin print(a); end")->parse_stmt());

  // TODO(hieule): Fix linking errors.
  // EXPECT_TRUE(
  //     CreateParser("while (a) loop begin print(b); end")->parse_stmt());
}

TEST_F(ParserTest, ParseIfStmt) {
  EXPECT_TRUE(CreateParser(
      "if (a < b) then "
      "begin "
        "print(a); "
      "end")->parse_if_stmt());

  EXPECT_TRUE(CreateParser(
      "if (a < b) then begin "
        "print(a); "
      "end "
      "else begin "
        "print(b); "
      "end")->parse_if_stmt());

  EXPECT_TRUE(CreateParser(
      "if (a < b) then begin "
        "if (a < c) then begin "
          "print(a); "
        "end "
        "else begin "
           "print(c); "
        "end; "
      "end "
      "else begin "
        "if (b < c) then begin "
          "print(b); "
        "end "
        "else begin "
          "print(c); "
        "end; "
      "end")->parse_if_stmt());

  EXPECT_FALSE(CreateParser(
      "if (a < b) then "
        "print(a); "
      "end")->parse_if_stmt());

  EXPECT_FALSE(CreateParser(
      "if (a < b) then begin "
        "print(a); "
      "end "
      "else begin "
        "print(b); "
      "en")->parse_if_stmt());
}

TEST_F(ParserTest, ParseWhileStmt) {
  EXPECT_TRUE(CreateParser(
      "while (a > 0) loop begin "
        "print(a); "
        "a := a - 1; "
      "end")->parse_while_stmt());

  EXPECT_TRUE(CreateParser(
      "while (a > 0) loop begin "
        "b := 5; "
        "while (b > 0) loop begin "
          "print(b); "
          "b := b - 1; "
        "end; "
        "a := a - 1; "
      "end")->parse_while_stmt());

  EXPECT_TRUE(CreateParser(
      "while (a > 0) loop begin "
        "if ((a and 1) = 0) then begin "
          "a := a / 2; "
        "end "
        "else begin "
          "a := a + 1; "
        "end; "
      "end")->parse_while_stmt());

  EXPECT_FALSE(CreateParser(
      "while b begin "
        "print(a); "
      "end")->parse_while_stmt());

  EXPECT_FALSE(CreateParser(
      "while b loop begin print(a); ")->parse_while_stmt());
}

TEST_F(ParserTest, ParsePrintStmt) {
  EXPECT_TRUE(CreateParser("print(1)")->parse_print_stmt());
  EXPECT_TRUE(CreateParser("print(((foo)))")->parse_print_stmt());
  EXPECT_TRUE(CreateParser("print(a = b)")->parse_print_stmt());
  EXPECT_FALSE(CreateParser("print((a)")->parse_print_stmt());
  EXPECT_FALSE(CreateParser("prin(a)")->parse_print_stmt());
}

TEST_F(ParserTest, ParseExpression) {
  expr_type expr_type_result = GARBAGE_T;
  EXPECT_TRUE(CreateParser("1")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("1 * 2")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("foo")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("(2 + 3) * a")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("a = 1")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("((((((1))))))")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("-3")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("-a")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("+(a + b)")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("-(a + 1)")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("not (a = 1)")->parse_expr(expr_type_result));
  EXPECT_TRUE(CreateParser("(not foo and quoz)")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("+")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("program")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("+")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("(())")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("((3 + 1)")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("())")->parse_expr(expr_type_result));
  EXPECT_FALSE(CreateParser("(()")->parse_expr(expr_type_result));
}

TEST_F(ParserTest, ParseTerm) {
  expr_type term_type = GARBAGE_T;
  EXPECT_TRUE(CreateParser("1 * 2")->parse_term(term_type));
  EXPECT_TRUE(CreateParser("1 * (2 + 3)")->parse_term(term_type));
  EXPECT_TRUE(CreateParser("foo * (bar + quoz)")->parse_term(term_type));
}

TEST_F(ParserTest, ParseFactor) {
  expr_type factor_type = GARBAGE_T;
  EXPECT_TRUE(CreateParser("foo")->parse_factor(factor_type));
  EXPECT_TRUE(CreateParser("100")->parse_factor(factor_type));
  EXPECT_TRUE(CreateParser("(foo)")->parse_factor(factor_type));
  EXPECT_TRUE(CreateParser("(1 + 2)")->parse_factor(factor_type));
  EXPECT_TRUE(CreateParser("-(1 + 2)")->parse_factor(factor_type));
  EXPECT_TRUE(CreateParser("not (foo and bar)")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("not")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("program")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("((1 + 2)")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("(while)")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("()")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("((")->parse_factor(factor_type));
  EXPECT_FALSE(CreateParser("")->parse_factor(factor_type));
}

TEST_F(ParserTest, ParseSign) {
  expr_type sign_type = GARBAGE_T;
  EXPECT_TRUE(CreateParser("-")->parse_sign(sign_type));
  EXPECT_TRUE(CreateParser("+")->parse_sign(sign_type));
  EXPECT_TRUE(CreateParser("not")->parse_sign(sign_type));
  EXPECT_FALSE(CreateParser("foo")->parse_sign(sign_type));
  EXPECT_FALSE(CreateParser("123")->parse_sign(sign_type));
  EXPECT_FALSE(CreateParser("=")->parse_sign(sign_type));
  EXPECT_FALSE(CreateParser("")->parse_sign(sign_type));
}
