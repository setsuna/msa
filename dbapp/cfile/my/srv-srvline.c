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

/*******************报告目录相关宏*******************************/
#ifndef __SRVLINREPORT_DIR__
#define __SRVLINREPORT_DIR__
#define srvreportdatacenterdir   "/datacenter/msa/report/srvlinereport"
#define srvlineplt               "/app/msa/msa/htmlplt/reportplt/srvlineplt"
#endif

//服务主线分析AJAX提交
int msaReportSrvLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
	  printf("msaReportSrvLineAjaxSubmit.................................\n");
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    
    char        sqlBuf[1024];    
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char        *srvreportname=NULL;
		char        *srvemail=NULL;
		char        *step=NULL;
		char        *groupid=NULL;
		long        oldGid,newGid,uid=-1;
		char        *lt_page_content=NULL;
		char        caTempDir[256];  
		char        responseDir[256];
    char        caLabel[256];
    char        strdayflow[32];
    unsigned long long lMaxBytes;
    unsigned long tmpDir=0;
    unsigned long fileName=0;
    char        sDate[64],sTime[64];
	  char        caFile1[256];   
    char        caCmd[256];
    int         fdwrite=0;
		
    long        lTime,lTime1,lTime2; 
    int         iFd=-1;
    int         srvindex; 
    int         gindex;
    char        oldDate[64];
    char        newDate[64];
    char        srvName[255];
    memset(oldDate,0,sizeof(oldDate));
    memset(newDate,0,sizeof(newDate));
    memset(srvName,0,sizeof(srvName));
    
    //连接数据库
  	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon!=NULL){
			printf("db connect ok\n");
		}else{
			fprintf(stderr,"db connect error\n");
		}
    
    
	  /***************************ajax参数********************/
		if(ltMsgGetVar_s(ltMsgPk,"srvsdate")){
	  	sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"srvsdate"));
	  }else{
	  	sprintf(caSdate,"%s","");
	  }
	  
	  printf("caSdate:%s\n",caSdate);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"srvedate")){
	  	 sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"srvedate"));
	  }else{
	  	 sprintf(caEdate,"%s","");
	  }
		
		printf("caEdate:%s\n",caEdate);
		
		if(ltMsgGetVar_s(ltMsgPk,"srvreportname")){		
	  	srvreportname=ltMsgGetVar_s(ltMsgPk,"srvreportname");
	  }
	  
	  printf("srvreportname:%s\n",srvreportname);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"srvemail")){
	  	srvemail=ltMsgGetVar_s(ltMsgPk,"srvemail");
	  }
	  
	  printf("srvemail:%s\n",srvemail);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"step")){
	  	step=ltMsgGetVar_s(ltMsgPk,"step");
	  }
    
    printf("step:%s\n",step);
    
    
    if(ltMsgGetVar_s(ltMsgPk,"gid")){
	  	groupid=ltMsgGetVar_s(ltMsgPk,"gid");
	  }
	   printf("groupid:%s\n",groupid);
	  if(ltMsgGetVar_s(ltMsgPk,"uid")){
	  	uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	  }
	   printf("uid:%ld\n",uid);
	   
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){	  
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }else{
	  	fileName=0;
	  }
    printf("fileName:%lu\n",fileName);
    
    
    /****************目录生成************************/
    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s/%lu/",srvreportdatacenterdir,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);	  
		    if(ltFileIsExist(caTempDir)!=1) {
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				      ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
							ltMsgFree(ltMsgPk);
			        return 0;
		       }
		 		}  
    }else{//从第二步开始传进第一步生成的目录
    	  sprintf(caTempDir,"%s/%lu/",srvreportdatacenterdir,fileName);   
    	  sprintf(responseDir,"%lu",fileName);
    }    

    printf("caTempDir:%s\n",caTempDir);

    /***********************日期处理*****************/
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        lTime1 = 0;
    } 
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        lTime2 = 0;
    } 
    
    
    if(lTime1 == 0) {
         lTime1 = lTime;       
    }
	  if(lTime2 == 0) {
         lTime2 = lTime;
    }
	  if(lTime1 > lTime) {
         lTime1 =lTime;
    }
    if(lTime2>lTime){
		 		 lTime2=lTime;
	  }
	 
	  
	  
	  /******************************拷贝模板报告文件*************************/
	  if(atoi(step)==1){
			  nasCvtStime(time(0),sDate,sTime); 		  
			  sprintf(caFile1,"%sinfo.ini",caTempDir);     
			  fdwrite = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
				if(fdwrite == (-1)) {
					 ltMsgPk->msgpktype=1;
					 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					 ltMsgFree(ltMsgPk);
					 return 0;
				}
				
				sprintf(caCmd,"date=%s_%s\n",sDate,sTime);
			  write(fdwrite,caCmd,strlen(caCmd));
			  sprintf(caCmd,"reportname=%s\n",srvreportname);
			  write(fdwrite,caCmd,strlen(caCmd));
			  close(fdwrite);			  
			  
			  sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/right-top-d.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/right-top.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/banner.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/up-1.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/up-2.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
				sprintf(caLabel,"/bin/cp %s/up-3.jpg  %s",srvlineplt,caTempDir);
			  system(caLabel);
    
   }
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		      
    /*************************数据库操作***************************************/  
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);

    if(atoi(step)==1){//服务流量报告
      sprintf(sqlBuf,"select service,sum(ubytes+dbytes) as abyte ,sum(ctime) as arequest\
        from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'\
        group by service order by abyte desc ",caSdate1,caSdate2); 
    }else if(atoi(step)==2){//部门汇总报告	      	      	  
    sprintf(sqlBuf,"select a.sdate as xdate,service,b.groupid0,sum(ubytes+dbytes) as abyte ,sum(ctime) as arequest from msasrvreport a,msauser b\
     where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'  and a.userid=b.userid  group by a.sdate,service,b.groupid0\
     order by xdate asc,service,b.groupid0,abyte desc",caSdate1,caSdate2);
    }else if(atoi(step)==3){//时间服务汇总
	      sprintf(sqlBuf,"select sdate,service,sum(ubytes+dbytes) as abyte ,sum(ctime) as arequest from msasrvreport a,msauser b\
		   		 where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid\
		   		 group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2);
    }else{//部门时间汇总
		  	 sprintf(sqlBuf,"select a.sdate as xdate,service,c.id ,sum(ubytes+dbytes) as abyte ,sum(ctime) as arequest\
		  	 from msasrvreport a,msauser b,msagroup c\
		  	 where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id \
		  	 group by a.sdate,service,c.id order by xdate asc,service,c.id,abyte desc ",caSdate1,caSdate2);    	
    }
    
	  printf("sqly: %s\n",sqlBuf);
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }
		printf("33333333333333333333333333333333333333333333\n");    	   
	  
	  /************图片数据存储文件****************/
	  if(atoi(step)==1){
		  	sprintf(caLabel,"%sdata.ini",caTempDir);
	      iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		    if(iFd == (-1)){
						sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
			    	system(caLabel);/*建立目录和正在处理标志*/
			      ltDbCloseCursor(tempCursor);
			      ltMsgFree(ltMsgPk);
			      return 0;
				}							
	  }
	  printf("2222222222222222222222222222222222222222222\n");    	   
	  
	  /**********************将数据显示的HTML页面*********************************/
	  srvindex=1;
	  lMaxBytes=1;
	  oldGid=-1;
	  newGid=-1;
	  gindex=0;
	  dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    tablePtr=lt_dbput_table(dbPtr,"list"); 
	  tempRow= ltDbFetchRow(tempCursor);   
	  while(tempRow!=NULL){	  
	  	    printf("11111111111111111111111111111111111111111111\n");    	                         	  
					if(atoi(step)==1){
					  if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }
					  
					  
					  sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[1]));
	          write(iFd, caLabel, strlen(caLabel));	
	          if(lMaxBytes<atoll(tempRow[1])){
	 		     		 lMaxBytes=atoll(tempRow[1]);
	          }		
				  	sprintf(srvName,"No%d:%s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname);				  
						lt_dbput_recordvars(tablePtr,3,
							"srvname",LT_TYPE_STRING,srvName,  
			   			"srvljnum",LT_TYPE_STRING,tempRow[2],
			   			"srvljflow",LT_TYPE_STRING,strdayflow
		   		  ); 
		   		  
					}else if(atoi(step)==2){			
							printf("444444444444444444444444444444444444444444444444\n");    	   			
							newGid=atol(tempRow[2]);	
							printf("newGid:%ld\n",newGid);
							if(atoll(tempRow[3])>1024000){
						      sprintf(strdayflow,"%0.3fM",atoll(tempRow[3])/1024000.00);
						  }else{
						      sprintf(strdayflow,"%0.3fK",atoll(tempRow[3])/1024.00);
						  }
							if(newGid==oldGid){							  
					  	  	lt_dbput_recordvars(tablePtr,4,
									  "srvgname",LT_TYPE_STRING,"",
										"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
						   			"srvljnum",LT_TYPE_STRING,tempRow[4],
						   			"srvljflow",LT_TYPE_STRING,strdayflow
				   		    );
				  	  }else{
				  	  	 printf("groupid[gindex]:%c\n",groupid[gindex]);							 
						  	 if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
						  	 	  lt_dbput_recordvars(tablePtr,4,
									  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
										"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
						   			"srvljnum",LT_TYPE_STRING,tempRow[4],
						   			"srvljflow",LT_TYPE_STRING,strdayflow
				   		      );  
						  	 
						  	 }
					  	  	
				  	  }												
							oldGid=atol(tempRow[2]);						
							printf("oldGid:%ld\n",oldGid);
							gindex++;
							int i;
							for(i=0;i<100;i++){
								printf("_ltPubInfo->_GroupList[%d].groupid:%ld\n",i,_ltPubInfo->_GroupList[i].groupid);
							}
							
							if(gindex>=1000)	{
							   break;			
						  }
						  
						  
						
					}else if(atoi(step)==3){//第三步
						  if(atoll(tempRow[2])>1024000){
						      sprintf(strdayflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
						  }else{
						      sprintf(strdayflow,"%0.3fK",atoll(tempRow[2])/1024.00);
						  }
							sprintf(newDate,"%s",tempRow[0]);
							if(strcmp(newDate,oldDate)==0){
								lt_dbput_recordvars(tablePtr,4,
								  "srvdate",LT_TYPE_STRING,"",
									"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
					   			"srvljnum",LT_TYPE_STRING,tempRow[3],
					   			"srvljflow",LT_TYPE_STRING,strdayflow
			   		    );
							}else{
								lt_dbput_recordvars(tablePtr,4,
								  "srvdate",LT_TYPE_STRING,newDate,
									"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
					   			"srvljnum",LT_TYPE_STRING,tempRow[3],
					   			"srvljflow",LT_TYPE_STRING,strdayflow
			   		    );
							}
								  
			   		  sprintf(oldDate,"%s",tempRow[0]);
					}else{//第四步
							 sprintf(newDate,"%s",tempRow[0]);
							 newGid=atol(tempRow[2]);
							 if(atoll(tempRow[3])>1024000){
						      sprintf(strdayflow,"%0.3fM",atoll(tempRow[3])/1024000.00);
						   }else{
						      sprintf(strdayflow,"%0.3fK",atoll(tempRow[3])/1024.00);
						   }
							 if(strcmp(newDate,oldDate)==0){			    
								  if(newGid==oldGid){							  
							  	  	lt_dbput_recordvars(tablePtr,5,
							  	  	  "srvdate",LT_TYPE_STRING,"",
											  "srvgname",LT_TYPE_STRING,"",
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljnum",LT_TYPE_STRING,tempRow[4],
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		    );
						  	  }else{			
								  	 if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
								  	 	  lt_dbput_recordvars(tablePtr,5,
								  	 	  "srvdate",LT_TYPE_STRING,"",
											  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljnum",LT_TYPE_STRING,tempRow[4],
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		      );  
								  	 
								  	 }	  	
						  	  }												
									oldGid=atol(tempRow[2]);
									if(gindex>=1000)	{
									   break;			
								  }else{
								  	 gindex++;
								  }
							 }else{
							    if(newGid==oldGid){							  
							  	  	lt_dbput_recordvars(tablePtr,5,
							  	  	  "srvdate",LT_TYPE_STRING,newDate,
											  "srvgname",LT_TYPE_STRING,"",
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljnum",LT_TYPE_STRING,tempRow[4],
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		    );
						  	  }else{			
								  	 if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
								  	 	  lt_dbput_recordvars(tablePtr,5,
								  	 	  "srvdate",LT_TYPE_STRING,newDate,
											  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljnum",LT_TYPE_STRING,tempRow[4],
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		      );  
								  	 
								  	 }	  	
						  	  }												
									oldGid=atol(tempRow[2]);
									if(gindex>=1000){
									   break;			
								  }else{
								  	 gindex++;
								  }
							  
							  
							}
									
	             sprintf(oldDate,"%s",tempRow[0]);
					}

        
	   			if(atoi(step)==1){
	          	srvindex++;
	        }
   			
        tempRow=ltDbFetchRow(tempCursor);//移动光标
    }
    ltDbCloseCursor(tempCursor);

    if(atoi(step)==1){//画图
			  close(iFd);	 
			  sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %ssrvreport.gif %llu ",caTempDir,caTempDir,lMaxBytes);  //柱状 饼图：onlinesrv1.pl
			  system(caLabel);	
			  lt_dbput_rootvars(dbPtr,1,"piechart","srvreport.gif" );
	  } 

	  
	  if(atoi(step)==1){
	  	lt_dbput_rootvars(dbPtr,3,"reportname",srvreportname,"bdate",caSdate1,"edate",caSdate2);
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/srvlineplt/index.htm",dbPtr->doc,0);
	  }else if(atoi(step)==2){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/srvlineplt/srvstep2.htm",dbPtr->doc,0);
	  }else if(atoi(step)==3){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/srvlineplt/srvstep3.htm",dbPtr->doc,0);
	  }else if(atoi(step)==4){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/srvlineplt/srvstep4.htm",dbPtr->doc,0);
	  }

	  if(lt_page_content==NULL){
		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	  		system(caLabel);/*建立目录和正在处理标志*/
	  		ltMsgFree(ltMsgPk);
	  		lt_dbfree(dbPtr);
     		return 0;
		}else{
			  lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
			  if(atoi(step)==1){
			  	sprintf(caLabel,"%sindex.htm",caTempDir);
			  }else if(atoi(step)==2){
			  	sprintf(caLabel,"%spage2.htm",caTempDir);
			  }else if(atoi(step)==3){
			  	sprintf(caLabel,"%spage3.htm",caTempDir);
			  }else{
			  	sprintf(caLabel,"%spage4.htm",caTempDir);
			  }			  
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		    if(iFd >0) {
		          write(iFd,lt_page_content,strlen(lt_page_content));
		    }
	    	close(iFd);
	  }
   	
   	
    if(atoi(step)==4){
	    sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,srvemail);
	    system(caLabel);
    }

    
    chdir(caTempDir);
    system("rm -f report.tgz"); 
	  system("tar -cvzf report.tgz *");  
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);    
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);   

	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
	  
	  printf("\nmsaReportSrvLineAjaxSubmit_____End.................................\n");
 
    return 0;
    
    

}




