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

#ifndef __WEBLINREPORT_DIR__
#define __WEBLINREPORT_DIR__
#define _webreport   "/report/weblinereport"
#define _weblineplt  "/reportplt/weblineplt"
#endif

/*
MSA:报告中心
				WEB主线
功能:web主线报告创建调用函数
*/
int msaReportWebLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;   
    char sqlBuf[1024];    
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char *reportname=NULL;
		char *email=NULL;
		char *lt_page_content=NULL;
		char *work=NULL;
		char caTempDir[256]; 
		char responseDir[256]; 
    char caLabel[256];
    char strdayflow[32];
    char htmlpage[256];
    unsigned long long lMaxBytes=1;
    unsigned long tmpDir=0;
    unsigned long fileName=0;
    long lTime,lTime1,lTime2; 
    int  iFd=-1;
    int  srvindex,step; 
    step=0;
   	memset(htmlpage,0,sizeof(htmlpage));
    memset(caLabel,0,sizeof(caLabel));
    memset(caTempDir,0,sizeof(caTempDir));
    memset(responseDir,0,sizeof(responseDir));
  	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			return 0;
		}    
		if(ltMsgGetVar_s(ltMsgPk,"sdate")){
	  	sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
	  }else{
	  	sprintf(caSdate,"%s","");
	  }  
	  if(ltMsgGetVar_s(ltMsgPk,"edate")){
	  	 sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
	  }else{
	  	 sprintf(caEdate,"%s","");
	  }
		if(ltMsgGetVar_s(ltMsgPk,"reportname")){	
	  	reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
	  }  
	  if(ltMsgGetVar_s(ltMsgPk,"email")){
	  	email=ltMsgGetVar_s(ltMsgPk,"email");
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"step")){
	  	step=atoi(ltMsgGetVar_s(ltMsgPk,"step"));
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"work")){
	  	work=ltMsgGetVar_s(ltMsgPk,"work");
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){	  
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }
    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_webreport,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);	
		    printf("caTempDir:%s\n",caTempDir);
		    if(ltFileIsExist(caTempDir)!=1){	    	
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				      ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
							ltMsgFree(ltMsgPk);
			        return 0;
       		 }
    		} 
    }else{
    	  sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_webreport,fileName);   
    	  sprintf(responseDir,"%lu",fileName);
    } 
 
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)){
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        lTime1 = 0;
    } 
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else{
        lTime2 = 0;
    } 
    
    if(lTime1 == 0) {
         lTime1 = lTime;       
    }
	  if(lTime2 == 0) {
         lTime2 = lTime;
    }
	  if(lTime1 > lTime) {
         lTime1 = lTime;
    }
    if(lTime2>lTime){
		 		 lTime2=lTime;
	  }
	  
	  char sDate[64],sTime[64];
	  char caFile1[256];   
    char caCmd[256];
    int  fdwrite=0;
    nasTimeGetDate(caSdate1,lTime);
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
	  sprintf(caCmd,"reportname=%s\n",reportname);
	  write(fdwrite,caCmd,strlen(caCmd));
	  close(fdwrite);
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/right-top-d.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/cp %s%s/right-top.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/banner.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-1.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-2.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
  	sprintf(caLabel,"/bin/cp %s%s/up-3.jpg  %s",_msahtmldir,_weblineplt,caTempDir);
    system(caLabel);
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(step==1){									//网址分类排序
       sprintf(sqlBuf,"select urlsort,sum(bytes) as abyte  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by abyte desc ",caSdate1,caSdate2,work);
    }else if(step==2){						//网址流量排序
    	 sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as acount,sum(ctime) as ctime from msauserhttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by host order by abyte asc",caSdate1,caSdate2,work); 
    }else if(step==3){						//top30流量人员网址访问
	     sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as acount,sum(ctime) as ctime from msauserhttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by host order by abyte asc limit %d offset 0",caSdate1,caSdate2,work,30); 
    }else if(step==4){						//网址访问次数排序 
    	 sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as acount,sum(ctime) as ctime from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by host order by acount desc",caSdate1,caSdate2,work);
    }else if(step==5){						//网址分类访问次数排序
    	 sprintf(sqlBuf,"select urlsort,sum(bytes) as abyte,sum(lcount) as acount  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by acount desc ",caSdate1,caSdate2,work);
    }
    printf("sql:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }	 

	  srvindex=1;
	  lMaxBytes=1;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"list");
	  tempRow= ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){				
					if(step==1){
					   if(atoll(tempRow[1])>1024000){
               	 sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	           }else{
	               sprintf(strdayflow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	           }	  	  			    
	   		     lt_dbput_recordvars(tablePtr,2,
	  	  	    "urlsort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"lbyte",LT_TYPE_STRING,strdayflow
	   		     ); 
	   		     printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
					}else if(step==2){
					  	if(atoll(tempRow[1])>1024000){
               	 sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	           }else{
	               sprintf(strdayflow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	           }	  	  			    
	   		     lt_dbput_recordvars(tablePtr,2,
	  	  	    "host",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,strdayflow
	   		     ); 
					}else if(step==3){
					  if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }				  
//						if(lMaxBytes<atoll(tempRow[1])){
//		         		lMaxBytes=atoll(tempRow[1]);
//		        }
		        
						lt_dbput_recordvars(tablePtr,4,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[2],
			   			"srvljtime",LT_TYPE_STRING,time2str(atoi(tempRow[3]))
		   		  );
					}else if(step==4){
					  if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }				  
						if(lMaxBytes<atoll(tempRow[1])){
		         		lMaxBytes=atoll(tempRow[1]);
		        }
		        
						lt_dbput_recordvars(tablePtr,4,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[2],
			   			"srvljtime",LT_TYPE_STRING,time2str(atoi(tempRow[3]))
		   		  );
					}else if(step==5){
					  if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }				  
						if(lMaxBytes<atoll(tempRow[1])){
		         		lMaxBytes=atoll(tempRow[1]);
		        }	        
						lt_dbput_recordvars(tablePtr,3,
							"sorname",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[2]		   			
		   		  );
					}
	   			if(step==1){
	          	srvindex++;
	        }
        	tempRow=ltDbFetchRow(tempCursor);//移动光标
    }
    ltDbCloseCursor(tempCursor);
//    if(step==1){//画图
//			  close(iFd);	 
//			  sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %ssrvreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);  //柱状 饼图：onlinesrv1.pl
//			  system(caLabel);	
//			  lt_dbput_rootvars(dbPtr,1,"piechart","srvreport.gif" );
//	  }   
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  sprintf(htmlpage,"%s%s/webflow%d.htm",_msahtmldir,_weblineplt,step);
	  printf("%s\n",htmlpage);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
	  if(lt_page_content==NULL){
	  	  printf("11111111111111111111111111\n");
		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	  		system(caLabel);/*建立目录和正在处理标志*/
	  		ltMsgFree(ltMsgPk);
	  		lt_dbfree(dbPtr);
     		return 0;
		}else{
				printf("22222222222222222222222222\n");
			  lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
			  if(step==1){
			  	sprintf(caLabel,"%sindex.htm",caTempDir);
			  }else{
			  	sprintf(caLabel,"%spage%d.htm",caTempDir,step);
			  }			  
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		    if(iFd >0) {
		          write(iFd,lt_page_content,strlen(lt_page_content));
		    }
	    	close(iFd);
	  }
   	
    if(step==5){
	    sprintf(caLabel,"%s/Scripts/html2mail  %s index.htm %sindex.mht report %s",_msahtmldir,caTempDir,caTempDir,email);
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
    return 0;
}

