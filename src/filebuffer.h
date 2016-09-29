// Buffer class to read characters from an input file.
// @author Hieu Le
// @version 09/29/2016

#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <fstream>
#include <memory>
#include <string>

#include "buffer.h"

class FileBuffer : public Buffer {
 public:
  // Opens the input program file and initializes the buffer.
  explicit FileBuffer(const std::string& filename);

  // Closes the file and performs any needed clean-up.
  ~FileBuffer();

  // Removes and returns the next character from the buffer.
  char next_char() override;

  // Places a character back into the buffer.
  void unread_char(char c) override;

 private:
  // The stream object for the source file.
  std::ifstream source_file_;

  // The composed stream buffer. 
  std::unique_ptr<Buffer> buffer_;
};

#endif
