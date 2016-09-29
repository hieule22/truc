// Implementation for MulopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "muloptoken.h"

MulopToken::MulopToken(const mulop_attr_type attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_MULOP);
}

MulopToken::MulopToken()
    : MulopToken(MULOP_NO_ATTR) {}

MulopToken::~MulopToken() {}

mulop_attr_type MulopToken::get_attribute() const {
  return attribute_;
}

void MulopToken::set_attribute(const mulop_attr_type attr) {
  attribute_ = attr;
}

std::string *MulopToken::to_string() const {
  switch (attribute_) {
    case mulop_attr_type::MULOP_MUL:
      return new std::string("TOKEN_MULOP:MULOP_MUL");

    case mulop_attr_type::MULOP_DIV:
      return new std::string("TOKEN_MULOP:MULOP_DIV");

    case mulop_attr_type::MULOP_AND:
      return new std::string("TOKEN_MULOP:MULOP_AND");

    case mulop_attr_type::MULOP_NO_ATTR:
      return new std::string("TOKEN_MULOP:MULOP_NO_ATTR");

    default:
      return nullptr;
  }
}
