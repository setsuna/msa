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
int msaloopgroupToPagetree(stralloc *strTemp,int level,int pid,char gid[]);
static char inbuf[512];
static stralloc line = {0};
//文件上传
int userimport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char caTempDir[128];
  char caFile1[128];
  char fnames[512];
  char caCmd[256];
  char file[256];
  char seled[16];
	char alias[2048];
	char message[128];
  DIR *dir;
  struct dirent *d;
  unsigned int ftime;
  struct stat st;
  ftime=0;
	 /*得到WEB传上来的变量*/
	memset(file,0,sizeof(file));
	memset(caCmd,0,sizeof(caCmd));
	memset(caFile1,0,sizeof(caFile1));
	memset(fnames,0,sizeof(fnames));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(message,0,sizeof(message));
  sprintf(caTempDir,"%s","/app/ns/upload");
  sprintf(seled,"%s",ltMsgGetVar_s(ltMsgPk,"seled"));
  if(ltFileIsExist(caTempDir)!=1){
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n",LT_TYPE_STRING,"{success:false,message:'Path error!'}");
			ltMsgFree(ltMsgPk);
			return 0;
  }
  if ( dir = opendir(caTempDir)) {
        while(d = readdir(dir)) {
             if (d->d_name[0] == '.') continue;
	           sprintf(caFile1, "%s/%s", caTempDir, d->d_name);
             if(stat(caFile1,&st) == 0 && S_ISREG(st.st_mode)){
             		if(st.st_mtime>ftime){
									 memset(fnames,0,sizeof(fnames));
	           			 sprintf(fnames,"%s",d->d_name);
             		}
    						ftime=st.st_mtime;
						 };
        }
  }
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
	printf("MYFNAME %s \n",fnames);
	//读取文件
	sprintf(file,"%s/%s",caTempDir,fnames);
	printf("file  %s \n",file);
	buffer ss;
	int fd,xlen,match,k,i,iii,jjj,freeid,count;
	i=0;
	iii=0;
	jjj=0;
	freeid=0;
	count=0;
	char username[32];
  char dispname[32];
  char email[32];
	char ip[32];
	char mac[32];
	int groupid[_MAX_GROUPLEVEL];
	char tempstr[32];
	char sqlstr[1024],policyid[129];
	fd = open_read(file);
  if (fd == -1)
  {
      printf("open user error\n");
      return 0;
  }
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;)
  {
	   if (getln(&ss,&line,&match,'\n') == -1) break;
	   if (!match && !line.len) { close(fd); break; }
	   striptrailingwhitespace(&line);
	   if (!stralloc_0(&line)) break;
	   if(line.len<5){
       continue;
	   }
		 memset(username,0,sizeof(username));
		 memset(dispname,0,sizeof(dispname));
		 memset(email,0,sizeof(email));
		 memset(ip,0,sizeof(ip));
		 memset(mac,0,sizeof(mac));
		 for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
		    	groupid[iii]=0;
		 }
		 memset(policyid,0,sizeof(policyid));
		 sprintf(policyid,"%s","00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
 		 if(seled[0]=='1'){
				k=0;
				for(i=0;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						username[k]=alias[i];
						k++;
					}
				}
		 }
 		 if(seled[1]=='1'){
				k=0;
				for(i=i+1;i<xlen;i++){
					 if( alias[i]==',' ){
							break;
					 }
					 else{
							dispname[k]=alias[i];
							k++;
					 }
				}
		 }
		 if(seled[2]=='1'){
				 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								email[k]=alias[i];
								k++;
							}
				 }
		 }
		 if(seled[3]=='1'){
				 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								ip[k]=alias[i];
								k++;
							}
				 }
		 }
		 if(seled[4]=='1'){
				 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								mac[k]=alias[i];
								k++;
							}
				 }
		 }
		 //部门处理
		 if(seled[5]=='1'){
		 		 //level  1
		 		 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")!=0){
				 			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
					 				if(strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)==0){
					 					groupid[0]=_ltPubInfo->_GroupList[iii].groupid;
						 				break;
					 				}
					 				if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)!=0){
						 					//find free groupid
						 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
						 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
		        								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",tempstr);
		        								_ltPubInfo->_GroupList[jjj].groupid=jjj;
		        								_ltPubInfo->_GroupList[jjj].pid=0;
		        								_ltPubInfo->_GroupList[jjj].level=0;
				 										memset(sqlstr,0,sizeof(sqlstr));
				 										sprintf(sqlstr,"insert into msagroup (id,name,pid) values (%d,'%s',%d)",jjj,tempstr,0);
				 										ltDbExecSql(G_DbCon,sqlstr);
				 										groupid[0]=jjj;
						 						 		break;
						 						 }
						 					}
					 				}
				 			}
				 }
				 //level 2
				 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")!=0){
				 			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
					 				if(strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)==0){
					 					groupid[1]=_ltPubInfo->_GroupList[iii].groupid;
						 				break;
					 				}
					 				if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)!=0){
						 					//find free groupid
						 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
						 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
		        								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",tempstr);
		        								_ltPubInfo->_GroupList[jjj].groupid=jjj;
		        								_ltPubInfo->_GroupList[jjj].pid=groupid[0];
		        								_ltPubInfo->_GroupList[jjj].level=1;
				 										memset(sqlstr,0,sizeof(sqlstr));
				 										sprintf(sqlstr,"insert into msagroup (id,name,level,pid) values (%d,%s',%d,%d)",jjj,tempstr,1,groupid[0]);
				 										ltDbExecSql(G_DbCon,sqlstr);
				 										groupid[1]=jjj;
						 						 		break;
						 						 }
						 					}
					 				}
				 			}
				 }
				 //level 3
				 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")!=0){
				 			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
				 				if(strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)==0){
				 					groupid[2]=_ltPubInfo->_GroupList[iii].groupid;
				 					break;
				 				}
				 				if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)!=0){
					 					//find free groupid
					 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
					 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
	        								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",tempstr);
	        								_ltPubInfo->_GroupList[jjj].groupid=jjj;
	        								_ltPubInfo->_GroupList[jjj].pid=groupid[1];
	        								_ltPubInfo->_GroupList[jjj].level=2;
			 										memset(sqlstr,0,sizeof(sqlstr));
			 										sprintf(sqlstr,"insert into msagroup (id,name,level,pid) values (%d,'%s',%d,%d)",jjj,tempstr,2,groupid[1]);
			 										ltDbExecSql(G_DbCon,sqlstr);
			 										groupid[2]=jjj;
					 						 		break;
					 						 }
					 					}
				 				}
				 			}
				 }
				 //level 4
				 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")!=0){
				 			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
					 				if(strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)==0){
					 					groupid[3]=_ltPubInfo->_GroupList[iii].groupid;
					 					break;
					 				}
					 				if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)!=0){
						 					//find free groupid
						 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
						 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
		        								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",tempstr);
		        								_ltPubInfo->_GroupList[jjj].groupid=jjj;
		        								_ltPubInfo->_GroupList[jjj].pid=groupid[2];
		        								_ltPubInfo->_GroupList[jjj].level=3;
				 										memset(sqlstr,0,sizeof(sqlstr));
				 										sprintf(sqlstr,"insert into msagroup (id,name,level,pid) values (%d,'%s',%d,%d)",jjj,tempstr,3,groupid[2]);
				 										ltDbExecSql(G_DbCon,sqlstr);
				 										groupid[3]=jjj;
						 						 		break;
						 						 }
						 					}
					 				}
				 			}
				 }
				 //level 5
				 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")!=0){
				 			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
					 				if(strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)==0){
					 					groupid[4]=_ltPubInfo->_GroupList[iii].groupid;
					 					break;
					 				}
					 				if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,tempstr)!=0){
					 						//find free groupid
						 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
						 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
		        								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",tempstr);
		        								_ltPubInfo->_GroupList[jjj].groupid=jjj;
		        								_ltPubInfo->_GroupList[jjj].pid=groupid[3];
		        								_ltPubInfo->_GroupList[jjj].level=4;
				 										memset(sqlstr,0,sizeof(sqlstr));
				 										sprintf(sqlstr,"insert into msagroup (id,name,level,pid) values (%d,'%s',%d,%d)",jjj,tempstr,4,groupid[3]);
				 										ltDbExecSql(G_DbCon,sqlstr);
				 										groupid[4]=jjj;
						 						 		break;
						 						 }
						 					}
					 				}
				 			}
				 }
		 }
		 //策略处理
		 for(iii=0;iii<_MAX_ALLPOLICY;iii++){
		 		 memset(tempstr,0,sizeof(tempstr));
		 		 k=0;
				 for(i=i+1;i<xlen;i++){
							if( alias[i]==',' ){
								break;
							}
							else{
								tempstr[k]=alias[i];
								k++;
							}
				 }
				 if(strcmp(tempstr,"")==0){
				 			break;
				 }else{
				 			for(jjj=0;jjj<_MAX_ALLPOLICY;jjj++){
				 					//if pname 存在
					 				if(strcmp(_ltPubInfo->_PolicyList[jjj].pname,tempstr)==0){
						 					policyid[_ltPubInfo->_PolicyList[jjj].pid]='1';
						 					break;
					 				}
					 				//if pname不存在
					 				if(jjj==_MAX_ALLPOLICY-1 && strcmp(_ltPubInfo->_PolicyList[jjj].pname,tempstr)!=0){
					 						//新建策略
							 				for(k=0;k<_MAX_ALLPOLICY;k++){
							 						 if(strcmp(_ltPubInfo->_PolicyList[k].pname,"")==0){
			        								snprintf(_ltPubInfo->_PolicyList[k].pname,48,"%s",tempstr);
			        								_ltPubInfo->_PolicyList[k].pid=k;
					 										memset(sqlstr,0,sizeof(sqlstr));
					 										sprintf(sqlstr,"insert into msapolicy (pid,pname) values (%d,'%s')",k,tempstr);
					 										ltDbExecSql(G_DbCon,sqlstr);
					 										policyid[k]=49;
							 						 		break;
							 						 }
						 					}
					 				}
				 			}
				 }
		 }
		 //添加
	 		nasDbUserInfo item;
		  nasDbUserInfo *itemnew;
			memset(&item,0,sizeof(nasDbUserInfo));
	    snprintf(item.username,32,"%s",username);
	    snprintf(item.dispname,32,"%s",dispname);
	    item.groupid[0]=groupid[0];
	    item.groupid[1]=groupid[1];
	    item.groupid[2]=groupid[2];
	    item.groupid[3]=groupid[3];
	    item.groupid[4]=groupid[4];
	    snprintf(item.bindemail,64,"%s",email);
    	item.bindip=ntohl(inet_addr(ip));
	    if(strlen(mac)==17 || strlen(mac)==12){
	       nasCvtMacI(mac,item.bindmac);
	    }
	    memcpy(item.policyid,policyid,128);
     	itemnew=msaNewInsertDbUser(_ltPubInfo,&item);
  		count++;
  }
  sprintf(message,"{success:true,message:'import %d users successful!'}",count);
  ltDbClose(G_DbCon);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,message);
	ltMsgFree(ltMsgPk);
	return 0;
}



