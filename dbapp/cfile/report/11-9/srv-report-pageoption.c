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

int reporttempstatus(char *tempindex,char *reportname,ltMsgHead *ltMsgPk,int confd);


/*
MSA:报告中心
新建报告模板
链接到定义模板页面调用函数
*/
int msareporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbHeadPtr dbPtr;
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8"); 
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/report/reportstylesetting.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}


/*
MSA:报告中心
    模板创建提交函数 
*/
int reporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char caLabel[256]; 
  char element[50];
  char checkb[10];
  int  deployscope0;
  int  deployscope1;
  char *templname=NULL;    
  char caTempDir[123];
  long tempindex;  
  char temptype0[256];
  char temptype1[256];
  char temptype[256];
  char tempdir[64];
  char email[64];
  char language[4];
  char topusernum[4];
  char createDate[36];
  long lTime;  
  
  
  memset(element,'0',50);            
  memset(checkb,'0',10);
  memset(language,0,sizeof(language));
 	memset(topusernum,0,sizeof(topusernum));
 
  if(ltMsgGetVar_s(ltMsgPk,"templname")==NULL){
     sprintf(templname,"%s","");
  }else{
     templname=ltMsgGetVar_s(ltMsgPk,"templname");
  } 
  
  if(ltMsgGetVar_s(ltMsgPk,"deployscope0")){//有值
  	deployscope0=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope0"));
  }else{
  	deployscope0=0;//否则初始化为0
  }
	
	if(ltMsgGetVar_s(ltMsgPk,"deployscope1")){//有值
		deployscope1=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope1"));
	}else{
		deployscope1=0;//否则初始化为0
	}
	
	
  if(ltMsgGetVar_s(ltMsgPk,"element")){
  	 sprintf(element,"%s",ltMsgGetVar_s(ltMsgPk,"element"));
  }else{
  	 sprintf(element,"%s","");
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"checkb")){
  	 sprintf(checkb,"%s",ltMsgGetVar_s(ltMsgPk,"checkb"));
  }else{
  	 sprintf(checkb,"%s","");
  }
 
 
  if(ltMsgGetVar_s(ltMsgPk,"email")==NULL){
     sprintf(email,"%s",""); 
  }else{
     sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
  }


  if(ltMsgGetVar_s(ltMsgPk,"language")){
  	 sprintf(language,"%s",ltMsgGetVar_s(ltMsgPk,"language"));
  }else{
  	 sprintf(language,"%s","");
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"topusernum")){
  	 sprintf(topusernum,"%s",ltMsgGetVar_s(ltMsgPk,"topusernum"));
  }else{
  	 sprintf(topusernum,"%s","");
  }
  
	 
   tempindex=time(0);
	 lTime = time(0);//获取当前系统时间
	 nasTimeGetDate(createDate,lTime);
   sprintf(caTempDir,"/datacenter/msa/report/page_data/%ld/",tempindex); //建立模板存储目录
   if(deployscope0==1){ 
	   	sprintf(temptype0,"%s","每天自动生成报告");
	    memset(temptype,0,sizeof(temptype));
	   	strgb2utf8(temptype0,temptype,strlen(temptype0));
	   	sprintf(temptype0,"%s",temptype);
	   	sprintf(tempdir,"dailyreport");
 	 }else{
   		sprintf(temptype0,"%s","");
 	 }
 	 
 	 if(deployscope1==1){
   		sprintf(temptype1,"%s","每周自动生成报告");
	    memset(temptype,0,sizeof(temptype));
	   	strgb2utf8(temptype1,temptype,strlen(temptype1));
	   	sprintf(temptype1,"%s",temptype);
	   	sprintf(tempdir,"dailygrpreport");
 	 }else{
   		sprintf(temptype1,"%s","");
 	 }
 	 
 	 if(deployscope0==0 && deployscope1==0){//两个都未勾选
 	 		sprintf(tempdir,"instantreport");
 	 }
   
 
   if(ltFileIsExist(caTempDir)!=1) {//判断目录是否存在
       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {       
        return 0;
       }
   }  
 
  sprintf(caLabel,"echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
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
  
  if(deployscope0==1){//等于1表示页面勾选每天自动生成报告模板
	  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",1,caTempDir);
	}else{
	  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",0,caTempDir);
	}
	system(caLabel);
	
  if(deployscope1==1){//等于1表示页面勾选每周自动生成报告模板
	  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",1,caTempDir);
	}else{
	  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",0,caTempDir);
	}
	system(caLabel);
	
  sprintf(caLabel,"echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'email=%s' >> %sinfo.ini",email,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  system(caLabel);
  sprintf(caLabel,"echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
  system(caLabel);
 
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
 	ltMsgFree(ltMsgPk); 
 	
  return 0;
 
}



/*已定义报告模板列表函数*/
int toreporttemplatelist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		char caFile1[256];
		ltDbHeadPtr dbPtr;
		msasDir *fnames;
		int len=0;
		char tempdir[128];
		char flowInfo[30000],flowInfodata[3000];
		char reportname[64];//模板名称
		char reporttype0[64];
		char reporttype1[64];
		char reporttype[64];
		char tempreporttype[64];
		char reportdir[64];
		char createDate[36];
		char   typ[10];
		char   typ0[10];
		char   typ1[10];
		char   *filedir;
		char   caCmd[256];
 
   filedir=ltMsgGetVar_s(ltMsgPk,"file");
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
    if((ltMsgGetVar_s(ltMsgPk,"filed"))!=NULL && (ltMsgGetVar_s(ltMsgPk,"reportdir"))!=NULL){   //删除某模板下的报告
				filedir=ltMsgGetVar_s(ltMsgPk,"filed");
				sprintf(caCmd,"rm -Rf /datacenter/msa/report/%s/%s",ltMsgGetVar_s(ltMsgPk,"reportdir"),filedir);
        system(caCmd);
        sleep(1);
    }

    if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){//delete
				filedir=ltMsgGetVar_s(ltMsgPk,"file");
				sprintf(caCmd,"rm -Rf /datacenter/msa/report/page_data/%s",filedir);//删除模板
        system(caCmd);
        sleep(1);
    }

    sprintf(reportdir,"/datacenter/msa/report/page_data/");
    sprintf(caFile1,"/datacenter/msa/report/page_data/");
    fnames=mydirlist(caFile1,&len);
	  if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	  }

	  memset(flowInfo,0,sizeof(flowInfo));
	  sprintf(flowInfo,"[");
    while(len--){
			memset(reportname,0,sizeof(reportname));
		  memset(reporttype0,0,sizeof(reporttype0));
		  memset(reporttype1,0,sizeof(reporttype1));
		  memset(typ,0,sizeof(typ));
		  memset(typ0,0,sizeof(typ0));
		  memset(typ1,0,sizeof(typ1));
			memset(tempdir,0,sizeof(tempdir));
			sprintf(caFile1,"/datacenter/msa/report/page_data/%s/info.ini",fnames[len].caDirName);
	    bcCnfGetValue_s(caFile1,"tempdir",tempdir);
	    bcCnfGetValue_s(caFile1,"templname",reportname);
	    bcCnfGetValue_s(caFile1,"temptype0",reporttype0);//每天自动生成报告
	    bcCnfGetValue_s(caFile1,"temptype1",reporttype1);//每周自动生成报告
	    bcCnfGetValue_s(caFile1,"type0",typ0);
	    bcCnfGetValue_s(caFile1,"type1",typ1);
			bcCnfGetValue_s(caFile1,"createDate",createDate);
			sprintf(caFile1,"/datacenter/msa/report/page_data/%s/errmsg.ini",fnames[len].caDirName);
		  if(strcmp(typ0,typ1)==0 && strcmp(typ0,"1")==0){//两者都勾选
	    	 sprintf(typ,"%s","2"); 
	    }else if(strcmp(typ0,"1")==0 && strcmp(typ1,"0")==0){//每天自动生成报告 被勾选
	    	 sprintf(typ,"%s","0");
	    }else if(strcmp(typ0,"0")==0 && strcmp(typ1,"1")==0){//每周自动生成报告 被勾选
	    	 sprintf(typ,"%s","1");
	    }
	    if(strlen(reporttype0)>0 && strlen(reporttype1)>0){
	    	sprintf(flowInfodata,"['%s','%s-%s','%s','%s','%s','%s','%s']",reportname,reporttype0,reporttype1,reportdir,fnames[len].caDirName,tempdir,typ,createDate); 
	    }else if(strlen(reporttype0)>0 && strlen(reporttype1)<=0){
	    	sprintf(flowInfodata,"['%s','%s','%s','%s','%s','%s','%s']",reportname,reporttype0,reportdir,fnames[len].caDirName,tempdir,typ,createDate); 
	    }else if(strlen(reporttype0)<=0 && strlen(reporttype1)>0){
	      sprintf(flowInfodata,"['%s','%s','%s','%s','%s','%s','%s']",reportname,reporttype1,reportdir,fnames[len].caDirName,tempdir,typ,createDate); 
			}else{//都没有勾选
				sprintf(reporttype,"%s","手动生成报告");
	   		memset(tempreporttype,0,sizeof(tempreporttype));
	   		strgb2utf8(reporttype,tempreporttype,strlen(reporttype));
	   		memset(reporttype,0,sizeof(reporttype));
	   		sprintf(reporttype,"%s",tempreporttype);
				sprintf(flowInfodata,"['%s','%s','%s','%s','%s','%s','%s']",reportname,reporttype,reportdir,fnames[len].caDirName,tempdir,typ,createDate); 
			}
			sprintf(flowInfodata,"%s,",flowInfodata);
			strcat(flowInfo,flowInfodata);  
	 } 
	 
	 if(len!=0){
		  flowInfo[strlen(flowInfo)-1]='\0'; 
	 }
          
   strcat(flowInfo,"]");
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
	 if(fnames){
 		free(fnames);
   }
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
 
  return 0;

}


