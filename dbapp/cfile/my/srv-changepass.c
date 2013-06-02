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
#define MAX_POSTKEY_NUM  256
#define ROUTCONFIG_PATH "/etc/msa/msa/routconfig"


int msauserchpass(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char *username;
	char *password;
	char *pass;
	char *repass;
	char newPass[50];
	char sqlBuf[1024];
	nasDbUserInfo     *dbUserInfo;
	int  passisokA;
	int  passisokB;
	
	char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	}
	
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(newPass,0,sizeof(newPass));
	if(ltMsgGetVar_s(ltMsgPk,"username")){
		username=ltMsgGetVar_s(ltMsgPk,"username");
	}else{
		username="";
	}

	if(ltMsgGetVar_s(ltMsgPk,"password")){
		password=ltMsgGetVar_s(ltMsgPk,"password");
	}else{
		password="";
	}

	if(ltMsgGetVar_s(ltMsgPk,"pass")){//新密码
		pass=ltMsgGetVar_s(ltMsgPk,"pass");
	}else{
		pass="";
	}

	if(ltMsgGetVar_s(ltMsgPk,"repass")){//确认密码
		repass=ltMsgGetVar_s(ltMsgPk,"repass");
	}else{
	  repass="";	
	}
	
	dbUserInfo=msaLookUpDbUserByName(_ltPubInfo,username);
	if(!dbUserInfo){/* 用户不存在 */  	
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >用户名错误，请重新输入!</body></html>");
		ltMsgFree(ltMsgPk);
		return 0;
	} 
	
	
	ltMd5Encrypto(password, strlen(password),newPass);
	if(strcmp(dbUserInfo->password,newPass)!=0){//原始密码判断
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >密码错误，请重新输入!</body></html>");
		ltMsgFree(ltMsgPk);
		return 0;
	}
	
	
	passisokA=0;
	passisokB=0;
  passisokA=ltPassIsOk(pass);
	if(passisokA==0){
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >新密码输入错误，请重新输入!</body></html>");
		ltMsgFree(ltMsgPk);
		return 0;
	}
	
	passisokB=ltPassIsOk(repass);
	if(passisokB==0){
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >确认新密码输入错误，请重新输入!</body></html>");
		ltMsgFree(ltMsgPk);
		return 0;
	}
	
	if(strcmp(pass,repass)!=0){
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >新密码与确认新密码输入不一致，请重新输入!</body></html>");
		ltMsgFree(ltMsgPk);
		return 0;
	}
	
	
	memset(newPass,0,sizeof(newPass));
	ltMd5Encrypto(pass, strlen(pass),newPass);
	sprintf(dbUserInfo->password,"%s",newPass);
	sprintf(sqlBuf,"update msauser set password='%s' where username='%s' ",newPass,username);//修改数据库
	ltDbExecSql(G_DbCon,sqlBuf);

   
 	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >密码修改成功!</body></html>");
	ltMsgFree(ltMsgPk);
  
  return 0;

}


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
		if(G_DbCon!=NULL){
			//printf("db connect ok\n");
		}else{
			fprintf(stderr,"db connect error\n");
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
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/videntityfilter.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
}


int msasecurityset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *closeping;
   char *sysdnoaccess;
   char *strenadpass;
   char *strenuserpass;
   char *notestservice;
   char *closessh;
	

	 if(ltMsgGetVar_s(ltMsgPk,"closeping")){
	 	 closeping=ltMsgGetVar_s(ltMsgPk,"closeping");
	 }else{
	 	 closeping="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"sysdnoaccess")){
	 	 sysdnoaccess=ltMsgGetVar_s(ltMsgPk,"sysdnoaccess");
	 }else{
	   sysdnoaccess="no";	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"strenadpass")){
	 	 strenadpass=ltMsgGetVar_s(ltMsgPk,"strenadpass");
	 }else{
	   strenadpass="no";	
	 }
	  
	 if(ltMsgGetVar_s(ltMsgPk,"strenuserpass")){
	 	 strenuserpass=ltMsgGetVar_s(ltMsgPk,"strenuserpass");
	 }else{
	   strenuserpass="no";	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"notestservice")){
	 	 notestservice=ltMsgGetVar_s(ltMsgPk,"notestservice");
	 }else{
	   notestservice="no";	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"closessh")){
	 	 closessh=ltMsgGetVar_s(ltMsgPk,"closessh");
	 }else{
	   closessh="no";	
	 }
	 
	 printf("closeping:%s\n",closeping);
	 printf("sysdnoaccess:%s\n",sysdnoaccess);
	 printf("strenadpass:%s\n",strenadpass);
	 printf("strenuserpass:%s\n",strenuserpass);
	 printf("notestservice:%s\n",notestservice);
	 printf("closessh:%s\n",closessh);

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >系统安全设置成功!</body></html>");
	ltMsgFree(ltMsgPk);
   
   return 0;
}



