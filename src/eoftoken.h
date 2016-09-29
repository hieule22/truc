// Token class for the EndOfFile symbol.
// @author Hieu Le
// @version 09/28/2016

#ifndef TOKENEOF_H
#define TOKENEOF_H

#include "token.h"

using namespace std;

// The EofToken has no attribute.

class EofToken : public Token
{
 public:
  EofToken();
  
  ~EofToken() override;

  // Debug string will be of the form TOKEN_EOF:EOF.
  string *to_string() const override;
};

#endif

  
