// Implementation of Parser class.
// @author Hieu Le
// @version November 8th, 2016.

#include "parser.h"

#include <string>

// Log a message to console for debugging.
#define DEBUGMODE 0
#define LOG(output) \
  if (DEBUGMODE) std::cerr << output << std::endl

Parser::Parser(Scanner *the_scanner) {
  /* Initialize the parser. */
  lex = the_scanner;
  parsing_formal_parm_list = false;
  word = lex->next_token();
  LOG("Parsing: " << *word->to_string());
}

Parser::~Parser() {
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
bool Parser::done_with_input() const {
  return word->get_token_type() == TOKEN_EOF;
}

void Parser::parse_error(string *expected, Token *found) const {
  std::cerr << "Parse error: Expected: " << *expected <<
      ", Found:  " << *(found->to_string()) << std::endl;
  delete expected;
}

void Parser::advance() {
  delete word;
  word = lex->next_token();
  LOG("Parsing: " << *word->to_string());
}

void Parser::multiply_defined_identifier(string *id) const {
  cerr << "The identifier " << *id << " has already been declared. " << endl;
  exit(EXIT_FAILURE);
}

void Parser::undeclared_identifier(string *id) const {
  cerr << "The identifier " << *id << " has not been declared. " << endl;
  exit(EXIT_FAILURE);
}

void Parser::type_error(const expr_type expected, const expr_type found) const {
  cerr << "Type error: expected " << *(stab.type_to_string(expected))
       << " found " << *(stab.type_to_string(found)) << "." << endl;
  exit(EXIT_FAILURE);
}

void Parser::type_error(const expr_type expected1, const expr_type expected2,
                        const expr_type found) const {
  cerr << "Type error: expected " << *(stab.type_to_string(expected1))
       << " or " << *(stab.type_to_string(expected2))
       << ", found " << *(stab.type_to_string(found)) << "." << endl;
  exit(EXIT_FAILURE);
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

bool Parser::parse_program() {
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
    if (is_identifier(word)) {
      // Semantic analysis.
      string *id_name = static_cast<IdToken *>(word)->get_attribute();
      string *global_env_name = new string("_EXTERNAL");
      stab.install(id_name, global_env_name, PROGRAM_T);
      current_env = new string(*id_name);
      main_env = new string(*id_name);
      delete global_env_name;
      delete id_name;

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

          // Dump the content of symbol table.
          if (DEBUGMODE) stab.dump();

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
              parse_error(expected, word);
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
        parse_error(expected, word);
        return false;
      }

      // We failed to match an identifier
    } else {
      string *expected = new string("identifier");
      parse_error(expected, word);
      return false;
    }

    // We failed to match the keyword program
  } else {
    string *expected = new string("keyword program");
    parse_error(expected, word);
    return false;
  }

  /* We shouldn't reach this statement, but it is here as a defensive
     programming measure. */
  return false;
}

