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

#define _reportdir   "/datacenter/msa/report"
#define _sysmodeldir   "/app/msa/msa/htmlplt/reportplt/syslineplt/"
#define _riskmodeldir   "/app/msa/msa/htmlplt/reportplt/risklineplt/"
#define _cgidir   "/app/ns/bin/cgi/"
#define _MAXRERORD   20   //最大记录条数
#define _p2pUrlSort   78   //P2P服务URLSORT
#define _videoUrlSort   79   //视频服务URLSORT

int sysstep1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	 
   //系统状态分析报告	
   	ltDbCursor *tempCursor;
		LT_DBROW tempRow;
		ltDbHeadPtr dbPtr;
		ltTablePtr  tablePtr;
   
		char caSdate[32],caEdate[32],caSdate2[32],caSdate1[32];
		char email[32],sqlBuf[1024],name[32];
		char caTempDataDir1[128],caTempDataDir2[128];
		char caLabel[256],caTempDir[256],caTempFile[256],index[16];
		char caItem[16];
		char caTemp[1024];
		char caTemp1[1024];
		char sDate[64],sTime[64];
				
		int j,srcfd1,srcfd2;
	 	unsigned int time1,time2,Max2;
	 	long lTime;
    unsigned long long lUbytes,lDbytes,lMaxBytes;
		
		memset(caSdate,0,sizeof(caSdate));//初始化
		memset(caEdate,0,sizeof(caEdate));
		memset(caSdate1,0,sizeof(caSdate1));
		memset(caSdate2,0,sizeof(caSdate2));
		memset(sDate,0,sizeof(sDate));
		memset(sTime,0,sizeof(sTime));
		
		memset(sqlBuf,0,sizeof(sqlBuf));
		memset(email,0,sizeof(email));
		memset(name,0,sizeof(name));

		memset(caTempDir,0,sizeof(caTempDir));
		memset(caTempDataDir1,0,sizeof(caTempDataDir1));
		memset(caTempDataDir2,0,sizeof(caTempDataDir2));
		memset(index,0,sizeof(index));
				
		j=0;
		lMaxBytes=0;
		Max2=0;
		
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
    
		
		
		dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		lt_db_htmlpage(dbPtr,"utf-8");
		
		if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				  sprintf(caSdate,"%s","");
	  }else{
	    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
	  }
	  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				  sprintf(caEdate,"%s","");
	  }else{
	    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
				  sprintf(email,"%s","");
	  }else{
	    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
				  sprintf(name,"%s","");
	  }else{
	    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
	  }
	  
	  
	  
	  //日期处理
	  lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        time1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        time1 = 0;
    } 
    if(!ltStrIsSpaces(caEdate)) {
        time2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        time2 = 0;
    } 
    
    if(time1 == 0) {
         time1 = lTime;       
    }
	  if(time2 == 0) {
         time2 = lTime;
    }
    
	  if(time1 > lTime){//如果开始时间比当前时间大，则开始等于当前时间
         time1 = lTime;
    }
    if(time2>lTime){//如果截止时间比当前时间大，则截止时间等于当前时间
		 		 time2 = lTime;
	  }
	  
	  
	  sprintf(index,"%lu",ltStrGetId());
	  sprintf(caTempDir,"%s/sysreport/%s/",_reportdir,index);
    if(ltFileIsExist(caTempDir)!=1) {
	        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");
						ltMsgFree(ltMsgPk);
            return 0;
	        }
   	}
   	
   	char caCmd[256],infofile[128];
   	memset(caCmd,0,sizeof(caCmd));
   	memset(infofile,0,sizeof(infofile));
   	
    int  fdwrite=0;
    sprintf(infofile,"%sinfo.ini",caTempDir);
    fdwrite = open(infofile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
		}
		
		nasCvtStime(time(0),sDate,sTime); 
		sprintf(caCmd,"date=%s_%s\n",sDate,sTime);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"reportname=%s\n",name);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"email=%s\n",email);
	  write(fdwrite,caCmd,strlen(caCmd));
	  close(fdwrite);

    
    sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/right-top-d.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/right-top.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/banner.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/up-1.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/up-2.jpg  %s",caTempDir);
	  system(caLabel);
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/syslineplt/up-3.jpg  %s",caTempDir);
	  system(caLabel);
    
    
		sprintf(caTempDataDir1,"%sdata1",caTempDir);
		srcfd1 = open(caTempDataDir1,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd1 == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);
		    	ltMsgFree(ltMsgPk);
		      return 0;
		}

		sprintf(caTempDataDir2,"%sdata2",caTempDir);
		srcfd2 = open(caTempDataDir2,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd2 == (-1)){
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);
		    	ltMsgFree(ltMsgPk);
		      return 0;
		}
		
	  char upflow[32];//up
	  char downflow[32];//down
	  
	  nasTimeGetDate(caSdate1,time1);
    nasTimeGetDate(caSdate2,time2);
    
		/*sprintf(sqlBuf,"select sdate,cast(avg(ubytes) as dec(18,2)) as ubytes,cast(avg(dbytes) as dec(18,2)) as dbytes,cast(avg(tcpconnect) as dec(18,2)) as tcpconnect,cast(avg(udpconnect) as dec(18,2)) as udpconnect,cast(avg(onlineuser) as dec(18,2)) as onlineuser
		 from msatimereport where rtime<=%u and rtime>=%u group by sdate order by sdate asc",time2,time1);*/
		
		sprintf(sqlBuf,"select sdate,cast(avg(ubytes) as dec(18,2)) as ubytes,cast(avg(dbytes) as dec(18,2)) as dbytes,cast(avg(tcpconnect) as dec(18,2)) as tcpconnect,cast(avg(udpconnect) as dec(18,2)) as udpconnect,cast(avg(onlineuser) as dec(18,2)) as onlineuser\
		 from msatimereport where sdate>='%s' and sdate<='%s' group by sdate order by sdate asc",caSdate1,caSdate2);
		 
    printf("step1 SQL:%s \n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){
	    		
		   		   memset(upflow,0,sizeof(upflow));
		   		   memset(downflow,0,sizeof(downflow));
		   		   memset(caItem,0,sizeof(caItem));
		   		   memset(caTemp,0,sizeof(caTemp));
		   		   memset(caTemp1,0,sizeof(caTemp1));
		   		   
		   		   if(atoll(tempRow[1])>1024000){
               sprintf(upflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
             }else{
               sprintf(upflow,"%0.3fKB",atoll(tempRow[1])/1024.00);
             }
		   		   if(atoll(tempRow[2])>1024000){
               sprintf(downflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
             }else{
               sprintf(downflow,"%0.3fKB",atoll(tempRow[2])/1024.00);
             }
						 sprintf(caItem,"%s",tempRow[0]);
						 
	           lt_dbput_recordvars(tablePtr,6,
		           "stime",LT_TYPE_STRING,tempRow[0],
		           "upflow",LT_TYPE_STRING,upflow,
		           "downflow",LT_TYPE_STRING,downflow,
			 				 "tcpcon",LT_TYPE_STRING,tempRow[3],
		           "udpcon",LT_TYPE_STRING,tempRow[4],
		           "online",LT_TYPE_STRING,tempRow[5]
	           );
	           
				     lUbytes=atoll(tempRow[2])/1024;
		         lDbytes=atoll(tempRow[3])/1024;
		         
			       if( lMaxBytes <= (lUbytes + lDbytes) ) {
			           lMaxBytes = lUbytes + lDbytes+5;
			       }
			       
	           if(Max2<atoll(tempRow[5]))
	            Max2=atoll(tempRow[5]+5);
	            
				 		 if( (j%3)==0 ){
			          sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",caItem,lUbytes+lDbytes,lDbytes,lUbytes);
			          write(srcfd1, caTemp, strlen(caTemp));
			       }else{
			      		sprintf(caTemp,"%s,%llu,%llu,%llu \r\n","",lUbytes+lDbytes,lDbytes,lUbytes);
			        	write(srcfd1, caTemp, strlen(caTemp));
			       }
			       
			       j++;

			       //人员曲线
	      		 sprintf(caTemp1,"%s,%llu \r\n","",atoll(tempRow[5]));
	        	 write(srcfd2, caTemp1, strlen(caTemp));

			    	 tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
    }
    
		memset(caTempFile,0,sizeof(caTempFile));
    sprintf(caTempFile,"%sflowview.gif",caTempDir);
    sprintf(caTemp,"%soutflowplt.pl   %s %s %llu ",_cgidir,caTempDataDir1,caTempFile,lMaxBytes);
    system(caTemp);
    
    lt_dbput_rootvars(dbPtr,1,"file1","flowview.gif");

		memset(caTempFile,0,sizeof(caTempFile));
    sprintf(caTempFile,"%sonlineuser.gif",caTempDir);
    sprintf(caTemp,"%soutflowpltline.pl   %s %s %u ",_cgidir,caTempDataDir2,caTempFile,Max2);
    system(caTemp);
    
    lt_dbput_rootvars(dbPtr,1,"file2","onlineuser.gif");
    
		lt_dbput_rootvars(dbPtr,3,"reportname",name,"bdate",caSdate1,"edate",caSdate2);
		
   	char  *lt_page_content;
	  int   iFd;
	  char  modelpath[128];
	  
	  memset(modelpath,0,sizeof(modelpath));
	  sprintf(modelpath,"%sstep1.htm",_sysmodeldir);
    lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
    
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  	}else{
		        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		        sprintf(caLabel,"%sindex.htm",caTempDir);
		        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
		        if(iFd >0) {
		            write(iFd,lt_page_content,strlen(lt_page_content));
		        }
		        close(iFd);
		      	/*生成html格式报告*/
  	}
  	
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);

		chdir(caTempDir);
	  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
   	close(srcfd1);
   	close(srcfd2);
   	
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,index);
	  ltMsgFree(ltMsgPk);
	  
	  return 0;
	  
}



