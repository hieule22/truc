// Implementation of Buffer interface.
// @author Hieu Le
// @version 09/28/2016

#include "buffer.h"

#include <cctype>
#include <cstdlib>

#include <algorithm>
#include <iostream>

using namespace std;

namespace {

const char kNonAlphanum[] =
{';', ':', '(', ')', ',', '=', '>', '<', '+', '-', '*', '/',
 COMMENT_MARKER, SPACE, TAB, NEW_LINE};

template <typename Container, typename T>
bool contains(const Container& container, const T& value) {
  return find(begin(container), end(container), value) != end(container);
}

}  // namespace

Buffer::~Buffer() {}

void Buffer::buffer_fatal_error() const
{
  cerr << "Exiting on BUFFER FATAL ERROR" << endl;
  exit(EXIT_FAILURE);
}

bool Buffer::validate(const char c) const {
  return islower(c) || isdigit(c) || contains(kNonAlphanum, c);
}
