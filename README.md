# CS420: Compiler

## About

This repository contains the basic implementation of a compiler for the
programming language TruPL. As the adopted C++ syntax is somewhat archaic,
check out [trupl-compiler](https://github.com/hieule22/trupl-compiler) for
a more updated version of the same compiler.

## Installation

The source package can be built using [Bazel](http://bazel.io) or GNU-compatible
Make.

* Obtain a local clone of the repository:

```bash
git clone https://github.com/hieule22/cs420.git
```

* Build the executable binary:

   * GNU Make: `cd src/ && make truc`

   * Bazel: `bazel build src:truc`

* Supply the compiler with a full path to the targeted source file:

   * GNU Make: `src/truc path/to/source.trupl`

   * Bazel: `bazel-bin/src/truc path/to/source.trupl`

* Execute unit tests:

   * GNU Make: `cd test/ && make all`

   * Bazel: `cd test/ && bazel test parser:all scanner:all tokens:all`

## Sample Input / Output

The following TruPL program computes the greatest common divisor of 2 integers.

```
program gcdfinder;
  a, b: int;

begin
  a := 28;
  b := 119;

  while a <> b loop begin
    if a < b then begin
      b := b - a;
    end else begin
      a := a - b;
    end;
  end;

  print a;
end;
```

Target assembly code generated for the above program.

```
_gcdfinder:
				move R0, #28
				move a, R0
				move R0, #119
				move b, R0
_while_cond0:
				move R0, a
				sub R0, b
				brez R0, _compare_false2
				move R0, #1
				brun _compare_done3
_compare_false2:
				move R0, #0
_compare_done3:
				brez R0, _while_done1
				move R0, a
				sub R0, b
				brez R0, _compare_false4
				brpo R0, _compare_false4
				move R0, #1
				brun _compare_done5
_compare_false4:
				move R0, #0
_compare_done5:
				brez R0, _else6
				move R0, b
				sub R0, a
				move b, R0
				brun _if_done7
_else6:
				move R0, a
				sub R0, b
				move a, R0
_if_done7:
				brun _while_cond0
_while_done1:
				move R0, a
				outb R0
				halt

a:				data 1
b:				data 1
```
