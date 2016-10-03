// Implementation of KeywordToken class.
// @author: Hieu Le
// @version: 09/28/2016

#include "keywordtoken.h"

KeywordToken::KeywordToken(const keyword_attr_type attr) : attribute_(attr) {
  Token::set_token_type(TOKEN_KEYWORD);
}

KeywordToken::KeywordToken()
  : KeywordToken(keyword_attr_type::KW_NO_ATTR) {}

KeywordToken::~KeywordToken() {}

keyword_attr_type KeywordToken::get_attribute() const {
  return attribute_;
}

void KeywordToken::set_attribute(const keyword_attr_type type) {
  attribute_ = type;
}

string *KeywordToken::to_string() const {
  const string prefix = "TOKEN_KEYWORD:";
  switch (attribute_) {
    case keyword_attr_type::KW_PROGRAM:
      return new string(prefix + "KW_PROGRAM");

    case keyword_attr_type::KW_PROCEDURE:
      return new string(prefix + "KW_PROCEDURE");

    case keyword_attr_type::KW_INT:
      return new string(prefix + "KW_INT");

    case keyword_attr_type::KW_BOOL:
      return new string(prefix + "KW_BOOL");

    case keyword_attr_type::KW_BEGIN:
      return new string(prefix + "KW_BEGIN");

    case keyword_attr_type::KW_END:
      return new string(prefix + "KW_END");

    case keyword_attr_type::KW_IF:
      return new string(prefix + "KW_IF");

    case keyword_attr_type::KW_THEN:
      return new string(prefix + "KW_THEN");

    case keyword_attr_type::KW_ELSE:
      return new string(prefix + "KW_ELSE");

    case keyword_attr_type::KW_WHILE:
      return new string(prefix + "KW_WHILE");

    case keyword_attr_type::KW_LOOP:
      return new string(prefix + "KW_LOOP");

    case keyword_attr_type::KW_PRINT:
      return new string(prefix + "KW_PRINT");

    case keyword_attr_type::KW_NOT:
      return new string(prefix + "KW_NOT");

    case keyword_attr_type::KW_NO_ATTR:
      return new string(prefix + "KW_NO_ATTR");

    default:
      return nullptr;
  }
}
