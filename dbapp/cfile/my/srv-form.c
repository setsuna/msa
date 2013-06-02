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
//规则列表
int msaformList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
//		  char *dbUser;
//		  char *dbPass;
//		  char *dbName;
//		  dbName=_ltPubInfo->_dbname;
//			dbUser=_ltPubInfo->_dbuser;
//			dbPass=_ltPubInfo->_dbpass;
//			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//			if(G_DbCon!=NULL){
//				//printf("db connect ok\n");
//			}else{
//				fprintf(stderr,"db connect error\n");
//			}
	 char sqlStr[1024],tdate[32],mydate[16],tablename[32],where[32];
	 int allcount,k,type;
	 k=0;
	 type=-1;
	 memset(tdate,0,sizeof(tdate));
	 memset(mydate,0,sizeof(mydate));
	 memset(tablename,0,sizeof(tablename));
	 memset(where,0,sizeof(where));
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
	 printf("tablename %s  \n",tablename);


	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select count(*) from %s",tablename);
	 printf("count :   %s \n",sqlStr);
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
	 printf("%s \n",sqlStr);
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
//可选择的日期
int msaDateStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  ltDbCursor *tempCursor;
    LT_DBROW tempRow;
	  stralloc strTemp;
    strTemp.s=0;
    char caTmpp[2048],sqlBuf[1024];
    int lNum=0;
    long lCount=0;
		memset(caTmpp,0,sizeof(caTmpp));
		memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"%s","select * from msaloglist where tabtype=6");
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){
	        lNum++;
	        if(lNum==1){
				      sprintf(caTmpp,"{\"sdate\":\"%s\"}",tempRow[0]);
					    stralloc_cats(&strTemp,caTmpp);
					}else{
						      sprintf(caTmpp,",{\"sdate\":\"%s\"}",tempRow[0]);
							    stralloc_cats(&strTemp,caTmpp);
				  }
			    tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
   }
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
		ltMsgFree(ltMsgPk);
	  return 0;
}
//表单类型
int msaformtypeStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  stralloc strTemp;
    strTemp.s=0;
    char caTmpp[2048];
    int lNum=0,i=0;
    long lCount=0;
		memset(caTmpp,0,sizeof(caTmpp));
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
   for(i=0;i<_MAX_FORMFILTER_NUM;i++){
   		if(strcmp(_ltPubInfo->_FilterListHead._ltFormFilter[i].caRname,"")!=0){
   			  lNum++;
	        if(lNum==1){
				      sprintf(caTmpp,"{\"filterid\":\"%d\",\"formfilter\":\"%s\"}",i,_ltPubInfo->_FilterListHead._ltFormFilter[i].caRname);
					    stralloc_cats(&strTemp,caTmpp);
					}else{
						      sprintf(caTmpp,",{\"filterid\":\"%d\",\"formfilter\":\"%s\"}",i,_ltPubInfo->_FilterListHead._ltFormFilter[i].caRname);
							    stralloc_cats(&strTemp,caTmpp);
				  }
   		}
   }
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
		ltMsgFree(ltMsgPk);
	  return 0;
}
//表单查询列表
int searchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
//		  char *dbUser;
//		  char *dbPass;
//		  char *dbName;
//		  dbName=_ltPubInfo->_dbname;
//			dbUser=_ltPubInfo->_dbuser;
//			dbPass=_ltPubInfo->_dbpass;
//			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//			if(G_DbCon!=NULL){
//				//printf("db connect ok\n");
//			}else{
//				fprintf(stderr,"db connect error\n");
//			}
	 char sqlStr[1024],tablename[128];
	 int allcount,k,j;
	 k=0;
	 j=0;
	 memset(tablename,0,sizeof(tablename));
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 stralloc strWhere;
	 char caTmpp[1024];
	 strTemp.s=0;
	 strWhere.s=0;
	 allcount=0;
 	 sprintf(tablename,"msaformlog%s",ltMsgGetVar_s(ltMsgPk,"mydate"));
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }

	 printf("mydate:%s \n",ltMsgGetVar_s(ltMsgPk,"mydate"));
	 printf("uname:%s \n",ltMsgGetVar_s(ltMsgPk,"uname"));
	 printf("url:%s \n",ltMsgGetVar_s(ltMsgPk,"url"));
	 printf("dip:%s \n",ltMsgGetVar_s(ltMsgPk,"dip"));
	 printf("formsize:%s \n",ltMsgGetVar_s(ltMsgPk,"formsize"));
	 printf("formatt:%s \n",ltMsgGetVar_s(ltMsgPk,"formatt"));

	 if(delflag){
	 					char where[128];
	 					memset(where,0,sizeof(where));
	 					if(strcmp(ltMsgGetVar_s(ltMsgPk,"uname"),"")!=0&&atoi(ltMsgGetVar_s(ltMsgPk,"uname"))!=-1){
	   						 sprintf(where," userid=%s",ltMsgGetVar_s(ltMsgPk,"uname"));
	   						 stralloc_cats(&strWhere,where);
	   			  		 j++;
	   			  }
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"url"),"")!=0){
   						 if(j==0){
	   						 sprintf(where," formurl='%s'",ltMsgGetVar_s(ltMsgPk,"url"));
	   						 stralloc_cats(&strWhere,where);
   						 }else{
	   						 sprintf(where," and formurl='%s'",ltMsgGetVar_s(ltMsgPk,"url"));
	   						 stralloc_cats(&strWhere,where);
   						 }
   						 j++;
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"dip"),"")!=0){
   						 if(j==0){
	   						 sprintf(where," dip='%s'",ltMsgGetVar_s(ltMsgPk,"dip"));
	   						 stralloc_cats(&strWhere,where);
   						 }else{
	   						 sprintf(where," and dip='%s'",ltMsgGetVar_s(ltMsgPk,"dip"));
	   						 stralloc_cats(&strWhere,where);
   						 }
   						 j++;
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"formsize"),"")!=0){
   						 if(j==0){
	   						 sprintf(where," formsize>%s",ltMsgGetVar_s(ltMsgPk,"formsize"));
	   						 stralloc_cats(&strWhere,where);
   						 }else{
	   						 sprintf(where," and formsize>%s",ltMsgGetVar_s(ltMsgPk,"formsize"));
	   						 stralloc_cats(&strWhere,where);
   						 }
   						 j++;
    				}
    				if(strcmp("0",ltMsgGetVar_s(ltMsgPk,"formatt"))!=0){
   						 if(j==0){
	   						 sprintf(where,"%s"," formasize>0");
	   						 stralloc_cats(&strWhere,where);
   						 }else{
	   						 sprintf(where,"%s"," and formasize>0");
	   						 stralloc_cats(&strWhere,where);
   						 }
   						 j++;
    				}
      stralloc_0(&strWhere);
      memset(sqlStr,0,sizeof(sqlStr));
	 		sprintf(sqlStr,"delete from %s where %s",tablename,strWhere.s);
	 		printf("%s \n",sqlStr);
	 		//ltDbExecSql(G_DbCon,sqlStr);
	 }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select b.userid,a.dispname,a.username,b.sdate,b.formfile,b.formhost,b.formurl,b.formatt,b.formasize,b.formsize,b.dip,b.sid from %s b left join msauser a on (a.userid =b.userid)",tablename);
	 printf("%s \n",sqlStr);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"uname"),"")!=0&&atoi(ltMsgGetVar_s(ltMsgPk,"uname"))!=-1){
							 if(atoi(tempRow[0])!=atoi(ltMsgGetVar_s(ltMsgPk,"uname"))){
							 			printf("111  %s  mismatch condition \n",tempRow[1]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("111%s  match condition \n",tempRow[1]);
    				}

						if(strcmp(ltMsgGetVar_s(ltMsgPk,"url"),"")!=0){
							 if(strcmp(tempRow[6],ltMsgGetVar_s(ltMsgPk,"url"))!=0){
							 			printf("666  %s  mismatch condition \n",tempRow[6]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("666  %s  match condition \n",tempRow[6]);
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"dip"),"")!=0){
							 if(strcmp(tempRow[10],ltMsgGetVar_s(ltMsgPk,"dip"))!=0){
							 			printf("10 %s  mismatch condition \n",tempRow[10]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("10 %s  match condition \n",tempRow[10]);
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"formsize"),"")!=0){
							 if(atoi(tempRow[9])<atoi(ltMsgGetVar_s(ltMsgPk,"formsize"))){
							 			printf("9 %s  mismatch condition \n",tempRow[9]);
					       		tempRow=ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("9 %s  match condition \n",tempRow[9]);
    				}
    				if(strcmp("0",ltMsgGetVar_s(ltMsgPk,"formatt"))!=0){
							 if(atoi(tempRow[8])==0){
							 			printf("8 %s  mismatch condition \n",tempRow[8]);
					       		tempRow=ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("8 %s  match condition \n",tempRow[8]);
    				}
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
	     	  	 sprintf(caTmpp,"{\"uid\":\"%s\",\"uname\":\"%s\",\"dname\":\"%s\",\"mydate\":\"%s\",\"formname\":\"%s\",\"host\":\"%s\",\"url\":\"%s\",\"matt\":\"%s\",\"mattsize\":\"%s\",\"size\":\"%s\",\"dip\":\"%s\",\"sid\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10],tempRow[11]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\",\"dname\":\"%s\",\"mydate\":\"%s\",\"formname\":\"%s\",\"host\":\"%s\",\"url\":\"%s\",\"matt\":\"%s\",\"mattsize\":\"%s\",\"size\":\"%s\",\"dip\":\"%s\",\"sid\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10],tempRow[11]);
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
功能：关键字列表节点动态链接页面和动态刷新用户
参数：groupid
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
//全文检索
int contentsearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
//		  char *dbUser;
//		  char *dbPass;
//		  char *dbName;
//		  dbName=_ltPubInfo->_dbname;
//			dbUser=_ltPubInfo->_dbuser;
//			dbPass=_ltPubInfo->_dbpass;
//			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//			if(G_DbCon!=NULL){
//				//printf("db connect ok\n");
//			}else{
//				fprintf(stderr,"db connect error\n");
//			}
	 struct stat st;
   FILE *fp;
	 char sqlStr[1024],tablename[128],caMsgFile[128],caContent[256],cafile[128],caTempDir[128],caFile1[128],caFile2[128],sDate[64],sTime[64];
	 int allcount,k,fdwrite;
   int  iReturn;
	 stralloc strTemp;
   char caTmpp[2048];
	 char utf8Cmd[256];
	 char caCmd[2048],path[128];
   strTemp.s=0;
	 k=0;
	 memset(path,0,sizeof(path));
	 memset(tablename,0,sizeof(tablename));
	 memset(caContent,0,sizeof(caContent));
	 memset(caTempDir,0,sizeof(caTempDir));
	 memset(cafile,0,sizeof(cafile));
	 memset(caFile1,0,sizeof(caFile1));
	 memset(caFile2,0,sizeof(caFile2));
	 memset(sDate,0,sizeof(sDate));
	 memset(caCmd,0,sizeof(caCmd));
	 memset(sTime,0,sizeof(sTime));
	 sprintf(caContent,"%s",ltMsgGetVar_s(ltMsgPk,"content"));
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 allcount=0;
	 nasCvtStime(time(0),sDate,sTime);
   sprintf(path,"%s/%lu",formdir,ltStrGetId());
   sprintf(caTempDir,"%s%s",_datacenterdir,path);
   if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;//生成该报告目录错误
        }
   }

	 sprintf(caFile2,"%s/info.ini",caTempDir);
	 printf("infourl:%s\n",caFile2);
	 fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
	 if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
	 }
	 char *manageruser=NULL;
   if(ltMsgGetVar_s(ltMsgPk,"manageruser")){
		 manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	 }

	  char tempscon[64],searchcon[256];
	  memset(searchcon,0,sizeof(searchcon));
	  memset(tempscon,0,sizeof(tempscon));
	 memset(utf8Cmd,0,sizeof(utf8Cmd));
	  sprintf(tempscon,"%s","|部门名称:");
	  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  sprintf(searchcon,"%s%s",utf8Cmd,ltMsgGetVar_s(ltMsgPk,"gname"));

	  memset(tempscon,0,sizeof(tempscon));
	  memset(utf8Cmd,0,sizeof(utf8Cmd));
	  sprintf(tempscon,"%s","|用户名:");
	  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,ltMsgGetVar_s(ltMsgPk,"uname"));

	  memset(tempscon,0,sizeof(tempscon));
	  memset(utf8Cmd,0,sizeof(utf8Cmd));
	  sprintf(tempscon,"%s","|分类:");
	  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,ltMsgGetVar_s(ltMsgPk,"mtype"));

	  memset(tempscon,0,sizeof(tempscon));
	  memset(utf8Cmd,0,sizeof(utf8Cmd));
	  sprintf(tempscon,"%s","|选择日期:");
	  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,ltMsgGetVar_s(ltMsgPk,"mydate"));

	  memset(tempscon,0,sizeof(tempscon));
	  memset(utf8Cmd,0,sizeof(utf8Cmd));
	  sprintf(tempscon,"%s","|表单大小:");
	  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,ltMsgGetVar_s(ltMsgPk,"formsize"));

	  memset(tempscon,0,sizeof(tempscon));
	  memset(utf8Cmd,0,sizeof(utf8Cmd));
	  if(atoi(ltMsgGetVar_s(ltMsgPk,"formatt"))==0){
			  sprintf(tempscon,"%s","|附件:全部");
			  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  		sprintf(searchcon,"%s%s",searchcon,utf8Cmd);
	  }else {
			  sprintf(tempscon,"%s","|附件:有附件的");
			  strgb2utf8(tempscon,utf8Cmd,strlen(tempscon));
	  		sprintf(searchcon,"%s%s",searchcon,utf8Cmd);
	  }

	 sprintf(caCmd,"path=%s\n",path);
	 write(fdwrite,caCmd,strlen(caCmd));
	 sprintf(caCmd,"skey=%s\n",caContent);
	 write(fdwrite,caCmd,strlen(caCmd));
	 sprintf(caCmd,"sdate=%s_%s\n",sDate,sTime);
	 write(fdwrite,caCmd,strlen(caCmd));
	 sprintf(caCmd,"suser=%s\n",manageruser);
	 write(fdwrite,caCmd,strlen(caCmd));
	 sprintf(caCmd,"scon=%s\n",searchcon);
	 write(fdwrite,caCmd,strlen(caCmd));
	 close(fdwrite);

 	 sprintf(tablename,"msaformlog%s",ltMsgGetVar_s(ltMsgPk,"mydate"));
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select b.userid,a.dispname,a.username,b.sdate,b.formfile,b.formhost,b.formurl,b.formatt,b.formasize,b.formsize,b.dip from %s b left join msauser a on (a.userid =b.userid)",tablename);
	 printf("%s \n",sqlStr);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"uname"),"")!=0&&atoi(ltMsgGetVar_s(ltMsgPk,"uname"))!=-1){
							 if(atoi(tempRow[0])!=atoi(ltMsgGetVar_s(ltMsgPk,"uname"))){
							 			printf("111  %s  mismatch condition \n",tempRow[1]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("111%s  match condition \n",tempRow[1]);
    				}

						if(strcmp(ltMsgGetVar_s(ltMsgPk,"url"),"")!=0){
							 if(strcmp(tempRow[6],ltMsgGetVar_s(ltMsgPk,"url"))!=0){
							 			printf("666  %s  mismatch condition \n",tempRow[6]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("666  %s  match condition \n",tempRow[6]);
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"dip"),"")!=0){
							 if(strcmp(tempRow[10],ltMsgGetVar_s(ltMsgPk,"dip"))!=0){
							 			printf("10 %s  mismatch condition \n",tempRow[10]);
					       		tempRow= ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("10 %s  match condition \n",tempRow[10]);
    				}
						if(strcmp(ltMsgGetVar_s(ltMsgPk,"formsize"),"")!=0){
							 if(atoi(tempRow[9])<atoi(ltMsgGetVar_s(ltMsgPk,"formsize"))){
							 			printf("9 %s  mismatch condition \n",tempRow[9]);
					       		tempRow=ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("9 %s  match condition \n",tempRow[9]);
    				}
    				if(strcmp("0",ltMsgGetVar_s(ltMsgPk,"formatt"))!=0){
							 if(atoi(tempRow[8])==0){
							 			printf("8 %s  mismatch condition \n",tempRow[8]);
					       		tempRow=ltDbFetchRow(tempCursor);
					       		continue;
							 }
							 			printf("8 %s  match condition \n",tempRow[8]);
    				}

            memset(caMsgFile,0,sizeof(caMsgFile));
            sprintf(caMsgFile,"%skey.txt",tempRow[4]);
            printf("path :  %s \n",caMsgFile);
            iReturn=stat(caMsgFile,&st);
            if(iReturn!=0){
            		printf("file read faile \n");
				        tempRow=ltDbFetchRow(tempCursor);
				        continue;
				    }
//				    if(S_ISDIR(st.st_mode)==0){//判断是否为目录
//				    	  tempRow=ltDbFetchRow(tempCursor);
//						    continue;
//				    }
    				fp=fopen(caMsgFile,"r");
 	  	   	  if(ltFileSkipUntilStr(fp,caContent,strlen(caContent))<0){
 	  	   	  		printf("Do not match the keywords \n");
		 	  	   	  fclose(fp);
						    tempRow=ltDbFetchRow(tempCursor);
	 	  	   	 	  continue;
 	  	   	  }
 	  	   	  printf("match the keywords \n");
 	  	   	  fclose(fp);

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
            //写文件
            memset(caTmpp,0,sizeof(caTmpp));
            sprintf(caTmpp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10]);
            stralloc_cats(&strTemp,caTmpp);
       		  tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
	 stralloc_0(&strTemp);
	 int wjson=0;
 	 sprintf(caFile1,"%s/search.csv",caTempDir);
 	 printf("searchurl:%s\n",caFile1);
 	 wjson = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
	 if(wjson == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
	 }
   write(wjson,strTemp.s,strlen(strTemp.s));
   close(wjson);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//表单全文检索列表
int contentsearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char  caFile1[256],path[128],caCmd[256];
	char  *filedir;
  stralloc strTemp;
  int   lNum;
  char  caTmpp[2048];
	int   len;
	struct stat st;
  msasDir *fnames;
  memset(caFile1,0,sizeof(caFile1));
  memset(path,0,sizeof(path));
  memset(caCmd,0,sizeof(caCmd));
  
  filedir=ltMsgGetVar_s(ltMsgPk,"file");//删除参数
  if(filedir){
  	if(strlen(filedir)>1){
			sprintf(caCmd,"rm -Rf %s/%s",_datacenterdir,filedir);
			system(caCmd);
		}
	}
	sprintf(path,"%s%s",_datacenterdir,formdir);
  fnames=mydirlist(path,&len);
//  if(fnames){
// 	 qsort(fnames,len,sizeof(msasDir),dirtimecmp); //按时间排序
//  }
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
  stralloc_cats(&strTemp,caTmpp);
  lNum=1;
  while(len--){
 	      int fileproc=-1;
			  char   keyword[256];
			  char   date[256];
			  char   user[256];
			  char   searchcon[2048];
        memset(keyword,0,sizeof(keyword));
        memset(date,0,sizeof(date));
        memset(user,0,sizeof(user));
        memset(searchcon,0,sizeof(searchcon));

				sprintf(caFile1,"%s/%s/info.ini",path,fnames[len].caDirName);
		    if(stat(caFile1,&st) == 0){
		    	fileproc=1;
		    }else{
		      fileproc=0;
		    }

		    bcCnfGetValue_s(caFile1,"path",path);
		    bcCnfGetValue_s(caFile1,"skey",keyword);
        bcCnfGetValue_s(caFile1,"sdate",date);
        bcCnfGetValue_s(caFile1,"suser",user);
        bcCnfGetValue_s(caFile1,"scon",searchcon);
		    memset(caTmpp,0,sizeof(caTmpp));
			  if(lNum==1){
					sprintf(caTmpp,"{\"path\":\"%s\",\"keyword\":\"%s\",\"date\":\"%s\",\"user\":\"%s\",\"searchcon\":\"%s\",\"fproc\":\"%s\"}",path,keyword,date,user,searchcon,fileproc==1?"Search success ":"Search failure");
					stralloc_cats(&strTemp,caTmpp);
	      }else{
	        sprintf(caTmpp,",{\"path\":\"%s\",\"keyword\":\"%s\",\"date\":\"%s\",\"user\":\"%s\",\"searchcon\":\"%s\",\"fproc\":\"%s\"}",path,keyword,date,user,searchcon,fileproc==1?"Search success ":"Search failure");
	        stralloc_cats(&strTemp,caTmpp);
	      }
		    lNum++;
  }
  stralloc_cats(&strTemp,"]}");
  stralloc_0(&strTemp);
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	if(fnames){
 		free(fnames);
  }
  ltMsgFree(ltMsgPk);
	return 0;
}
//全文检索详情
int searchDetailList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char   caFile1[256];
  long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  char  caTmpp11[1024];
  int   fd;
  int   totalnum;
  int   nownum;
  int   iCount;
  int   inum;
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  int   iReturn;
  char  *caUrl;
  static char inbuf[4096];
  static stralloc line = {0};
  buffer ss;
  int    match;
  int    kkkk;
  int    jjjj;
  int    iiii;

  if(ltMsgGetVar_s(ltMsgPk,"url")){
  	caUrl=ltMsgGetVar_s(ltMsgPk,"url");
  }else{
  	ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");
		ltMsgFree(ltMsgPk);
  	return 0;
  }
  printf("url:%s\n",caUrl);

	sprintf(caFile1,"%s/search.csv",caUrl);
	printf("caFile1:%s\n",caFile1);
	iReturn=access(caFile1,F_OK);
	printf("iReturn1:%d\n",iReturn);
  if(iReturn!=0){
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");
		 ltMsgFree(ltMsgPk);
		 return 0;
  }
  lRowNum=100;
  lSumRec=0;
  lCount=0;
  lStartRec=0;
  lNum=0;
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
	fd=open(caFile1,O_RDONLY | O_NDELAY);
	if(fd<1){
	     ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");
			 ltMsgFree(ltMsgPk);
			 return 0;
	}
	totalnum=0;
	iCount=read(fd,inbuf,8192);
	while(iCount){
	     for(inum=0;inum<iCount;inum++){                /*统计有读取多少行记录*/
	     		if(inbuf[inum]=='\n'){
	     		   totalnum++;
	     		}
	     }
	     iCount=read(fd,inbuf,8192);
	}
	close(fd);
//判断分页参数
	 if(lStartRec>(totalnum-1)){
	 	  lStartRec=totalnum-1;
	 }
	 if(lStartRec<0){
	 	  lStartRec=0;
	 }
	 if((lStartRec+lRowNum)>totalnum){
	 	  lRowNum=totalnum-lStartRec;
	 }
//读文件
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  fd=open(caFile1,O_RDONLY | O_NDELAY);
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");
				 ltMsgFree(ltMsgPk);

				 return 0;
  }
  nownum=0;
  memset(tempStr,0,sizeof(tempStr));
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		nownum++;
		if(nownum>=lStartRec){
			striptrailingwhitespace(&line);
			xIndex++;
			iiii=0;
      iIndex=0;
      kkkk=0;
      if(xIndex==1){
        stralloc_cats(&strTemp,"{");
      }else{
        stralloc_cats(&strTemp,",{");
      }
			for(jjjj=0;jjjj<line.len;jjjj++){
				 if(line.s[jjjj]==','){
					 iIndex++;
           if(iIndex==1){
           	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
			     }else{
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 }
					 jjjj++;
					 iiii=jjjj;
					 while(line.s[jjjj]==','){
					 	iIndex++;
				    sprintf(tempStr,",\"v%d\":\"\"",iIndex);
				    stralloc_cats(&strTemp,tempStr);
				    jjjj++;
					  iiii=jjjj;
					 }
			  }
		  }
		  {
		  	iIndex++;
	      memset(caTmpp11,0,sizeof(caTmpp11));
		    memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
		    sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
		    stralloc_cats(&strTemp,tempStr);
		  }
		  stralloc_cats(&strTemp,"}");
		  if(nownum==(lRowNum+lStartRec)){
     		break;
      }
    }
  }
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}
int msaFormDownLoad(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char *downLoadFile;
    char cafile[512];
    int  fd,iCount;
    char inbuf[8192];
    downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
    if(!downLoadFile){
            downLoadFile="";
            ltMsgPk->msgpktype=2;
                     lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.txt","contenttype","Application/MS-DOWNLOAD");
              write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
              return 0;
    }
    sprintf(cafile,"%s%s/%s",_datacenterdir,downLoadFile,"search.csv");
    printf("%s \n",cafile);
    fd=open(cafile,O_RDONLY | O_NDELAY);
            if(fd<1){
                ltMsgPk->msgpktype=2;
                        lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.txt","contenttype","Application/MS-DOWNLOAD");
                       write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
                       return 0;
            }
            ltMsgPk->msgpktype=2;
            lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"search.csv","contenttype","Application/MS-DOWNLOAD");
            iCount=read(fd,inbuf,8192);
            while(iCount){
                        write(confd,inbuf,iCount);
                 iCount=read(fd,inbuf,8192);
            }
            close(fd);
   ltMsgFree(ltMsgPk);
   return 0;
}
//关键字列表表单提交函数、查询、条件导出、删除
int ltmsasearchkeyhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    unsigned long lTime,L,lTime1;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;

    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//删除条件
    char caWhere[512],xDate[30];

    char *caDtype=NULL;
    char *caHost=NULL;
    char *caSdate=NULL;
    char *caEdate=NULL;
    char *searchkey=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *caDdd=NULL;
    char *caUnit=NULL;
    char *caBef=NULL;
    char *m_action=NULL;
    char *caSdate3=NULL;
    char *caStime=NULL;
    char *caEtime=NULL;
    char *caUser=NULL;

    char *gid=NULL;
    int intGid,glevel=0;
    char caTmpp[2048];

    stralloc strTemp;
    strTemp.s=0;

    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     iAction=0;
    }else{
      iAction=atol(m_action);
    }
    printf("iAction=%d\n",iAction);
    caUser=ltMsgGetVar_s(ltMsgPk,"user");
     printf("caUser=%s\n",caUser);
    caHost=ltMsgGetVar_s(ltMsgPk,"host");
    printf("caHost=%s\n",caHost);
    searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
    printf("searchkey=%s\n",searchkey);
    caDtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    printf("caDtype=%s\n",caDtype);
    caDdd=ltMsgGetVar_s(ltMsgPk,"ddd");
    printf("caDdd=%s\n",caDdd);
    caUnit=ltMsgGetVar_s(ltMsgPk,"unit");  // 天。周。月
    printf("caUnit=%s\n",caUnit);
    caBef=ltMsgGetVar_s(ltMsgPk,"bef"); //以内 以前
    printf("caBef=%s\n",caBef);
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    printf("caSdate=%s\n",caSdate);
    caEdate=ltMsgGetVar_s(ltMsgPk,"edate");
    printf("caEdate=%s\n",caEdate);
    caSdate3=ltMsgGetVar_s(ltMsgPk,"sdate3");   //day
    printf("Sdate3=%s\n",caSdate3);
    caStime=ltMsgGetVar_s(ltMsgPk,"stime");   //hour
    printf("caStime=%s\n",caStime);
    caEtime=ltMsgGetVar_s(ltMsgPk,"etime");
    printf("caEtime=%s\n",caEtime);
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
    printf("caOrder=%s\n",caOrder);
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
    printf("caOrderby=%s\n",caOrderby);
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    printf("gid=%s\n",gid);
   intGid=atol(gid);
	 printf("intGid::%d\n",intGid);

