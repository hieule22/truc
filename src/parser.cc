#include "parser.h"

#include <string>

Parser::Parser(Scanner *the_scanner) 
{
  /* Initialize the parser. */
  lex = the_scanner;
  word = lex->next_token();
}

Parser::~Parser() 
{
  /* Delete the parser. */
  if (lex != nullptr) {
    delete lex;
  }
  if (word != nullptr) {
    delete word;
  }
}

// If we have parsed the entire program, then word
// should be the EOF Token.  This function tests
// that condition.
bool Parser::done_with_input()
{
  return word->get_token_type() == TOKEN_EOF;
}

bool Parser::parse_program()
{
  // PROGRAM -> program identifier ; DECL_LIST BLOCK ;
  // Predict (program identifier ; DECL_LIST BLOCK ;) == {program}

  // Match keyword program, first symbol on RHS
  if (word->get_token_type() == TOKEN_KEYWORD 
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROGRAM) {

    /* ADVANCE - Notice that we only delete a token on an ADVANCE,
       and, if we ADVANCE, it is the ADVANCE code that is responsible
       for getting the next token.
    */
    delete word; 
    word = lex->next_token();

    // Match identifier, 2nd symbol on RHS
    if (word->get_token_type() == TOKEN_ID) {		
      // ADVANCE
      delete word; 
      word = lex->next_token();

      // Match semicolon(;), 3rd symbol on RHS
      if (word->get_token_type() == TOKEN_PUNC 
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
        // ADVANCE
        delete word; 
        word = lex->next_token();

        /* Match DECL_LIST, 4th bymbol on RHS.  This is an ACTION,
           not an advance, so we don't grab another token if we
           succeed.  We are just growing the parse tree at this
           point.
        */
        if (parse_decl_list()) {
	    
          // Match BLOCK, 5th on RHS - ACTION
          if (parse_block()) {
	      
            // Match semicolon(;), 6th and last on RHS -  
            if (word->get_token_type() == TOKEN_PUNC
                && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
              // ADVANCE
              // Since we advanced, we matched a token so we get the next one.
              delete word;
              word = lex->next_token();

              // Parse_program succeeded.
              return true;
		
              // We failed to match the second semicolon
            } else {
              string *expected = new string ("';'");
              // Variable expected should be deleted in parse_error()
              parse_error (expected, word);
              return false;
            }
	      
            /* We failed to parse BLOCK.  Don't print an error here;
               instead, print the error when you discover it,
               i.e. when you are trying to advance. */
          } else {
            return false;
          }
	    
          // We failed to parse DECL_LIST
        } else {
          return false;
        }
	  
        // We failed to match the first semicolon
      } else {
        string *expected = new string ("';'");
        parse_error (expected, word);
        return false;
      }
	
      // We failed to match an identifier
    } else {
      string *expected = new string ("identifier");
      parse_error (expected, word);
      return false;
    }
     
    // We failed to match the keyword program
  } else {
    string *expected = new string ("keyword program");
    parse_error (expected, word);
    return false;
  }
   
  /* We shouldn't reach this statement, but it is here as a defensive
     programming measure. */
  return false;
}

bool Parser::parse_decl_list()
{
  /* DECL_LIST -> VARIABLE_DECL_LIST PROCEDURE_DECL_LIST 

     Predict(VARIABLE_DECL_LIST PROCEDURE_DECL_LIST) 
     = First(VARIABLE_DECL_LIST) 
     union First (PROCEDURE_DECL_LIST) 
     union Follow (DECL_LIST) = {identifier, procedure, begin}

     Note that we don't actually need to check the predict set
     here.  The predict set is used to choose the correct
     production, but there isn't a choice here.

     In addition, we take advantage of C++'s short circuit
     evaluation of Boolean expressions. */

  return parse_variable_decl_list() && parse_procedure_decl_list();
}

