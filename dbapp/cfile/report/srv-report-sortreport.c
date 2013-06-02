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
#include "msa.h"
#define _sortreportplt  "/reportplt/sortreportplt"
#define JAVACMD "/app/ns/java/jdk1.6.0_01/bin/java -Duser.language=zh_CN.UTF-8 -Dfile.encoding=UTF-8 -cp /app/ns/java/poi/poi-3.7-20101029.jar:./ "
#define BINCMD	"/app/msa/bin"


int msasortreportlink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
	 int j;
	 j=0;
   dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
 	 for(j=1;j<121;j++){
 			if(strcmp(_ltPubInfo->ltService[j].srvName,"")!=0){
 				lt_dbput_recordvars(tablePtr,2,
   	     "twosrvid",LT_TYPE_LONG,j,
   	     "twosrvname",LT_TYPE_STRING,_ltPubInfo->ltService[j].srvName);
		  }

	 }
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/report/sort7.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);

	 return 0;
}




/*
MSA:报告中心
部门流量排序
*/
int msasortreport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		char *dbName;
		char *dbUser;
		char *dbPass;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			return 0;
		}
		ltDbConn	*G_DbCon1;
		G_DbCon1=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon1==NULL){
			return 0;
		}

		char *gid=NULL;
		char *work=NULL;
		char *lt_page_content=NULL;
		ltDbCursor *tempCursor;
    LT_DBROW tempRow;
		ltDbCursor *tempCursor1;
    LT_DBROW tempRow1;
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char sqlBuf[4096],dateSourceSql[4096];
    char caSdate[32],caEdate[32];
    char caSdate1[32],caTime1[32];
    char caSdate2[32];
   	char fileDate[32];
    char caTempDir[256];
    char htmlpage[256];
    char caLabel[256];
    char caTmpp[2048];
    char name[256];
    char caLine[2048],tojava[1024],utf8Str[2046],urlsortstr[64];
    char caFile[256];
		long lTime,lTime1,lTime2,lTime0;
		stralloc strTemp;
		long num;
		int datenum;
		datenum=30;
		unsigned    long long lMaxBytes;
		int   jjj;
		int   iFd;
		int   fd;
		int   type;
    int   i;
    int groupby;
    int   index;
		int gtype;
		gtype=0;
		iFd=-1;
		fd=-1;
    type=0;
		lTime=0;
    lTime1=0;
    lTime2=0;
    lTime0=0;
    groupby=0;
    lMaxBytes=1;
    index=1;
    memset(name,0,sizeof(name));
    memset(htmlpage,0,sizeof(htmlpage));
    memset(sqlBuf,0,sizeof(sqlBuf));
    memset(caSdate,0,sizeof(caSdate));
    memset(caEdate,0,sizeof(caEdate));
    memset(caSdate1,0,sizeof(caSdate1));
    memset(caTime1,0,sizeof(caTime1));
    memset(caSdate2,0,sizeof(caSdate2));
    memset(fileDate,0,sizeof(fileDate));
    memset(caTempDir,0,sizeof(caTempDir));
    memset(caLabel,0,sizeof(caLabel));
    memset(caTmpp,0,sizeof(caTmpp));
    tempCursor=NULL;
    tempCursor1=NULL;
		gtype=atoi(ltMsgGetVar_s(ltMsgPk,"gtype"));
 		if(ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"gid")==NULL){
				gid="";
    }else{
    		gid=ltMsgGetVar_s(ltMsgPk,"gid");
    }
    if(ltMsgGetVar_s(ltMsgPk,"work")==NULL){
				work="";
    }else{
    		work=ltMsgGetVar_s(ltMsgPk,"work");
    }
    if(ltMsgGetVar_s(ltMsgPk,"num")==NULL){
				num=0;
    }else{
    		num=atol(ltMsgGetVar_s(ltMsgPk,"num"));
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"datenum")!=NULL){
				datenum=atoi(ltMsgGetVar_s(ltMsgPk,"datenum"));
    }

    if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
    		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"groupby")!=NULL){
    		groupby=atoi(ltMsgGetVar_s(ltMsgPk,"groupby"));
    }
    
		memset(urlsortstr,0,sizeof(urlsortstr));
    if(ltMsgGetVar_s(ltMsgPk,"urlid")!=NULL){
    		if(atoi(ltMsgGetVar_s(ltMsgPk,"urlid"))!=-1){
    			sprintf(urlsortstr," and urlsort=%s",ltMsgGetVar_s(ltMsgPk,"urlid"));
    		}
    }
    if(ltMsgGetVar_s(ltMsgPk,"name")!=NULL){
    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
    }
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
          lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else {
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
		 			lTime2 = lTime;
    }
    strTemp.s=0;
	  jjj=0;
		sprintf(caTmpp,"%s","");
		stralloc_cats(&strTemp,caTmpp);
		if(gtype!=1){
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
		}
		stralloc_cats(&strTemp,"");
		stralloc_0(&strTemp);
    memset(caTempDir,0,sizeof(caTempDir));
    if(type==1){
    	 sprintf(caTempDir,"%s/report/groupflowsort/",_datacenterdir);
    }else if(type==2){
    	 sprintf(caTempDir,"%s/report/userflowsort/",_datacenterdir);
    }else if(type==3){
    	 sprintf(caTempDir,"%s/report/serviceflowsort/",_datacenterdir);
    }else if(type==4){
    	 sprintf(caTempDir,"%s/report/exportratesort/",_datacenterdir);
    }else if(type==5){
    	 sprintf(caTempDir,"%s/report/datetotalflowsort/",_datacenterdir);
    }else if(type==6){
    	 sprintf(caTempDir,"%s/report/siteclasssort/",_datacenterdir);
    }else if(type==7){
    	 sprintf(caTempDir,"%s/report/webaccesssort/",_datacenterdir);
    }

		if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }

		sprintf(caTempDir,"%s%lu/",caTempDir,ltStrGetId());
    mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    memset(caTime1,0,sizeof(caTime1));
    memset(fileDate,0,sizeof(fileDate));
    nasCvtStime(lTime,fileDate,caTime1);
    memset(caLabel,0,sizeof(caLabel));
		sprintf(caLabel,"echo 'date=%s_%s' > %sinfo.ini",fileDate,caTime1,caTempDir);
		system(caLabel);
    sprintf(caLabel,"echo 'reportname=%s' >> %sinfo.ini",name,caTempDir);
    system(caLabel);
    sprintf(caLabel,"echo 'condition=%s-%s' >> %sinfo.ini",caSdate,caEdate,caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);
		sprintf(caLabel,"/bin/cp -f %s/reportplt/dailyreport_ch/banner.jpg %s/reportplt/dailyreport_ch/right-top.jpg %s/reportplt/dailyreport_ch/right-top.jpg %s/reportplt/dailyreport_ch/up-1.jpg %s/reportplt/dailyreport_ch/up-2.jpg %s/reportplt/dailyreport_ch/up-3.jpg %s",_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,caTempDir);
		system(caLabel);
		//创建CSV文件
		sprintf(caFile,"%sreport.csv",caTempDir);
		fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
				 ltMsgFree(ltMsgPk);
				 close(fd);
				 return 0;
		}
		memset(caSdate1,0,sizeof(caSdate1));
		memset(caSdate2,0,sizeof(caSdate2));
	  nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
		memset(sqlBuf,0,sizeof(sqlBuf));
		if(type==1){
			 sprintf(sqlBuf,"select c.name,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select c.name,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==2){
			 sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==3){//服务
			 sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==4){
			 if(groupby==0){
			 	 			sprintf(sqlBuf,"select hours,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by hours order by abyte desc ",caSdate1,caSdate2);
			 }else if(groupby==1){
			 				sprintf(sqlBuf,"select sdate,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by sdate order by abyte desc ",caSdate1,caSdate2);
			 }
		}else if(type==5){
			 sprintf(sqlBuf,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.sdate order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==6){
			 sprintf(sqlBuf,"select urlsort,sum(bytes) as abyte  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by abyte desc ",caSdate1,caSdate2,work);
		}else if(type==7){
			 sprintf(sqlBuf,"select a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s)%s and workflag=%s group by a.host order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,urlsortstr,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id%s and workflag=%s group by a.host order by abyte desc limit %ld offset 0",caSdate1,caSdate2,urlsortstr,work,num);
			 }
		}
		printf("SORT SQL %s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/
			return 0;
		}
		if(type==3||type==4){
	     sprintf(caLabel,"%sdata.ini",caTempDir);
       iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	     if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		      ltDbCloseCursor(tempCursor);
		      ltMsgFree(ltMsgPk);
		      return 0;
		   }
	  }

		dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    char srvStr[64];
    char flow[64];
    memset(srvStr,0,sizeof(srvStr));
    memset(flow,0,sizeof(flow));
	  tempRow=ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){
	  	  if(type==1){//部门流量排序
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));

		  	  	lt_dbput_recordvars(tablePtr,2,
							"groupname",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
	  	  }else if(type==2){//人员流量排序
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"uname",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
	  	  }else if(type==3){//服务流量排序
	  	  		sprintf(srvStr,"NO.%d",index);
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  lt_dbput_recordvars(tablePtr,3,
	  	  	  	"index",LT_TYPE_STRING,srvStr,
							"sname",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );

	  	  	  sprintf(caLabel,"%d,%lld\r\n",index,atoll(tempRow[1]));
						write(iFd, caLabel, strlen(caLabel));
						if(lMaxBytes<atoll(tempRow[1])){
								lMaxBytes=atoll(tempRow[1])+10;
						}
	  	  }else if(type==4){//出口速率分布
	  	  	  if(groupby==0){//小时
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }else if(groupby==1){//天
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%lld",
											tempRow[0],
											atoll(tempRow[1])/60
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  sprintf(caLabel,"%s,%lld\r\n",tempRow[0],atoll(tempRow[1])/60);
						write(iFd, caLabel, strlen(caLabel));
						if(lMaxBytes<atoll(tempRow[1])/60){
								lMaxBytes=atoll(tempRow[1])/60+10;
						}
	  	  }else if(type==5){
	  	  		  if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s|%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	   lt_dbput_recordvars(tablePtr,2,
			   			"lbyte",LT_TYPE_STRING,flow,
			   			"date",LT_TYPE_STRING,tempRow[0]
	   		     );
	  	  }else if(type==6){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }

							memset(caLine,0,sizeof(caLine));
							sprintf(caLine,"%s,%s",
												tempRow[0],
												flow
							);
							write(fd,caLine,strlen(caLine));
							write(fd,"\r\n",strlen("\r\n"));
	   		      lt_dbput_recordvars(tablePtr,2,
	  	  	    "urlsort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"lbyte",LT_TYPE_STRING,flow
	   		      );
	  	  }else if(type==7){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	           }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	           }
	           memset(caLine,0,sizeof(caLine));
						 sprintf(caLine,"%s,%s,%s,%s",
											tempRow[0],
											flow,
											tempRow[2],
											tempRow[3]
						 );
						 write(fd,caLine,strlen(caLine));
						 write(fd,"\r\n",strlen("\r\n"));
	  	  	   lt_dbput_recordvars(tablePtr,4,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow,
			   			"lcount",LT_TYPE_STRING,tempRow[2],
			   			"ctime",LT_TYPE_STRING,tempRow[3]
	   		     );
	  	  }
	  	  if(type==3){
					index++;
				}

    		tempRow=ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    close(fd);
    switch(type){
	      case 1:
	      strgb2utf8("部门流量排序 部门名称 流量",utf8Str,strlen("部门流量排序 部门名称 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA %s\n",tojava);
				system(tojava);
	      break;
	      case 2:
	      strgb2utf8("人员流量排序 用户名称 流量",utf8Str,strlen("人员流量排序 用户名称 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 3:
	      strgb2utf8("服务流量排序 服务名称 流量",utf8Str,strlen("服务流量排序 服务名称 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 4:
	      strgb2utf8("出口速率分布 时间 速率",utf8Str,strlen("出口速率分布 时间 速率"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s(kbps)",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 5:
	      strgb2utf8("日期流量排序 日期 流量",utf8Str,strlen("日期流量排序 日期 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 6:
	      strgb2utf8("网站分类排序 网站类型 流量",utf8Str,strlen("网站分类排序 网站类型 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 7:
	      strgb2utf8("网址访问排序 网址 流量 点击次数 连接时间",utf8Str,strlen("网址访问排序 网址 流量 点击次数 连接时间"));
				sprintf(tojava,"%s sortWebExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				system(tojava);
	      break;
		}


		lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
		if(type==3){
			close(iFd);
      sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
	    system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"piechart","srvreporta.gif" );
	  }
	  if(type==4){
			close(iFd);
      sprintf(caLabel,"%s/Scripts/outflowpltline.pl  %sdata.ini %sflowline.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
	    system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"flowline","flowline.gif" );
	  }
		sprintf(htmlpage,"%s%s/sortplt%d.htm",_msahtmldir,_sortreportplt,type);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		if(lt_page_content==NULL){
				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
		    system(caLabel);
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
		}
		chdir(caTempDir);
		//Date source 查询
		sprintf(caFile,"%ssource.csv",caTempDir);
		fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
				 ltMsgFree(ltMsgPk);
				 close(fd);
				 return 0;
		}
		if(type==1){
			 sprintf(dateSourceSql,"select c.name,c.id from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name,c.id order by c.id desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select c.name,c.id from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name,c.id  order by c.id desc",caSdate1,caSdate2,work);
			 }
		}else if(type==2){
			 sprintf(dateSourceSql,"select a.userid,b.dispname from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by a.userid desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select a.userid,b.dispname from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by a.userid desc",caSdate1,caSdate2,work);
			 }
		}else if(type==3){//服务
			 sprintf(dateSourceSql,"select b.username,a.userid from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.username,a.userid order by a.userid desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select b.username,a.userid from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.username,a.userid order by a.userid desc",caSdate1,caSdate2,work);
			 }
		}else if(type==4){
			 if(groupby==0){
			 	 			sprintf(dateSourceSql,"select hours,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by hours order by abyte desc ",caSdate1,caSdate2);
			 }else if(groupby==1){
			 				sprintf(dateSourceSql,"select sdate,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by sdate order by abyte desc ",caSdate1,caSdate2);
			 }
		}else if(type==5){
			 //sprintf(dateSourceSql,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by abyte desc",caSdate1,caSdate2,work);
			 sprintf(dateSourceSql,"select a.sdate from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by a.sdate ASC",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select a.sdate from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by a.sdate ASC",caSdate1,caSdate2,work);
			 }
		}else if(type==6){
			 sprintf(dateSourceSql,"select urlsort,sum(bytes) as abyte  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by abyte desc ",caSdate1,caSdate2,work);
		}else if(type==7){
			 sprintf(dateSourceSql,"select b.dispname,a.userid from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s)%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.userid order by b.dispname desc",caSdate1,caSdate2,strTemp.s,urlsortstr,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select b.dispname,a.userid from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s'%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.userid order by b.dispname desc",caSdate1,caSdate2,urlsortstr,work);
			 }
		}

		memset(srvStr,0,sizeof(srvStr));
    memset(flow,0,sizeof(flow));
    printf("\n DateSourceSql  %s\n",dateSourceSql);
    tempCursor = ltDbOpenCursor(G_DbCon,dateSourceSql);
	  tempRow=ltDbFetchRow(tempCursor);
	  char sTime[64];
	  while(tempRow!=NULL){
	  	  if(type==1){//部门流量排序
		       	memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s, , , , , ,",tempRow[0]);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
						
						memset(dateSourceSql,0,sizeof(dateSourceSql));
						sprintf(dateSourceSql,"select b.dispname,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0=%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,tempRow[1],work,datenum);
						printf("\n DateSourceSql  %s\n",dateSourceSql);
						
						
						//二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						ltDbCloseCursor(tempCursor1);
	  	  }else if(type==2){//人员流量排序
	  	  		memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s, , , , , ,",tempRow[1]);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
						
						memset(dateSourceSql,0,sizeof(dateSourceSql));
						sprintf(dateSourceSql,"select c.name,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by c.name,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[0],datenum);
						if(gtype==1){
								memset(dateSourceSql,0,sizeof(dateSourceSql));
								sprintf(dateSourceSql,"select c.name,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by c.name,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[0],datenum);
						}
						printf("\n DateSourceSql  %s\n",dateSourceSql);
						 //二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						 ltDbCloseCursor(tempCursor1);
	  	  }else if(type==3){//服务流量排序
	  	  	 memset(caLine,0,sizeof(caLine));
					 sprintf(caLine,"%s, , , , , ,",tempRow[0]);
					 write(fd,caLine,strlen(caLine));
					 write(fd,"\r\n",strlen("\r\n"));
					 
		  	   memset(dateSourceSql,0,sizeof(dateSourceSql));
		  	   sprintf(dateSourceSql,"select a.service,sum(bytes) as flow,c.name,a.sdate,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.service,c.name,a.sdate order by flow desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[1],datenum);
			 		 if(gtype==1){
							memset(dateSourceSql,0,sizeof(dateSourceSql));
							sprintf(dateSourceSql,"select a.service,sum(bytes) as flow,c.name,a.sdate,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.service,b.username,c.name,a.sdate order by flow desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[1],datenum);
			 		 }
					 printf("\n DateSourceSql  %s\n",dateSourceSql);
					 //二次查询
				   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
					 tempRow1=ltDbFetchRow(tempCursor1);
					 while(tempRow1!=NULL){
					 			 if(atoll(tempRow1[1])>1024000){
             			 sprintf(flow,"%0.3fM",atoll(tempRow1[1])/1024000.00);
			           }else{
			               sprintf(flow,"%0.3fKB",atoll(tempRow1[1])/1024.00);
			           }
			           memset(caLine,0,sizeof(caLine));
			           memset(sTime,0,sizeof(sTime));
		             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
						     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
						     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
								 sprintf(caLine," ,%s,%s,%s,%s,%s",
													_ltPubInfo->topSrvName[atoi(tempRow1[0])].srvname,
													tempRow1[2],
													flow,
													tempRow1[3],
													sTime
								 );
								 write(fd,caLine,strlen(caLine));
								 write(fd,"\r\n",strlen("\r\n"));
								 tempRow1=ltDbFetchRow(tempCursor1);
					 }
					 ltDbCloseCursor(tempCursor1);
	  	  }else if(type==4){//出口速率分布
	  	  	  if(groupby==0){//小时
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }else if(groupby==1){//天
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%lld",
											tempRow[0],
											atoll(tempRow[1])/60
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  }else if(type==5){
	  	  		memset(caLine,0,sizeof(caLine));
					  sprintf(caLine,"%s, , , , , ,",tempRow[0]);
					  write(fd,caLine,strlen(caLine));
					  write(fd,"\r\n",strlen("\r\n"));
					 
			  	  memset(dateSourceSql,0,sizeof(dateSourceSql));
			  	  sprintf(dateSourceSql,"select b.dispname,c.name,a.service,sum(bytes) as flow,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.sdate='%s' group by b.dispname,c.name,a.service order by flow desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[0],datenum);
				 		if(gtype==1){
							memset(dateSourceSql,0,sizeof(dateSourceSql));
							sprintf(dateSourceSql,"select b.dispname,c.name,a.service,sum(bytes) as flow,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.sdate='%s' group by b.dispname,c.name,a.service order by flow desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[0],datenum);
				 		}
						printf("\n DateSourceSql  %s\n",dateSourceSql);
						//二次查询
					  tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						tempRow1=ltDbFetchRow(tempCursor1);
						while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
	             			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						}
						ltDbCloseCursor(tempCursor1);
	  	  }else if(type==6){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }

							memset(caLine,0,sizeof(caLine));
							sprintf(caLine,"%s,%s",
												tempRow[0],
												flow
							);
							write(fd,caLine,strlen(caLine));
							write(fd,"\r\n",strlen("\r\n"));
	  	  }else if(type==7){
	  	  		 memset(caLine,0,sizeof(caLine));
						 sprintf(caLine,"%s, , , , , ,",tempRow[0]);
						 write(fd,caLine,strlen(caLine));
						 write(fd,"\r\n",strlen("\r\n"));
						 
			  	   memset(dateSourceSql,0,sizeof(dateSourceSql));
			  	   sprintf(dateSourceSql,"select c.name,a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s)%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.host,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,urlsortstr,work,tempRow[1],datenum);
						 if(gtype==1){
								memset(dateSourceSql,0,sizeof(dateSourceSql));
								sprintf(dateSourceSql,"select c.name,a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s'%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.host,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,urlsortstr,work,tempRow[1],datenum);
						 }
						 printf("\n DateSourceSql  %s\n",dateSourceSql);
						 //二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[2])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[2])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[2])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[1],
														tempRow1[0],
														flow,
														tempRow1[3],
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						 ltDbCloseCursor(tempCursor1);
	  	  }
    		tempRow=ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    close(fd);
		switch(type){
	      case 1:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,1);
				chdir(BINCMD);
				printf("JAVA %s\n",tojava);
				system(tojava);
	      break;
	      case 2:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,2);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 3:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,3);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 4:
	      strgb2utf8("出口速率分布 时间 速率",utf8Str,strlen("出口速率分布 时间 速率"));
				sprintf(tojava,"%s sortReportExcel %s %ssource.xls %s(kbps)",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 5:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,5);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 6:
	      strgb2utf8("网站分类排序 网站类型 流量",utf8Str,strlen("网站分类排序 网站类型 流量"));
				sprintf(tojava,"%s sortReportExcel %s %ssource.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 7:
			  sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,7);
			  chdir(BINCMD);
			  system(tojava);
	      break;
		}
		printf("JAVA   %s\n",tojava);
		chdir(caTempDir);
    system("rm -f report.tgz");
	  system("tar -cvzf report.tgz *.jpg *.gif *.htm");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
    ltDbClose(G_DbCon);
    ltDbClose(G_DbCon1);
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
    return 0;
}