//部门导入
int msaaddgroup(char *groupname,int level,int pid){
	char sqlstr[1024];
	int groupid,iii,jjj;
	iii=0;
	jjj=0;
	groupid=0;
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
	for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
			if(strcmp(_ltPubInfo->_GroupList[iii].name,groupname)==0){
				memset(sqlstr,0,sizeof(sqlstr));
				sprintf(sqlstr,"update msagroup set pid=%d where name='%s'",pid,groupname);
				ltDbExecSql(G_DbCon,sqlstr);
				groupid=_ltPubInfo->_GroupList[iii].groupid;
				_ltPubInfo->_GroupList[iii].pid=pid;
				_ltPubInfo->_GroupList[iii].level=level;
				break;
			}
			if(iii==_MAX_ALLGRPNUM-1 && strcmp(_ltPubInfo->_GroupList[iii].name,groupname)!=0){
					//find free groupid
 					for(jjj=0;jjj<_MAX_ALLGRPNUM;jjj++){
 						 if(strcmp(_ltPubInfo->_GroupList[jjj].name,"")==0){
								snprintf(_ltPubInfo->_GroupList[jjj].name,64,"%s",groupname);
								_ltPubInfo->_GroupList[jjj].groupid=jjj;
								_ltPubInfo->_GroupList[jjj].pid=pid;
								_ltPubInfo->_GroupList[jjj].level=level;
								memset(sqlstr,0,sizeof(sqlstr));
								sprintf(sqlstr,"insert into msagroup (id,name,level,pid) values (%d,'%s',%d,%d)",jjj,groupname,level,pid);
								ltDbExecSql(G_DbCon,sqlstr);
								groupid=jjj;
 						 		break;
 						 }
 					}
			}
	}
  ltDbClose(G_DbCon);
  return groupid;
}