//系统分析第二步
int sysstep2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32],srvname[128];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
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
    
	
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/sysreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"%s","select sdate,hours,service,ubytes+dbytes as flow,onlinetime from msatimesrvreport order by flow desc");
  printf("||||||SQL %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
	  			 stime = nasCvtLongTime(tempRow[0],tempRow[1]);
    			 if(stime<time1||stime>time2){
		    	 		tempRow=ltDbFetchRow(tempCursor);
    			 		continue;
    			 }
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   memset(srvname,0,sizeof(srvname));
	   		   if(atoll(tempRow[3])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[3])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[3])/1024.00);
           }
           sprintf(srvname,"%s",_ltPubInfo->topSrvName[atoi(tempRow[2])].srvname);
           lt_dbput_recordvars(tablePtr,3,
           "srvname",LT_TYPE_STRING,srvname,
           "srvtime",LT_TYPE_STRING,tempRow[4],
	 				 "srvflow",LT_TYPE_STRING,flow);
	 				 if(num>=_MAXRERORD){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }
  

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep2.htm",_sysmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step2.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  
  return 0;
}




//部门和人员
int sysstep3(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32];
	int num,srcfd;
 	unsigned int time1,time2,stime;
	char flow[32],gname[64],uname[64],ctime[32],otime[32],numchar[8],caTempDataDir[128],caTempFile[128];
	char caTemp[18192];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caTempDataDir,0,sizeof(caTempDataDir));
	memset(caTempFile,0,sizeof(caTempFile));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	memset(caTemp,0,sizeof(caTemp));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	nasDbUserInfo *tmpdbuserinfo;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr1;
	ltTablePtr  tablePtr2;
	
	
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
    
	
	
	dbPtr=lt_dbinit();
  tablePtr1=lt_dbput_table(dbPtr,"list1");
  tablePtr2=lt_dbput_table(dbPtr,"list2");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/sysreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);

  sprintf(caTempDataDir,"%s/data3",caTempDir);
	srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		      return 0;
	}

	sprintf(sqlBuf,"%s","select userid,sdate,bytes as flow,onlinetime from msasrvreport order by flow desc");
	printf("SQL:  %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
	  			 stime = nasCvtLongTime(tempRow[1],"23:59:59");
    			 if(stime<time1||stime>time2){
		    	 		tempRow=ltDbFetchRow(tempCursor);
    			 		continue;
    			 }
		    	 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   memset(gname,0,sizeof(gname));
	   		   memset(uname,0,sizeof(uname));
	   		   memset(ctime,0,sizeof(ctime));
	   		   memset(otime,0,sizeof(otime));
	   		   memset(numchar,0,sizeof(numchar));
	   		   if(atoll(tempRow[2])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[2])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[2])/1024.00);
           }
           //部门
           tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,atoi(tempRow[0]));
     			 if(tmpdbuserinfo!=NULL){
		           sprintf(uname,"%s",tmpdbuserinfo->dispname);
		           sprintf(gname,"%s",_ltPubInfo->_GroupList[tmpdbuserinfo->groupid[0]].name);
     			 }
           //时间
					 nasTimeFormat(ctime,atol(tempRow[3]));
					 nasTimeFormat(otime,atol(tempRow[4]));

					 sprintf(numchar,"%d",num);
           lt_dbput_recordvars(tablePtr2,5,
           "num",LT_TYPE_STRING,numchar,
           "gname",LT_TYPE_STRING,gname,
           "uname",LT_TYPE_STRING,uname,
           "allflow",LT_TYPE_STRING,flow,
	 				 "otime",LT_TYPE_STRING,otime);
	 				 if(num>=_MAXRERORD){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }
  //部门汇总
  num=0;
  memset(sqlBuf,0,sizeof(sqlBuf));
  sprintf(sqlBuf,"select cast(avg(msasrvreport.bytes) as dec(18,3)) as flow,cast(avg(msasrvreport.onlinetime) as dec(18,3)) as onlinetime,msauser.groupid0 from msasrvreport left join msauser on msasrvreport.userid=msauser.userid where msasrvreport.sdate<'%s' and msasrvreport.sdate>'%s' group by msauser.groupid0 order by flow desc",caEdate,caSdate);
	printf("gourp SQL:  %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
		    	 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   memset(gname,0,sizeof(gname));
	   		   memset(otime,0,sizeof(otime));
	   		   if(atoll(tempRow[0])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
           }
           if(strcmp(tempRow[0],"")!=0){
		       	 sprintf(gname,"%s",_ltPubInfo->_GroupList[atoi(tempRow[2])].name);
           }
					 nasTimeFormat(otime,atol(tempRow[1]));
           lt_dbput_recordvars(tablePtr1,3,
           "gname",LT_TYPE_STRING,gname,
           "allflow",LT_TYPE_STRING,flow,
	 				 "ctime",LT_TYPE_STRING,ctime);
	 				 //饼图
	 				 sprintf(caTemp,"%s,%s \r\n",gname,flow);
       		 write(srcfd, caTemp, strlen(caTemp));
	 				 if(num>=_MAXRERORD){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }
	unsigned int Max = 10000;
  sprintf(caTempFile,"%s/groupPie.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas2/cgi/onlinesrv1_s.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file","groupPie.gif");

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep3.htm",_sysmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step3.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  return 0;
}



