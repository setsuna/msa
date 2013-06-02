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

#define SYSSECCONFIG_PATH "/etc/msa/msa/syssecconfig" //系统安全设置配置文件存放路径

/*
MSA:策略中心
		系统安全策略
		设置系统安全策略 调用函数
*/
int msasecurityset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *closeping;
   char *strenadpass;
   char *strenuserpass;
   char *notestservice;
   char *closessh;
   
   int  fdwrite=0;
   char caFile[256];
   char caCmd[512];
   

	 memset(caFile,0,sizeof(caFile));
	 memset(caCmd,0,sizeof(caCmd));
	 if(ltMsgGetVar_s(ltMsgPk,"closeping")){
	 	 closeping=ltMsgGetVar_s(ltMsgPk,"closeping");
	 }else{
	 	 closeping="no";
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
	 
	 
	 
	sprintf(caFile,"%s/syssecsetting.ini",SYSSECCONFIG_PATH);
	fdwrite = open(caFile,O_WRONLY | O_CREAT, 0644);
	if(fdwrite == (-1)) {
			ltMsgPk->msgpktype=1;
	 		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
   		ltMsgFree(ltMsgPk);
	 		return 0;
	}
	
	/*存储配置信息*/
	sprintf(caCmd,"closeping=%s\n",closeping);
	write(fdwrite,caCmd,strlen(caCmd));
	memset(caCmd,0,sizeof(caCmd));
	sprintf(caCmd,"strenadpass=%s\n",strenadpass);
	write(fdwrite,caCmd,strlen(caCmd));
	memset(caCmd,0,sizeof(caCmd));
	sprintf(caCmd,"strenuserpass=%s\n",strenuserpass);
	write(fdwrite,caCmd,strlen(caCmd));
	memset(caCmd,0,sizeof(caCmd));
	sprintf(caCmd,"notestservice=%s\n",notestservice);
	write(fdwrite,caCmd,strlen(caCmd));
  memset(caCmd,0,sizeof(caCmd));
	sprintf(caCmd,"closessh=%s\n",closessh);
	write(fdwrite,caCmd,strlen(caCmd));
	
	close(fdwrite);
	
	/*启动脚本*/
	 sprintf(caFile,"/etc/msa/msa/start/sec.up");
	 fdwrite = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);		
	 if(fdwrite==-1){
	 	  
	 		ltMsgPk->msgpktype=1;
	  	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
    	ltMsgFree(ltMsgPk);
    	
	    return 0;	
	 }
	 
	 char tmpCloseping[64];
	 char tmpStrenadpass[64];
	 char tmpStrenuserpass[64];
	 char tmpNotestservice[64];
	 char tmpClosessh[64];
	 
	 sprintf(caFile,"%s/syssecsetting.ini",SYSSECCONFIG_PATH);
	 
	 bcCnfGetValue_s(caFile,"closeping",tmpCloseping);
	 bcCnfGetValue_s(caFile,"strenadpass",tmpStrenadpass);
	 bcCnfGetValue_s(caFile,"strenuserpass",tmpStrenuserpass);
	 bcCnfGetValue_s(caFile,"notestservice",tmpNotestservice);
	 bcCnfGetValue_s(caFile,"closessh",tmpClosessh);
	 
	 if(strcmp(tmpCloseping,"yes")==0){//关闭ping
	 		 sprintf(caCmd,"echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all");
	 		 system(caCmd);//确认时须执行
			 write(fdwrite,caCmd,sizeof(caCmd));//写入脚本 系统开机时执行
	 }else{//响应ping
	 		 sprintf(caCmd,"echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_all");
	 		 system(caCmd);//确认时须执行
			 write(fdwrite,caCmd,sizeof(caCmd));//写入脚本 系统开机时执行
	 }

   if(strcmp(tmpStrenadpass,"yes")==0){//管理员密码强化安全   更新内存即可
   		 _ltPubInfo->_msaSecHead.adminpasssec=1;
   }else{
   		 _ltPubInfo->_msaSecHead.adminpasssec=0;
   }
   
   if(strcmp(tmpStrenuserpass,"yes")==0){//用户密码强化安全   更新内存即可
   		 _ltPubInfo->_msaSecHead.userpasssec=1;
   }else{
   		 _ltPubInfo->_msaSecHead.userpasssec=0;
   }
   
  
   
   if(strcmp(notestservice,"yes")==0){ //不启动验证服务 	 		  	
				    sprintf(caFile,"cp -f /app/httpd/conf/httpd.conf.plt2  /app/httpd/conf/httpd.conf");
				    system(caFile);
					
   }else{
   					sprintf(caFile,"cp -f /app/httpd/conf/httpd.conf.plt1  /app/httpd/conf/httpd.conf");
				    system(caFile);
   }
   
   
   if(strcmp(closessh,"yes")==0){//关闭
//			 sprintf(caCmd,"chkconfig sshd off");  
//			 system(caCmd);
//			 write(fdwrite,caCmd,sizeof(caCmd));
         printf("hello,world\n");
   }else{//开启ssh
//   		 sprintf(caCmd,"/etc/init.d/sshd start");  
//			 system(caCmd);
//			 write(fdwrite,caCmd,sizeof(caCmd));
				printf("hello,world\n");
   }
	 
	 close(fdwrite);
	

	ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
  ltMsgFree(ltMsgPk);

  printf("function end .....\n");
  return 0;
}

