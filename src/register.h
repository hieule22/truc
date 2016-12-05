// Representation of a register from TrAL architecture.
// @author Hieu Le
// @version 12/05/2016

#ifndef REGISTER_H
#define REGISTER_H

#include <stdlib.h>

#include <iostream>
#include <stack>
#include <string>

using namespace std;

// This class describes a single register.  You should not manipulate
// register objects directly.
//
// Instead, call the methods in the RegisterAllocator class to
// allocate and deallocate registers.
class Register {
 public:
  // Constructs a register with a specified number.
  explicit Register(int num);

  ~Register();

  // Returns the number of this register.
  int get_num() const;

  // Marks the register as inuse and unavailable for allocation.
  void set_inuse();

  // Marks the register as free and available for allocation.
  void clear_inuse();

  // Checks if this register has been allocated.
  bool is_inuse() const;

  // Pushes a specified memory location onto the register's spill stack.
  void spill(string *location);

  // Restores the content of the register from the memory relocation
  // before spilling.
  string *unspill();

  // Checks if this register has any spilled content in memory.
  bool has_spilled() const;

 private:
  // The register's number.
  int num;
  // The flag indicating if this register is currently in use.
  bool inuse;
  // The register's spill stack.
  stack<string *> *spill_stack;
};

#endif
