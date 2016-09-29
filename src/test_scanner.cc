#include <iostream>
#include <stdlib.h>

#include "scanner.h"

/* A program to print all the TruPL tokens and their attributes in 
   a file.
*/
int main (int argc, char **argv)
{

  char *filename;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input file name>" << std::endl;
    exit (-1);
  }

  filename = argv[1];

  // Declare a Scanner object and a pointer to a Token object.
  Scanner s(filename);
  Token *t = NULL;

  // Grab and print tokens until the EOF token is returned.
  do {

    if (t != NULL) {
      delete t;
    }
    
    t = s.next_token();

    // Print out the token type and its attributes, if any.
    if (t->get_token_type() == TOKEN_NO_TYPE) {
      std::cout << "Error: next_token() returned typeless token." << std::endl;
    } else {
      std::cout << *(t->to_string()) << std::endl;
    }

  } while (t->get_token_type() != TOKEN_EOF);

}

    
