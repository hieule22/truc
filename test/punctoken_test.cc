// Unit tests for PuncToken class.
// @author: Hieu Le
// @version: 09/28/2016

#include "src/punctoken.h"

#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(PuncTokenTest, GetTokenType) {
  const PuncToken token(punc_attr_type::PUNC_CLOSE);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_PUNC);
  const PuncToken default_token;
  EXPECT_EQ(default_token.get_token_type(), token_type_type::TOKEN_PUNC);
}

TEST(PuncTokenTest, GetAttribute) {
  const PuncToken default_token;
  EXPECT_EQ(default_token.get_attribute(), punc_attr_type::PUNC_NO_ATTR);
  const PuncToken initialized_token(punc_attr_type::PUNC_COLON);
  EXPECT_EQ(initialized_token.get_attribute(), punc_attr_type::PUNC_COLON);
  PuncToken changed_token;
  changed_token.set_attribute(punc_attr_type::PUNC_ASSIGN);
  EXPECT_EQ(changed_token.get_attribute(), punc_attr_type::PUNC_ASSIGN);
}

TEST(PuncTokenTest, SetAttribute) {
  PuncToken token;
  token.set_attribute(punc_attr_type::PUNC_COMMA);
  EXPECT_EQ(token.get_attribute(), punc_attr_type::PUNC_COMMA);
}

TEST(PuncTokenTest, ToString) {
  const std::string prefix = "TOKEN_PUNC:";
  const std::vector<std::pair<punc_attr_type, std::string>> attributes = {
    {punc_attr_type::PUNC_SEMI, "PUNC_SEMI"},
    {punc_attr_type::PUNC_COLON, "PUNC_COLON"},
    {punc_attr_type::PUNC_COMMA, "PUNC_COMMA"},
    {punc_attr_type::PUNC_ASSIGN, "PUNC_ASSIGN"},
    {punc_attr_type::PUNC_OPEN, "PUNC_OPEN"},
    {punc_attr_type::PUNC_CLOSE, "PUNC_CLOSE"},
    {punc_attr_type::PUNC_NO_ATTR, "PUNC_NO_ATTR"}
  };

  for (const auto& attribute : attributes) {
    PuncToken token(attribute.first);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute.second);
  }
}

}  // namespace
