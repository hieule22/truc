// Unit tests for EofToken class.
// @author Hieu Le
// @version 09/28/2016

#include "src/eoftoken.h"

#include <memory>

#include "gtest/gtest.h"

namespace {

TEST(EofTokenTest, GetTokenType) {
  const EofToken token;
  EXPECT_EQ(token.get_token_type(), token_type_type::TOKEN_EOF);
}

TEST(EofTokenTest, ToString) {
  const EofToken token;
  std::unique_ptr<std::string> debug_string(token.to_string());
  EXPECT_EQ(*debug_string, "TOKEN_EOF:EOF");
}

}  // namespace
