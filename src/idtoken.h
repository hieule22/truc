// Token class for identifiers from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef IDTOKEN_H
#define IDTOKEN_H

#include "token.h"
#include <string>

class IdToken : public Token
{
 public:
  // Default constructor. Identifier name is initialized to an empty string.
  IdToken();

  // Constructs an identifier token from an input string representing the
  // identifier name.
  IdToken(const std::string& attr);
  
  ~IdToken() override;

  // Returns a string representing this identifier token.
  // The return value is not own by this instance.
  std::string *get_attribute() const;

  void set_attribute(const std::string& attr);

  // Debug string will be of the form TOKEN_ID:<identifier name>
  std::string *to_string() const override;

 private:
  // The string representing this identifier name.
  std::string attribute_;
};

#endif
