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