int msaroutsetinglink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

 	 ltDbHeadPtr dbPtr;
 	 char caFile[128];
 	 char dip[64];
 	 char subask[64];
 	 char getway[64];
 	 char start[64];
 	 
 	 char Html_dip[64];
 	 char Html_subask[64];
 	 char Html_getway[64];
 	 char Html_start[64];
 	 int i=0;
 	 struct stat st; 
 	 memset(caFile,0,sizeof(caFile));
 	 
 	 memset(dip,0,sizeof(dip));
 	 memset(subask,0,sizeof(subask));
 	 memset(getway,0,sizeof(getway));
 	 memset(start,0,sizeof(start));
 	 
 	 memset(Html_dip,0,sizeof(Html_dip));
 	 memset(Html_subask,0,sizeof(Html_subask));
 	 memset(Html_getway,0,sizeof(Html_getway));
 	 memset(Html_start,0,sizeof(Html_start));
 	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
	 sprintf(caFile,"%s/routsetting.ini",ROUTCONFIG_PATH);
	 if(stat(caFile,&st) == 0){	
	 		 for(i =1 ;i< 21;i++){
	 		 	 sprintf(dip,"dip%d",i);
	 		 	 sprintf(subask,"subask%d",i);
	 		 	 sprintf(getway,"getway%d",i);
	 		 	 sprintf(start,"start%d",i);
			 	 bcCnfGetValue_s(caFile,dip,Html_dip);
			 	 bcCnfGetValue_s(caFile,subask,Html_subask);
		     bcCnfGetValue_s(caFile,getway,Html_getway);
		     bcCnfGetValue_s(caFile,start,Html_start);
		     
		     if(strcmp(Html_start,"yes")==0){
		       lt_dbput_rootvars(dbPtr,4,dip,Html_dip,subask,Html_subask,getway,Html_getway,start,"checked");
		     }else{
		     	 lt_dbput_rootvars(dbPtr,4,dip,Html_dip,subask,Html_subask,getway,Html_getway,start,"");
		     }
		     
		    
	     }
	 
	 }
	 


	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/routseting.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
}

