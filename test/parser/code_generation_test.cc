// Unit tests for Code Generation.
// NOTE: Comments must be disabled before testing.
// Copyright 2016 Hieu Le.

#include "src/parser.h"

#include <memory>
#include <sstream>

#include "gtest/gtest.h"
#include "util/ptr_util.h"

namespace {

class CodeGenerationTest : public testing::Test {
 protected:
  // Create a parser from given input string.
  std::unique_ptr<Parser> CreateParser(const std::string& input) {
    ss_ = util::make_unique<std::istringstream>(input);
    return util::make_unique<Parser>(new Scanner(new Buffer(ss_.get())));
  }

  void MatchOutput(const std::string& source, const std::string& expected) {
    testing::internal::CaptureStdout();
    EXPECT_TRUE(CreateParser(source)->parse_program());
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected);
  }

 private:
  std::unique_ptr<std::istringstream> ss_;
};

TEST_F(CodeGenerationTest, ParseBasicStatement) {
  // a := 10;
  MatchOutput("program foo; a: int; begin a := 10; end;",

              "_foo:\n"
              "\t\tmove R0, #10\n"
              "\t\tmove a, R0\n"
              "\t\thalt\n"
              "a:\t\tdata 1\n");

  // print a > 1;
  MatchOutput("program foo; a: int; begin print a > 1; end;",

              "_foo:\n"
              "\t\tmove R0, a\n"
              "\t\tsub R0, #1\n"
              "\t\tbrne R0, _false0\n"
              "\t\tbrez R0, _false0\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done1\n"
              "_false0:\n"
              "\t\tmove R0, #0\n"
              "_done1:\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "a:\t\tdata 1\n");

  // print b <> c;
  MatchOutput("program foo; b, c: int; begin print b <> c; end;",

              "_foo:\n"
              "\t\tmove R0, b\n"
              "\t\tsub R0, c\n"
              "\t\tbrez R0, _false0\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done1\n"
              "_false0:\n"
              "\t\tmove R0, #0\n"
              "_done1:\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "b:\t\tdata 1\n"
              "c:\t\tdata 1\n");

  // print d or e;
  MatchOutput("program foo; d, e: bool; begin print d or e; end;",

              "_foo:\n"
              "\t\tmove R0, d\n"
              "\t\tadd R0, e\n"
              "\t\tbrez R0, _done0\n"
              "\t\tmove R0, #1\n"
              "_done0:\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "d:\t\tdata 1\n"
              "e:\t\tdata 1\n");

  // print not f and f;
  MatchOutput("program foo; f: bool; begin print not f and f; end;",

              "_foo:\n"
              "\t\tmove R0, f\n"
              "\t\tnot R0\n"
              "\t\tmul R0, f\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "f:\t\tdata 1\n");

  // print g + h;
  MatchOutput("program foo; g, h: int; begin print g + h; end;",

              "_foo:\n"
              "\t\tmove R0, g\n"
              "\t\tadd R0, h\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "g:\t\tdata 1\n"
              "h:\t\tdata 1\n");

  // print -i * i;
  MatchOutput("program foo; i: int; begin print -i * i; end;",

              "_foo:\n"
              "\t\tmove R0, i\n"
              "\t\tneg R0\n"
              "\t\tmul R0, i\n"

              "\t\toutb R0\n"
              "\t\thalt\n"
              "i:\t\tdata 1\n");

  // a := b;
  MatchOutput("program foo; a, b: int; begin a := b; end;",

              "_foo:\n"
              "\t\tmove R0, b\n"
              "\t\tmove a, R0\n"
              "\t\thalt\n"
              "a:\t\tdata 1\n"
              "b:\t\tdata 1\n");

  // c := d / (e + f * g);
  MatchOutput("program foo; c, d, e, f, g: int;"
              "begin c := d / (e + f * g); end;",

              "_foo:\n"
              "\t\tmove R0, f\n"
              "\t\tmul R0, g\n"
              "\t\tmove R1, e\n"
              "\t\tadd R1, R0\n"

              "\t\tmove R0, d\n"
              "\t\tdiv R0, R1\n"
              "\t\tmove c, R0\n"
              "\t\thalt\n"

              "c:\t\tdata 1\n"
              "d:\t\tdata 1\n"
              "e:\t\tdata 1\n"
              "f:\t\tdata 1\n"
              "g:\t\tdata 1\n");

  // print h;
  MatchOutput("program foo; h: int; begin print h; end;",

              "_foo:\n"
              "\t\tmove R0, h\n"
              "\t\toutb R0\n"
              "\t\thalt\n"
              "h:\t\tdata 1\n");

  // print 10;
  MatchOutput("program foo; begin print 10; end;",

              "_foo:\n"
              "\t\tmove R0, #10\n"
              "\t\toutb R0\n"
              "\t\thalt\n");

  // print i or j or k;
  MatchOutput("program foo; i, j, k: bool;"
              "begin print i or j and k; end;",

              "_foo:\n"
              "\t\tmove R0, j\n"
              "\t\tmul R0, k\n"
              "\t\tmove R1, i\n"
              "\t\tadd R1, R0\n"
              "\t\tbrez R1, _done0\n"
              "\t\tmove R1, #1\n"

              "_done0:\n"
              "\t\toutb R1\n"
              "\t\thalt\n"
              "i:\t\tdata 1\n"
              "j:\t\tdata 1\n"
              "k:\t\tdata 1\n");
}

TEST_F(CodeGenerationTest, IfStatement) {
  // if l then begin m := 0; end;
  MatchOutput("program foo; l: bool; m: int;"
              "begin if l then begin m:= 0; end; end;",

              "_foo:\n"
              "\t\tmove R0, l\n"
              "\t\tbrez R0, _else0\n"
              "\t\tmove R0, #0\n"
              "\t\tmove m, R0\n"
              "\t\tbrun _if_done1\n"
              "_else0:\n"
              "_if_done1:\n"
              "\t\thalt\n"
              "l:\t\tdata 1\n"
              "m:\t\tdata 1\n");

  // if n > p + 1 then begin q := r * 2; end;
  // else begin s := t < v; end;
  MatchOutput("program foo; n, p, q, r, t, v: int; s: bool;"
              "begin if n > p + 1 then begin q := r * 2; end "
              "else begin s := t < v; end; end;",

              "_foo:\n"
              "\t\tmove R0, p\n"
              "\t\tadd R0, #1\n"
              "\t\tmove R1, n\n"
              "\t\tsub R1, R0\n"
              "\t\tbrne R1, _false0\n"
              "\t\tbrez R1, _false0\n"
              "\t\tmove R1, #1\n"
              "\t\tbrun _done1\n"
              "_false0:\n"
              "\t\tmove R1, #0\n"
              "_done1:\n"
              "\t\tbrez R1, _else2\n"

              "\t\tmove R0, r\n"
              "\t\tmul R0, #2\n"
              "\t\tmove q, R0\n"
              "\t\tbrun _if_done3\n"

              "_else2:\n"
              "\t\tmove R0, t\n"
              "\t\tsub R0, v\n"
              "\t\tbrez R0, _false4\n"
              "\t\tbrpo R0, _false4\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done5\n"
              "_false4:\n"
              "\t\tmove R0, #0\n"
              "_done5:\n"
              "\t\tmove s, R0\n"
              "_if_done3:\n"
              "\t\thalt\n"

              "n:\t\tdata 1\n"
              "p:\t\tdata 1\n"
              "q:\t\tdata 1\n"
              "r:\t\tdata 1\n"
              "t:\t\tdata 1\n"
              "v:\t\tdata 1\n"
              "s:\t\tdata 1\n");
}

TEST_F(CodeGenerationTest, WhileStatement) {
  // while a loop begin print b; end;
  MatchOutput("program foo; a, b: bool;"
              "begin while a loop begin print b; end; end;",

              "_foo:\n"
              "_while_cond0:\n"
              "\t\tmove R0, a\n"
              "\t\tbrez R0, _while_done1\n"
              "\t\tmove R0, b\n"
              "\t\toutb R0\n"
              "\t\tbrun _while_cond0\n"
              "_while_done1:\n"

              "\t\thalt\n"
              "a:\t\tdata 1\n"
              "b:\t\tdata 1\n");

  // while w < 10 loop begin x := x + 1; end;
  MatchOutput("program foo; x, w: int;"
              "begin while w < 10 loop begin x := x + 1; end; end;",

              "_foo:\n"
              "_while_cond0:\n"
              "\t\tmove R0, w\n"
              "\t\tsub R0, #10\n"
              "\t\tbrez R0, _false2\n"
              "\t\tbrpo R0, _false2\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done3\n"
              "_false2:\n"
              "\t\tmove R0, #0\n"
              "_done3:\n"
              "\t\tbrez R0, _while_done1\n"

              "\t\tmove R0, x\n"
              "\t\tadd R0, #1\n"
              "\t\tmove x, R0\n"
              "\t\tbrun _while_cond0\n"
              "_while_done1:\n"

              "\t\thalt\n"
              "x:\t\tdata 1\n"
              "w:\t\tdata 1\n");
}

TEST_F(CodeGenerationTest, Expression) {
  MatchOutput("program foo; a: int; "
              "begin a := (a * a) + ((a / a) - (a * (a + a) - a)); end;",

              "_foo:\n"
              "\t\tmove R0, a\n"
              "\t\tmul R0, a\n"
              "\t\tmove R1, a\n"
              "\t\tdiv R1, a\n"

              "\t\tmove R2, a\n"
              "\t\tadd R2, a\n"
              "\t\tmove _spill0, R2\n"
              "\t\tmove R2, a\n"
              "\t\tmul R2, _spill0\n"

              "\t\tsub R2, a\n"
              "\t\tsub R1, R2\n"
              "\t\tadd R0, R1\n"
              "\t\tmove a, R0\n"
              "\t\thalt\n"
              "a:\t\tdata 1\n"
              "_spill0:\tdata 1\n");

  MatchOutput("program foo; a, b: int; begin "
              "a := 1; b := 2; "
              "print 1 / b * a + 4 - b + (a / 2 * 3 + 4 - 5 + (b - -a)); end;",

              "_foo:\n"
              "\t\tmove R0, #1\n"
              "\t\tmove a, R0\n"
              "\t\tmove R0, #2\n"
              "\t\tmove b, R0\n"
              "\t\tmove R0, #1\n"
              "\t\tdiv R0, b\n"
              "\t\tmul R0, a\n"
              "\t\tadd R0, #4\n"
              "\t\tsub R0, b\n"

              "\t\tmove R1, a\n"
              "\t\tdiv R1, #2\n"
              "\t\tmul R1, #3\n"
              "\t\tadd R1, #4\n"
              "\t\tsub R1, #5\n"

              "\t\tmove R2, a\n"
              "\t\tneg R2\n"
              "\t\tmove _spill0, R2\n"
              "\t\tmove R2, b\n"
              "\t\tsub R2, _spill0\n"

              "\t\tadd R1, R2\n"
              "\t\tadd R0, R1\n"
              "\t\toutb R0\n"
              "\t\thalt\n"
              "a:\t\tdata 1\n"
              "b:\t\tdata 1\n"
              "_spill0:\tdata 1\n");

  MatchOutput("program foo; b1, b2, b3, b4, b5: bool; begin "
              "b5 := not (b1 and b2) or not (b3 and b4); "
              "b5 := b1 and ((b2 or b3) and b4) and not b5; end;",

              "_foo:\n"
              "\t\tmove R0, b1\n"
              "\t\tmul R0, b2\n"
              "\t\tnot R0\n"
              "\t\tmove R1, b3\n"
              "\t\tmul R1, b4\n"
              "\t\tnot R1\n"
              "\t\tadd R0, R1\n"
              "\t\tbrez R0, _done0\n"
              "\t\tmove R0, #1\n"
              "_done0:\n"
              "\t\tmove b5, R0\n"

              "\t\tmove R0, b2\n"
              "\t\tadd R0, b3\n"
              "\t\tbrez R0, _done1\n"
              "\t\tmove R0, #1\n"
              "_done1:\n"
              "\t\tmul R0, b4\n"
              "\t\tmove R1, b1\n"
              "\t\tmul R1, R0\n"
              "\t\tmove R0, b5\n"
              "\t\tnot R0\n"
              "\t\tmul R1, R0\n"
              "\t\tmove b5, R1\n"

              "\t\thalt\n"
              "b1:\t\tdata 1\n"
              "b2:\t\tdata 1\n"
              "b3:\t\tdata 1\n"
              "b4:\t\tdata 1\n"
              "b5:\t\tdata 1\n");

  MatchOutput("program foo; a, b, c, d: bool; begin "
              "a := (b and not c) or (not d and not a) and ((10 = 12) "
              "and (not (not c))); end;",

              "_foo:\n"
              "\t\tmove R0, c\n"
              "\t\tnot R0\n"
              "\t\tmove R1, b\n"
              "\t\tmul R1, R0\n"  // Compute b and not c in R1.

              "\t\tmove R0, d\n"
              "\t\tnot R0\n"
              "\t\tmove R2, a\n"
              "\t\tnot R2\n"
              "\t\tmul R0, R2\n"  // Compute not d and not a in R0.

              "\t\tmove R2, #10\n"
              "\t\tsub R2, #12\n"
              "\t\tbrne R2, _false0\n"
              "\t\tbrpo R2, _false0\n"
              "\t\tmove R2, #1\n"
              "\t\tbrun _done1\n"
              "_false0:\n"
              "\t\tmove R2, #0\n"  // Compute 10 = 12 in R2.
              "_done1:\n"

              "\t\tmove _spill2, R2\n"
              "\t\tmove R2, c\n"
              "\t\tnot R2\n"
              "\t\tnot R2\n"
              "\t\tmove _spill3, R2\n"
              "\t\tmove R2, _spill2\n"
              "\t\tmul R2, _spill3\n"  // Compute 10 = 12 and not (not c) in R2.

              "\t\tmul R0, R2\n"
              "\t\tadd R1, R0\n"
              "\t\tbrez R1, _done4\n"
              "\t\tmove R1, #1\n"
              "_done4:\n"
              "\t\tmove a, R1\n"
              "\t\thalt\n"

              "a:\t\tdata 1\n"
              "b:\t\tdata 1\n"
              "c:\t\tdata 1\n"
              "d:\t\tdata 1\n"
              "_spill2:\tdata 1\n"
              "_spill3:\tdata 1\n");
}

TEST_F(CodeGenerationTest, General) {
  MatchOutput("program translate; "
                "sum, current: int; "
              "begin "
                "sum := 0; "
                "current := 1; "
                "while current < 100 loop "
                "begin "
                  "if (current + 1) / 2 = current / 2 then "
                  "begin "
                    "sum := sum + current; "
                  "end; "
                  "current := current + 1; "
                "end; "
                "print sum; "
              "end;",

              "_translate:\n"
              "\t\tmove R0, #0\n"
              "\t\tmove sum, R0\n"
              "\t\tmove R0, #1\n"
              "\t\tmove current, R0\n"

              "_while_cond0:\n"
              "\t\tmove R0, current\n"
              "\t\tsub R0, #100\n"
              "\t\tbrez R0, _false2\n"
              "\t\tbrpo R0, _false2\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done3\n"
              "_false2:\n"
              "\t\tmove R0, #0\n"
              "_done3:\n"
              "\t\tbrez R0, _while_done1\n"

              "\t\tmove R0, current\n"
              "\t\tadd R0, #1\n"
              "\t\tdiv R0, #2\n"
              "\t\tmove R1, current\n"
              "\t\tdiv R1, #2\n"
              "\t\tsub R0, R1\n"
              "\t\tbrne R0, _false4\n"
              "\t\tbrpo R0, _false4\n"
              "\t\tmove R0, #1\n"
              "\t\tbrun _done5\n"
              "_false4:\n"
              "\t\tmove R0, #0\n"
              "_done5:\n"
              "\t\tbrez R0, _else6\n"

              "\t\tmove R0, sum\n"
              "\t\tadd R0, current\n"
              "\t\tmove sum, R0\n"
              "\t\tbrun _if_done7\n"
              "_else6:\n"
              "_if_done7:\n"

              "\t\tmove R0, current\n"
              "\t\tadd R0, #1\n"
              "\t\tmove current, R0\n"
              "\t\tbrun _while_cond0\n"
              "_while_done1:\n"

              "\t\tmove R0, sum\n"
              "\t\toutb R0\n"
              "\t\thalt\n"
              "sum:\t\tdata 1\n"
              "current:\tdata 1\n");
}

}  // namespace
