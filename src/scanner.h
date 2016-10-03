// Scanner class to extract valid TruPL lexemes from a source file.
// @author Hieu Le
// @version September 30th, 2016

#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>

#include <iostream>
#include <string>

// The scanner reads from the buffer.
#include "buffer.h"

// The scanner returns objects from the Token class when
// next_token() is called.
#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"

using namespace std;

class Scanner {
 public:
  // Constructs a Scanner for a given file.
  explicit Scanner(char *filename);

  // Constructs a Scanner from a given buffer. Useful for testing.
  // Ownership of the buffer is acquired by this object.
  explicit Scanner(Buffer *buffer);

  ~Scanner();

  // Return the next token in this file.
  // Returned value becomes property of the caller.
  Token *next_token();

 private:
  // Checks if c represents an alphabetic character.
  inline bool is_alpha(const char c) const {
    return c >= 'a' && c <= 'z';
  }

  // Checks if c represents a digit.
  inline bool is_digit(const char c) const {
    return c >= '0' && c <= '9';
  }

  // Checks if c represents an alphabetic character or a digit.
  inline bool is_alphanum(const char c) const {
    return is_alpha(c) || is_digit(c);
  }

  // Checks if c represents a space.
  inline bool is_space(const char c) const {
    return c == SPACE;
  }

  // If a lexical error OR an internal scanner error occurs,
  // call this method. It will print the mesg and exit.
  void scanner_fatal_error(const std::string& mesg) const;

  // The character buffer.
  Buffer* buffer_;
};

#endif
