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
#ifndef __FLOWLINREPORT_DIR__
#define __FLOWLINREPORT_DIR__
#define flowreportdatacenterdir   "/datacenter/msa/report/flowlinereport"
#define flowlineplt               "/app/msa/msa/htmlplt/reportplt/flowlineplt"
#endif
/*******************报告目录相关宏*******************************/

//流量主线分析AJAX提交
int msaReportFlowLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
	  printf("msaReportFlowLineAjaxSubmit.................................\n");
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    
    char        sqlBuf[1024];    
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char        *reportname=NULL;
		char        *email=NULL;
		char        *step=NULL;
		long        gid=-1,uid=-1;
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
    long        onlinetime=0;
    char        atime[64];
    char        srvName[255];
    memset(atime,0,sizeof(atime));
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
    
    
    if(ltMsgGetVar_s(ltMsgPk,"gid")){
	  	gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
	  }
	   printf("groupid:%ld\n",gid);
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
		    sprintf(caTempDir,"%s/%lu/",flowreportdatacenterdir,tmpDir);
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
    	  sprintf(caTempDir,"%s/%lu/",flowreportdatacenterdir,fileName);   
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
         lTime1 = lTime;
    }
    if(lTime2>lTime){
		 		 lTime2=lTime;
	  }
	  
	  printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
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
			  sprintf(caCmd,"reportname=%s\n",reportname);
			  write(fdwrite,caCmd,strlen(caCmd));
			  close(fdwrite);
			  sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/right-top-d.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/right-top.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/banner.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/up-1.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s/up-2.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
				sprintf(caLabel,"/bin/cp %s/up-3.jpg  %s",flowlineplt,caTempDir);
			  system(caLabel);
    
   }
    printf("ffffffffffffffffffffffffffffffffffffffffffffffffff\n");
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		      
    /*************************数据库操作***************************************/  
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(atoi(step)==1){//服务流量报告
      sprintf(sqlBuf,"select service,sum(ubytes+dbytes) as abyte ,sum(ctime) as arequest\
        from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'\
        group by service order by abyte desc ",caSdate1,caSdate2); 
    }else if(atoi(step)==2){//部门流量排序
    	if(gid==-1){
    		sprintf(sqlBuf,"select a.groupid0,sum(ubytes+dbytes) as abyte,sum(onlinetime) as atime,c.name\
	       from msauser a,msasrvreport b,msagroup c\
	       where a.userid=b.userid and c.id =a.groupid0 and substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'\
	       group by groupid0,c.name order by abyte desc",caSdate1,caSdate2);  
    	}else{
    		sprintf(sqlBuf,"select a.groupid0,sum(ubytes+dbytes) as abyte,sum(onlinetime) as atime,c.name\
	       from msauser a,msasrvreport b,msagroup c\
	       where a.userid=b.userid and c.id =a.groupid0 and substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and c.id=%ld\
	       group by groupid0,c.name order by abyte desc",caSdate1,caSdate2,gid);  
    	}	      	      	  
      
    }else if(atoi(step)==3){//人员流量排序
    	printf("dddddddddddddddddddddddddddddddddddddddddddddddddddd\n");
    	if(uid==-1){
    		sprintf(sqlBuf,"select a.userid,a.dispname,sum(ubytes+dbytes) as abyte,sum(onlinetime) as atime\
	      from msauser a,msasrvreport b,msagroup c where a.userid=b.userid and\
	      substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by a.userid,a.dispname order by abyte desc",caSdate1,caSdate2);  
    	}else{
    		sprintf(sqlBuf,"select a.userid,a.dispname,sum(ubytes+dbytes) as abyte,sum(onlinetime) as atime\
	       from msauser a,msasrvreport b,msagroup c where a.userid=b.userid and\
	       substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=%ld\
	       group by a.userid,a.dispname order by abyte desc",caSdate1,caSdate2,uid);  
    	}
	    printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    }else if(atoi(step)==4){//网站流量排序msauserhttpreport
    	if(uid==-1){
    		sprintf(sqlBuf,"select b.host,b.bytes,b.ctime,b.lcount,b.urlsort\
				  from msauser a,msauserhttpreport b\
					where b.sdate >= '%s' and b.sdate <= '%s' and a.userid=b.userid \
		 			order by b.bytes desc",caSdate1,caSdate2);	
    	}else{
    	  sprintf(sqlBuf,"select b.host,b.bytes,b.ctime,b.lcount,b.urlsort\
				  from msauser a,msauserhttpreport b\
					where b.sdate >= '%s' and b.sdate <= '%s' and a.userid=b.userid and uid=%ld\
		 			order by b.bytes desc",caSdate1,caSdate2,uid);	
    	}
		   
    }else{//目的地址流量排序msaDipReport
    	 sprintf(sqlBuf,"select sdate,dip,bytes,ctime,onlinetime\
				  from msaDipReport where sdate >= '%s' and sdate <= '%s'\
		 			order by bytes desc ",caSdate1,caSdate2);
    }

	  printf("sqly: %s\n",sqlBuf);
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }

	  printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
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
	  
	   printf("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	  /**********************将数据显示的HTML页面*********************************/
	  srvindex=1;
	  lMaxBytes=1;
	  gindex=0;
	  dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    tablePtr=lt_dbput_table(dbPtr,"list"); 
	  tempRow= ltDbFetchRow(tempCursor);   
	  while(tempRow!=NULL){	      	                         	  
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
						if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }
           

            onlinetime=atol(tempRow[2]);
            sprintf(atime,"%s",time2str(onlinetime));
						lt_dbput_recordvars(tablePtr,3,
							"gname",LT_TYPE_STRING,tempRow[3],  
			   			"flow",LT_TYPE_STRING,strdayflow,
			   			"time",LT_TYPE_STRING,atime
		   		  );

					}else if(atoi(step)==3){//人员流量排序
						
						if(atoll(tempRow[2])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[2])/1024.00);
					  }
					  
					  onlinetime=atol(tempRow[3]);
            sprintf(atime,"%s",time2str(onlinetime));
					  
					  
						lt_dbput_recordvars(tablePtr,3,
							"uname",LT_TYPE_STRING,tempRow[1],  
			   			"flow",LT_TYPE_STRING,strdayflow,
			   			"time",LT_TYPE_STRING,atime
		   		  );
		   		  
					}else if(atoi(step)==4){//网站流量排序
						if(atoll(tempRow[1])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
					  }
						
						onlinetime=atol(tempRow[2]);
            sprintf(atime,"%s",time2str(onlinetime));
						
						lt_dbput_recordvars(tablePtr,3,
							"webname",LT_TYPE_STRING,tempRow[0],  
			   			"flow",LT_TYPE_STRING,strdayflow,
			   			"time",LT_TYPE_STRING,atime
		   		  );
		   		  
					}else{
						if(atoll(tempRow[2])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[2])/1024.00);
					  }
					  
					  onlinetime=atol(tempRow[4]);
            sprintf(atime,"%s",time2str(onlinetime));
						
						lt_dbput_recordvars(tablePtr,3,
							"dipname",LT_TYPE_STRING,tempRow[1],  
			   			"flow",LT_TYPE_STRING,strdayflow,
			   			"time",LT_TYPE_STRING,atime
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

	  
	  if(atoi(step)==1){
	  	lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/flowlineplt/index.htm",dbPtr->doc,0);
	  }else if(atoi(step)==2){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/flowlineplt/grouppxplt.htm",dbPtr->doc,0);
	  }else if(atoi(step)==3){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/flowlineplt/peoplepxplt.htm",dbPtr->doc,0);
	  }else if(atoi(step)==4){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/flowlineplt/webpxplt.htm",dbPtr->doc,0);
	  }else{
	  	lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportplt/flowlineplt/dippxplt.htm",dbPtr->doc,0);
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
	  printf("AAAAA.................................%s\n",caTempDir);	
	  system(caLabel);    
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);   

	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
	  
	  printf("\nmsaReportSrvLineAjaxSubmit_____End.................................\n");
 
    return 0;
}

