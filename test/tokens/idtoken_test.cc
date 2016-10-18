// Unit tests for IdToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/idtoken.h"

#include <memory>

#include "gtest/gtest.h"

namespace {

TEST(IdTokenTest, GetTokenType) {
  const IdToken token("foo");
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_ID);
}

TEST(IdTokenTest, GetAttribute) {
  {
    const IdToken default_token;
    std::unique_ptr<std::string> attribute(default_token.get_attribute());
    EXPECT_TRUE(attribute->empty());
  }
  {
    const IdToken initialized_token("bar");
    std::unique_ptr<std::string> attribute(initialized_token.get_attribute());
    EXPECT_EQ(*attribute, "bar");
  }
  {
    IdToken changed_token("quoz");
    changed_token.set_attribute("foo");
    std::unique_ptr<std::string> attribute(changed_token.get_attribute());
    EXPECT_EQ(*attribute, "foo");
  }
}

TEST(IdTokenTest, SetAttribute) {
  IdToken token("foo");
  token.set_attribute("bar");
  std::unique_ptr<std::string> attribute(token.get_attribute());
  EXPECT_EQ(*attribute, "bar");
}

TEST(IdTokenTest, ToString) {
  const IdToken token("quoz");
  std::unique_ptr<std::string> debug_string(token.to_string());
  EXPECT_EQ(*debug_string, "TOKEN_ID:quoz");
}

}  // namespace
