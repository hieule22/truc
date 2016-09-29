// Unit tests for NumToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/numtoken.h"

#include <memory>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(NumTokenTest, GetTokenType) {
  const NumToken token("12345");
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_NUM);
}

TEST(NumTokenTest, GetAttribute) {
  {
    const NumToken default_token;
    std::unique_ptr<std::string> attribute(default_token.get_attribute());
    EXPECT_EQ(*attribute, "0");
  }
  {
    const NumToken initialized_token("2980");
    std::unique_ptr<std::string> attribute(initialized_token.get_attribute());
    EXPECT_EQ(*attribute, "2980");
  }
  {
    NumToken changed_token;
    changed_token.set_attribute("123");
    std::unique_ptr<std::string> attribute(changed_token.get_attribute());
    EXPECT_EQ(*attribute, "123");
  }
}

TEST(NumTokenTest, SetAttribute) {
  NumToken token;
  token.set_attribute("1000");
  std::unique_ptr<std::string> attribute(token.get_attribute());
  EXPECT_EQ(*attribute, "1000");
}

TEST(NumTokenTest, ToString) {
  const std::string prefix = "TOKEN_NUM:";
  const std::vector<int> numbers = {0, 1, 17, 11, 9999, 1065};
  for (const int& number : numbers) {
    const std::string attribute = std::to_string(number);
    const NumToken token(attribute);
    std::unique_ptr<std::string> debug_string(token.to_string());
    EXPECT_EQ(*debug_string, prefix + attribute);
  }
}

}  // namespace
