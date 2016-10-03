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
  // Default constructor initializes attribute to "0".
  NumToken();

  // Constructs a number token from an input string representing the numerical
  // value of that number.
  explicit NumToken(const string& attr);

  ~NumToken() override;

  // Returns a string representing the value of this token.
  // Returned value becomes property of the caller.
  string *get_attribute() const;

  // Set the attribute of this token to specified value.
  void set_attribute(const string& attr);

  // Debug string will be of the form TOKEN_NUM:<value>.
  string *to_string() const override;

 private:
  // The string representing the numerical value of this number token.
  string attribute_;
};

#endif
