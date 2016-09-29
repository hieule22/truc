// Token class for numbers from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef NUMTOKEN_H
#define NUMTOKEN_H

#include "token.h"
#include <string>

using namespace std;

class NumToken : public Token
{
 public:
  // Default constructor. The numerical value is initialized to "0".
  NumToken();

  // Constructs a number token from an input string representing the numerical
  // value of that number.
  explicit NumToken(const string& attr);
  
  ~NumToken() override;

  // Returns a string representing the value of this token.
  // Return value is not owned by this object.
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
