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
#ifndef __WEBLINREPORT_DIR__
#define __WEBLINREPORT_DIR__
#define webreportdatacenterdir   "/datacenter/msa/report/weblinereport"
#define weblineplt               "/app/msa/msa/htmlplt/reportplt/weblineplt"
#endif

int msashowwebreportpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
     	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/webline.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

int msaReportWebLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
		printf("msaReportWebLineAjaxSubmit.................................\n");
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    
    char sqlBuf[1024];    
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char *reportname=NULL;
		char *email=NULL;
		char *step=NULL;
		char *lt_page_content=NULL;
		char caTempDir[256]; 
		char responseDir[256]; 
    char caLabel[256];
    char strdayflow[32];
    unsigned long long lMaxBytes=1;
    unsigned long tmpDir=0;
    unsigned long fileName=0;
		
    long lTime,lTime1,lTime2; 
    int  iFd=-1;
    int  srvindex;
    
    
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
		if(ltMsgGetVar_s(ltMsgPk,"sdate")){
	  	sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
	  }else{
	  	sprintf(caSdate,"%s","");
	  }
printf("caSdate:%s\n",caSdate);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"edate")){
	  	 sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
	  }else{
	  	 sprintf(caEdate,"%s","");
	  }
printf("caEdate:%s\n",caEdate);
		
		if(ltMsgGetVar_s(ltMsgPk,"reportname")){	
	  	reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
	  }
printf("reportname:%s\n",reportname);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"email")){
	  	email=ltMsgGetVar_s(ltMsgPk,"email");
	  }
printf("email:%s\n",email);
	  
	  if(ltMsgGetVar_s(ltMsgPk,"step")){
	  	step=ltMsgGetVar_s(ltMsgPk,"step");
	  }
printf("step:%s\n",step);
    
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){	  
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }else{
	  	fileName=0;
	  }
printf("fileName:%ld\n",fileName);    
    
    /****************目录生成************************/
    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s/%lu/",webreportdatacenterdir,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);	
		    if(ltFileIsExist(caTempDir)!=1){
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				      ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
							ltMsgFree(ltMsgPk);
			        return 0;
       		 }
    		} 
     }else{//从第二步开始传进第一步生成的目录
    	  sprintf(caTempDir,"%s/%lu/",webreportdatacenterdir,fileName);   
    	  sprintf(responseDir,"%lu",fileName);
     } 


    /***********************日期处理*****************/
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
	  
	  
	  /******************************拷贝模板报告文件*************************/
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
	  sprintf(caLabel,"/bin/cp %s/right-top-d.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/cp %s/right-top.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s/banner.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s/up-1.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s/up-2.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
  	sprintf(caLabel,"/bin/cp %s/up-3.jpg  %s",weblineplt,caTempDir);
    system(caLabel);
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		
    
    
    /*************************数据库操作***************************************/  
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(atoi(step)==1){//根据时间段生成服务协议分析流量汇总报表
      sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as lcount,sum(ctime) as ctime from msauserhttpreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by host,sdate order by host asc",caSdate1,caSdate2); 
printf("sqlBufno.1: %s\n",sqlBuf);
    }else if(atoi(step)==2){
    	sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as lcount,sum(ctime) as ctime from msauserhttpreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by host,sdate order by abyte desc",caSdate1,caSdate2);    
printf("sqlBufno.2: %s\n",sqlBuf);
    }else if(atoi(step)==3){
	    sprintf(sqlBuf,"select b.dispname,host,sum(bytes) as abyte,sum(lcount) as lcount,sum(ctime) as ctime from msauserhttpreport,msaUser b where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and msauserhttpreport.userid=b.userid group by host,b.dispname order by abyte desc limit 30",caSdate1,caSdate2);
printf("sqlBufno.3: %s\n",sqlBuf);
    }else if(atoi(step)==4){
    	sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as lcount,sum(ctime) as ctime from msauserhttpreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by host,sdate order by lcount desc",caSdate1,caSdate2);
printf("sqlBufno.4: %s\n",sqlBuf);
    }else if(atoi(step)==5){
    	sprintf(sqlBuf,"select host,sum(bytes) as abyte,sum(lcount) as lcount,sum(ctime) as ctime,sorname from msauserhttpreport,nasurlsort where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and sortid=urlsort group by sorname,host,urlsort,sdate order by urlsort,abyte desc",caSdate1,caSdate2);
printf("sqlBufno.5: %s\n",sqlBuf);
    }
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }
	  
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
	  
	  /**********************将数据显示的HTML页面*********************************/
	  srvindex=1;
	  lMaxBytes=1;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"list");
	  tempRow= ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){	      			
				/**********************模板化参数****************************************/    
					if(atoi(step)==1){
						 /***********流量单位转化*****************/
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
		        
						lt_dbput_recordvars(tablePtr,4,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[2],
			   			"srvljtime",LT_TYPE_STRING,tempRow[3]
		   		  ); 
					}else if(atoi(step)==2){
						/***********流量单位转化*****************/
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
			   			"srvljtime",LT_TYPE_STRING,tempRow[3]
		   		  );
					}else if(atoi(step)==3){
						/***********流量单位转化*****************/
					  if(atoll(tempRow[2])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[2])/1024.00);
					  }
					  
						if(lMaxBytes<atoll(tempRow[2])){
		         		lMaxBytes=atoll(tempRow[2]);
		        }
		        
						lt_dbput_recordvars(tablePtr,5,
							"srvuname",LT_TYPE_STRING,tempRow[0],
							"host",LT_TYPE_STRING,tempRow[1],
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[3],
			   			"srvljtime",LT_TYPE_STRING,tempRow[4]
		   		  );
					}else if(atoi(step)==4){
						/***********流量单位转化*****************/
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
			   			"srvljtime",LT_TYPE_STRING,tempRow[3]
		   		  );
					}else if(atoi(step)==5){
						/***********流量单位转化*****************/
					  if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }
					  
						if(lMaxBytes<atoll(tempRow[1])){
		         		lMaxBytes=atoll(tempRow[1]);
		        }
		        
						lt_dbput_recordvars(tablePtr,5,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"srvljflow",LT_TYPE_STRING,strdayflow,  
			   			"srvlcount",LT_TYPE_STRING,tempRow[2],
			   			"srvljtime",LT_TYPE_STRING,tempRow[3],
			   			"sorname",LT_TYPE_STRING,tempRow[4]
		   		  );
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
	  
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  if(atoi(step)==1){
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/weblineplt/webflow.htm",dbPtr->doc,0);
	  }else if(atoi(step)==2){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/weblineplt/webflow2.htm",dbPtr->doc,0);
	  }else if(atoi(step)==3){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/weblineplt/webflow3.htm",dbPtr->doc,0);
	  }else if(atoi(step)==4){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/weblineplt/webflow4.htm",dbPtr->doc,0);
	  }else if(atoi(step)==5){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/weblineplt/webflow5.htm",dbPtr->doc,0);
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
			  }else if(atoi(step)==4){
			  	sprintf(caLabel,"%spage4.htm",caTempDir);
			  }else{
			  	sprintf(caLabel,"%spage5.htm",caTempDir);
			  }			  
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		    if(iFd >0) {
		          write(iFd,lt_page_content,strlen(lt_page_content));
		    }
	    	close(iFd);
	  }
   	
    if(atoi(step)==5){
	    sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
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
	  
printf("caTempDir:::::::::%s\n",caTempDir);///datacenter/msa/report/weblinereport/71540000/
printf("responseDir:::::::::%s\n",responseDir);///datacenter/msa/report/weblinereport/71540000/
		//把web主线生成的报告备份到手动生成报告列表
		//system("cp -r -f /datacenter/msa/report/weblinereport/* /datacenter/msa/report/instantreport/");
    return 0;

}


