// Token class for relation operators from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef RELOPTOKEN_H
#define RELOPTOKEN_H

#include <string>
#include "token.h"

using namespace std;

// Attributes for relational operators from TruPL.
typedef enum relop_attr { RELOP_EQ = 300,
			  RELOP_NE = 301,
			  RELOP_GT = 302,
			  RELOP_GE = 303,
			  RELOP_LT = 304,
			  RELOP_LE = 305,
			  RELOP_NO_ATTR = 399 } relop_attr_type;

class RelopToken : public Token
{
 public:
  // Constructs a token with RELOP_NO_ATTR as the default attribute.
  RelopToken();

  // Constructs a token from specified attribute.
  explicit RelopToken(relop_attr_type attr);

  ~RelopToken() override;

  // Returns the attribute of this token.
  relop_attr_type get_attribute() const;

  // Sets the attribute of this token to given value.
  void set_attribute(relop_attr_type attr);

  // Debug string will be of the form TOKEN_RELOP:<relop_attr_type>.
  string *to_string() const override;

 private:
  // The attribute of this relational operator token.
  relop_attr_type attribute_;
};

#endif
