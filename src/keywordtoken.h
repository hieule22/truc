// Token class for keywords from TruPL.
// @author: Hieu Le
// @version: 09/28/2016

#ifndef KEYWORDTOKEN_H
#define KEYWORDTOKEN_H

#include "token.h"
#include <string>

// Attributes for keyword tokens from TruPL.
typedef enum keyword_attr { KW_PROGRAM  = 100, 
			    KW_PROCEDURE = 101,
			    KW_INT      = 102, 
			    KW_BOOL     = 103,
			    KW_BEGIN    = 104, 
			    KW_END      = 105,
			    KW_IF       = 106, 
			    KW_THEN     = 107,
			    KW_ELSE     = 108, 
			    KW_WHILE    = 109,
			    KW_LOOP     = 110,
			    KW_PRINT    = 111, 
			    KW_NOT      = 112,
			    KW_NO_ATTR  = 199 } keyword_attr_type;

class KeywordToken : public Token
{
 public:
  // Constructs an uninitialized keyword token.
  KeywordToken();

  // Constructs a keywork token with an attribute.
  explicit KeywordToken(keyword_attr_type attr);

  ~KeywordToken() override;

  // Returns the attribute of this token.
  keyword_attr_type get_attribute() const;

  // Sets the attribute of this token.
  void set_attribute(keyword_attr_type attr);

  // Debug string of the form TOKEN_KEYWORD:<keyword_attr_type>
  std::string *to_string() const override;

 private:
  // This token's attribute.
  keyword_attr_type attribute_;
};

#endif