/*获取部门管理级别*/
sprintf(strBuf,"select level from msagroup where id=%d",intGid);
printf("strBuf::%s\n",strBuf);
tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			printf("tempRow[0]::%s\n",tempRow[0]);
			glevel=atol(tempRow[0]);
		}
	}
ltDbCloseCursor(tempCursor);
printf("glevel::%d\n",glevel);

lRowNum=100; /*每页的行数*/
lSumRec=0;/*总行数*/
lCount=0; //
lStartRec=0;//
lNum=0; /*该页剩余的行数*/

char Shour[32];//
char Ehour[32];//
unsigned int sTime3,eTime3,sTime,eTime;	//存储转换为整型的日期
sprintf(Shour,"%s:00:00",caStime);//开始小时
sprintf(Ehour,"%s:59:59",caEtime);//结束小时
sTime3= msaCvtLongTime(caSdate3,Shour);//将开始时间转换为整型
eTime3= msaCvtLongTime(caSdate3,Ehour);//将结束时间转换为整型
printf("sTime3=%u\n",sTime3);
printf("eTime3=%u\n",eTime3);
sTime=msaCvtLongTime(caSdate,"00:00:00");//将开始时间转换为整型
eTime=msaCvtLongTime(caEdate,"23:59:59");//将结束时间转换为整型
printf("sTime=%u\n",sTime);
printf("eTime=%u\n",eTime);


