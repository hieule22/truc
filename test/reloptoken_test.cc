// Unit tests for RelopToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/reloptoken.h"

#include <memory>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(RelopTokenTest, GetTokenType) {
  const RelopToken token(relop_attr_type::RELOP_NE);
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_RELOP);  
}

TEST(RelopTokenTest, GetAttribute) {
  const RelopToken default_token;
  EXPECT_EQ(default_token.get_attribute(), relop_attr_type::RELOP_NO_ATTR);
  const RelopToken initialized_token(relop_attr_type::RELOP_LT);
  EXPECT_EQ(initialized_token.get_attribute(), relop_attr_type::RELOP_LT);
  RelopToken changed_token;
  changed_token.set_attribute(relop_attr_type::RELOP_GT);
  EXPECT_EQ(changed_token.get_attribute(), relop_attr_type::RELOP_GT);
}

TEST(RelopTokenTest, SetAttribute) {
  RelopToken token;
  token.set_attribute(relop_attr_type::RELOP_GE);
  EXPECT_EQ(token.get_attribute(), relop_attr_type::RELOP_GE);
}

TEST(RelopTokenTest, ToString) {
  const std::string prefix = "TOKEN_RELOP:";
  const std::vector<std::pair<relop_attr_type, std::string>> attributes = {
    {relop_attr_type::RELOP_EQ, "RELOP_EQ"},
    {relop_attr_type::RELOP_NE, "RELOP_NE"},
    {relop_attr_type::RELOP_GT, "RELOP_GT"},
    {relop_attr_type::RELOP_GE, "RELOP_GE"},
    {relop_attr_type::RELOP_LT, "RELOP_LT"},
    {relop_attr_type::RELOP_LE, "RELOP_LE"},
    {relop_attr_type::RELOP_NO_ATTR, "RELOP_NO_ATTR"}
  };

  for (const auto& attribute : attributes) {
    const RelopToken token(attribute.first);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute.second);
  }
}

}  // namespace
