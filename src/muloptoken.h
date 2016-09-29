// Token class for multiplicative operators from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef MULOPTOKEN_H
#define MULOPTOKEN_H

#include "token.h"
#include <string>

// Attributes for multiplicative operators from TruPL.
typedef enum mulop_attr { MULOP_MUL = 500,
			  MULOP_DIV = 501,
			  MULOP_AND = 502,
			  MULOP_NO_ATTR = 599 } mulop_attr_type;

class MulopToken : public Token {
 public:
  MulopToken();
  
  MulopToken(mulop_attr_type attr);
  
  ~MulopToken() override;

  mulop_attr_type get_attribute () const;
  
  void set_attribute (mulop_attr_type attr);

  // Debug string will be of the form TOKEN_MULOP:<mulop_attr_type>.
  std::string *to_string() const override;

 private:
  mulop_attr_type attribute_;
};

#endif
