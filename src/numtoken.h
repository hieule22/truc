// Token class for numbers from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef NUMTOKEN_H
#define NUMTOKEN_H

#include <string>

#include "token.h"

using namespace std;

class NumToken : public Token {
 public:
  // Constructs a number token with "0" as default attribute.
  NumToken();

  // Constructs a number token from a specified attribute.
  explicit NumToken(const string& attr);

  ~NumToken() override;

  // Returns the attribute of this number token.
  // Returned value becomes property of the caller.
  string *get_attribute() const;

  // Set the attribute of this number token to a specified value.
  void set_attribute(const string& attr);

  // Debug string will be of the form TOKEN_NUM:<value>.
  string *to_string() const override;

 private:
  // The attribute of this number token.
  string attribute_;
};

#endif
