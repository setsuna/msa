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

char *dbName;
char *dbUser;
char *dbPass;
char tempname[123];

 
/*部门网络时间汇总 */
int usertimereport(char date1[36],char date2[36],char caTempDir[256],int gid,int lang)
{
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long lTime,lTime1,lTime2;
    char reportname[123];
    char *lt_page_content=NULL;
    int  iFd;
    char caLabel[256];
	ltDbConn *G_DbCon;
    printf("usertimereport start............\n");   
	printf("date1:%s\n",date1);
	printf("date2:%s\n",date2);
	printf("gid:%d\n",gid);
	printf("caTempDir:%s\n",caTempDir);
	printf("lang:%d\n\n",lang);
    dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (G_DbCon==NULL){
		printf("report:can't open db\n");
		_exit(0);
	}
    iFd=-1;
  
	sprintf(reportname,"部门网络时间汇总");
	strgb2utf8(reportname,tempname,sizeof(reportname));
	memset(reportname,0,sizeof(reportname));
	sprintf(reportname,"%s",tempname);
    if(reportname==NULL){
       sprintf(reportname,"******");
    } 
 
    if(ltFileIsExist(caTempDir)!=1) {
    	mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }

    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            printf("mkdir daily report dir error!\n");
            return 0;
        } 
    }
    printf("caTempDir:%s\n",caTempDir);
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    tempCursor=NULL;
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

    printf("lTime1::%ld\n",lTime1);
	printf("lTime2::%ld\n",lTime2);
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
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    int  groupnum;

    typedef struct gname_s {
		int             gid;
		char            gname[50];
		long long       gflow;
		int             gtime;
	}nasGname;

    nasGname gnamelist[100];
    int      i;
    char     caLine[30000];
	char     tmpLine[30000];
    int      oldgroupid;
	int      oldsrvid;
	int      xgroupid;
	char     oldDate[64];
	
	memset(tmpLine,0,sizeof(tmpLine));
	memset(caLine,0,sizeof(caLine));
    for(i=0;i<100;i++){
    	gnamelist[i].gid=-1;
    	memset(gnamelist[i].gname,0,50);
    	gnamelist[i].gflow=0;
    	gnamelist[i].gtime=0;
    }
	/*显示部门 */
	groupnum=0;	  
    if(gid==-1){
     sprintf(sqlBuf,"select id,name from msagroup order by id asc ");
    }else{
  	 sprintf(sqlBuf,"select id,name from msagroup where id=%d order by id asc ",gid);
    }	 
    printf("sqla:%s\n",sqlBuf);
	tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor == NULL) {
	  sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	  system(caLabel);/*建立目录和正在处理标志*/
	  return 0;
	}

	dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"glist");
	tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    	gnamelist[groupnum].gid=atol(tempRow[0]);
    	sprintf(gnamelist[groupnum].gname,"%s",tempRow[1]);
    	lt_dbput_recordvars(tablePtr,1,
		    "groupname",LT_TYPE_STRING,tempRow[1]
		);
    	tempRow= ltDbFetchRow(tempCursor);
    	groupnum++;
    	if(groupnum>99){
    		break;
    	}
    }
    ltDbCloseCursor(tempCursor);
	
	oldgroupid=-1;
	oldsrvid=-1;
	memset(oldDate,0,sizeof(oldDate));    
    if(gid==-1){
     sprintf(sqlBuf,"select sum(onlinetime) as atime,sdate,a.userid,c.id from msasrvreport a,msauser b ,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id group by sdate,a.userid,c.id,b.groupid0 order by sdate,b.groupid0,atime desc",caSdate1,caSdate2);
    }else{
  	 sprintf(sqlBuf,"select sum(onlinetime) as atime,sdate,a.userid,c.id from msasrvreport a,msauser b ,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and  b.groupid0=%d and a.userid=b.userid and b.groupid0=c.id group by sdate,a.userid,c.id,b.groupid0 order by sdate,b.groupid0,atime desc",caSdate1,caSdate2,gid);
    }  
    printf("sqlBufb:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor == NULL){
		sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		system(caLabel);/*建立目录和正在处理标志*/  
		return 0;
	}

	tablePtr=lt_dbput_table(dbPtr,"llist");
	tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    	if(strcmp(oldDate,tempRow[1])!=0){/*day change 全变了，新的一行加时间*/
    		if(oldDate[0]!='\0'){/*写入旧的一行*/	
				for(i=0;i<100;i++){
					if(gnamelist[i].gid==-1){
						break;
					}
					sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);
				}
				lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
				memset(caLine,0,sizeof(caLine));

				if(lang==0){//Ch
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
					strgb2utf8(caLine,tmpLine,sizeof(caLine));
					memset(caLine,0,sizeof(caLine));
					sprintf(caLine,"%s",tmpLine);
				}else if(lang==1){//Eng
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Time</td>");
				}
				for(i=0;i<100;i++){/*时间*/
					if(gnamelist[i].gid==-1){
						break;
					}
					sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
				}

				lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
				memset(caLine,0,sizeof(caLine));
				if(lang==0){//Ch
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
					strgb2utf8(caLine,tmpLine,sizeof(caLine));
					memset(caLine,0,sizeof(caLine));
					sprintf(caLine,"%s",tmpLine);
				}else if(lang==1){//Eng
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Average</td>");
				}

				for(i=0;i<100;i++){/*平均*/
					if(gnamelist[i].gid==-1){
						break;
					}

					if(gnamelist[i].gflow){
						sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime/gnamelist[i].gflow));
					}else{
						sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>0:0:0</td>",caLine);
					}
				}
				lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
    		}

    		oldgroupid=-1;
	  		oldsrvid=-1;
    		memset(caLine,0,sizeof(caLine));
    		for(i=0;i<100;i++){
		    	gnamelist[i].gflow=0;
		    	gnamelist[i].gtime=0;
		    }

    		sprintf(oldDate,"%s",tempRow[1]);
    		memset(caLine,0,sizeof(caLine));
			if(lang==0){//Ch
				sprintf(caLine,"<tr height=19 style='height:14.25pt'>\r\n \
  <td rowspan=3 height=57 class=xl488922 style='border-bottom:.5pt solid black;\r\n \
  height:42.75pt;border-top:none'>%s</td> \r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;\r\n \
 width:61pt'>上网人数</td>",oldDate);
                strgb2utf8(caLine,tmpLine,sizeof(caLine));
				memset(caLine,0,sizeof(caLine));
				sprintf(caLine,"%s",tmpLine);
			}else if(lang==1){//Eng
				sprintf(caLine,"<tr height=19 style='height:14.25pt'>\r\n \
  <td rowspan=3 height=57 class=xl488922 style='border-bottom:.5pt solid black;\r\n \
  height:42.75pt;border-top:none'>%s</td> \r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;\r\n \
 width:61pt'>Online number</td>",oldDate);
			}

    		
    		xgroupid=atol(tempRow[3]);
    		for(i=0;i<100;i++){
				if(gnamelist[i].gid==xgroupid){
					gnamelist[i].gflow++;
					gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
					break;
				}
			}



    	}else{/*keep day*/
    	  	xgroupid=atol(tempRow[3]);
    		for(i=0;i<100;i++){
				if(gnamelist[i].gid==xgroupid){
					gnamelist[i].gflow++;
					gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
					break;
				}
			}
 
        }
        tempRow= ltDbFetchRow(tempCursor);
    }
	ltDbCloseCursor(tempCursor);
	  
	if(oldDate[0]!='\0' ){/*写入旧的一行*/	
		for(i=0;i<100;i++){
			if(gnamelist[i].gid==-1){
				break;
			}
			sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);
		} 
		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);

		if(lang==0){//Ch
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
			strgb2utf8(caLine,tmpLine,sizeof(caLine));
			memset(caLine,0,sizeof(caLine));
			sprintf(caLine,"%s",tmpLine);
		}else if(lang==1){//Eng
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Time</td>");
		}
		
		for(i=0;i<100;i++){/*时间*/
			if(gnamelist[i].gid==-1){
				break;
			}
			sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
		}

		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		if(lang==0){//Ch
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
			  <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
			strgb2utf8(caLine,tmpLine,sizeof(caLine));
			memset(caLine,0,sizeof(caLine));
			sprintf(caLine,"%s",tmpLine);
		}else if(lang==1){//Eng
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
				<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Average</td>");
		}
									

		for(i=0;i<100;i++){/*平均*/
			if(gnamelist[i].gid==-1){
				break;
			}

			if(gnamelist[i].gflow){
				sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime/gnamelist[i].gflow));
			}else{
			  sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>0:0:0</td>",caLine);
			}
		}
		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);

    }
 
    lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);

	if(lang==0){//Ch
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usertimeplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usertimeplt_en.htm",dbPtr->doc,0);
	}
	  
 
	if(lt_page_content==NULL){ 
		sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    system(caLabel);/*建立目录和正在处理标志*/
	    //ltMsgFree(ltMsgPk);
        lt_dbfree(dbPtr);
	    return 0;
	}else{
		lt_page_content=strstr(lt_page_content,"<html>");
		sprintf(caLabel,"%sdeparttime.htm",caTempDir);
		iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		if(iFd >0) {
	       write(iFd,lt_page_content,strlen(lt_page_content));
	    }
	    close(iFd);
		  /*生成html格式报告*/
	}
 
	lt_dbfree(dbPtr);
	printf("usertimereport end ..........\n");
 
    return 0;
} 