int msaroutseting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char *dns;//DNS
	char *bakdns;//备用DNS
	char *dfgetway;//默认网关
	
	
	/*
		配置静态路由参数
	*/
	char *dip1,*dip2,*dip3,*dip4,*dip5;
	char *dip6,*dip7,*dip8,*dip9,*dip10;
	char *dip11,*dip12,*dip13,*dip14,*dip15;
	char *dip16,*dip17,*dip18,*dip19,*dip20;
	
	char *subask1,*subask2,*subask3,*subask4,*subask5;
	char *subask6,*subask7,*subask8,*subask9,*subask10;
	char *subask11,*subask12,*subask13,*subask14,*subask15;
	char *subask16,*subask17,*subask18,*subask19,*subask20;
	
	char *getway1,*getway2,*getway3,*getway4,*getway5;
	char *getway6,*getway7,*getway8,*getway9,*getway10;
	char *getway11,*getway12,*getway13,*getway14,*getway15;
	char *getway16,*getway17,*getway18,*getway19,*getway20;
	
	char *start1,*start2,*start3,*start4,*start5;
	char *start6,*start7,*start8,*start9,*start10;
	char *start11,*start12,*start13,*start14,*start15;
	char *start16,*start17,*start18,*start19,*start20;
	
	char   caFile[256];
	char   caFile1[256];
	char   caCmd[1024];
	int    fdwrite=0;
	int    fd=0;
	struct stat st; 

	 if(ltMsgGetVar_s(ltMsgPk,"dns")){
	 	 dns=ltMsgGetVar_s(ltMsgPk,"dns");
	 }else{
	 	 dns="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"bakdns")){
	 	 bakdns=ltMsgGetVar_s(ltMsgPk,"bakdns");
	 }else{
	 	 bakdns="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dfgetway")){
	 	 dfgetway=ltMsgGetVar_s(ltMsgPk,"dfgetway");
	 }else{
	 	 dfgetway="";
	 }
	 
	 /*-----------------------1----------------------------*/
	 if(ltMsgGetVar_s(ltMsgPk,"dip1")){
	 	 dip1=ltMsgGetVar_s(ltMsgPk,"dip1");
	 }else{
	 	 dip1="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask1")){
	 	 subask1=ltMsgGetVar_s(ltMsgPk,"subask1");
	 }else{
	 	 subask1="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway1")){
	 	 getway1=ltMsgGetVar_s(ltMsgPk,"getway1");
	 }else{
	 	 getway1="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start1")){
	 	 start1=ltMsgGetVar_s(ltMsgPk,"start1");
	 }else{
	 	 start1="no";
	 }
	 
	  /*-----------------------2----------------------------*/
	 if(ltMsgGetVar_s(ltMsgPk,"dip2")){
	 	 dip2=ltMsgGetVar_s(ltMsgPk,"dip2");
	 }else{
	 	 dip2="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask2")){
	 	 subask2=ltMsgGetVar_s(ltMsgPk,"subask2");
	 }else{
	 	 subask2="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway2")){
	 	 getway2=ltMsgGetVar_s(ltMsgPk,"getway2");
	 }else{
	 	 getway2="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start2")){
	 	 start2=ltMsgGetVar_s(ltMsgPk,"start2");
	 }else{
	 	 start2="no";
	 }
	 
	 
	  /*-----------------------3----------------------------*/
	 if(ltMsgGetVar_s(ltMsgPk,"dip3")){
	 	 dip3=ltMsgGetVar_s(ltMsgPk,"dip3");
	 }else{
	 	 dip3="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask3")){
	 	 subask3=ltMsgGetVar_s(ltMsgPk,"subask3");
	 }else{
	 	 subask3="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway3")){
	 	 getway3=ltMsgGetVar_s(ltMsgPk,"getway3");
	 }else{
	 	 getway3="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start3")){
	 	 start3=ltMsgGetVar_s(ltMsgPk,"start3");
	 }else{
	 	 start3="no";
	 }
	 
	 	  /*-----------------------4----------------------------*/
	 if(ltMsgGetVar_s(ltMsgPk,"dip4")){
	 	 dip4=ltMsgGetVar_s(ltMsgPk,"dip4");
	 }else{
	 	 dip4="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask4")){
	 	 subask4=ltMsgGetVar_s(ltMsgPk,"subask4");
	 }else{
	 	 subask4="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway4")){
	 	 getway4=ltMsgGetVar_s(ltMsgPk,"getway4");
	 }else{
	 	 getway4="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start4")){
	 	 start4=ltMsgGetVar_s(ltMsgPk,"start4");
	 }else{
	 	 start4="no";
	 }
	 
	 /*-----------------------5到20-----------------------*/
	 if(ltMsgGetVar_s(ltMsgPk,"dip5")){
	 	 dip5=ltMsgGetVar_s(ltMsgPk,"dip5");
	 }else{
	 	 dip5="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip6")){
	 	 dip6=ltMsgGetVar_s(ltMsgPk,"dip6");
	 }else{
	 	 dip6="";
	 }
	 
	  if(ltMsgGetVar_s(ltMsgPk,"dip7")){
	 	 dip7=ltMsgGetVar_s(ltMsgPk,"dip7");
	 }else{
	 	 dip7="";
	 }
	 
	  if(ltMsgGetVar_s(ltMsgPk,"dip8")){
	 	 dip8=ltMsgGetVar_s(ltMsgPk,"dip8");
	 }else{
	 	 dip8="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip9")){
	 	 dip9=ltMsgGetVar_s(ltMsgPk,"dip9");
	 }else{
	 	 dip9="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip10")){
	 	 dip10=ltMsgGetVar_s(ltMsgPk,"dip10");
	 }else{
	 	 dip10="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip11")){
	 	 dip11=ltMsgGetVar_s(ltMsgPk,"dip11");
	 }else{
	 	 dip11="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip12")){
	 	 dip12=ltMsgGetVar_s(ltMsgPk,"dip12");
	 }else{
	 	 dip12="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip13")){
	 	 dip13=ltMsgGetVar_s(ltMsgPk,"dip13");
	 }else{
	 	 dip13="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip14")){
	 	 dip14=ltMsgGetVar_s(ltMsgPk,"dip14");
	 }else{
	 	 dip14="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip15")){
	 	 dip15=ltMsgGetVar_s(ltMsgPk,"dip15");
	 }else{
	 	 dip15="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip16")){
	 	 dip16=ltMsgGetVar_s(ltMsgPk,"dip16");
	 }else{
	 	 dip16="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip17")){
	 	 dip17=ltMsgGetVar_s(ltMsgPk,"dip17");
	 }else{
	 	 dip17="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip18")){
	 	 dip18=ltMsgGetVar_s(ltMsgPk,"dip18");
	 }else{
	 	 dip18="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip19")){
	 	 dip19=ltMsgGetVar_s(ltMsgPk,"dip19");
	 }else{
	 	 dip19="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"dip20")){
	 	 dip20=ltMsgGetVar_s(ltMsgPk,"dip20");
	 }else{
	 	 dip20="";
	 }
	 
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask5")){
	 	 subask5=ltMsgGetVar_s(ltMsgPk,"subask5");
	 }else{
	 	 subask5="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask6")){
	 	 subask6=ltMsgGetVar_s(ltMsgPk,"subask6");
	 }else{
	 	 subask6="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask7")){
	 	 subask7=ltMsgGetVar_s(ltMsgPk,"subask7");
	 }else{
	 	 subask7="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask8")){
	 	 subask8=ltMsgGetVar_s(ltMsgPk,"subask8");
	 }else{
	 	 subask8="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask9")){
	 	 subask9=ltMsgGetVar_s(ltMsgPk,"subask9");
	 }else{
	 	 subask9="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask10")){
	 	 subask10=ltMsgGetVar_s(ltMsgPk,"subask10");
	 }else{
	 	 subask10="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask11")){
	 	 subask11=ltMsgGetVar_s(ltMsgPk,"subask11");
	 }else{
	 	 subask11="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask12")){
	 	 subask12=ltMsgGetVar_s(ltMsgPk,"subask12");
	 }else{
	 	 subask12="";
	 } 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask13")){
	 	 subask13=ltMsgGetVar_s(ltMsgPk,"subask13");
	 }else{
	 	 subask13="";
	 } 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask14")){
	 	 subask14=ltMsgGetVar_s(ltMsgPk,"subask14");
	 }else{
	 	 subask14="";
	 } 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask15")){
	 	 subask15=ltMsgGetVar_s(ltMsgPk,"subask15");
	 }else{
	 	 subask15="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask16")){
	 	 subask16=ltMsgGetVar_s(ltMsgPk,"subask16");
	 }else{
	 	 subask16="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask17")){
	 	 subask17=ltMsgGetVar_s(ltMsgPk,"subask17");
	 }else{
	 	 subask17="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask18")){
	 	 subask18=ltMsgGetVar_s(ltMsgPk,"subask18");
	 }else{
	 	 subask18="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask19")){
	 	 subask19=ltMsgGetVar_s(ltMsgPk,"subask19");
	 }else{
	 	 subask19="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"subask20")){
	 	 subask20=ltMsgGetVar_s(ltMsgPk,"subask20");
	 }else{
	 	 subask20="";
	 }
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway5")){
	 	 getway5=ltMsgGetVar_s(ltMsgPk,"getway5");
	 }else{
	 	 getway5="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway6")){
	 	 getway6=ltMsgGetVar_s(ltMsgPk,"getway6");
	 }else{
	 	 getway6="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway7")){
	 	 getway7=ltMsgGetVar_s(ltMsgPk,"getway7");
	 }else{
	 	 getway7="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway8")){
	 	 getway8=ltMsgGetVar_s(ltMsgPk,"getway8");
	 }else{
	 	 getway8="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway9")){
	 	 getway9=ltMsgGetVar_s(ltMsgPk,"getway9");
	 }else{
	 	 getway9="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway10")){
	 	 getway10=ltMsgGetVar_s(ltMsgPk,"getway10");
	 }else{
	 	 getway10="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway11")){
	 	 getway11=ltMsgGetVar_s(ltMsgPk,"getway11");
	 }else{
	 	 getway11="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway12")){
	 	 getway12=ltMsgGetVar_s(ltMsgPk,"getway12");
	 }else{
	 	 getway12="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway13")){
	 	 getway13=ltMsgGetVar_s(ltMsgPk,"getway13");
	 }else{
	 	 getway13="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway14")){
	 	 getway14=ltMsgGetVar_s(ltMsgPk,"getway14");
	 }else{
	 	 getway14="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway15")){
	 	 getway15=ltMsgGetVar_s(ltMsgPk,"getway15");
	 }else{
	 	 getway15="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway16")){
	 	 getway16=ltMsgGetVar_s(ltMsgPk,"getway16");
	 }else{
	 	 getway16="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway17")){
	 	 getway17=ltMsgGetVar_s(ltMsgPk,"getway17");
	 }else{
	 	 getway17="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway18")){
	 	 getway18=ltMsgGetVar_s(ltMsgPk,"getway18");
	 }else{
	 	 getway18="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway19")){
	 	 getway19=ltMsgGetVar_s(ltMsgPk,"getway19");
	 }else{
	 	 getway19="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"getway20")){
	 	 getway20=ltMsgGetVar_s(ltMsgPk,"getway20");
	 }else{
	 	 getway20="";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start5")){
	 	 start5=ltMsgGetVar_s(ltMsgPk,"start5");
	 }else{
	 	 start5="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start6")){
	 	 start6=ltMsgGetVar_s(ltMsgPk,"start6");
	 }else{
	 	 start6="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start7")){
	 	 start7=ltMsgGetVar_s(ltMsgPk,"start7");
	 }else{
	 	 start7="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start8")){
	 	 start8=ltMsgGetVar_s(ltMsgPk,"start8");
	 }else{
	 	 start8="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start9")){
	 	 start9=ltMsgGetVar_s(ltMsgPk,"start9");
	 }else{
	 	 start9="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start10")){
	 	 start10=ltMsgGetVar_s(ltMsgPk,"start10");
	 }else{
	 	 start10="no";
	 }
		 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start11")){
	 	 start11=ltMsgGetVar_s(ltMsgPk,"start11");
	 }else{
	 	 start11="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start12")){
	 	 start12=ltMsgGetVar_s(ltMsgPk,"start12");
	 }else{
	 	 start12="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start13")){
	 	 start13=ltMsgGetVar_s(ltMsgPk,"start13");
	 }else{
	 	 start13="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start14")){
	 	 start14=ltMsgGetVar_s(ltMsgPk,"start14");
	 }else{
	 	 start14="no";
	 } 
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start15")){
	 	 start15=ltMsgGetVar_s(ltMsgPk,"start15");
	 }else{
	 	 start15="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start16")){
	 	 start16=ltMsgGetVar_s(ltMsgPk,"start16");
	 }else{
	 	 start16="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start17")){
	 	 start17=ltMsgGetVar_s(ltMsgPk,"start17");
	 }else{
	 	 start17="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start18")){
	 	 start18=ltMsgGetVar_s(ltMsgPk,"start18");
	 }else{
	 	 start18="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start19")){
	 	 start19=ltMsgGetVar_s(ltMsgPk,"start19");
	 }else{
	 	 start19="no";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start20")){
	 	 start20=ltMsgGetVar_s(ltMsgPk,"start20");
	 }else{
	 	 start20="no";
	 }
	 
	 

	  sprintf(caFile,"%s/routsetting.ini",ROUTCONFIG_PATH);
		fdwrite = open(caFile,O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >配置失败!</body></html>");
			ltMsgFree(ltMsgPk);
	 		return 0;
		}
		
		sprintf(caCmd,"DNS=%s\n",dns);
		write(fdwrite,caCmd,strlen(caCmd));
		
		sprintf(caCmd,"BAkDNS=%s\n",bakdns);
		write(fdwrite,caCmd,strlen(caCmd));
		
		sprintf(caCmd,"DGateway=%s\n",dfgetway);
		write(fdwrite,caCmd,strlen(caCmd));
		

		//if(strlen(dip1)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip1=%s\n",dip1);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask1=%s\n",subask1);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway1=%s\n",getway1);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start1=%s\n",start1);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		

		//if(strlen(dip2)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip2=%s\n",dip2);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask2=%s\n",subask2);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway2=%s\n",getway2);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start2=%s\n",start2);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		

		//if(strlen(dip3)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip3=%s\n",dip3);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask3=%s\n",subask3);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway3=%s\n",getway3);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start3=%s\n",start3);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
	
		
		//if(strlen(dip4)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip4=%s\n",dip4);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask4=%s\n",subask4);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway4=%s\n",getway4);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start4=%s\n",start4);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip5)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip5=%s\n",dip5);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask5=%s\n",subask5);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway5=%s\n",getway5);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start5=%s\n",start5);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		
		//if(strlen(dip6)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip6=%s\n",dip6);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask6=%s\n",subask6);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway6=%s\n",getway6);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start6=%s\n",start6);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip7)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip7=%s\n",dip7);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask7=%s\n",subask7);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway7=%s\n",getway7);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start7=%s\n",start7);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip8)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip8=%s\n",dip8);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask8=%s\n",subask8);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway8=%s\n",getway8);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start8=%s\n",start8);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip9)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip9=%s\n",dip9);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask9=%s\n",subask9);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway9=%s\n",getway9);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start9=%s\n",start9);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip10)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip10=%s\n",dip10);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask10=%s\n",subask10);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway10=%s\n",getway10);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start10=%s\n",start10);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip11)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip11=%s\n",dip11);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask11=%s\n",subask11);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway11=%s\n",getway11);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start11=%s\n",start11);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip12)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip12=%s\n",dip12);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask12=%s\n",subask12);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway12=%s\n",getway12);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start12=%s\n",start12);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip13)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip13=%s\n",dip13);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask13=%s\n",subask13);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway13=%s\n",getway13);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start13=%s\n",start13);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip14)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip14=%s\n",dip14);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask14=%s\n",subask14);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway14=%s\n",getway14);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start14=%s\n",start14);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip15)>0){
			memset(caCmd,0,sizeof(caCmd));
		  sprintf(caCmd,"dip15=%s\n",dip15);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
		  sprintf(caCmd,"subask15=%s\n",subask15);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway15=%s\n",getway15);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start15=%s\n",start15);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip16)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip16=%s\n",dip16);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask16=%s\n",subask16);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway16=%s\n",getway16);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start16=%s\n",start16);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip17)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip17=%s\n",dip17);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask17=%s\n",subask17);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway17=%s\n",getway17);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start17=%s\n",start17);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip18)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip18=%s\n",dip18);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask18=%s\n",subask18);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway18=%s\n",getway18);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start18=%s\n",start18);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip19)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip19=%s\n",dip19);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask19=%s\n",subask19);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway19=%s\n",getway19);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start19=%s\n",start19);
			write(fdwrite,caCmd,strlen(caCmd));
		//}
		
		//if(strlen(dip20)>0){
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"dip20=%s\n",dip20);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"subask20=%s\n",subask20);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"getway20=%s\n",getway20);
			write(fdwrite,caCmd,strlen(caCmd));
			
			memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"start20=%s\n",start20);
			write(fdwrite,caCmd,strlen(caCmd));

		//}
		close(fdwrite);		
		
		
		 sprintf(caFile1,"/etc/msa/msa/start/rout.up");
		 fd = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);		
		 if(fd==-1){
		 	  
		 	  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
				"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >配置失败!</body></html>");
				ltMsgFree(ltMsgPk);
		 	
		    return 0;	
		 }
		
		
		sprintf(caFile,"%s/routsetting.ini",ROUTCONFIG_PATH);
		if(stat(caFile,&st) == 0){			
			 
			 bcCnfGetValue_s(caFile,"DNS",dns);
			 if(strlen(dns)>0){//首选DNS
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"nameserver %s > /etc/resovle.conf",dns);
					write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"BAkDNS",bakdns);
			 if(strlen(bakdns)>0){//默认DNS
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"nameserver %s >> /etc/resovle.conf",bakdns);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }

						
			 bcCnfGetValue_s(caFile,"DGateway",dfgetway);
			 if(strlen(dfgetway)>0){//添加默认网关
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add default gw %s",dfgetway);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 	  //system(caCmd);
    	 }
			
			
			  
			 bcCnfGetValue_s(caFile,"dip1",dip1);
    	 bcCnfGetValue_s(caFile,"subask1",subask1);
    	 bcCnfGetValue_s(caFile,"getway1",getway1);
    	 bcCnfGetValue_s(caFile,"start1",start1); 
    	 if(strcmp(start1,"yes")==0 && strlen(dip1)>0 &&strlen(subask1)>0 && strlen(getway1)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip1,subask1,getway1);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	   printf("33333333333333333333333333\n");	
    	 char tmpdip2[64];
    	 char tmpsubask2[64];
    	 char tmpgetway2[64];
    	 char tmpstart2[64];
    	 bcCnfGetValue_s(caFile,"dip2",tmpdip2);
    	 bcCnfGetValue_s(caFile,"subask2",tmpsubask2);
    	 bcCnfGetValue_s(caFile,"getway2",tmpgetway2);
    	 bcCnfGetValue_s(caFile,"start2",tmpstart2); 
    	  printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");	
    	 if(strcmp(start2,"yes")==0 && strlen(dip2)>0 &&strlen(subask2)>0 && strlen(getway2)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip2,subask2,getway2);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	  
    	 bcCnfGetValue_s(caFile,"dip3",dip3);
    	 bcCnfGetValue_s(caFile,"subask2",subask3);
    	 bcCnfGetValue_s(caFile,"getway3",getway3);
    	 bcCnfGetValue_s(caFile,"start3",start3); 
    	 if(strcmp(start3,"yes")==0 && strlen(dip3)>0 &&strlen(subask3)>0 && strlen(getway3)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip3,subask3,getway3);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip4",dip4);
    	 bcCnfGetValue_s(caFile,"subask4",subask4);
    	 bcCnfGetValue_s(caFile,"getway4",getway4);
    	 bcCnfGetValue_s(caFile,"start4",start4); 
    	 if(strcmp(start4,"yes")==0 && strlen(dip4)>0 &&strlen(subask4)>0 && strlen(getway4)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip4,subask4,getway4);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip5",dip5);
    	 bcCnfGetValue_s(caFile,"subask5",subask5);
    	 bcCnfGetValue_s(caFile,"getway5",getway5);
    	 bcCnfGetValue_s(caFile,"start5",start5); 
    	 if(strcmp(start5,"yes")==0 && strlen(dip5)>0 &&strlen(subask5)>0 && strlen(getway5)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip5,subask5,getway5);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip6",dip6);
    	 bcCnfGetValue_s(caFile,"subask6",subask6);
    	 bcCnfGetValue_s(caFile,"getway6",getway6);
    	 bcCnfGetValue_s(caFile,"start6",start6); 
    	 if(strcmp(start6,"yes")==0 && strlen(dip6)>0 &&strlen(subask6)>0 && strlen(getway6)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip6,subask6,getway6);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip7",dip7);
    	 bcCnfGetValue_s(caFile,"subask7",subask7);
    	 bcCnfGetValue_s(caFile,"getway7",getway7);
    	 bcCnfGetValue_s(caFile,"start7",start7); 
    	 if(strcmp(start7,"yes")==0 && strlen(dip7)>0 &&strlen(subask7)>0 && strlen(getway7)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip7,subask7,getway7);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip8",dip8);
    	 bcCnfGetValue_s(caFile,"subask8",subask8);
    	 bcCnfGetValue_s(caFile,"getway8",getway8);
    	 bcCnfGetValue_s(caFile,"start8",start8); 
    	 if(strcmp(start8,"yes")==0 && strlen(dip8)>0 &&strlen(subask8)>0 && strlen(getway8)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip8,subask8,getway8);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip9",dip9);
    	 bcCnfGetValue_s(caFile,"subask9",subask9);
    	 bcCnfGetValue_s(caFile,"getway9",getway9);
    	 bcCnfGetValue_s(caFile,"start9",start9); 
    	 if(strcmp(start9,"yes")==0 && strlen(dip9)>0 &&strlen(subask9)>0 && strlen(getway9)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip9,subask9,getway9);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip10",dip10);
    	 bcCnfGetValue_s(caFile,"subask10",subask10);
    	 bcCnfGetValue_s(caFile,"getway10",getway10);
    	 bcCnfGetValue_s(caFile,"start10",start10); 
    	 if(strcmp(start10,"yes")==0 && strlen(dip10)>0 &&strlen(subask10)>0 && strlen(getway10)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip10,subask10,getway10);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip11",dip11);
    	 bcCnfGetValue_s(caFile,"subask11",subask11);
    	 bcCnfGetValue_s(caFile,"getway11",getway11);
    	 bcCnfGetValue_s(caFile,"start11",start11); 
    	 if(strcmp(start11,"yes")==0 && strlen(dip11)>0 &&strlen(subask11)>0 && strlen(getway11)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip11,subask11,getway11);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip12",dip12);
    	 bcCnfGetValue_s(caFile,"subask12",subask12);
    	 bcCnfGetValue_s(caFile,"getway12",getway12);
    	 bcCnfGetValue_s(caFile,"start12",start12); 
    	 if(strcmp(start12,"yes")==0 && strlen(dip12)>0 &&strlen(subask12)>0 && strlen(getway12)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip12,subask12,getway12);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	  printf("555555555555555555555555555555\n");	
    	 bcCnfGetValue_s(caFile,"dip13",dip13);
    	 bcCnfGetValue_s(caFile,"subask13",subask13);
    	 bcCnfGetValue_s(caFile,"getway13",getway13);
    	 bcCnfGetValue_s(caFile,"start13",start13); 
    	 if(strcmp(start13,"yes")==0 && strlen(dip13)>0 &&strlen(subask13)>0 && strlen(getway13)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip13,subask13,getway13);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip14",dip14);
    	 bcCnfGetValue_s(caFile,"subask14",subask14);
    	 bcCnfGetValue_s(caFile,"getway14",getway14);
    	 bcCnfGetValue_s(caFile,"start14",start14); 
    	 if(strcmp(start14,"yes")==0 && strlen(dip14)>0 &&strlen(subask14)>0 && strlen(getway14)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip14,subask14,getway14);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip15",dip15);
    	 bcCnfGetValue_s(caFile,"subask15",subask15);
    	 bcCnfGetValue_s(caFile,"getway15",getway15);
    	 bcCnfGetValue_s(caFile,"start15",start15); 
    	 if(strcmp(start15,"yes")==0 && strlen(dip15)>0 &&strlen(subask15)>0 && strlen(getway15)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip15,subask15,getway15);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip16",dip16);
    	 bcCnfGetValue_s(caFile,"subask16",subask16);
    	 bcCnfGetValue_s(caFile,"getway16",getway16);
    	 bcCnfGetValue_s(caFile,"start16",start16); 
    	 if(strcmp(start16,"yes")==0 && strlen(dip16)>0 &&strlen(subask16)>0 && strlen(getway16)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip16,subask16,getway16);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip17",dip17);
    	 bcCnfGetValue_s(caFile,"subask17",subask17);
    	 bcCnfGetValue_s(caFile,"getway17",getway17);
    	 bcCnfGetValue_s(caFile,"start17",start17); 
    	 if(strcmp(start17,"yes")==0 && strlen(dip17)>0 &&strlen(subask17)>0 && strlen(getway17)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip17,subask17,getway17);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"dip18",dip18);
    	 bcCnfGetValue_s(caFile,"subask18",subask18);
    	 bcCnfGetValue_s(caFile,"getway18",getway18);
    	 bcCnfGetValue_s(caFile,"start18",start18); 
    	 if(strcmp(start18,"yes")==0 && strlen(dip18)>0 &&strlen(subask18)>0 && strlen(getway18)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip18,subask18,getway18);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 
    	 
    	 bcCnfGetValue_s(caFile,"dip19",dip19);
    	 bcCnfGetValue_s(caFile,"subask19",subask19);
    	 bcCnfGetValue_s(caFile,"getway19",getway19);
    	 bcCnfGetValue_s(caFile,"start19",start19); 
    	 if(strcmp(start19,"yes")==0 && strlen(dip19)>0 &&strlen(subask19)>0 && strlen(getway19)>0){
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip19,subask19,getway19);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
			printf("4444444444444444444444\n");	
    	 bcCnfGetValue_s(caFile,"dip20",dip20);
    	 bcCnfGetValue_s(caFile,"subask20",subask20);
    	 bcCnfGetValue_s(caFile,"getway20",getway20);
    	 bcCnfGetValue_s(caFile,"start20",start20);
    	 memset(caCmd,0,sizeof(caCmd));
    	 if(strcmp(start20,"yes")==0 && strlen(dip20)>0 &&strlen(subask20)>0 && strlen(getway20)>0){   	
    	 		memset(caCmd,0,sizeof(caCmd)); 		
    	 	  sprintf(caCmd,"route add -net %s netmask  %s  gw %s",dip20,subask20,getway20);
    	 	  write(fd,caCmd,sizeof(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }

		}			
		close(fd);
	  
	  

		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >配置成功!</body></html>");
		ltMsgFree(ltMsgPk);
   
    return 0;
    
}


