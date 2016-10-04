// Implementation of Buffer class.
// @author Hieu Le
// @version 09/28/2016

#include "buffer.h"

#include <ctype.h>

#include <algorithm>
#include <iostream>

using namespace std;

// Placing declarations in an anonymous namespace makes them visible only to
// this compilation unit and not polluting the global namespace.
namespace {

// All valid non-alphanumeric symbols from TruPL.
const char kNonAlphanum[] =
{';', ':', '(', ')', ',', '=', '>', '<', '+', '-', '*', '/',
 COMMENT_MARKER, SPACE, TAB, NEW_LINE};

// Checks if a specified value is a member of the given container.
template <typename Container, typename T>
bool contains(const Container& container, const T& value) {
  return find(begin(container), end(container), value) != end(container);
}

// Checks if a given character c belongs to the TruPL alphabet.
bool validate(const char c) {
  return islower(c) || isdigit(c) || contains(kNonAlphanum, c);
}

// Checks if given input stream is empty.
bool is_empty_stream(istream *const stream) {
  return stream->peek() == EOF;
}

// Fills buffer with characters from an input stream up to some specified limit.
void fill_buffer(istream *const stream, list<char>* buffer, const size_t lim) {
  for (size_t i = 0; i < lim && !is_empty_stream(stream); ++i) {
    buffer->push_back(stream->get());
  }
}

}  // namespace

void Buffer::buffer_fatal_error() const {
  cerr << "Exiting on BUFFER FATAL ERROR" << endl;
  exit(EXIT_FAILURE);
}

Buffer::Buffer(istream *const stream) : stream_(stream), exhausted_(false) {
  remove_space_and_comment();  // Remove any preceding whitespace or comment.
}

Buffer::Buffer(const char *const filename) : exhausted_(false) {
  source_file_.open(filename);
  if (source_file_.fail()) {
    cerr << "Can't open source file " << filename << endl;
    buffer_fatal_error();
  }
  stream_ = &source_file_;
  remove_space_and_comment();  // Remove any preceding whitespace or comment.
}

Buffer::~Buffer() {
  if (source_file_.is_open()) {
    source_file_.close();
  }
}

char Buffer::next() {
  // Refill buffer if empty.
  if (buffer_.empty()) {
    fill_buffer(stream_, &buffer_, MAX_BUFFER_SIZE);
  }
  // Return EOF if buffer is still empty after refill attempt.
  if (buffer_.empty()) {
    exhausted_ = true;
    return EOF_MARKER;
  }

  char head = buffer_.front();
  buffer_.pop_front();
  return head;
}

void Buffer::skip_line() {
  char head = next();
  // Skips character until the nearest new line character or until encountering
  // the end of file, whichever comes first.
  while (head != NEW_LINE && !exhausted_) {
    head = next();
  }
}

bool Buffer::remove_space_and_comment() {
  char head = next();
  bool has_whitespace_or_comment = false;
  
  // Advance head to skip all whitespaces and comments.p
  while (is_whitespace(head) || head == COMMENT_MARKER) {
    has_whitespace_or_comment = true;
    while (is_whitespace(head)) {  // Remove whitespaces.
      head = next();
    }
    if (head == COMMENT_MARKER) {  // Remove comments.
      skip_line();
      head = next();
    }
  }

  // head now stores the nearest character that is neither a whitespace nor
  // part of a comment. If buffer is not exhausted, i.e head does not contain
  // EOF_MARKER, places it back at the front of buffer.
  if (!exhausted_) {
    buffer_.push_front(head);
  }
  return has_whitespace_or_comment;
}

char Buffer::next_char() {
  // Remove any subsequent region of whitespaces and comments and return the
  // default space delimiter if any removal takes place.
  if (remove_space_and_comment()) {
    return SPACE;
  }

  char head = next();
  // Crash if head does not belong the TruPL alphabet. The case where EOF symbol
  // is returned has to be excluded however.
  if (!validate(head)) {
    if (!(head == EOF_MARKER && exhausted_)) {
      cerr << "Invalid character: " << head << endl;
      buffer_fatal_error();
    }
  }
  return head;
}

void Buffer::unread_char(const char c) {
  if (c != EOF_MARKER) {
    exhausted_ = false;
    buffer_.push_front(c);
  }
}
