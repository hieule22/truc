// Unit tests for KeywordToken class.
// @author: Hieu Le
// @version: 09/28/2016

#include "src/keywordtoken.h"

#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(KeywordTokenTest, GetTokenType) {
  const KeywordToken token(keyword_attr_type::KW_WHILE);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_KEYWORD);
  const KeywordToken default_token;
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_KEYWORD);
}

TEST(KeywordTokenTest, GetAttribute) {
  const KeywordToken default_token;
  EXPECT_EQ(default_token.get_attribute(), keyword_attr_type::KW_NO_ATTR);
  const KeywordToken initialized_token(keyword_attr_type::KW_PRINT);
  EXPECT_EQ(initialized_token.get_attribute(), keyword_attr_type::KW_PRINT);
  KeywordToken changed_token;
  changed_token.set_attribute(keyword_attr_type::KW_IF);
  EXPECT_EQ(changed_token.get_attribute(), keyword_attr_type::KW_IF);
}

TEST(KeywordTokenTest, ToString) {
  const std::string prefix = "TOKEN_KEYWORD:";
  const std::vector<std::pair<keyword_attr_type, std::string>> attributes = {
    {keyword_attr_type::KW_PROGRAM, "KW_PROGRAM"},
    {keyword_attr_type::KW_PROCEDURE, "KW_PROCEDURE"},
    {keyword_attr_type::KW_INT, "KW_INT"},
    {keyword_attr_type::KW_BOOL, "KW_BOOL"},
    {keyword_attr_type::KW_BEGIN, "KW_BEGIN"},
    {keyword_attr_type::KW_END, "KW_END"},
    {keyword_attr_type::KW_IF, "KW_IF"},
    {keyword_attr_type::KW_THEN, "KW_THEN"},
    {keyword_attr_type::KW_ELSE, "KW_ELSE"},
    {keyword_attr_type::KW_WHILE, "KW_WHILE"},
    {keyword_attr_type::KW_LOOP, "KW_LOOP"},
    {keyword_attr_type::KW_PRINT, "KW_PRINT"},
    {keyword_attr_type::KW_NOT, "KW_NOT"},
    {keyword_attr_type::KW_NO_ATTR, "KW_NO_ATTR"}
  };
  
  for (const auto& attribute : attributes) {
    KeywordToken token(attribute.first);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute.second);
  }
}

}  // namespace