if(ltMsgGetVar_s(ltMsgPk,"limit")){
	lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	}//相当于结束位置
if(ltMsgGetVar_s(ltMsgPk,"start")){
	lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	}//相当于起始位置

memset(caWhere,0,sizeof(caWhere));
memset(delWhere,0,sizeof(delWhere));

if(strcmp(gid,"-1")==0){
	if(strcmp(caUser,"-1")==0){
	       sprintf(caWhere,"%s","2=2");//查找整个部门用户
	       sprintf(delWhere,"%s","2=2");//查找整个部门用户
		}
	}
 printf("caWhere1111=%s\n",caWhere);
if(strcmp(gid,"-1")){//不等于-1
	   if(strcmp(caUser,"-1")==0){
	       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
	       sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
		}else{
			    sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  为b表
				sprintf(delWhere,"userid = %s",caUser);
			}
	}

printf("caWhere1111:%s\n",caWhere);
printf("delWhere111:%s\n",delWhere);

    if(caHost!=NULL){//HOST关键字判断
	     if(strlen(caHost)>0) {//
	         sprintf(caWhere,"%s and (b.host like '%c%s%c') ",caWhere,'%',caHost,'%');
	         sprintf(delWhere,"%s and (host like '%c%s%c') ",delWhere,'%',caHost,'%');
	     }
    }