int groupimport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char caTempDir[128];
  char caFile1[128];
  char fnames[512];
  char caCmd[256];
  char file[256];
	char alias[2048];
	char message[128];
  DIR *dir;
  struct dirent *d;
  unsigned int ftime;
  struct stat st;
  ftime=0;
	 /*得到WEB传上来的变量*/
	memset(file,0,sizeof(file));
	memset(caCmd,0,sizeof(caCmd));
	memset(caFile1,0,sizeof(caFile1));
	memset(fnames,0,sizeof(fnames));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(message,0,sizeof(message));
  sprintf(caTempDir,"%s","/app/ns/upload");
  if(ltFileIsExist(caTempDir)!=1){
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n",LT_TYPE_STRING,"{success:false,message:'Path error!'}");
			ltMsgFree(ltMsgPk);
			return 0;
  }
  if ( dir = opendir(caTempDir)) {
        while(d = readdir(dir)) {
             if (d->d_name[0] == '.') continue;
	           sprintf(caFile1, "%s/%s", caTempDir, d->d_name);
             if(stat(caFile1,&st) == 0 && S_ISREG(st.st_mode)){
             		if(st.st_mtime>ftime){
									 memset(fnames,0,sizeof(fnames));
	           			 sprintf(fnames,"%s",d->d_name);
             		}
    						ftime=st.st_mtime;
						 };
        }
  }
	printf("MYFNAME %s \n",fnames);
	//读取文件
	sprintf(file,"%s/%s",caTempDir,fnames);
	printf("file  %s \n",file);
	buffer ss;
	int fd,xlen,match,k,i,iii,freeid,lineflag,count;
	int temppid;
	i=0;
	iii=0;
	freeid=0;
	lineflag=0;
	temppid=0;
	count=0;
	char tempstr[32];
	fd = open_read(file);
  if (fd == -1)
  {
      printf("open user error\n");
      return 0;
  }
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;)
  {	 count++;
	   if (getln(&ss,&line,&match,'\n') == -1) break;
	   if (!match && !line.len) { close(fd); break; }
	   striptrailingwhitespace(&line);
	   if (!stralloc_0(&line)) break;
	   if(line.len<5){
       continue;
	   }
	   //level 0
	   memset(tempstr,0,sizeof(tempstr));
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
				k=0;
				for(i=0;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						tempstr[k]=alias[i];
						k++;
					}
				}
			if(strcmp(tempstr,"")==0){
					if(lineflag==0){//第一行第一列为空  error
								lineflag=1;
		 						ltMsgPk->msgpktype=1;
								lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"{success:true,message:'File format error !'}");
								ltMsgFree(ltMsgPk);
								return 0;
					}
			}else {
					lineflag=1;
					temppid=0;
					temppid=msaaddgroup(tempstr,0,temppid);
					continue;
			}
	  	//level 1
	   memset(tempstr,0,sizeof(tempstr));
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
				k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						tempstr[k]=alias[i];
						k++;
					}
				}
			if(strcmp(tempstr,"")!=0){
					temppid=msaaddgroup(tempstr,1,temppid);
					continue;
			}
			//level 2
	   memset(tempstr,0,sizeof(tempstr));
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
				k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						tempstr[k]=alias[i];
						k++;
					}
				}
			if(strcmp(tempstr,"")!=0){
					temppid=msaaddgroup(tempstr,2,temppid);
					continue;
			}
			//level 3
	   memset(tempstr,0,sizeof(tempstr));
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
				k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						tempstr[k]=alias[i];
						k++;
					}
				}
			if(strcmp(tempstr,"")!=0){
					temppid=msaaddgroup(tempstr,3,temppid);
					continue;
			}
			//level 4
	   memset(tempstr,0,sizeof(tempstr));
 		 sprintf(alias,"%s",line.s);
 		 xlen=strlen(alias);
				k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						tempstr[k]=alias[i];
						k++;
					}
				}
			if(strcmp(tempstr,"")!=0){
					temppid=msaaddgroup(tempstr,4,temppid);
					continue;
			}
  }
  sprintf(message,"%s","{success:true,message:'import group successful!'}");
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,message);
	ltMsgFree(ltMsgPk);
	return 0;
}


