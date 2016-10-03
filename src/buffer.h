// Buffer to read characters from an input source.
// @author Hieu Le
// @version 09/29/2016

#ifndef BUFFER_H
#define BUFFER_H

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <list>

// Not part of TruPL alphabet. Used only by the lexical analyzer to denote EOF.
#define EOF_MARKER '$'

// Symbol denoting the start of a comment line.
#define COMMENT_MARKER '#'

// Whitespace symbols.
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'

using namespace std;

class Buffer {
 public:
  // Opens the input program file and initializes the buffer.
  explicit Buffer(const char *filename);

  // Initializes the buffer from an input stream. Useful for testing.
  // The stream remains property of the caller and should not be modified or
  // destroyed during the lifetime of this buffer.
  explicit Buffer(istream *stream);

  ~Buffer();

  // Removes and returns the next character from the buffer. Any preceding
  // region of whitespaces and comments will be ignored. Any intervening or
  // trailing region of whitespaces and comments will be compressed into a
  // single delimiting space. The EOF marker is returned when user attempts
  // to read past the end of input. The buffer will crash upon encountering
  // a symbol that is not part of the TruPL alphabet.
  char next_char();

  // Places a character back into the buffer. c must be the last character
  // returned by next_char(). Should not be called more than once without an
  // intervening call to next_char();
  void unread_char(char c);

 private:
  // Capacity of internal character buffer.
  static const int MAX_BUFFER_SIZE = 1024;

  // If something catastrophic happens in the buffer, print an error message
  // and then call this method to exit.
  void buffer_fatal_error() const;

  // Useful utility function.  Is c a whitespace char?
  inline bool is_whitespace(const char c) const {
    return (c == SPACE || c == TAB || c == NEW_LINE);
  }

  // Gets the next character and performs any necessary buffer refill.
  char next();

  // Skips the current line of characters.
  void skip_line();

  // Removes any nearby whitespace or comment. If there is any remaining token
  // to process, the first character of that token would be stored in the buffer
  // front. Returns true if any removal takes place; false otherwise.
  bool remove_space_and_comment();

  // The stream object for the source file.
  ifstream source_file_;

  // The underlying input stream to read characters from.
  istream *stream_;

  // The character buffer.
  list<char> buffer_;

  // Flag indicating if there is any remaining character to read.
  bool exhausted_;
};

#endif
