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

/*
**抓包
*/
int naspackageok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char   caCmd[512];
  char   *ethdev;
  char   *packagenum;
  char   *ipaddr;
  char   *portnum;
  int    haveip;
  int    saveshow;
  char   caTempDir[512];
  char   caBack[256];
  char   sDate[36];
  char   sTime[36];
  
  haveip=0;
  ethdev=ltMsgGetVar_s(ltMsgPk,"ethdev");
  packagenum=ltMsgGetVar_s(ltMsgPk,"packagenum");
  ipaddr=ltMsgGetVar_s(ltMsgPk,"ipaddr");
  saveshow=atoi(ltMsgGetVar_s(ltMsgPk,"saveshow"));  
  portnum=ltMsgGetVar_s(ltMsgPk,"portnum");

  sprintf(caCmd,"/bin/tcpdump -c %s -i %s ",packagenum,ethdev);
  if(strlen(ipaddr)>4){
       sprintf(caCmd,"%s host %s ",caCmd,ipaddr);
       haveip=1;
  }  
  if(strlen(portnum)>1){
       if(haveip==1){
           sprintf(caCmd,"%s and port %s ",caCmd,portnum);
       }else{
           sprintf(caCmd,"%s port %s ",caCmd,portnum);
    }
  }

  nasCvtStime(time(0),sDate,sTime);
  sprintf(caTempDir,"/app/mnt/localback/package/%lu/",ltStrGetId());
  if(ltFileIsExist(caTempDir)!=1) {
    if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
        /// 生成该报告目录错误
        printf("ASASAS生成报告错误......\n");
        return 0;
    }
  }
  sprintf(caBack,"/bin/echo 'date=%s-%s' > %sinfo.ini",sDate,sTime,caTempDir);
  system(caBack);//建立目录标志
  sprintf(caBack,"/bin/echo 'name=%s-%s' >> %sinfo.ini",sDate,sTime,caTempDir);
  system(caBack);//建立目录标志
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body>Success</body></html>");    
  sprintf(caBack,"/bin/echo '1' > %sproccess.ini",caTempDir);
  system(caBack);//建立目录正在处理标志
  if(saveshow==1){
     sprintf(caCmd,"%s > %spackage.txt ",caCmd,caTempDir);
     system(caCmd);
  }else{
     sprintf(caCmd,"%s -w %spackage.cap ",caCmd,caTempDir);
     system(caCmd);
  }
  sprintf(caCmd,"/bin/rm  %sproccess.ini",caTempDir);
  system(caCmd);
  ltMsgFree(ltMsgPk); 
  return 0;
}
int naspackagelist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char caCmd[256];
   char caTmpp[512];
   stralloc strTemp;
   int k;
   char caFile1[256];
   char cDate[128];
   char reportname[64];
   int  fileproc;
   struct stat st;
   int len=0; //sum of file names 
   msasDir *fnames;
   sprintf(caFile1,"%s","/app/mnt/localback/package/");//pFile="/etc/msa/msa/dev/dev0";
   fnames=mydirlist(caFile1,&len);
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   
   int cadeflag;
	 if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL){
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1){
				 char *filedir;
         filedir=ltMsgGetVar_s(ltMsgPk,"del_id");
         if(filedir){
		       if(strlen(filedir)>1){
		          sprintf(caCmd,"rm -Rf /app/mnt/localback/package/%s",filedir);
		          system(caCmd);
		       }else{
		          ltMsgPk->msgpktype=1;
		          lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
		       }
			   }else{
			       ltMsgPk->msgpktype=1;
			       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
			   }
			}
			if(cadeflag==2){
				 char *filedir;
         filedir=ltMsgGetVar_s(ltMsgPk,"del_id");
         if(filedir){
		       if(strlen(filedir)>1){
		          sprintf(caCmd,"rm -Rf /app/mnt/localback/package/%s",filedir);
		          system(caCmd);
		       }else{
		          ltMsgPk->msgpktype=1;
		          lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
		       }
			   }else{
			       ltMsgPk->msgpktype=1;
			       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
			   }
			}
			ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
		  ltMsgFree(ltMsgPk);
		  return 0;	   
	 }
   while(len--){
      fileproc=0;
      memset(reportname,0,sizeof(reportname));
      memset(cDate,0,sizeof(cDate));                                                             
      sprintf(caFile1,"/app/mnt/localback/package/%s/info.ini",fnames[len].caDirName);
      bcCnfGetValue_s(caFile1,"date",cDate);
      bcCnfGetValue_s(caFile1,"name",reportname);
      if(k==0){
	       sprintf(caFile1,"/app/mnt/localback/package/%s/proccess.ini",fnames[len].caDirName);
	       if(stat(caFile1,&st) == 0){
	              fileproc=1;
	       }       
	       sprintf(caFile1,"/app/mnt/localback/package/%s/package.txt",fnames[len].caDirName);
	       if(stat(caFile1,&st) == 0){
				       	sprintf(caTmpp,"{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully");
				       	stralloc_cats(&strTemp,caTmpp);
	       }else{
				       	sprintf(caTmpp,"{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully");
				       	stralloc_cats(&strTemp,caTmpp);
	       }
      }else{
	       //sprintf(caFile1,"/app/mnt/localback/package/%s/proccess.ini",fnames[len].caDirName);
	       if(stat(caFile1,&st) == 0){
	                fileproc=1;
	       }       
	       sprintf(caFile1,"/app/mnt/localback/package/%s/package.txt",fnames[len].caDirName);
	       if(stat(caFile1,&st) == 0){
	       	sprintf(caTmpp,",{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully");
	       	stralloc_cats(&strTemp,caTmpp);
	       }else{
	       	sprintf(caTmpp,",{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully");
	       	stralloc_cats(&strTemp,caTmpp);
	       }
      }
      k++;
   }
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//下载数据包
/*1
int msaloadPack(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *downLoadFile;
    downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
printf("myfile: %s\n",downLoadFile);
    ltMsgPk->msgpktype=2;
		lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"package.cap","contenttype","Application/MS-DOWNLOAD");
    
    ltMsgFree(ltMsgPk); 
printf("~~~~~~~~~~~~~~~~~\n");
		return 0;
}*/
/*2
int msaloadPack(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *downLoadFile;
    char cafile[512];
    int  fd;
    static char inbuf[4096];
	  static stralloc line = {0};
		buffer ss;
	  int    match;
	  int    j;
    downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
    if(!downLoadFile){
    	downLoadFile="";
    	ltMsgPk->msgpktype=2;
			lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"package.cap","contenttype","Application/MS-DOWNLOAD");
		  write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
		  return 0;
    }
    sprintf(cafile,"%s/%s",_datacenterdir,downLoadFile);
    fd=open(cafile,O_RDONLY | O_NDELAY); 
		ltMsgPk->msgpktype=2;
		lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"package.cap","contenttype","Application/MS-DOWNLOAD");
	  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
		for (;;)
	  {
			if (getln(&ss,&line,&match,'\n') == -1) break;
			if (!match && !line.len) { break; }
			for(j=0;j<line.len;j++){
					 if(line.s[j]=='\t'){
					  line.s[j]=',';
				  }
			}
			write(confd,line.s,line.len);
	  }
   ltMsgFree(ltMsgPk);    
   return 0;
}*/

int msaloadPack(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *downLoadFile;
    char cafile[512];
    int  fd,iCount;
    char inbuf[8192];
    char *downLoadFileName;
    downLoadFileName=ltMsgGetVar_s(ltMsgPk,"downLoadFileName");
    downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
    if(!downLoadFile){
        downLoadFile="";
        ltMsgPk->msgpktype=2;
        lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.txt","contenttype","Application/MS-DOWNLOAD");
        write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
       return 0;
    }
    sprintf(cafile,"%s/%s",_datacenterdir,downLoadFile);
    fd=open(cafile,O_RDONLY | O_NDELAY); 
    if(fd<1){
        ltMsgPk->msgpktype=2;
        lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.txt","contenttype","Application/MS-DOWNLOAD");
        write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
       return 0;
    }
    ltMsgPk->msgpktype=2;
    lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,downLoadFileName,"contenttype","Application/MS-DOWNLOAD");
    iCount=read(fd,inbuf,8192); 
    while(iCount){ 
       write(confd,inbuf,iCount);
       iCount=read(fd,inbuf,8192);
    } 
    close(fd);
   ltMsgFree(ltMsgPk);    
   return 0;
}

