// Representation of an operand from the TrAL instruction set.
// @author Hieu Le
// @version 12/05/2016

#ifndef OPERAND_H
#define OPERAND_H

#include <iostream>
#include <string>

#include "register.h"

using namespace std;

/* We will use three types of TrAL operands: immediate,
   register direct, and memory direct.  
*/
typedef enum OPERAND_TYPE { OPTYPE_IMMEDIATE = 900,
                            OPTYPE_REGISTER  = 901,
                            OPTYPE_MEMORY    = 902,
                            OPTYPE_GARBAGE   = 999 } op_type_type;

#define OPTYPE_GARBAGE_I_VALUE 99999999

class Operand {
 public:
  Operand(const op_type_type type, const int val);
  Operand(const op_type_type type, const string *memory_location_name);
  Operand(const op_type_type type, Register *reg);
  ~Operand();

  op_type_type get_type() const;

  int get_i_value() const;

  Register *get_r_value() const;

  string *get_m_value() const;

 private:
  // The type of this operand.
  op_type_type op_type;

  /* Depending on the type of the operand, it will either have an
     immediate value, be in a register or be in memory. */
  int i_value;
  Register *r_value;
  string *m_value;

  void bad_op_request(const char *message) const;
};

#endif