printf("222:%s\n",caWhere);
printf("delWhere222:%s\n",delWhere);
    if(searchkey!=NULL){//搜索关键字
	     if(strlen(searchkey)>0) {
	         sprintf(caWhere,"%s and (b.searchkey like '%c%s%c') ",caWhere,'%',searchkey,'%');
	         sprintf(delWhere,"%s and (searchkey like '%c%s%c') ",delWhere,'%',searchkey,'%');
	     }
    }
printf("333:%s\n",caWhere);
printf("delWhere333:%s\n",delWhere);
    if(strcmp(caDtype,"unit") == 0) { /* 按天数计1天、周、月：以内、以前*/
	        lTime = time(0);//获取当前时间
	        if(caDdd!=NULL){
	            if(strlen(caDdd)>0) { /*计算几天以内*/
			             L= atol(caDdd);
			             if(caUnit[0] == '1') { /*天*/
			                 lTime1 = lTime - L* 86400L;
			             }else if(caUnit[0] == '2'){ /*周*/
			                 lTime1 = lTime - L* 7 * 86400L;
			             }else {//月
			                 lTime1 = lTime - L* 30 * 86400L;
			             }
//printf("lTime00000000:%llu\n",lTime1);
char     nowTime[32];
memset(nowTime,0,sizeof(nowTime));
nasTimeFormat(nowTime,lTime1);
printf("lTime11111111:%s\n",nowTime);

char myday[10];
char dateDesc[6];
myday[0]='2';
myday[1]='0';
myday[2]=nowTime[0];
myday[3]=nowTime[1];
myday[4]=nowTime[3];
myday[5]=nowTime[4];
myday[6]=nowTime[6];
myday[7]=nowTime[7];
myday[8]='\0';
sprintf(dateDesc,"%c%c%c%c%c%c%c%c",myday[0],myday[1],myday[2],myday[3],myday[4],myday[5],myday[6],myday[7]);
printf("dateDesc11111111:%s\n",dateDesc);

			             if(caBef[0] == '1') { /* 以内 */
			                 sprintf(caWhere,"%s and  (b.ltime >= %s)",caWhere,dateDesc);/* 以内 */
			                 sprintf(delWhere,"%s and (ltime >= %s)",delWhere,dateDesc);/* 以内 */
			             }else { /* 以前  */
			                 sprintf(caWhere,"%s and (b.ltime <= %s)",caWhere,dateDesc); /* 以前  */
			                 sprintf(delWhere,"%s and (ltime<= %s)",delWhere,dateDesc); /* 以前  */
			             }
	          }
	      }
    }else if(strcmp(caDtype,"unit3") == 0){ /* 时间范围在2010-12-08从00-23*/
		        if((caSdate3!=NULL) ){
			             if(strlen(caSdate3)==8){
					              if( strlen(caEtime)==2 && strlen(caStime)==2 ){
					               sprintf(caWhere,"%s and (b.ltime> %u)",caWhere,sTime3);
					               sprintf(caWhere,"%s and (b.ltime< %u)",caWhere,eTime3);
					               sprintf(delWhere,"%s and (ltime > %u)",delWhere,sTime3);
					               sprintf(delWhere,"%s and (ltime < %u)",delWhere,eTime3);//时间：00-23
					              }else{
						              sprintf(caWhere,"%s and (b.ltime = %u)",caWhere,sTime3);
						              sprintf(delWhere,"%s and (ltime = %u)",delWhere,sTime3);
					              }
			             }
		         }
    }else if(strcmp(caDtype,"unit2") == 0){ /* 时间范围从2010-12-10到2010-12-20*/
						     if( (caSdate!=NULL) && (caEdate!=NULL)){
						         if(strlen(caSdate)>0) {
						             if(strlen(caSdate)==8) {
						              sprintf(caWhere,"%s and (b.ltime >= %u)",caWhere,sTime);
						              sprintf(delWhere,"%s and ltime >= %u",delWhere,sTime);
						              }
						         }
						         if(strlen(caEdate)>0) {
						              if(strlen(caEdate)==8) {
						              sprintf(caWhere,"%s and b.ltime <= %u",caWhere,eTime);
						              sprintf(delWhere,"%s and ltime <= %u",delWhere,eTime);
						              }
						         }
						   }
    }
