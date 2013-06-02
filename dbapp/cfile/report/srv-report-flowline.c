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

#ifndef __FLOWLINREPORT_DIR__
#define __FLOWLINREPORT_DIR__
#define _flowreport   "/report/flowlinereport"
#define _flowlineplt "/reportplt/flowlineplt"
#endif

/*
MSA:报告中心
				流量主线
功能:流量主线分析报告创建调用函数
*/
int msaReportFlowLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;   
    stralloc strTemp;  
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char        *reportname=NULL;
		char        *email=NULL;
		char        *lt_page_content=NULL;
		char        *gid=NULL;
		char        *work=NULL;
		char        caTempDir[256];  
		char        responseDir[256];
    char        caLabel[256];
    char        htmlpage[256];
    char        sqlBuf[1024]; 
    char        strdayflow[32];
    char        sDate[64],sTime[64];
	  char        caFile1[256];   
    char        caCmd[256];   
    char        atime[64];
    char        srvName[255];
    char 				caTmpp[2048];
    char 				flow[128];
   	long        lTime,lTime1,lTime2;
    int         iFd,srvindex,gindex,fdwrite,step,jjj;
    unsigned long long lMaxBytes;
    unsigned long tmpDir=0;
    unsigned long fileName=0;    
    step=0;
    strTemp.s=0;
	  jjj=0;
    memset(caTempDir,0,sizeof(caTempDir));
    memset(responseDir,0,sizeof(responseDir));
    memset(caLabel,0,sizeof(caLabel));
    memset(htmlpage,0,sizeof(htmlpage));
    memset(sqlBuf,0,sizeof(sqlBuf));
    memset(atime,0,sizeof(atime));
    memset(srvName,0,sizeof(srvName));
   	memset(caTmpp,0,sizeof(caTmpp));
   	memset(flow,0,sizeof(flow));

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
    if(ltMsgGetVar_s(ltMsgPk,"gid")){
	  	gid=ltMsgGetVar_s(ltMsgPk,"gid");
	  		sprintf(caTmpp,"%s","");
				stralloc_cats(&strTemp,caTmpp);		
				int i;					
				for(i=0;i<1000;i++){
					if(gid[i]=='1'){
							jjj++;
						  if(jjj==1){
								sprintf(caTmpp,"%d",i); 
				        stralloc_cats(&strTemp,caTmpp);			
		          }else{
		            sprintf(caTmpp,",%d",i);  
				        stralloc_cats(&strTemp,caTmpp);				
		          }  					
					}	
				}
				stralloc_cats(&strTemp,"");
				stralloc_0(&strTemp); 
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){	  
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }
		 if(ltMsgGetVar_s(ltMsgPk,"work")){	  
	  	work=ltMsgGetVar_s(ltMsgPk,"work");
	  }

    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_flowreport,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);	  
		    if(ltFileIsExist(caTempDir)!=1) {
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				      ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
							ltMsgFree(ltMsgPk);
			        return 0;
		       }
		 		}  
    }else{			
    	  sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_flowreport,fileName);   
    	  sprintf(responseDir,"%lu",fileName);
    }    

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
	  fdwrite=-1;
	  if(step==1){
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
			  sprintf(caLabel,"/bin/cp %s%s/right-top-d.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/right-top.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/banner.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/up-1.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/up-2.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);
				sprintf(caLabel,"/bin/cp %s%s/up-3.jpg  %s",_msahtmldir,_flowlineplt,caTempDir);
			  system(caLabel);    
   }

    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		      
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(step==1){							//服务流量报告
      sprintf(sqlBuf,"select service,sum(onlinetime) as acount,sum(bytes) as abyte from msasrvreport where sdate>='%s' and sdate<='%s' and workflag=%s group by service order by abyte desc ",caSdate1,caSdate2,work); 
    }else if(step==2){				//部门流量排序
    	sprintf(sqlBuf,"select c.name,sum(bytes)   as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by c.name order by abyte desc",caSdate1,caSdate2,strTemp.s,work);    	      	       
    }else if(step==3){				//人员流量排序
			sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by abyte desc",caSdate1,caSdate2,strTemp.s,work);
    }else if(step==4){				//网站流量排序
      sprintf(sqlBuf,"select host,sum(bytes)       as abyte from msauserhttpreport where sdate >= '%s' and sdate <= '%s' group by host order by abyte desc",caSdate1,caSdate2);			   
    }else{										//目的地址流量排序
    	 sprintf(sqlBuf,"select dip,sum(bytes)       as abyte from msaDipReport where sdate >= '%s' and sdate <= '%s' group by dip order by abyte desc ",caSdate1,caSdate2);
    }
	  printf("sql: %s\n",sqlBuf);
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }
		iFd=-1;
	  if(step==1){
		   sprintf(caLabel,"%sdata.ini",caTempDir);
	      iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		    if(iFd == (-1)){
						sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
			    	system(caLabel);
			      ltDbCloseCursor(tempCursor);
			      ltMsgFree(ltMsgPk);
			      return 0;
			 }							
	  }
	  
	  srvindex=1;
	  lMaxBytes=1;
	  gindex=0;
	  dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    tablePtr=lt_dbput_table(dbPtr,"list"); 
	  tempRow= ltDbFetchRow(tempCursor);   
	  while(tempRow!=NULL){	      	                         	  
					if(step==1){										//服务流量报告
		   		  if(atoll(tempRow[2])>1024000){
					      sprintf(strdayflow,"%0.3fM",atoll(tempRow[2])/1024000.00);
					  }else{
					      sprintf(strdayflow,"%0.3fK",atoll(tempRow[2])/1024.00);
					  }			  
					  sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[2]));
	          write(iFd, caLabel, strlen(caLabel));	
	          if(lMaxBytes<atoll(tempRow[2])){
	 		     		 lMaxBytes=atoll(tempRow[2]);
	          }						  	
				  	sprintf(srvName,"No%d:%s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname);				  
						lt_dbput_recordvars(tablePtr,3,
							"srvname",LT_TYPE_STRING,srvName,  
			   			"srvljtime",LT_TYPE_STRING,tempRow[1],
			   			"srvljflow",LT_TYPE_STRING,strdayflow
		   		  ); 
					}else if(step==2){							//部门流量排序
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
             
		  	  	lt_dbput_recordvars(tablePtr,2,
							"groupname",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    ); 	

					}else if(step==3){							//人员流量排序
						
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"uname",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    ); 	
		   		  
					}else if(step==4){							//网站流量排序
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"host",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
					}else{													//目的地址流量排序
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"dip",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
					}

        
	   			if(step==1){
	          	srvindex++;
	        }   			
        tempRow=ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);

    if(step==1){											//画图
			  close(iFd);	 
			  sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %ssrvreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);  //柱状 饼图：onlinesrv1.pl
			  system(caLabel);	
			  lt_dbput_rootvars(dbPtr,1,"piechart","srvreport.gif" );
	  } 

	  
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  sprintf(htmlpage,"%s%s/flow%d.htm",_msahtmldir,_flowlineplt,step);
	  printf("%s",htmlpage);
	  lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
	  if(lt_page_content==NULL){
		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	  		system(caLabel);
	  		ltMsgFree(ltMsgPk);
	  		lt_dbfree(dbPtr);
     		return 0;
		}else{
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


