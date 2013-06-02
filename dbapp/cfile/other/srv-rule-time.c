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
#include "msa.h"

int msaServerPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*
**时间策略加载页面
*/
int ltschinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
msaServerPage(confd,ltMsgPk,lt_MMHead);
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char mytime[400];
    memset(mytime,0,sizeof(mytime));
    
    char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		}
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"infolist");
    sprintf(sqlBuf,"select schid,schname,schdesc,schtime,schtime1 from nasschedule");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
              tempRow= ltDbFetchRow(tempCursor);
              while(tempRow!=NULL){
                       sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);                
                       lt_dbput_recordvars(tablePtr,4,
                                "schid",LT_TYPE_STRING,tempRow[0],
                                "schname",LT_TYPE_STRING,tempRow[1],
                                "schdesc",LT_TYPE_STRING,tempRow[2],
                                "schtime",LT_TYPE_STRING,mytime);
                       tempRow= ltDbFetchRow(tempCursor);
              }
              ltDbCloseCursor(tempCursor);
    }
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/schedit.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);
    ltDbClose(G_DbCon);
    return 0;
}
/*
**添加新的时间策略
*/
int ltaddsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 nasschtime _nasschtime[NAS_MAXSCH_NUM];
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   long lCount=0;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
	 }
   memset((void *)_nasschtime,0,sizeof(nasschtime)*NAS_MAXSCH_NUM);
   
   sprintf(sqlBuf,"select count(*) from nasschedule");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
   }
   if(lCount == 0) {
         lCount=1;
   }else{
    sprintf(sqlBuf,"select max(schid)+1 from nasschedule");
    lCount = 1;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
     tempRow= ltDbFetchRow(tempCursor);
     if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
     }
     ltDbCloseCursor(tempCursor);
		}
   }       
    sprintf(sqlBuf,"insert into nasschedule values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf); 
    
		_ltPubInfo->_schtimelist[lCount].schid=lCount;
		strcpy(_ltPubInfo->_schtimelist[lCount].schname,ltMsgGetVar_s(ltMsgPk,"schname"));
		strcpy(_ltPubInfo->_schtimelist[lCount].schdesc,ltMsgGetVar_s(ltMsgPk,"schdesc"));
		strcpy(_ltPubInfo->_schtimelist[lCount].schtime,ltMsgGetVar_s(ltMsgPk,"schtime"));
		
		snprintf(caMsg,255,"%s","add nasschedule success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltschinfolist(confd,ltMsgPk,lt_MMHead);
 return 0;
}
/*
**修改时间策略
*/
int ltupdatesch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
	 }
   
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   sprintf(sqlBuf,"update nasschedule set schname='%s',schdesc='%s',schtime='%s',schtime1='%s' where schid=%s",
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"schid"));
   ltDbExecSql(G_DbCon,sqlBuf); 
   
	 strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schname,ltMsgGetVar_s(ltMsgPk,"schname"));
	 strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schdesc,ltMsgGetVar_s(ltMsgPk,"schdesc"));
	 strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schtime,ltMsgGetVar_s(ltMsgPk,"schtime"));
   
   snprintf(caMsg,255,"%s","update nasschedule success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltschinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除时间策略
*/
int ltdelsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1048];
         char caMsg[256];
         char *dbUser;
				 char *dbPass;
				 char *dbName;
				 dbName=_ltPubInfo->_dbname;
				 dbUser=_ltPubInfo->_dbuser;
				 dbPass=_ltPubInfo->_dbpass;
				 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
				 }
        sprintf(sqlBuf,"delete from nasschedule where schid in (%s) ",
        ltMsgGetVar_s(ltMsgPk,"checkbox"));
        ltDbExecSql(G_DbCon,sqlBuf); 
        
			  _ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schid=0;
			  strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schname,"");
			  strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schdesc,"");
			  strcpy(_ltPubInfo->_schtimelist[atoi(ltMsgGetVar_s(ltMsgPk,"schid"))].schtime,"");
	 
			  snprintf(caMsg,255,"%s","delete nasschedule success.");
				logWrite(ltMsgPk,caMsg,3);
				ltDbClose(G_DbCon);
        ltschinfolist(confd,ltMsgPk,lt_MMHead);
        return 0;
}
/*
**节假日加载页面
*/
int ltjjrinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){	
 	 char sqlBuf[1024];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char mytime[400];
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	 }
   
   memset(mytime,0,sizeof(mytime));