//前50名访问流量的网址和网址分类
int sysstep4(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],numchar[8];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32],sortname[128];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	memset(numchar,0,sizeof(numchar));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
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
    
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/sysreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"%s","select sdate,host,bytes,lcount,ctime,urlsort from msahttpreport order by bytes desc");
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
	  			 stime = nasCvtLongTime(tempRow[0],"23:59:59");
    			 if(stime<time1||stime>time2){
		    	 		tempRow=ltDbFetchRow(tempCursor);
    			 		continue;
    			 }
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   memset(sortname,0,sizeof(sortname));
	   		   if(atoll(tempRow[2])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[2])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[2])/1024000.00);
           }
           sprintf(sortname,"%s",_ltPubInfo->ltService[atoi(tempRow[5])].srvName);
           sprintf(numchar,"%d",num);
           lt_dbput_recordvars(tablePtr,5,
           "num",LT_TYPE_STRING,numchar,
           "sortname",LT_TYPE_STRING,sortname,
           "webhost",LT_TYPE_STRING,tempRow[1],
           "ctimes",LT_TYPE_STRING,tempRow[3],
	 				 "flow",LT_TYPE_STRING,flow);
	 				 if(num>=50){
	 				 		break;
	 				 }
		    	 tempRow=ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }
 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep4.htm",_sysmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
        system(caLabel);/*建立目录和正在处理标志*/
        ltMsgFree(ltMsgPk);
        lt_dbfree(dbPtr);
        return 0;
	}else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
        sprintf(caLabel,"%s/step4.htm",caTempDir);
        printf("caLabel    %s \n",caLabel);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  
  return 0;
}


