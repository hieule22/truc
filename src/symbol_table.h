// Symbol table for TruPL compiler.
// @author Hieu Le
// @version 11/11/2016

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* Types of expressions that the semantic analyzer must
   manipulate.  These are stored in the symbol table
   or returned by parser functions as an attribute
   of a non-terminal.
*/
typedef enum expression_type { INT_T       = 700,  // int
                               BOOL_T      = 701,  // bool
                               PROGRAM_T   = 702,  // program name
                               PROCEDURE_T = 703,  // procedure name
                               UNKNOWN_T   = 704,  // unknown type
                               NO_T        = 705,  // no associated type
                               GARBAGE_T   = 799 } expr_type;  // default value

class Symbol_Table {
 public:
  Symbol_Table();

  ~Symbol_Table();

  /* 
     These are the methods that I used to manipulate the symbol table in
     my compiler.  You may or may not use all of these in yours, and you
     may add others as well.
  */

  /* Install an identifier in the symbol table if the environment
     and type are known. */
  void install(const string *id, const string *environment,
               const expr_type t); /*U*/

  /* Install a formal parameter in the symbol table. */
  void install(const string *id, const string *environment,
               const expr_type t, const int position);

  /* Has an identifier been defined in this environment? */
  bool is_decl(const string *id, const string *environment);

  /* Get the type of an identifier in a given environment. Used
     when determining whether an expression or statment
     is semantically correct. */
  expr_type get_type(string *id, string *environment);

  /* Get the type of the formal parameter in the indicated position of
     the procedure proc_id. */
  expr_type get_type(string *proc_id, const int position);

  /* Iterate over symbol table, updating all entries with
     type == UNKNOWN_T to standard_type_type. */
  void update_type(const expr_type standard_type_type);

  // Convert an expr_type to a string - useful for debugging.
  string *type_to_string(const expr_type t) const;

  // Dump the content of symbol table to console - useful for debugging.
  void dump() const;

 private:
  // One symbol table entry.
  typedef struct stab_entry {
    string id;    // Identifier name
    string env;   // Environment this id declared in
    int position;  // Position in formal parameter list if
                   // this id is a parm.  Undefined otherwise.
    expr_type type;  // Data type of this id.
  } STAB_ENTRY;

  // Dump an entry from symbol table.
  void dump_entry(const STAB_ENTRY& entry) const;

  // The storage for the symbol table itself.
  vector<STAB_ENTRY> stab;
};

#endif
