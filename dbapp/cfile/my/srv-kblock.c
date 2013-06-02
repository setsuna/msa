#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <gdfontmb.h>
#include <dirent.h>
#include "msa.h"
#include "lber.h"
#include "ldap.h"

int ltbandlink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char    ENABLEBAND[24];
	char    BANDUPLINK[24];
	char    BANDDOWNLINK[24];
	char    BANDUPDEF[24];
	char    BANDDOWNDEF[24];

	char    *pFile="/etc/msa/msa/qos/settings";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	if( atol(bcCnfGetValueS("/etc/msa/msa","advance", "bandlimit"))==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
	}else{
			memset(ENABLEBAND,0,sizeof(ENABLEBAND));
			bcCnfGetValue_s(pFile,"ENABLEBAND",ENABLEBAND);
			if(strcmp(ENABLEBAND,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"greensel","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
		  }
	}  
		  memset(BANDUPLINK,0,sizeof(BANDUPLINK));
			bcCnfGetValue_s(pFile,"BANDUPLINK",BANDUPLINK);
	    lt_dbput_rootvars(dbPtr,1,"BANDUPLINK",BANDUPLINK );	
			
			
		  memset(BANDDOWNLINK,0,sizeof(BANDDOWNLINK));
		 	bcCnfGetValue_s(pFile,"BANDDOWNLINK",BANDDOWNLINK);
	    lt_dbput_rootvars(dbPtr,1,"BANDDOWNLINK",BANDDOWNLINK );	
			
			if(ltStrIsDigital(BANDDOWNLINK)==0){
				_bcKernelHead->banddownlink=atol(BANDDOWNLINK);
			}
			if(ltStrIsDigital(BANDUPLINK)==0){
				_bcKernelHead->banduplink=atol(BANDUPLINK);
			}
			
			memset(BANDUPDEF,0,sizeof(BANDUPDEF));
			bcCnfGetValue_s(pFile,"BANDUPDEF",BANDUPDEF);
	    lt_dbput_rootvars(dbPtr,1,"BANDUPDEF",BANDUPDEF );	
			
			
		  memset(BANDDOWNDEF,0,sizeof(BANDDOWNDEF));
		 	bcCnfGetValue_s(pFile,"BANDDOWNDEF",BANDDOWNDEF);
	    lt_dbput_rootvars(dbPtr,1,"BANDDOWNDEF",BANDDOWNDEF );	
			
			if(ltStrIsDigital(BANDDOWNDEF)==0){
				_bcKernelHead->banddowndef=atol(BANDDOWNDEF)*1000;
			}
			if(ltStrIsDigital(BANDUPDEF)==0){
				_bcKernelHead->bandupdef=atol(BANDUPDEF)*1000;
			}
							
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/bandlink.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int ltbandlinkok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    *ENABLEBAND;
	char    *BANDUPLINK;
	char    *BANDDOWNLINK;
	char    *BANDUPDEF;
	char    *BANDDOWNDEF;
	char    caCmdCon[512];
	char    *pFile="/etc/msa/msa/qos/settings";

  if(atol(bcCnfGetValueS("/etc/msa/msa","advance", "bandlimit"))==0 ){
	 	 ENABLEBAND="NO";
	}else{
		ENABLEBAND=ltMsgGetVar_s(ltMsgPk,"ENABLEBAND");
		printf("AAAENABLEBAND:%s\n",ENABLEBAND);
		if(ENABLEBAND==NULL){
			ENABLEBAND="NO";
		}
  }
	
	BANDUPLINK=ltMsgGetVar_s(ltMsgPk,"BANDUPLINK");
	BANDDOWNLINK=ltMsgGetVar_s(ltMsgPk,"BANDDOWNLINK");

	if(ltStrIsDigital(BANDDOWNLINK)==0){
				_bcKernelHead->banddownlink=atol(BANDDOWNLINK);
	}
	if(ltStrIsDigital(BANDUPLINK)==0){
				_bcKernelHead->banduplink=atol(BANDUPLINK);
	}
	BANDUPDEF=ltMsgGetVar_s(ltMsgPk,"BANDUPDEF");
	BANDDOWNDEF=ltMsgGetVar_s(ltMsgPk,"BANDDOWNDEF");

	if(ltStrIsDigital(BANDDOWNDEF)==0){
				_bcKernelHead->banddowndef=atol(BANDDOWNDEF)*1000;
	}
	if(ltStrIsDigital(BANDUPDEF)==0){
				_bcKernelHead->bandupdef=atol(BANDUPDEF)*1000;
	}
	
	if(_bcKernelHead->banddownlink<10 || _bcKernelHead->banduplink<10 ){
		ENABLEBAND="NO";
	}
	
	printf("ENABLEBAND:%s\n",ENABLEBAND);
	
	system("/bin/cp /etc/msa/msa/qos/settings /etc/msa/msa/qos/settings.old");
		/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEBAND=%s' > %s",ENABLEBAND,pFile);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'BANDUPLINK=%s' >> %s",BANDUPLINK,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDDOWNLINK=%s' >> %s",BANDDOWNLINK,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDDOWNDEF=%s' >> %s",BANDDOWNDEF,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDUPDEF=%s' >> %s",BANDUPDEF,pFile);
	system(caCmdCon);
	
		
	ltbandlink(confd,ltMsgPk,lt_MMHead);
	
  return 0;

}

