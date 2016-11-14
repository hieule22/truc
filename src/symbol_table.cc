// Implementation of symbol table.
// @author Hieu Le
// @version 11/11/2016

#include "symbol_table.h"

Symbol_Table::Symbol_Table() {}

Symbol_Table::~Symbol_Table() {}

void Symbol_Table::install(const string *id, const string *env,
                           const expr_type t) {
  /* Install an identifier from environment env with type t into
     symbol table.  Does not check for duplicates. */

  STAB_ENTRY *new_entry = new STAB_ENTRY;
  new_entry->id = *id;
  new_entry->env = *env;
  new_entry->position = -1;
  new_entry->type = t;
  stab.push_back(*new_entry);
  cout << "Installing new entry in symbol table." << endl;
  dump_entry(*new_entry);
}

void Symbol_Table::install(const string *id, const string *env,
                           const expr_type t, const int pos) {
  /* Install an identifier from environment env with type t into
     symbol table.  Does not check for duplicates. */

  STAB_ENTRY *new_entry = new STAB_ENTRY;
  new_entry->id = *id;
  new_entry->env = *env;
  new_entry->position = pos;
  new_entry->type = t;
  stab.push_back(*new_entry);
  cout << "Installing new entry in symbol table." << endl;
  dump_entry(*new_entry);
}

bool Symbol_Table::is_decl(const string *id, const string *env) {
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->id.compare(*id) == 0 && it->env.compare(*env) == 0) {
      return true;
    }
  }
  return false;
}

expr_type Symbol_Table::get_type(string *id, string *env) {
  // Return the type of identifier id of environment env.  Results in
  // garbage garbage type if (*id, *env) are not in the table.
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->id.compare(*id) == 0 && it->env.compare(*env) == 0) {
      return it->type;
    }
  }

  return GARBAGE_T;
}

expr_type Symbol_Table::get_type(string *proc_id, const int pos) {
  /* Get the type of the formal parameter in the indicated position of
     the procedure proc_id. */
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->env.compare(*proc_id) == 0
        && it->position == pos) {
      return it->type;
    }
  }

  return GARBAGE_T;
}

void Symbol_Table::update_type(expr_type standard_type_type) {
  /* Change the type of all symbol table variables with type UNKNOWN_T
     to standard_type_type. */
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->type == UNKNOWN_T) {
      it->type = standard_type_type;
      cout << "Updating existing entry in symbol table." << endl;
      dump_entry(*it);
    }
  }
}

string *Symbol_Table::type_to_string(const expr_type t) const {
  string *result;

  switch (t) {
  case INT_T:
    result = new string ("INT_T");
    break;
  case BOOL_T:
    result = new string ("BOOL_T");
    break;
  case PROCEDURE_T:
    result = new string ("PROCEDURE_T");
    break;
  case PROGRAM_T:
    result = new string ("PROGRAM_T");
    break;
  case NO_T:
    result = new string ("NO_T");
    break;
  case UNKNOWN_T:
    result = new string ("UNKNOWN_T");
    break;
  default:
    result = new string ("GARBAGE_T");
    break;
  }

  return result;
}

void Symbol_Table::dump_entry(const STAB_ENTRY& entry) const {
    cout << "ID: " << entry.id << endl;
    cout << "ENV: " << entry.env << endl;
    cout << "POS: " << entry.position << endl;
    cout << "TYPE: " << *type_to_string(entry.type) << endl;
    cout << endl;
}

void Symbol_Table::dump() const {
  cout << "Content of symbol table." << endl;
  for (auto iter = stab.begin(); iter != stab.end(); ++iter) {
    dump_entry(*iter);
  }
}
