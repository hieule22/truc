token.o:	token.h token.cc
	g++ -c -g -std=c++11 -Wall --pedantic token.cc

keywordtoken.o:	keywordtoken.h keywordtoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic keywordtoken.cc

punctoken.o:	punctoken.h punctoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic punctoken.cc

reloptoken.o:	reloptoken.h reloptoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic reloptoken.cc

addoptoken.o:	addoptoken.h addoptoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic addoptoken.cc

muloptoken.o:	muloptoken.h muloptoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic muloptoken.cc

idtoken.o:	idtoken.h idtoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic idtoken.cc

numtoken.o:	numtoken.h numtoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic numtoken.cc

eoftoken.o:	eoftoken.h eoftoken.cc token.h
	g++ -c -g -std=c++11 -Wall --pedantic eoftoken.cc

buffer.o:	buffer.h buffer.cc
	g++ -c -g -std=c++11 -Wall --pedantic buffer.cc

streambuffer.o:	streambuffer.h streambuffer.cc buffer.h
	g++ -c -g -std=c++11 -Wall --pedantic streambuffer.cc

filebuffer.o:	filebuffer.h filebuffer.cc buffer.h streambuffer.h
	g++ -c -g -std=c++11 -Wall --pedantic filebuffer.cc

scanner.o:	scanner.h scanner.cc buffer.h filebuffer.h \
		token.h keywordtoken.h punctoken.h reloptoken.h \
		addoptoken.h muloptoken.h idtoken.h numtoken.h eoftoken.h
	g++ -c -g -std=c++11 -Wall --pedantic scanner.cc	

test_scanner.o:	test_scanner.cc scanner.h token.h keywordtoken.h \
		punctoken.h reloptoken.h addoptoken.h muloptoken.h \
		idtoken.h numtoken.h eoftoken.h
	g++ -c -g -std=c++11 -Wall --pedantic test_scanner.cc

test_scanner:	test_scanner.o scanner.o buffer.o filebuffer.o streambuffer.o \
		token.o keywordtoken.o punctoken.o reloptoken.o addoptoken.o \
		muloptoken.o idtoken.o numtoken.o eoftoken.o
	g++ -o test_scanner -g -Wall --pedantic scanner.o buffer.o filebuffer.o \
		streambuffer.o eoftoken.o numtoken.o idtoken.o muloptoken.o \
		addoptoken.o reloptoken.o punctoken.o keywordtoken.o \
		token.o test_scanner.o

# A dependancy-less rule.  Always executes target when invoked.
clean:	
	rm *.o

# A target-less rule.  Used to test and rebuild anything listed
# in the dependency list.  
all:	token.o keywordtoken.o punctoken.o reloptoken.o addoptoken.o \
	muloptoken.o idtoken.o numtoken.o eoftoken.o \
	buffer.o streambuffer.o filebuffer.o \
	scanner.o test_scanner.o test_scanner