/*
MSA:报告中心
 		查看该模板调用函数
*/
int reporttempledit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char caFile[256];  
  char temptypes0[256];
  char temptypes1[256];
  long temptype0;
  long temptype1;
  char element[50];
  char checkb[10];
  char templname[100];     
  char email[64];
  char language[4];
  char topusernum[128];
  
  ltDbHeadPtr dbPtr;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
 
	sprintf(caFile,"/datacenter/msa/report/page_data/%s/info.ini",ltMsgGetVar_s(ltMsgPk,"tempdir"));
	memset(element,0,sizeof(element)); 
	memset(checkb,0,sizeof(checkb));
	memset(element,'0',50);           
	memset(checkb,'0',10);
	memset(topusernum,0,sizeof(topusernum));
 
	bcCnfGetValue_s(caFile,"element",element); 
	bcCnfGetValue_s(caFile,"checkb",checkb); 
	bcCnfGetValue_s(caFile,"email",email);
	bcCnfGetValue_s(caFile,"templname",templname);
	bcCnfGetValue_s(caFile,"type0",temptypes0);
	bcCnfGetValue_s(caFile,"type1",temptypes1);
	bcCnfGetValue_s(caFile,"language",language);
	bcCnfGetValue_s(caFile,"topusernum",topusernum);
	temptype0=atol(temptypes0);
	temptype1=atol(temptypes1);
 
	if(temptype0==1){
		lt_dbput_rootvars(dbPtr,1,"se0","checked");
	}else{
		lt_dbput_rootvars(dbPtr,1,"se0","");
	}
  
	if(temptype1==1){
		lt_dbput_rootvars(dbPtr,1,"se1","checked");
	}else{
		lt_dbput_rootvars(dbPtr,1,"se1","");
	}

	if(checkb[0]=='1'){
		lt_dbput_rootvars(dbPtr,1,"b1","checked");
	}if(checkb[1]=='1'){
		lt_dbput_rootvars(dbPtr,1,"b2","checked");
	}if(checkb[2]=='1'){
		lt_dbput_rootvars(dbPtr,1,"b3","checked");
	}if(checkb[3]=='1'){
		lt_dbput_rootvars(dbPtr,1,"b4","checked");
	}if(checkb[4]=='1'){
		lt_dbput_rootvars(dbPtr,1,"b5","checked");
	} 
	
	if(element[0]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr1","checked");
	}if(element[1]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr2","checked");
	}if(element[2]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr3","checked");
	}if(element[3]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr4","checked");
	}if(element[4]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr5","checked");
	}if(element[5]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr6","checked");
	}if(element[6]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr7","checked");
	}if(element[7]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr8","checked");
	}if(element[8]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr9","checked");
	}if(element[9]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr10","checked");
	}if(element[10]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr11","checked");
	}if(element[11]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr12","checked");
	}if(element[12]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr13","checked");
	}if(element[13]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr14","checked");
	}if(element[14]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr15","checked");
	}if(element[15]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr16","checked");
	}if(element[16]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr17","checked");
	}if(element[17]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr18","checked");
	}if(element[18]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr19","checked");
	}if(element[19]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr20","checked");
	}if(element[20]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr21","checked");
	}if(element[21]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr22","checked");
	}if(element[22]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr23","checked");
	}if(element[23]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr24","checked");
	}if(element[24]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr25","checked");
	}if(element[25]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr26","checked");
	}if(element[26]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr27","checked");
	}if(element[27]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr28","checked");
	}if(element[28]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr29","checked");
	}if(element[29]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr30","checked");
	}if(element[30]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr31","checked");
	}if(element[31]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr32","checked");
	}if(element[32]=='1'){
		lt_dbput_rootvars(dbPtr,1,"chr33","checked");
	}
	
	if(atoi(language)==0){
	    lt_dbput_rootvars(dbPtr,1,"san1","selected");
	}if(atoi(language)==1){
	    lt_dbput_rootvars(dbPtr,1,"san2","selected");
	}

  lt_dbput_rootvars(dbPtr,4,"topusernum",topusernum,"templname",templname,"email",email,"tempid",ltMsgGetVar_s(ltMsgPk,"tempdir"));
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/report/reportstylesettingedit.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk); 

  return 0;
 
}