bool Parser::parse_decl_list() {
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

bool Parser::parse_variable_decl_list() {
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

bool Parser::parse_variable_decl() {
  /* VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE
     Predict(IDENTIFIER_LIST : STANDARD_TYPE) == First(IDENTIFIER_LIST)
     == {identifier} */
  if (is_identifier(word)) {
    LOG("VARIABLE_DECL -> IDENTIFIER_LIST : STANDARD_TYPE");

    // Match IDENTIFIER_LIST - ACTION.
    if (parse_identifier_list()) {

      // Match colon(:).
      if (is_punctuation(word, PUNC_COLON)) {

        expr_type standard_type_type = GARBAGE_T;

        // ADVANCE.
        advance();

        // Match STANDARD_TYPE - ACTION.
        if (parse_standard_type(standard_type_type)) {
          // Semantic analysis.
          stab.update_type(standard_type_type);
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

bool Parser::parse_procedure_decl_list() {
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

bool Parser::parse_identifier_list() {
  /* IDENTIFIER_LIST -> identifier IDENTIFIER_LIST_PRM
     Predict(identifier IDENTIFIER_LIST_PRM) == {identifier} */
  if (is_identifier(word)) {
    LOG("IDENTIFIER_LIST -> identifier IDENTIFIER_LIST_PRM");

    // Semantic analysis.
    string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
    if (stab.is_decl(identifier_attr, current_env)) {
      multiply_defined_identifier(identifier_attr);
    } else {
      stab.install(identifier_attr, current_env, UNKNOWN_T);
    }
    // delete identifier_attr;

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

bool Parser::parse_identifier_list_prm() {
  /* IDENTIFIER_LIST_PRM = , identifier IDENTIFIER_LIST_PRM
     Predict(, identifier IDENTIFIER_LIST_PRM) == {,} */
  if (is_punctuation(word, PUNC_COMMA)) {
    LOG("IDENTIFIER_LIST_PRM -> , identifier IDENTIFIER_LIST_PRM");

    // ADVANCE.
    advance();

    // Match an identifier.
    if (is_identifier(word)) {

      // Semantic analysis.
      string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
      if (stab.is_decl(identifier_attr, current_env)) {
        multiply_defined_identifier(identifier_attr);
      } else {
        if (parsing_formal_parm_list) {
          stab.install(identifier_attr, current_env, UNKNOWN_T,
                       formal_parm_position);
          ++formal_parm_position;
        } else {
          stab.install(identifier_attr, current_env, UNKNOWN_T);
        }
      }
      // delete identifier_attr;

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

bool Parser::parse_standard_type(expr_type& standard_type_type) {
  /* STANDARD_TYPE -> int
     Predict(int) = {int} */
  if (is_keyword(word, KW_INT)) {
    LOG("STANDARD_TYPE -> int");

    // Semantic analysis.
    standard_type_type = INT_T;
    LOG("standard_type_type: " << *stab.type_to_string(standard_type_type));

    // ADVANCE.
    advance();

    return true;

    /* STANDARD_TYPE -> bool
       Predict(bool) = {bool} */
  } else if (is_keyword(word, KW_BOOL)) {
    LOG("STANDARD_TYPE -> bool");

    // Semantic analysis.
    standard_type_type = BOOL_T;
    LOG("standard_type_type: " << *stab.type_to_string(standard_type_type));

    // ADVANCE.
    advance();

    return true;
  }

  return false;
}

bool Parser::parse_block() {
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

bool Parser::parse_procedure_decl() {
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

      // Semantic analysis.
      string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
      if (stab.is_decl(identifier_attr, current_env)) {
        multiply_defined_identifier(identifier_attr);
      } else {
        stab.install(identifier_attr, current_env, PROCEDURE_T);
        current_env = identifier_attr;
        formal_parm_position = 0;
      }

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
            if (parse_variable_decl_list() && parse_block()) {
              // Semantic analysis.
              current_env = main_env;
              return true;

              // Fail to match VARIABLE_DECL_LIST and BLOCK.
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

bool Parser::parse_procedure_args() {
  /* PROCEDURE_ARGS -> FORMAL_PARM_LIST
     Predict(FORMAL_PARM_LIST) == First(FORMAL_PARM_LIST) == {identifier} */
  if (is_identifier(word)) {
    LOG("PROCEDURE_ARGS -> FORMAL_PARM_LIST");

    parsing_formal_parm_list = true;

    // Match FORMAL_PARM_LIST - ACTION.
    if (parse_formal_parm_list()) {
      parsing_formal_parm_list = false;
      return true;

      // Fail to match FORMAL_PARM_LIST.
    } else {
      return false;
    }

    /* PROCEDURE_ARGS -> lambda */
  } else {
    LOG("PROCEDURE_ARGS -> lambda");
    return true;
  }

  return false;
}

bool Parser::parse_formal_parm_list() {
  /* FORMAL_PARM_LIST ->
     identifier IDENTIFIER_LIST_PRM : STANDARD_TYPE FORMAL_PARM_LIST_HAT
     Predict(LHS) = {identifier} */
  if (is_identifier(word)) {
    LOG("FORMAL_PARM_LIST -> identifier IDENTIFIER_LIST_PRM : "
        "STANDARD_TYPE FORMAL_PARM_LIST_HAT");

    // Semantic analysis.
    string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
    if (stab.is_decl(identifier_attr, current_env)) {
      multiply_defined_identifier(identifier_attr);
    } else {
      stab.install(identifier_attr, current_env, UNKNOWN_T,
                   formal_parm_position);
      ++formal_parm_position;
    }
    // delete identifier_attr;

    // ADVANCE.
    advance();

    // Match IDENTIFIER_LIST_PRM - ACTION.
    if (parse_identifier_list_prm()) {

      // Match colon(:).
      if (is_punctuation(word, PUNC_COLON)) {

        expr_type standard_type_type = GARBAGE_T;

        // ADVANCE.
        advance();

        // Match STANDARD_TYPE - ACTION.
        if (parse_standard_type(standard_type_type)) {
          // Semantic analysis.
          stab.update_type(standard_type_type);

          // Match FORMAL_PARM_LIST_HAT - ACTION.
          return parse_formal_parm_list_hat();

          // Fail to match STANDARD_TYPE.
        } else {
          return false;
        }

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

bool Parser::parse_formal_parm_list_hat() {
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

bool Parser::parse_stmt_list() {
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

bool Parser::parse_stmt_list_prm() {
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

bool Parser::parse_stmt() {
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

    // Semantic analysis.
    string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
    if (!stab.is_decl(identifier_attr, current_env)) {
      undeclared_identifier(identifier_attr);
    } else {
      procedure_name = identifier_attr;
    }

    // ADVANCE.
    advance();

    expr_type adhoc_as_pc_tail_type = GARBAGE_T;

    // Match ADHOC_AS_PC_TAIL - ACTION.
    if (parse_adhoc_as_pc_tail(adhoc_as_pc_tail_type)) {

      // Semantic analysis.
      expr_type identifier_type = stab.get_type(identifier_attr, current_env);
      if (adhoc_as_pc_tail_type != identifier_type) {
        type_error(identifier_type, adhoc_as_pc_tail_type);
      }
      return true;

      // Fail to match ADHOC_AS_PC_TAIL.
    } else {
      return false;
    }
  }

  return false;
}

bool Parser::parse_adhoc_as_pc_tail(expr_type& adhoc_as_pc_tail_type) {
  /* ADHOC_AS_PC_TAIL -> := EXPR
     Predict(:= EXPR) == {:=} */
  if (is_punctuation(word, PUNC_ASSIGN)) {
    LOG("ADHOC_AS_PC_TAIL -> := EXPR");

    expr_type expr_type_result = GARBAGE_T;

    // ADVANCE.
    advance();

    // Match EXPR - ACTION.
    if (parse_expr(expr_type_result)) {
      // Semantic analysis.
      adhoc_as_pc_tail_type = expr_type_result;
      LOG("adhoc_as_pc_tail_type: "
          << *stab.type_to_string(adhoc_as_pc_tail_type));
      return true;

      // Fail to match EXPR.
    } else {
      return false;
    }

    /* ADHOC_AS_PC_TAIL -> ( EXPR_LIST )
       Predict( ( EXPR_LIST ) ) = { ( } */
  } else if (is_punctuation(word, PUNC_OPEN)) {
    LOG("ADHOC_AS_PC_TAIL -> ( EXPR_LIST )");

    // Semantic analysis.
    expr_type procedure_type = stab.get_type(procedure_name, main_env);
    if (procedure_type != PROCEDURE_T) {
      type_error(PROCEDURE_T, procedure_type);
    }
    actual_parm_position = 0;

    // ADVANCE.
    advance();

    // Match EXPR_LIST - ACTION.
    if (parse_expr_list()) {

      // Match closing bracket.
      if (is_punctuation(word, PUNC_CLOSE)) {

        // Semantic analysis.
        adhoc_as_pc_tail_type = PROCEDURE_T;
        LOG("adhoc_as_pc_tail_type: "
            << *stab.type_to_string(adhoc_as_pc_tail_type));

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

bool Parser::parse_if_stmt() {
  /* IF_STMT -> if EXPR then BLOCK IF_STMT_HAT
     Predict(...) = {if} */
  if (is_keyword(word, KW_IF)) {
    LOG("IF_STMT -> if EXPR then BLOCK IF_STMT_HAT");

    // ADVANCE.
    advance();

    expr_type expr_type_result = GARBAGE_T;

    // Match EXPR - ACTION.
    if (parse_expr(expr_type_result)) {

      // Semantic analysis.
      if (expr_type_result != BOOL_T) {
        type_error(BOOL_T, expr_type_result);
      }

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

bool Parser::parse_if_stmt_hat() {
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

bool Parser::parse_while_stmt() {
  /* WHILE_STMT -> while EXPR loop BLOCK
     Predict(while EXPR loop BLOCK) = {while} */
  if (is_keyword(word, KW_WHILE)) {
    LOG("WHILE_STMT -> while EXPR loop BLOCK");

    // ADVANCE.
    advance();

    expr_type expr_type_result = GARBAGE_T;

    // Match EXPR - ACTION.
    if (parse_expr(expr_type_result)) {

      // Semantic analysis.
      if (expr_type_result != BOOL_T) {
        type_error(BOOL_T, expr_type_result);
      }

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

bool Parser::parse_print_stmt() {
  /* PRINT_STMT -> print EXPR
     Predict(print EXPR) == {print} */
  if (is_keyword(word, KW_PRINT)) {
    LOG("PRINT -> print EXPR");

    // ADVANCE.
    advance();

    expr_type expr_type_result = GARBAGE_T;

    // Match EXPR - ACTION.
    if (parse_expr(expr_type_result)) {
      if (expr_type_result != INT_T && expr_type_result != BOOL_T) {
        type_error(INT_T, BOOL_T, expr_type_result);
      }
      return true;

      // Fail to match EXPR.
    } else {
      return false;
    }

    // Fail to match keyword print.
  } else {
    parse_error(new string("print"), word);
    return false;
  }

  return false;
}

bool Parser::parse_expr_list() {
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

bool Parser::parse_actual_parm_list() {
  /* ACTUAL_PARM_LIST -> EXPR ACTUAL_PARM_LIST_HAT */
  LOG("ACTUAL_PARM_LIST -> EXPR ACTUAL_PARM_LIST_HAT");

  expr_type expr_type_result = GARBAGE_T;

  // Match EXPR - ACTION.
  if (parse_expr(expr_type_result)) {
    expr_type expected_type =
        stab.get_type(procedure_name, actual_parm_position);
    if (expr_type_result != expected_type) {
      type_error(expected_type, expr_type_result);
    }
    ++actual_parm_position;

    // Match ACTUAL_PARM_LIST_HAT - ACTION.
    return parse_actual_parm_list_hat();
  }

  return false;
}

bool Parser::parse_actual_parm_list_hat() {
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

bool Parser::parse_expr(expr_type& expr_type_result) {
  /* EXPR -> SIMPLE_EXPR EXPR_HAT */
  LOG("EXPR -> SIMPLE_EXPR EXPR_HAT");

  expr_type simple_expr_type = GARBAGE_T;
  expr_type expr_hat_type = GARBAGE_T;

  // Match SIMPLE_EXPR and EXPR_HAT - ACTION.
  if (parse_simple_expr(simple_expr_type) && parse_expr_hat(expr_hat_type)) {
    // Semantic analysis.
    if (expr_hat_type == NO_T) {
      expr_type_result = simple_expr_type;
    } else if (simple_expr_type == INT_T && expr_hat_type == INT_T) {
      expr_type_result = BOOL_T;
    } else {
      type_error(INT_T, simple_expr_type, expr_hat_type);
    }
    LOG("expr_type_result: " << *stab.type_to_string(expr_type_result));

    return true;
  }

  return false;
}

bool Parser::parse_expr_hat(expr_type& expr_hat_type) {
  /* EXPR_HAT -> relop SIMPLE_EXPnR
     Predict(relop SIMPLE_EXPR) = {relop} */
  if (is_relop(word)) {
    LOG("EXPR_HAT -> relop SIMPLE_EXPR");

    // ADVANCE.
    advance();

    expr_type simple_expr_type = GARBAGE_T;

    // Match SIMPLE_EXPR - ACTION.
    if (parse_simple_expr(simple_expr_type)) {
      // Semantic analysis.
      if (simple_expr_type == INT_T) {
        expr_hat_type = INT_T;
      } else {
        type_error(INT_T, simple_expr_type);
      }
      LOG("EXPR_HAT_TYPE: " << *stab.type_to_string(expr_hat_type));
      return true;

      // Fail to match SIMPLE_EXPR.
    } else {
      return false;
    }

    /* EXPR_HAT -> lambda */
  } else {
    LOG("EXPR_HAT -> lambda");
    LOG("EXPR_HAT_TYPE: " << *stab.type_to_string(expr_hat_type));
    expr_hat_type = NO_T;
    return true;
  }

  return false;
}

bool Parser::parse_simple_expr(expr_type& simple_expr_type) {
  /* SIMPLE_EXPR -> TERM SIMPLE_EXPR_PRM */
  LOG("SIMPLE_EXPR -> TERM SIMPLE_EXPR_PRM");

  expr_type term_type = GARBAGE_T;
  expr_type simple_expr_prm_type = GARBAGE_T;

  // Match TERM and SIMPLE_EXPR_PRM - ACTION.
  if (parse_term(term_type) && parse_simple_expr_prm(simple_expr_prm_type)) {
    // Semantic analysis.
    if (simple_expr_prm_type == NO_T) {
      simple_expr_type = term_type;
    } else if (term_type == simple_expr_prm_type) {
      simple_expr_type = term_type;
    } else {
      type_error(term_type, simple_expr_prm_type);
    }
    LOG("simple_expr_type: " << *stab.type_to_string(simple_expr_type));

    return true;
  }

  // Fail to match TERM and SIMPLE_EXPR_PRM.
  return false;
}

bool Parser::parse_simple_expr_prm(expr_type& simple_expr_prm0_type) {
  /* SIMPLE_EXPR_PRM -> addop TERM SIMPLE_EXPR_PRM
     Predict(addop TERM SIMPLE_EXPR_PRM) == {addop} */
  if (is_addop(word)) {
    LOG("SIMPLE_EXPR_PRM -> addop TERM SIMPLE_EXPR_PRM");

    expr_type addop_type = GARBAGE_T;
    addop_attr_type addop_attr =
        static_cast<AddopToken*>(word)->get_attribute();
    if (addop_attr == ADDOP_ADD || addop_attr == ADDOP_SUB) {
      addop_type = INT_T;
    } else {
      addop_type = BOOL_T;
    }
    expr_type term_type = GARBAGE_T;
    expr_type simple_expr_prm1_type = GARBAGE_T;

    // ADVANCE.
    advance();

    // Match TERM and SIMPLE_EXPR_PRM - ACTION.
    if (parse_term(term_type) && parse_simple_expr_prm(simple_expr_prm1_type)) {
      // Semantic analysis.
      if (simple_expr_prm1_type == NO_T) {
        if (addop_type == term_type) {
          simple_expr_prm0_type = addop_type;
        } else {
          type_error(addop_type, term_type);
        }
      } else if (addop_type == term_type &&
                 term_type == simple_expr_prm1_type) {
        simple_expr_prm0_type = addop_type;
      } else {
        type_error(addop_type, term_type, simple_expr_prm1_type);
      }

      return true;

      // Fail to match TERM and SIMPLE_EXPR_PRM.
    } else {
      return false;
    }

    /* SIMPLE_EXPR_PRM -> lambda */
  } else {
    LOG("SIMPLE_EXPR_PRM -> lambda");
    simple_expr_prm0_type = NO_T;
    LOG("simple_expr_prm_type: " <<
        *stab.type_to_string(simple_expr_prm0_type));
    return true;
  }

  return false;
}

bool Parser::parse_term(expr_type& term_type) {
  /* TERM -> FACTOR TERM_PRM */
  LOG("TERM -> FACTOR TERM_PRM");

  expr_type factor_type = GARBAGE_T;
  expr_type term_prm_type = GARBAGE_T;

  // Match FACTOR - ACTION.
  if (parse_factor(factor_type)) {
    // Match TERM_PRM - ACTION.
    if (parse_term_prm(term_prm_type)) {

      // Semantic analysis.
      if (term_prm_type == NO_T) {
        term_type = factor_type;
      } else if (factor_type == term_prm_type) {
        term_type = factor_type;
      } else {
        type_error(factor_type, term_prm_type);
      }
      LOG("term_type: " << *stab.type_to_string(term_type));

      return true;

      // Fail to match TERM_PRM.
    } else {
      return false;
    }

    // Fail to match FACTOR.
  } else {
    return false;
  }

  return false;
}

bool Parser::parse_term_prm(expr_type& term_prm0_type) {
  /* TERM_PRM -> mulop FACTOR TERM_PRM
     Predict(mulop FACTOR TERM_PRM) = {mulop} */
  if (is_mulop(word)) {
    LOG("TERM_PRM -> mulop FACTOR TERM_PRM");

    // Semantic analysis.
    expr_type mulop_type = GARBAGE_T;
    mulop_attr_type mulop_attr = MULOP_NO_ATTR;
    mulop_attr = static_cast<MulopToken *>(word)->get_attribute();
    if (mulop_attr == MULOP_MUL || mulop_attr == MULOP_DIV) {
      mulop_type = INT_T;
    } else {
      mulop_type = BOOL_T;
    }
    expr_type factor_type = GARBAGE_T;
    expr_type term_prm1_type = GARBAGE_T;

    // ADVANCE.
    advance();

    // Match FACTOR and TERM_PRM - ACTION.
    if (parse_factor(factor_type) && parse_term_prm(term_prm1_type)) {
      // Semantic analysis.
      if (term_prm1_type == NO_T && mulop_type == factor_type) {
        term_prm0_type = mulop_type;
      } else if (mulop_type == factor_type && factor_type == term_prm1_type) {
        term_prm0_type = mulop_type;
      } else if (term_prm1_type == NO_T) {
        type_error(mulop_type, factor_type);
      } else {
        type_error(mulop_type, factor_type, term_prm1_type);
      }

      return true;

      // Fail to match FACTOR and TERM_PRM.
    } else {
      return false;
    }

    /* TERM_PRM -> lambda */
  } else {
    LOG("TERM_PRM -> lambda");
    term_prm0_type = NO_T;
    return true;
  }

  return false;
}

bool Parser::parse_factor(expr_type& factor0_type) {
  /* FACTOR -> identifier
     Predict(identifier) = {identifier} */
  if (is_identifier(word)) {
    LOG("FACTOR -> identifier");

    // Semantic analysis.
    string* identifier_attr = static_cast<IdToken*>(word)->get_attribute();
    if (!stab.is_decl(identifier_attr, current_env)) {
      undeclared_identifier(identifier_attr);
    } else {
      factor0_type = stab.get_type(identifier_attr, current_env);
    }
    // delete identifier_attr;
    LOG("factor_type: " << *stab.type_to_string(factor0_type));

    // ADVANCE.
    advance();

    return true;

    /* FACTOR -> num
       Predict(num) = {num} */
  } else if (is_number(word)) {
    LOG("FACTOR -> number");

    // Semantic analysis.
    factor0_type = INT_T;
    LOG("factor_type: " << *stab.type_to_string(factor0_type));

    // ADVANCE.
    advance();

    return true;

    /* FACTOR -> ( EXPR )
       Predict( ( EXPR ) ) == { ( } */
  } else if (is_punctuation(word, PUNC_OPEN)) {
    LOG("FACTOR -> ( EXPR )");

    // ADVANCE.
    advance();

    expr_type expr_type_result = GARBAGE_T;

    // Match EXPR.
    if (parse_expr(expr_type_result)) {

      // Match close bracket.
      if (is_punctuation(word, PUNC_CLOSE)) {
        // Semantic analysis.
        factor0_type = expr_type_result;
        LOG("factor_type: " << *stab.type_to_string(factor0_type));

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

    expr_type sign_type = GARBAGE_T;
    expr_type factor1_type = GARBAGE_T;

    // Match SIGN and FACTOR.
    if (parse_sign(sign_type) && parse_factor(factor1_type)) {
      if (sign_type != factor1_type) {
        type_error(sign_type, factor1_type);
      } else {
        factor0_type = factor1_type;
      }
      LOG("factor_type: " << *stab.type_to_string(factor0_type));
      return true;

      // Fail to match SIGN and FACTOR.
    } else {
      return false;
    }
  }

  return false;
}

bool Parser::parse_sign(expr_type& sign_type) {
  /* SIGN -> +
     Predict(SIGN) == {+} */
  if (is_addop(word, ADDOP_ADD)) {
    LOG("SIGN -> +");

    // Semantic analysis.
    sign_type = INT_T;

    // ADVANCE.
    advance();

    return true;

    /* SIGN -> -
       Predict(-) == {-) */
  } else if (is_addop(word, ADDOP_SUB)) {
    LOG("SIGN -> -");

    // Semantic analysis.
    sign_type = INT_T;

    // ADVANCE.
    advance();

    return true;

    /* SIGN -> not
       Predict(not) == {not} */
  } else if (is_keyword(word, KW_NOT)) {
    LOG("SIGN -> not");

    // Semantic analysis.
    sign_type = BOOL_T;

    // ADVANCE.
    advance();

    return true;
  }

  return false;
}
