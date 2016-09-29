// Implementation for EofToken class.
// @author Hieu Le
// @version 09/28/2016

#include "eoftoken.h"

EofToken::EofToken() {
  Token::set_token_type(token_type_type::TOKEN_EOF);
}

EofToken::~EofToken() {}

std::string *EofToken::to_string() const {
  return new std::string("TOKEN_EOF:EOF");
}