/*
MSA:报告中心
		模板更新函数
*/ 
int reporttemplateupdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	  char caLabel[256]; 
	  char reportname[40];
	  char element[50];
	  char checkb[10];
	  int  deployscope0;
	  int  deployscope1;
	  char *templname=NULL;     
	  char caTempDir[123];
	  char *tempindex;
	  char temptype0[256];
	  char temptype1[256];
	  char temptype[256];
	  char tempdir[64];
	  char email[64];
	  char language[4];
  	char topusernum[4];
	  char createDate[36];
	  long lTime;
	
	  if(ltMsgGetVar_s(ltMsgPk,"tempid")){
	     tempindex=ltMsgGetVar_s(ltMsgPk,"tempid");
	  }else{
	  	 tempindex="";
	  }
	  
	  
	  if( ltMsgGetVar_s(ltMsgPk,"templname")==NULL){
	      sprintf(templname,"%s","");
	  }else{
	      templname=ltMsgGetVar_s(ltMsgPk,"templname");
	  } 
	 
	  
	  if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
			   sprintf(reportname,"%s","");
	  }else{
	       sprintf(reportname,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
	  } 
	 
	
	  memset(element,'0',50);            
	  memset(checkb,'0',10);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"element")){
	    sprintf(element,ltMsgGetVar_s(ltMsgPk,"element"));
	  }else{
	  	sprintf(element,"%s","");
	  }
	  
	  if(ltMsgGetVar_s(ltMsgPk,"checkb")){
	  	sprintf(checkb,ltMsgGetVar_s(ltMsgPk,"checkb"));
	  }else{
	  	sprintf(checkb,"%s","");
	  }
	  
	  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
	    sprintf(email,"%s",""); 
	  }else{
	    sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
	  }
	 
		 memset(language,0,sizeof(language));
		 if(ltMsgGetVar_s(ltMsgPk,"language")){
		 	 sprintf(language,"%s",ltMsgGetVar_s(ltMsgPk,"language"));
		 }else{
		 	 sprintf(language,"%s","");
		 }
	
		 memset(topusernum,0,sizeof(topusernum));
		 if(ltMsgGetVar_s(ltMsgPk,"topusernum")){
		 	 sprintf(topusernum,"%s",ltMsgGetVar_s(ltMsgPk,"topusernum"));
		 }else{
		 	 sprintf(topusernum,"%s","");
		 }
	 
		 if(ltMsgGetVar_s(ltMsgPk,"deployscope0")){
		 	 deployscope0=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope0"));
		 }else{
		 	 deployscope0=0;
		 }
		 
		 if(ltMsgGetVar_s(ltMsgPk,"deployscope1")){
		   deployscope1=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope1"));
		 }else{
		 	 deployscope1=0;
		 }
		 
	   lTime = time(0);
		 nasTimeGetDate(createDate,lTime);
	
	   sprintf(caTempDir,"/datacenter/msa/report/page_data/%s/",tempindex); 
	   if(deployscope0==1){ 
		   	sprintf(temptype0,"%s","每天自动生成报告");
		    memset(temptype,0,sizeof(temptype));
		   	strgb2utf8(temptype0,temptype,strlen(temptype0));
		   	sprintf(temptype0,"%s",temptype);
		   	sprintf(tempdir,"dailyreport");
	 	 }else{
	   		sprintf(temptype0,"%s","");
	 	 }
	 	 
	 	 if(deployscope1==1){
	   		sprintf(temptype1,"%s","每周自动生成报告");
		    memset(temptype,0,sizeof(temptype));
		   	strgb2utf8(temptype1,temptype,strlen(temptype1));
		   	sprintf(temptype1,"%s",temptype);
		   	sprintf(tempdir,"dailygrpreport");
	 	 }else{
	   		sprintf(temptype1,"%s","");
	 	 }
	 	 
	 	 if(deployscope0==0 && deployscope1==0){//两个都未勾选
	 	 		sprintf(tempdir,"instantreport");
	 	 }
	    
	   if(ltFileIsExist(caTempDir)!=1) {
	       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {/// 生成该报告目录错误         
	        printf("mkdir daily report dir error!\n");
	        return 0;
	       }
	   }  
 
	  sprintf(caLabel,"echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
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
	  
	  if(deployscope0==1){//表示勾选
		  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",1,caTempDir);
		}else{
		  sprintf(caLabel,"echo 'type0=%d' >> %sinfo.ini",0,caTempDir);
		}
	  system(caLabel);
		
	  if(deployscope1==1){//表示勾选
		  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",1,caTempDir);
		}else{
		  sprintf(caLabel,"echo 'type1=%d' >> %sinfo.ini",0,caTempDir);
		}
		system(caLabel);
		
	  sprintf(caLabel,"echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"echo 'email=%s' >> %sinfo.ini",email,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"echo 'language=%s' >> %sinfo.ini",language,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
	  system(caLabel);
	 
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
	  ltMsgFree(ltMsgPk);
	 
	  
	  return 0;
 
}



/*浏览该模板的报告*/
int templatereportlista(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		ltDbHeadPtr dbPtr;
		ltTablePtr tablePtr;
		msasDir *fnames;
		char caFile1[256];
	  char cDate[128];
		char reportname[64];
		char reportuser[64];
    struct stat st;
    char   *reportdir;
    int    len=0; 

		
		reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir"); 
		printf("reportdir:%s\n",reportdir);
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
//		tablePtr=lt_dbput_table(dbPtr,"list");
//		sprintf(caFile1,"/datacenter/msa/report/%s/",reportdir);
//		fnames=mydirlist(caFile1,&len);
//
//	  if(fnames){
//	 	 qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
//	  }
//
//
////   while(len--){
////						memset(reportname,0,sizeof(reportname));
////					  memset(reportuser,0,sizeof(reportuser));
////					  memset(cDate,0,sizeof(cDate));
////						sprintf(caFile1,"/datacenter/msa/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
////				    bcCnfGetValue_s(caFile1,"date",cDate);
////				    bcCnfGetValue_s(caFile1,"name",reportname);
////				    bcCnfGetValue_s(caFile1,"user",reportuser);
////						sprintf(caFile1,"/datacenter/msa/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
////				    if(stat(caFile1,&st) == 0){
////				    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
////				    	lt_dbput_recordvars(tablePtr,4,
////							"reportname",LT_TYPE_STRING,reportname,
////							"reportstate",LT_TYPE_STRING,caFile1,
////							"bfile",LT_TYPE_STRING,fnames[len].caDirName,
////							"reportdir",LT_TYPE_STRING,reportdir);
////							continue;
////				     }
////
////				    lt_dbput_recordvars(tablePtr,4,
////							"reportname",LT_TYPE_STRING,reportname,
////							"reportstate",LT_TYPE_STRING,cDate,
////							"bfile",LT_TYPE_STRING,fnames[len].caDirName,
////							"reportdir",LT_TYPE_STRING,reportdir
////						);
////   }  

   lt_dbput_rootvars(dbPtr,1,"reportdirs",reportdir); 
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/report/templatereportlist.htm",dbPtr->head,0);  
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);

   return 0;

}



