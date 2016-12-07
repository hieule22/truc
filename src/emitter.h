// Emitter class.
// @author Hieu Le
// @version 12/05/2016

#ifndef EMITTER_H
#define EMITTER_H

#include <iostream>
// Used in itos function.
#include <sstream>
#include <string>

#include "register.h"

using namespace std;

// Instruction mnemonics.
typedef enum instruction_type { INST_MOVE = 802,
                                INST_ADD =  803,
                                INST_SUB =  804,
                                INST_MUL =  805,
                                INST_DIV =  806,
                                INST_NEG =  807,
                                INST_NOT =  808,
                                INST_LEA =  809,
                                INST_BRUN = 810,
                                INST_BREZ = 811,
                                INST_BRPO = 812,
                                INST_BRNE = 813,
                                INST_OUTB = 814,
                                INST_HALT = 815,
                                INST_GARBAGE = 899} inst_type;

class Emitter {
 public:
  Emitter();
  ~Emitter();

  /* Label handling. */

  // Generates a new, unique label
  string *get_new_label();

  // Generates a new, unique label with a specific prefix.
  // Useful for debugging or self- documenting the assembly language code.
  string *get_new_label(const char prefix[]);

  // Outputs a previously generated label.
  void emit_label(const string *label) const;

  /* Instruction handling. */

  /* The first set handles the move to register instrucitons. */
  // For immediate mode.
  void emit_move(const Register *reg, int immedidate) const;
  // For register direct mode.
  void emit_move(const Register *reg, const Register *regd) const;
  // For memory direct mode.
  void emit_move(const Register *reg, const string *var) const;

  // Emit instructions of the form "move dest, Rn".

  // Here is reg to memory move.
  void emit_move(const string *var, const Register *reg) const;

  /* All the other two-address instructions are handled here.
     The first address is always a register. */

  // To output "add R0, foovar", call
  // emit_2addr (ADD, <pointer to object for register 0>,
  //             <pointer to string containing "foovar">)
  void emit_2addr(inst_type inst, const Register *reg, int immediate) const;
  void emit_2addr(inst_type inst, const Register *reg,
                  const Register *src) const;
  void emit_2addr(inst_type inst, const Register *reg, const string *var) const;

  /* One address instructions. */

  void emit_1addr(inst_type inst, const Register *reg) const;

  /* Branch instructions. */

  // For brun.
  void emit_branch(const string *dest) const;
  // For the conditional branches with immmediate mode targets.
  void emit_branch(inst_type inst, const Register *reg, int dest) const;
  // For conditional branches that target labels
  void emit_branch(inst_type inst, const Register *reg,
                   const string *dest) const;

  /* Halt instruction. */
  void emit_halt() const;

  /* Data directives. */
  void emit_data_directive(const string *label, int size) const;
  void emit_data_directive(int size) const;

  /* If you want your compiler to add comments to your Tral program,
     this is the ticket. Hmm, compilers that write comments! */
  void emit_comment(const char comment[]) const;

 private:
  // The current unique number used to generate each label.
  unsigned int label_num;

  // Convert an unsigned int to the corresponding ASCII string.
  string *itos(unsigned int i) const;

  // Emit an instruction mnemonic.
  void translate_and_emit(inst_type inst) const;
};

#endif
