// Token class for numbers from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef NUMTOKEN_H
#define NUMTOKEN_H

#include "token.h"
#include <string>

class NumToken : public Token
{
 public:
  // Default constructor. The numerical value is initialized to "0".
  NumToken();

  // Constructs a number token from an input string representing the numerical
  // value of that number.
  explicit NumToken(const std::string& attr);
  
  ~NumToken() override;

  // Returns a string representing the value of this token.
  // Return value is not owned by this instance.
  std::string *get_attribute() const;

  void set_attribute(const std::string& attr);

  // Debug string will be of the form TOKEN_NUM:<value>
  std::string *to_string() const override;

 private:
  std::string attribute_;
};

#endif