//某模板下的所有报告
int templatereportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
	printf("templatereportlist........\n");
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr; 
	char caFile1[256];
 	char cDate[128];
  char flowInfo[30000],flowInfodata[3000];
	char reportname[64];
	char reportuser[64];char reportstate[123];
  struct stat st;
  char    *reportdir;
  int     len=0;
  msasDir *fnames;
  char    *filedir;
  char    caCmd[256];
 
  filedir=ltMsgGetVar_s(ltMsgPk,"file");
  reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir"); 
	if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){//delete
		filedir=ltMsgGetVar_s(ltMsgPk,"file");
		sprintf(caCmd,"rm -Rf /datacenter/msa/report/%s/%s",reportdir,filedir);
    system(caCmd);
 
	}
 

	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");

	 tablePtr=lt_dbput_table(dbPtr,"list");
	 sprintf(caFile1,"/datacenter/msa/report/%s/",reportdir);
   fnames=mydirlist(caFile1,&len);

   if(fnames){
 	    qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
   }
   sprintf(flowInfo,"[ ");         
   while(len--){
		       	printf("%s\n",fnames[len].caDirName);
   				  memset(reportname,0,sizeof(reportname));
				    memset(reportuser,0,sizeof(reportuser));
				    memset(cDate,0,sizeof(cDate));
						sprintf(caFile1,"/datacenter/msa/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
				    bcCnfGetValue_s(caFile1,"date",cDate);
				    bcCnfGetValue_s(caFile1,"name",reportname);
				    bcCnfGetValue_s(caFile1,"user",reportuser);
						sprintf(reportstate,"0");
						sprintf(caFile1,"/datacenter/msa/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
				    if(stat(caFile1,&st) == 0){
				    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
						  sprintf(reportstate,caFile1);
				    }
 

						sprintf(caFile1,"/datacenter/msa/report/%s/%s/proccess.ini",reportdir,fnames[len].caDirName);
				    if(stat(caFile1,&st) == 0){
							sprintf(reportstate,"1");
				    }


//				    lt_dbput_recordvars(tablePtr,4,
//							"reportname",LT_TYPE_STRING,reportname,
//							"reportstate",LT_TYPE_STRING,cDate,
//							"bfile",LT_TYPE_STRING,fnames[len].caDirName,
//							"reportdir",LT_TYPE_STRING,reportdir
//						);
            sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,reportdir,fnames[len].caDirName,reportstate);
					  sprintf(flowInfodata,"%s,",flowInfodata);
					  strcat(flowInfo,flowInfodata);
  } 

	if(len!=0){
	  flowInfo[strlen(flowInfo)-1]='\0'; 
	}
  strcat(flowInfo,"]"); 

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
	if(fnames){ 
	 free(fnames); 
  }
 
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
	printf("function end ....\n");
 
	return 0;

}