/*****************************json****************************************/
int msaReportRecordJson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
 
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	char caFile1[256];
 	char cDate[128];
  char flowInfo[30000],flowInfodata[3000];
	char reportname[64];
	char reportuser[64];
	char reportstate[123];
	char templname[256];
  struct stat st;
  char   *reportdir;
	int len=0;
	msasDir *fnames;
	char   *filedir;
	char  caCmd[256];

 
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
			    bcCnfGetValue_s(caFile1,"reportname",reportname);

		      	    
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
			    
			    memset(templname,0,sizeof(templname));
			    strgb2utf8(reportstate,templname,strlen(reportstate));
		      memset(reportstate,0,sizeof(reportstate));
		      sprintf(reportstate,"%s",templname);
  
          sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,reportdir,fnames[len].caDirName,reportstate);                
				  sprintf(flowInfodata,"%s,",flowInfodata);
				  strcat(flowInfo,flowInfodata);
 
  }

	if(len!=0){
	  flowInfo[strlen(flowInfo)-1]='\0'; //增加字符串结尾标记
	}
 strcat(flowInfo,"]");
  
 
 printf("flowInfo:%s\n",flowInfo);
  
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
 if(fnames){
 	free(fnames);
 }
 
 ltMsgFree(ltMsgPk);
 lt_dbfree(dbPtr);
 
 return 0;
}




int msaZhuXianFenXiClick(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 char  *reportdir=NULL;
	 char  reporturl[255];
	 if(ltMsgGetVar_s(ltMsgPk,"reportdir")){
	 	reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
	 }
       
   dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 memset(reporturl,0,sizeof(reporturl));
	 sprintf(reporturl,"%sindex",reportdir);
	 printf("reporturl:%s\n",reporturl);
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,reporturl,dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	  
	 printf("end33333..........................\n");
   
	 return 0;   
}
