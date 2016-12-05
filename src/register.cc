// Implementation of Register class.
// @author Hieu Le
// @version 12/05/2016

#include "register.h"

Register::Register(int num) {
  this->num = num;
  inuse = false;
  // Initialize the spill stack for this register.
  spill_stack = new stack<string *>;
}

Register::~Register() {
  delete spill_stack;
}

int Register::get_num() const {
  return num;
}

void Register::set_inuse() {
  inuse = true;
}

void Register::clear_inuse() {
  inuse = false;
}

bool Register::is_inuse() const {
  return inuse;
}

// We have to spill this register.  Push the memory location where it
// was spilled onto the register's spill stack.
void Register::spill(string *location) {
  spill_stack->push(location);
}

// This register was previously spilled, and is now free.  Restore its
// contents from the memory location where we saved it.
string *Register::unspill() {
  if (spill_stack->empty()) {
    cout << "Error: popping empty spill_stack." << endl;
    exit(EXIT_FAILURE);
  }

  string *location = new string (*(spill_stack->top()));
  spill_stack->pop();
  return location;
}

// Does this register have any spilled contents in memory?
bool Register::has_spilled() const {
  return (!spill_stack->empty());
}