bool Parser::parse_variable_decl_list()
{
  /* VARIABLE_DECL_LIST -> VARIABLE_DECL ; VARIABLE_DECL_LIST     
     Predict(VARIABLE_DECL ; VARIABLE_DECL_LIST) = First(VARIABLE_DECL)
     = {identifier}
  */
  if (word->get_token_type() == TOKEN_ID) {

    // Match VARIABLE_DECL.
    if (parse_variable_decl()) {
      
      // Match semicolon(;).
      if (word->get_token_type() == TOKEN_PUNC
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
        
        // ADVANCE
        delete word;
        word = lex->next_token();

        // Match VARIABLE_DECL_LIST.
        if (parse_variable_decl_list()) {
          return true;

          // Fail to match VARIABLE_DECL_LIST.
        } else {
          return false;
        }

        // Fail to match semicolon.
      } else {
        string *expected = new string("';'");
        parse_error(expected, word);
        return false;
      }

      // Fail to match VARIABLE_DECL.      
    } else {
      return false;
    }

    /* VARIABLE_DECL_LIST -> lambda       
       Predict(lambda) = First(lambda) union Follow(VARIABLE_DECL_LIST)
       = {lambda, procedure, begin} */
  } else {
    return true;
  }

  return false;
}

bool Parser::parse_variable_decl()
{
  /* VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE
     Predict(IDENTIFIER_LIST : STANDARD_TYPE) = First(IDENTIFIER_LIST)
     = {identifier} */

  if (word->get_token_type() == TOKEN_ID) {

    // Match IDENTIFIER_LIST
    if (parse_identifier_list()) {

      // Match colon(:).
      if (word->get_token_type() == TOKEN_PUNC
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

        // ADVANCE
        delete word;
        word = lex->next_token();

        // Match STANDARD_TYPE
        if (parse_standard_type()) {

          // parse_standard_type succeeds.
          return true;

          // Fail to match STANDARD_TYPE.
        } else {
          return false;
        }

        // Fail to match colon.
      } else {
        string *expected = new string("':'");
        parse_error(expected, word);
        return false;
      }

      // Fail to match IDENTIFIER_LIST.
    } else {
      return false;
    }

    // Fail to match an identifier.
  } else {
    string *expected = new string("identifier");
    parse_error(expected, word);
    return false;
  }

  return false;
}

bool Parser::parse_procedure_decl_list()
{
  /* PROCEDURE_DECL_LIST -> PROCEDURE_DECL ; PROCEDURE_DECL_LIST

     Predict(PROCEDURE_DECL ; PROCEDURE_DECL_LIST) = First(PROCEDURE_DECL)
     = {procedure} */

  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE) {

    // Match PROCEDURE_DECL.
    if (parse_procedure_decl()) {

      // Match semicolon (;).
      if (word->get_token_type() == TOKEN_PUNC
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

        // ADVANCE.
        delete word;
        word = lex->next_token();

        // Match PROCEDURE_DECL_LIST.
        if (parse_procedure_decl_list()) {

          // parse_procedure_decl_list succeeds.
          return true;

          // Fail to match PROCEDURE_DECL_LIST.
        } else {
          return false;
        }

        // Fail to match semicolon.
      } else {
        parse_error(new string("';'"), word);
        return false;
      }

      // Fail to match PROCEDURE_DECL.
    } else {
      return false;
    }

    /* PROCEDURE_DECL_LIST -> lambda
       Predict(lambda) = First(lambda) union Follow(PROCEDURE_DECL_LIST)
       = {lambda, begin} */
  } else {
    return true;
  }

  return false;
}

bool Parser::parse_identifier_list()
{
  /* IDENTIFIER_LIST = identifier IDENTIFIER_LIST_PRM

     Predict(identifier IDENTIFIER_LIST_PRM) = {identifier} */

  if (word->get_token_type() == TOKEN_ID) {

    // Match IDENTIFIER_LIST_PRM.
    if (parse_identifier_list_prm()) {

      // parse_identifier_list_prm succeeds.
      return true;

      // Fail to match IDENTIFIER_LIST_PRM.
    } else {
      return false;
    }

    // Fail to match an identifier.
  } else {
    parse_error(new string("identifier"), word);
    return false;
  }

  return false;
}