/* 服务协议 相关报告 */
int srvreport(char date1[36],char date2[36],int userid,int gid,int otype,char caTempDir[256],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW    tempRow;
    char        sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char        oldname[256];
    char        caSrv[64];
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long        lTime,lTime1,lTime2;
    int         srvindex;
    char        *reportname=NULL;
    char        *lt_page_content=NULL;
    int         iFd; 
    char        caLabel[256];
    unsigned long long lMaxBytes;   
    iFd=-1;       
    
    printf("srvreport start ..........................\n");
	printf("date1:%s\n",date1);
	printf("date2:%s\n",date2);
	printf("userid:%d\n",userid);
	printf("gid:%d\n",gid);
	printf("otype:%d\n",otype);
	printf("caTempDir:%s\n",caTempDir);
	printf("lang:%d\n\n",lang);
	
    ltDbConn *G_DbCon;
    dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (G_DbCon==NULL){
			printf("report:can't open db\n");
			_exit(0);
		}

    if(reportname==NULL){
    	reportname="***";
    }
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);    
    printf("srv huizong reportdir:%s\n",caTempDir);//服务汇总相关报告生成目录：
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) { 
            printf("mkdir daily report dir error!\n");
            return 0;
        }
    } 
  
    userid=0;
    lMaxBytes=1;    
    tempCursor=NULL;
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
    if(lTime1 == 0){
        lTime1 = lTime;
    }
	
	if(lTime2 == 0){
        lTime2 = lTime;
    }
	
	if(lTime1 > lTime){
        lTime1 = lTime;
    }
    if(lTime2>lTime){
		lTime2=lTime;
	}
	 
    nasTimeGetDate(caSdate1,lTime); 
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    

	if(otype==1){/*全部汇总*/
    if(gid==-1){
        sprintf(sqlBuf,"select 'All User', service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by service order by abyte desc ",caSdate1,caSdate2);
    }else{
		if(userid==-1){
			sprintf(sqlBuf,"select c.name,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime,c.id from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by c.id,c.name,service order by c.id,abyte desc ",caSdate1,caSdate2,gid);	
		}else{
			sprintf(sqlBuf,"select b.dispname,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime from msasrvreport a,msauser b where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.userid=%d group by a.userid,service order by a.userid,b.dispname,abyte desc",caSdate1,caSdate2,userid);
		} 
    }
    printf("all huizong  sqlBuf:%s\n",sqlBuf);
	}else if(otype==2){/*部门汇总*/
		if(gid==-1){
	        sprintf(sqlBuf,"select c.name,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime,c.id from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id group by c.id,c.name,service order by c.id,abyte desc ",caSdate1,caSdate2);
		}else{
		    sprintf(sqlBuf,"select c.name,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime,c.id from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id and b.id=%d group by c.id,c.name,service order by c.id,abyte desc ",caSdate1,caSdate2,gid);
		}
        printf("*group huizong  sqlBuf:%s\n",sqlBuf);
		
	}else if(otype==4){/*按日期汇总*/
        if(gid==-1){
			sprintf(sqlBuf,"select sdate,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(ctime) as arequest,sum(onlinetime) as atime from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' group by sdate,service order by sdate asc,abyte desc",caSdate1,caSdate2);
        }else{
			if(userid==-1){
				sprintf(sqlBuf,"select sdate,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime from msasrvreport a,msauser b where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=%d group by sdate,service order by sdate asc,abyte desc",caSdate1,caSdate2,gid);
			}else{
				sprintf(sqlBuf,"select sdate,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime from msasrvreport a where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=%d  group by sdate,service order by sdate asc,abyte desc",caSdate1,caSdate2,userid);
			}
		} 
	printf("*an date huizong  sqlBuf:%s\n",sqlBuf);  
	}else if(otype==3|| otype==7){/*人员汇总*/
		int       i;
		int       iTemp;
		long long *srvflow;
		int       *srvrequest;
		long long usersrvflow;
		int       usersrvrequest;
		char      caLine[25000];
		char      caggg[64];	  
		if(gid==-1){
		    sprintf(sqlBuf,"select b.dispname,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id group by a.userid,b.dispname,service,c.name order by a.userid,abyte desc",caSdate1,caSdate2);
		}else{
		    if(userid==-1){
	    		sprintf(sqlBuf,"select b.username,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=%d and b.groupid0=c.id group by a.userid,b.dispname,service,c.name order by a.userid,abyte desc",caSdate1,caSdate2,gid);
	        }else{
	    		sprintf(sqlBuf,"select b.username,service,sum(ubytes+dbytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.userid=%d and b.groupid0=c.id group by a.userid,b.dispname,service,c.name order by a.userid,abyte desc",caSdate1,caSdate2,userid);
	    	} 
		}	   

		srvflow=(long long *)malloc(sizeof(long long)*(MAX_TOP_SRVNUM+1));
		srvrequest=(int *)malloc(sizeof(int)*(MAX_TOP_SRVNUM+1));
		usersrvflow=0;
		usersrvrequest=0;
		for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
			srvflow[iTemp]=0;
			srvrequest[iTemp]=0;
		}
		printf("人员汇总 3 7 sqlBuf:%s\n",sqlBuf);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL) {
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/
			return 0;
		}

		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"glist");
		for(i=0;i<MAX_TOP_SRVNUM;i++){
			if(strlen(_ltPubInfo->topSrvName[i].srvname)>0){//剔除空白名字
			 lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[i].srvname);
			}else{
			 break;	
			}		
		}


		tablePtr=lt_dbput_table(dbPtr,"llist");
		tempRow= ltDbFetchRow(tempCursor);
		memset(oldname,0,sizeof(oldname));
		memset(caggg,0,sizeof(caggg));
		while(tempRow!=NULL){
			if(strcmp(oldname,tempRow[0])!=0){/*新的一行*/  	    	 
				if(oldname[0]!='\0'){/*新的一行开始写*/						  	
					if(otype==3){
						sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,caggg,usersrvflow);
					}else if(otype==7){
						sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
					}
					
					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
						if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){//剔除空白名字
							 break;	
						}
						if(otype==3){
						   sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
						}else{
						   sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
						}	
					}									
					sprintf(caLine,"%s</tr>",caLine);
					lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
				}
				
				sprintf(oldname,"%s",tempRow[0]);
				sprintf(caggg,"%s",tempRow[4]);
				usersrvflow=0;
				usersrvrequest=0;
				for(i=0;i<MAX_TOP_SRVNUM;i++){
						srvflow[i]=0;
						srvrequest[i]=0;
				}						  
				srvflow[atol(tempRow[1])]=atol(tempRow[2]);
				srvrequest[atol(tempRow[1])]=atol(tempRow[3]);
				usersrvflow=usersrvflow+atol(tempRow[2]);
				usersrvrequest=usersrvrequest+atol(tempRow[3]);  	
			}else{
				srvflow[atol(tempRow[1])]=atol(tempRow[2]);
				srvrequest[atol(tempRow[1])]=atol(tempRow[3]);
				usersrvflow=usersrvflow+atol(tempRow[2]);
				usersrvrequest=usersrvrequest+atol(tempRow[3]);
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	  
		if(oldname[0]!='\0'){/*新的一行开始写*/						  	
			if(otype==3){
				sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%lldK</td>",oldname,caggg,usersrvflow);
			}else if(otype==7){
				sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
			}
			for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
				if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){//剔除空白名字
						break;	
				}
				if(otype==3){
					sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lld</td>",caLine,srvflow[iTemp]);
				}else{
					sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
				} 	
			}
			sprintf(caLine,"%s</tr>",caLine);
			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		}
		lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usersrvplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng  
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usersrvplt_en.htm",dbPtr->doc,0);
		}

		if(lt_page_content==NULL){
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/
			lt_dbfree(dbPtr);
			return 0;
		}else{
			lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
			sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
			iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
			if(iFd >0) {
			  write(iFd,lt_page_content,strlen(lt_page_content));
			}
			close(iFd);
			/*生成html格式报告*/
		} 
		lt_dbfree(dbPtr);	 
		printf("people huizong end .....\n");	
		return 0;
    }else if(otype==5 || otype==6 ){/*部门日期汇总*/
		int  groupnum;
		typedef struct gname_s {
			int             gid;
			char            gname[50];
			long long       gflow;
			int             gtime;
		}nasGname;
		nasGname  gnamelist[100];
		long long gflowlist[100];
		long long gtimelist[100];
		int       i;
		char      caLine[16396];
		char      oldDate[64];
		int       oldgroupid;
		int       oldsrvid;
		int       xgroupid;
		    
		for(i=0;i<100;i++){
			gnamelist[i].gid=-1;
			memset(gnamelist[i].gname,0,50);
			gnamelist[i].gflow=0;
			gnamelist[i].gtime=0;
			gflowlist[i]=0;
			gtimelist[i]=0;
		}
	    
		/*显示部门 */
		groupnum=0;
		if(gid==-1){
	    sprintf(sqlBuf,"select id,name from msagroup order by groupid asc ");
		}else{
		  sprintf(sqlBuf,"select id,name from msagroup where id=%d order by id asc ",gid);
		}
        printf("group date huizong  5 6 sqlBuf:%s\n",sqlBuf);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
			    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			    system(caLabel);/*建立目录和正在处理标志*/
		    	//ltMsgFree(ltMsgPk);
		        return 0;
		}
		dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    tablePtr=lt_dbput_table(dbPtr,"glist");
		tempRow= ltDbFetchRow(tempCursor);
	    while(tempRow!=NULL){
	    	gnamelist[groupnum].gid=atol(tempRow[0]);
	    	sprintf(gnamelist[groupnum].gname,"%s",tempRow[1]);
	    	lt_dbput_recordvars(tablePtr,1,"groupname",LT_TYPE_STRING,tempRow[1]);
	    	tempRow= ltDbFetchRow(tempCursor);
	    	groupnum++;
	    	if(groupnum>99){
	    		break;
	    	}
	    }
		ltDbCloseCursor(tempCursor);
		oldgroupid=-1;
		oldsrvid=-1;
		memset(oldDate,0,sizeof(oldDate));
		if(gid==-1){
	     sprintf(sqlBuf,"select a.sdate as xdate,service,c.groupid ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.id from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id group by a.sdate,service,c.id order by xdate asc,service,c.id,abyte desc ",caSdate1,caSdate2);
		}else{
		   sprintf(sqlBuf,"select a.sdate as xdate,service,c.groupid ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.id from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by a.sdate,service,c.id order by xdate asc,service,c.id,abyte desc ",caSdate1,caSdate2,gid);
		}
	
	    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL) {
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/
			return 0;
		}
		tablePtr=lt_dbput_table(dbPtr,"llist");
		tempRow= ltDbFetchRow(tempCursor);
	    while(tempRow!=NULL){
	    	if(strcmp(oldDate,tempRow[0])!=0){/*day change 全变了，新的一行加时间*/    	
	    		if( oldDate[0]!='\0' ){/*写入旧的一行*/	
	    		    for(i=0;i<100;i++){   		  	
							if(gnamelist[i].gid==-1){
								break;
							}
							if(otype==5){
								sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[i].gflow);
							}else{
								sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
							}
				    }
	    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
	    			if(otype==5){
					       sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>Totle Flow</td>");
					}else if(otype==6){
					       sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>Totle Time</td>");
					}
	 			    
					for(i=0;i<100;i++){
						if(gnamelist[i].gid==-1){
							break;
						}
						if(otype==5){
							sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gflowlist[i]);
						}else{
							sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gtimelist[i]));
						}
					}
	    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
	    		}
	    		oldgroupid=-1;
		  		oldsrvid=-1;
	    		memset(caLine,0,sizeof(caLine));
	    		for(i=0;i<100;i++){
			    	gnamelist[i].gflow=0;
			    	gnamelist[i].gtime=0;
			    	gflowlist[i]=0;
			    	gtimelist[i]=0;
			    }
			    sprintf(caSrv,"%s","??");
				sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);			 		  
	    		sprintf(oldDate,"%s",tempRow[0]);
	    		sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'>%s</td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",tempRow[0],caSrv);
	    		oldsrvid=atol(tempRow[1]);
	    		xgroupid=atol(tempRow[2]);
	    		for(i=0;i<100;i++){
	    			if(gnamelist[i].gid==xgroupid){
	    				 gnamelist[i].gflow=atol(tempRow[3]);
			    		 gnamelist[i].gtime=atol(tempRow[4]);
			    		 gflowlist[i]=gflowlist[i]+gnamelist[i].gflow;
			    		 gtimelist[i]=gtimelist[i]+gnamelist[i].gtime;
			    		 break;
	    			}
			    }
	    	}else{/*keep day*/
					if(oldsrvid!=atol(tempRow[1])){/*服务变了,新的一行开始*/
						if( oldsrvid!=-1 ){
							for(i=0;i<100;i++){
								if(gnamelist[i].gid==-1){
									break;
								}
								if(otype==5){
									sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[i].gflow);
								}else{
									sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
								}
							}
						    lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
					    }
						oldgroupid=-1;
						oldsrvid=-1;
						for(i=0;i<100;i++){
							gnamelist[i].gflow=0;
							gnamelist[i].gtime=0;
						}
						oldsrvid=atol(tempRow[1]);
						xgroupid=atol(tempRow[2]);
						memset(caLine,0,sizeof(caLine));
						sprintf(caSrv,"%s","??");
						sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
						sprintf(oldDate,"%s",tempRow[0]);
						sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",caSrv);	
						oldsrvid=atol(tempRow[1]);
						xgroupid=atol(tempRow[2]);
						for(i=0;i<100;i++){
							if(gnamelist[i].gid==xgroupid){
								gnamelist[i].gflow=atoll(tempRow[3]);
								gnamelist[i].gtime=atol(tempRow[4]);
								gflowlist[i]=gflowlist[i]+gnamelist[i].gflow;
								gtimelist[i]=gtimelist[i]+gnamelist[i].gtime;
								break;
							}
						}
	    	        }else{/*服务没变，增加部门*/
	    	  	        xgroupid=atol(tempRow[2]);
	    		        for(i=0;i<100;i++){
			    			if(gnamelist[i].gid==xgroupid){
			    				gnamelist[i].gflow=atol(tempRow[3]);
					    		gnamelist[i].gtime=atol(tempRow[4]);
					    		gflowlist[i]=gflowlist[i]+gnamelist[i].gflow;
				    		  gtimelist[i]=gtimelist[i]+gnamelist[i].gtime;
					    		break;
			    			}
				        }
				    
	    	        }/*end 服务没变，增加部门*/
	        }
	        tempRow= ltDbFetchRow(tempCursor);
	    }
	    ltDbCloseCursor(tempCursor);
		
		if(oldDate[0]!='\0' ){/*写入旧的一行*/	
			for(i=0;i<100;i++){					
				if(gnamelist[i].gid==-1){
					break;
				}
				if(otype==5){
					sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[i].gflow);
				}else{
					sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
				}
			}
			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		}
		lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvgroupdateplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvgroupdateplt_en.htm",dbPtr->doc,0);
		}
	 
	
		if(lt_page_content==NULL){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
			system(caLabel); 
			lt_dbfree(dbPtr);
			return 0;
		}else{
			lt_page_content=strstr(lt_page_content,"<!DOCTYPE");  
			sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
			iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
			if(iFd >0) {
			 write(iFd,lt_page_content,strlen(lt_page_content));
			}
			close(iFd);
		   /*生成html格式报告*/
		}	   
		lt_dbfree(dbPtr);
		return 0;
    }//
    printf("sqlBuf:%s\n",sqlBuf);
	tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor == NULL){
		sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    system(caLabel);/*建立目录和正在处理标志*/
	    return 0;
	}
    if(otype==1){
        printf(caLabel,"%sdata.ini",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	    if(iFd == (-1)){
	      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
    	  system(caLabel);/*建立目录和正在处理标志*/
    	  ltDbCloseCursor(tempCursor);
          return 0;
		}									
	}

    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    tempRow=ltDbFetchRow(tempCursor);
    memset(oldname,0,sizeof(oldname));
    srvindex=1;
    while(tempRow!=NULL){
        if(strcmp(oldname,tempRow[0])==0){
			sprintf(caSrv,"%s","??");		
			if(otype==1){ 
				sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
			}else{
				sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
			}
			  
			lt_dbput_recordvars(tablePtr,5,
				"gname",LT_TYPE_STRING,"",
				"sname",LT_TYPE_STRING,caSrv,
				"sc",LT_TYPE_LONGLONG,atoll(tempRow[3]),
				"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"st",LT_TYPE_STRING,time2str(atoll(tempRow[4])) 
			);
					
			if(otype==1){ 
				sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[2]));
				write(iFd, caLabel, strlen(caLabel));	
				if(lMaxBytes<atoll(tempRow[2])){
				  lMaxBytes=atoll(tempRow[2]);
				}
			}					
        }else{ 
			char caGname[512]; 
			sprintf(caSrv,"%s","??");
			if(otype==1){
				sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
			}else{
			  sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
			}
		    if(otype==2){
	          sprintf(caGname,"%s",tempRow[0]);
			}else{
			  sprintf(caGname,"%s",tempRow[0]);
			}
			sprintf(oldname,"%s",tempRow[0]);
			lt_dbput_recordvars(tablePtr,5,
				"gname",LT_TYPE_STRING,caGname,
				"sname",LT_TYPE_STRING,caSrv,
				"sc",LT_TYPE_LONGLONG,atoll(tempRow[3]),
				"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"st",LT_TYPE_STRING,time2str(atoll(tempRow[4]))
			);
			if(otype==1){  
			    sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[2]));
				write(iFd, caLabel, strlen(caLabel));	
				if(lMaxBytes<atoll(tempRow[2])){
	         		lMaxBytes=atoll(tempRow[2]);
				}						
			}   	    
		}
		if(otype==1){ 
			srvindex++;
		}
     
		tempRow= ltDbFetchRow(tempCursor);
	}
    ltDbCloseCursor(tempCursor);
    if(otype==1){
		close(iFd);							  
        sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",caTempDir,caTempDir,lMaxBytes);
	    system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"srvreporta.gif\">" );
	}
  /* 结果输出  */
    lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
    if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvreportplta.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvreportplta_en.htm",dbPtr->doc,0);
	}
 
    if(lt_page_content==NULL){
	    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
    	lt_dbfree(dbPtr);
        return 0;
	}else{  
	    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	    sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
		iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
           write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
	  /*生成html格式报告*/
    }
    lt_dbfree(dbPtr);
    printf("srvreport end ..................................\n");
    return 0;
}

  

