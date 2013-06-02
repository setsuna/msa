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

/*
MSA:策略中心
虚拟身份过滤
		虚拟身份过滤清单
		    树节点连接 记录添加 记录删除 调用函数
*/
int videntityFilter(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 //数据库连接
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
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 addflag=NULL;
	 delflag=NULL;
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");

	 tablePtr=lt_dbput_table(dbPtr,"list");
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }

	 if(addflag){//添加
	 		int lCount;
	 		lCount=0;
	 		char *rulevid=ltMsgGetVar_s(ltMsgPk,"rulevid");
	 		char *ruletype=ltMsgGetVar_s(ltMsgPk,"ruletype");
      
      printf("rulevid:%s\n",rulevid);
      printf("ruletype:%s\n",ruletype);
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"select max(ruleid)+1 from nasvidrule");
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
      if(tempCursor!=NULL){
         tempRow= ltDbFetchRow(tempCursor);
         if(tempRow!=NULL){
                  lCount=atoi(tempRow[0]);
         }
         ltDbCloseCursor(tempCursor);
			}
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into nasvidrule values(%d,'%s','%s')",lCount,rulevid,ruletype);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 
	 if(delflag){
	 		char *id=ltMsgGetVar_s(ltMsgPk,"id");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from nasvidrule where ruleid=%s",id);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }

	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nasvidrule");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				char type[32];
				memset(type,0,sizeof(type));
				sprintf(type,"tse%s",tempRow[2]);				
				lt_dbput_recordvars(tablePtr,3,
					"id",LT_TYPE_STRING,tempRow[0],
					"rulevid",LT_TYPE_STRING,tempRow[1],
					 type,LT_TYPE_STRING,"selected");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);

	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/videntityfilter.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
}