//前50名访问流量的目的地址
int sysstep5(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],numchar[8],otime[16];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	memset(numchar,0,sizeof(numchar));
	memset(otime,0,sizeof(otime));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
	
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
    
	
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/sysreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"%s","select sdate,dip,bytes,ctime,onlinetime from msaDipReport order by bytes desc");
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
	  			 stime = nasCvtLongTime(tempRow[0],"23:59:59");
    			 if(stime<time1||stime>time2){
		    	 		tempRow=ltDbFetchRow(tempCursor);
    			 		continue;
    			 }
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   if(atoll(tempRow[2])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[2])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[2])/1024.00);
           }
           sprintf(numchar,"%d",num);
					 nasTimeFormat(otime,atol(tempRow[4]));
           lt_dbput_recordvars(tablePtr,5,
           "num",LT_TYPE_STRING,numchar,
           "otime",LT_TYPE_STRING,otime,
           "webhost",LT_TYPE_STRING,tempRow[1],
           "ctimes",LT_TYPE_STRING,tempRow[3],
	 				 "flow",LT_TYPE_STRING,flow);
	 				 if(num>=50){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep5.htm",_sysmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
        system(caLabel);/*建立目录和正在处理标志*/
        ltMsgFree(ltMsgPk);
        lt_dbfree(dbPtr);
        return 0;
	}else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
        sprintf(caLabel,"%s/step5.htm",caTempDir);
        printf("caLabel    %s \n",caLabel);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  
  return 0;
}



//网络风险分析
int riskstep1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
		char caSdate[32],caEdate[32],caSdate2[32],caSdate1[32],name[32];
		char caLabel[256],caTempDir[256],sqlBuf[1024],email[32],index[16];
		long lTime;
		int j,num;
	 	unsigned int time1,time2;
		char sortname[128],numchar[8];
		char sDate[64],sTime[64];
		
		memset(sDate,0,sizeof(sDate));
		memset(sTime,0,sizeof(sTime));
		memset(caSdate,0,sizeof(caSdate));
		memset(caEdate,0,sizeof(caEdate));
		memset(caSdate2,0,sizeof(caSdate2));
		memset(caSdate1,0,sizeof(caSdate1));
		
		memset(sqlBuf,0,sizeof(sqlBuf));
		memset(email,0,sizeof(email));
		memset(name,0,sizeof(name));	
		memset(caTempDir,0,sizeof(caTempDir));
		memset(index,0,sizeof(index));
		j=0;
	 	ltDbCursor *tempCursor;
		LT_DBROW tempRow;
		ltDbHeadPtr dbPtr;
		ltTablePtr  tablePtr1;
		ltTablePtr  tablePtr2;
		
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
    
		
		dbPtr=lt_dbinit();
    tablePtr1=lt_dbput_table(dbPtr,"list1");
    tablePtr2=lt_dbput_table(dbPtr,"list2");
		lt_db_htmlpage(dbPtr,"utf-8");
		if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				  sprintf(caSdate,"%s","");
	  }else{
	    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
	  }
	  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				  sprintf(caEdate,"%s","");
	  }else{
	    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
				  sprintf(email,"%s","");
	  }else{
	    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
				  sprintf(name,"%s","");
	  }else{
	    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
	  }
	  
	  
	  //日期处理
	  lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        time1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        time1 = 0;
    } 
    if(!ltStrIsSpaces(caEdate)) {
        time2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        time2 = 0;
    } 
    
    if(time1 == 0) {
         time1 = lTime;       
    }
	  if(time2 == 0) {
         time2 = lTime;
    }
    
	  if(time1 > lTime){//如果开始时间比当前时间大，则开始等于当前时间
         time1 = lTime;
    }
    if(time2>lTime){//如果截止时间比当前时间大，则截止时间等于当前时间
		 		 time2 = lTime;
	  }
	  
	  
	  sprintf(index,"%lu",ltStrGetId());
	  sprintf(caTempDir,"%s/riskreport/%s/",_reportdir,index);
    if(ltFileIsExist(caTempDir)!=1) {
	        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            // 生成该报告目录错误
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");
						ltMsgFree(ltMsgPk);
            return 0;
	        }
   	}

   	char caCmd[256],infofile[128];
   	memset(caCmd,0,sizeof(caCmd));
   	memset(infofile,0,sizeof(infofile));
    int  fdwrite=0;
    sprintf(infofile,"%sinfo.ini",caTempDir);
    fdwrite = open(infofile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
		}
		
		nasCvtStime(time(0),sDate,sTime); 
		sprintf(caCmd,"date=%s_%s\n",sDate,sTime);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"reportname=%s\n",name);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"email=%s\n",email);
	  write(fdwrite,caCmd,strlen(caCmd));
	  close(fdwrite);
  
    
    sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/banner.jpg %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/right-top.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/banner.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/up-1.jpg  %s",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/up-2.jpg  %s",caTempDir);
	  system(caLabel);
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/reportplt/risklineplt/up-3.jpg  %s",caTempDir);
	  system(caLabel);

	  char flow[32];
	  num=0;
	  
	  nasTimeGetDate(caSdate1,time1);
    nasTimeGetDate(caSdate2,time2);
	  
		sprintf(sqlBuf,"select cast(avg(bytes) as dec(18,3)) as flow,cast(avg(lcount) as dec(18,0)) as lcount,cast(avg(ctime) as dec(18,0)) as ctime,urlsort from msahttpreport where sdate>='%s' and sdate<='%s'group by urlsort order by flow desc",caSdate1,caSdate2);
    printf("SQL %s \n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){
	    			 num++;
		   		   memset(flow,0,sizeof(flow));
		   		   memset(sortname,0,sizeof(sortname));
		   		   if(atoll(tempRow[0])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
             }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
             }
           	 sprintf(sortname,"%s",_ltPubInfo->ltService[atoi(tempRow[3])].srvName);
             sprintf(numchar,"%d",num);
	           lt_dbput_recordvars(tablePtr1,5,
           	 "num",LT_TYPE_STRING,numchar,
	           "flow",LT_TYPE_STRING,flow,
	           "lcount",LT_TYPE_STRING,tempRow[1],
		 				 "ctime",LT_TYPE_STRING,tempRow[2],
	           "sortname",LT_TYPE_STRING,sortname);
		 				 if(num>=_MAXRERORD){
		 				 		break;
		 				 }
		    	   tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
    }
    
    //点击次数排序
    memset(sqlBuf,0,sizeof(sqlBuf));
    num=0;
    sprintf(sqlBuf,"select cast(avg(bytes) as dec(18,3)) as flow,cast(avg(lcount) as dec(18,0)) as lcount,cast(avg(ctime) as dec(18,0)) as ctime,urlsort from msahttpreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by urlsort order by lcount desc",caSdate1,caSdate2);
    printf("SQL %s \n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){
	    			 num++;
		   		   memset(flow,0,sizeof(flow));
		   		   memset(sortname,0,sizeof(sortname));
		   		   if(atoll(tempRow[0])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
             }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
             }
           	 sprintf(sortname,"%s",_ltPubInfo->ltService[atoi(tempRow[3])].srvName);
             sprintf(numchar,"%d",num);
	           lt_dbput_recordvars(tablePtr2,5,
           	 "num",LT_TYPE_STRING,numchar,
	           "flow",LT_TYPE_STRING,flow,
	           "lcount",LT_TYPE_STRING,tempRow[1],
		 				 "ctime",LT_TYPE_STRING,tempRow[2],
	           "sortname",LT_TYPE_STRING,sortname);
		 				 if(num>=_MAXRERORD){
		 				 		break;
		 				 }
		    	 	 tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
    }
    
    lt_dbput_rootvars(dbPtr,3,"reportname",name,"bdate",caSdate1,"edate",caSdate2);
    
   	char  *lt_page_content;
	  int   iFd;
	  char  modelpath[128];
	  memset(modelpath,0,sizeof(modelpath));
	  sprintf(modelpath,"%sstep1.htm",_riskmodeldir);
    lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  	}else{
		        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		        sprintf(caLabel,"%sindex.htm",caTempDir);
		        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
		        if(iFd >0) {
		            write(iFd,lt_page_content,strlen(lt_page_content));
		        }
		        close(iFd);
		      	/*生成html格式报告*/
  	}
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);

		chdir(caTempDir);
	  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,index);
	  ltMsgFree(ltMsgPk);
	  return 0;
}



