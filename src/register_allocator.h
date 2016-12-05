// Register allocator manages Register objects.
// @author Hieu Le
// @version 12/05/2016

#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

// The total number of registers.
#define N_REGS 4
// R3 is reserved.
#define STACK_REG 3

#include <stdlib.h>

#include <iostream>

#include "register.h"

using namespace std;

class Register_Allocator {
 public:
  Register_Allocator();

  ~Register_Allocator();

  // Gets an unused register.
  Register *allocate_register();

  // Frees a previously allocated register.
  void deallocate_register(Register *r);

 private:
  // The register set we can allocate from.
  Register *register_set[N_REGS];

  // Internal error routine.
  void freeing_unallocated_register(Register *reg);
};

#endif