//编辑关键字
int msaEditkeyword(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 char *ktype=ltMsgGetVar_s(ltMsgPk,"ktype");
	 if(addflag){
	 	  long lCount;
	 	  ltDbCursor *tempCursor;
    	LT_DBROW tempRow;
	 		sprintf(sqlBuf,"select count(*) from msakeyword");
	    lCount =0;
	    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	    if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
	    }
	    if(lCount == 0) {
	        lCount=1;
	    }else{
          sprintf(sqlBuf,"select max(kid)+1 from msakeyword");
          lCount = 1;
          tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
          if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             if(tempRow!=NULL){
                       lCount=atol(tempRow[0]);
             }
             ltDbCloseCursor(tempCursor);
    			}
	    }
	 		char *kname=ltMsgGetVar_s(ltMsgPk,"kname");
	 		char *kutf8=ltMsgGetVar_s(ltMsgPk,"kutf8");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		if(atol(kutf8)==1){
	 			if(atol(ktype)==1){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'#%s',1,1)",lCount,kname);
		 		}
	 			if(atol(ktype)==2){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'#%s',1,2)",lCount,kname);
		 		}
	 			if(atol(ktype)==3){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'#%s',1,3)",lCount,kname);
		 		}
		 	}else{
	 			if(atol(ktype)==1){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'%s',0,1)",lCount,kname);
		 		}
	 			if(atol(ktype)==2){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'%s',0,2)",lCount,kname);
		 		}
	 			if(atol(ktype)==3){
		 			sprintf(sqlBuf,"insert into msakeyword values(%ld,'%s',0,3)",lCount,kname);
		 		}
		 	}
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *kid=ltMsgGetVar_s(ltMsgPk,"kid");
printf("kid：%s\n",kid);
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from msakeyword where kid=%s",kid);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
printf("sqlBuf:%s\n",sqlBuf);
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 char url[64];
printf("1............\n");
	 memset(url,0,sizeof(url));
printf("2............\n");
	 ktype=ltMsgGetVar_s(ltMsgPk,"ktype");
printf("ktype：%s\n",ktype);
	 if(atol(ktype)==1){
	 		sprintf(sqlBuf,"select * from msakeyword where ktype=1");
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/users/mailkeyword.htm");
	 }
	 if(atol(ktype)==2){
	 		sprintf(sqlBuf,"select * from msakeyword where ktype=2");
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/users/formkeyword.htm");
	 }
	 if(atol(ktype)==3){
	 		sprintf(sqlBuf,"select * from msakeyword where ktype=3");
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/users/pagekeyword.htm");
	 }
printf("3............\n");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,
				"kid",LT_TYPE_STRING,tempRow[0],
				"kname",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
printf("4............\n");
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
