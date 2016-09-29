// Buffer interface to read characters from an input source.
// @author Hieu Le
// @version 09/29/2016

#ifndef BUFFER_H
#define BUFFER_H

// Not part of TruPL alphabet. Used only by the lexical analyzer to denote EOF.
#define EOF_MARKER '$'

// Symbol denoting the start of a comment line.
#define COMMENT_MARKER '#'

// Whitespace symbols.
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'

class Buffer
{
 public:
  virtual ~Buffer();
  
  // Removes and returns the next character from the buffer. Any preceding
  // region of whitespaces and comments will be ignored. Any intervening or
  // trailing region of whitespaces and comments will be compressed into a
  // single delimiting space. The EOF marker is returned when user attempts
  // to read past the end of input. The buffer will crash upon encountering
  // a symbol that is not part of the TruPL alphabet.
  virtual char next_char() = 0;
  
  // Places a character back into the buffer. c must be the last character
  // returned by next_char(). Should not be called more than once without an
  // intervening call to next_char();
  virtual void unread_char (char c) = 0;
  
  
 protected:
  // If something catastrophic happens in the buffer, print
  // an error message and then call this method to exit.
  void buffer_fatal_error() const;
  
  // Useful utility function.  Is c a whitespace char?
  inline bool is_whitespace (const char c) const
  {
    return (c == SPACE || c == TAB || c == NEW_LINE);
  }

  // Checks if a given character c belongs to the TruPL alphabet.
  // Returns true if it does; false otherwise.
  bool validate(const char c) const;
};

#endif