//策略组列表
int msaListUserPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *sid=ltMsgGetVar_s(ltMsgPk,"sid");
	 		sprintf(sqlStr,"delete from msauserpolicy where sid='%s'",sid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select sid,policyname,ratelimit,ratetype,maxudp,maxtcp,maxudpp,maxtcpp from msauserpolicy order by prio DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"maxudp\":%s,\"maxtcp\":%s,\"maxudpp\":%s,\"maxtcpp\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"maxudp\":%s,\"maxtcp\":%s,\"maxudpp\":%s,\"maxtcpp\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
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



//优先级别修改
int msaUpdatePrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *before,*after,*table;
	 char sql[128];
	 int type;
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
	 type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 before=ltMsgGetVar_s(ltMsgPk,"before");
	 after=ltMsgGetVar_s(ltMsgPk,"after");
	 if(type==0){
	 		sprintf(sql,"update %s set prio=%s where sid=%s",table,after,before);
	 }else if(type==1){
	 		sprintf(sql,"update %s set rulerate=%s where ruleid=%s",table,after,before);
	 }
	 ltDbExecSql(G_DbCon,sql);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",LT_TYPE_STRING,0);
	 ltMsgFree(ltMsgPk);
	 return 0;
}




//策略组添加
int msaAddPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char policy[129],policyname[64],topSrvNameList[36],moreflag[33],g1[251],g2[251],g3[251],g4[251],group[1001];
	 int sid,jftype,ratelimit,ratetype;
	 float accountnum;
	 unsigned int maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc;
	 unsigned long long lLimitBytes,lLimitBytesw,lLimitBytesm;	 
	 //数据库连接
	 /* char *dbUser;
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
		}*/
	 char sqlBuf[2048];
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 memset(moreflag,0,sizeof(moreflag));
	 memset(topSrvNameList,0,sizeof(topSrvNameList));
	 memset(policyname,0,sizeof(policyname));
	 
	 sid=0;
	 if(ltMsgGetVar_s(ltMsgPk,"sid")){
   	sid=atoi(ltMsgGetVar_s(ltMsgPk,"sid"));
   }
   
    
   if(ltMsgGetVar_s(ltMsgPk,"jftype")){
   	jftype=atoi(ltMsgGetVar_s(ltMsgPk,"jftype"));
   }else{
   	jftype=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"accountnum")){
   	accountnum=atof(ltMsgGetVar_s(ltMsgPk,"accountnum"));
   }else{
   	accountnum=0.0;
   }
	 
   if(ltMsgGetVar_s(ltMsgPk,"ratelimit")){
   	ratelimit=atoi(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
   }else{
   	ratelimit=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"ratetype")){  	
   	ratetype=atoi(ltMsgGetVar_s(ltMsgPk,"ratetype"));
   	printf("ratetype:%d\n",ratetype);
   }else{
   	ratetype=0;
   }
   
   
   if(ltMsgGetVar_s(ltMsgPk,"topSrvNameList")){
   	sprintf(topSrvNameList,"%s",ltMsgGetVar_s(ltMsgPk,"topSrvNameList"));
   }else{
   	sprintf(topSrvNameList,"%s","11111111111111111111111111111111111");
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"lLimitBytes")){
   	lLimitBytes=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytes"));
   }else{
   	lLimitBytes=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"maxudp")){
   	maxudp=atol(ltMsgGetVar_s(ltMsgPk,"maxudp"));
   }else{
   	maxudp=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"lLimitTimes")){
   	lLimitTimes=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimes"));
   }else{
   	lLimitTimes=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"maxtcp")){
   	maxtcp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcp"));
   }else{
   	maxtcp=0;
   }
   
   if(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw")){
   	lLimitBytesw=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw"));
   }else{
   	lLimitBytesw=0;
   }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxudpp")){
	 	maxudpp=atol(ltMsgGetVar_s(ltMsgPk,"maxudpp"));
	 }else{
	 	maxudpp=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw")){
	 	lLimitTimesw=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw"));
	 }else{
	 	lLimitTimesw=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxtcpp")){
	 	maxtcpp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcpp"));
	 }else{
	 	maxtcpp=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm")){
	 	lLimitBytesm=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm"));
	 }else{
	 	lLimitBytesm=0;
	 }
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxupp")){
	 	maxupp=atol(ltMsgGetVar_s(ltMsgPk,"maxupp"));
	 }else{
	 	maxupp=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm")){
	 	lLimitTimesm=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm"));
	 }else{
	  lLimitTimesm=0;	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxdownp")){
	 	maxdownp=atol(ltMsgGetVar_s(ltMsgPk,"maxdownp"));
	 }else{
	 	maxdownp=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxpackage")){
	 	 maxpackage=atol(ltMsgGetVar_s(ltMsgPk,"maxpackage"));
	 }else{
	   maxpackage=0;	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxsp")){
	 	 maxsp=atol(ltMsgGetVar_s(ltMsgPk,"maxsp"));
	 }else{
	 	 maxsp=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxBytes")){
	 	 maxBytes=atol(ltMsgGetVar_s(ltMsgPk,"maxBytes"));
	 }else{
	   maxBytes=0;	
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"maxnc")){
	 	 maxnc=atol(ltMsgGetVar_s(ltMsgPk,"maxnc"));
	 }else{
	 	 maxnc=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"policy")){
	 	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 }else{
	   sprintf(policy,"%s","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"group")){
	 	sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 }else{
	 	sprintf(group,"%s","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"moreflag")){
	 	sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"moreflag"));
	 }else{
	 	sprintf(moreflag,"%s","00000000000000000000000000000000");
	 }
 
	 

   memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));
	 
 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 
	 if(ltMsgGetVar_s(ltMsgPk,"policyname")){
   	  sprintf(policyname,"%s",ltMsgGetVar_s(ltMsgPk,"policyname"));
   }
   
	 sprintf(sqlBuf,"update msauserpolicy set policyid='%s',policyname='%s',ratelimit=%d,ratetype=%d,topsrvnamelist='%s',moreflag='%s',lLimitBytesw=%lld,lLimitBytes=%lld,lLimitBytesm=%lld,maxBytes=%d,accountnum=%0.6f,lLimitTimes=%d,lLimitTimesw=%d,lLimitTimesm=%d,jftype=%d,maxpackage=%d,maxudp=%d,maxtcp=%d,maxudpp=%d,maxtcpp=%d,maxupp=%d,maxdownp=%d,maxsp=%d,maxnc=%d,group0='%s',group1='%s',group2='%s',group3='%s' where sid=%d",policy,policyname,ratelimit,ratetype,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,g1,g2,g3,g4,sid);	 
	 printf("mysql:%s \n",sqlBuf);
	 ltDbExecSql(G_DbCon,sqlBuf);	 
	 ltDbClose(G_DbCon);
	 
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	 //lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   
   return 0;
}




