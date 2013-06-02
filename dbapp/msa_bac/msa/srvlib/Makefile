prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msa/lib
srcdir = /app/msa/msa
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all: srv-manager.o srv-group.o
	ar rv ${libdir}/srvlib.a  *.o
clean: 
	rm -f  ${libdir}/srvlib.a  srv-manager.o srv-group.o
	
srv-manager.o: srv-manager.c
	gcc -c  srv-manager.c  $(CFLAGS)

srv-group.o: srv-group.c
	gcc -c  srv-group.c  $(CFLAGS)