//报告列表
int toautoreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		char caFile1[256];
	 	char cDate[128];
	  char flowInfo[30000],flowInfodata[3000];
		char reportname[64];
		char reportuser[64];
		char reportstate[123];
		char tempreportstate[123];
    struct  stat st;
    char    *reportdir;
    int 		len=0,i,j=0;
    msasDir *fnames=NULL;
    msasDir *fnames2=NULL;
    char   lastDir[25];
    char   *filedir;
    char   caCmd[256];


   reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
   printf("reportdir:%s\n",reportdir);
   filedir=ltMsgGetVar_s(ltMsgPk,"file");
   if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){
	    filedir=ltMsgGetVar_s(ltMsgPk,"file");
	    sprintf(caCmd,"rm -Rf /datacenter/msa/report/%s/%s",ltMsgGetVar_s(ltMsgPk,"reportdird"),filedir);
      system(caCmd);
      sleep(1);
   }

	 sprintf(caFile1,"/datacenter/msa/report/%s/",reportdir);
	 fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	   qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	 }


   memset(flowInfo,0,sizeof(flowInfo));
   memset(tempreportstate,0,sizeof(tempreportstate));
   sprintf(flowInfo,"[ "); 
   for(i=0;i<len;i++) { 
	   sprintf(caFile1,"/datacenter/msa/report/%s/%s/",reportdir,fnames[i].caDirName);
	   sprintf(lastDir,"%s",fnames[i].caDirName); 
	   fnames2=mydirlist(caFile1,&j); 
	   if(fnames){
	 	   qsort(fnames2,j,sizeof(msasDir),dirtimecmp); 
	   }
	
	   while(j--){
							memset(reportname,0,sizeof(reportname));
						  memset(reportuser,0,sizeof(reportuser));
						  memset(cDate,0,sizeof(cDate));			
						  sprintf(caFile1,"/datacenter/msa/report/%s/%s/%s/info.ini",reportdir,lastDir,fnames2[j].caDirName);
						  printf("caFile1:%s\n",caFile1);
						  bcCnfGetValue_s(caFile1,"date",cDate);
						  bcCnfGetValue_s(caFile1,"name",reportname);
						  bcCnfGetValue_s(caFile1,"user",reportuser);
						  sprintf(reportstate,"报告已生成");
						  strgb2utf8(reportstate,tempreportstate,sizeof(reportstate));
						  memset(reportstate,0,sizeof(reportstate));
						  sprintf(reportstate,"%s",tempreportstate);
							sprintf(caFile1,"/datacenter/msa/report/%s/%s%s/errmsg.ini",reportdir,lastDir,fnames2[j].caDirName);
						  if(stat(caFile1,&st) == 0){
						    	sprintf(caFile1,"<a href=\"/report/%s/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,lastDir,fnames2[j].caDirName);
						    	sprintf(reportstate,caFile1);
						  }
						  sprintf(caFile1,"/datacenter/msa/report/%s/%s/%s/proccess.ini",reportdir,lastDir,fnames2[j].caDirName);
						  if(stat(caFile1,&st) == 0){		
						  	  memset(reportstate,0,sizeof(reportstate));			    	 
									sprintf(reportstate,"报告正在生成中");
									strgb2utf8(reportstate,tempreportstate,sizeof(reportstate));
						      memset(reportstate,0,sizeof(reportstate));
						      sprintf(reportstate,"%s",tempreportstate);
						  }
	            sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,lastDir,fnames2[j].caDirName,reportstate);
							sprintf(flowInfodata,"%s,",flowInfodata);
							strcat(flowInfo,flowInfodata);
	   }
   }
	 if(len!=0){
		 flowInfo[strlen(flowInfo)-1]='\0'; 
	 }
	 strcat(flowInfo,"]"); 


	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);

	  if(fnames){
	 		 free(fnames);
    }
    if(fnames2){
			 free(fnames2);
    }
   
	 ltMsgFree(ltMsgPk);

	 return 0;

}