printf("caWhere444:%s\n",caWhere);
printf("delWhere444:%s\n",delWhere);

    if(iAction==2){ /*del条件删除操作*/
    	 printf("delWhere:%s\n",delWhere);
         if( strlen(delWhere)>0){
		        sprintf(sqlBuf,"delete from msasearchkey where %s ",delWhere);
		        printf("sqlbuf:%s\n",sqlBuf);
		        ltDbExecSql(G_DbCon,sqlBuf);
          }
            printf("wwwwwwwwwwww\n");
		    ltMsgPk->msgpktype=1;
		     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n", "lthtml",LT_TYPE_STRING,"0");
		    ltMsgFree(ltMsgPk);
		    printf("gggggggggggggggggg\n");
		    return 0;
    }else if(iAction==1){/*export导出*/
			       char caBack[128];
			       int  lTime;
			       char sDate[36];
			       char caLine[8012];
			       memset(sDate,0,sizeof(sDate));
			       lTime = time(0);//获取当前时间
			       nasTimeFormat(sDate,lTime);
			       sDate[8]='\0';
			       sprintf(caBack,"%s-searchexport.csv",sDate);//文件路径
				   ltMsgPk->msgpktype=2;
				   lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caBack,"contenttype","Application/MS-DOWNLOAD");

		 if(caOrder!=NULL){
			        if( strlen(caOrder)>0 ){
				         if(strcmp(caOrder,"nosort")!=0) {
						            if(strcmp(caOrder,"date")==0) {
						                sprintf(caWhere,"%s order by b.ltime ",caWhere);
						            }else if(strcmp(caOrder,"userid") == 0) {
						                sprintf(caWhere,"%s order by b.userid ",caWhere);
						             }

						            if(strlen(caOrderby)) {
						                if(caOrderby[0] == '1') {
						                   if(strcmp(caOrder,"date")==0) {
						                       ltStrAddF(caWhere," desc");
						                    }else{
						                       ltStrAddF(caWhere," desc ");
						                   }
						                }else{
						                    if(strcmp(caOrder,"date")==0) {
						                        ltStrAddF(caWhere," asc");
						                    }else{
						                        ltStrAddF(caWhere," asc ");
						                    }
						                }
						            }
				         }
			       }
		       }

		   printf("caWhere:%s\n",caWhere);
		sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey from msasearchkey b left join msauser a on a.userid=b.userid where %s limit %lu offset %lu",caWhere,lRowNum,lStartRec);
	     printf("sqlbuf:%s\n",sqlBuf);
	     tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	     if(tempCursor!=NULL){
	     tempRow=ltDbFetchRow(tempCursor);
		   while(tempRow!=NULL){
		   	printf("555555555\n");
		   	    memset(xDate,0,sizeof(xDate));
				    nasTimeFormat(xDate,atol(tempRow[2]));
						sprintf(caLine,"%s,%s,%s,%s\n",tempRow[0],tempRow[1],xDate,tempRow[3]);
						printf("caLine=%s\n",caLine);
						write(confd,caLine,strlen(caLine));
						tempRow= ltDbFetchRow(tempCursor);
		   }
	     ltDbCloseCursor(tempCursor);
	   }
	    return 0;
   }


	  sprintf(strBuf,"select count(*) from msasearchkey b left join msauser a on a.userid=b.userid where %s",caWhere);
