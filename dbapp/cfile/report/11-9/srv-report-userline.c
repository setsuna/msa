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

#ifndef __USERLINREPORT_DIR__
#define __USERLINREPORT_DIR__
#define _userreport   "/report/userlinereport"
#define _userlineplt  "/reportplt/userlineplt"
#endif


/*
MSA:报告中心
				用户主线
功能:主线报告创建调用函数
*/
int msaReportUserLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;  
   	stralloc strTemp;
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char *reportname,*email,*lt_page_content,*work,*gid;
		char sqlBuf[1024];  
		char htmlpage[256];
		char caTempDir[256]; 
		char responseDir[256]; 
    char caLabel[256];
    char flow[128];
    char caTmpp[2048];
    char sDate[64],sTime[64],caFile1[256],caCmd[256];
    unsigned long long lMaxBytes;
    unsigned long tmpDir,fileName;
    long lTime,lTime1,lTime2;
    int  iFd,fdwrite,srvindex,jjj,step;
  	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  gid=NULL;
		reportname=NULL;
		email=NULL;
		lt_page_content=NULL;
		work=NULL;
	  lTime=lTime1=lTime2=0;
	  iFd=fdwrite=step=-1;
	  tmpDir=fileName=0;
	  lMaxBytes=1;  	  
	  strTemp.s=0;
	  jjj=0;
	  memset(sqlBuf,0,sizeof(sqlBuf));
	  memset(htmlpage,0,sizeof(htmlpage));
	  memset(caTempDir,0,sizeof(caTempDir));
	  memset(responseDir,0,sizeof(responseDir));
	 	memset(caLabel,0,sizeof(caLabel));
	 	memset(caCmd,0,sizeof(caCmd));
	 	memset(caFile1,0,sizeof(caFile1));
	 	memset(caTmpp,0,sizeof(caTmpp));
	  
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
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){	  
	  	 fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
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
	  
	  if(ltMsgGetVar_s(ltMsgPk,"work")){
	  	 work=ltMsgGetVar_s(ltMsgPk,"work");
	  }
	  
    
    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_userreport,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);	
		    if(ltFileIsExist(caTempDir)!=1){
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				      ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
							ltMsgFree(ltMsgPk);
			        return 0;
       		 }
    		} 
    }else{							
    	  sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_userreport,fileName);   
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
	  sprintf(caLabel,"/bin/cp %s%s/right-top-d.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/cp %s%s/right-top.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/banner.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-1.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-2.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
  	sprintf(caLabel,"/bin/cp %s%s/up-3.jpg  %s",_msahtmldir,_userlineplt,caTempDir);
    system(caLabel);
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);		   
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(step==1){							//部门排序
      	 sprintf(sqlBuf,"select c.name,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by c.name order by abyte desc",caSdate1,caSdate2,strTemp.s,work);
    }else if(step==2){				//人员流量排序				
      	 sprintf(sqlBuf,"select b.dispname,sum(bytes)      as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by abyte desc",caSdate1,caSdate2,strTemp.s,work);
    }else if(step==3){				//人员时间排序				
      	 sprintf(sqlBuf,"select b.dispname,sum(onlinetime) as atime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by atime desc",caSdate1,caSdate2,strTemp.s,work);
    }else if(step==4){				//流量前30名的的服务访问情况			
      	 sprintf(sqlBuf,"select a.service,sum(bytes)       as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.service order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,30);
    }else if(step==5){				//流量前30名的网站访问情况			
      	 sprintf(sqlBuf,"select b.dispname,c.name,a.host,sum(bytes) as abyte,sum(ctime) as atime,a.urlsort from msauserhttpreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname,c.name,a.host,a.urlsort order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,30);
    }else if(step==6){				//服务部门日期汇总			
      	 sprintf(sqlBuf,"select a.sdate,a.service,c.id,  sum(bytes) as abyte,sum(onlinetime) as atime from msasrvreport a,msauser b,msagroup c where a.userid=b.userid and b.groupid0=c.id and sdate>='%s' and sdate<='%s' and c.id in(%s) and workflag=%s group by a.sdate,service,c.id order by abyte desc ",caSdate1,caSdate2,strTemp.s,work);    	
    }
    printf("sql:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk); 
	        return 0;
	  }
	  
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
	  char  srvStr[64];
	  char  oldDate[64];
    char  newDate[64];
    int   index,gindex; 
	  long  oldGid,newGid;
	  gindex=0;
    oldGid=-1;
	  newGid=-1;
	  index=1;
	  memset(oldDate,0,sizeof(oldDate));
    memset(newDate,0,sizeof(newDate));
    memset(srvStr,0,sizeof(srvStr));
    memset(flow,0,sizeof(flow));
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"list");
	  tempRow=ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){	      			
					if(step==1){								//部门排序
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
             
		  	  	lt_dbput_recordvars(tablePtr,2,
							"groupname",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    ); 	
					}else if(step==2){					//人员流量排序		
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"uname",LT_TYPE_STRING,tempRow[0],  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    ); 	
					}else if(step==3){					//人员时间排序	
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"uname",LT_TYPE_STRING,tempRow[0],  
			   			"onlinetime",LT_TYPE_STRING,time2str(atoi(tempRow[1]))
	   		    ); 	
					}else if(step==4){					//流量前30名的的服务访问情况
						if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"sname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname,  
			   			"lbyte",LT_TYPE_STRING,flow
	   		    ); 	
					}else if(step==5){					//流量前30名的网站访问情况	
						sprintf(srvStr,"NO.%d",index); 	
						lt_dbput_recordvars(tablePtr,6,
							"index",LT_TYPE_STRING,srvStr,  
			   			"gname",LT_TYPE_STRING,tempRow[1],
			   			"uname",LT_TYPE_STRING,tempRow[0],  
			   			"host", LT_TYPE_STRING,tempRow[2],
			   			"urlsort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[5])].srvName,  
			   			"onlinetime",LT_TYPE_STRING,time2str(atoi(tempRow[4]))
	   		    ); 		   		    
						index++;
					}else if(step==6){					//服务部门日期汇总	
							 sprintf(newDate,"%s",tempRow[0]);
							 newGid=atol(tempRow[2]);
							 if(atoll(tempRow[3])>1024000){
						      sprintf(flow,"%0.3fM",atoll(tempRow[3])/1024000.00);
						   }else{
						      sprintf(flow,"%0.3fK",atoll(tempRow[3])/1024.00);
						   }
							 if(strcmp(newDate,oldDate)==0){			    
								  if(newGid==oldGid){							  
							  	  	lt_dbput_recordvars(tablePtr,5,
							  	  	  "srvdate",LT_TYPE_STRING,"",
											  "srvgname",LT_TYPE_STRING,"",
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
								   			"srvljflow",LT_TYPE_STRING,flow
						   		    );
						  	  }else{	
						  	  	for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
											if(gid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
													 lt_dbput_recordvars(tablePtr,5,
										  	 	  "srvdate",LT_TYPE_STRING,"",
													  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
														"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
										   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
										   			"srvljflow",LT_TYPE_STRING,flow
								   		      );  
											}	
									  }				  	
						  	  }												
									oldGid=atol(tempRow[2]);
							 }else{
							    if(newGid==oldGid){							  
							  	  	lt_dbput_recordvars(tablePtr,5,
							  	  	  "srvdate",LT_TYPE_STRING,newDate,
											  "srvgname",LT_TYPE_STRING," ",
												"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
								   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
								   			"srvljflow",LT_TYPE_STRING,flow
						   		    );
						  	  }else{
						  	  	for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
											if(gid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
												lt_dbput_recordvars(tablePtr,5,
									  	 	  "srvdate",LT_TYPE_STRING,newDate,
												  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
													"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,  
									   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
									   			"srvljflow",LT_TYPE_STRING,flow
						   		      );  
											}	
									  }					  	
						  	  }												
									oldGid=atol(tempRow[2]);						  
							}									
	            sprintf(oldDate,"%s",tempRow[0]);
					}
					
          tempRow=ltDbFetchRow(tempCursor); //移动光标
    }
    ltDbCloseCursor(tempCursor);  
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
  	sprintf(htmlpage,"%s%s/userflow%d.htm",_msahtmldir,_userlineplt,step);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);

	  if(lt_page_content==NULL){
		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	  		system(caLabel);										/*建立目录和正在处理标志*/
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

