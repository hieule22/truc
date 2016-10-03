// Unit tests for AddopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/addoptoken.h"

#include <memory>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(AddopTokenTest, GetTokenType) {
  const AddopToken token(addop_attr_type::ADDOP_OR);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_ADDOP);
  const AddopToken default_token;
  EXPECT_EQ(default_token.get_token_type(), token_type_type::TOKEN_ADDOP);
}

TEST(AddopTokenTest, GetAttribute) {
  const AddopToken default_token;
  EXPECT_EQ(default_token.get_attribute(), addop_attr_type::ADDOP_NO_ATTR);
  const AddopToken initialized_token(addop_attr_type::ADDOP_ADD);
  EXPECT_EQ(initialized_token.get_attribute(), addop_attr_type::ADDOP_ADD);
  AddopToken changed_token;
  changed_token.set_attribute(addop_attr_type::ADDOP_SUB);
  EXPECT_EQ(changed_token.get_attribute(), addop_attr_type::ADDOP_SUB);
}

TEST(AddopTokenTest, SetAttribute) {
  AddopToken token;
  token.set_attribute(addop_attr_type::ADDOP_OR);
  EXPECT_EQ(token.get_attribute(), addop_attr_type::ADDOP_OR);
}

TEST(AddopTokenTest, ToString) {
  const std::string prefix = "TOKEN_ADDOP:";
  const std::vector<std::pair<addop_attr_type, std::string>> attributes = {
    {addop_attr_type::ADDOP_ADD, "ADDOP_ADD"},
    {addop_attr_type::ADDOP_SUB, "ADDOP_SUB"},
    {addop_attr_type::ADDOP_OR, "ADDOP_OR"},
    {addop_attr_type::ADDOP_NO_ATTR, "ADDOP_NO_ATTR"}
  };

  for (const auto& attribute : attributes) {
    const AddopToken token(attribute.first);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute.second);
  }
}

}  // namespace
