// Implementation of Emitter class.
// @author Hieu Le
// @version 12/05/2016

#include "emitter.h"

Emitter::Emitter() {
  label_num = 0;
}

Emitter::~Emitter() {}

// Return a label of the form "_Lz", where z is guaranteed
// to be a unique number.
string *Emitter::get_new_label() {
  string *label = new string("_L");
  string *number = itos(label_num);
  label_num++;
  *label += *number;
  delete number;
  return label;
}

// Return a label of the form "_prefixn", where n is guaranteed
// to be a unique number.
string *Emitter::get_new_label(const char prefix[]) {
  string *label = new string("_");
  string *number = itos(label_num);
  label_num++;
  *label = *label + prefix + *number;
  delete number;
  return label;
}

// Convert an int to its string equivalent.  Idea shamelessly
// stolen from Java.
string *Emitter::itos(unsigned int num) const {
  stringstream *out = new stringstream();
  *out << num;
  string *label = new string(out->str());
  delete out;
  return label;
}

void Emitter::emit_label(const string *label) const {
  cout << *label << ":" << endl;
}

// move Ri, #1
void Emitter::emit_move(const Register *reg, int immediate) const {
  cout << "\t\t" << "move " << " R" << reg->get_num();
  cout << ", #" << immediate << endl;
}

// move Ri, Rj
void Emitter::emit_move(const Register *reg, const Register *regr) const {
  cout << "\t\t" << "move " << " R" << reg->get_num();
  cout << ", R" << regr->get_num() << endl;
}

// move Ri, variable
void Emitter::emit_move(const Register *reg, const string *var) const {
  cout << "\t\t" << "move " << " R" << reg->get_num();
  cout << ", " << *var << endl;
}

// move variable, Ri
void Emitter::emit_move(const string *id, const Register *reg) const {
  cout << "\t\t" << "move " << *id << ", ";
  cout << 'R' << reg->get_num() << endl;
}

void Emitter::emit_2addr(inst_type inst, const Register *reg,
                         int immediate) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << ", #" << immediate << endl;
}

void Emitter::emit_2addr(inst_type inst, const Register *reg,
                         const Register *src) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << ", R" << src->get_num() << endl;
}

void Emitter::emit_2addr(inst_type inst, const Register *reg,
                         const string *var) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << ", " << *var << endl;
}

void Emitter::emit_1addr(inst_type inst, const Register *reg) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << endl;
}

void Emitter::emit_branch(const string *dest) const {
  cout << "\t\t" << "brun " << *dest << endl;
}

void Emitter::emit_branch(inst_type inst, const Register *reg, int dest) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << ", " << dest << endl;
}

void Emitter::emit_branch(inst_type inst, const Register *reg,
                          const string *dest) const {
  cout << "\t\t";
  translate_and_emit(inst);
  cout << " R" << reg->get_num() << ", " << *dest << endl;
}

void Emitter::emit_halt() const {
  cout << "\t\t" << "halt" << endl;
}

void Emitter::emit_data_directive(const string *label, int size) const {
  int length = label->size();
  if (length < 7) {
    cout << *label << ':' << "\t\t" << "data " << size << endl;
  } else if (length < 15) {
    cout << *label << ':' << "\t" << "data " << size << endl;
  } else {
    cout << *label << ':' << " " << "data " << size << endl;
  }
}

void Emitter::emit_data_directive(int size) const {
  cout << "\t\t" << "data " << size << endl;
}

void Emitter::emit_comment(const char comment[]) const {
  cout << "\t\t" << "; " << comment << endl;
}

void Emitter::translate_and_emit(inst_type inst) const {
  switch (inst) {
    case INST_MOVE:
      cout << "move";
      break;
    case INST_ADD:
      cout << "add";
      break;
    case INST_SUB:
      cout << "sub";
      break;
    case INST_MUL:
      cout << "mul";
      break;
    case INST_DIV:
      cout << "div";
      break;
    case INST_NEG:
      cout << "neg";
      break;
    case INST_NOT:
      cout << "not";
      break;
    case INST_LEA:
      cout << "lea";
      break;
    case INST_BRUN:
      cout << "brun";
      break;
    case INST_BREZ:
      cout << "brez";
      break;
    case INST_BRPO:
      cout << "brpo";
      break;
    case INST_BRNE:
      cout << "brne";
      break;
    case INST_OUTB:
      cout << "outb";
      break;
    case INST_HALT:
      cout << "halt";
      break;
    case INST_GARBAGE:
    default:
      cout << "BAD TrAL INSTRUCTION";
      break;
  }
}