//编辑策略组
int msaEditPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int sid=0;
	 char sqlBuf[1024];
	 ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   int myratelimit=0;
   
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");

   if(ltMsgGetVar_s(ltMsgPk,"sid")){
   	 sid=atol(ltMsgGetVar_s(ltMsgPk,"sid"));
   }
   

   sprintf(sqlBuf,"select sid,policyname,prio,ratelimit,ratetype,topsrvnamelist,llimitbytes,llimittimes,llimitbytesw,llimittimesw,llimitbytesm,llimittimesm,moreflag,jftype,accountnum,maxpackage,maxbytes,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,group0,group1,group2,group3,policyid from msauserpolicy where sid=%d",sid);     
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
		    tempRow=ltDbFetchRow(tempCursor);
				if(tempRow){
					//基本参数
					lt_dbput_rootvars(dbPtr,1,"sid",tempRow[0]);
					lt_dbput_rootvars(dbPtr,1,"policyname",tempRow[1]);
					
					char tempjftype[32];
					sprintf(tempjftype,"jftype%s",tempRow[13]);
					lt_dbput_rootvars(dbPtr,1,tempjftype,"selected");
					
					lt_dbput_rootvars(dbPtr,1,"accountnum",tempRow[14]);
					
					myratelimit=atoi(tempRow[3]);
					
					char tempratetype[32];
					sprintf(tempratetype,"ratetype%s",tempRow[4]);
					lt_dbput_rootvars(dbPtr,1,tempratetype,"selected");
					
					//service 服务按钮控制
					lt_dbput_rootvars(dbPtr,1,"topsrvnamelist",tempRow[5]);
					printf("tempRow[5]:%s\n",tempRow[5]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitBytes",tempRow[6]);
					lt_dbput_rootvars(dbPtr,1,"maxudp",tempRow[17]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitTimes",tempRow[7]);
					lt_dbput_rootvars(dbPtr,1,"maxtcp",tempRow[18]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitBytesw",tempRow[8]);
					lt_dbput_rootvars(dbPtr,1,"maxudpp",tempRow[19]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitTimesw",tempRow[9]);
					lt_dbput_rootvars(dbPtr,1,"maxtcpp",tempRow[20]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitBytesm",tempRow[10]);
					lt_dbput_rootvars(dbPtr,1,"maxupp",tempRow[21]);
					
					lt_dbput_rootvars(dbPtr,1,"lLimitTimesm",tempRow[11]);
					lt_dbput_rootvars(dbPtr,1,"maxdownp",tempRow[22]);
					
					lt_dbput_rootvars(dbPtr,1,"maxpackage",tempRow[15]);
					lt_dbput_rootvars(dbPtr,1,"maxsp",tempRow[23]);
					
					lt_dbput_rootvars(dbPtr,1,"maxBytes",tempRow[16]);
					lt_dbput_rootvars(dbPtr,1,"maxnc",tempRow[24]);
					
					//部门
					char mygid[1001];
					memset(mygid,0,sizeof(mygid));
					sprintf(mygid,"%s%s%s%s",tempRow[25],tempRow[26],tempRow[27],tempRow[28]);
					lt_dbput_rootvars(dbPtr,1,"mygid",mygid);
					
					//策略
					lt_dbput_rootvars(dbPtr,1,"policyid",tempRow[29]);
					printf("tempRow[25]:%s\n",tempRow[29]);
					
					//附加属性
					char moreflag[33];
					int i;
				  memcpy(moreflag,tempRow[12],32);
				  printf("moreflag:%s\n",moreflag);
       	 	for(i=0;i<32;i++){
       	 		char tempsel[32];
       	 		sprintf(tempsel,"c%d",i);
       	 		if(moreflag[i]=='1'){
       	 			lt_dbput_rootvars(dbPtr,1,tempsel,"true");
       	 		}else if(moreflag[i]=='0'){
       	 			lt_dbput_rootvars(dbPtr,1,tempsel,"false");
       	 		}
       	 	}
       	 
					      	 				
			}
		 ltDbCloseCursor(tempCursor);
		}
	 
	 
	 	char qossel[32];
		sprintf(qossel,"qsel%d",myratelimit);
		tablePtr=lt_dbput_table(dbPtr,"qoslist");
		memset(sqlBuf,0,sizeof(sqlBuf));	
		sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							lt_dbput_recordvars(tablePtr,3,"qosid",LT_TYPE_STRING,tempRow[0],
								qossel,LT_TYPE_STRING,
								"selected","qosname",LT_TYPE_STRING,tempRow[1]);
						    tempRow=ltDbFetchRow(tempCursor);
					}
			ltDbCloseCursor(tempCursor);
		}
	
	 	ltDbClose(G_DbCon);

	 
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policylistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
}


//服务
int msaServiceToPageTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		 int i;
		 char tmpstr[1024];
		 char tempname[32];
		 stralloc strTemp;
		 char topSrvNameList[MAX_TOP_SRVNUM+1];
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;

	   strTemp.s=0;
	   
	   if(ltMsgGetVar_s(ltMsgPk,"topsrvnamelist")){
	   	  sprintf(topSrvNameList,"%s",ltMsgGetVar_s(ltMsgPk,"topsrvnamelist"));
	 	 }
 
    printf("topSrvNameList:%s\n",topSrvNameList);
	 
	  stralloc_cats(&strTemp,"[");
	   	   	
 	 	for(i=0;i<MAX_TOP_SRVNUM;i++){
			   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
			   		memset(tempname,0,sizeof(tempname));
			   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
			   		if(topSrvNameList[i]=='0'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
							}
						}else if(topSrvNameList[i]=='1'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
						}
						stralloc_cats(&strTemp,tmpstr);	  
		  }
		 }
	 	 
	   stralloc_cats(&strTemp,"]");   
	   stralloc_0(&strTemp);
	    printf("servicetree:%s\n",strTemp.s);
	   
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   
	   return 0;
}




int msaPolicyToPageTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char tmpstr[512];
		 char tempname[32];
	   int i;
	   stralloc strTemp;
	   int iHead;
	   char topPolicyNameList[_MAX_ALLPOLICY+1];
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   strTemp.s=0;
	   
	 	 if(ltMsgGetVar_s(ltMsgPk,"policyid")){
	 	 	  sprintf(topPolicyNameList,"%s",ltMsgGetVar_s(ltMsgPk,"policyid"));
	 	 }

     printf("topPolicyNameList:%s\n",topPolicyNameList);
	 	 stralloc_cats(&strTemp,"[");
	 		 for(i=0;i<_MAX_ALLPOLICY;i++){
			   	if(strcmp(_ltPubInfo->_PolicyList[i].pname,"")!=0){
			   		memset(tempname,0,sizeof(tempname));
			   		sprintf(tempname,"%s",_ltPubInfo->_PolicyList[i].pname);
			   		if(topPolicyNameList[i]=='0'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
							}
						}else if(topPolicyNameList[i]=='1'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
						}
						stralloc_cats(&strTemp,tmpstr);
		   	 }
		   }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   
	   
	   printf("policytree:%s\n",strTemp.s);
	   
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   
	   return 0;
	   
}




//部门主线树
int msaloopgroupToPagetree(stralloc *strTemp,int level,int pid,char gid[]){
	char tmpstr[512]; 
	int iHead;
	int j;
	iHead=1;
	level++;
 
	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				sprintf(tmpstr,"%s",",children: [");
			  stralloc_cats(strTemp,tmpstr);
			  
			  if(gid[j]=='1'){
					sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{			  
				  sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
			  }
				iHead=0;
			}else{
				if(gid[j]=='1'){
					 sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
			     sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
		    }
		  }			
			stralloc_cats(strTemp,tmpstr);
			pid=_ltPubInfo->_GroupList[j].groupid;
			
			if(level<5){
				msaloopgroupToPagetree(strTemp,level,pid,gid);
			}
			stralloc_cats(strTemp,"}");
	  }
  }
  if(iHead==1){
  	  sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			return 0;
  }
  stralloc_cats(strTemp,"]");
	return 0;

}


