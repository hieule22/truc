// Implementation for IdToken class.
// @author Hieu Le
// @version 09/28/2016

#include "idtoken.h"

IdToken::IdToken(const string& attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_ID);
}

IdToken::IdToken() : IdToken("") {}

IdToken::~IdToken() {}

string *IdToken::get_attribute() const {
  return new string(attribute_);
}

void IdToken::set_attribute(const string& attr) {
  attribute_ = attr;
}

string *IdToken::to_string() const {
  return new string("TOKEN_ID:" + attribute_);
}
