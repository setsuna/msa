prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msa/lib
srcdir = /app/msa/msa
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all:
	ar rv ${libdir}/srvlib.a  *.o
	
srv-admin.o:
	gcc -c  srv-admin.c  $(CFLAGS)

srv-manager.o:
	gcc -c  srv-manager.c  $(CFLAGS)

srv-group.o:
	gcc -c  srv-group.c  $(CFLAGS)

srv-nowview.o: 
	gcc -c  srv-nowview.c  $(CFLAGS)

srv-nowconect.o:
	gcc -c  srv-nowconect.c  $(CFLAGS)

srv-nowviewuser.o:
	gcc -c  srv-nowviewuser.c  $(CFLAGS)
	
srv-nowtools.o:
	gcc -c  srv-nowtools.c  $(CFLAGS)

srv-nowfun.o:
	gcc -c  srv-nowfun.c  $(CFLAGS)	

srv-users.o:
	gcc -c  srv-users.c  $(CFLAGS)	

srv-adusers.o:
	gcc -c  srv-adusers.c  $(CFLAGS)	

srv-userexport.o:
	gcc -c  srv-userexport.c  $(CFLAGS)	
	
srv-userimport.o: srv-userimport.c
	gcc -c  srv-userimport.c  $(CFLAGS)	
	
srv-userpolicy.o: srv-userpolicy.c
	gcc -c  srv-userpolicy.c  $(CFLAGS)	
	
srv-sysclean.o: srv-sysclean.c
	gcc -c  srv-sysclean.c  $(CFLAGS)	

srv-bushu.o: srv-bushu.c
	gcc -c  srv-bushu.c  $(CFLAGS)	

srv-rule.o: srv-rule.c
	gcc -c  srv-rule.c  $(CFLAGS)	

srv-url.o: srv-url.c
	gcc -c  srv-url.c  $(CFLAGS)	
	
srv-auth.o: srv-auth.c
	gcc -c  srv-auth.c  $(CFLAGS)	

srv-qos.o: srv-qos.c
	gcc -c  srv-qos.c  $(CFLAGS)

srv-firewall.o: srv-firewall.c
	gcc -c  srv-firewall.c  $(CFLAGS)	
	
srv-route.o: srv-route.c
	gcc -c  srv-route.c  $(CFLAGS)	

srv-keyword.o: srv-keyword.c
	gcc -c  srv-keyword.c  $(CFLAGS)
	
srv-rule-vid.o: srv-rule-vid.c
	gcc -c  srv-rule-vid.c  $(CFLAGS)	

srv-rule-sec.o: srv-rule-sec.c
	gcc -c  srv-rule-sec.c  $(CFLAGS)	

srv-data.o: srv-data.c
	gcc -c  srv-data.c  $(CFLAGS)
	
srv-linereport.o: srv-linereport.c
	gcc -c  srv-linereport.c  $(CFLAGS)

srv-report.o: srv-report.c
	gcc -c  srv-report.c  $(CFLAGS)

page-option.o: page-option.c
	gcc -c  page-option.c  $(CFLAGS)

srv-srvline.o: srv-srvline.c
	gcc -c  srv-srvline.c  $(CFLAGS)

srv-flowline.o: srv-flowline.c
	gcc -c  srv-flowline.c  $(CFLAGS)	