bool Parser::parse_identifier_list_prm()
{

  /* IDENTIFIER_LIST_PRM = , identifier IDENTIFIER_LIST_PRM

     Predict(, identifier IDENTIFIER_LIST_PRM) = {,} */

  if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA) {

    // ADVANCE
    delete word;
    word = lex->next_token();

    // Match an identifier.
    if (word->get_token_type() == TOKEN_ID) {

      // Match IDENTIFIER_LIST_PRM.
      if (parse_identifier_list_prm()) {

        // parse_identifier_list_prm() succeeds.
        return true;

        // Fail to match IDENTIFIER_LIST_PRM.
      } else {
        return false;
      }

      // Fail to match an identifier.
    } else {
      parse_error(new string("identifier"), word);
      return false;
    }

    /* IDENTIFIER_LIST_PRM = lambda

       Predict(lambda) = First(lambda) union Follow(IDENTIFIER_LIST_PRM)
       = {lambda, :} */
  } else {
    return true;
  }
  
  return false;
}

bool Parser::parse_standard_type()
{
  /* STANDARD_TYPE -> int

     Predict(int) = {int} */
  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_INT) {

    // ADVANCE.
    delete word;
    word = lex->next_token();
    return true;

    /* STANDARD_TYPE -> bool
       
       Predict(bool) = {bool} */
  } else if (word->get_token_type() == TOKEN_KEYWORD
             && static_cast<KeywordToken *>(word)->get_attribute() == KW_BOOL) {

    // ADVANCE.
    delete word;
    word = lex->next_token();
    return true;
  }

  return false;
}

bool Parser::parse_block()
{
  /* BLOCK -> begin STMT_LIST end

     Predict(begin STMT_LIST end) = {begin} */
  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN) {

    // ADVANCE.
    delete word;
    word = lex->next_token();

    // Match STMT_LIST.
    if (parse_stmt_list()) {

      // Match keyword end.
      if (word->get_token_type() == TOKEN_KEYWORD
          && static_cast<KeywordToken *>(word)->get_attribute() == KW_END) {

        // ADVANCE.
        delete word;
        word = lex->next_token();

        return true;

        // Fail to match keyword end.
      } else {
        parse_error(new string("end"), word);
        return false;
      }

      // Fail to match STMT_LIST.
    } else {
      return false;
    }

    // Fail to match keyword begin.
  } else {
    parse_error(new string("begin"), word);
    return false;
  }

  return false;
}

bool Parser::parse_procedure_decl()
{
  /* PROCEDURE_DECL ->
     procedure identifier ( PROCEDURE_ARGS ) VARIABLE_DECL_LIST BLOCK

     Predict(PROCEDURE_DECL) = {procedure} */
  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE) {

    // ADVANCE.
    delete word;
    word = lex->next_token();

    // Match an identifier.
    if (word->get_token_type() == TOKEN_ID) {

      // ADVANCE.
      delete word;
      word = lex->next_token();

      // Match an opening bracket.
      if (word->get_token_type() == TOKEN_PUNC
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) {

        // ADVANCE.
        delete word;
        word = lex->next_token();

        // Match PROCEDURE_ARGS.
        if (parse_procedure_args()) {

          // Match a closing bracket.
          if (word->get_token_type() == TOKEN_PUNC
              && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE) {

            // ADVANCE.
            delete word;
            word = lex->next_token();

            // Match VARIABLE_DECL_LIST.
            if (parse_variable_decl_list()) {

              // Match BLOCK.
              if (parse_block()) {
                return true;

                // Fail to match block.
              } else {
                return false;
              }

              // Fail to match VARIABLE_DECL_LIST.
            } else {
              return false;
            }

            // Fail to match a closing bracket.
          } else {
            parse_error(new string("')'"), word);
            return false;
          }

          // Fail to match PROCEDURE_ARGS.
        } else {
          return false;
        }

        // Fail to match an opening bracket.
      } else {
        parse_error(new string("'('"), word);
        return false;
      }

      // Fail to match an identifier.
    } else {
      parse_error(new string("identifier"), word);
      return false;
    }

    // Fail to match keyword procedure.
  } else {
    parse_error(new string("procedure"), word);
    return false;
  }

  return false;
}

