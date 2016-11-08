# CS420: Compiler

## About

This repository contains the basic implementation of a compiler for the
programming language TruPL. As the adopted C++ syntax is somewhat archaic,
check out [trupl-compiler](https://github.com/hieule22/trupl-compiler) for
a more updated version of the same compiler.

## Installation

The source package can be built using [Bazel](http://bazel.io) or GNU-compatible
Make.

* Obtain a local clone of the repository.

```bash
git clone https://github.com/hieule22/cs420.git
```

* Build the executable binary.

```bash
cd cs420/src
make truc
```

* Supply the compiler with a full path to the targeted source file.

```bash
./truc path/to/source.trupl
```