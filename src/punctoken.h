// Token class for punctuation symbols from TruPL.
// @author Hieu Le
// @version 09/28/2016

#ifndef PUNCTOKEN_H
#define PUNCTOKEN_H

#include <string>

#include "token.h"

using namespace std;

// Attributes for punctuation tokens from TruPL.
typedef enum punc_attr { PUNC_SEMI = 200,
			 PUNC_COLON = 201,
			 PUNC_COMMA = 202,
			 PUNC_ASSIGN = 203,
			 PUNC_OPEN = 204,
			 PUNC_CLOSE = 205,
			 PUNC_NO_ATTR = 299 } punc_attr_type;

class PuncToken : public Token {
 public:
  // Default constructor initializes attribute to PUNC_NO_ATTR.
  PuncToken();

  // Constructs a punctuation token from a given attribute.
  explicit PuncToken(punc_attr_type attr);

  ~PuncToken() override;

  // Returns the attribute of this token.
  punc_attr_type get_attribute() const;

  // Sets the attribute of this token.
  void set_attribute(punc_attr_type attr);

  // Debug string will be of the form TOKEN_PUNC:<punc_attr_type>.
  string *to_string() const override;

 private:
  // The attribute of this punctuation token.
  punc_attr_type attribute_;
};

#endif
