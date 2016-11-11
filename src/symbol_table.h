#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

/* Types of expressions that the semantic analyzer must
   manipulate.  These are stored in the symbol table
   or returned by parser functions as an attribute
   of a non-terminal.
*/
typedef enum expression_type {INT_T       = 700,  // int
			      BOOL_T      = 701,  // bool
			      PROGRAM_T   = 702,  // program name
			      PROCEDURE_T = 703,  // procedure name
			      UNKNOWN_T   = 704,  // has a type but it is currently unknown
			      NO_T        = 705,  // no type associated with this attribute
			      GARBAGE_T   = 799} expr_type; // initial value

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
  void install (const string *id, const string *environment, 
		const expr_type t); /*U*/
  
  /* Install a formal parameter in the symbol table. */
  void install (const string *id, const string *environment,
		const expr_type t, const int position); /*U*/
  
  /* Has an identifier been defined in this environment? */
  bool is_decl (const string *id, const string *environment); /*U*/
  
  /* Get the type of an identifier in a given environment. Used
     when determining whether an expression or statment
     is semantically correct. */
  expr_type get_type (string *id, string *environment); /*U*/

  /* Get the type of the formal parameter in the indicated position of
     the procedure proc_id declared in the indicated environment. */
  expr_type get_type (string *proc_id, string *environment, 
		      const int position); /*U*/
  
  /* Iterate over symbol table, updating all entries with 
     type == UNKNOWN_T to standard_type_type. */  
  void update_type (const expr_type standard_type_type); /*U*/
  
  // Convert an expr_type to a string - useful for debugging.
  string *type_to_string (const expr_type t) const;
  
 private:
  
  // One symbol table entry.
  typedef struct stab_entry {
    string id;    // Identifier name
    string env;   // Environment this id declared in
    int position;  // Position in formal parameter list if 
                   //   this id is a parm.  Undefined otherwise.
    expr_type type;  // Data type of this id.
  } STAB_ENTRY;

  // The storage for the symbol table itself.
  vector<STAB_ENTRY> stab;
};

#endif