/*网络流量  相关报告*/
int connectflow(char date1[36],char date2[36],int userid,int gid,int lUnit,char caTempDir[128],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024]; 
    struct nasFlowCount_s {
        char               caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    }sCount[35];
 
    unsigned long iMaxItem;  
    unsigned long long lUbytes,lDbytes,lMaxBytes;
    int  i,iMax;
    long lStep,lTime,lTime1,lTime2,lTime0;
    char caSdate[32],caTime[32],caItem[16];
    char caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    char caLabel[512];
    char caTemp[8192];
   
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;  
    char caEdate[32];
    char reportname[123];
    char *lt_page_content;
    int  iFd;
    unsigned long long *lTop;
    int  iTemp;
    char oldgname[128];
	char egname[128];
    int  topuser=100;
    unsigned long long  lMax;     
    ltDbConn *G_DbCon;
    printf("connectflow start..............\n");
	printf("date1:%s\n",date1);
	printf("date2:%s\n",date2);
	printf("userid:%d\n",userid);
	printf("gid:%d\n",gid);
	printf("lUnit:%d\n",lUnit);
	printf("caTempDir:%s\n",caTempDir);
	printf("lang:%d\n\n",lang);
    /*连接数据库*/
    dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (G_DbCon==NULL){
        printf("report:can't open db\n");
		_exit(0);
	}
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);  
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)){
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)){
        lTime2 = nasCvtLongTime(caEdate,"24:00:00");
    }else{
        lTime2 = lTime;
    }
    
    sprintf(reportname,"report%d",lUnit);
    if(reportname==NULL){
    	 sprintf(reportname,"*****");
    }
  
    if(ltFileIsExist(caTempDir)!=1) {
      if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {         
        return 0;
      }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
    tempCursor=NULL;
    iMaxItem = 32;
    dbPtr=NULL;
    lt_page_content=NULL;
    lTop=NULL;
    if(lUnit==1){/*按天汇总 */
        memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
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
			printf("caSdate+8:%s\n",caSdate+8);
            memcpy(sCount[i].caItem,caSdate+8,2);
			printf("sCount[i].caItem:%s\n",sCount[i].caItem);
            lTime0 = lTime0 + lStep;
        }
        nasTimeGetDate(caSdate1,lTime1);
        nasTimeGetDate(caSdate2,lTime2);
        nasTimeGetTime(caTime1,lTime1);
	    nasTimeGetTime(caTime2,lTime2);
        caTime1[2]=0;
	    caTime2[2]=0;	 
	    sprintf(caSdate,"%s",date1);
        if(gid==-1){
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport where ((substring(sdate ,3,8) > '%s') or(substring(sdate ,3,8) = '%s' and hours>='%s')) and ((substring(sdate,3,8) < '%s') or(substring(sdate ,3,8) = '%s' and hours<='%s')) group by sdate",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
	    }else{
			if(userid!=-1){
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport where userid = %d and ((substring(a.sdate ,3,8) > '%s') or(substring(a.sdate ,3,8) = '%s' and hours>='%s')) and ((substring(a.sdate ,3,8) < '%s') or(substring(a.sdate ,3,8) = '%s' and hours<='%s')) group by sdate ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
			}else{
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport a,msauser b where a.userid = b.userid and b.groupid0=%d and ((substring(a.sdate ,3,8) > '%s') or(substring(a.sdate ,3,8) = '%s' and hours>='%s')) and ((substring(a.sdate ,3,8) < '%s') or(substring(a.sdate ,3,8) = '%s' and hours<='%s'))group by sdate ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
			}       
		}
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);   
    }else if(lUnit==3){/*用户*/	
        lStep = 24 * 3600;
        if(lTime1 == 0) {
			lTime1 = lTime-31*lStep;
        }
        if(lTime2>lTime){
			lTime2=lTime;
		}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
		for(iTemp=0;iTemp<topuser;iTemp++){
			lTop[iTemp]=0;
		}
	    nasTimeGetDate(caSdate1,lTime1);
  	    nasTimeGetDate(caSdate2,lTime2);

		if(gid==-1){
		    sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1, c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by a.userid,b.dispname,c.name order by abyte desc limit %d ",caSdate1,caSdate2,topuser);
		}else{
		 	sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d",caSdate1,caSdate2,gid,topuser);
		}  
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==4){/*用户上流量排序*/
        lStep = 24 * 3600;
        if(lTime1 == 0) {
           lTime1 = lTime-31*lStep;
        }   
        if(lTime2>lTime){
		   	lTime2=lTime;
		}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
		for(iTemp=0;iTemp<topuser;iTemp++){//限制用户显示数量
			lTop[iTemp]=0;
		}
        nasTimeGetDate(caSdate1,lTime1);
  	    nasTimeGetDate(caSdate2,lTime2);
	    if(gid==-1){
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by a.userid,b.dispname,c.name order by abyte desc limit %d ",caSdate1,caSdate2,topuser);
		}else{
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d ",caSdate1,caSdate2,gid,topuser);
		}
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==5){/*用户下流量排序*/
        lStep = 24 * 3600;
        if(lTime1 == 0){
			lTime1 = lTime-31*lStep;
        }
        if(lTime2>lTime){
			lTime2=lTime;
		}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
		for(iTemp=0;iTemp<topuser;iTemp++){
			lTop[iTemp]=0;
		}
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		if(gid==-1){
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by a.userid,b.dispname,c.name order by abyte desc limit %d",caSdate1,caSdate2,topuser);
		}else{
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d",caSdate1,caSdate2,gid,topuser);
		}	   
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==6){//部门人员汇总
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));
		sprintf(sqlBuf,"select b.dispname,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime) from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and  b.groupid0=c.id group by b.dispname,c.name order by mybytes desc",caSdate1,caSdate2);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	}else if(lUnit==7){//部门人员汇总
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));  
		sprintf(sqlBuf,"select b.dispname,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by b.dispname,c.name,b.groupid0 order by b.groupid0,mybytes desc",caSdate1,caSdate2);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==8){//部门日期汇总
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));		  
		sprintf(sqlBuf,"select c.name,a.sdate,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and  b.groupid0=c.id group by a.sdate,c.name order by a.sdate,mybytes desc",caSdate1,caSdate2);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	}  
    printf("sqlBuf:%s\n\n",sqlBuf);	
    if(tempCursor == NULL) {
		sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
        return 0;
    }

    if(lUnit==1 || lUnit==2 ){ //2已删除(按小时的)
		lMaxBytes = 1;
		iMax = 0;
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			sprintf(caItem,"%s",tempRow[0]);
			lUbytes=atoll(tempRow[1]);
			lDbytes=atoll(tempRow[2]);
			printf("caItem:%s\n",caItem);
			if(lMaxBytes <= (lUbytes + lDbytes) / 1024) {
			   lMaxBytes = (lUbytes + lDbytes) / 1024;
			}
			for(i=0;i<iMaxItem;i++) {
			    if(strcmp(sCount[i].caItem,caItem+8)==0) {
				    printf("sCount[i].caItem:%s\n",sCount[i].caItem);
					if(iMax < i+1) iMax = i+1;
					sCount[i].lUbytes = lUbytes / 1024;
					sCount[i].lDbytes = lDbytes / 1024;
					printf("sCount[i].lUbytes:%lld\n",sCount[i].lUbytes);
					printf("sCount[i].lDbytes:%lld\n",sCount[i].lDbytes);
					break;
			    }
			}
			tempRow= ltDbFetchRow(tempCursor);
		}   
		ltDbCloseCursor(tempCursor);
		if(lUnit==1){
			iMaxItem = iMax;
		}else{
			iMaxItem = 24;
		}
	
		dbPtr=lt_dbinit();  
		sprintf(caLabel,"%sdata.ini",caTempDir);
		iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		if(iFd==(-1)){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
			system(caLabel);/*建立目录和正在处理标志*/
			ltDbCloseCursor(tempCursor);
			return 0;
		}
	
		sprintf(caLabel,"%s--%s",caSdate,caEdate);
		lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
		lt_dbput_rootvars(dbPtr,1,"reportname",reportname); 
	   
		if(lUnit==1){
			lt_dbput_rootvars(dbPtr,1,"xunit","Day");
		}else{
			lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
		}
		
		tablePtr=lt_dbput_table(dbPtr,"list2");
		for(i=0;i<iMaxItem;i++) {
			lt_dbput_recordvars(tablePtr,4,
				"mytime",LT_TYPE_STRING,sCount[i].caItem,
				"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
				"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
				"abyte",LT_TYPE_LONGLONG,sCount[i].lUbytes+sCount[i].lDbytes
			);
			sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes+sCount[i].lDbytes,sCount[i].lDbytes,sCount[i].lUbytes);
			write(iFd, caLabel, strlen(caLabel));	
		}
		close(iFd); 
		if(lUnit==1){ 
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowplt.pl  %sdata.ini %sflowreport11_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file1","flowreport11_1.jpg");
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowpltline.pl  %sdata.ini %sflowreport22_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file2","flowreport22_1.jpg");	    
		}else{
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowplt.pl  %sdata.ini %sflowreport11_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file1","flowreport11_2.jpg");
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowpltline.pl  %sdata.ini %sflowreport22_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file2","flowreport22_2.jpg");	
		}
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/flowreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/flowreportplt_en.htm",dbPtr->doc,0);
		}
 	}else if(lUnit==3 || lUnit==4  || lUnit==5 ){ //3用户总流量排序 4用户上流量排序 5用户下流量排序 
		iTemp=0;
		lMax=0;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"flowlist");
		tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			lt_dbput_recordvars(tablePtr,6,
				"iIndex",LT_TYPE_LONG,iTemp,
				"uname",LT_TYPE_STRING,tempRow[1],
				"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"lubyte",LT_TYPE_LONGLONG,atoll(tempRow[3]),
				"ldbyte",LT_TYPE_LONGLONG,atoll(tempRow[4]),
				"groupname",LT_TYPE_STRING,tempRow[5]
			);
			if(lUnit==3){
				lTop[iTemp]=atoll(tempRow[2]);
			}else if(lUnit==4){
				lTop[iTemp]=atoll(tempRow[3]);
			}else if(lUnit==5){
				lTop[iTemp]=atoll(tempRow[4]);
			}
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
			system(caLabel);/*建立目录和正在处理标志*/
			ltDbCloseCursor(tempCursor);
			return 0;
		}	
		for(i=0;i<iTemp;i++) {
			ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
			sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
			write(iFd, caLabel, strlen(caLabel));			    
		}
		close(iFd);	
		sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv1.pl  %sdata.ini %sflowreportt.gif %llu ",caTempDir,caTempDir,lMax);
		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreportt.gif\">" );
		
		{
			int i;
			sprintf(caLabel,"%sdata.ini",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
				system(caLabel);/*建立目录和正在处理标志*/
				ltDbCloseCursor(tempCursor);	
				return 0;
			}	
			for(i=0;i<iTemp;i++){ 
				ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
				sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
				write(iFd, caLabel, strlen(caLabel));	
			}
			close(iFd);	
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport13.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file1","flowreport13.jpg"); 
		}
		
		sprintf(caLabel,"%s--%s",caSdate,caEdate);
		lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
		lt_dbput_rootvars(dbPtr,1,"reportname",reportname); 
	
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userreportplt_en.htm",dbPtr->doc,0);
		}
    }else if(lUnit==6 || lUnit==7  || lUnit==8 ){ 	
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"flowlist");   
		tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			if(lUnit==6 ){				
				lt_dbput_recordvars(tablePtr,4,
				"uname",LT_TYPE_STRING," ",
				"groupname",LT_TYPE_STRING,tempRow[1],
				"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"ltime",LT_TYPE_STRING,tempRow[3]);
				sprintf(oldgname,"%s",tempRow[1]);			  
			}else{			 
				if(strcmp(oldgname,tempRow[1])==0){
				    printf("oldgname:%s\n",oldgname);
					memset(egname,0,sizeof(egname));
					memset(egname,' ',strlen(oldgname));
					lt_dbput_recordvars(tablePtr,4,
					"uname",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,egname,
					"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
					"ltime",LT_TYPE_STRING,tempRow[3]);
				}else{
				    lt_dbput_recordvars(tablePtr,4,
					"*",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_STRING," ",
					"ltime",LT_TYPE_STRING," ");
					sprintf(oldgname,"%s",tempRow[1]);
					
					memset(egname,0,sizeof(egname));
					memset(egname,' ',strlen(oldgname));
					lt_dbput_recordvars(tablePtr,4,
					"uname",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,egname,
					"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
					"ltime",LT_TYPE_STRING,tempRow[3]);
				}				
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
		sprintf(caLabel,"%s--%s",caSdate,caEdate);
		lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
		lt_dbput_rootvars(dbPtr,1,"reportname",reportname); 
		
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/groupreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/groupreportplt_en.htm",dbPtr->doc,0);
		}   
	}
 
    if(lt_page_content==NULL){
	    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
        system(caLabel);/*建立目录和正在处理标志*/
    	lt_dbfree(dbPtr);
        return 0;
	}else{
	    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	    sprintf(caLabel,"%sflow%d.htm",caTempDir,lUnit);
	    iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
	  /*生成html格式报告*/
    }
    lt_dbfree(dbPtr);
    printf("connectflow end ...............\n");
    return 0;
}


 
/* part8  WEB 访问相关报告 */  
int webreport(char date1[36],char date2[36],int userid,int gid,int oType,char caTempDir[128],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    int iTemp;
    char xName[64];
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    long lTime,lTime1,lTime2;
    char caLabel[128];
    int  sType;
    
    struct xs_group{
    	char strGroup[128];
		int  idGroup;
	};

    char   	   oldname[256];
	struct     xs_group xmyGroup[256]; /*最多256个部门*/
	char       reportname[64];
    char       *lt_page_content=NULL;
    int        iFd;    
	memset(oldname,0,sizeof(oldname));	  
	
	printf("webreport start..................\n");
	printf("date1:%s\n",date1);
	printf("date2:%s\n",date2);
	printf("userid:%d\n",userid);
	printf("gid:%d\n",gid);
	printf("oType:%d\n",oType);
	printf("caTempDir:%s\n",caTempDir);
	printf("lang:%d\n\n",lang);
	
	sprintf(sqlBuf,"select id,name from msagroup");
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor == NULL) {
      printf(" wu date,not to report!\n");
      return 0;
    }
    tempRow= ltDbFetchRow(tempCursor);
    iTemp=0;
    while(tempRow!=NULL){
    	xmyGroup[iTemp].idGroup=atol(tempRow[0]);
    	sprintf(xmyGroup[iTemp].strGroup,"%s",tempRow[1]);
    	iTemp++;
    	if(iTemp>250){
    		break;
    	}
    	tempRow= ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    sType=1;  
	sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);

    tempCursor=NULL;
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
		lTime2=lTime;
    }
	 
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            printf("make dir error");/// 生成该报告目录错误 
            return 0;
        }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
	/*top30 访问次数的排序*/
	if(oType==1){
        sprintf(reportname,"全部汇总");	
		memset(tempname,0,sizeof(tempname));
        strgb2utf8(reportname,tempname,sizeof(reportname));
	    memset(reportname,0,sizeof(reportname));
	    sprintf(reportname,"%s",tempname);		
	  	if(gid==-1){  
				if(sType==2){
					sprintf(sqlBuf,"select 'All User', host,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from msauserhttpreport where substring(sdate,3,8) >= '%s' and substring(sdate,3,8) <= '%s' group by host,urlsort order by acount desc limit 50",caSdate1,caSdate2);
				}else if(sType==1){
					sprintf(sqlBuf,"select  'All User',host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from msauserhttpreport where substring(sdate,3,8) >= '%s' and substring(sdate,3,8) <= '%s' group by host,urlsort order by abyte desc limit 50",caSdate1,caSdate2);
				}
	    }else{
	    	if(userid==-1){
				if(sType==2){
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by c.name,a.host,a.urlsort order by acount desc limit 30",caSdate1,caSdate2,gid);
				}else if(sType==1){
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id  and b.groupid0=%d group by c.name,a.host,a.urlsort order by abyte desc limit 30",caSdate1,caSdate2,gid);
				}																
			}else{	    		
				if(sType==2){
					sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by a.host,a.urlsort,b.dispname order by acount desc limit 30",caSdate1,caSdate2,userid);
				}else if(sType==1){
					sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by a.host,a.urlsort,b.dispname order by abyte desc limit 30",caSdate1,caSdate2,userid);
				}
	    	} 
	    }
		
	}else if(oType==2){
		sprintf(reportname,"部门汇总");
		memset(tempname,0,sizeof(tempname));
        strgb2utf8(reportname,tempname,sizeof(reportname));
	    memset(reportname,0,sizeof(reportname));
	    sprintf(reportname,"%s",tempname);	
		if(gid==-1){
			if(sType==2){
				sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id group by a.urlsort,a.host,c.name order by c.name,acount desc",caSdate1,caSdate2);
			}else if(sType==1){
				sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id group by a.urlsort,a.host,c.name order by c.name,abyte desc",caSdate1,caSdate2);
			}
		}else{
			if(userid==-1){
				if(sType==2){
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by c.name,a.host,a.urlsort order by acount desc",caSdate1,caSdate2,gid);
				}else if(sType==1){
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id  and b.groupid0=%d  group by c.name,a.host,a.urlsort order by abyte desc",caSdate1,caSdate2,gid);
				}	
			}else{
				if(sType==2){
					sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by b.dispname,a.host,a.urlsort order by acount desc",caSdate1,caSdate2,userid);
				}else if(sType==1){
					sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by b.dispname,a.host,a.urlsort order by abyte desc",caSdate1,caSdate2,userid);
				} 
			} 
		}
		
	}else if(oType==3){
	    sprintf(reportname,"用户汇总");
		memset(tempname,0,sizeof(tempname));
        strgb2utf8(reportname,tempname,sizeof(reportname));
	    memset(reportname,0,sizeof(reportname));
	    sprintf(reportname,"%s",tempname);	
		if(gid==-1){
			if(sType==2){
				sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid group by a.userid,a.urlsort, a.host,b.dispname order by a.userid,acount desc",caSdate1,caSdate2);
			}else if(sType==1){
				sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid  group by a.userid,a.urlsort, a.host,b.dispname order by a.userid,abyte desc",caSdate1,caSdate2);
			}
		}else{
			if(userid==-1){
				if(sType==2){
					sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid, a.host,a.urlsort,b.dispname order by a.userid,acount desc",caSdate1,caSdate2,gid);
				}else if(sType==1){
					sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid, a.host,a.urlsort,b.dispname order by a.userid,abyte desc",caSdate1,caSdate2,gid);
				}
			}else{
				if(sType==2){
					sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by a.host,b.dispname,a.urlsort order by acount desc",caSdate1,caSdate2,userid);
				}else if(sType==1){
					sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.userid=%d  group by a.host,b.dispname,a.urlsort order by abyte desc",caSdate1,caSdate2,userid);
				} 
			} 
		}
	}else if(oType==4){
		sprintf(reportname,"日期汇总");
		memset(tempname,0,sizeof(tempname));
        strgb2utf8(reportname,tempname,sizeof(reportname));
	    memset(reportname,0,sizeof(reportname));
	    sprintf(reportname,"%s",tempname);	
		if(gid==-1){
			if(sType==2){
					sprintf(sqlBuf,"select a.sdate, host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,a.urlsort from msauserhttpreport a where sdate >= '%s' and sdate <= '%s' group by a.sdate,a.urlsort,host order by a.sdate,acount desc",caSdate1,caSdate2);
			}else if(sType==1){
					sprintf(sqlBuf,"select  a.sdate,host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,a.urlsort from msauserhttpreport a where sdate >= '%s' and sdate <= '%s' group by a.sdate,host,a.urlsort order by a.sdate,abyte desc",caSdate1,caSdate2);
			}
		}else{
			if(userid==-1){
				if(sType==2){
					sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid  and b.groupid=%d group by a.sdate,a.host,a.urlsort order by a.sdate,acount desc",caSdate1,caSdate2,gid);
				}else if(sType==1){
					sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=b.userid and b.groupid=%d  group by a.sdate,a.host,a.urlsort order by a.sdate,abyte desc ",caSdate1,caSdate2,gid);
				}  
			}else{
				if(sType==2){
					sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and  a.userid=%d  group by a.sdate, a.host,a.urlsort order by a.sdate,acount desc",caSdate1,caSdate2,userid);
				}else if(sType==1){
					sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid=%d  group by a.sdate, a.host,a.urlsort order by a.sdate,abyte desc",caSdate1,caSdate2,userid);
				}  
			} 
		} 
	}

    printf("sql:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor == NULL) {
	    sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
        return 0;
    }
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"list1");
    tempRow= ltDbFetchRow(tempCursor);
    iTemp=0;
    while(tempRow!=NULL){     
        if(strcmp(oldname,tempRow[0])==0){
            if(iTemp<30){
			    lt_dbput_recordvars(tablePtr,5,
					"uname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_LONG,atol(tempRow[2]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
					"lrequest",LT_TYPE_STRING,tempRow[4],
					"usort",LT_TYPE_STRING,_ltPubInfo->_urlSortlist[atol(tempRow[5])].sortname);
			    }
				iTemp++;				
			}else{
			    sprintf(oldname,"%s",tempRow[0]);
			    sprintf(xName,"<b>%s</b>",oldname);
			    lt_dbput_recordvars(tablePtr,4,
					"uname",LT_TYPE_STRING,xName,
					"lbyte",LT_TYPE_STRING,"",
					"ltime",LT_TYPE_STRING,"",
					"lrequest",LT_TYPE_STRING,"");
			   
			    lt_dbput_recordvars(tablePtr,5,
					"uname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_LONG,atol(tempRow[2]),
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
					"lrequest",LT_TYPE_STRING,tempRow[4],
					"usort",LT_TYPE_STRING,_ltPubInfo->_urlSortlist[atol(tempRow[5])].sortname);
			    iTemp=0;
			}
		tempRow= ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    /* 结果输出  */
	memset(caSdate1,0,sizeof(caSdate1));
    memset(caSdate2,0,sizeof(caSdate2));
    nasCvtStime(lTime1,caSdate1,caTime1);
    nasCvtStime(lTime2,caSdate2,caTime2);
    caTime1[5]=0;
    caTime2[5]=0;
    sprintf(caLabel,"Web Report:  %s--%s",caSdate1,caSdate2);
   
	lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
	lt_dbput_rootvars(dbPtr,1,"reportname",reportname);
	if(lang==0){//Ch
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/webreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/webreportplt_en.htm",dbPtr->doc,0);
	}
    
    if(lt_page_content==NULL){
		sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
		system(caLabel);/*建立目录和正在处理标志*/
		lt_dbfree(dbPtr);
		return 0;
	}else{
		lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		sprintf(caLabel,"%sweb%d.htm",caTempDir,oType);  
		iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		if(iFd >0) {
		  write(iFd,lt_page_content,strlen(lt_page_content));
		} 
	    close(iFd);
		/*生成html格式报告*/
    }
   
    lt_dbfree(dbPtr);
    printf("webreport end...................\n");
    return 0;
}
