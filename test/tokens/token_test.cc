// Unit tests for Token base class.
// @author Hieu Le
// @version 09/28/2016

#include "src/token.h"

#include "gtest/gtest.h"

namespace {

TEST(TokenTest, DefaultConstructor) {
  const Token token;
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_NO_TYPE);
}

TEST(TokenTest, GetTokenType) {
  Token token;
  token.set_token_type(token_type_type::TOKEN_KEYWORD);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_KEYWORD);
}

TEST(TokenTest, ToString) {
  const Token token;
  EXPECT_EQ(token.to_string(), nullptr);
}

}  // namespace
