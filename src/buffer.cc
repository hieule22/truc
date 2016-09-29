// Implementation of Buffer interface.
// @author Hieu Le
// @version 09/28/2016

#include "buffer.h"

#include <cctype>
#include <cstdlib>

#include <algorithm>
#include <iostream>

namespace {

const char kNonAlphanum[] =
{';', ':', '(', ')', ',', '=', '>', '<', '+', '-', '*', '/',
 COMMENT_MARKER, SPACE, TAB, NEW_LINE};

template <typename Container, typename T>
bool contains(const Container& container, const T& value) {
  return std::find(std::begin(container), std::end(container), value)
      != std::end(container);
}

}  // namespace

Buffer::~Buffer() {}

void Buffer::buffer_fatal_error() const
{
  std::cerr << "Exiting on BUFFER FATAL ERROR" << std::endl;
  std::exit(EXIT_FAILURE);
}

bool Buffer::validate(const char c) const {
  return std::islower(c) || std::isdigit(c) || contains(kNonAlphanum, c);
}
