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

#ifndef __SRVLINREPORT_DIR__
#define __SRVLINREPORT_DIR__
#define _srvreport   "/report/srvlinereport"
#define _srvlineplt     "/reportplt/srvlineplt"
#endif

/*
MSA:报告中心
				服务主线
功能:服务主线分析报告创建调用函数
*/
int msaReportSrvLineAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
   	stralloc strTemp;
   	char  			caTmpp[2048];
    char        sqlBuf[1024];
    char        htmlpage[512];
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
		char        *reportname=NULL;
		char        *email=NULL;
		char        *step=NULL;
		char        *groupid=NULL;
		char        *work=NULL;
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
    long        lTime,lTime1,lTime2;
   	long        oldGid,newGid;
    int         iFd,srvindex,gindex,fdwrite,jjj;
    char        oldDate[64];
    char        newDate[64];
    char        srvName[255];
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
		fdwrite=0;
		iFd=-1;
		srvindex=1;
	  lMaxBytes=1;
	  oldGid=-1;
	  newGid=-1;
	  gindex=0;
    memset(oldDate,0,sizeof(oldDate));
    memset(newDate,0,sizeof(newDate));
    memset(srvName,0,sizeof(srvName));
    memset(caTmpp,0,sizeof(caTmpp));
    memset(htmlpage,0,sizeof(htmlpage));

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
	  	step=ltMsgGetVar_s(ltMsgPk,"step");
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"work")){
	  	work=ltMsgGetVar_s(ltMsgPk,"work");
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }

  	strTemp.s=0;
	  jjj=0;
	  if(ltMsgGetVar_s(ltMsgPk,"gid")){
	  	groupid=ltMsgGetVar_s(ltMsgPk,"gid");
			sprintf(caTmpp,"%s","");
			stralloc_cats(&strTemp,caTmpp);
			for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
				if(groupid[gindex]=='1'){
						jjj++;
					  if(jjj==1){
							sprintf(caTmpp,"%d",gindex);
			        stralloc_cats(&strTemp,caTmpp);
	          }else{
	            sprintf(caTmpp,",%d",gindex);
			        stralloc_cats(&strTemp,caTmpp);
	          }
				}
			}
			stralloc_cats(&strTemp,"");
			stralloc_0(&strTemp);
	  }

    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_srvreport,tmpDir);
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
    	  sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_srvreport,fileName);
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
         lTime1 =lTime;
    }
    if(lTime2>lTime){
		 		 lTime2=lTime;
	  }

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
			  sprintf(caLabel,"/bin/cp %s%s/right-top-d.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/right-top.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/banner.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/up-1.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
			  sprintf(caLabel,"/bin/cp %s%s/up-2.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
				sprintf(caLabel,"/bin/cp %s%s/up-3.jpg  %s",_msahtmldir,_srvlineplt,caTempDir);
			  system(caLabel);
   }

    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(atoi(step)==1){						//服务流量报告
      sprintf(sqlBuf,"select service,sum(bytes) as abyte ,sum(onlinetime) as atime from msasrvreport where sdate>='%s' and sdate<='%s' and workflag=%s group by service order by abyte desc ",caSdate1,caSdate2,work);
    }else if(atoi(step)==2){			//部门汇总报告
	    sprintf(sqlBuf,"select service,sum(bytes) as abyte ,sum(onlinetime) as atime,c.id       from msasrvreport a,msauser b,msagroup c where a.userid=b.userid and b.groupid0=c.id and sdate>='%s' and sdate<='%s' and c.id in(%s) and workflag=%s group by service,c.id order by abyte desc",caSdate1,caSdate2,strTemp.s,work);
    }else if(atoi(step)==3){			//时间服务汇总
	    sprintf(sqlBuf,"select service,sum(bytes) as abyte ,sum(onlinetime) as atime,a.sdate      from msasrvreport a,msauser b,msagroup c where a.userid=b.userid and b.groupid0=c.id and sdate>='%s' and sdate<='%s' and c.id in(%s) and workflag=%s group by a.sdate,service  order by abyte desc ",caSdate1,caSdate2,strTemp.s,work);
    }else{												//部门时间汇总
	  	sprintf(sqlBuf,"select a.sdate,service,c.id,sum(bytes) as abyte ,sum(onlinetime) as atime from msasrvreport a,msauser b,msagroup c where a.userid=b.userid and b.groupid0=c.id and sdate>='%s' and sdate<='%s' and c.id in(%s) and workflag=%s group by a.sdate,service,c.id order by abyte desc ",caSdate1,caSdate2,strTemp.s,work);
    }
    printf("sql:%s\n",sqlBuf);
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    	  system(caLabel);
	    	  ltMsgFree(ltMsgPk);
	        return 0;
	  }

	  if(atoi(step)==1){
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
			   			"srvtime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
			   			"srvljflow",LT_TYPE_STRING,strdayflow
		   		  );

					}else if(atoi(step)==2){
							newGid=atol(tempRow[3]);
							printf("newGid:%ld\n",newGid);
							if(atoll(tempRow[1])>1024000){
						      sprintf(strdayflow,"%0.3fM",(atoll(tempRow[1])/1024000.00));
						  }else{
						      sprintf(strdayflow,"%0.3fK",(atoll(tempRow[1])/1024.00));
						  }

							if(newGid==oldGid){
					  	  	lt_dbput_recordvars(tablePtr,4,
									  "srvgname",LT_TYPE_STRING," ",
										"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname,
						   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
						   			"srvljflow",LT_TYPE_STRING,strdayflow
				   		    );
				  	  }else{

				  	  		for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
											if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
												  printf("hello2.........\n");
												  printf("%s\n",_ltPubInfo->_GroupList[newGid].name);
												  printf("%s\n",_ltPubInfo->topSrvName[atol(tempRow[0])].srvname);
												  printf("%s\n",time2str(atol(tempRow[2])));
													lt_dbput_recordvars(tablePtr,4,
													  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
														"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname,
										   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
										   			"srvljflow",LT_TYPE_STRING,strdayflow
								   		    );
											}
									}

				  	  }
							oldGid=atol(tempRow[3]);
					}else if(atoi(step)==3){	//第三步
						  if(atoll(tempRow[1])>1024000){
						      sprintf(strdayflow,"%0.3fM",atoll(tempRow[1])/1024000.00);
						  }else{
						      sprintf(strdayflow,"%0.3fK",atoll(tempRow[1])/1024.00);
						  }
							sprintf(newDate,"%s",tempRow[3]);
							if(strcmp(newDate,oldDate)==0){
								lt_dbput_recordvars(tablePtr,4,
								  "srvdate",LT_TYPE_STRING," ",
									"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname,
					   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
					   			"srvljflow",LT_TYPE_STRING,strdayflow
			   		    );
							}else{
								lt_dbput_recordvars(tablePtr,4,
								  "srvdate",LT_TYPE_STRING,newDate,
									"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[0])].srvname,
					   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
					   			"srvljflow",LT_TYPE_STRING,strdayflow
			   		    );
							}
			   		  sprintf(oldDate,"%s",tempRow[3]);
					}else{										//第四步
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
								   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		    );
						  	  }else{
						  	  	for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
											if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
													 lt_dbput_recordvars(tablePtr,5,
										  	 	  "srvdate",LT_TYPE_STRING,"",
													  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
														"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,
										   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
										   			"srvljflow",LT_TYPE_STRING,strdayflow
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
								   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		    );
						  	  }else{
						  	  	for(gindex=0;gindex<_MAX_ALLGRPNUM;gindex++){
											if(groupid[gindex]=='1' &&_ltPubInfo->_GroupList[gindex].groupid==newGid){
												lt_dbput_recordvars(tablePtr,5,
									  	 	  "srvdate",LT_TYPE_STRING,newDate,
												  "srvgname",LT_TYPE_STRING,_ltPubInfo->_GroupList[newGid].name,
													"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname,
									   			"srvljtime",LT_TYPE_STRING,time2str(atol(tempRow[4])),
									   			"srvljflow",LT_TYPE_STRING,strdayflow
						   		      );
											}
									  }
						  	  }
									oldGid=atol(tempRow[2]);
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
			  sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %ssrvreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);  //柱状 饼图：onlinesrv1.pl
			  system(caLabel);
			  lt_dbput_rootvars(dbPtr,1,"piechart","srvreport.gif" );
	  }


	  if(atoi(step)==1){
	  	lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  }
	  sprintf(htmlpage,"%s%s/srvstep%d.htm",_msahtmldir,_srvlineplt,atoi(step));
	  lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);

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
			  }else{
			  	sprintf(caLabel,"%spage%d.htm",caTempDir,atoi(step));
			  }
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
		    if(iFd >0) {
		          write(iFd,lt_page_content,strlen(lt_page_content));
		    }
	    	close(iFd);
	  }

    if(atoi(step)==4){
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


/*
MSA:报告中心
功能：生成的报告列表显示
*/
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
		sprintf(caCmd,"rm -Rf %s/report/%s/%s",_datacenterdir,reportdir,filedir);
    system(caCmd);
	}


	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"list");
	sprintf(caFile1,"%s/report/%s/",_datacenterdir,reportdir);
  fnames=mydirlist(caFile1,&len);
  if(fnames){
	    qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
  }
  sprintf(flowInfo,"[ ");
  while(len--) {
				  memset(reportname,0,sizeof(reportname));
			    memset(reportuser,0,sizeof(reportuser));
			    memset(cDate,0,sizeof(cDate));
				  sprintf(caFile1,"%s/report/%s/%s/info.ini",_datacenterdir,reportdir,fnames[len].caDirName);
			    bcCnfGetValue_s(caFile1,"date",cDate);
			    bcCnfGetValue_s(caFile1,"reportname",reportname);


				  sprintf(reportstate,"报告已生成");
				  sprintf(caFile1,"%s/report/%s/%s/errmsg.ini",_datacenterdir,reportdir,fnames[len].caDirName);
			    if(stat(caFile1,&st) == 0){
			    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
					  sprintf(reportstate,caFile1);
			    }

			    sprintf(caFile1,"%s/report/%s/%s/proccess.ini",_datacenterdir,reportdir,fnames[len].caDirName);
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
功能：生成的报告列表JSONstore
*/
int msaReportJsonStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	stralloc strTemp;
  char caTmpp[2048];
  char caFile1[256];
 	char cDate[64];
 	char cDate2[64];
	char reportname[64];
	char condition[64];
	char reportuser[64];
	char reportstate[123];
	char templname[256];
	char caCmd[256];
  struct stat st;
  msasDir *fnames;
  char   *reportdir;
  char   *filedir;
  int    jjj;
  int    len=0;

  reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
  if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){
		filedir=ltMsgGetVar_s(ltMsgPk,"file");
		sprintf(caCmd,"rm -Rf %s/report/%s/%s",_datacenterdir,reportdir,filedir);
    system(caCmd);
	}
	sprintf(caFile1,"%s/report/%s/",_datacenterdir,reportdir);
	fnames=mydirlist(caFile1,&len);
  if(fnames){
	    qsort(fnames,len,sizeof(msasDir),dirtimecmp);
  }
	 strTemp.s=0;
	 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
	 stralloc_cats(&strTemp,caTmpp);
	 jjj=0;
 while(len--)
 {
              jjj++;
              memset(condition,0,sizeof(condition));
              memset(reportname,0,sizeof(reportname));
					    memset(reportuser,0,sizeof(reportuser));
					    memset(templname,0,sizeof(templname));
					    memset(reportstate,0,sizeof(reportstate));
					    memset(cDate,0,sizeof(cDate));
					    memset(cDate2,0,sizeof(cDate2));
						  sprintf(caFile1,"%s/report/%s/%s/info.ini",_datacenterdir,reportdir,fnames[len].caDirName);
					    bcCnfGetValue_s(caFile1,"date",cDate);
					    bcCnfGetValue_s(caFile1,"date",cDate2);
					    bcCnfGetValue_s(caFile1,"reportname",reportname);
					    bcCnfGetValue_s(caFile1,"condition",condition);
					    cDate2[10]='\0';
						  sprintf(reportstate,"报告已生成");
						  sprintf(caFile1,"%s/report/%s/%s/errmsg.ini",_datacenterdir,reportdir,fnames[len].caDirName);
					    if(stat(caFile1,&st) == 0){
					    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
							  sprintf(reportstate,caFile1);
					    }
					    sprintf(caFile1,"%s/report/%s/%s/proccess.ini",_datacenterdir,reportdir,fnames[len].caDirName);
					    if(stat(caFile1,&st) == 0){
								sprintf(reportstate,"报告正在生成中");
					    }
					    strgb2utf8(reportstate,templname,strlen(reportstate));
				      sprintf(reportstate,"%s",templname);
              memset(caTmpp,0,sizeof(caTmpp));
							if(jjj==1){
								sprintf(caTmpp,"{\"reportname\":\"%s\",\"dates\":\"%s\",\"reportdir\":\"%s\",\"bfile\":\"%s\",\"reportstate\":\"%s\",\"bydate\":\"%s\",\"condition\":\"%s\"}",reportname,cDate,reportdir,fnames[len].caDirName,reportstate,cDate2,condition);
				        stralloc_cats(&strTemp,caTmpp);
              }else{
                sprintf(caTmpp,",{\"reportname\":\"%s\",\"dates\":\"%s\",\"reportdir\":\"%s\",\"bfile\":\"%s\",\"reportstate\":\"%s\",\"bydate\":\"%s\",\"condition\":\"%s\"}",reportname,cDate,reportdir,fnames[len].caDirName,reportstate,cDate2,condition);
				        stralloc_cats(&strTemp,caTmpp);
              }

 }
 stralloc_cats(&strTemp,"]}");
 stralloc_0(&strTemp);
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
 ltMsgFree(ltMsgPk);
 return 0;

}
