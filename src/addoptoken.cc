// Implementation for AddopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "addoptoken.h"

AddopToken::AddopToken(const addop_attr_type attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_ADDOP);
}

AddopToken::AddopToken()
    : AddopToken(addop_attr_type::ADDOP_NO_ATTR) {}

AddopToken::~AddopToken() {}

addop_attr_type AddopToken::get_attribute() const {
  return attribute_;
}

void AddopToken::set_attribute(const addop_attr_type attr) {
  attribute_ = attr;
}

std::string *AddopToken::to_string() const {
  switch (attribute_) {
    case addop_attr_type::ADDOP_ADD:
      return new std::string("TOKEN_ADDOP:ADDOP_ADD");

    case addop_attr_type::ADDOP_SUB:
      return new std::string("TOKEN_ADDOP:ADDOP_SUB");

    case addop_attr_type::ADDOP_OR:
      return new std::string("TOKEN_ADDOP:ADDOP_OR");

    case addop_attr_type::ADDOP_NO_ATTR:
      return new std::string("TOKEN_ADDOP:ADDOP_NO_ATTR");

    default:
      return nullptr;
  }
}
