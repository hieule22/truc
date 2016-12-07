// Implementation of Register class.
// @author Hieu Le
// @version 12/05/2016

#include "register.h"

Register::Register(const int number) : num(number), inuse(false) {}

Register::~Register() {}

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
