// Base class for all TruPL tokens.
// @author Hieu Le
// @version 09/28/2016

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

// Types of tokens from TruPL.
typedef enum token_type { TOKEN_KEYWORD =  0,
			  TOKEN_PUNC    =  1,
			  TOKEN_RELOP   =  2,
			  TOKEN_ADDOP   =  3,
			  TOKEN_MULOP   =  4,
			  TOKEN_ID      =  5,
			  TOKEN_NUM     =  6,
			  TOKEN_EOF     =  7,
			  TOKEN_NO_TYPE = 99 } token_type_type;

class Token {
 public:
  // Constructs a token with TOKEN_NO_TYPE as default attribute.
  Token();

  // Virtual destructor for base class.
  virtual ~Token();

  // Forms a string consisting of the token type and its' attribute.
  // Useful for debugging. Output should be of the form TOKEN_TYPE:Attribute.
  // Returned value becomes property of the caller.
  virtual string *to_string() const {
    return nullptr;
  }

  // Sets the type of this token to a specified value.
  void set_token_type(token_type_type type);

  // Returns the type of this token.
  token_type_type get_token_type() const;

 private:
  // The type of this token.
  token_type_type type_;
};

#endif
