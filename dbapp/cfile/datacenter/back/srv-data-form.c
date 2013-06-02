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
#define formdir   "/formsearch"
/*
MSA:数据中心
        表单提交内容
功能:当日表单记录查询
*/
int msaformList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
     
	 char sqlStr[1024],tdate[32],mydate[16],tablename[32],where[32];
	 int allcount,k,type;
	 k=0;
	 type=-1;
	 memset(tdate,0,sizeof(tdate));
	 memset(mydate,0,sizeof(mydate));
	 memset(tablename,0,sizeof(tablename));
	 memset(where,0,sizeof(where));
	 
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
	 
	 if(ltMsgGetVar_s(ltMsgPk,"webtype")!=NULL){
	 		type=atoi(ltMsgGetVar_s(ltMsgPk,"webtype"));
	 }
	 if(type==1){
	 		memset(where,0,sizeof(where));
	 }else if(type==2) {
	 		sprintf(where,"%s","where formasize=0");
	 }else if(type==3) {
	 		sprintf(where,"%s","where formsize>=1024000.00");
	 }else if(type==4) {
	 		sprintf(where,"%s","where urlflag=1");
	 }else if(type==5) {
	 		sprintf(where,"%s","where urlflag=2");
	 }else if(type==6) {
	 		sprintf(where,"%s","where urlflag=3");
	 }
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 nasTimeLongFormat(tdate,time(0));
 	 memcpy(mydate,tdate,8);
 	 sprintf(tablename,"msaformlog%s",mydate);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select count(*) from %s",tablename);
	 
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select b.userid,a.dispname,a.username,b.sdate,b.formfile,b.formhost,b.formurl,b.formatt,b.formasize,b.formsize,b.dip from %s b left join msauser a on (a.userid =b.userid) %s",tablename,where);

	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
		   		  char formasize[32];//附件
		   		  char formsize[32];//邮件
		   		  memset(formasize,0,sizeof(formasize));
		   		  memset(formsize,0,sizeof(formsize));
		   		  if(atol(tempRow[8])>1024000){
               sprintf(formasize,"%0.3fM",atol(tempRow[8])/1024000.00);
            }else{
               sprintf(formasize,"%0.3fK",atol(tempRow[8])/1024.00);
            }

		   		  if(atol(tempRow[9])>1024000){
               sprintf(formsize,"%0.3fM",atol(tempRow[9])/1024000.00);
            }else{
               sprintf(formsize,"%0.3fK",atol(tempRow[9])/1024.00);
            }
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"uid\":\"%s\",\"uname\":\"%s\",\"dname\":\"%s\",\"mydate\":\"%s\",\"formname\":\"%s\",\"host\":\"%s\",\"url\":\"%s\",\"matt\":\"%s\",\"mattsize\":\"%s\",\"size\":\"%s\",\"dip\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\",\"dname\":\"%s\",\"mydate\":\"%s\",\"formname\":\"%s\",\"host\":\"%s\",\"url\":\"%s\",\"matt\":\"%s\",\"mattsize\":\"%s\",\"size\":\"%s\",\"dip\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}



/*
MSA:数据中心
				搜索引擎记录
				    关键字列表
功能:关键字列表链接页面调用函数
*/
int ltmsasearchkeyquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltDbHeadPtr dbPtr;
  char *gid;
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msasearchkeyhead.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}