/*
MSA:策略中心
		系统安全策略
		设置系统安全策略 页面链接调用
*/
int msasecurityLink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
   char tmpCloseping[64];
	 char tmpStrenadpass[64];
	 char tmpStrenuserpass[64];
	 char tmpNotestservice[64];
	 char tmpClosessh[64];
	 struct stat st;
	 char caFile[64];
	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 memset(tmpCloseping,0,sizeof(tmpCloseping));
	  memset(tmpStrenadpass,0,sizeof(tmpStrenadpass));
	   memset(tmpStrenuserpass,0,sizeof(tmpStrenuserpass));
	    memset(tmpNotestservice,0,sizeof(tmpNotestservice));
	     memset(tmpClosessh,0,sizeof(tmpClosessh));
	     memset(caFile,0,sizeof(caFile));
	 sprintf(caFile,"%s/syssecsetting.ini",SYSSECCONFIG_PATH);
   if(stat(caFile,&st) == 0){
    	 bcCnfGetValue_s(caFile,"closeping",tmpCloseping);
			 bcCnfGetValue_s(caFile,"strenadpass",tmpStrenadpass);
			 bcCnfGetValue_s(caFile,"strenuserpass",tmpStrenuserpass);
			 bcCnfGetValue_s(caFile,"notestservice",tmpNotestservice);
			 bcCnfGetValue_s(caFile,"closessh",tmpClosessh); 			
   }
   
   if(strcmp(tmpCloseping,"yes")==0){
   	   lt_dbput_rootvars(dbPtr,1,"closeping","checked");
   }else{
   	   lt_dbput_rootvars(dbPtr,1,"closeping","");
   }
   
   if(strcmp(tmpStrenadpass,"yes")==0){
   	  lt_dbput_rootvars(dbPtr,1,"strenadpass","checked");
   }else{
   	  lt_dbput_rootvars(dbPtr,1,"strenadpass","");
   }
   
   if(strcmp(tmpStrenuserpass,"yes")==0){
   	  lt_dbput_rootvars(dbPtr,1,"strenuserpass","checked");
   }else{
   	  lt_dbput_rootvars(dbPtr,1,"strenuserpass","");
   }
   
   if(strcmp(tmpNotestservice,"yes")==0){
   	  lt_dbput_rootvars(dbPtr,1,"notestservice","checked");
   }else{
   	  lt_dbput_rootvars(dbPtr,1,"notestservice","");
   }
   
   if(strcmp(tmpClosessh,"yes")==0){
   	  lt_dbput_rootvars(dbPtr,1,"closessh","checked");
   }else{
   	  lt_dbput_rootvars(dbPtr,1,"closessh","");
   }
	 
  
	 
 
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/syssecurityset.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
   
	 return 0;   
}


