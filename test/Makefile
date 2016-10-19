# Makefile for building and running tests.

GTEST_DIR = ../third_party/gtest

CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


# Build tests.

SRC_DIR = ../src

PROJECT_ROOT = ..

CXXFLAGS += -std=c++11 --pedantic

# All tests produced by this Makefile.
TESTS = buffer_test scanner_test

PROJECT_SRCS = $(SRC_DIR)/scanner.cc $(SRC_DIR)/buffer.cc $(SRC_DIR)/*token.cc

buffer_test:	scanner/buffer_test.cc $(PROJECT_SRCS) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(PROJECT_ROOT) -lpthread $^  -o $@ \
	&& ./$@

scanner_test:	scanner/scanner_test.cc $(PROJECT_SRCS) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(PROJECT_ROOT) -lpthread $^ -o $@ \
	&& ./$@

all : $(TESTS)

clean :
	rm -rf $(TESTS) gtest.a gtest_main.a *.o *.dSYM
