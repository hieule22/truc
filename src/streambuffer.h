// Buffer class to read characters from an input stream.
// @author Hieu Le
// @version 09/28/2016

#ifndef STREAMBUFFER_H
#define STREAMBUFFER_H

#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "buffer.h"

using namespace std;

class StreamBuffer : public Buffer {
 public:
  // Initializes the buffer from given input stream. The associated stream
  // remains property of the caller and shall not be destroyed or modified
  // during the lifetime of this object.
  explicit StreamBuffer(istream *stream);

  // Removes and returns the next character from the stream. Returns EOF if
  // there is no more character to read.
  char next_char() override;

  // Places a character back into the buffer.
  void unread_char(char c) override;

 private:
  // Capacity of internal character buffer.
  static const int MAX_BUFFER_SIZE = 1024;
  
  // Gets the next character and performs any necessary buffer refill.
  char next();

  // Skips the current line of characters.
  void skip_line();

  // Removes any subsequent whitespace and comment. If there is any remaining
  // token to process, the first character of that token would be stored in the
  // buffer front.
  // Returns true if any removal takes place; false otherwise.
  bool remove_space_and_comment();

  // Input stream to read characters from.
  istream * const stream_;

  // Internal character buffer.
  list<char> buffer_;

  // Flags indicating if there are any remaining character to read.
  bool exhausted;
};
  
#endif
