# Bibleajax program
# Bob Kasper, MVNU Computer Science
# updated January 2020 to use
# c++11 compiler option, current paths on cs.mvnu.edu

# This is a Makefile for the Bible web app demo program.
# Copy this directory to a location within your home directory.
# Change the USER name value below to your own user name.
# Then use "make" to build the server program,
# and deploy it to the live web server directory.
# To test the program, go to http://cs.mvnu.edu/class/csc3004/USER/
# and open the bibleajax.html link.

# TODO: Must replace "username" by your username
USER= coehall

# Use GNU C++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

#all:	bibleajax.cgi PutCGI PutHTML
all: testreader lookupserver

# TODO: For bibleajax.cgi, add dependencies to include
# compiled classes from Project 1 to be linked into the executable program
#bibleajax.cgi:	bibleajax.o Bible.o Ref.o Verse.o
	#$(CC) $(CFLAGS) -o bibleajax.cgi bibleajax.o Bible.o Ref.o Verse.o -lcgicc
	# -l option is necessary to link with cgicc library

# main program to handle AJAX/CGI requests for Bible references
bibleajax.o:	bibleajax.cpp Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c bibleajax.cpp

testreader: testreader.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o testreader testreader.o Bible.o Ref.o Verse.o

testreader.o: testreader.cpp Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c testreader.cpp

# TODO: copy targets to build classes from Project 1:
# Bible.o, Ref.o, Verse.o

Bible.o: Bible.cpp Bible.h Ref.h Verse.h
	 $(CC) $(CFLAGS) -c Bible.cpp

Ref.o: Ref.cpp Ref.h
	 $(CC) $(CFLAGS) -c Ref.cpp

Verse.o: Verse.cpp Verse.h Ref.h
	 $(CC) $(CFLAGS) -c Verse.cpp

fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

lookupserver.o: lookupserver.cpp fifo.h Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c lookupserver.cpp

lookupserver: lookupserver.o fifo.o
	$(CC) $(CFLAGS) -o lookupserver lookupserver.o Bible.o Ref.o Verse.o fifo.o

#PutCGI:	bibleajax.cgi
	#chmod 755 bibleajax.cgi
	#cp bibleajax.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

	#echo "Current contents of your cgi-bin directory: "
	#ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

#PutHTML:
#	cp bibleajax.html /var/www/html/class/csc3004/$(USER)
#
#	echo "Current contents of your HTML directory: "
#	ls -l /var/www/html/class/csc3004/$(USER)

clean:
	rm *.o core bibleajax.cgi testreader lookupserver
