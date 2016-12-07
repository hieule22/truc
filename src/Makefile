# Compiler options
CFLAGS = -g -std=c++11 -Wall --pedantic

token.o:	token.h token.cc
	g++ -c $(CFLAGS) token.cc

keywordtoken.o:	keywordtoken.h keywordtoken.cc token.h
	g++ -c $(CFLAGS) keywordtoken.cc

punctoken.o:	punctoken.h punctoken.cc token.h
	g++ -c $(CFLAGS) punctoken.cc

reloptoken.o:	reloptoken.h reloptoken.cc token.h
	g++ -c $(CFLAGS) reloptoken.cc

addoptoken.o:	addoptoken.h addoptoken.cc token.h
	g++ -c $(CFLAGS) addoptoken.cc

muloptoken.o:	muloptoken.h muloptoken.cc token.h
	g++ -c $(CFLAGS) muloptoken.cc

idtoken.o:	idtoken.h idtoken.cc token.h
	g++ -c $(CFLAGS) idtoken.cc

numtoken.o:	numtoken.h numtoken.cc token.h
	g++ -c $(CFLAGS) numtoken.cc

eoftoken.o:	eoftoken.h eoftoken.cc token.h
	g++ -c $(CFLAGS) eoftoken.cc

buffer.o:	buffer.h buffer.cc
	g++ -c $(CFLAGS) buffer.cc

scanner.o:	scanner.h scanner.cc buffer.h token.h keywordtoken.h \
		punctoken.h reloptoken.h addoptoken.h muloptoken.h \
		idtoken.h numtoken.h eoftoken.h
	g++ -c $(CFLAGS) scanner.cc

symbol_table.o:	symbol_table.h symbol_table.cc
	g++ -c $(CFLAGS) symbol_table.cc

register.o:	register.h register.cc
	g++ -c $(CFLAGS) register.cc

register_allocator.o:	register_allocator.h register_allocator.cc register.h
	g++ -c $(CFLAGS) register_allocator.cc

operand.o:	operand.h operand.cc register.h
	g++ -c $(CFLAGS) operand.cc

emitter.o:	emitter.h emitter.cc register.h
	g++ -c $(CFLAGS) emitter.cc

parser.o:	parser.h parser.cc scanner.h token.h keywordtoken.h \
		punctoken.h reloptoken.h addoptoken.h muloptoken.h \
		idtoken.h numtoken.h eoftoken.h symbol_table.h \
		register.h register_allocator.h emitter.h operand.h
	g++ -c $(CFLAGS) parser.cc

test_scanner.o:	test_scanner.cc scanner.h token.h keywordtoken.h \
		punctoken.h reloptoken.h addoptoken.h muloptoken.h \
		idtoken.h numtoken.h eoftoken.h
	g++ -c $(CFLAGS) test_scanner.cc

test_scanner:	test_scanner.o scanner.o buffer.o token.o keywordtoken.o \
		punctoken.o reloptoken.o addoptoken.o \
		muloptoken.o idtoken.o numtoken.o eoftoken.o
	g++ -o test_scanner $(CFLAGS) scanner.o buffer.o eoftoken.o numtoken.o \
		idtoken.o muloptoken.o \
		addoptoken.o reloptoken.o punctoken.o keywordtoken.o \
		token.o test_scanner.o

truc.o:	truc.cc parser.h scanner.h token.h keywordtoken.h punctoken.h \
	reloptoken.h addoptoken.h muloptoken.h idtoken.h numtoken.h eoftoken.h \
	register.h register_allocator.h emitter.h operand.h
	g++ -c $(CFLAGS) truc.cc

truc:	truc.o parser.o scanner.o buffer.o token.o keywordtoken.o \
	punctoken.o reloptoken.o addoptoken.o muloptoken.o idtoken.o \
	numtoken.o eoftoken.o symbol_table.o register.o register_allocator.o \
	emitter.o operand.o
	g++ -o truc $(CFLAGS) truc.o parser.o scanner.o buffer.o eoftoken.o \
	numtoken.o idtoken.o muloptoken.o addoptoken.o reloptoken.o \
	punctoken.o keywordtoken.o token.o symbol_table.o register.o \
	register_allocator.o emitter.o operand.o

# A dependancy-less rule.  Always executes target when invoked.
clean:	
	rm *.o

# A target-less rule.  Used to test and rebuild anything listed
# in the dependency list.  
all:	token.o keywordtoken.o punctoken.o reloptoken.o addoptoken.o \
	muloptoken.o idtoken.o numtoken.o eoftoken.o \
	register.o register_allocator.o emitter.o operand.o \
	buffer.o scanner.o parser.o test_scanner.o test_scanner truc.o truc
