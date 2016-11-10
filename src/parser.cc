// Implementation of Parser class.
// @author Hieu Le
// @version November 8th, 2016.

#include "parser.h"

#include <string>

// Log a message to console for debugging.
#define DEBUGMODE 1
#define LOG(output) \
  if (DEBUGMODE) std::cerr << output << std::endl

Parser::Parser(Scanner *the_scanner) 
{
  /* Initialize the parser. */
  lex = the_scanner;
  word = lex->next_token();
  LOG("Parsing: " << *word->to_string());
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

void Parser::parse_error(string *expected, Token *found) {
  std::cerr << "Parse error: Expected: " << *expected <<
      ", Found:  " << *found->to_string() << std::endl;
  delete expected;
}

void Parser::advance() {
  delete word;
  word = lex->next_token();
  LOG("Parsing: " << *word->to_string());
}

namespace {

// Checks if a given token is an identifier.
inline bool is_identifier(const Token* token) {
  return token->get_token_type() == TOKEN_ID;
}

// Checks if a given token is a keyword with the specified attribute.
inline bool is_keyword(const Token* token, const keyword_attr attr) {
  return token->get_token_type() == TOKEN_KEYWORD
      && static_cast<const KeywordToken *>(token)->get_attribute() == attr;
}

// Checks if a given token is a punctuation with the specified attribute.
inline bool is_punctuation(const Token* token, const punc_attr attr) {
  return token->get_token_type() == TOKEN_PUNC
      && static_cast<const PuncToken *>(token)->get_attribute() == attr;
}

// Checks if a given token is an additive operator.
inline bool is_addop(const Token* token) {
  return token->get_token_type() == TOKEN_ADDOP;
}

// Checks if a given token is an addop with the specified attribute.
inline bool is_addop(const Token* token, const addop_attr attr) {
  return is_addop(token) &&
      static_cast<const AddopToken *>(token)->get_attribute() == attr;
}

// Checks if a given token is a multiplicative operator.
inline bool is_mulop(const Token* token) {
  return token->get_token_type() == TOKEN_MULOP;
}

// Checks if a given token is a relational operator.
inline bool is_relop(const Token* token) {
  return token->get_token_type() == TOKEN_RELOP;
}

// Checks if a given token is a number.
inline bool is_number(const Token* token) {
  return token->get_token_type() == TOKEN_NUM;
}

}  // namespace

bool Parser::parse_program()
{
  // PROGRAM -> program identifier ; DECL_LIST BLOCK ;
  // Predict (program identifier ; DECL_LIST BLOCK ;) == {program}

  // Match keyword program, first symbol on RHS
  if (is_keyword(word, KW_PROGRAM)) {
    LOG("PROGRAM -> program identifier ; DECL_LIST BLOCK ;");

    /* ADVANCE - Notice that we only delete a token on an ADVANCE,
       and, if we ADVANCE, it is the ADVANCE code that is responsible
       for getting the next token.
    */
    advance(); 
 
    // Match identifier, 2nd symbol on RHS
    if (word->get_token_type() == TOKEN_ID) {		
      // ADVANCE
      advance(); 
      

      // Match semicolon(;), 3rd symbol on RHS
      if (is_punctuation(word, PUNC_SEMI)) {
        // ADVANCE
        advance(); 
 
        /* Match DECL_LIST, 4th bymbol on RHS.  This is an ACTION,
           not an advance, so we don't grab another token if we
           succeed.  We are just growing the parse tree at this
           point.
        */
        if (parse_decl_list()) {
	    
          // Match BLOCK, 5th on RHS - ACTION
          if (parse_block()) {
	      
            // Match semicolon(;), 6th and last on RHS -
            if (is_punctuation(word, PUNC_SEMI)) {
              // ADVANCE
              // Since we advanced, we matched a token so we get the next one.
              advance();
 
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

  LOG("DECL_LIST -> VARIABLE_DECL_LIST PROCEDURE_DECL_LIST");
  
  return parse_variable_decl_list() && parse_procedure_decl_list();
}

bool Parser::parse_variable_decl_list()
{
  /* VARIABLE_DECL_LIST -> VARIABLE_DECL ; VARIABLE_DECL_LIST     
     Predict(VARIABLE_DECL ; VARIABLE_DECL_LIST) == First(VARIABLE_DECL)
     == {identifier} */
  if (is_identifier(word)) {
    LOG("VARIABLE_DECL_LIST -> VARIABLE_DECL ; VARIABLE_DECL_LIST");

    // Match VARIABLE_DECL - ACTION.
    if (parse_variable_decl()) {
      
      // Match semicolon(;).
      if (is_punctuation(word, PUNC_SEMI)) {
        
        // ADVANCE.
        advance();

        // Match VARIABLE_DECL_LIST - ACTION.
	return parse_variable_decl_list();

        // Fail to match semicolon.
      } else {
        parse_error(new string("';'"), word);
        return false;
      }

      // Fail to match VARIABLE_DECL.      
    } else {
      return false;
    }

    /* VARIABLE_DECL_LIST -> lambda */
  } else {
    LOG("VARIABLE_DECL_LIST -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_variable_decl()
{
  /* VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE
     Predict(IDENTIFIER_LIST : STANDARD_TYPE) == First(IDENTIFIER_LIST)
     == {identifier} */
  if (is_identifier(word)) {
    LOG("VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE");
    
    // Match IDENTIFIER_LIST - ACTION.
    if (parse_identifier_list()) {

      // Match colon(:).
      if (is_punctuation(word, PUNC_COLON)) {

        // ADVANCE.
        advance();        

        // Match STANDARD_TYPE - ACTION.
        return parse_standard_type();

        // Fail to match colon.
      } else {
        parse_error(new string("':'"), word);
        return false;
      }

      // Fail to match IDENTIFIER_LIST.
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

bool Parser::parse_procedure_decl_list()
{
  /* PROCEDURE_DECL_LIST -> PROCEDURE_DECL ; PROCEDURE_DECL_LIST
     Predict(PROCEDURE_DECL ; PROCEDURE_DECL_LIST) == First(PROCEDURE_DECL)
     == {procedure} */
  if (is_keyword(word, KW_PROCEDURE)) {
    LOG("PROCEDURE_DECL_LIST -> PROCEDURE_DECL ; PROCEDURE_DECL_LIST");

    // Match PROCEDURE_DECL - ACTION.
    if (parse_procedure_decl()) {

      // Match semicolon (;).
      if (is_punctuation(word, PUNC_SEMI)) {

        // ADVANCE.
        advance();  

        // Match PROCEDURE_DECL_LIST - ACTION.
        return parse_procedure_decl_list();

        // Fail to match semicolon.
      } else {
        parse_error(new string("';'"), word);
        return false;
      }

      // Fail to match PROCEDURE_DECL.
    } else {
      return false;
    }

    /* PROCEDURE_DECL_LIST -> lambda */
  } else {
    LOG("PROCEDURE_DECL_LIST -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_identifier_list()
{
  /* IDENTIFIER_LIST -> identifier IDENTIFIER_LIST_PRM
     Predict(identifier IDENTIFIER_LIST_PRM) == {identifier} */
  if (is_identifier(word)) {
    LOG("IDENTIFIER_LIST -> identifier IDENTIFIER_LIST_PRM");

    // ADVANCE.
    advance();

    // Match IDENTIFIER_LIST_PRM - ACTION.
    return parse_identifier_list_prm();

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
     Predict(, identifier IDENTIFIER_LIST_PRM) == {,} */
  if (is_punctuation(word, PUNC_COMMA)) {
    LOG("IDENTIFIER_LIST_PRM -> , identifier IDENTIFIER_LIST_PRM");
    
    // ADVANCE.
    advance();    

    // Match an identifier.
    if (is_identifier(word)) {

      // ADVANCE.
      advance();

      // Match IDENTIFIER_LIST_PRM - ACTION.
      return parse_identifier_list_prm();

      // Fail to match an identifier.
    } else {
      parse_error(new string("identifier"), word);
      return false;
    }

    /* IDENTIFIER_LIST_PRM = lambda */
  } else {
    LOG("IDENTIFIER_LIST_PRM -> lambda");
    return true;
  }
  
  return false;
}

bool Parser::parse_standard_type()
{
  /* STANDARD_TYPE -> int
     Predict(int) = {int} */
  if (is_keyword(word, KW_INT)) {
    LOG("STANDARD_TYPE -> int");
    
    // ADVANCE.
    advance();
    
    return true;

    /* STANDARD_TYPE -> bool       
       Predict(bool) = {bool} */ 
  } else if (is_keyword(word, KW_BOOL)) {
    LOG("STANDARD_TYPE -> bool");
    
    // ADVANCE.
    advance();
    
    return true;    
  }

  return false;
}

bool Parser::parse_block()
{
  /* BLOCK -> begin STMT_LIST end
     Predict(begin STMT_LIST end) = {begin} */
  if (is_keyword(word, KW_BEGIN)) {
    LOG("BLOCK -> begin STMT_LIST end");
    
    // ADVANCE.
    advance();
    
    // Match STMT_LIST - ACTION.
    if (parse_stmt_list()) {

      // Match keyword end.
      if (is_keyword(word, KW_END)) {

        // ADVANCE.
        advance();    

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
     Predict(PROCEDURE_DECL) == {procedure} */
  if (is_keyword(word, KW_PROCEDURE)) {
    LOG("PROCEDURE_DECL -> procedure identifier ( PROCEDURE_ARGS ) "
        "VARIABLE_DECL_LIST BLOCK");

    // ADVANCE.
    advance();

    // Match an identifier.
    if (is_identifier(word)) {

      // ADVANCE.
      advance();

      // Match an opening bracket.
      if (is_punctuation(word, PUNC_OPEN)) {

        // ADVANCE.
        advance();

        // Match PROCEDURE_ARGS - ACTION.
        if (parse_procedure_args()) {

          // Match a closing bracket.
          if (is_punctuation(word, PUNC_CLOSE)) {

            // ADVANCE.
            advance();

            // Match VARIABLE_DECL_LIST and BLOCK - ACTION.
            return parse_variable_decl_list() && parse_block();

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
     Predict(FORMAL_PARM_LIST) == First(FORMAL_PARM_LIST) == {identifier} */
  if (is_identifier(word)) {
    LOG("PROCEDURE_ARGS -> FORMAL_PARM_LIST");

    // Match FORMAL_PARM_LIST - ACTION.
    return parse_formal_parm_list();

    /* PROCEDURE_ARGS -> lambda */
  } else {
    LOG("PROCEDURE_ARGS -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_formal_parm_list()
{
  /* FORMAL_PARM_LIST ->
     identifier IDENTIFIER_LIST_PRM : STANDARD_TYPE FORMAL_PARM_LIST_HAT
     Predict(...) = {identifier} */
  if (is_identifier(word)) {
    LOG("FORMAL_PARM_LIST -> identifier IDENTIFIER_LIST_PRM : "
        "STANDARD_TYPE FORMAL_PARM_LIST_HAT");

    // ADVANCE.
    advance();

    // Match IDENTIFIER_LIST_PRM - ACTION.
    if (parse_identifier_list_prm()) {

      // Match colon(:).
      if (is_punctuation(word, PUNC_COLON)) {

        // ADVANCE.
        advance();

        // Match STANDARD_TYPE and FORMAL_PARM_LIST_HAT - ACTION.
        return parse_standard_type() && parse_formal_parm_list_hat();

        // Fail to match colon.
      } else {
        parse_error(new string("':'"), word);
        return false;
      }

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

bool Parser::parse_formal_parm_list_hat()
{
  /* FORMAL_PARM_LIST_HAT -> ; FORMAL_PARM_LIST
     Predict(; FORMAL_PARM_LIST) == {;} */
  if (is_punctuation(word, PUNC_SEMI)) {
    LOG("FORMAL_PARM_LIST_HAT -> ; FORMAL_PARM_LIST");

    // ADVANCE.
    advance();
    
    // Match FORMAL_PARM_LIST.
    return parse_formal_parm_list();

    /* FORMAL_PARM_LIST_HAT = lambda */
  } else {
    LOG("FORMAL_PARM_LIST_HAT -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_stmt_list()
{
  /* STMT_LIST -> STMT ; STMT_LIST_PRM
     Predict(STMT_LIST) == First(STMT) == {identifier, if, while, print} */
  if (is_identifier(word)
      || is_keyword(word, KW_IF)
      || is_keyword(word, KW_WHILE)
      || is_keyword(word, KW_PRINT)) {
    LOG("STM_LIST -> STMT ; STMT_LIST_PRM");

    // Match STMT - ACTION.
    if (parse_stmt()) {
    
      // Match a semicolon(;).
      if (is_punctuation(word, PUNC_SEMI)) {

        // ADVANCE.
        advance();
      
        // Match STMT_LIST_PRM - ACTION.
        return parse_stmt_list_prm();

        // Fail to match a semicolon.
      } else {
        parse_error(new string("';'"), word);
        return false;
      }

      // Fail to match STMT.
    } else {
      return false;
    }

    /* STMT_LIST -> ; STMT_LIST_PRM
       Predict(; STMT_LIST_PRM) == {;} */
  } else if (is_punctuation(word, PUNC_SEMI)) {
    LOG("STMT_LIST -> ; STMT_LIST_PRM");
    
    // ADVANCE.
    advance();

    // Match STMT_LIST_PRM - ACTION.
    return parse_stmt_list_prm();    
  }

  return false;
}

bool Parser::parse_stmt_list_prm()
{
  /* STMT_LIST_PRM -> STMT ; STMT_LIST_PRM
     Predict(STMT ; STMT_LIST_PRM) == First(STMT)
     == {identifier, if, while, print} */
  if (is_identifier(word)
      || is_keyword(word, KW_IF)
      || is_keyword(word, KW_WHILE)
      || is_keyword(word, KW_PRINT)) {
    LOG("STMT_LIST_PRM -> STMT ; STMT_LIST_PRM");

    // Match STMT - ACTION.
    if (parse_stmt()) {

      // Match semicolon(;).
      if (is_punctuation(word, PUNC_SEMI)) {

        // ADVANCE.
        advance();

        // Match STMT_LIST_PRM - ACTION.
        return parse_stmt_list_prm();

        // Fail to match semicolon.
      } else {
        parse_error(new string("';'"), word);
        return false;
      }

      // Fail to match STMT.
    } else {
      return false;
    }

    /* STMT_LIST_PRM -> lambda */
  } else {
    LOG("STMT_LIST_PRM -> lambda");
    return true;
  }
  
  return false;
}

bool Parser::parse_stmt()
{
  /* STMT -> IF_STMT
     Predict(IF_STMT) == First(IF_STMT) == {if} */
  if (is_keyword(word, KW_IF)) {
    LOG("STMT -> IF_STMT");

    // Match IF_STMT - ACTION.
    return parse_if_stmt();

    /* STMT -> WHILE_STMT
       Predict(WHILE_STMT) = First(WHILE_STMT) = {while} */
  } else if (is_keyword(word, KW_WHILE)) {
    LOG("STMT -> WHILE_STMT");

    // Match WHILE_STMT - ACTION.
    return parse_while_stmt();

    /* STMT -> PRINT_STMT
       Predict(PRINT_STMT) = First(PRINT_STMT) = {print} */
  } else if (is_keyword(word, KW_PRINT)) {
    LOG("STMT -> PRINT_STMT");
    
    // Match PRINT_STMT - ACTION.
    return parse_print_stmt();

    /* STMT -> identifier ADHOC_AS_PC_TAIL
       Predict(identifier ADHOC_AS_PC_TAIL) = {identifier} */
  } else if (is_identifier(word)) {
    LOG("STMT -> identifier ADHOC_AS_PC_TAIL");

    // ADVANCE.
    advance();

    // Match ADHOC_AS_PC_TAIL - ACTION.
    return parse_adhoc_as_pc_tail();    
  }

  return false;
}

bool Parser::parse_adhoc_as_pc_tail()
{
  /* ADHOC_AS_PC_TAIL -> := EXPR
     Predict(:= EXPR) == {:=} */
  if (is_punctuation(word, PUNC_ASSIGN)) {
    LOG("ADHOC_AS_PC_TAIL -> := EXPR");

    // ADVANCE.
    advance();

    // Match EXPR - ACTION.
    return parse_expr();

    /* ADHOC_AS_PC_TAIL -> ( EXPR_LIST )
       Predict( ( EXPR_LIST ) ) = { ( } */
  } else if (is_punctuation(word, PUNC_OPEN)) {
    LOG("ADHOC_AS_PC_TAIL -> ( EXPR_LIST )");

    // ADVANCE.
    advance();

    // Match EXPR_LIST - ACTION.
    if (parse_expr_list()) {

      // Match closing bracket.
      if (is_punctuation(word, PUNC_CLOSE)) {

        // ADVANCE.
        advance();

        return true;

        // Fail to match closing bracket.
      } else {
        parse_error(new string("')'"), word);
        return false;
      }

      // Fail to match EXPR_LIST.
    } else {
      return false;
    }
  }

  return false;
}

bool Parser::parse_if_stmt()
{
  /* IF_STMT -> if EXPR then BLOCK IF_STMT_HAT
     Predict(...) = {if} */
  if (is_keyword(word, KW_IF)) {
    LOG("IF_STMT -> if EXPR then BLOCK IF_STMT_HAT");

    // ADVANCE.
    advance();

    // Match EXPR - ACTION.
    if (parse_expr()) {

      // Match keyword then.
      if (is_keyword(word, KW_THEN)) {

        // ADVANCE.
        advance();

        // Match BLOCK and IF_STMT_HAT - ACTION.
        return parse_block() && parse_if_stmt_hat();

        // Fail to match keyword then.
      } else {
        parse_error(new string("then"), word);
        return false;
      }

      // Fail to match EXPR.
    } else {
      return false;
    }

    // Fail to match keyword if.
  } else {
    parse_error(new string("if"), word);
    return false;
  }

  return false;
}

bool Parser::parse_if_stmt_hat()
{
  /* IF_STMT_HAT -> else BLOCK
     Predict(else BLOCK) == {else} */
  if (is_keyword(word, KW_ELSE)) {
    LOG("IF_STMT_HAT -> else BLOCK");

    // ADVANCE.
    advance();

    // Match BLOCK - ACTION.
    return parse_block();

    /* IF_STMT_HAT -> lambda */
  } else {
    LOG("IF_STMT_HAT -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_while_stmt()
{
  /* WHILE_STMT -> while EXPR loop BLOCK
     Predict(while EXPR loop BLOCK) = {while} */
  if (is_keyword(word, KW_WHILE)) {
    LOG("WHILE_STMT -> while EXPR loop BLOCK");

    // ADVANCE.
    advance();

    // Match EXPR - ACTION.
    if (parse_expr()) {

      // Match keyword loop.
      if (is_keyword(word, KW_LOOP)) {

        // ADVANCE.
        advance();

        // Match BLOCK - ACTION.
        return parse_block();

        // Fail to match keyword loop.
      } else {
        parse_error(new string("loop"), word);
        return false;
      }

      // Fail to match EXPR.
    } else {
      return false;
    }

    // Fail to match keyword while.
  } else {
    parse_error(new string("while"), word);
    return false;
  }

  return false;
}

bool Parser::parse_print_stmt()
{
  /* PRINT_STMT -> print EXPR
     Predict(print EXPR) == {print} */
  if (is_keyword(word, KW_PRINT)) {
    LOG("PRINT -> print EXPR");

    // ADVANCE.
    advance();

    // Match EXPR - ACTION.
    return parse_expr();

    // Fail to match keyword print.
  } else {
    parse_error(new string("print"), word);
    return false;
  }

  return false;
}

bool Parser::parse_expr_list()
{
  /* EXPR_LIST -> ACTUAL_PARM_LIST
     Predict(ACTUAL_PARM_LIST) == First(ACTUAL_PARM_LIST)
     == {identifier, num, (, +, -, not} */
  if (is_identifier(word)
      || is_number(word)
      || is_punctuation(word, PUNC_OPEN)
      || is_addop(word, ADDOP_ADD)
      || is_addop(word, ADDOP_SUB)
      || is_keyword(word, KW_NOT)) {
    LOG("EXPR_LIST -> ACTUAL_PARM_LIST");

    // Match ACTUAL_PARM_LIST - ACTION.
    return parse_actual_parm_list();

    /* EXPR_LIST -> lambda */
  } else {
    LOG("EXPR_LIST -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_actual_parm_list()
{ 
  /* ACTUAL_PARM_LIST -> EXPR ACTUAL_PARM_LIST_HAT */
  LOG("ACTUAL_PARM_LIST -> EXPR ACTUAL_PARM_LIST_HAT");
  
  // Match EXPR and ACTUAL_PARM_LIST_HAT - ACTION.
  return parse_expr() && parse_actual_parm_list_hat();
}

bool Parser::parse_actual_parm_list_hat()
{
  /* ACTUAL_PARM_LIST_HAT -> , ACTUAL_PARM_LIST
     Predict(, ACTUAL_PARM_LIST) == {,} */
  if (is_punctuation(word, PUNC_COMMA)) {
    LOG("ACTUAL_PARM_LIST_HAT -> , ACTUAL_PARM_LIST");

    // ADVANCE.
    advance();

    // Match ACTUAL_PARM_LIST - ACTION.
    return parse_actual_parm_list();

    /* ACTUAL_PARM_LIST_HAT -> lambda */
  } else {
    LOG("ACTUAL_PARM_LIST_HAT -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_expr()
{
  /* EXPR -> SIMPLE_EXPR EXPR_HAT */
  LOG("EXPR -> SIMPLE_EXPR EXPR_HAT");

  // Match SIMPLE_EXPR and EXPR_HAT - ACTION.
  return parse_simple_expr() && parse_expr_hat();
}

bool Parser::parse_expr_hat()
{
  /* EXPR_HAT -> relop SIMPLE_EXPR
     Predict(relop SIMPLE_EXPR) = {relop} */
  if (is_relop(word)) {
    LOG("EXPR_HAT -> relop SIMPLE_EXPR");
    
    // ADVANCE.
    advance();

    // Match SIMPLE_EXPR - ACTION.
    return parse_simple_expr();

    /* EXPR_HAT -> lambda */
  } else {
    LOG("EXPR_HAT -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_simple_expr()
{  
  /* SIMPLE_EXPR -> TERM SIMPLE_EXPR_PRM */
  LOG("SIMPLE_EXPR -> TERM SIMPLE_EXPR_PRM");
    
  // Match TERM and SIMPLE_EXPR_PRM - ACTION.
  return parse_term() && parse_simple_expr_prm();
}

bool Parser::parse_simple_expr_prm()
{
  /* SIMPLE_EXPR_PRM -> addop TERM SIMPLE_EXPR_PRM
     Predict(addop TERM SIMPLE_EXPR_PRM) == {addop} */
  if (is_addop(word)) {
    LOG("SIMPLE_EXPR_PRM -> addop TERM SIMPLE_EXPR_PRM");
    
    // ADVANCE.
    advance();

    // Match TERM and SIMPLE_EXPR_PRM - ACTION.
    return parse_term() && parse_simple_expr_prm();

    /* SIMPLE_EXPR_PRM -> lambda */
  } else {
    LOG("SIMPLE_EXPR_PRM -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_term()
{
  /* TERM -> FACTOR TERM_PRM */
  LOG("TERM -> FACTOR TERM_PRM");
  
  // Match FACTOR and TERM_PRM - ACTION.
  return parse_factor() && parse_term_prm();
}

bool Parser::parse_term_prm()
{
  /* TERM_PRM -> mulop FACTOR TERM_PRM
     Predict(mulop FACTOR TERM_PRM) = {mulop} */
  if (is_mulop(word)) {
    LOG("TERM_PRM -> mulop FACTOR TERM_PRM");

    // ADVANCE.
    advance();

    // Match FACTOR and TERM_PRM - ACTION.
    return parse_factor() && parse_term_prm();

    /* TERM_PRM -> lambda */
  } else {
    LOG("TERM_PRM -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_factor()
{
  /* FACTOR -> identifier
     Predict(identifier) = {identifier} */
  if (is_identifier(word)) {
    LOG("FACTOR -> identifier");

    // ADVANCE.
    advance();

    return true;

    /* FACTOR -> num
       Predict(num) = {num} */
  } else if (is_number(word)) {
    LOG("FACTOR -> number");

    // ADVANCE.
    advance();

    return true;

    /* FACTOR -> ( EXPR )
       Predict( ( EXPR ) ) == { ( } */
  } else if (is_punctuation(word, PUNC_OPEN)) {
    LOG("FACTOR -> ( EXPR )");

    // ADVANCE.
    advance();

    // Match EXPR.
    if (parse_expr()) {

      // Match close bracket.
      if (is_punctuation(word, PUNC_CLOSE)) {

        // ADVANCE.
        advance();

        return true;

        // Fail to match a close bracket.
      } else {
        parse_error(new string("')'"), word);
        return false;
      }

      // Fail to match EXPR.
    } else {
      return false;
    }

    /* FACTOR -> SIGN FACTOR
       Predict(SIGN FACTOR) == First(SIGN) == {+, -, not} */
  } else if (is_addop(word, ADDOP_ADD)
             || is_addop(word, ADDOP_SUB)
             || is_keyword(word, KW_NOT)) {
    LOG("FACTOR -> SIGN FACTOR");

    // Match SIGN and FACTOR.
    return parse_sign() && parse_factor();
  }

  return false;
}

bool Parser::parse_sign()
{
  /* SIGN -> +
     Predict(SIGN) == {+} */
  if (is_addop(word, ADDOP_ADD)) {
    LOG("SIGN -> +");

    // ADVANCE.
    advance();

    return true;

    /* SIGN -> -
       Predict(-) == {-) */
  } else if (is_addop(word, ADDOP_SUB)) {
    LOG("SIGN -> -");

    // ADVANCE.
    advance();

    return true;

    /* SIGN -> not
       Predict(not) == {not} */
  } else if (is_keyword(word, KW_NOT)) {
    LOG("SIGN -> not");

    // ADVANCE.
    advance();

    return true;
  }

  return false;
}