//表单关键字规则和内容关键字规则应用

int Apprules(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char caLabel[256],sqlBuf[128];
		char caFile[64];
		char *type;
		int head;
		int icount;
		
		head=0;
		icount=0;
		int fd;
		fd=0;
    LT_DBROW tempRow;
	  ltDbCursor *tempCursor;
	  
		memset(sqlBuf,0,sizeof(sqlBuf));
		
		if(ltMsgGetVar_s(ltMsgPk,"type")){
	 	 	type=ltMsgGetVar_s(ltMsgPk,"type");
	  }else{
	 	 	type="";
	  }
		
		sprintf(sqlBuf,"select hostkeyword,keyword from naskeyword where type=%s",type);
		printf("%s \n",sqlBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			    sprintf(caFile,"/etc/msa/msa/start/firewallup");
				  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							memset(caLabel,0,sizeof(caLabel));
						  if(strcmp(type,"0")==0){//表单
						  		if(head==0){
										sprintf(caLabel,"echo 'c22%010u|%s' >> /proc/baochuang/firewall",ntohl(inet_addr(tempRow[0])),tempRow[1]);							   							
										head=1;
							    }else{
							    	sprintf(caLabel,"echo '2%010u|%s' >> /proc/baochuang/firewall",ntohl(inet_addr(tempRow[0])),tempRow[1]);
							    }
						  }
						  
						  if(strcmp(type,"1")==0){//web
						  	  if(head==0){
										sprintf(caLabel,"echo 'c33%010u|%s' >> /proc/baochuang/firewall",ntohl(inet_addr(tempRow[0])),tempRow[1]);							   							
										head=1;
							    }else{
							    	sprintf(caLabel,"echo '3%010u|%s' >> /proc/baochuang/firewall",ntohl(inet_addr(tempRow[0])),tempRow[1]);
							    }
						  }
						
							
						 	if(fd){
						 		    printf("caLabel:%s\n",caLabel);
										write(fd,caLabel,sizeof(caLabel));
										write(fd,"\r\n",strlen("\r\n"));
							}	
							
							icount++;
							if(icount>MAX_POSTKEY_NUM){
									break;
							}
							
							tempRow=ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
		}
	  ltDbClose(G_DbCon);
    close(fd);
    
    
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
    ltMsgFree(ltMsgPk);
    return 0;
}