//人员流量风险
int riskstep2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],numchar[8];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
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
    
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/riskreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"select cast(avg(msauserhttpreport.bytes) as dec(18,3)) as flow,cast(avg(msauserhttpreport.lcount) as dec(18,0)) as lcount,cast(avg(msauserhttpreport.ctime) as dec(18,0)) as ctime,msauser.dispname from msauserhttpreport left join msauser on msauserhttpreport.userid=msauser.userid where msauserhttpreport.sdate<'%s' and msauserhttpreport.sdate>'%s' group by msauser.dispname order by flow desc",caEdate,caSdate);
  printf("||||||SQL %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   if(atoll(tempRow[0])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
           }
           sprintf(numchar,"%d",num);
           lt_dbput_recordvars(tablePtr,5,
           "num",LT_TYPE_STRING,numchar,
           "name",LT_TYPE_STRING,tempRow[3],
           "ctime",LT_TYPE_STRING,tempRow[2],
	 				 "lcount",LT_TYPE_STRING,tempRow[1],
	 				 "flow",LT_TYPE_STRING,flow);
	 				 if(num>=_MAXRERORD){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep2.htm",_riskmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step2.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  return 0;
}




//p2p与视频流量top50人员
int riskstep3(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],numchar[8];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
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
    
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");

	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/riskreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"select cast(avg(msauserhttpreport.bytes) as dec(18,3)) as flow,cast(avg(msauserhttpreport.lcount) as dec(18,0)) as lcount,cast(avg(msauserhttpreport.ctime) as dec(18,0)) as ctime,msauser.dispname from msauserhttpreport left join msauser on msauserhttpreport.userid=msauser.userid where msauserhttpreport.sdate<'%s' and msauserhttpreport.sdate>'%s' and urlsort=%d or urlsort=%d group by msauser.dispname order by flow desc",caEdate,caSdate,_p2pUrlSort,_videoUrlSort);
  printf("||||||SQL %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   if(atoll(tempRow[0])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
           }
           sprintf(numchar,"%d",num);
           lt_dbput_recordvars(tablePtr,5,
           "num",LT_TYPE_STRING,numchar,
           "name",LT_TYPE_STRING,tempRow[3],
           "ctime",LT_TYPE_STRING,tempRow[2],
	 				 "lcount",LT_TYPE_STRING,tempRow[1],
	 				 "flow",LT_TYPE_STRING,flow);
	 				 if(num>=50){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep3.htm",_riskmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step3.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  return 0;
}



