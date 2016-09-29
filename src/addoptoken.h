// Token class for additive operators from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef ADDOPTOKEN_H
#define ADDOPTOKEN_H

#include "token.h"
#include <string>

// Attributes for additive operators from TruPL.
typedef enum addop_attr { ADDOP_ADD = 400,
			  ADDOP_SUB = 401,
			  ADDOP_OR = 402,
			  ADDOP_NO_ATTR = 499 } addop_attr_type;

class AddopToken : public Token
{
 public:
  AddopToken();
  
  explicit AddopToken(addop_attr_type attr);
  
  ~AddopToken() override;

  addop_attr_type get_attribute() const;
  
  void set_attribute (addop_attr_type attr);

  // Output will be of the form TOKEN_ADDOP:<addop_attr_type>.
  std::string *to_string() const override;

 private:
  addop_attr_type attribute_;
};

#endif
