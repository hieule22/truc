// Main program of the TruPL compiler.
// @author Hieu Le
// @version November 9th, 2016

#include <cstdlib>

#include <iostream>

#include "parser.h"
#include "scanner.h"

int main(int argc, char **argv)
{
  char *filename;
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input file name>" << std::endl;
    exit(EXIT_FAILURE);
  }

  filename = argv[1];

  // Create a Parser for this source file.
  Parser parser(new Scanner(filename));

  if (parser.parse_program()) {
    if (parser.done_with_input()) {
      std::cout << "Parsing succeeded!" << std::endl;
    } else {
      std::cout << "Extra tokens found at the end of program!" << std::endl;
    }
  } else {
    std::cout << "Parsing failed" << std::endl;
  }

  return 0;
}