//非工作网站访问排序top50人员
int riskstep4(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],numchar[8];
	int num;
 	unsigned int time1,time2,stime;
	char flow[32],gname[64],ctime[64];
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	num=0;
 	stime=0;
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
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
    
	
	dbPtr=lt_dbinit();
  tablePtr=lt_dbput_table(dbPtr,"list");
	lt_db_htmlpage(dbPtr,"utf-8");
	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }
  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/riskreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));
  printf("caTempDir   %s \n",caTempDir);
	sprintf(sqlBuf,"select userid,sum(bytes)as flow,sum(ctime)as ctime from msauserhttpreport where sdate<='%s' and sdate>='%s' group by userid order by flow desc",caEdate,caSdate);
  printf("||||||SQL %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	nasDbUserInfo *tmpdbuserinfo;
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
    			 num++;
	   		   memset(flow,0,sizeof(flow));
	   		   if(atoll(tempRow[1])>1024000){
             sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
           }else{
             sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
           }
	 		 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,atoi(tempRow[0]));
	 		 		 //部门处理
	   		   memset(gname,0,sizeof(gname));
	   		   sprintf(gname,"%s",_ltPubInfo->_GroupList[0].name);
	 		 		 int j;
	 		 		 for(j=0;j<5;j++){
	 		 		 		if(tmpdbuserinfo->groupid[j]!=0){
	 		 		 			sprintf(gname,"%s",_ltPubInfo->_GroupList[tmpdbuserinfo->groupid[j]].name);
	 		 		 		}
	 		 		 }
	 		 		 //时间
	   		   memset(ctime,0,sizeof(ctime));
					 nasTimeGetTime(ctime,atol(tempRow[2]));

           sprintf(numchar,"%d",num);
           lt_dbput_recordvars(tablePtr,5,
           "num",LT_TYPE_STRING,numchar,
           "group",LT_TYPE_STRING,gname,
           "name",LT_TYPE_STRING,tmpdbuserinfo->dispname,
           "flow",LT_TYPE_STRING,flow,
	 				 "ctime",LT_TYPE_STRING,ctime);
	 				 if(num>=20){
	 				 		break;
	 				 }
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }

 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep4.htm",_riskmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step4.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  return 0;
}



//非工作网站访问排序top50人员的详情
typedef struct username_s {
    char     name[32];
}username;

int riskstep5(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	username  _username[20];
	char sqlBuf[1024];
	char caTempDir[256],caLabel[128],caSdate[32],caEdate[32],ctime[32],gname[32],sortname[32],uname[32];
	int num,i,iii,index;
 	unsigned int time1,time2;
	char numchar[8],caTempDataDir[128],caTempFile[128];
  
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(caTempDir,0,sizeof(caTempDir));
	memset(caTempDataDir,0,sizeof(caTempDataDir));
	memset(caTempFile,0,sizeof(caTempFile));
	memset(caLabel,0,sizeof(caLabel));
	memset(caSdate,0,sizeof(caSdate));
	memset(caEdate,0,sizeof(caEdate));
	
	num=0;
	index=0;
 	ltDbCursor *tempCursor;
	nasDbUserInfo *tmpdbuserinfo;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	
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
    
	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	int userid[20];
	ltTablePtr  tablePtr[20];
	char tablename[16];
	for(iii=0;iii<20;iii++){
			memset(tablename,0,sizeof(tablename));
			sprintf(tablename,"list%d",iii);
  		tablePtr[iii]=lt_dbput_table(dbPtr,tablename);
	  	userid[iii]=-1;
	}
	
	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			  sprintf(caSdate,"%s","");
  }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
  }

  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
  }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
  }
  time1 = nasCvtLongTime(caSdate,"00:00:00");
  time2 = nasCvtLongTime(caEdate,"23:59:59");
  sprintf(caTempDir,"%s/riskreport/%ld",_reportdir,atol(ltMsgGetVar_s(ltMsgPk,"index")));

	sprintf(sqlBuf,"select msauser.dispname,sum(msauserhttpreport.bytes)as flow,msauserhttpreport.userid from msauserhttpreport left join msauser on msauser.userid=msauserhttpreport.userid where msauserhttpreport.sdate<='%s' and msauserhttpreport.sdate>='%s' group by msauser.dispname,msauserhttpreport.userid order by flow desc",caEdate,caSdate);
	printf("SQL:  %s \n",sqlBuf);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
	    tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
		    	 sprintf(_username[num].name,"%s",tempRow[0]);
		    	 userid[num]=atoi(tempRow[2]);
		    	 num++;
		    	 tempRow= ltDbFetchRow(tempCursor);
	    }
      ltDbCloseCursor(tempCursor);
  }
  int j;
	for(i=0;i<num;i++){
			index=0;
		  tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,userid[i]);
		  //部门处理
	    memset(gname,0,sizeof(gname));
	    sprintf(gname,"%s",_ltPubInfo->_GroupList[0].name);
		  for(j=0;j<5;j++){
			 		if(tmpdbuserinfo->groupid[j]!=0){
			 			sprintf(gname,"%s",_ltPubInfo->_GroupList[tmpdbuserinfo->groupid[j]].name);
			 		}
		  }
		  //用户名
	    memset(uname,0,sizeof(uname));
	    sprintf(uname,"username%d",i);
			lt_dbput_rootvars(dbPtr,1,uname,_username[i].name);
			
			memset(sqlBuf,0,sizeof(sqlBuf));
			sprintf(sqlBuf,"select host,urlsort,sum(ctime) as ctime,sum(bytes) as flow from msauserhttpreport where userid=%d and msauserhttpreport.sdate<='%s' and msauserhttpreport.sdate>='%s' group by host,urlsort order by flow desc",userid[i],caEdate,caSdate);
			printf("SQL2:  %s \n",sqlBuf);
		  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		  if(tempCursor!=NULL){
			    tempRow= ltDbFetchRow(tempCursor);
		    	while(tempRow!=NULL){
							 index++;
							 sprintf(numchar,"%d",index);
							 //时间
							 memset(ctime,0,sizeof(ctime));
							 nasTimeGetTime(ctime,atol(tempRow[2]));
							 //URL分类
							 memset(sortname,0,sizeof(sortname));
							 sprintf(sortname,"%s",_ltPubInfo->ltService[atoi(tempRow[1])].srvName);
							 lt_dbput_recordvars(tablePtr[i],6,
			           "iIndex",LT_TYPE_STRING,numchar,
			           "groupname",LT_TYPE_STRING,gname,
			           "uname",LT_TYPE_STRING,_username[i].name,
				 				 "host",LT_TYPE_STRING,tempRow[0],
				 				 "urlsort",LT_TYPE_STRING,sortname,
				 				 "contime",LT_TYPE_STRING,ctime);
			 				 if(index>=10){
			 				 		break;
			 				 }
				    	 tempRow= ltDbFetchRow(tempCursor);
			    }
		      ltDbCloseCursor(tempCursor);
		  }
	}
 	char  *lt_page_content;
  int   iFd;
  char  modelpath[128];
  memset(modelpath,0,sizeof(modelpath));
  sprintf(modelpath,"%sstep5.htm",_riskmodeldir);
  lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
  if(lt_page_content==NULL){
          sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
          system(caLabel);/*建立目录和正在处理标志*/
          ltMsgFree(ltMsgPk);
          lt_dbfree(dbPtr);
          return 0;
	}else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%s/step5.htm",caTempDir);
	        printf("caLabel    %s \n",caLabel);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	        close(iFd);
	      	/*生成html格式报告*/
	}
	chdir(caTempDir);
  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  return 0;
}

