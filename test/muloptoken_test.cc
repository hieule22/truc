// Unit tests for MulopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/muloptoken.h"

#include <memory>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(MulopTokenTest, GetTokenType) {
  const MulopToken token(mulop_attr_type::MULOP_MUL);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_MULOP);
}

TEST(MulopTokenTest, GetAttribute) {
  const MulopToken default_token;
  EXPECT_EQ(default_token.get_attribute(), mulop_attr_type::MULOP_NO_ATTR);
  const MulopToken initialized_token(mulop_attr_type::MULOP_DIV);
  EXPECT_EQ(initialized_token.get_attribute(), mulop_attr_type::MULOP_DIV);
  MulopToken changed_token;
  changed_token.set_attribute(mulop_attr_type::MULOP_AND);
  EXPECT_EQ(changed_token.get_attribute(), mulop_attr_type::MULOP_AND);
}

TEST(MulopTokenTest, SetAttribute) {
  MulopToken token;
  token.set_attribute(mulop_attr_type::MULOP_MUL);
  EXPECT_EQ(token.get_attribute(), mulop_attr_type::MULOP_MUL);
}

TEST(MulopTokenTest, ToString) {
  const std::string prefix = "TOKEN_MULOP:";
  const std::vector<std::pair<mulop_attr_type, std::string>> attributes = {
    {mulop_attr_type::MULOP_MUL, "MULOP_MUL"},
    {mulop_attr_type::MULOP_DIV, "MULOP_DIV"},
    {mulop_attr_type::MULOP_AND, "MULOP_AND"},
    {mulop_attr_type::MULOP_NO_ATTR, "MULOP_NO_ATTR"}
  };

  for (const auto& attribute : attributes) {
    const MulopToken token(attribute.first);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute.second);
  }
}

}  // namespace
