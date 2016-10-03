// Token class for additive operators from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef ADDOPTOKEN_H
#define ADDOPTOKEN_H

#include <string>

#include "token.h"

using namespace std;

// Attributes for additive operators from TruPL.
typedef enum addop_attr { ADDOP_ADD = 400,
			  ADDOP_SUB = 401,
			  ADDOP_OR = 402,
			  ADDOP_NO_ATTR = 499 } addop_attr_type;

class AddopToken : public Token {
 public:
  // Constructs an addop token with ADDOP_NO_ATTR as default attribute.
  AddopToken();

  // Constructs an addop token from a specified attribute.
  explicit AddopToken(addop_attr_type attr);

  ~AddopToken() override;

  // Returns the attribute of this addop token.
  addop_attr_type get_attribute() const;

  // Sets the attribute of this addop token to a specified value.
  void set_attribute(addop_attr_type attr);

  // Output will be of the form TOKEN_ADDOP:<addop_attr_type>.
  string *to_string() const override;

 private:
  // The attribute of this addop token.
  addop_attr_type attribute_;
};

#endif