//计费报告
typedef struct userlist_s {
    int     userid;
    int     jftype;
    char    dispname[32];
    char    groupname[32];
}userlist;

static int sortuserlist(const void *p1,const void *p2)
{
    userlist *gname1,*gname2;
    gname1 = (userlist *)p1;
    gname2 = (userlist *)p2;
    return (gname2->groupname - gname1->groupname);
}


int jfreport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	  
	  char caSdate[32],caEdate[32],sqlBuf[1024],email[32],caTime1[32],caSdate1[32],name[32];
		char caLabel[256],caTempDir[256],index[16],gname[32],tempstr[32],nowaccount[8],otime[32];
		long lTime;
		int i,j,num,lcount;
		int gid,level;
	 	unsigned int time1,time2;
	 	nasDbUserInfo *tmpdbuserinfo;
		memset(caSdate,0,sizeof(caSdate));
		memset(caEdate,0,sizeof(caEdate));
		memset(sqlBuf,0,sizeof(sqlBuf));
		memset(email,0,sizeof(email));
		memset(name,0,sizeof(name));
		memset(caTime1,0,sizeof(caTime1));
		memset(caSdate1,0,sizeof(caSdate1));
		memset(caTempDir,0,sizeof(caTempDir));
		memset(index,0,sizeof(index));
		memset(otime,0,sizeof(otime));
		j=0;
		i=0;
		lcount=1;
	 	ltDbCursor *tempCursor;
		LT_DBROW tempRow;
		ltDbHeadPtr dbPtr;
		ltTablePtr  tablePtr;
		
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
    
		
		dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		lt_db_htmlpage(dbPtr,"utf-8");
		gid=atoi(ltMsgGetVar_s(ltMsgPk,"gid"));
		if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				  sprintf(caSdate,"%s","");
	  }else{
	    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
	  }
	  if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				  sprintf(caEdate,"%s","");
	  }else{
	    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
				  sprintf(email,"%s","");
	  }else{
	    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
	  }
		if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
				  sprintf(name,"%s","");
	  }else{
	    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
	  }
	  time1 = nasCvtLongTime(caSdate,"00:00:00");
	  time2 = nasCvtLongTime(caEdate,"23:59:59");
	  lTime=time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  sprintf(index,"%lu",ltStrGetId());
	  sprintf(caTempDir,"%s/jfreport/%s/",_reportdir,index);
    if(ltFileIsExist(caTempDir)!=1) {
	        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            // 生成该报告目录错误
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");
						ltMsgFree(ltMsgPk);
            return 0;
	        }
   	}

   	char caCmd[256],infofile[128];
   	memset(caCmd,0,sizeof(caCmd));
   	memset(infofile,0,sizeof(infofile));
    int  fdwrite=0;
    sprintf(infofile,"%sinfo.ini",caTempDir);
    fdwrite = open(infofile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
		}
		sprintf(caCmd,"date=%s_%s\n",caSdate1,caTime1);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"reportname=%s\n",name);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"email=%s\n",email);
	  write(fdwrite,caCmd,strlen(caCmd));
	  close(fdwrite);
	  
		sprintf(caLabel,"/bin/cp %sindex.htm  %s",_sysmodeldir,caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/    
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
		memset(sqlBuf,0,sizeof(sqlBuf));
    sprintf(sqlBuf,"select count(*) from (select count(*) from msaaccountreport where sdate>='%s' and sdate<='%s' group by userid) foo",caSdate,caEdate);
		printf("countSQL   %s  \n",sqlBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
				lcount=atoi(tempRow[0]);
			}
			ltDbCloseCursor(tempCursor);
		}
		//查询USER
		userlist  _userlist[lcount];
	  char flow[32];
	  num=0;
	  level=_ltPubInfo->_GroupList[gid].level;
	  memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select userid from msaaccountreport where sdate>='%s' and sdate<='%s' group by userid",caSdate,caEdate);
    printf("SQL %s \n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){
	    			 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,atoi(tempRow[0]));
	    			 if(gid!=-1&&tmpdbuserinfo->groupid[level]!=gid){
			    			//部门过滤
		    	   		tempRow= ltDbFetchRow(tempCursor);
			    			continue;
	    			 }
					   //部门处理
				     memset(gname,0,sizeof(gname));
				     sprintf(gname,"%s",_ltPubInfo->_GroupList[0].name);
					   for(j=0;j<5;j++){
						 		if(tmpdbuserinfo->groupid[j]!=0){
						 			sprintf(gname,"%s",_ltPubInfo->_GroupList[tmpdbuserinfo->groupid[j]].name);
						 		}
					   }
		    	 	 sprintf(_userlist[num].groupname,"%s",gname);
		    	 	 sprintf(_userlist[num].dispname,"%s",tmpdbuserinfo->dispname);
		   		   _userlist[num].userid=atoi(tempRow[0]);
		   		   _userlist[num].jftype=tmpdbuserinfo->jftype;
	    			 num++;
		    	   tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
    }
    //部门排序
		qsort(_userlist,lcount,sizeof(userlist),sortuserlist);
    for(i=0;i<num;i++){
    		//账户余额查询
    		memset(sqlBuf,0,sizeof(sqlBuf));
		    sprintf(sqlBuf,"select nowaccount from msaaccountreport where sdate>='%s' and userid=%d order by sdate asc,nowaccount desc",caSdate,_userlist[i].userid);
				printf("nowaccountSQL   %s  \n",sqlBuf);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow){
						memset(nowaccount,0,sizeof(nowaccount));
						sprintf(nowaccount,"%s",tempRow[0]);
					}
					ltDbCloseCursor(tempCursor);
				}
    		//按部门统计
    		memset(sqlBuf,0,sizeof(sqlBuf));
				sprintf(sqlBuf,"select sum(ubytes+dbytes) as flow,sum(onlinetime) as onlinetime,sum(account) as account from msaaccountreport where sdate>='%s' and sdate<='%s' and userid=%d group by userid",caSdate,caEdate,_userlist[i].userid);
		    printf("SQL%d %s \n",i,sqlBuf);
		    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		    if(tempCursor!=NULL){
				    tempRow= ltDbFetchRow(tempCursor);
			    	while(tempRow!=NULL){
				   		   memset(flow,0,sizeof(flow));
				   		   if(atoll(tempRow[0])>1024000){
		               sprintf(flow,"%0.3fM",atoll(tempRow[0])/1024000.00);
		             }else{
		               sprintf(flow,"%0.3fKB",atoll(tempRow[0])/1024.00);
		             }
							   //jftype计费方式
							   switch(_userlist[i].jftype)
								 {
								      case 0:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("不计费",tempstr,strlen("不计费"));
								      break;
								      case 1:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("包时计费",tempstr,strlen("包时计费"));
								      break;
								      case 2:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("后付流量计费",tempstr,strlen("后付流量计费"));
								      break;
								      case 3:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("后付时间计费",tempstr,strlen("后付时间计费"));
								      break; 
								      case 4:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("预付流量计费",tempstr,strlen("预付流量计费"));
								      break;
								      case 5:
								      memset(tempstr,0,sizeof(tempstr));
      								strgb2utf8("预付流量计费",tempstr,strlen("预付流量计费"));
								      break;
								 }
								 //计费时间
								 nasTimeGetTime(otime,atol(tempRow[1]));
			           lt_dbput_recordvars(tablePtr,8,
		           	 "uname",LT_TYPE_STRING,_userlist[i].dispname,
			           "gname",LT_TYPE_STRING,_userlist[i].groupname,
			           "flow",LT_TYPE_STRING,flow,
				 				 "jftime",LT_TYPE_STRING,otime,
			           "jftype",LT_TYPE_STRING,tempstr,
			           "sum",LT_TYPE_STRING,tempRow[2],
			           "balance",LT_TYPE_STRING,nowaccount,
			           "payments",LT_TYPE_STRING,tempRow[2]);
				    	   tempRow= ltDbFetchRow(tempCursor);
				    }
			      ltDbCloseCursor(tempCursor);
		    }
    }
	  
    lt_dbput_rootvars(dbPtr,1,"sdate",caSdate);
    lt_dbput_rootvars(dbPtr,1,"edate",caEdate);
   	char  *lt_page_content;
	  int   iFd;
	  char  modelpath[128];
	  memset(modelpath,0,sizeof(modelpath));
	  sprintf(modelpath,"%sindex.htm","/app/msa/msa/htmlplt/reportplt/jfreportlt/");
    lt_page_content=ltPltFileParse(modelpath,dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  	}else{
		        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		        sprintf(caLabel,"%sindex.htm",caTempDir);
		        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
		        if(iFd >0) {
		            write(iFd,lt_page_content,strlen(lt_page_content));
		        }
		        close(iFd);
		      	/*生成html格式报告*/
  	}
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);

		chdir(caTempDir);
	  system("tar -cvzf report.tgz *.htm *.gif *.jpg");
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
	  ltMsgFree(ltMsgPk);
	  return 0;
	  
}
