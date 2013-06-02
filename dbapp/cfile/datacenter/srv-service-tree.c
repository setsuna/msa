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
#include <dirent.h>
#include <gdfontmb.h>
#include <ctype.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <stdarg.h>
#include "msa.h"

int msaOneServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char tmpstr[1024];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_TOP_SRVNUM;j++){
 			if(strcmp(_ltPubInfo->topSrvName[j].srvname,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
				  }
				  stralloc_cats(&strTemp,tmpstr);
				  stralloc_cats(&strTemp,",leaf: true}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}


int msaTwoServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char tmpstr[1024];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	long srvnum=0;
  srvnum=atol(ltMsgGetVar_s(ltMsgPk,"id"));
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_SYS_SRVNUM;j++){
 			if(_ltPubInfo->ltService[j].topSrv==srvnum&&strcmp(_ltPubInfo->ltService[j].srvName,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{id: '%d',text: '%s'",j,_ltPubInfo->ltService[j].srvName);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{id: '%d',text: '%s'",j,_ltPubInfo->ltService[j].srvName);
				  }
				  stralloc_cats(&strTemp,tmpstr);
				  stralloc_cats(&strTemp,",leaf: true}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}

