// Implementation for RelopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "reloptoken.h"

RelopToken::RelopToken(const relop_attr_type attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_RELOP);
}

RelopToken::RelopToken()
    : RelopToken(relop_attr_type::RELOP_NO_ATTR) {}

RelopToken::~RelopToken() {}

relop_attr_type RelopToken::get_attribute() const {
  return attribute_;
}

void RelopToken::set_attribute(const relop_attr_type attr) {
  attribute_ = attr;
}

std::string *RelopToken::to_string() const {
  switch (attribute_) {
    case relop_attr_type::RELOP_EQ:
      return new std::string("TOKEN_RELOP:RELOP_EQ");

    case relop_attr_type::RELOP_NE:
      return new std::string("TOKEN_RELOP:RELOP_NE");

    case relop_attr_type::RELOP_GT:
      return new std::string("TOKEN_RELOP:RELOP_GT");

    case relop_attr_type::RELOP_GE:
      return new std::string("TOKEN_RELOP:RELOP_GE");

    case relop_attr_type::RELOP_LT:
      return new std::string("TOKEN_RELOP:RELOP_LT");

    case relop_attr_type::RELOP_LE:
      return new std::string("TOKEN_RELOP:RELOP_LE");

    case relop_attr_type::RELOP_NO_ATTR:
      return new std::string("TOKEN_RELOP:RELOP_NO_ATTR");

    default:
      return nullptr;
  }
}
