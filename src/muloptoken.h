// Token class for multiplicative operators from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef MULOPTOKEN_H
#define MULOPTOKEN_H

#include "token.h"
#include <string>

using namespace std;

// Attributes for multiplicative operators from TruPL.
typedef enum mulop_attr { MULOP_MUL = 500,
			  MULOP_DIV = 501,
			  MULOP_AND = 502,
			  MULOP_NO_ATTR = 599 } mulop_attr_type;

class MulopToken : public Token {
 public:
  // Constructs a token with MULOP_NO_ATTR as the default attribute.
  MulopToken();
  
  // Constructs a token from a given attribute.
  explicit MulopToken(mulop_attr_type attr);
  
  ~MulopToken() override;

  // Returns the attribute of this token.
  mulop_attr_type get_attribute () const;
  
  // Sets the attribute of this token to given value.
  void set_attribute (mulop_attr_type attr);

  // Debug string will be of the form TOKEN_MULOP:<mulop_attr_type>.
  string *to_string() const override;

 private:
  // The attribute of this multiplicative operator token.
  mulop_attr_type attribute_;
};

#endif
