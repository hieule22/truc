#include "parser.h"

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
