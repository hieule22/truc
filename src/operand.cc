// Implementation of Operand class.
// @author Hieu Le
// @version 12/05/2016

#include "operand.h"

Operand::Operand(const op_type_type o_type, const int val)
    : op_type(o_type), i_value(val), r_value(nullptr), m_value(nullptr) {}

Operand::Operand(const op_type_type o_type, const string *memory_location_name)
    : op_type(o_type), i_value(OPTYPE_GARBAGE_I_VALUE), r_value(nullptr),
      m_value(new string(*memory_location_name)) {}

Operand:: Operand(const op_type_type o_type, Register *reg)
    : op_type(o_type), i_value(OPTYPE_GARBAGE_I_VALUE), r_value(reg),
      m_value(nullptr) {}

Operand::~Operand() {
  if (m_value != NULL) {
    delete m_value;
  }
}

op_type_type Operand::get_type() const {
  return op_type;
}

int Operand::get_i_value() const {
  // Sanity check
  switch (op_type) {
    case OPTYPE_IMMEDIATE:
      return i_value;
      break;

    case OPTYPE_MEMORY:
      bad_op_request("int value of memory operand");
      break;

    case OPTYPE_REGISTER:
      bad_op_request("int value of register operand");
      break;

    case OPTYPE_GARBAGE:
      bad_op_request("int value of garbage type operand");
      break;

    default:
      bad_op_request("int value of undefined type operand");
      break;
  }
  return OPTYPE_GARBAGE_I_VALUE;
}

Register *Operand::get_r_value() const {
  // Sanity check
  switch (op_type) {
    case OPTYPE_REGISTER:
      if (r_value != NULL) {
        return r_value;
      } else {
        bad_op_request("register is null");
      }
      break;

    case OPTYPE_IMMEDIATE:
      bad_op_request("register value of immediate operand");
      break;

    case OPTYPE_MEMORY:
      bad_op_request("register value of memory operand");
      break;

    case OPTYPE_GARBAGE:
      bad_op_request("register value of garbage operand");
      break;

    default:
      bad_op_request("register valueof undefined type operand");
      break;
  }

  return NULL;
}


string *Operand::get_m_value() const {
  // Sanity check
  switch (op_type) {
    case OPTYPE_MEMORY:
      if (m_value != NULL) {
        return new string (*m_value);
      } else {
        bad_op_request("name of m operand is undefined");
      }
      break;

    case OPTYPE_IMMEDIATE:
    case OPTYPE_REGISTER:
      bad_op_request("name of immediate or register op");
      break;

    case OPTYPE_GARBAGE:
      bad_op_request("name of garbage type operand");
      break;

    default:
      bad_op_request("name of undefined operand");
      break;
  }
  return NULL;
}

void Operand::bad_op_request(const char *message) const {
  cout << "Operand error: requested " << message << endl;
  return;
}
