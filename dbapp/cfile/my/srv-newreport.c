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

/*到定义模板页面*/
int toreporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltDbHeadPtr dbPtr;
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  int type;
  type=atoi(ltMsgGetVar_s(ltMsgPk,"type")); 
  if(type==1){
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/reporttemplatelist.htm",dbPtr->head,0);
	}else{
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/reportstylesetting.htm",dbPtr->head,0);
	}
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
 
  return 0;

}

/*
接收页面的数据 保存到文件 
*/
int reporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
  char caLabel[256]; 
  int  gid;
  int  userid;
  char element[50];
  char checkb[10];
  int  deployscope0;
  int  deployscope1;
  char *templname=NULL;  
  char *topusernum;   
  char caTempDir[123];
  long tempindex;  
  char temptype0[256];
  char temptype1[256];
  char temptype[256];
  char tempdir[64];
  char email[64];
  char language[4];
  char createDate[36];
  long lTime;  
  ltDbHeadPtr dbPtr;
  
  tempindex=time(0);
  gid=atoi(ltMsgGetVar_s(ltMsgPk,"groupid")); 
  topusernum=ltMsgGetVar_s(ltMsgPk,"topusernum");
  userid=atoi(ltMsgGetVar_s(ltMsgPk,"user"));
  if( ltMsgGetVar_s(ltMsgPk,"templname")==NULL){
     sprintf(templname,"%s","");
  }else{
     templname=ltMsgGetVar_s(ltMsgPk,"templname");
  } 
 
printf("templname:%s\n",templname);
   
	deployscope0=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope0"));
	deployscope1=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope1"));

  memset(element,0,sizeof(element));
  memset(checkb,0,sizeof(checkb));
  memset(element,'0',50);            
  memset(checkb,'0',10);
 
  sprintf(element,ltMsgGetVar_s(ltMsgPk,"element"));
  sprintf(checkb,ltMsgGetVar_s(ltMsgPk,"checkb"));
 
  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
       sprintf(email,"%s",""); 
  }else{
       sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
  }

 	memset(language,0,sizeof(language));

  sprintf(language,"%s",ltMsgGetVar_s(ltMsgPk,"language"));
	 printf("gid:%d\n",gid);
	 printf("userid:%d\n",userid);
	 printf("element:%s\n",element);
	 printf("checkb:%s\n",checkb);
	 printf("templname:%s\n",templname);
	 printf("email:%s\n",email);
	 printf("language:%s\n",language);

	 lTime = time(0);
	 nasTimeGetDate(createDate,lTime);
 
   sprintf(caTempDir,"/datacenter/msa/report/page_data/%ld/",tempindex); 
   if(deployscope0==1){ 
	   	sprintf(temptype0,"%s","每天自动生成报告");
	    memset(temptype,0,sizeof(temptype));
	   	strgb2utf8(temptype0,temptype,strlen(temptype0));
	   	sprintf(temptype0,"%s",temptype);
 	 }else{
   	sprintf(temptype0,"%s","");
 	 }
 	 
 	 if(deployscope1==1){
   		sprintf(temptype1,"%s","每周自动生成报告");
	    memset(temptype,0,sizeof(temptype));
	   	strgb2utf8(temptype1,temptype,strlen(temptype1));
	   	sprintf(temptype1,"%s",temptype);
 	 }else{
   	sprintf(temptype1,"%s","");
 	 }
   
   sprintf(tempdir,"dailyreport");
   if(ltFileIsExist(caTempDir)!=1) {
         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
          /// 生成该报告目录错误 
          printf("mkdir daily report dir error!\n");
          return 0;
         }
   }  
 
  sprintf(caLabel,"echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
  system(caLabel); 
  sprintf(caLabel,"echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);
  system(caLabel); 
  sprintf(caLabel,"echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'element=%s' >> %sinfo.ini",element,caTempDir);
  system(caLabel); 
  sprintf(caLabel,"echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'temptype0=%s' >> %sinfo.ini",temptype0,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'temptype1=%s' >> %sinfo.ini",temptype1,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);
  system(caLabel);
  if(deployscope0==1){
	  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",0,caTempDir);
	  system(caLabel);
	}else{
	  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",1,caTempDir);
	  system(caLabel);
	}
	
  if(deployscope1==1){
	  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",1,caTempDir);
	  system(caLabel);
	}else{
	  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",0,caTempDir);
	  system(caLabel);
	}
	
  sprintf(caLabel,"echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'email=%s' >> %sinfo.ini",email,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
  system(caLabel);
 
 dbPtr=lt_dbinit();
 lt_db_htmlpage(dbPtr,"utf-8");
 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/reportstylesetting.htm",dbPtr->head,0);
 ltMsgFree(ltMsgPk); 
 
 printf("program end ....\n");
  
 return 0;
 

}
