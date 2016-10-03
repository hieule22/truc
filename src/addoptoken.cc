// Implementation of AddopToken class.
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

string *AddopToken::to_string() const {
  const string prefix = "TOKEN_ADDOP:";
  switch (attribute_) {
    case addop_attr_type::ADDOP_ADD:
      return new string(prefix + "ADDOP_ADD");

    case addop_attr_type::ADDOP_SUB:
      return new string(prefix + "ADDOP_SUB");

    case addop_attr_type::ADDOP_OR:
      return new string(prefix + "ADDOP_OR");

    case addop_attr_type::ADDOP_NO_ATTR:
      return new string(prefix + "ADDOP_NO_ATTR");

    default:
      return nullptr;
  }
}
