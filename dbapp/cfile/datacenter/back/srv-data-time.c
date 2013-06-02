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

/*
MSA:数据中心
				日期主线
功能:日期树构造
*/
int msaDateTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[512];
	char tmpstr[512];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	stralloc strTemp;

	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	
	  char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
		}
	
	stralloc_cats(&strTemp,"[");
 	sprintf(sqlBuf,"select distinct sdate from msauserhttpreport union all select distinct sdate from msasrvreport union all select distinct sdate from msaloglist union all select distinct sdate from msafileloglist order by sdate");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			char myday[8];
      char *tabDate=NULL;
			char dateDesc[10];
			while(tempRow!=NULL){
	      tabDate=tempRow[0];
	      myday[0]=tabDate[0];
	      myday[1]=tabDate[1];
	      myday[2]=tabDate[2];
	      myday[3]=tabDate[3];
	      myday[4]=tabDate[4];
	      myday[5]=tabDate[5];
	      myday[6]=tabDate[6];
	      myday[7]=tabDate[7];
	      myday[8]='\0';
			  sprintf(dateDesc,"%c%c%c%c-%c%c-%c%c",myday[0],myday[1],myday[2],myday[3],myday[4],myday[5],myday[6],myday[7]);
				/*判断有没有子节点*/
				if(iHead==1){
					if(myday[4]=='-'){
						sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[0],tempRow[0]);
					}else{
						sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",dateDesc,dateDesc,dateDesc);
					}
					iHead=0;
				}else{
					if(myday[4]=='-'){
						sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[0],tempRow[0]);
					}else{
						sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",dateDesc,dateDesc,dateDesc);
					}
			  }
			  stralloc_cats(&strTemp,tmpstr);
				stralloc_cats(&strTemp,",leaf: true}");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}

