// Implementation of FileBuffer class.
// @author Hieu Le
// @version 09/29/2016

#include "filebuffer.h"

#include <utility>
#include <iostream>

#include "streambuffer.h"

FileBuffer::FileBuffer(const std::string& filename) {
  // Open the file and fill the buffer.
  source_file_.open(filename);
  if (source_file_.fail()) {
    std::cerr << "Can't open source file " << filename << std::endl;
    buffer_fatal_error();
  }
  buffer_ = std::make_unique<StreamBuffer>(&source_file_);
}

FileBuffer::~FileBuffer() {
  source_file_.close();
}

char FileBuffer::next_char() {
  return buffer_->next_char();
}

void FileBuffer::unread_char(const char c) {
  buffer_->unread_char(c);
}