printf("strBuf:%s\n",strBuf);
		 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		 if(tempCursor!=NULL){
			  tempRow= ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   lCount=atol(tempRow[0]);
			  }
		 }
		 ltDbCloseCursor(tempCursor);
          printf("lCount=%ld\n",lCount);

printf("5555:%s\n",caWhere);
	if(caOrder!=NULL){
				     if( strlen(caOrder)>0 ){
					      if(strcmp(caOrder,"nosort")!=0) {
							         if(strcmp(caOrder,"date")==0) {
							                sprintf(caWhere,"%s order by b.ltime ",caWhere);
							         }else if(strcmp(caOrder,"userid") == 0){
							                sprintf(caWhere,"%s order by b.userid ",caWhere);
							         }

							         if(strlen(caOrderby)){
								                if(caOrderby[0] == '1') {
								                   if(strcmp(caOrder,"date")==0) {
								                      ltStrAddF(caWhere,"desc");
								                    }else{
								                    	 ltStrAddF(caWhere,"desc");
								                    }
								                }else{
								                     if(strcmp(caOrder,"date")==0) {
								                        ltStrAddF(caWhere,"asc");
								                     }else{
								                	    ltStrAddF(caWhere,"asc");
								                	}
								                }
							         }

					     }
				    }
		    }

