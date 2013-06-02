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

#define ROUTCONFIG_PATH "/etc/msa/msa/routconfig"

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
 	 char dns[64];
 	 char bakdns[64];
 	 char dfgetway[64];
 	 int i=0;
 	 struct stat st; 
 	 
 	 memset(dns,0,sizeof(dns));
 	 memset(bakdns,0,sizeof(bakdns));
 	 memset(dfgetway,0,sizeof(dfgetway));
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
	 	    bcCnfGetValue_s(caFile,"DNS",dns);
	 	    bcCnfGetValue_s(caFile,"BAkDNS",bakdns);
	 	    bcCnfGetValue_s(caFile,"DGateway",dfgetway);
	 	    lt_dbput_rootvars(dbPtr,3,"dns",dns,"bakdns",bakdns,"dfgetway",dfgetway);
	 		 for(i =1 ;i< 13;i++){
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

 	 char dip[64];
 	 char subask[64];
 	 char getway[64];
 	 char start[64];
 	 
 	 char Html_dip[64];
 	 char Html_subask[64];
 	 char Html_getway[64];
 	 char Html_start[64];
 	 
 	 char dns[64];
 	 char bakdns[64];
 	 char dfgetway[64];
 	 
 	 int i=0;

	 char   caFile[256];
	 char   caFile1[256];
	 char   caCmd[1024];
	 int    fdwrite=0;
	 int    fd=0;
	 struct stat st; 
	 
	 memset(caFile1,0,sizeof(caFile1));
	 memset(caFile,0,sizeof(caFile));
	 memset(caCmd,0,sizeof(caCmd));
	 memset(dns,0,sizeof(dns));
 	 memset(bakdns,0,sizeof(bakdns));
 	 memset(dfgetway,0,sizeof(dfgetway));
 	
 	 
 	 memset(dip,0,sizeof(dip));
 	 memset(subask,0,sizeof(subask));
 	 memset(getway,0,sizeof(getway));
 	 memset(start,0,sizeof(start));
 	 
 	 memset(Html_dip,0,sizeof(Html_dip));
 	 memset(Html_subask,0,sizeof(Html_subask));
 	 memset(Html_getway,0,sizeof(Html_getway));
 	 memset(Html_start,0,sizeof(Html_start));


	  sprintf(caFile,"%s/routsetting.ini",ROUTCONFIG_PATH);
		fdwrite = open(caFile,O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
			ltMsgFree(ltMsgPk);
	 		return 0;
		}
	 
	   sprintf(caCmd,"DNS=%s\n",ltMsgGetVar_s(ltMsgPk,"dns"));
		 write(fdwrite,caCmd,strlen(caCmd));
		 sprintf(caCmd,"BAkDNS=%s\n",ltMsgGetVar_s(ltMsgPk,"bakdns"));
		 write(fdwrite,caCmd,strlen(caCmd));
		 if(strcmp(ltMsgGetVar_s(ltMsgPk,"dfgetway"),"")==0){
		 	 sprintf(caCmd,"DGateway=%s\n","192.168.0.1");
		 }else{
		 	 sprintf(caCmd,"DGateway=%s\n",ltMsgGetVar_s(ltMsgPk,"dfgetway"));
		 }
		
		 write(fdwrite,caCmd,strlen(caCmd));

 	   for(i =1 ;i< 13;i++){
	 		 	  sprintf(dip,"dip%d",i);
	 		 	  sprintf(subask,"subask%d",i);
	 		 	  sprintf(getway,"getway%d",i);
	 		 	  sprintf(start,"start%d",i);
					sprintf(caCmd,"dip%d=%s\n",i,ltMsgGetVar_s(ltMsgPk,dip));
					write(fdwrite,caCmd,strlen(caCmd));     
					sprintf(caCmd,"subask%d=%s\n",i,ltMsgGetVar_s(ltMsgPk,subask));
					write(fdwrite,caCmd,strlen(caCmd));
					sprintf(caCmd,"getway%d=%s\n",i,ltMsgGetVar_s(ltMsgPk,getway));
					write(fdwrite,caCmd,strlen(caCmd));
					sprintf(caCmd,"start%d=%s\n",i,ltMsgGetVar_s(ltMsgPk,start));
					write(fdwrite,caCmd,strlen(caCmd));
     }
	 	
		close(fdwrite);		
		
	 int iReturn=-1;
	 sprintf(caFile1,"/etc/msa/msa/start/rout.up");
	 iReturn=access(caFile1,F_OK);
	 if(iReturn==0){
	 	memset(caCmd,0,sizeof(caFile1));
	 	sprintf(caFile1,"rm -f /etc/msa/msa/start/rout.up");
	 	system(caFile1);
	 }
	 memset(caCmd,0,sizeof(caFile1));
	 sprintf(caFile1,"/etc/msa/msa/start/rout.up");
	 fd = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0755);		
	 if(fd==-1){
	 	 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		 ltMsgFree(ltMsgPk);
	 	
	   return 0;	
	 }
		
		
		sprintf(caFile,"%s/routsetting.ini",ROUTCONFIG_PATH);
		if(stat(caFile,&st) == 0){						 
			

			 bcCnfGetValue_s(caFile,"DNS",dns);
			 if(strlen(dns)>0){//首选DNS
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"echo 'nameserver %s' > /etc/resovle.conf",dns);
					write(fd,caCmd,strlen(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 bcCnfGetValue_s(caFile,"BAkDNS",bakdns);
			 if(strlen(bakdns)>0){//默认DNS
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"echo 'nameserver %s' >> /etc/resovle.conf",bakdns);
    	 	  write(fd,caCmd,strlen(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }
    	 
    	 if(strlen(dns)>0||strlen(bakdns)>0){
    	 	 	memset(caCmd,0,sizeof(caCmd));
					sprintf(caCmd,"chmod 644 /etc/resovle.conf");
		 	    write(fd,caCmd,strlen(caCmd));
					write(fd,"\r\n",strlen("\r\n"));	 
    	 }
 	     
	  
    	 
    	  for(i =1 ;i< 13;i++){
		 		 	 sprintf(dip,"dip%d",i);
		 		 	 sprintf(subask,"subask%d",i);
		 		 	 sprintf(getway,"getway%d",i);
		 		 	 sprintf(start,"start%d",i);
				 	 bcCnfGetValue_s(caFile,dip,Html_dip);
				 	 bcCnfGetValue_s(caFile,subask,Html_subask);
			     bcCnfGetValue_s(caFile,getway,Html_getway);
			     bcCnfGetValue_s(caFile,start,Html_start);
			     
			     if(strcmp(Html_start,"yes")==0){
				     	memset(caCmd,0,sizeof(caCmd));
	    	 	    sprintf(caCmd,"route add -net %s netmask  %s  gw %s ",Html_dip,Html_subask,Html_getway);
	    	 	    write(fd,caCmd,strlen(caCmd));
					  	write(fd,"\r\n",strlen("\r\n"));
			     }
	     }
	     
	     bcCnfGetValue_s(caFile,"DGateway",dfgetway);
			 if(strlen(dfgetway)>0){//添加默认网关
    	 	  memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add default gw %s",dfgetway);
    	 	  write(fd,caCmd,strlen(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }else{
    	   	memset(caCmd,0,sizeof(caCmd));
    	 	  sprintf(caCmd,"route add default gw %s","192.168.0.1");
    	 	  write(fd,caCmd,strlen(caCmd));
					write(fd,"\r\n",strlen("\r\n"));
    	 }

		}else{
			
			ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		  ltMsgFree(ltMsgPk);
		  
		  return 0;
		}			
		close(fd);
	  

		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
		ltMsgFree(ltMsgPk);
   
    return 0;
    
}

