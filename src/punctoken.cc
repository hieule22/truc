// Implementation for PuncToken class.
// @author Hieu Le
// @version 09/28/2016

#include "punctoken.h"

PuncToken::PuncToken(const punc_attr_type attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_PUNC);
}

PuncToken::PuncToken()
    : PuncToken(punc_attr_type::PUNC_NO_ATTR) {}

PuncToken::~PuncToken() {}

punc_attr_type PuncToken::get_attribute() const {
  return attribute_;
}

void PuncToken::set_attribute(const punc_attr_type attr) {
  attribute_ = attr;
}

string *PuncToken::to_string() const {
  switch (attribute_) {
    case punc_attr_type::PUNC_SEMI:
      return new string("TOKEN_PUNC:PUNC_SEMI");

    case punc_attr_type::PUNC_COLON:
      return new string("TOKEN_PUNC:PUNC_COLON");

    case punc_attr_type::PUNC_COMMA:
      return new string("TOKEN_PUNC:PUNC_COMMA");

    case punc_attr_type::PUNC_ASSIGN:
      return new string("TOKEN_PUNC:PUNC_ASSIGN");

    case punc_attr_type::PUNC_OPEN:
      return new string("TOKEN_PUNC:PUNC_OPEN");

    case punc_attr_type::PUNC_CLOSE:
      return new string("TOKEN_PUNC:PUNC_CLOSE");

    case punc_attr_type::PUNC_NO_ATTR:
      return new string("TOKEN_PUNC:PUNC_NO_ATTR");

    default:
        return nullptr;
  }
}


