// Implementation of StreamBuffer class.
// @author Hieu Le
// @version 09/28/2016

#include "streambuffer.h"

#include <sstream>
#include <utility>

#include "buffer.h"

namespace {

// Checks if given input stream is empty.
bool is_empty_stream(std::istream* stream) {
  return stream->peek() == EOF;
}

// Fills buffer with characters from givn input stream up to
// some specified limit.
void fill_buffer(std::istream* const stream, std::list<char>* buffer,
                 const size_t limit) {
  for (size_t i = 0; i < limit && !is_empty_stream(stream); ++i) {
    buffer->push_back(stream->get());
  }
}

}  // namespace

StreamBuffer::StreamBuffer(std::istream* stream)
    : stream_(stream), exhausted(false) {
  // Remove any preceding whitespace or comment.
  remove_space_and_comment();
}

char StreamBuffer::next() {
  // Refill buffer if empty.
  if (buffer_.empty()) {
    fill_buffer(stream_, &buffer_, MAX_BUFFER_SIZE);
  }
  // Signal EOF if buffer is still empty after refill attempt.
  if (buffer_.empty()) {
    exhausted = true;
    return EOF_MARKER;
  }

  char head = buffer_.front();
  buffer_.pop_front();
  return head;
}

void StreamBuffer::skip_line() {
  char head = next();
  while (head != NEW_LINE && !exhausted) {
    head = next();
  }
}

bool StreamBuffer::remove_space_and_comment() {
  char head = next();
  bool has_whitespace_or_comment = false;
  while (is_whitespace(head) || head == COMMENT_MARKER) {
    has_whitespace_or_comment = true;
    while (is_whitespace(head)) {  // Remove whitespaces.
      head = next();
    }
    if (head == COMMENT_MARKER) {
      skip_line();
      head = next();
    }
  }

  // head now stores the nearest character that is neither a whitespace nor
  // part of a comment. If buffer is not exhausted, i.e head does not contain
  // EOF_MARKER, places it back at the front of buffer.
  if (!exhausted) {
    buffer_.push_front(head);
  }
  return has_whitespace_or_comment;
}

char StreamBuffer::next_char() {
  // Remove any subsequent region of white spaces and comments and return the
  // default space delimiter.
  if (remove_space_and_comment()) {
    return SPACE;
  }

  char head = next();
  // Flags error if current does not belong the TruPL alphabet.
  if (!validate(head)) {
    if (!(head == EOF_MARKER && exhausted)) {
      std::cerr << "Invalid character: " << head << std::endl;
      buffer_fatal_error();
    }
  }
  return head;
}

void StreamBuffer::unread_char(const char c) {
  if (c != EOF_MARKER) {
    exhausted = false;
    buffer_.push_front(c);
  }
}