/*每天汇总的报告列表*/
int toreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	char caFile1[256];
 	char cDate[128];
  char flowInfo[30000],flowInfodata[3000];
	char reportname[64];
	char reportuser[64];char reportstate[123];
  struct stat st;
  char   *reportdir;
	int    len=0; //sum of file names 
	msasDir *fnames;
	char   *filedir;
	char   caCmd[256];

 
   reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
   if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){
		filedir=ltMsgGetVar_s(ltMsgPk,"file");
		sprintf(caCmd,"rm -Rf /datacenter/msa/report/%s/%s",reportdir,filedir);
        system(caCmd);
	 }
 

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"list");
	sprintf(caFile1,"/datacenter/msa/report/%s/",reportdir);
  fnames=mydirlist(caFile1,&len);
  if(fnames){
	    qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
  }
  sprintf(flowInfo,"[ ");
  while(len--) {
				  memset(reportname,0,sizeof(reportname));
			    memset(reportuser,0,sizeof(reportuser));
			    memset(cDate,0,sizeof(cDate));
				  sprintf(caFile1,"/datacenter/msa/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
			    bcCnfGetValue_s(caFile1,"date",cDate);
			    bcCnfGetValue_s(caFile1,"name",reportname);
			    bcCnfGetValue_s(caFile1,"user",reportuser);
				  sprintf(reportstate,"报告已生成");
				  sprintf(caFile1,"/datacenter/msa/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
			    if(stat(caFile1,&st) == 0){
			    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
					  sprintf(reportstate,caFile1);
			    }  

			    sprintf(caFile1,"/datacenter/msa/report/%s/%s/proccess.ini",reportdir,fnames[len].caDirName);
			    if(stat(caFile1,&st) == 0){
						sprintf(reportstate,"报告正在生成中");
			    }

			    lt_dbput_recordvars(tablePtr,4,
						"reportname",LT_TYPE_STRING,reportname,
						"reportstate",LT_TYPE_STRING,cDate,
						"bfile",LT_TYPE_STRING,fnames[len].caDirName,
						"reportdir",LT_TYPE_STRING,reportdir);
          sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,reportdir,fnames[len].caDirName,reportstate);               
				  sprintf(flowInfodata,"%s,",flowInfodata);
			  	strcat(flowInfo,flowInfodata);
 
  }

	if(len!=0){
	  flowInfo[strlen(flowInfo)-1]='\0'; 
	}
  strcat(flowInfo,"]");
  
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
 if(fnames){
 	free(fnames);
 }
 
 ltMsgFree(ltMsgPk);
 lt_dbfree(dbPtr);
 
 return 0;
 
}


