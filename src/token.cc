// Implementation of KeywordToken class.
// @author Hieu Le
// @version 09/28/2016

#include "token.h"

Token::Token() : type_(TOKEN_NO_TYPE) {}

Token::~Token() {}

void Token::set_token_type (const token_type_type type) {
  type_ = type;
}

token_type_type Token::get_token_type() const
{
  return type_;
}
