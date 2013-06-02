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
关键字内容过滤
		表单递交内容关键字
		网页内容关键字拦截
		    应用内容关键字调用函数
*/
#define MAX_POSTKEY_NUM    256 //匹配最多关键字个数

int Apprules(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
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
		
		
	  if(strcmp(type,"2")==0){
	  	  int flag=-1;
	  	  
	  		_ltPubInfo->lRunning=0;
				flag=nasInitVidList(_ltPubInfo);
				_ltPubInfo->lRunning=1;

				if(flag==0){
					  ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
				    ltMsgFree(ltMsgPk);
				    return 0;
				}else{
					  ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
				    ltMsgFree(ltMsgPk);
				    return 0;
				}
	  }
		
		sprintf(sqlBuf,"select hostkeyword,keyword from naskeyword where type=%s",type);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			    sprintf(caFile,"/etc/msa/msa/start/firewall.up");
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
										write(fd,caLabel,sizeof(caLabel));
										write(fd,"\r\n",strlen("\r\n"));
										system(caLabel);
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

/*
MSA:策略中心
关键字内容过滤
		表单递交内容关键字
		网页内容关键字拦截
		    树节点连接 记录添加 记录删除 调用函数
*/
int editkeyword(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
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
		
	 char sqlBuf[128],page[128],code[16];
	 char *addflag,*delflag,*type;
	 
	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 
	 addflag=NULL;
	 delflag=NULL;
	 type=NULL;
 
	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 memset(code,0,sizeof(code));
	 
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 	 type=ltMsgGetVar_s(ltMsgPk,"type");
	 }
	 if(addflag){
	 		int lCount;
	 		lCount=0;
	 		char *host=ltMsgGetVar_s(ltMsgPk,"host");
	 		char *keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
	 		char *utf8=ltMsgGetVar_s(ltMsgPk,"utf8");
	 		char *web=ltMsgGetVar_s(ltMsgPk,"web");

	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"select max(kid)+1 from naskeyword");
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
      if(tempCursor!=NULL){
         tempRow= ltDbFetchRow(tempCursor);
         if(tempRow!=NULL){
                  lCount=atoi(tempRow[0]);
         }
         ltDbCloseCursor(tempCursor);
			}
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into naskeyword values(%d,'%s','%s',%s,%s,%s)",lCount,host,keyword,utf8,web,type);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *id=ltMsgGetVar_s(ltMsgPk,"id");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from naskeyword where kid=%s",id);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }

	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from naskeyword where type=%s",type);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				memset(code,0,sizeof(code));
				if(atoi(tempRow[3])==1){
						sprintf(code,"%s-","utf8");
				}
				if(atoi(tempRow[4])==1){
						sprintf(code,"%s-web",code);
				}
				lt_dbput_recordvars(tablePtr,4,
					"id",LT_TYPE_STRING,tempRow[0],
					"host",LT_TYPE_STRING,tempRow[1],
					"keyword",LT_TYPE_STRING,tempRow[2],
					"code",LT_TYPE_STRING,code);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
	 if(atoi(type)==0){
	 		sprintf(page,"%s","/app/msa/msa/htmlplt/rule/formkeyword.htm");
	 }else if(atoi(type)==1){
	 		sprintf(page,"%s","/app/msa/msa/htmlplt/rule/webkeyword.htm");
	 }
   lt_dbput_rootvars(dbPtr,1,"type",type);
   
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,page,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
   
}

