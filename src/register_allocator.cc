// Implementation of RegisterAllocator class.
// @author Hieu Le
// @version 12/05/2016

#include "register_allocator.h"

Register_Allocator::Register_Allocator() {
  for (int i = 0; i < N_REGS; i++) {
    register_set[i] = new Register(i);
  }

  // Reserve the stack register. It can not be used except to access the
  // procedure call stack (which we will not be doing.)
  register_set[STACK_REG]->set_inuse();
}

Register_Allocator::~Register_Allocator() {
  for (int i = 0; i < N_REGS; i++) {
    delete register_set[i];
  }
}

// Find a free register and return it.
Register *Register_Allocator::allocate_register() {
  // Iterate over all the registers, looking for one that has
  // not been allocated.
  for (int i = 0; i < N_REGS; i++) {
    if (!register_set[i]->is_inuse()) {
      register_set[i]->set_inuse();
      return register_set[i];
    }
  }
  cout << "NO FREE REGISTERS!!!";
  return NULL;
}

// Make a previously allocated register available for reallocation.
void Register_Allocator::deallocate_register(Register *reg) {
  if (!register_set[reg->get_num()]->is_inuse()) {
    // Ooops, your intermediate code generator is broken.
    freeing_unallocated_register(reg);
  }
  register_set[reg->get_num()]->clear_inuse();
}


void Register_Allocator::freeing_unallocated_register(Register *reg) {
  cout << "Attempt to free unallocated register number ";
  cout << reg->get_num() << endl;
}