/*已定义报告模板 立即生成报告 生成报告 返回视图页面*/
int toreportcreate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	  char *tempIndex; 
    ltDbHeadPtr dbPtr; 
    dbPtr=lt_dbinit(); 
    lt_db_htmlpage(dbPtr,"utf-8");    
	  tempIndex=ltMsgGetVar_s(ltMsgPk,"tempindex"); 
    lt_dbput_rootvars(dbPtr,1,"tempindex",tempIndex);  
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/reportcreat.htm",dbPtr->head,0);  
    ltMsgFree(ltMsgPk); 
    lt_dbfree(dbPtr); 
    
    return 0;
}





/*已定义报告模板 立即生成报告*/
int reportcreate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char caLabel[2028];
  char caSdates[63]; 
  char caEdates[63]; 
	char reportname[40];
	char createDate[63];
	char caTempDir[123];
	char tempindex[64];//模板目录
	char *type;
  char **binqqargs;
  int  iPid;//子进程标记
	ltDbHeadPtr dbPtr;
	long lTime;
	struct stat st;
  char caFile1[256];
  char reportstate[123];
  char repname[1024];
  msasDir *fnames;
  char *groupstr;
  char *worktime;
  int  len=0;
  int  iFlag=-1;
 
  dbPtr=lt_dbinit();//页面初始化
  lt_db_htmlpage(dbPtr,"utf-8");
	lTime=time(0);
	nasTimeGetDate(createDate,lTime);//获取当前日期
	memset(caLabel,0,sizeof(caLabel));
	
	if(ltMsgGetVar_s(ltMsgPk,"st")){
		iFlag=atoi(ltMsgGetVar_s(ltMsgPk,"st"));
	}
  printf("iFlag:%d\n",iFlag);
	
 	if(iFlag==0){ 
		    if(ltMsgGetVar_s(ltMsgPk,"sdate")==NULL || ltMsgGetVar_s(ltMsgPk,"edate")==NULL){//判断时间是否为空
					sprintf(caSdates,"%s",createDate);//存入当前时间
		      sprintf(caEdates,"%s",createDate);
		    }else{
		    	sprintf(caSdates,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));//获取用户所选时间
					sprintf(caEdates,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
		    } 
	
		    if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){//获取报告的名字
					sprintf(reportname,"%s","");
		    }else{
		    	sprintf(reportname,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
		    } 

				if(ltMsgGetVar_s(ltMsgPk,"tempindex")){//获取目录
					sprintf(tempindex,"%s",ltMsgGetVar_s(ltMsgPk,"tempindex"));
		    }else{
		    	sprintf(tempindex,"%s","");
		    } 
		    
		    if(ltMsgGetVar_s(ltMsgPk,"type")){//获取type参数
		    	 type=ltMsgGetVar_s(ltMsgPk,"type"); 
		    }else{
		    	 type=""; 
		    }
		    
		    if(ltMsgGetVar_s(ltMsgPk,"worktime")){//工作段
		    	 worktime=ltMsgGetVar_s(ltMsgPk,"worktime"); 
		    }else{
		    	 worktime=""; 
		    }
		    
		    if(ltMsgGetVar_s(ltMsgPk,"groupstr")){//部门字符串
		    	 groupstr=ltMsgGetVar_s(ltMsgPk,"groupstr"); 
		    }else{
		    	 groupstr=""; 
		    }
		    
		    
		    sprintf(caTempDir,"/datacenter/msa/report/page_data/%s/",tempindex);
		    sprintf(caLabel,"echo 'sdate=%s' > %ssdate",caSdates,caTempDir);
		    system(caLabel); 
		    sprintf(caLabel,"echo 'edate=%s' > %sedate",caEdates,caTempDir);
		    system(caLabel); 
		    sprintf(caLabel,"echo 'rpname=%s' > %srpname",reportname,caTempDir);
		    system(caLabel);  
		    sprintf(caLabel,"echo 'work=%s' > %swork",worktime,caTempDir);
		    system(caLabel);   
		    sprintf(caLabel,"echo 'group=%s' > %sgroup",groupstr,caTempDir);
		    system(caLabel);
 
		    
			  chdir("/app/msa/bin"); //转到程序执行目录
			  iPid=fork(); 		 
				if(iPid==0){ 
						binqqargs = (char **) malloc(2 * sizeof(char *));
						binqqargs[0]= "dailyreporttemplate";
						binqqargs[1]=type;
						binqqargs[2]=tempindex;//模板目录
						binqqargs[3]=0;
						execv(*binqqargs,binqqargs);  
						die(0);
				}
				wait(NULL);   
				char flowInfo[1000];
				sprintf(flowInfo,"%s","{success:true}");	
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\r\n","lthtml",LT_TYPE_STRING,flowInfo);		 
   }else{
			 sprintf(tempindex,"%s",ltMsgGetVar_s(ltMsgPk,"tempindex"));	  
			 sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/",tempindex);
	   	 fnames=mydirlist(caFile1,&len);
		   if(fnames){
			    qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
		   }
		 	 while(len--){    
		      memset(repname,0,sizeof(repname));
			    memset(reportstate,0,sizeof(reportstate));
		      sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/%s/info.ini",tempindex,fnames[len].caDirName);
		      bcCnfGetValue_s(caFile1,"name",repname);
				  if(strcmp(ltMsgGetVar_s(ltMsgPk,"reportname"),repname)==0){
			         sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/%s/proccess.ini",tempindex,fnames[len].caDirName);
					     if(stat(caFile1,&st) == 0){    
						 	    sprintf(reportstate,"%s","0");//处理中
					     }else{
							    sprintf(reportstate,"%s","1");//处理完毕
						   }
				       break;
				  }
			 }
			 	 	
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\r\n","lthtml",LT_TYPE_STRING,reportstate);

     }
    
		ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);   
    printf("reportcreate end .........\n");   
    
    return 0;
} 



