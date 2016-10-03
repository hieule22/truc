#!/bin/bash
# Script to package source file.

# Current directory.
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET="src"
SRC="$DIR/../$TARGET"

PACKAGE_NAME="le_h_init.tar"

# Change directory to source directory.
cd $DIR/..

# Package all relevant files.
tar c --exclude="BUILD" --exclude="*.o" \
    --exclude="test_scanner" -vf $PACKAGE_NAME $TARGET

# Move created package to output.
mv $PACKAGE_NAME $DIR
