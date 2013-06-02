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


/*加载服务类型页面*/
int msasrvlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/srv.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

//实现编辑更新  记录删除  grid 的json构造
int ltSrvlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		 char *casrvid;
		 char *casrvname;
 		 char *camatchDirect;
 		 char *camatchAgree;
 		 char *camatchportL;
 		 char *camatchportU;
 		 char *camatchload;
 		 char *casrvip;
 		 char *casrvarea;
 		 char *cadescmatch1;
 		 char *cadescmatch2;
 		 char *cadescmatch3;
 		 char *caascmatch1;
	   char *caascmatch2;
 		 char *caascmatch3;
 		 int srvascmatchtype;
 		 
 		 srvascmatchtype=0;

	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
		 
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
		 
	   dbPtr=lt_dbinit();
	   char caMsg[256];
	   lt_db_htmlpage(dbPtr,"utf-8");

    casrvid=ltMsgGetVar_s(ltMsgPk,"srvid");
    casrvname=ltMsgGetVar_s(ltMsgPk,"srvname");
		camatchDirect=ltMsgGetVar_s(ltMsgPk,"matchDirect");
		camatchAgree=ltMsgGetVar_s(ltMsgPk,"matchAgree");
		camatchportL=ltMsgGetVar_s(ltMsgPk,"matchportL");
		camatchportU=ltMsgGetVar_s(ltMsgPk,"matchportU");
		camatchload=ltMsgGetVar_s(ltMsgPk,"matchload");
		casrvip=ltMsgGetVar_s(ltMsgPk,"srvip");
		casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
		cadescmatch1=ltMsgGetVar_s(ltMsgPk,"srvdescmatch1");
		cadescmatch2=ltMsgGetVar_s(ltMsgPk,"srvdescmatch2");
		cadescmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
		caascmatch1=ltMsgGetVar_s(ltMsgPk,"srvascmatch1");
		caascmatch2=ltMsgGetVar_s(ltMsgPk,"srvascmatch2");
		caascmatch3=ltMsgGetVar_s(ltMsgPk,"srvascmatch3");
		if(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype")){
			srvascmatchtype=atoi(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype"));
		}
		
		
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL){
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1){
				 sprintf(sqlBuf,"delete from nassrv where srvid='%s'",casrvid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete nassrv success.");
			}
			if(cadeflag==2){//编辑
				char *myid;
				 myid==ltMsgGetVar_s(ltMsgPk,"srvid");
				 sprintf(sqlBuf,"update nassrv set srvname='%s',matchDirect='%s',matchAgree='%s',matchportL='%s',matchportU='%s',matchload='%s',srvip='%s',srvarea='%s',srvascmatchtype=%d,srvdescmatch1='%s',srvdescmatch2='%s',srvdescmatch3='%s',srvascmatch1='%s',srvascmatch2='%s',srvascmatch3='%s' where srvid='%s'",casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3,casrvid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","update nassrv success.");
			}
				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
		}

    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0;
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }

    sprintf(strBuf,"select count(*) from nassrv");
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL){
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL){
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    memset(strBuf,0,sizeof(strBuf));
    sprintf(strBuf,"select srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,srvip,srvarea,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3 from nassrv order by srvid  limit %lu offset %lu ",lRowNum,lStartRec);
    printf("strBuf:%s\n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL){
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL){
          if(k==0){
            sprintf(caTmpp,"{\"srvid\":\"%s\",\"srvname\":\"%s\",\"matchDirect\":\"%s\",\"matchAgree\":\"%s\",\"matchport\":\"%s-%s\",\"matchload\":\"%s\",\"srvip\":\"%s\",\"srvarea\":\"%s\",\"srvdescmatch1\":\"%s\",\"srvdescmatch2\":\"%s\",\"srvdescmatch3\":\"%s\",\"srvascmatch1\":\"%s\",\"srvascmatch2\":\"%s\",\"srvascmatch3\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
						stralloc_cats(&strTemp,caTmpp);
					}
					else{
				    sprintf(caTmpp,",{\"srvid\":\"%s\",\"srvname\":\"%s\",\"matchDirect\":\"%s\",\"matchAgree\":\"%s\",\"matchport\":\"%s-%s\",\"matchload\":\"%s\",\"srvip\":\"%s\",\"srvarea\":\"%s\",\"srvdescmatch1\":\"%s\",\"srvdescmatch2\":\"%s\",\"srvdescmatch3\":\"%s\",\"srvascmatch1\":\"%s\",\"srvascmatch2\":\"%s\",\"srvascmatch3\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
		        stralloc_cats(&strTemp,caTmpp);
					}
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		   }
       ltDbCloseCursor(tempCursor);
    }
    stralloc_cats(&strTemp,"]}");
    stralloc_0(&strTemp);
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    ltMsgFree(ltMsgPk);
    ltDbClose(G_DbCon);
    return 0;
}



//实现显示编辑页面功能
int msaShowSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL)
	 {
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,srvip,srvarea,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3  from nassrv where srvid='%s'",my_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL){
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL){
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);
						 
						 char str[16];
						 lt_dbput_rootvars(dbPtr,1,"srvname",tempRow[0]);
						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"direct%ssel",tempRow[1]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"agree%ssel",tempRow[2]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 lt_dbput_rootvars(dbPtr,1,"matchportL",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"matchportU",tempRow[4]);
 						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"load%ssel",tempRow[5]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
						 
						 lt_dbput_rootvars(dbPtr,1,"srvip",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"srvarea",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch1",tempRow[8]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch2",tempRow[9]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch3",tempRow[10]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch1",tempRow[11]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch2",tempRow[12]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch3",tempRow[13]);
						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}


