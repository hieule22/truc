// Implementation of NumToken class.
// @author Hieu Le
// @version 09/28/2016

#include "numtoken.h"

NumToken::NumToken(const string& attr) : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_NUM);
}

NumToken::NumToken() : NumToken("0") {}

NumToken::~NumToken() {}

string *NumToken::get_attribute() const {
  return new string(attribute_);
}

void NumToken::set_attribute(const string& attr) {
  attribute_ = attr;
}

string *NumToken::to_string() const {
  return new string("TOKEN_NUM:" + attribute_);
}
