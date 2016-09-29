// Token class for identifiers from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef IDTOKEN_H
#define IDTOKEN_H

#include "token.h"
#include <string>

using namespace std;

class IdToken : public Token
{
 public:
  // Default constructor. Identifier name is initialized to an empty string.
  IdToken();

  // Constructs an identifier token from an input string representing the
  // identifier name.
  explicit IdToken(const string& attr);
  
  ~IdToken() override;

  // Returns a string representing this identifier token.
  // The return value is not owned by this object.
  std::string *get_attribute() const;

  // Sets the name of this identifier to specified value.
  void set_attribute(const string& attr);

  // Debug string will be of the form TOKEN_ID:<identifier name>
  string *to_string() const override;

 private:
  // The string representing this identifier name.
  string attribute_;
};

#endif
