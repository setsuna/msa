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
	char tmpstr[512];
	stralloc strTemp;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;

  int dayarr[5];
	dayarr[0]=_ltPubInfo->_msaOptionHead.mailkeepday;
	dayarr[1]=_ltPubInfo->_msaOptionHead.aimkeepday;
	dayarr[2]=_ltPubInfo->_msaOptionHead.formkeepday;
	dayarr[3]=_ltPubInfo->_msaOptionHead.historykeepday;
	dayarr[4]=_ltPubInfo->_msaOptionHead.vidkeepday;
	
	int tempday;
	int j;
	for(j=0;j<5;j++)
	{
			if(dayarr[j]<dayarr[j+1])
			{
				tempday=dayarr[j];
				dayarr[j]=dayarr[j+1];
				dayarr[j+1]=tempday;
			}
	}
				
	char sDate[32];
	long lTime;
	stralloc_cats(&strTemp,"[");	
	int k;
	for(k=0;k<=dayarr[0];k++)
	{
		  lTime=time(0)-(k*24*60*60);
		  memset(sDate,0,sizeof(sDate));
		  nasTimeGetDate(sDate,lTime);
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",sDate,sDate,sDate);
				iHead=0;
			}else{
				sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",sDate,sDate,sDate);
		  }
		  stralloc_cats(&strTemp,tmpstr);
			stralloc_cats(&strTemp,",leaf: true}");
	}
		

	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}


/*
MSA:数据中心
        日期主线
功能:点击日期连接页面函数
*/

int msaShowTimeRecord(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *sdate;
   ltDbHeadPtr dbPtr;
   sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
	 if(!sdate){
	 		sdate="";
	 }
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"sdate",sdate);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/daterecord.htm",dbPtr->head,0);    
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