//   if(checkRight(confd,ltMsgPk,21)==-1){//权限判断
//             return -1;
//   }
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   tablePtr=lt_dbput_table(dbPtr,"infolist");
   sprintf(sqlBuf,"select jjrid,jjrname,jjrdesc,jjrtime,jjrtime1 from nasjjr");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             while(tempRow!=NULL){
                      sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);                
                      lt_dbput_recordvars(tablePtr,4,
                               "jjrid",LT_TYPE_STRING,tempRow[0],
                               "jjrname",LT_TYPE_STRING,tempRow[1],
                               "jjrdesc",LT_TYPE_STRING,tempRow[2],
                               "jjrtime",LT_TYPE_STRING,mytime);
                      tempRow= ltDbFetchRow(tempCursor);
             }
             ltDbCloseCursor(tempCursor);
   }
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/jjredit.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 ltDbClose(G_DbCon);
	 return 0;
}


/*
**添加新的节假日
*/
int ltaddjjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   long lCount=0;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	 }
   
   sprintf(sqlBuf,"select count(*) from nasjjr");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
   }
   if(lCount == 0) {
         lCount=1;
   }else{
    sprintf(sqlBuf,"select max(jjrid)+1 from nasjjr");
    lCount = 1;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
     tempRow= ltDbFetchRow(tempCursor);
     if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
     }
     ltDbCloseCursor(tempCursor);
		}
   }       
    sprintf(sqlBuf,"insert into nasjjr values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf); 
        
		_ltPubInfo->_jjrtimelist[lCount].jjrid=lCount;
		strcpy(_ltPubInfo->_jjrtimelist[lCount].jjrname,ltMsgGetVar_s(ltMsgPk,"jjrname"));
		strcpy(_ltPubInfo->_jjrtimelist[lCount].jjrdesc,ltMsgGetVar_s(ltMsgPk,"jjrdesc"));
		strcpy(_ltPubInfo->_jjrtimelist[lCount].jjrtime,ltMsgGetVar_s(ltMsgPk,"jjrtime"));
    snprintf(caMsg,255,"%s","add nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}

/*
**修改节假日
*/
int ltupdatejjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	 }
   
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   sprintf(sqlBuf,"update nasjjr set jjrname='%s',jjrdesc='%s',jjrtime='%s',jjrtime1='%s' where jjrid=%s",
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"jjrid"));
   ltDbExecSql(G_DbCon,sqlBuf); 
           
	 strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrname,ltMsgGetVar_s(ltMsgPk,"jjrname"));
	 strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrdesc,ltMsgGetVar_s(ltMsgPk,"jjrdesc"));
	 strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrtime,ltMsgGetVar_s(ltMsgPk,"jjrtime"));
		
   snprintf(caMsg,255,"%s","update nasjjr success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除节假日
*/
int ltdeljjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sqlBuf[1048];
    char caMsg[256];
    char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
	  dbUser=_ltPubInfo->_dbuser;
	  dbPass=_ltPubInfo->_dbpass;
	  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	  if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	  }
    sprintf(sqlBuf,"delete from nasjjr where jjrid in (%s) ",
    ltMsgGetVar_s(ltMsgPk,"checkbox"));
    ltDbExecSql(G_DbCon,sqlBuf); 
               
	  _ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrid=0;
	  strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrname,"");
	  strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrdesc,"");
	  strcpy(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrtime,""); 
    snprintf(caMsg,255,"%s","delete nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
    ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}
int msaServerPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	_ltPubInfo->lRunning=0;
	msaInitServiceArray(msaPubInfo  *ltPubInfo);
	_ltPubInfo->lRunning=1;
	printf("_ltPubInfo->lRunning:%d\n",_ltPubInfo->lRunning);
	return 0;
}
