// Implementation of RelopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "reloptoken.h"

RelopToken::RelopToken(const relop_attr_type attr) : attribute_(attr) {
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

string *RelopToken::to_string() const {
  const string prefix = "TOKEN_RELOP:";
  switch (attribute_) {
    case relop_attr_type::RELOP_EQ:
      return new string(prefix + "RELOP_EQ");

    case relop_attr_type::RELOP_NE:
      return new string(prefix + "RELOP_NE");

    case relop_attr_type::RELOP_GT:
      return new string(prefix + "RELOP_GT");

    case relop_attr_type::RELOP_GE:
      return new string(prefix + "RELOP_GE");

    case relop_attr_type::RELOP_LT:
      return new string(prefix + "RELOP_LT");

    case relop_attr_type::RELOP_LE:
      return new string(prefix + "RELOP_LE");

    case relop_attr_type::RELOP_NO_ATTR:
      return new string(prefix + "RELOP_NO_ATTR");

    default:
      return nullptr;
  }
}
