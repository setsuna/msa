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
        人员主线
功能:用户树构造函数
*/
int msaUserTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[512];
	char tmpstr[512];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char *level;
	char *gid;
	stralloc strTemp;
	int iHead;
	
	memset(tmpstr,0,sizeof(tmpstr));
	level=0;
	gid=0;
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
	
	if(ltMsgGetVar_s(ltMsgPk,"gid")!=NULL){
		gid=ltMsgGetVar_s(ltMsgPk,"gid");
	}
	if(ltMsgGetVar_s(ltMsgPk,"level")!=NULL){
		level=ltMsgGetVar_s(ltMsgPk,"level");
	}
	stralloc_cats(&strTemp,"[");
 	sprintf(sqlBuf,"select userid,dispname from msauser where groupid%s=%s order by userid asc",level,gid);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
			/*判断有没有子节点*/
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[1],tempRow[1]);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[1],tempRow[1]);
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

/*
MSA:数据中心
        人员主线
功能:点击部门连接页面函数
*/

int msaShowPeopleRecord(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *gid;
   char *uid;
   ltDbHeadPtr dbPtr;
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
   uid=ltMsgGetVar_s(ltMsgPk,"uid");
	 if(!gid){
	 		gid="";
	 }
	 if(!uid){
	 		uid="";
	 }
	 printf("gid:%s\n",gid);
	 printf("uid:%s\n",uid);
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"gid",gid);
   lt_dbput_rootvars(dbPtr,1,"uid",uid);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/peoplerecord.htm",dbPtr->head,0);    
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