int reporttempstatus(char *tempindex,char *reportname,ltMsgHead *ltMsgPk,int confd)
{ 
    struct stat st;
    char caFile1[256];
    char reportstate[123],repname[1024];
    msasDir *fnames;
    int len=0;
	  
   sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/",tempindex);
   printf("repname:%s,caFile1:: %s\n",reportname,caFile1);   
   fnames=mydirlist(caFile1,&len);
   if(fnames){
	    qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
   }
   while(len--){    
		  memset(repname,0,sizeof(repname));
			memset(reportstate,0,sizeof(reportstate));
      sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/%s/info.ini",tempindex,fnames[len].caDirName);     
      bcCnfGetValue_s(caFile1,"name",repname);
		  if(strcmp(reportname,repname)==0){
	         sprintf(caFile1,"/datacenter/msa/report/instantreport/%s/%s/proccess.ini",tempindex,fnames[len].caDirName);
		       if(stat(caFile1,&st) == 0){    
			 			sprintf(reportstate,"%s","0"); 
            printf("==============proceeing\n");
		     	 }else{
				 		sprintf(reportstate,"%s","1");
            printf("==============over\n");
			 		 }
		  		break;
		  }
	 }

	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\r\n","lthtml",LT_TYPE_STRING,reportstate);
	 printf("reporttempstatus end.........\n");
		
	 return 0;
} 