int msaGroupToPageTreeView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char tmpstr[512];
	int level,pid;
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
  
  char gid[1001];
  printf("test>>>>>>>>>>\n");
  printf("test:%s\n",ltMsgGetVar_s(ltMsgPk,"mygid"));
  if(ltMsgGetVar_s(ltMsgPk,"mygid")){
     sprintf(gid,"%s",ltMsgGetVar_s(ltMsgPk,"mygid"));
  }
  
  printf("gid:%s\n",gid);
  
	pid=0;
	level=0;
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
	
 	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				if(gid[j]=='1'){				
					sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
				 sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}
				iHead=0;
			}else{
				if(gid[j]=='1'){
				  sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
			   sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
		    }
		  }
		  stralloc_cats(&strTemp,tmpstr);	
			pid=_ltPubInfo->_GroupList[j].groupid;	
			if(level<5){
				msaloopgroupToPagetree(&strTemp,level,pid,gid);
			}
			stralloc_cats(&strTemp,"}");	
		}
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}









//显示批量策略模板
int showPolicyApply(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   int j;
   char sid[32];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   tablePtr=lt_dbput_table(dbPtr,"qoslist");
   for(j=0;j<_MAX_QOSSTATIC_CHANEL;j++){
   	if(strcmp(_ltPubInfo->_QosList[j].qosname,"")!=0){
	 		memset(sid,0,sizeof(sid));
	 		sprintf(sid,"%d",_ltPubInfo->_QosList[j].sid);
   		lt_dbput_recordvars(tablePtr,2,
			"qosid",LT_TYPE_STRING,sid,
			"qosname",LT_TYPE_STRING,_ltPubInfo->_QosList[j].qosname);
   	}
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policylistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}




int msaPiLiangAppPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;

	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       		if(allcount>_MAX_USERPOLICY_NUM){
       			allcount=_MAX_USERPOLICY_NUM;
       		}
       	}
       	ltDbCloseCursor(tempCursor);
   }
	 
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select sid,policyname,prio,ratelimit,ratetype,topsrvnamelist,llimitbytes,llimittimes,llimitbytesw,llimittimesw,llimitbytesm,llimittimesm,moreflag,jftype,accountnum,maxpackage,maxbytes,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,group0,group1,group2,group3,policyid from msauserpolicy order by prio DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		  int i;
            for(i=0;i<allcount;i++){
							 	 _ltPubInfo->_userPolicylist[i].useflag=0;
							 	 _ltPubInfo->_userPolicylist[i].sid=atoi(tempRow[0]);
							 	 sprintf(_ltPubInfo->_userPolicylist[i].policyname,"%s",tempRow[1]);
							 	 _ltPubInfo->_userPolicylist[i].prio=atof(tempRow[2]);
							 	 _ltPubInfo->_userPolicylist[i].ratelimit=atoi(tempRow[3]);
							 	 _ltPubInfo->_userPolicylist[i].ratetype=atoi(tempRow[4]);
                 sprintf(_ltPubInfo->_userPolicylist[i].topSrvNameList,"%s",tempRow[5]);
                 _ltPubInfo->_userPolicylist[i].lLimitBytes=atoll(tempRow[6]);	
                 _ltPubInfo->_userPolicylist[i].lLimitTimes=atoi(tempRow[7]);    
                 _ltPubInfo->_userPolicylist[i].lLimitBytesw=atoll(tempRow[8]);
                 _ltPubInfo->_userPolicylist[i].lLimitTimesw=atoi(tempRow[9]);
                 _ltPubInfo->_userPolicylist[i].lLimitBytesm=atoll(tempRow[10]);
                 _ltPubInfo->_userPolicylist[i].lLimitTimesm=atoi(tempRow[11]);
                 sprintf(_ltPubInfo->_userPolicylist[i].moreflag,"%s",tempRow[12]);
                 _ltPubInfo->_userPolicylist[i].jftype=atoi(tempRow[13]);
                 _ltPubInfo->_userPolicylist[i].accountnum=atof(tempRow[14]);
						     _ltPubInfo->_userPolicylist[i].maxpackage=atoi(tempRow[15]);
						     _ltPubInfo->_userPolicylist[i].maxBytes=atoll(tempRow[16]);
                 _ltPubInfo->_userPolicylist[i].maxudp=atoi(tempRow[17]);
                 _ltPubInfo->_userPolicylist[i].maxtcp=atoi(tempRow[18]);
                 _ltPubInfo->_userPolicylist[i].maxudpp=atoi(tempRow[19]);
                 _ltPubInfo->_userPolicylist[i].maxtcpp=atoi(tempRow[20]);
                 _ltPubInfo->_userPolicylist[i].maxupp=atoi(tempRow[21]);
                 _ltPubInfo->_userPolicylist[i].maxdownp=atoi(tempRow[22]);
                 _ltPubInfo->_userPolicylist[i].maxsp=atoi(tempRow[23]);
                 _ltPubInfo->_userPolicylist[i].maxnc=atoi(tempRow[24]);              
                 	//部门
									char mygid[1001];
									memset(mygid,0,sizeof(mygid));
									sprintf(mygid,"%s%s%s%s",tempRow[25],tempRow[26],tempRow[27],tempRow[28]);                 
                  sprintf(_ltPubInfo->_userPolicylist[i].grouplist,"%s",mygid);

						      sprintf(_ltPubInfo->_userPolicylist[i].pid,"%s",tempRow[29]);
						      printf("apply load data success..............\n");
						 }

       		tempRow=ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   
	 return 0;
}



