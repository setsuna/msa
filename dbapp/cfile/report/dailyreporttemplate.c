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
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <dirent.h>
#include <stdarg.h>
#include "msa.h"

#define _dailyreport 		"/report/dailyreport"
#define _dailygrpreport	"/report/dailygrpreport"
#define _instantreport	"/report/instantreport"
#define _dailyreport_ch	"/reportplt/dailyreport_ch"
#define _dailyreport_en	"/reportplt/dailyreport_en"



/*全部变量*/
stralloc strTemp;
char  element[256];
char  checkb[20];
char  reportname[120];
char  email[64];
char  language[8];
char  topusernum[32];
char  caSdates[32],caEdates[32];
char  worktime[64];
char  groupstr[1000];
char  caTmpp[2048];
char  *dbUser;
char  *dbPass;
char  *dbName;

int   lang;
int   work;
int   jjj;
int   deployscope;
int   inttopusernum;
int   xgid[1000];


void catch_alarm(int signum)
{
	_exit(0);
}



/* 统计数据 */
int dailyReport(long tempIndex)
{
		ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32],caTime1[32],caTime2[32],caTime[32];
    char caTempDir[256];
    char caTempFile[256];
    char htmlpage[512];
    char reportnames[64];
    char caLabel[256];
    //char caTemp[18192];
		char fileDate[32];
		char caItem[128];
		char oldgname[128];
		char egname[128];
		char G_sdate[32];
		char sqlBuf[1024];
		char *lt_page_content=NULL;
		char userNumber[64];
    unsigned long iMaxItem;
    unsigned long long lUbytes,lDbytes,lMaxBytes,lMax;
    long lTime,lTime1,lTime2,lTime0;
    int  iFd,lStep,iTemp,i,iMax;

    lTime=0;
    lTime1=0;
    lTime2=0;
    lTime0=0;
    lMaxBytes=0;
    lUbytes=0;
    lDbytes=0;
    memset(sqlBuf,0,sizeof(sqlBuf));
    memset(G_sdate,0,sizeof(G_sdate));
    memset(caSdate,0,sizeof(caSdate));
    memset(caEdate,0,sizeof(caEdate));
    memset(caSdate2,0,sizeof(caSdate2));
    memset(caTime2,0,sizeof(caTime2));
    memset(htmlpage,0,sizeof(htmlpage));
    memset(userNumber,0,sizeof(userNumber));
    tempCursor=NULL;
    sprintf(userNumber,"%d",inttopusernum);

		if(deployscope==0){				//每天
				lTime = time(0)-3600*24 ;
				nasTimeGetDate(caSdate,lTime);
				sprintf(G_sdate,"%s",caSdate);
				sprintf(caEdate,"%s",caSdate);
				sprintf(reportnames,"%s-%s-daily",reportname,G_sdate);
				lTime = time(0);
				lTime1 = nasCvtLongTime(caSdate,"00:00:01");
				lTime2 = nasCvtLongTime(caSdate,"23:59:59");
		}

		if(deployscope==1){				//每周
		    lTime = time(0);
	    	nasTimeGetDate(caSdate,lTime-3600*24*6);
	    	nasTimeGetDate(caEdate,lTime);
	    	sprintf(caSdate,"%s",caSdate);
	    	sprintf(caEdate,"%s",caEdate);
	    	sprintf(G_sdate,"%s",caEdate);
	    	lTime1 = nasCvtLongTime(caSdate,"00:00:01");
				lTime2 = nasCvtLongTime(caEdate,"23:59:59");
				sprintf(reportnames,"%s-%s-weekly",reportname,G_sdate);
		}
		if(deployscope==2){			//立即生成
				lTime = time(0);
				sprintf(caSdate,"%s",caSdates);
				sprintf(caEdate,"%s",caEdates);
			  sprintf(reportnames,"%s",reportname);
				lTime1 = nasCvtLongTime(caSdate,"00:00:01");
				lTime2 = nasCvtLongTime(caEdate,"23:59:59");
				nasTimeGetDate(caSdate,lTime1);
				nasTimeGetDate(caEdate,lTime2);
				sprintf(G_sdate,"%s",caSdate);
		}
		

    if(deployscope==0){		//每天
			sprintf(caTempDir,"%s%s/%ld/",_datacenterdir,_dailyreport,tempIndex);
		}
		if(deployscope==1){		//每周
			sprintf(caTempDir,"%s%s/%ld/",_datacenterdir,_dailygrpreport,tempIndex);
		}
		if(deployscope==2){		//手动
	    sprintf(caTempDir,"%s%s/%ld/",_datacenterdir,_instantreport,tempIndex);
		}


    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }

		ltStrAddF(caTempDir,"%lu/",ltStrGetId());
    mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    nasCvtStime(lTime1,caSdate1,caTime1);
    caTime1[5]=0;
    if(deployscope==1){				//每周
				nasCvtStime(lTime2,caSdate2,caTime2);
				sprintf(caLabel,"echo 'date=%s %s' > %sinfo.ini",caSdate2,caTime2,caTempDir);
				system(caLabel);
    }else if(deployscope==0){ //每天
				nasTimeGetDate(fileDate,time(0));
			  nasCvtStime(time(0),fileDate,caTime1);
				sprintf(caLabel,"echo 'date=%s %s' > %sinfo.ini",fileDate,caTime1,caTempDir);
				system(caLabel);
    }else{
		    nasTimeGetDate(fileDate,lTime);
		    nasCvtStime(lTime,fileDate,caTime1);
				sprintf(caLabel,"echo 'date=%s %s' > %sinfo.ini",fileDate,caTime1,caTempDir);
				system(caLabel);
    }

    sprintf(caLabel,"echo 'name=%s' >> %sinfo.ini",reportnames,caTempDir);
    system(caLabel);
    sprintf(caLabel,"echo 'user=system' >> %sinfo.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);

	  if(lang==0){			//ch
			sprintf(caLabel,"/bin/cp -f %s%s/*.gif %s",_msahtmldir,_dailyreport_ch,caTempDir);
			system(caLabel);
			sprintf(caLabel,"/bin/cp -f %s%s/*.jpg %s",_msahtmldir,_dailyreport_ch,caTempDir);
			system(caLabel);
		}else if(lang==1){//en
			sprintf(caLabel,"/bin/cp -f %s%s/*.gif %s",_msahtmldir,_dailyreport_en,caTempDir);
			system(caLabel);
			sprintf(caLabel,"/bin/cp -f %s%s/*.jpg %s",_msahtmldir,_dailyreport_en,caTempDir);
			system(caLabel);
		}


    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate);
    if(lang==0){			//ch
	    	sprintf(htmlpage,"%s%s/dailyreportpart1.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}else if(lang==1){//en
				sprintf(htmlpage,"%s%s/dailyreportpart1.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}

    if(lt_page_content==NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
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
		lt_dbfree(dbPtr);

		if(element[0]=='1'){                    //设备状态
				FILE *fp;
				char *p;
				char caBuffer[132],caDev[32],caRate[32],caMount[32],caLine[2048];
				unsigned long long rootdiskinfo,appdiskinfo,lTotal,lUsed,lFree;
				p=NULL;
				fp=NULL;
				rootdiskinfo=0;
				appdiskinfo=0;
				lTotal=0;
				lUsed=0;
				lFree=0;
				memset(caLine,0,sizeof(caLine));

				dbPtr=lt_dbinit();
				lt_db_htmlpage(dbPtr,"utf-8");
		  	tablePtr=lt_dbput_table(dbPtr,"llista");
				fp = popen("/sbin/df -k","r");
				if(fp){
						p = fgets(caBuffer,131,fp);
						p = fgets(caBuffer,131,fp);
						while(p){
								sscanf(caBuffer,"%s %lld %lld %lld %s %s",caDev,&lTotal,&lUsed,&lFree,caRate,caMount);
								if(strcmp(caMount,"/")==0) {
									rootdiskinfo=(lUsed*100)/lTotal;
									sprintf(caLine,"<tr>\r\n<td>%s</td><td>%s</td><td>%lldKb</td><td>%lldKb</td><td>%lldKb</td><td>%s</td>\r\n</tr>",caDev,caMount,lTotal,lUsed,lFree,caRate);
									lt_dbput_recordvars(tablePtr,1,"diskinfo",LT_TYPE_STRING,caLine);
								}
								if(strcmp(caMount,"/app")==0) {
									appdiskinfo=(lUsed*100)/lTotal;
									sprintf(caLine,"<tr>\r\n<td>%s</td><td>%s</td><td>%lldKb</td><td>%lldKb</td><td>%lldKb</td><td>%s</td>\r\n</tr>",	caDev,caMount,lTotal,lUsed,lFree,caRate);
									lt_dbput_recordvars(tablePtr,1,"diskinfo",LT_TYPE_STRING,caLine);
									break;
								}
								p = fgets(caBuffer,132,fp);
						}
						if(pclose(fp) == -1){
						  return -1;
						}
				}

				if(lang==0){      //ch
					sprintf(htmlpage,"%s%s/dailyreportpart2.htm",_msahtmldir,_dailyreport_ch);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}else if(lang==1){//en
					sprintf(htmlpage,"%s%s/dailyreportpart2.htm",_msahtmldir,_dailyreport_en);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}

				if(lt_page_content==NULL){
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    		system(caLabel);
					return 0;
				}else{
			    lt_page_content=strstr(lt_page_content,"<tr>");
			    sprintf(caLabel,"%sindex.htm",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
					if(iFd >0) {
						write(iFd,lt_page_content,strlen(lt_page_content));
					}
					close(iFd);
				}
	    	lt_dbfree(dbPtr);

   	}


  	if(element[1]=='1'){										//网络出口流量
			    struct nasFlowCount_s {
									char          		 caItem[16];
									unsigned long long lDbytes;
									unsigned long long lUbytes;
									unsigned long long lTbytes;
					}sCount[35];
					memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
					lTime1 = nasCvtLongTime(caSdate,"00:00:01");
					lTime2 = nasCvtLongTime(caEdate,"23:59:59");
					if(strcmp(caSdate,caEdate)==0){
							lStep = 3600;
							if(lTime2>time(0)){
							   lTime2=time(0);
							}
							lTime0 = lTime1;
							memset(caSdate1,0,sizeof(caSdate1));
							memset(caSdate2,0,sizeof(caSdate2));
							nasTimeGetDate(caSdate1,lTime1);
							nasTimeGetDate(caSdate2,lTime2);
							
							sprintf(sqlBuf,"select sdate,hours,avg(ubytes),avg(dbytes)from msatimereport where sdate>='%s' and sdate<='%s' group by sdate,hours order by sdate asc,hours asc",caSdate1,caSdate2);
							tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
							if(tempCursor == NULL) {
								sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
								system(caLabel);
							  return 0;
							}
							lMaxBytes = 1;
							iMax = 0;
							tempRow=ltDbFetchRow(tempCursor);
							while(tempRow!=NULL){
								sprintf(caItem,"%s %s:00",tempRow[0],tempRow[1]);
								lUbytes=atoll(tempRow[2])/60;
								lDbytes=atoll(tempRow[3])/60;
								if(lMaxBytes <= (lUbytes + lDbytes)){
								   lMaxBytes  = (lUbytes + lDbytes);
								}
								sprintf(sCount[iMax].caItem,"%s",caItem);
								sCount[iMax].lUbytes = lUbytes ;
								sCount[iMax].lDbytes = lDbytes ;
								sCount[iMax].lTbytes = lUbytes+lDbytes ;
								iMax++;
								tempRow=ltDbFetchRow(tempCursor);
							}
							ltDbCloseCursor(tempCursor);
							iMaxItem = iMax;
							dbPtr=lt_dbinit();	//生成图表
							lt_db_htmlpage(dbPtr,"utf-8");
							sprintf(caLabel,"%sdata.ini",caTempDir);
							iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
							if(iFd == (-1)) {
								sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
								system(caLabel);
								ltDbCloseCursor(tempCursor);
								return 0;
							}
							lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
							tablePtr=lt_dbput_table(dbPtr,"list2");
							for(i=0;i<iMaxItem;i++) {
								lt_dbput_recordvars(tablePtr,4,
									"mytime",LT_TYPE_STRING, sCount[i].caItem,
									"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
									"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
									"abyte",LT_TYPE_LONGLONG,sCount[i].lTbytes
								);
								sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lTbytes,sCount[i].lDbytes,sCount[i].lUbytes);
								write(iFd, caLabel, strlen(caLabel));
							}
							close(iFd);

							sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl %sdata.ini %soutflow1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
							system(caLabel);
							lt_dbput_rootvars(dbPtr,1,"file1","outflow1.jpg");
							sprintf(caLabel,"%s/Scripts/outflowpltline.pl %sdata.ini %soutflow2.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
							system(caLabel);
							lt_dbput_rootvars(dbPtr,1,"file2","outflow2.jpg");

					}else{
							lStep = 24 * 3600;
							if(lTime1 == 0) {
								lTime1 = lTime2-31*lStep;
							}
							if(lTime2>(lTime1+31*lStep)){
								lTime2=lTime1+31*lStep;
							}
							if(lTime2>lTime){
								lTime2=lTime;
							}
							lTime0 = lTime1;
							memset(caSdate1,0,sizeof(caSdate1));
							memset(caSdate2,0,sizeof(caSdate2));
							nasTimeGetDate(caSdate1,lTime1);
							nasTimeGetDate(caSdate2,lTime2);
							iMaxItem = 32;
							for(i=0;i<iMaxItem;i++) {
										nasCvtStime(lTime0,caSdate,caTime);
										memset(sCount[i].caItem,0,16);
										lTime0 = lTime0 + lStep;
										sCount[i].lDbytes=0;
										sCount[i].lUbytes=0;
							}
							sprintf(sqlBuf,"select sdate,avg(ubytes),avg(dbytes) from msatimereport where sdate>='%s' and sdate<='%s' group by sdate",caSdate1,caSdate2);
							tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					    if(tempCursor == NULL){
									sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
									system(caLabel);
					        return 0;
					    }
					    lMaxBytes = 1;
					    iMax = 0;
					    tempRow=ltDbFetchRow(tempCursor);
					    while(tempRow!=NULL){								//速率
							    sprintf(caItem,"%s",tempRow[0]);
									lUbytes=atoll(tempRow[1])/60;
					        lDbytes=atoll(tempRow[2])/60;
					        if(lMaxBytes <= (lUbytes + lDbytes)) {
					           lMaxBytes = (lUbytes + lDbytes) ;
					        }
					        sprintf(sCount[iMax].caItem,"%s",caItem);
					        sCount[iMax].lUbytes = lUbytes ;
					        sCount[iMax].lDbytes = lDbytes ;
					        iMax++;
					        tempRow= ltDbFetchRow(tempCursor);
					    }
					    ltDbCloseCursor(tempCursor);
					    iMaxItem = iMax;
					    dbPtr=lt_dbinit();
					    lt_db_htmlpage(dbPtr,"utf-8");
					    sprintf(caLabel,"%sdata.ini",caTempDir);
					    iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
							if(iFd == (-1)) {
								sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
								system(caLabel);
								ltDbCloseCursor(tempCursor);
								return 0;
							}

					    lt_dbput_rootvars(dbPtr,1,"xunit","Day");
					    tablePtr=lt_dbput_table(dbPtr,"list2");
					    for(i=0;i<iMaxItem;i++){
					        lt_dbput_recordvars(tablePtr,4,
										"mytime",LT_TYPE_STRING, sCount[i].caItem,
										"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
										"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
										"abyte",LT_TYPE_LONGLONG,sCount[i].lUbytes+sCount[i].lDbytes
									);
									sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes+sCount[i].lDbytes,sCount[i].lDbytes,sCount[i].lUbytes);
									write(iFd, caLabel, strlen(caLabel));
					    }
					    close(iFd);

							sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl %sdata.ini %soutflow1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
							system(caLabel);
							lt_dbput_rootvars(dbPtr,1,"file1","outflow1.jpg");
							sprintf(caLabel,"%s/Scripts/outflowpltline.pl  %sdata.ini %soutflow2.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
							system(caLabel);
							lt_dbput_rootvars(dbPtr,1,"file2","outflow2.jpg");
					}

					if(element[2]=='0'){			//直方图 没有选中则将生成的图片html部分注释
						lt_dbput_rootvars(dbPtr,1,"hidestart1","<!--");
						lt_dbput_rootvars(dbPtr,1,"hideend1","-->");
					}

					if(element[3]=='0'){			//趋势图 没有选中则将生成的图片html部分注释;
						lt_dbput_rootvars(dbPtr,1,"hidestart2","<!--");
						lt_dbput_rootvars(dbPtr,1,"hideend2","-->");
					}

					if(lang==0){							//ch
						sprintf(htmlpage,"%s%s/dailyreportpart3.htm",_msahtmldir,_dailyreport_ch);
						lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}else if(lang==1){				//en
						sprintf(htmlpage,"%s%s/dailyreportpart3.htm",_msahtmldir,_dailyreport_en);
						lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}

					if(lt_page_content==NULL){
							sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
				  		system(caLabel);
							return 0;
					}else{
					    lt_page_content=strstr(lt_page_content,"<tr>");
					    sprintf(caLabel,"%sindex.htm",caTempDir);
							iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
							if(iFd >0) {
								write(iFd,lt_page_content,strlen(lt_page_content));
							}
							close(iFd);
					}
		    	lt_dbfree(dbPtr);
		}



		if(element[4]=='1'||checkb[1]=='1')			//服务汇总
		{
			{
				char srvStr[64];
				char workTime[32];
				int  srvindex;
				memset(workTime,0,sizeof(workTime));
				srvindex=1;
				nasTimeGetDate(caSdate1,lTime1);
				nasTimeGetDate(caSdate2,lTime2);
				
				//sprintf(sqlBuf,"select service,sum(bytes)/1024+1 as abyte,workflag,sum(onlinetime) as atime from msasrvreport where sdate>='%s' and sdate<='%s' and workflag=%d group by service,workflag order by abyte desc ",caSdate1,caSdate2,work);
				sprintf(sqlBuf,"select a.service,sum(bytes)/1024+1 as abyte,workflag,sum(onlinetime) as atime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) group by a.service,a.workflag order by abyte desc limit %s offset 0",caSdate1,caSdate2,strTemp.s,userNumber);
				tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor == NULL) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				    system(caLabel);
				    return 0;
				}
				sprintf(caLabel,"%sdata.ini",caTempDir);
			  iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
				if(iFd == (-1)) {
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
					system(caLabel);
					ltDbCloseCursor(tempCursor);
					return 0;
				}
				write(iFd, "0,0\r\n", strlen("0,0\r\n"));
				lMaxBytes = 1;
				dbPtr=lt_dbinit();
				lt_db_htmlpage(dbPtr,"utf-8");
				tablePtr=lt_dbput_table(dbPtr,"flowlist");
				tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
						sprintf(srvStr,"NO.%d",srvindex);
						if(atol(tempRow[2])==0){
							sprintf(workTime,"%s","Alltime");
						}else if(atol(tempRow[2])==1){
							sprintf(workTime,"%s","Worktime");
						}

						lt_dbput_recordvars(tablePtr,5,
							"index",LT_TYPE_STRING,srvStr,
							"sname",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
							"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[1]),
							"worktime",LT_TYPE_STRING,workTime,
							"onlinetime",LT_TYPE_STRING,time2str(atol(tempRow[3]))
						);
						sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[1]));
						write(iFd, caLabel, strlen(caLabel));
						if(lMaxBytes<atoll(tempRow[1])){
								lMaxBytes=atoll(tempRow[1]);
						}
						srvindex++;
						tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
				close(iFd);

				if(element[5]=='0'){//全部汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart5","<!--");
		 			lt_dbput_rootvars(dbPtr,1,"hideend5","-->");
				}

				if(element[6]=='0'){//按部门汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart6","<!--");
		 			lt_dbput_rootvars(dbPtr,1,"hideend6","-->");
				}

				if(element[7]=='0'){//按用户流量汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart7","<!--");
		 			lt_dbput_rootvars(dbPtr,1,"hideend7","-->");
				}

				if(element[8]=='0'){//按用户连接汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart8","<!--");
					lt_dbput_rootvars(dbPtr,1,"hideend8","-->");
				}

				if(element[9]=='0'){				//按日期汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart9","<!--");
					lt_dbput_rootvars(dbPtr,1,"hideend9","-->");
				}

				if(element[10]=='0'){				//按部门日期流量汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart10","<!--");
					lt_dbput_rootvars(dbPtr,1,"hideend10","-->");
				}

				if(element[11]=='0'){				//按用户日期连接汇总
					lt_dbput_rootvars(dbPtr,1,"hidestart11","<!--");
					lt_dbput_rootvars(dbPtr,1,"hideend11","-->");
				}
				sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
				system(caLabel);
				lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"srvreport.gif\">" );

				if(lang==0){								//Ch
					sprintf(htmlpage,"%s%s/dailyreportpart4.htm",_msahtmldir,_dailyreport_ch);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}else if(lang==1){					//Eng
					sprintf(htmlpage,"%s%s/dailyreportpart4.htm",_msahtmldir,_dailyreport_en);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}

				if(lt_page_content==NULL){
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
		    		system(caLabel);
		    		lt_dbfree(dbPtr);
						return 0;
				}else{
						lt_page_content=strstr(lt_page_content,"<tr>");
						sprintf(caLabel,"%sindex.htm",caTempDir);
						iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
						if(iFd >0) {
							  write(iFd,lt_page_content,strlen(lt_page_content));
						}
						close(iFd);
						lt_dbfree(dbPtr);
				}
			}
		}



		if(element[5]=='1'){						//相关报告---- 全部汇总
			srvreport(caSdate1,caSdate2,strTemp.s,1,caTempDir,lang,work);
		}

		if(element[6]=='1'){						//相关报告---- 部门汇总
			srvreport(caSdate1,caSdate2,strTemp.s,2,caTempDir,lang,work);
		}

		if(element[7]=='1'){						//相关报告---- 用户流量汇总
			srvreport(caSdate1,caSdate2,strTemp.s,3,caTempDir,lang,work);
		}

		if(element[8]=='1'){						//相关报告---- 用户连接汇总
			srvreport(caSdate1,caSdate2,strTemp.s,7,caTempDir,lang,work);
		}

		if(element[9]=='1'){						//相关报告---- 日期汇总
			srvreport(caSdate1,caSdate2,strTemp.s,4,caTempDir,lang,work);
		}

		if(element[10]=='1'){						//相关报告---- 部门日期流量汇总
			srvreport(caSdate1,caSdate2,strTemp.s,5,caTempDir,lang,work);
		}

		if(element[11]=='1'){						//相关报告---- 部门日期连接汇总
			srvreport(caSdate1,caSdate2,strTemp.s,6,caTempDir,lang,work);
		}



		if(element[12]=='1'||checkb[2]=='1'){//网络访问总计
	  {
			struct nasFlowCount_s {
							char          		 caItem[16];
							unsigned long long lDbytes;
							unsigned long long lUbytes;
							unsigned long long lTbytes;
							unsigned long long lConTime;
			}sCount[35];
			memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));

	    dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
			if(strcmp(caSdate,caEdate)==0){
					lStep = 3600;
					if(lTime1 == 0) {
							lTime1 = lTime - 24*lStep;
					}
					if(lTime2>lTime){
						  lTime2=lTime;
					}
					lTime0 = lTime1;
					iMaxItem = 25;
					for(i=0;i<iMaxItem;i++){
						nasCvtStime(lTime0,caSdate,caTime);
						memcpy(sCount[i].caItem,caTime,2);
						lTime0 = lTime0 + lStep;
					}
					nasTimeGetDate(caSdate1,lTime1);
					nasTimeGetDate(caSdate2,lTime2);
					nasTimeGetTime(caTime1,lTime1);
					nasTimeGetTime(caTime2,lTime2);
					caTime1[2]=0;
					caTime2[2]=0;
					sprintf(sqlBuf,"select hours,sum(ubytes),sum(dbytes) from msatimesrvreport where sdate>='%s' and sdate<='%s' and hours>=%s and hours<=%s group by sdate order by date desc ",caSdate1,caSdate2,caTime1,caTime2);
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
			}else{
					lStep = 24 * 3600;
					if(lTime1 == 0) {
						lTime1 = lTime-31*lStep;
					}
					if(lTime2>(lTime1+31*lStep)){
						lTime2=lTime1+31*lStep;
					}
					if(lTime2>lTime){
						lTime2=lTime;
					}
					lTime0 = lTime1;
					iMaxItem = 32;
					for(i=0;i<iMaxItem;i++) {
						nasCvtStime(lTime0,caSdate,caTime);
						memset(sCount[i].caItem,0,16);
						lTime0 = lTime0 + lStep;
					}
					nasTimeGetDate(caSdate1,lTime1);
					nasTimeGetDate(caSdate2,lTime2);
					sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimesrvreport where sdate>='%s' and sdate<='%s' group by sdate order by sdate desc",caSdate1,caSdate2);
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					lt_dbput_rootvars(dbPtr,1,"xunit","Day");
			}
			if(tempCursor == NULL) {
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
					system(caLabel);
			    return 0;
			}
			lMaxBytes = 1;
			iMax = 0;
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lUbytes=atoll(tempRow[1]);
				lUbytes=atoll(tempRow[2]);
				if(lMaxBytes <= (lUbytes + lDbytes) / 1024) {
				   lMaxBytes = (lUbytes + lDbytes) / 1024;
				}
				sprintf(sCount[iMax].caItem,"%s",tempRow[0]);
				sCount[iMax].lUbytes = lUbytes / 1024;
				sCount[iMax].lDbytes = lDbytes / 1024;
				sCount[iMax].lTbytes = sCount[iMax].lUbytes+sCount[iMax].lDbytes;
				iMax++;
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
			iMaxItem = iMax;
			sprintf(caLabel,"%sdata.ini",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
				system(caLabel);
				ltDbCloseCursor(tempCursor);
				return 0;
			}
			write(iFd, "0,0,0,0\r\n", strlen("0,0,0,0\r\n"));
			tablePtr=lt_dbput_table(dbPtr,"list2");
			for(i=0;i<iMaxItem;i++) {
		     lt_dbput_recordvars(tablePtr,4,
						"mytime",LT_TYPE_STRING,sCount[i].caItem,
						"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
						"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
						"abyte",LT_TYPE_LONGLONG,sCount[i].lTbytes
				 );
				 sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lTbytes,sCount[i].lDbytes,sCount[i].lUbytes);
				 write(iFd, caLabel, strlen(caLabel));
		  }
			close(iFd);
			sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl %sdata.ini %sflowreports1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"file1","flowreports1.jpg");
			sprintf(caLabel,"%s/Scripts/outflowpltline.pl %sdata.ini %sflowreports2.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"file2","flowreports2.jpg");
			lt_dbput_rootvars(dbPtr,1,"topusernum",userNumber);

			if(element[13]=='0'){//直方图
				lt_dbput_rootvars(dbPtr,1,"hidestart13","<!--");
	 			lt_dbput_rootvars(dbPtr,1,"hideend13","-->");
			}
		  if(element[14]=='0'){//趋势图
				lt_dbput_rootvars(dbPtr,1,"hidestart14","<!--");
	 			lt_dbput_rootvars(dbPtr,1,"hideend14","-->");
		  }
		  if(element[15]=='0'){//按天汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart15","<!--");
	 			lt_dbput_rootvars(dbPtr,1,"hideend15","-->");
		  }

	    if(element[16]=='0'){//用户总流量排序
				lt_dbput_rootvars(dbPtr,1,"hidestart16","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend16","-->");
	    }
	    if(element[17]=='0'){//用户上流量排序
				lt_dbput_rootvars(dbPtr,1,"hidestart17","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend17","-->");
	    }
	    if(element[18]=='0'){//用户下流量排序
				lt_dbput_rootvars(dbPtr,1,"hidestart18","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend18","-->");
	    }
	    if(element[19]=='0'){//部门汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart19","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend19","-->");
	    }
	    if(element[20]=='0'){//部门人员汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart20","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend20","-->");
	    }
	    if(element[21]=='0'){//部门日期汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart21","<!--");
 				lt_dbput_rootvars(dbPtr,1,"hideend21","-->");
	    }

			if(lang==0){//Ch
				sprintf(htmlpage,"%s%s/dailyreportpart5.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}else if(lang==1){//Eng
				sprintf(htmlpage,"%s%s/dailyreportpart5.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}

			if(lt_page_content==NULL){
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
					system(caLabel);/*建立目录和正在处理标志*/
	    		lt_dbfree(dbPtr);
					return 0;
			}else{
			    lt_page_content=strstr(lt_page_content,"<tr>"); /*生成html格式报告*/
			    sprintf(caLabel,"%sindex.htm",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
					if(iFd >0) {
		          write(iFd,lt_page_content,strlen(lt_page_content));
					}
					close(iFd);
					lt_dbfree(dbPtr);
			}

		}
	}



	 //网络流量部分
	if(element[15]=='1'){//相关报告---- 网络流量分析-按天汇总
		connectflow(caSdate1,caSdate2,strTemp.s,1,caTempDir,lang,work,inttopusernum);
	}

	if(element[16]=='1'){//相关报告---- 网络流量分析-用户总流量排序
		connectflow(caSdate1,caSdate2,strTemp.s,3,caTempDir,lang,work,inttopusernum);
	}

	 if(element[17]=='1'){//相关报告---- 网络流量分析-用户上流量排序
		connectflow(caSdate1,caSdate2,strTemp.s,4,caTempDir,lang,work,inttopusernum);
	 }

	 if(element[18]=='1'){//相关报告---- 网络流量分析-用户下流量排序
		connectflow(caSdate1,caSdate2,strTemp.s,5,caTempDir,lang,work,inttopusernum);
	 }

	 if(element[19]=='1'){//相关报告---- 网络流量分析-部门汇总
		connectflow(caSdate1,caSdate2,strTemp.s,6,caTempDir,lang,work,inttopusernum);
	 }

	 if(element[20]=='1'){//相关报告---- 网络流量分析-部门人员汇总
		connectflow(caSdate1,caSdate2,strTemp.s,7,caTempDir,lang,work,inttopusernum);
	 }

	 if(element[21]=='1'){//相关报告---- 网络流量分析-部门日期汇总
		connectflow(caSdate1,caSdate2,strTemp.s,8,caTempDir,lang,work,inttopusernum);
	 }




	if(element[22]=='1'||checkb[3]=='1'){     //部门与人员报告
	{
			nasTimeGetDate(caSdate1,lTime1);
	    nasTimeGetDate(caSdate2,lTime2);
	    memset(oldgname,0,sizeof(oldgname));
	    memset(egname,0,sizeof(egname));
			sprintf(sqlBuf,"select c.id,c.name,sum(a.bytes)/1024+1 as mybytes,sum(a.ctime) \
				from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s'\
				and a.userid = b.userid and  b.groupid0=c.id and b.groupid0 in(%s) and workflag=%d group by c.id,c.name  \
				order by mybytes desc limit %d offset 0 ",caSdate1,caSdate2,strTemp.s,work,inttopusernum
			);
			//printf("sql5:%s\n",sqlBuf);						//部门访问排序
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				system(caLabel);										//建立目录和正在处理标志
				return 0;
			}
			sprintf(caLabel,"%sdata.ini",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
				system(caLabel);//建立目录和正在处理标志
				ltDbCloseCursor(tempCursor);
		    return 0;
			}
			
			write(iFd, "0,0\r\n", strlen("0,0\r\n"));
			lMax=0;
			int xxxi=0;
			dbPtr=lt_dbinit();
			lt_db_htmlpage(dbPtr,"utf-8");
			tablePtr=lt_dbput_table(dbPtr,"flowlist");
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,4,
					"gid",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3]))
				);
				sprintf(oldgname,"%s",tempRow[1]);
				if(atoll(tempRow[2])>lMax){
					lMax=atoll(tempRow[2]);
				}
				sprintf(caLabel,"%d,%llu\r\n",xxxi+1,atoll(tempRow[2]));
				write(iFd, caLabel, strlen(caLabel));
				xxxi++;
				tempRow=ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
			close(iFd);

			sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %sdeppie.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMax);
			system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"deppie.gif\">" );
			if(element[26]=='0'){		//部门网络时间汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart26","<!--");
	 			lt_dbput_rootvars(dbPtr,1,"hideend26","-->");
			}

			if(lang==0)							{//Ch
				sprintf(htmlpage,"%s%s/dailyreportpart7.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}else if(lang==1){			//Eng
				sprintf(htmlpage,"%s%s/dailyreportpart7.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}

			if(lt_page_content==NULL){
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    		system(caLabel);
	    		lt_dbfree(dbPtr);
					return 0;
			}else{
					lt_page_content=strstr(lt_page_content,"<tr>");
					sprintf(caLabel,"%sindex.htm",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
					if(iFd >0) {
						write(iFd,lt_page_content,strlen(lt_page_content));
					}
					close(iFd);
					lt_dbfree(dbPtr);
			}

			for(xxxi=0;xxxi<1000;xxxi++){//循序部门
				if(xgid[xxxi]==-1){
				   break;
				}

				{
					int  topuser=100;
					unsigned long long *lTop;
					lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
					for(iTemp=0;iTemp<topuser;iTemp++){
					   lTop[iTemp]=0;
					}
					nasTimeGetDate(caSdate1,lTime1);
					nasTimeGetDate(caSdate2,lTime2);
					sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.bytes)/1024+1 abyte,sum(a.ctime),c.name from msauserhttpreport a,\
					  msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0=%d and workflag=%d group by a.userid,b.dispname,c.name \
					  order by abyte desc limit %d offset 0 ",caSdate1,caSdate2,xgid[xxxi],work,inttopusernum
					);
					//printf("sql6:%s\n",sqlBuf);
					iTemp=0;
					lMax=0;
					dbPtr=lt_dbinit();
					lt_db_htmlpage(dbPtr,"utf-8");
					tablePtr=lt_dbput_table(dbPtr,"flowlist");
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor == NULL) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
						system(caLabel);/*建立目录和正在处理标志*/
						return 0;
					}
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						lt_dbput_recordvars(tablePtr,5,
							"iIndex",LT_TYPE_LONG,iTemp,
							"uname",LT_TYPE_STRING,tempRow[1],
							"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
							"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
							"groupname",LT_TYPE_STRING,tempRow[4]
						);
						lTop[iTemp]=atoll(tempRow[2]);
						if(lTop[iTemp]>lMax){
							lMax=lTop[iTemp];
						}
						iTemp++;
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
					sprintf(caLabel,"%sdata.ini",caTempDir);////饼状图开始
					iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
					if(iFd == (-1)) {
							sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
							system(caLabel);/*建立目录和正在处理标志*/
							ltDbCloseCursor(tempCursor);
							return 0;
					}
					for(i=0;i<iTemp;i++) {
							//ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
							sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
					    write(iFd, caLabel, strlen(caLabel));
					}
					close(iFd);
					sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %sggg%dflowreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,xgid[xxxi],lMax);
					system(caLabel);
					sprintf(caLabel,"<img src=\"ggg%dflowreport.gif\"  >",xgid[xxxi]);
					lt_dbput_rootvars(dbPtr,1,"piechart",caLabel);
					sprintf(caLabel,"%sdata.ini",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
					if(iFd == (-1)) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
						system(caLabel);
						ltDbCloseCursor(tempCursor);
						return 0;
					}

					for(i=0;i<iTemp;i++) {
					  sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
					  write(iFd, caLabel, strlen(caLabel));
					}

					close(iFd);
					sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %sggg%duserreport1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,xgid[xxxi],(unsigned long long)(lMax+1));
					system(caLabel);
					sprintf(caTempFile,"ggg%duserreport1.jpg",xgid[xxxi]);
					lt_dbput_rootvars(dbPtr,1,"file",caTempFile);//直方图结束

					if(element[24]=='0'){
						lt_dbput_rootvars(dbPtr,1,"hidestart24","<!--");
						lt_dbput_rootvars(dbPtr,1,"hideend24","-->");
					}

					if(element[25]=='0'){
						lt_dbput_rootvars(dbPtr,1,"hidestart25","<!--");
						lt_dbput_rootvars(dbPtr,1,"hideend25","-->");
					}


					if(lang==0){					//Ch
						sprintf(htmlpage,"%s%s/dailyreportpart_depart.htm",_msahtmldir,_dailyreport_ch);
						lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}else if(lang==1){		//Eng
						sprintf(htmlpage,"%s%s/dailyreportpart_depart.htm",_msahtmldir,_dailyreport_en);
						lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}

					if(lt_page_content==NULL){
							sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
			    		system(caLabel);/*建立目录和正在处理标志*/
							return 0;
					}else{
							lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
							sprintf(caLabel,"%sggg%d.htm",caTempDir,xgid[xxxi]);
							iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
							if(iFd >0) {
								write(iFd,lt_page_content,strlen(lt_page_content));
							}
							close(iFd);
					}
					lt_dbfree(dbPtr);
				}
			}
		}
	}


	if(element[23]=='1'){										//用户访问排序
			int  topuser=inttopusernum;
			unsigned long long *lTop;
			long lTopId[inttopusernum];
			lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
			for(iTemp=0;iTemp<topuser;iTemp++){
				lTop[iTemp]=0;
			}
			for(iTemp=0;iTemp<inttopusernum;iTemp++){
				lTopId[iTemp]=0;
			}
			nasTimeGetDate(caSdate1,lTime1);
			nasTimeGetDate(caSdate2,lTime2);
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.bytes)/1024+1 abyte,sum(onlinetime), c.name from msasrvreport a, \
			 msauser b,msagroup c where a.sdate>='%s' and a.sdate<='%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0 in(%s) and workflag=%d group by a.userid,b.dispname,c.name  \
			 order by abyte desc limit %d offset 0 ",caSdate1,caSdate2,strTemp.s,work,inttopusernum);
			//printf("sql7:%s\n",sqlBuf);
			iTemp=0;
			lMax=0;
			dbPtr=lt_dbinit();
			lt_db_htmlpage(dbPtr,"utf-8");
			tablePtr=lt_dbput_table(dbPtr,"flowlist");
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				system(caLabel);/*建立目录和正在处理标志*/
				return 0;
			}
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,5,
					"iIndex",LT_TYPE_LONG,iTemp,
					"uname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
					"groupname",LT_TYPE_STRING,tempRow[4]);
				lTop[iTemp]=atoll(tempRow[2]);
				lTopId[iTemp]=atol(tempRow[0]);
				if(lTop[iTemp]>lMax){
					lMax=lTop[iTemp];
				}
				iTemp++;
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
			sprintf(caLabel,"%sdata.ini",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)) {
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
					system(caLabel);										//建立目录和正在处理标志
					ltDbCloseCursor(tempCursor);
					return 0;
			}
			for(i=0;i<iTemp;i++) {
					sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
					write(iFd, caLabel, strlen(caLabel));
			}
			close(iFd);
			sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %suserreport1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMax+1));
			system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"file1","userreport1.jpg");
			sprintf(caLabel,"%sdata.ini",caTempDir);//饼状图开始
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
				system(caLabel);											/*建立目录和正在处理标志*/
				ltDbCloseCursor(tempCursor);
				return 0;
			}

			for(i=0;i<iTemp;i++) {
				  //ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
				  sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
			    write(iFd, caLabel, strlen(caLabel));
			}
			close(iFd);
			sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %sflowreport.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMax);
			system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreport.gif\">" );


			if(element[24]=='0'){
				lt_dbput_rootvars(dbPtr,1,"hidestart24","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend24","-->");
			}

			if(element[25]=='0'){
				lt_dbput_rootvars(dbPtr,1,"hidestart25","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend25","-->");
			}

			if(lang==0){//Ch
				sprintf(htmlpage,"%s%s/dailyreportpart6.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}else if(lang==1){//Eng
				sprintf(htmlpage,"%s%s/dailyreportpart6.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}

			if(lt_page_content==NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
				system(caLabel);/*建立目录和正在处理标志*/
		        return 0;
			}else{
				lt_page_content=strstr(lt_page_content,"<tr>");
				sprintf(caLabel,"%sindex.htm",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
				if(iFd >0) {
				  write(iFd,lt_page_content,strlen(lt_page_content));
				}
			    close(iFd);
			}
			lt_dbfree(dbPtr);
	}

	if(element[26]=='1'){										//相关报告——部门网络时间汇总
		usertimereport(caSdate1,caSdate2,caTempDir,strTemp.s,lang,work);
	}

	if(element[27]=='1'||element[28]=='1'){ //WEB报告——前20/30/50名访问次数最多站点
	{
			nasTimeGetDate(caSdate1,lTime1);
			nasTimeGetDate(caSdate2,lTime2);
			sprintf(sqlBuf,"select host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from msahttpreport \
			   where sdate>='%s' and sdate<='%s' and workflag=%d group by host,urlsort order by acount desc limit 50 offset 0",caSdate1,caSdate2,work
			);
			//printf("sql9:%s\n",sqlBuf);
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				system(caLabel);									//建立目录和正在处理标志
				return 0;
			}
			dbPtr=lt_dbinit();
			lt_db_htmlpage(dbPtr,"utf-8");
			tablePtr=lt_dbput_table(dbPtr,"list1");
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,5,
					"uname",LT_TYPE_STRING,tempRow[0],
					"lbyte",LT_TYPE_LONG,atol(tempRow[1]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
					"lrequest",LT_TYPE_STRING,tempRow[3],
					"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[4])].srvName);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);


			sprintf(sqlBuf,"select sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort\
			   from msahttpreport where sdate>='%s' and sdate<='%s' group by urlsort order by acount desc ",caSdate1,caSdate2
			);
			//printf("sql0:%s\n",sqlBuf);
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				system(caLabel);											//建立目录和正在处理标志
				return 0;
			}
			tablePtr=lt_dbput_table(dbPtr,"list3");
			tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,4,
					"lbyte",LT_TYPE_LONG,atol(tempRow[0]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[1])),
					"lrequest",LT_TYPE_STRING,tempRow[2],
					"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[3])].srvName);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);

			//top30流量的排序
		  sprintf(sqlBuf,"select host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from msahttpreport \
		     where sdate>='%s' and sdate<='%s' and workflag=%d group by host,urlsort order by abyte desc limit 30 offset 0",caSdate1,caSdate2,work
	    );
			//printf("sql1:%s\n",sqlBuf);
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL) {
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
					system(caLabel);
					return 0;
			}

			tablePtr=lt_dbput_table(dbPtr,"list2");
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,5,
				"uname",LT_TYPE_STRING,tempRow[0],
				"lbyte",LT_TYPE_LONG,atol(tempRow[1]),
				"ltime",LT_TYPE_STRING,time2str(atol(tempRow[2])),
				"lrequest",LT_TYPE_STRING,tempRow[3],
				"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[4])].srvName);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);

			sprintf(sqlBuf,"select sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from msahttpreport \
				   where sdate>='%s' and sdate<='%s' and  workflag=%d group by urlsort order by abyte desc ",caSdate1,caSdate2,work
			);
			//printf("sql2:%s\n",sqlBuf);
			tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor == NULL) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				system(caLabel);
				return 0;
			}
			tablePtr=lt_dbput_table(dbPtr,"list4");
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,4,
					"lbyte",LT_TYPE_LONG,atol(tempRow[0]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[1])),
					"lrequest",LT_TYPE_STRING,tempRow[2],
					"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[3])].srvName);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);

			//结果输出
			memset(caSdate1,0,sizeof(caSdate1));
			memset(caSdate2,0,sizeof(caSdate2));
			nasCvtStime(lTime1,caSdate1,caTime1);
			nasCvtStime(lTime2,caSdate2,caTime2);
			caTime1[5]=0;
			caTime2[5]=0;
			sprintf(caLabel,"Web Report: %s",caSdate1);
			lt_dbput_rootvars(dbPtr,1,"lab",caLabel);

			if(element[27]=='0'){//访问次数最多站点
				lt_dbput_rootvars(dbPtr,1,"hidestart27","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend27","-->");
			}

			if(element[28]=='0'){//访问流量最多站点
				lt_dbput_rootvars(dbPtr,1,"hidestart28","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend28","-->");
			}

			if(element[29]=='0'){//全部汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart29","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend29","-->");
			}

			if(element[30]=='0'){//部门汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart30","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend30","-->");
			}

			if(element[31]=='0'){//用户汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart31","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend31","-->");
			}

			if(element[32]=='0'){//日期汇总
				lt_dbput_rootvars(dbPtr,1,"hidestart32","<!--");
				lt_dbput_rootvars(dbPtr,1,"hideend32","-->");
			}

			if(lang==0){//Ch
				sprintf(htmlpage,"%s%s/dailyreportpart8.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}else if(lang==1){//Eng
				sprintf(htmlpage,"%s%s/dailyreportpart8.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}

			if(lt_page_content==NULL){
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
					system(caLabel);//建立目录和正在处理标志
					lt_dbfree(dbPtr);
					return 0;
			}else{
					lt_page_content=strstr(lt_page_content,"<tr>");
					sprintf(caLabel,"%sindex.htm",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
					if(iFd >0) {
						write(iFd,lt_page_content,strlen(lt_page_content));
					}
					close(iFd);
					lt_dbfree(dbPtr);//生成html格式报告
			}
		}
	}


  //web报告相关报告
  if(element[29]=='1'){//相关报告---- 全部汇总
		webreport(caSdate1,caSdate2,strTemp.s,1,caTempDir,lang,work);
  }

	if(element[30]=='1'){//相关报告---- 部门汇总
		webreport(caSdate1,caSdate2,strTemp.s,2,caTempDir,lang,work);
	}

	if(element[31]=='1'){//相关报告---- 用户汇总
		webreport(caSdate1,caSdate2,strTemp.s,3,caTempDir,lang,work);
	}

	if(element[32]=='1'){//相关报告---- 日期汇总
		webreport(caSdate1,caSdate2,strTemp.s,4,caTempDir,lang,work);
	}

	{
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		sprintf(htmlpage,"%s%s/dailyreportpart10.htm",_msahtmldir,_dailyreport_ch);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		if(lt_page_content==NULL){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
			system(caLabel);//建立目录和正在处理标志
			lt_dbfree(dbPtr);
			return 0;
		}else{
			lt_page_content=strstr(lt_page_content,"<tr>");
			sprintf(caLabel,"%sindex.htm",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_NDELAY | O_APPEND | O_CREAT,0666);
			if(iFd >0) {
				write(iFd,lt_page_content,strlen(lt_page_content));
			}
			close(iFd);
			lt_dbfree(dbPtr);//生成html格式报告
		}

	}

  chdir(caTempDir);
  sprintf(caLabel,"%s/Scripts/html2mail  %s index.htm %sindex.mht report %s",_msahtmldir,caTempDir,caTempDir,email);
  system(caLabel);

	system("tar -cvf report.tar *");
	sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	system(caLabel);
	sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	system(caLabel);

	//printf("dailyReport end\n");

  return 0;

}



int main(int argc,char **argv)
{

  int  typ;
  long tempIndex;
  int  iPid;
  int  pstatus;
  char caFile[236];
  char caFile2[236];
  char temp[123];
  char *p;
	int  intMaxShmSize;
	int  intShmKey;
	int  i;

  lt_shmHead   *lt_MMHead;

 	memset(topusernum,0,sizeof(topusernum));
 	memset(worktime,0,sizeof(worktime));
  memset(groupstr,0,sizeof(groupstr));
  memset(caSdates,0,sizeof(caSdates));
  memset(caEdates,0,sizeof(caEdates));
  inttopusernum=0;

  typ=atoi(argv[1]);
  tempIndex=atol(argv[2]);
	//printf("tempIndex:%ld\n",tempIndex); //模板文件夹名称
  //printf("typ:%d\n",typ); //类型

 	sprintf(caFile,"/datacenter/msa/report/page_data/%ld/info.ini",tempIndex);	  
 	//printf("caFile:%s\n",caFile);
 	bcCnfGetValue_s(caFile,"reportname",reportname);
	if(typ==0){//每天

	}

	if(typ==1){//每周
	   bcCnfGetValue_s(caFile,"reportname",reportname);
	}

	if(typ==2){//立即
	   sprintf(caFile2,"/datacenter/msa/report/page_data");
	   sprintf(temp,"%ld",tempIndex);
	   sprintf(caSdates,"%s", 	bcCnfGetValueS(caFile2,temp,"sdate"));
	   sprintf(caEdates,"%s", 	bcCnfGetValueS(caFile2,temp,"edate"));
	   sprintf(reportname,"%s", bcCnfGetValueS(caFile2,temp,"rpname"));
	   sprintf(worktime,"%s", 	bcCnfGetValueS(caFile2,temp,"work"));//工作时间段
	   sprintf(groupstr,"%s",		bcCnfGetValueS(caFile2,temp,"group"));//部门

	}
	  bcCnfGetValue_s(caFile,"element",element);
	  bcCnfGetValue_s(caFile,"checkb",checkb); ;
	  bcCnfGetValue_s(caFile,"email",email);
	  bcCnfGetValue_s(caFile,"language",language);
    bcCnfGetValue_s(caFile,"topusernum",topusernum);  //暂未处理该变量

    inttopusernum=atol(topusernum);
    if(inttopusernum>99){//默认为100
    	inttopusernum=99;
    }

		lang=atoi(language);
		work=atoi(worktime);


	  strTemp.s=0;
	  jjj=0;

		sprintf(caTmpp,"%s","");
		stralloc_cats(&strTemp,caTmpp);
		for(i=0;i<1000;i++){
			xgid[i]=-1;
			if(groupstr[i]=='1'){
					jjj++;
					xgid[i]=i;
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

	 	deployscope=typ;

    iPid = fork();
		if(iPid == 0) {
			    /*读取最大共享内存数*/
			   	p=bcCnfGetValueS(_pubconfdir,"kernel", "maxshmsize");
					if(p==NULL){
						fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
						_exit(0);
					}
					intMaxShmSize=atol(p);

					/*读取系统shmKey*/
					p=bcCnfGetValueS(_pubconfdir,"kernel", "shmkey");
					if(p==NULL){
						fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
						_exit(0);
					}
					intShmKey=atol(p);

					lt_MMHead=openShmMem(intShmKey,intMaxShmSize);
					if(lt_MMHead==NULL){
						fprintf(stderr,"can open share memory!\n");
						 _exit(0);
					}

				  lt_shmHead   *kernel_MMHead;
          p=bcCnfGetValueS(_pubconfdir,"kernel", "maxkernelshmsize");
          if(p==NULL){
              fprintf(stderr,"Start applicatin error: maxkernelshmsize is NULL!\n");
              return -1;
          }
          intMaxShmSize=atol(p);
          kernel_MMHead=msaopenBigBootMem(intMaxShmSize);
          if(!kernel_MMHead){
              closeShmMem(lt_MMHead);
              return -1;
          }

         	_bcKernelHead=(msaKernelHead *)kernel_MMHead->ShmMemPtr;
         	msaInitPubVar(lt_MMHead,kernel_MMHead);
					dbName=_ltPubInfo->_dbname;
	        dbUser=_ltPubInfo->_dbuser;
	        dbPass=_ltPubInfo->_dbpass;
          G_DbCon=ltDbConnect(dbUser,dbPass,dbName);

					if(G_DbCon!=NULL){
						printf("db connect ok\n");
					}else{
						fprintf(stderr,"db connect error\n");
					}

			    signal(SIGALRM, catch_alarm);
	    		alarm(3600);
			    dailyReport(tempIndex);
				  ltDbClose(G_DbCon);
	        _exit(0);
    }
    wait(&pstatus);

    //printf("main end.......\n");

    return 0;
}
