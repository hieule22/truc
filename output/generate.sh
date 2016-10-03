#!/bin/bash
# Script to package source file.

# Current directory.
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET="src"
SRC="$DIR/../$TARGET"
PACKAGE_NAME="le_h_init.tar"

# Command line args.
ARG=$1

if [ $# -eq 1 ] && [ $ARG == "bazel" ]; then
   # Build all targets.
   cd $DIR/../src/
   bazel build :all

   # Run all tests
   cd $DIR/../test/
   bazel test :all
fi

# Package all relevant files.
cd $DIR/..
tar c --exclude="*~" --exclude="*.o" --exclude="test_scanner" \
    --exclude="BUILD" -vf $PACKAGE_NAME $TARGET

# Move created package to output.
mv $PACKAGE_NAME $DIR
