// Token class for punctuation symbols from TruPL.
// @author: Hieu Le
// @version: 09/28/2016

#ifndef PUNCTOKEN_H
#define PUNCTOKEN_H

#include "token.h"
#include <string>

// Attributes for punctuation tokens from TruPL.
typedef enum punc_attr { PUNC_SEMI = 200,
			 PUNC_COLON = 201,
			 PUNC_COMMA = 202,
			 PUNC_ASSIGN = 203,
			 PUNC_OPEN = 204,
			 PUNC_CLOSE = 205,
			 PUNC_NO_ATTR = 299 } punc_attr_type;

class PuncToken : public Token
{
 public:
  // Constructs an uninitialized punctuation token.
  PuncToken();

  // Constructs a punctuation token with an attribute.
  explicit PuncToken(punc_attr_type attr);

  ~PuncToken() override;

  // Returns the attribute of this token.
  punc_attr_type get_attribute() const;

  // Sets the attribute of this token.
  void set_attribute (punc_attr_type type);

  // Debug string will be of the form TOKEN_PUNC:<punc_attr_type>
  std::string *to_string() const override;

 private:
  punc_attr_type attribute_;
};

#endif