printf("6666:%s\n",caWhere);
//json生成
	sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
    sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey from msasearchkey b left join msauser a on (a.userid =b.userid) where %s limit %lu offset %lu",caWhere,lRowNum,lStartRec);//limit:返回lRowNum行  offset 说明在开始返回行之前忽略多少行
    printf("sqlBuf=%s\n",sqlBuf);

		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		   tempRow= ltDbFetchRow(tempCursor);
		   	while(tempRow!=NULL){
		   		printf("5555555555\n");
		   		lNum++;
						if(lNum==1){
							      memset(xDate,0,sizeof(xDate));
							      nasTimeFormat(xDate,atol(tempRow[2]));
								  sprintf(caTmpp,"{\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\"}",tempRow[0],tempRow[1],xDate,tempRow[3]);
								  printf("caTmpp=%s\n",caTmpp);
								  stralloc_cats(&strTemp,caTmpp);
						}else{
							       memset(xDate,0,sizeof(xDate));
							       nasTimeFormat(xDate,atol(tempRow[2]));
			                       printf("xDate:%s\n",xDate);//10-12-16 18:14:41
							     sprintf(caTmpp,",{\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\"}",tempRow[0],tempRow[1],xDate,tempRow[3]);
								 printf("caTmpp=%s\n",caTmpp);
								  stralloc_cats(&strTemp,caTmpp);
						 }
						tempRow= ltDbFetchRow(tempCursor);
			}
		 ltDbCloseCursor(tempCursor);
		}
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);

		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
		ltMsgFree(ltMsgPk);

		return 0;



}
