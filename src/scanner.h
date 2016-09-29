#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <memory>
#include <stdlib.h>

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

class Scanner
{
 public:
  // Constructs a Scanner for a given file.
  explicit Scanner(char *filename);

  // Constructs a Scanner from a given buffer. Intended for testing.
  explicit Scanner(std::unique_ptr<Buffer> buffer);

  // Return the next token in this file.
  // Return value is not owned by this object.
  Token *next_token();

 private:
  inline bool is_alpha (char c) const 
  {
    return c >= 'a' && c <= 'z';
  }

  inline bool is_digit (char c) const
  {
    return c >= '0' && c <= '9';
  }

  inline bool is_alphanum (char c) const
  {
    return is_alpha(c) || is_digit(c);
  }

  inline bool is_space(char c) const
  {
    return c == SPACE;
  }

  /* If a lexical error OR an internal scanner error occurs,
     call this method.  It will print the mesg and exit. */
  void scanner_fatal_error(const std::string& mesg) const;

  // The character buffer.
  std::unique_ptr<Buffer> buffer_;
};
#endif
  