bool Parser::parse_procedure_args()
{
  /* PROCEDURE_ARGS -> FORMAL_PARM_LIST

     Predict(FORMAL_PARM_LIST) = First(FORMAL_PARM_LIST) = {identifier} */
  if (word->get_token_type() == TOKEN_ID) {

    // Match FORMAL_PARM_LIST.
    if (parse_formal_parm_list()) {
      return true;

      // Fail to match FORMAL_PARM_LIST.
    } else {
      return false;
    }

    /* PROCEDURE_ARGS -> lambda

       Predict(lambda) = First(lambda) union Follow(PROCEDURE_ARGS)
       = {lambda, )} */
  } else {
    return true;
  }

  return false;
}

bool Parser::parse_formal_parm_list()
{
  /* FORMAL_PARM_LIST -> identifier IDENTIFIER_LIST_PRM : STANDARD_TYPE
     FORMAL_PARM_LIST_HAT

     Predict(...) = {identifier} */
  if (word->get_token_type() == TOKEN_ID) {

    // ADVANCE.
    delete word;
    word = lex->next_token();

    // Match IDENTIFIER_LIST_PRM.
    if (parse_identifier_list_prm()) {

      // Match colon (:).
      if (word->get_token_type() == TOKEN_PUNC
          && static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON) {

        // ADVANCE.
        delete word;
        word = lex->next_token();

        // Match STANDARD_TYPE.
        if (parse_standard_type()) {
          return true;

          // Fail to match STANDARD_TYPE.
        } else {
          return false;
        }

        // Fail to match colon.
      } else {
        parse_error(new string("':'"), word);
        return false;
      }

      // Fail to match IDENTIFIER_LIST_PRM
    } else {
      return false;
    }

    // Fail to match an identifier.
  } else {
    parse_error(new string("identifier"), word);
    return false;
  }

  return false;
}

bool Parser::parse_formal_parm_list_hat()
{
  /* FORMAL_PARM_LIST_HAT -> ; FORMAL_PARM_LIST

     Predict(; FORMAL_PARM_LIST) = {;} */
  if (word->get_token_type() == TOKEN_PUNC
      && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

    // ADVANCE.
    delete word;
    word = lex->next_token();
    
    // Match FORMAL_PARM_LIST.
    if (parse_formal_parm_list()) {
      return true;

      // Fail to match FORMAL_PARM_LIST.
    } else {
      return false;
    }

    /* FORMAL_PARM_LIST_HAT = lambda

       Predict(lambda) = First(lambda) union Follow(FORMAL_PARM_LIST_HAT)
       = {lambda, )} */
  } else {
    return true;
  }

  return false;
}

bool Parser::parse_stmt_list()
{
  /* STMT_LIST -> STMT ; STMT_LIST_PRM

     Predict(STMT_LIST) = First(STMT) = {identifier, if, while, print} */
  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_KEYWORD && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF) ||
      (word->get_token_type() == TOKEN_KEYWORD && static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE) ||
      (word->get_token_type() == TOKEN_KEYWORD && static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)) {

    // ADVANCE.
    delete word;
    word = lex->next_token();
    
    // Match a semicolon (;).
    if (word->get_token_type() == TOKEN_PUNC
        && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

      // ADVANCE.
      delete word;
      word = lex->next_token();
      
      // Match STMT_LIST_PRM.
      if (parse_stmt_list_prm()) {
        return true;

        // Fail to match STMT_LIST_PRM.
      } else {
        return false;
      }

      // Fail to match a semicolon.
    } else {
      parse_error(new string("';'"), word);
      return false;
    }

    /* STMT_LIST -> ; STMT_LIST_PRM

       Predict(; STMT_LIST_PRM) = {;} */
  } else if (word->get_token_type() == TOKEN_PUNC
             || static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {

    // ADVANCE.
    delete word;
    word = lex->next_token();

    // Match STMT_LIST_PRM.
    if (parse_stmt_list_prm()) {
      return true;

      // Fail to match STMT_LIST_PRM.
    } else {
      return false;
    }
  }

  return false;
}

void Parser::parse_error(string *expected, Token *found) {
  std::cerr << "Parse error: Expected: " << *expected <<
      ", found  " << found->to_string() << std::endl;
  delete expected;
}
