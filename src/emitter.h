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
  void emit_label(string *label);

  /* Instruction handling. */

  /* The first set handles the move to register instrucitons. */
  // For immediate mode.
  void emit_move(Register *reg, int immedidate);
  // For register direct mode.
  void emit_move(Register *reg, Register *regd);
  // For memory direct mode.
  void emit_move(Register *reg, string *var);

  // Emit instructions of the form "move dest, Rn".

  // Here is reg to memory move.
  void emit_move(string *var, Register *reg);

  /* All the other two-address instructions are handled here.
     The first address is always a register. */

  // To output "add R0, foovar", call
  // emit_2addr (ADD, <pointer to object for register 0>,
  //             <pointer to string containing "foovar">)
  void emit_2addr(inst_type inst, Register *reg, int immediate);
  void emit_2addr(inst_type inst, Register *reg, Register *src);
  void emit_2addr(inst_type inst, Register *reg, string *var);

  /* One address instructions. */

  void emit_1addr(inst_type inst, Register *reg);

  /* Branch instructions. */

  // For brun.
  void emit_branch(string *dest);
  // For the conditional branches with immmediate mode targets.
  void emit_branch(inst_type inst, Register *reg, int dest);
  // For conditional branches that target labels
  void emit_branch(inst_type inst, Register *reg, string *dest);

  /* Halt instruction. */
  void emit_halt();

  /* Data directives. */
  void emit_data_directive(string *label, int size);
  void emit_data_directive(int size);

  /* If you want your compiler to add comments to your Tral program,
     this is the ticket. Hmm, compilers that write comments! */
  void emit_comment(const char comment[]);

 private:
  // The current unique number used to generate each label.
  unsigned int label_num;

  // Convert an unsigned int to the corresponding ASCII string.
  string *itos(unsigned int i);

  // Emit an instruction mnemonic.
  void translate_and_emit(inst_type inst);
};

#endif
