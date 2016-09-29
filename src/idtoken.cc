// Implementation for IdToken class.
// @author Hieu Le
// @version 09/28/2016

#include "idtoken.h"

IdToken::IdToken(const std::string& attr)
    : attribute_(attr) {
  Token::set_token_type(token_type_type::TOKEN_ID);
}

IdToken::IdToken() : IdToken("") {}

IdToken::~IdToken() {}

std::string *IdToken::get_attribute() const {
  return new std::string(attribute_);
}

void IdToken::set_attribute(const std::string& attr) {
  attribute_ = attr;
}

std::string *IdToken::to_string() const {
  return new std::string("TOKEN_ID:" + attribute_);
}