/*
**主页面 实现添加新记录功能
*/
int ltSrvaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	 char *casrvname;
	 long  camatchDirect;
	 long  camatchAgree;
	 char *camatchportL;
	 char *camatchportU;
	 long  camatchload;
	 char *casrvip;
	 char *casrvarea;
	 long cadescmatch1;
	 long cadescmatch2;
	 char *cadescmatch3;
	 long caascmatch1;
   long caascmatch2;
	 char *caascmatch3;
	 int srvascmatchtype;
 	 srvascmatchtype=0;
		
   char sqlBuf[2048];
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char caMsg[256];
   long lCount;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   
   sprintf(sqlBuf,"select count(*) from nassrv");
   lCount =0;
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
        sprintf(sqlBuf,"select max(srvid)+1 from nassrv");
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

   if(ltMsgGetVar_s(ltMsgPk,"srvname")){
   	 casrvname=ltMsgGetVar_s(ltMsgPk,"srvname");
   }else{
   	 casrvname=" ";
   }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchDirect")){
	 	 camatchDirect=atol(ltMsgGetVar_s(ltMsgPk,"matchDirect"));
	 }else{
	 	 camatchDirect=0;
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"matchAgree")){
	 	 camatchAgree=atol(ltMsgGetVar_s(ltMsgPk,"matchAgree"));
	 }else{
	 	 camatchAgree=0;
	 }
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchportL")){
	 	 camatchportL=ltMsgGetVar_s(ltMsgPk,"matchportL");
	 }else{
	 	 camatchportL=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchportU")){
	 	 camatchportU=ltMsgGetVar_s(ltMsgPk,"matchportU");
	 }else{
	 	 camatchportU=" ";
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"matchload")){
	 	 camatchload=atol(ltMsgGetVar_s(ltMsgPk,"matchload"));
	 }else{
	 	 camatchload=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvip")){
	 	 casrvip=ltMsgGetVar_s(ltMsgPk,"srvip");
	 }else{
	 	 casrvip=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvarea")){
	 	 casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
	 }else{
	 	 casrvarea=" "; 
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvarea")){
	 	 casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
	 }else{
	 	 casrvarea=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch1")){
	 	  cadescmatch1=atol(ltMsgGetVar_s(ltMsgPk,"srvdescmatch1"));
	 }else{
	 	  cadescmatch1=0;
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch2")){
	 	  cadescmatch2=atol(ltMsgGetVar_s(ltMsgPk,"srvdescmatch2"));
	 }else{
	 	  cadescmatch2=0;
	 }
	
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch3")){//字节是否在0到e
	 	 cadescmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
	 }else{
	 	 cadescmatch3="";
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatch1")){
	 	 caascmatch1=atol(ltMsgGetVar_s(ltMsgPk,"srvascmatch1"));
	 }else{
	 	 caascmatch1=0; 
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatch2")){
	 	 caascmatch2=atol(ltMsgGetVar_s(ltMsgPk,"srvascmatch2"));
	 }else{
	 	 caascmatch2=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch3")){
	 	 caascmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
	 }else{
	 	 caascmatch3="";
	 }
	 
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype")){
			srvascmatchtype=atoi(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype"));
	 }else{
	 	  srvascmatchtype=0;
	 }
	 
	 if(srvascmatchtype==0){//and操作
	 	    if(strlen(cadescmatch3)>0 && strlen(caascmatch3)>0){
	 	      sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,\
           srvip,srvarea,srvascmatchtype,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d,%ld,%ld,'%s',%ld,%ld,'%s')",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,
           casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3);
        }
	 	
	 }else{//or操作
	 	 if(strlen(cadescmatch3)>0 || strlen(caascmatch3)>0){
	 	 	   sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,\
           srvip,srvarea,srvascmatchtype,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d,%ld,%ld,'%s',%ld,%ld,'%s')",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,
           casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3);
	 	 }else{
	 	 	  sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,\
	 	 	     matchportL,matchportU,matchload,srvip,srvarea,srvascmatchtype)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d)",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,casrvarea,srvascmatchtype);
	 	 }
	 	
	 
	 }
	 
  ltDbExecSql(G_DbCon,sqlBuf);
  snprintf(caMsg,255,"%s","add nassrv success.");
  logWrite(ltMsgPk,caMsg,3);
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  ltDbClose(G_DbCon); 
  return 0;
}

