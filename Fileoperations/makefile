# GRUP : 9
# başak yıldırım b151210107 
# levent can şençamlar b141210026  
# Osman Kiraz B151210105 
# yasemin çerçi b140910053  
# esra koyuncu b151210032  



CLASSDIR = ./
INCLUDE = -I$(CLASSDIR)/include
LIBDIR = $(CLASSDIR)/objs
CC = gcc
LIBS = $(LIBDIR)/libfdr.a
CFLAGS = -g $(INCLUDE)
ALL = pro

all: $(ALL)

clean:
	rm -f *.o core $(ALL)

pro: pro.o 
	$(CC) $(CFLAGS) -o pro pro.o $(LIBS)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c


