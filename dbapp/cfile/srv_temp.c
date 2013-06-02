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

#include "nas.h"

char dbName[64];
char dbUser[64];
char dbPass[64];
 
static char *time2str(int itime){
	static char caTime[32];
	int ih;
	int im;
	int is;
	int itemp;
	if(itime==0){
		return "00:00:00";
	}
	caTime[0]='\0';
	memset(caTime,0,sizeof(caTime));
	ih=itime/3600;
	itemp=itime%3600;
	im=itemp/60;
	is=itemp%60;
	sprintf(caTime,"%02d:%02d:%02d",ih,im,is);
	return caTime;
}
 
/*部门网络时间汇总 */
int usertimereport(char date1[36],char date2[36],char caTempDir[256],int gid,int lang)
{

    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long lTime,lTime1,lTime2;
    char reportname[123];
    char *lt_page_content;
    int  iFd;
    char caLabel[256];
 
    ltDbConn *G_DbCon;
    sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
    sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
    sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (G_DbCon==NULL){
				printf("report:can't open db\n");
				_exit(0);
	}
    iFd=-1;
  
	sprintf(reportname,"部门网络时间汇总");
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

    printf("---------------部门网络时间汇总---------------caTempDir:%s \n",caTempDir);
 
    sprintf(caSdate,"%s",date1);

    sprintf(caEdate,"%s",date2);

    printf("caSdate::%s ;; caEdate::%s \n",caSdate,caEdate);

    tempCursor=NULL;

    lTime = time(0);

    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }

    else {
        lTime1 = 0;
    }

    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }

    else {
        lTime2 = 0;
    }

    printf("lTime1::%d ;; lTime2::%d \n",lTime1,lTime2);

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

		} nasGname;

    nasGname gnamelist[100];
 
    int      i;

    char     caLine[30000];
 
    int  oldgroupid;

	int  oldsrvid;

	int  xgroupid;

	char oldDate[64];

    for(i=0;i<100;i++){

    	gnamelist[i].gid=-1;

    	memset(gnamelist[i].gname,0,50);

    	gnamelist[i].gflow=0;

    	gnamelist[i].gtime=0;

    }

	  /*显示部门 */

	  groupnum=0;
	  
      if(gid==-1){
	     sprintf(sqlBuf,"select groupid,groupname from nasgroup order by groupid asc ");
      }else{
	  	 sprintf(sqlBuf,"select groupid,groupname from nasgroup where groupid=%d order by groupid asc ",gid);
	  }	 

	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

	  if(tempCursor == NULL) {

		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	system(caLabel);/*建立目录和正在处理标志*/
	        return 0;

	  }

	  dbPtr=lt_dbinit();

      lt_db_htmlpage(dbPtr,"gb2312");

      tablePtr=lt_dbput_table(dbPtr,"glist");

	  tempRow= ltDbFetchRow(tempCursor);

      while(tempRow!=NULL){

    	gnamelist[groupnum].gid=atol(tempRow[0]);

    	sprintf(gnamelist[groupnum].gname,"%s",tempRow[1]);

    	lt_dbput_recordvars(tablePtr,1,

		            "groupname",LT_TYPE_STRING,tempRow[1]);

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
	  sprintf(sqlBuf,"select sum(time) as atime,sdate,a.userid,c.groupid from nasuseraccount a,nasuser b ,nasgroup c \
 where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid  group by sdate,userid order by sdate,b.groupid,atime desc",caSdate1,caSdate2);

      }else{
	  	 sprintf(sqlBuf,"select sum(time) as atime,sdate,a.userid,c.groupid from nasuseraccount a,nasuser b ,nasgroup c \
 where sdate>='%s' and sdate<='%s' and  b.groupid=%d and a.userid=b.userid and b.groupid=c.groupid  group by sdate,userid order by sdate,b.groupid,atime desc",caSdate1,caSdate2,gid);

	  }
printf("sqlBuf :%s\n",sqlBuf);

    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

	  if(tempCursor == NULL) {

		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    	system(caLabel);/*建立目录和正在处理标志*/  
	        return 0;

	  }

	  tablePtr=lt_dbput_table(dbPtr,"llist");

	  tempRow= ltDbFetchRow(tempCursor);
 
     while(tempRow!=NULL){

    	if( strcmp(oldDate,tempRow[1])!=0){/*day change 全变了，新的一行加时间*/
 
    		if( oldDate[0]!='\0' ){/*写入旧的一行*/	

    		  for(i=0;i<100;i++){
   
    		  	if(gnamelist[i].gid==-1){

    		  		break;
    		  	}

    		  	sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%d</td>",caLine,gnamelist[i].gflow);

			    }
 
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);

    			memset(caLine,0,sizeof(caLine));

				if(lang==0){//Ch
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
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
 
    			 memset(caLine,0,sizeof(caLine));

				 if(lang==0){//Ch
					 sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
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

	  if( oldDate[0]!='\0' ){/*写入旧的一行*/	

    		   for(i=0;i<100;i++){
 
    		  	if(gnamelist[i].gid==-1){

    		  		break;

    		  	}

    		  	sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%d</td>",caLine,gnamelist[i].gflow);

			    }
 
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
 
				if(lang==0){//Ch
					sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
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
		   lt_page_content=ltPltFileParse("/app/ns/html/main/usertimeplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/ns/html/main/usertimeplt_en.htm",dbPtr->doc,0);
		}
	  
 
	    if(lt_page_content==NULL){ 

				  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    	      system(caLabel);/*建立目录和正在处理标志*/
	    	      //	ltMsgFree(ltMsgPk);
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
 
      return 0;

} 

/* 服务协议 相关报告 */
int srvreport(char date1[36],char date2[36],int userid,int gid,int otype,char caTempDir[256],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char       oldname[256];
    char       caSrv[64];
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long lTime,lTime1,lTime2;
    int  srvindex;
    char *reportname;
    char *lt_page_content;
    int  iFd; 
    char caLabel[256];
    unsigned long long lMaxBytes;   
    iFd=-1;       
    
   /*
       if(checkRight(confd,ltMsgPk,13,lt_MMHead)==-1){
			return -1;
		}*/
    
   // reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
   
   
    ltDbConn *G_DbCon;
    sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
    sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
    sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);

    if(reportname==NULL){

    	reportname="***";

    }
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    printf("caSdate::%s ;; caEdate::%s ;; otype::%d \n",date1,date2,otype);
  
   lTime1 = nasCvtLongTime(caSdate,"00:00:00");
   lTime2 = nasCvtLongTime(caEdate,"23:59:59");
   nasTimeGetDate(caSdate1,lTime1);
   nasTimeGetDate(caSdate2,lTime2);
   
  
    printf("服务汇总相关报告生成目录：%s\n",caTempDir);
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
    }
    else {
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }
    else {
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
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    

if(otype==1){/*全部汇总*/
  	if(gid==-1){
        sprintf(sqlBuf,"select 'All User', service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest ,sum(onlinetime) as atime  \
from nassrvreport where sdate>='%s' and sdate<='%s' group by service order by abyte desc ",caSdate1,caSdate2);
    }else{
    		if(userid==-1){
    			  sprintf(sqlBuf,"select c.groupname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,,sum(onlinetime) as atime,c.groupid \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by c.groupid,service order by c.groupid,abyte desc ",caSdate1,caSdate2,gid);
	
    		}else{
    				 sprintf(sqlBuf,"select b.dispname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime \
from nassrvreport a,nasuser b where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.userid=%d group by a.userid,service order by a.userid,abyte desc ",caSdate1,caSdate2,userid);
	
    	  } 
    }
    printf("*全部汇总   sqlBuf:%s\n",sqlBuf);
		
}else if(otype==2){/*部门汇总*/
  	if(gid==-1){
        sprintf(sqlBuf,"select c.groupname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime,c.groupid \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid group by c.groupid,service order by c.groupid,abyte desc ",caSdate1,caSdate2);
	  }else{
	  		 sprintf(sqlBuf,"select c.groupname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime,c.groupid \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by c.groupid,service order by c.groupid,abyte desc ",caSdate1,caSdate2,gid);
	  }
	   printf("*部门汇总  sqlBuf:%s\n",sqlBuf);
		
}else if(otype==4){/*按日期汇总*/
	 if(gid==-1){
        sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime \
from nassrvreport where sdate>='%s' and sdate<='%s' group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2);
   }else{
   	if(userid==-1){
   		sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime \
from nassrvreport a,nasuser b where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=%d  group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2,gid);
   	}else{
   		sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,sum(onlinetime) as atime \
from nassrvreport a where sdate>='%s' and sdate<='%s' and a.userid=%d  group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2,userid);
    }
   }
   printf("*日期汇总  sqlBuf:%s\n",sqlBuf);
   
}else if(otype==3|| otype==7){/*人员汇总*/
    int i;
    int iTemp;
	long long *srvflow;
	int       *srvrequest;
	long long usersrvflow;
	int       usersrvrequest;

    char      caLine[25000];
    char      caggg[64];

  if(gid=-1){
	   sprintf(sqlBuf,"select b.dispname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.groupname \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid group by a.userid,service order by a.userid,abyte desc ",caSdate1,caSdate2);
	  }else{
	  		if(userid==-1){
    			  sprintf(sqlBuf,"select b.username ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.groupname  \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=%d and b.groupid=c.groupid group by a.userid,service order by a.userid,abyte desc ",caSdate1,caSdate2,gid);
	
    		}else{
    				sprintf(sqlBuf,"select b.username ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.groupname \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.userid=%d and b.groupid=c.groupid group by a.userid,service order by a.userid,abyte desc ",caSdate1,caSdate2,userid);
    	  } 
	}	 

  srvflow=(long long *)malloc(sizeof(long long)*(NAS_MAX_SYSSRVNUM+1));
  srvrequest=(int *)malloc(sizeof(int)*(NAS_MAX_SYSSRVNUM+1));
  usersrvflow=0;
  usersrvrequest=0;
	for(iTemp=0;iTemp<NAS_MAX_SYSSRVNUM;iTemp++){
   		srvflow[iTemp]=0;
    	srvrequest[iTemp]=0;
   }
  printf("人员汇总 3 7 sqlBuf:%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor == NULL) {
	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/

        return 0;
  }

  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"gb2312");
  tablePtr=lt_dbput_table(dbPtr,"glist");
  for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
   	if(_ltPubInfo->topSrvName[i].srvid==-1){
   		break;
    }
    lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[i].srvname);
  }


  tablePtr=lt_dbput_table(dbPtr,"llist");
  tempRow= ltDbFetchRow(tempCursor);
  memset(oldname,0,sizeof(oldname));
  memset(caggg,0,sizeof(caggg));
  while(tempRow!=NULL){
    	    if(strcmp(oldname,tempRow[0])!=0){/*新的一行*/
    	    	 
						  if(oldname[0]!='\0'){/*新的一行开始写*/
						  	
						  		if(otype==3){
    				sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n \
 <td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,caggg,usersrvflow);
									}else if(otype==7){
    				sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n \
 <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
									}
									
									for(iTemp=0;iTemp<NAS_MAX_SYSSRVNUM;iTemp++){
								   	if(_ltPubInfo->topSrvName[iTemp].srvid==-1){
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

						  for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
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
    				sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n \
 <td class=xl25724 style='border-top:none;border-left:none'>%lldK</td>",oldname,caggg,usersrvflow);
									}else if(otype==7){
    				sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n \
 <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
									}
									
									for(iTemp=0;iTemp<NAS_MAX_SYSSRVNUM;iTemp++){
								   	if(_ltPubInfo->topSrvName[iTemp].srvid==-1){
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
			lt_page_content=ltPltFileParse("/app/ns/html/main/usersrvplt.htm",dbPtr->doc,0);
	  }else if(lang==1){//Eng  
			lt_page_content=ltPltFileParse("/app/ns/html/main/usersrvplt_en.htm",dbPtr->doc,0);
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
	  
    return 0;
  
}else if(otype==5 || otype==6 ){/*部门日期汇总*/
    int  groupnum;
    typedef struct gname_s {
		    int             gid;
		    char            gname[50];
		    long long       gflow;
		    int             gtime;
		} nasGname;
    nasGname gnamelist[100];
    long long gflowlist[100];
    long long gtimelist[100];
    int      i;
    char     caLine[16396];
    
    int  oldgroupid;
	int  oldsrvid;
	int  xgroupid;
	char oldDate[64];
		
    
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
      	sprintf(sqlBuf,"select groupid,groupname from nasgroup order by groupid asc ");
	  }else{
	  		sprintf(sqlBuf,"select groupid,groupname from nasgroup where  groupid=%d order by groupid asc ",gid);
	  }
	  printf("部门日期汇总 5 6 sqlBuf:%s\n",sqlBuf);
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			  system(caLabel);/*建立目录和正在处理标志*/

	    	//	ltMsgFree(ltMsgPk);
	        return 0;
	  }
	  dbPtr=lt_dbinit();
      lt_db_htmlpage(dbPtr,"gb2312");
      tablePtr=lt_dbput_table(dbPtr,"glist");
	  tempRow= ltDbFetchRow(tempCursor);
      while(tempRow!=NULL){
    	gnamelist[groupnum].gid=atol(tempRow[0]);
    	sprintf(gnamelist[groupnum].gname,"%s",tempRow[1]);
    	lt_dbput_recordvars(tablePtr,1,
		            "groupname",LT_TYPE_STRING,tempRow[1]);
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
        sprintf(sqlBuf,"select a.sdate as xdate,service,c.groupid ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.groupid \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid group by a.sdate,service,c.groupid order by xdate asc,service,c.groupid,abyte desc ",caSdate1,caSdate2);
	  }else{
	  		 sprintf(sqlBuf,"select a.sdate as xdate,service,c.groupid ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.groupid \
from nassrvreport a,nasuser b,nasgroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by a.sdate,service,c.groupid order by xdate asc,service,c.groupid,abyte desc ",caSdate1,caSdate2,gid);
	  }

    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    		system(caLabel);/*建立目录和正在处理标志*/
	    	//	ltMsgFree(ltMsgPk);
	        return 0;
	  }
	  tablePtr=lt_dbput_table(dbPtr,"llist");
	  tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    	if( strcmp(oldDate,tempRow[0])!=0){/*day change 全变了，新的一行加时间*/
    	
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
    				sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n \
 <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n \
 <td class=xl2816118 style='border-top:none;border-left:none'>Totle Flow</td>");
					}else if(otype==6){
    				sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n \
 <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n \
 <td class=xl2816118 style='border-top:none;border-left:none'>Totle Time</td>");
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
    		sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n \
 <td class=xl5016118 style='border-top:none;border-left:none'>%s</td>\r\n \
 <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",tempRow[0],caSrv);

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
    		if( oldsrvid!=atol(tempRow[1]) ){/*服务变了,新的一行开始*/
					
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
	    		sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n \
	 <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n \
	 <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",caSrv);
	
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
    }
	  
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);

		if(lang==0){//Ch
		    lt_page_content=ltPltFileParse("/app/ns/html/main/srvgroupdateplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			 lt_page_content=ltPltFileParse("/app/ns/html/main/srvgroupdateplt_en.htm",dbPtr->doc,0);
		}
	 

	    if(lt_page_content==NULL){

				  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);

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
	 	
}
  // printf("sqlBuf:%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor == NULL) {
	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
        return 0;
  }

  if(otype==1){
	  sprintf(caLabel,"%sdata.ini",caTempDir);
      iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	  if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
		    		system(caLabel);/*建立目录和正在处理标志*/
		    		ltDbCloseCursor(tempCursor);

		            return 0;
		}		
							
	}
printf("It is here 。。。。。。。\n");
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    tempRow= ltDbFetchRow(tempCursor);
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
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[4])) );
								
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
						  
						  if(otype==1){printf("???????????????????。\n");
									sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
						  }else{
								    sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
						  }
				
						  if(otype==2){
    	        	//sprintf(caGname,"<a href='/main/main.xp?Fun=nassrvreport+ptype=0+sdate=%s+edate=%s+gid=%s+otype=3'>%s</a>",
    	        	//	caSdate,caEdate,tempRow[4],tempRow[0]);
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
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[4])) );
							
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
                    sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",caTempDir,caTempDir,lMaxBytes);
					system(caLabel);	
					lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"srvreporta.gif\">" );
	}

/* 结果输出  */
  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);

    if(lang==0){//Ch
		  lt_page_content=ltPltFileParse("/app/ns/html/main/srvreportplta.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		  lt_page_content=ltPltFileParse("/app/ns/html/main/srvreportplta_en.htm",dbPtr->doc,0);
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
  return 0;
}

  

/*网络流量  相关报告*/
int connectflow(char date1[36],char date2[36],int userid,int gid,int lUnit,char caTempDir[128],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024]; 

    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    } sCount[35];
 
    unsigned long iMaxItem;
   
    unsigned long long lUbytes,lDbytes,lMaxBytes;
    int      i,iMax;
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
    char  oldgname[128];
	char  egname[128];
    int  topuser=100;
    unsigned long long  lMax;
     
     ltDbConn *G_DbCon;
    sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
    sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
    sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
   
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    printf("caSdate::%s ;; caEdate::%s \n",date1,date2);

    
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }
    else {
        lTime1 = 0;
    }
    
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"24:00:00");
    }
    else {
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
    if(lUnit==1) {/*天*/
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
            memcpy(sCount[i].caItem,caSdate+8,2);
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
        			sprintf(sqlBuf,"select substring(sdate,7,2),sum(ubytes),sum(dbytes) \
from nassrvreport \
where ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);

       }else{
          if(userid!=-1){
       	     sprintf(sqlBuf,"select substring(sdate,7,2),sum(ubytes),sum(dbytes) \
from nassrvreport \
where userid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
				}else{
						 sprintf(sqlBuf,"select substring(sdate,7,2),sum(ubytes),sum(dbytes) \
from nassrvreport a,nasuser b \
where a.userid = b.userid and b.groupid=%d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
				
			  }     
      }

        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

    }        
    else if(lUnit==2) {/*时*/
        memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
        lStep = 3600;
        if(lTime1 == 0) {
            lTime1 = lTime - 24*lStep;
        }
        if(lTime2>lTime){
		       lTime2=lTime;
	    }
        lTime0 = lTime1;
        iMaxItem = 25;

        for(i=0;i<iMaxItem;i++) {
            nasCvtStime(lTime0,caSdate,caTime);
            memcpy(sCount[i].caItem,caTime,2);
            lTime0 = lTime0 + lStep;
            sCount[i].lUbytes = 0;

            sCount[i].lDbytes = 0;
            sCount[i].lTbytes = 0;
            sCount[i].lConTime = 0;
        }
	    nasTimeGetDate(caSdate1,lTime1);
        nasTimeGetDate(caSdate2,lTime2);
        nasTimeGetTime(caTime1,lTime1);
	    nasTimeGetTime(caTime2,lTime2);
        caTime1[2]=0;
	    caTime2[2]=0;
	    
		sprintf(caSdate,"%s",date1);

			if(gid==-1){
          sprintf(sqlBuf,"select stime,sum(ubytes),sum(dbytes) \
from nassrvreport \
where ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
  printf("sqlbuf: : %s\n",sqlBuf);
	   }else{
				if(userid!=-1){		   
	       sprintf(sqlBuf,"select stime,sum(ubytes),sum(dbytes) \
from nassrvreport \
where  userid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2); printf("sqlbuf: : %s\n",sqlBuf);
	     }else{
	     		sprintf(sqlBuf,"select stime,sum(ubytes),sum(dbytes) \
from nassrvreport a,nasuser b  \
where a.userid=b.userid and  b.groupid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2); printf("sqlbuf: : %s\n",sqlBuf);
	     
	     }
	   
	    }
       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
    }
    else if(lUnit==3) {/*用户*/
        
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
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1, c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,topuser);
		 }else{
		 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid and b.groupid=%d  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,gid,topuser);
		 }
	   
       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
    }else if(lUnit==4) {/*用户上流量排序*/
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
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,topuser);
		 }else{
		 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid and b.groupid=%d  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,gid,topuser);
		 }
	   
       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
    }else if(lUnit==5) {/*用户下流量排序*/
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
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,topuser);
		 }else{
		 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid and b.groupid=%d  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,gid,topuser);
		 }
	   
       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
 }else if(lUnit==6){
  nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select c.groupname,c.groupname, \
sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)\
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and  b.groupid=c.groupid  \
group by c.groupid  \
order by mybytes desc  ",caSdate1,caSdate2);

  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
  
}else if(lUnit==7){
	nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select b.username ,  c.groupname , \
sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)    \
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and b.groupid=c.groupid  \
group by b.userid  \
order by b.groupid, mybytes desc  ",caSdate1,caSdate2);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

}else if(lUnit==8){
	nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select c.groupname,a.sdate, \
sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)\
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and  b.groupid=c.groupid  \
group by a.sdate,c.groupid  \
order by a.sdate,mybytes desc  ",caSdate1,caSdate2);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
}
         
    if(tempCursor == NULL) {
		sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
        return 0;
    }

 if(lUnit==1 || lUnit==2 )   { 
    lMaxBytes = 1;
    iMax = 0;
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
        sprintf(caItem,"%s",tempRow[0]);
				lUbytes=atoll(tempRow[1]);
        lDbytes=atoll(tempRow[2]);
        if(lMaxBytes <= (lUbytes + lDbytes) / 1024) {
           lMaxBytes = (lUbytes + lDbytes) / 1024;
        }
        for(i=0;i<iMaxItem;i++) {
           if(strcmp(sCount[i].caItem,caItem)==0) {
                if(iMax < i+1) iMax = i+1;
                sCount[i].lUbytes = lUbytes / 1024;
                sCount[i].lDbytes = lDbytes / 1024;
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
	  if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
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
				"abyte",LT_TYPE_LONGLONG,sCount[i].lUbytes+sCount[i].lDbytes);

		sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes+sCount[i].lDbytes,sCount[i].lDbytes,sCount[i].lUbytes);

		write(iFd, caLabel, strlen(caLabel));	
   }

        close(iFd); 
		 
    if(lUnit==1){ 
         sprintf(caLabel,"/app/ns/bin/cgi/outflowplt.pl  %sdata.ini %sflowreport11_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file1","flowreport11_1.jpg");

		 sprintf(caLabel,"/app/ns/bin/cgi/outflowpltline.pl  %sdata.ini %sflowreport22_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file2","flowreport22_1.jpg");	
    
    }else{
		 sprintf(caLabel,"/app/ns/bin/cgi/outflowplt.pl  %sdata.ini %sflowreport11_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file1","flowreport11_2.jpg");

         sprintf(caLabel,"/app/ns/bin/cgi/outflowpltline.pl  %sdata.ini %sflowreport22_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file2","flowreport22_2.jpg");	
   
    }
   
    if(lang==0){//Ch
		 lt_page_content=ltPltFileParse("/app/ns/html/main/flowreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		 lt_page_content=ltPltFileParse("/app/ns/html/main/flowreportplt_en.htm",dbPtr->doc,0);
	}
  

}else if(lUnit==3 || lUnit==4  || lUnit==5 )   { 
	
	 iTemp=0;
    lMax=0;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,6,
							"iIndex",LT_TYPE_LONG,iTemp,
							"uname",LT_TYPE_STRING,tempRow[1],
							"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
							"lubyte",LT_TYPE_LONGLONG,atoll(tempRow[3]),
							"ldbyte",LT_TYPE_LONGLONG,atoll(tempRow[4]),
							"groupname",LT_TYPE_STRING,tempRow[5]);
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
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
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
						  
		sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv1.pl  %sdata.ini %sflowreportt.gif %llu ",caTempDir,caTempDir,lMax);
		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreportt.gif\">" );

   {
   
      int i;
 
      sprintf(caLabel,"%sdata.ini",caTempDir);

      iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	  if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
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
			 
		sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv-bar.pl  %sdata.ini %sflowreport13.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));

		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"file1","flowreport13.jpg"); 
      
	 }

	 if(lang==0){//Ch
		 lt_page_content=ltPltFileParse("/app/ns/html/main/userreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		 lt_page_content=ltPltFileParse("/app/ns/html/main/userreportplt_en.htm",dbPtr->doc,0);
	}
	
	
}else if(lUnit==6 || lUnit==7  || lUnit==8 )   { 
	
	dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    
    tempRow= ltDbFetchRow(tempCursor);
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
	    lt_page_content=ltPltFileParse("/app/ns/html/main/groupreportplt.htm",dbPtr->doc,0);

	}else if(lang==1){//Eng
		lt_page_content=ltPltFileParse("/app/ns/html/main/groupreportplt_en.htm",dbPtr->doc,0);

	}
    
}
 
   if(lt_page_content==NULL){
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
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

  return 0;
}



/*.网络次数分析*/
int connectreport(char date1[36],char date2[36],int userid,int gid,int lUnit,char caTempDir[256],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
 
    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    } sCount[35];

    unsigned long iMaxItem;
    int lSize;
    unsigned long long lUbytes,lMaxBytes;
    int      i,iMax;
    long lStep,lTime,lTime1,lTime2,lTime0;
    char caSdate[32],caTime[32],caItem[16];
    char caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    char caLabel[256];
    char caTemp[8192];
 
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    
    char caEdate[32];
    char reportname[123];
    char *lt_page_content;
    int  iFd;
    unsigned long long *lTop;
    int  iTemp;
    char  oldgname[128];
	char  egname[128];
    int  topuser=100;
    unsigned long long  lMax;
  
    ltDbConn *G_DbCon;
    sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
    sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
    sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    printf("caSdate::%s ;; caEdate::%s \n",date1,date2);
    
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:01");
    }
    else {
        lTime1 = 0;
    }
    
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }
    else {
        lTime2 = lTime;
    }
  
    sprintf(reportname,"report%d",lUnit);
    if(reportname==NULL){
    	sprintf(reportname,"******");
    }

    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            
            return 0;
        }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
    
    tempCursor=NULL;

    if(lUnit==1) {/*天*/
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
            memcpy(sCount[i].caItem,caSdate+8,2);
            lTime0 = lTime0 + lStep;
        }
        nasTimeGetDate(caSdate1,lTime1);
        nasTimeGetDate(caSdate2,lTime2);
        nasTimeGetTime(caTime1,lTime1);
	      nasTimeGetTime(caTime2,lTime2);
        caTime1[2]=0;
	      caTime2[2]=0;
				if(gid==-1){
        			sprintf(sqlBuf,"select substring(sdate,7,2),sum(ctime) \
from nassrvreport \
where ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);

       }else{
          if(userid!=-1){
       	     sprintf(sqlBuf,"select substring(sdate,7,2),sum(ctime) \
from nassrvreport \
where userid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
				}else{
						 sprintf(sqlBuf,"select substring(sdate,7,2),sum(ctime) \
from nassrvreport a,nasuser b \
where a.userid = b.userid and b.groupid=%d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by sdate ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
				
			  }
      }
        printf("sql:%s\n",sqlBuf);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

    }        
    else if(lUnit==2) {/*时*/
        memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
        lStep = 3600;
        if(lTime1 == 0) {
            lTime1 = lTime - 24*lStep;
        }
        if(lTime2>lTime){
		       lTime2=lTime;
	      }
        lTime0 = lTime1;
        iMaxItem = 25;

        for(i=0;i<iMaxItem;i++) {
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

			if(gid==-1){
          sprintf(sqlBuf,"select stime,sum(ctime) \
from nassrvreport \
where ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
	   }else{
				if(userid!=-1){		   
	       sprintf(sqlBuf,"select stime,sum(ctime) \
from nassrvreport \
where  userid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
	     }else{
	     		sprintf(sqlBuf,"select stime,sum(ctime) \
from nassrvreport a,nasuser b  \
where a.userid=b.userid and  b.groupid = %d and ((sdate > '%s') or(sdate = '%s' and stime>='%s')) and \
((sdate < '%s') or(sdate = '%s' and stime<='%s')) \
group by stime ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
	     
	     }
	   
	    }
	    printf("sql:%s\n",sqlBuf);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
    }
    else if(lUnit==3) {/*用户*/
        
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
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte, c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,topuser);
		 }else{
		 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte,c.groupname from nassrvreport a, \
		nasuser b,nasgroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
		and a.userid = b.userid and b.groupid=c.groupid and b.groupid=%d  \
		group by a.userid,b.username \
		order by abyte desc limit 0,%d ",caSdate1,caSdate2,gid,topuser);
		 }
	    printf("sql:%s\n",sqlBuf);
       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
 }else if(lUnit==6){
 	nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select c.groupname,c.groupname, \
sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes \
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and  b.groupid=c.groupid  \
group by c.groupid  \
order by mybytes desc  ",caSdate1,caSdate2);
  printf("sql:%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
  
}else if(lUnit==7){
  nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select b.username ,  c.groupname , \
sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes \
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and b.groupid=c.groupid  \
group by b.userid  \
order by b.groupid, mybytes desc  ",caSdate1,caSdate2);
printf("sql:%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

}else if(lUnit==8){
	nasTimeGetDate(caSdate1,lTime1);
  nasTimeGetDate(caSdate2,lTime2);
  memset(oldgname,0,sizeof(oldgname));
  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select c.groupname,a.sdate, \
sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes \
from nassrvreport a,nasuser b ,nasgroup c \
where a.sdate >= '%s' and a.sdate <= '%s'  \
and a.userid = b.userid and  b.groupid=c.groupid  \
group by a.sdate,c.groupid  \
order by a.sdate,mybytes desc  ",caSdate1,caSdate2);
printf("sql:%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

} 
          
    if(tempCursor == NULL) {
		sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    	system(caLabel);/*建立目录和正在处理标志*/
        return 0;
    }
  if(lUnit==1 || lUnit==2 )   { 
    lMaxBytes = 1;
    iMax = 0;
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
        sprintf(caItem,"%s",tempRow[0]);
		lUbytes=atoll(tempRow[1]);
        if(lMaxBytes <= lUbytes) {
           lMaxBytes = lUbytes;
        }
        for(i=0;i<iMaxItem;i++) {
           if(strcmp(sCount[i].caItem,caItem)==0) {
                if(iMax < i+1) iMax = i+1;
                sCount[i].lUbytes = lUbytes ;
                break;
           }
        }
        tempRow= ltDbFetchRow(tempCursor);
    }
    
    ltDbCloseCursor(tempCursor);
    iMaxItem = iMax;
    /* 结果输出  */

/*  生成图表  */    
 /*   lBaseX = lMaxBytes / 20;
    lHight = 330;
    lWidth = 650;
    
    nasCvtStime(lTime1,caSdate1,caTime1);
    nasCvtStime(lTime2,caSdate2,caTime2);
    caTime1[5]=0;
    caTime2[5]=0;
    sprintf(caLabel,"%s--%s",caSdate1,caSdate2);
    strcpy(caTemp,"<?xml version=\"1.0\"?>\n<db>\n");
    ltStrAddF(caTemp,"<pic>\n<height>%d</height>\n<width>%d</width>\n<sample>3</sample>\n",lHight,lWidth);
    ltStrAddF(caTemp,"<title>%s</title>\n<style>%s</style>\n<piconline>3</piconline>\n",caLabel,"2");
    ltStrAddF(caTemp,"<lbase>70</lbase>\n<rbase>70</rbase>\n<tbase>30</tbase>\n<bbase>50</bbase>\n");
    ltStrAddF(caTemp,"<maxvalue>%llu</maxvalue>\n</pic>\n",(unsigned long long)(lMaxBytes+1));
  
    ltStrAddF(caTemp,"<xbase>\n<points>%d</points>\n",iMaxItem+1);
    for(i=0;i<iMaxItem;i++) {
        ltStrAddF(caTemp,"<pointv>%s</pointv>\n",sCount[i].caItem);
    }
    if(lUnit==1){
    	ltStrAddF(caTemp,"<pointv>%s</pointv>\n","Day");
    }else{
		ltStrAddF(caTemp,"<pointv>%s</pointv>\n","Hour");
    }

    ltStrAddF(caTemp,"</xbase>\n");        

    ltStrAddF(caTemp,"<ybase>\n<points>%d</points>\n",20);
    for(i=0;i<20;i++) {
        lBaseY = (i+1)*lBaseX;
        ltStrAddF(caTemp,"<pointv>%llu</pointv>\n",lBaseY);
    }
    ltStrAddF(caTemp,"</ybase>\n");        

    ltStrAddF(caTemp,"<sample color=\"0\" title=\"%s\">\n","Up");
    for(i=0;i<iMaxItem;i++) {
        
        lBaseY = sCount[i].lUbytes ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);

    ltStrAddF(caTemp,"</sample>\n");
    
    ltStrAddF(caTemp,"<sample color=\"1\" title=\"%s\">\n","Down");
    for(i=0;i<iMaxItem;i++) {
        lBaseY = sCount[i].lDbytes ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);

    ltStrAddF(caTemp,"</sample>\n");
    ltStrAddF(caTemp,"<sample color=\"2\" title=\"%s\">\n","Total");
    for(i=0;i<iMaxItem;i++) {
        lBaseY = (sCount[i].lUbytes) ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);
    ltStrAddF(caTemp,"</sample>\n");
    ltStrAddF(caTemp,"</db>\n");
    
    sprintf(caTempFile,"%sflowreportn1_%d.jpg",caTempDir,lUnit);
    pJpg=drawJPG(caTemp,3,&lSize);
    saveJPG(pJpg,lSize, caTempFile);
    //freeJPG(pJpg);
    dbPtr=lt_dbinit();
    if(lUnit==1){
    lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_1.jpg");
    }else{  
    lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_2.jpg");	
	}

    sprintf(caTempFile,"%sflowreportn2_%d.jpg",caTempDir,lUnit);

    pJpg=drawJPG(caTemp,1,&lSize);
    saveJPG(pJpg,lSize, caTempFile);
    if(lUnit==1){
    lt_dbput_rootvars(dbPtr,1,"file2","flowreportn2_1.jpg");
    }else{
    lt_dbput_rootvars(dbPtr,1,"file2","flowreportn2_2.jpg");	
	}

   sprintf(caLabel,"%s--%s",caSdate1,caSdate2);
   lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
   lt_dbput_rootvars(dbPtr,1,"reportname",reportname);     
   if(lUnit==1){
    	lt_dbput_rootvars(dbPtr,1,"xunit","Day");
   }else{
			lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
   }
   */
    
    dbPtr=lt_dbinit();
    sprintf(caLabel,"%sdata.ini",caTempDir);

    iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	  if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
		    		system(caLabel);/*建立目录和正在处理标志*/
		    		ltDbCloseCursor(tempCursor);

		            return 0;
		}	

	 

   if(lUnit==1){  
    	lt_dbput_rootvars(dbPtr,1,"xunit","Day");
   }else{
		lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
   }
    
   tablePtr=lt_dbput_table(dbPtr,"list2");
   for(i=0;i<iMaxItem;i++) {
        lt_dbput_recordvars(tablePtr,2,
				"mytime",LT_TYPE_STRING,sCount[i].caItem,
				"request",LT_TYPE_LONGLONG,sCount[i].lUbytes);

		sprintf(caLabel,"%s,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes);

		write(iFd, caLabel, strlen(caLabel));	
   }

   	     close(iFd);	
		 if(lUnit==1){
    	 
		   sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv-bar.pl  %sdata.ini %sflowreportn1_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));

		   system(caLabel);	 
		   lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_1.jpg");
		 }else{
			 sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv-bar.pl  %sdata.ini %sflowreportn1_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));

		     system(caLabel);	 
		     lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_2.jpg");
		 }
   
	if(lang==0){//Ch
	     lt_page_content=ltPltFileParse("/app/ns/html/main/requestreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		 lt_page_content=ltPltFileParse("/app/ns/html/main/requestreportplt_en.htm",dbPtr->doc,0);
	}
  

}else if(lUnit==3  )   { 
	
	iTemp=0;
    lMax=0;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");   
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,4,
							"iIndex",LT_TYPE_LONG,iTemp,
							"uname",LT_TYPE_STRING,tempRow[1],
							"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
							"groupname",LT_TYPE_STRING,tempRow[3]);
				
				lTop[iTemp]=atoll(tempRow[2]);
			  
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
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
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
						  
		sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv1.pl  %sdata.ini %sflowreportnum.gif %llu ",caTempDir,caTempDir,lMax);
		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreportnum.gif\">" );

   { 

    int i;
  
 
    sprintf(caLabel,"%s--%s",caSdate,caSdate);
   lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
   lt_dbput_rootvars(dbPtr,1,"reportname",reportname);   

    sprintf(caLabel,"%sdata.ini",caTempDir);

    iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	  if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
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
			 
		sprintf(caLabel,"/app/ns/bin/cgi/onlinesrv-bar.pl  %sdata.ini %sflowreport1s.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));

		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"file1","flowreport1s.jpg"); 
   }
   
	if(lang==0){//Ch
	   lt_page_content=ltPltFileParse("/app/ns/html/main/userrequestreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		lt_page_content=ltPltFileParse("/app/ns/html/main/userrequestreportplt_en.htm",dbPtr->doc,0);
	}
   
	
}else if(lUnit==6 || lUnit==7  || lUnit==8 )   { 
	
	dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    
    tempRow= ltDbFetchRow(tempCursor);
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
    
   sprintf(caLabel,"%s--%s",caSdate1,caSdate2);
   lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
   lt_dbput_rootvars(dbPtr,1,"reportname",reportname);   
    if(lang==0){//Ch
        lt_page_content=ltPltFileParse("/app/ns/html/main/grouprequestreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		 lt_page_content=ltPltFileParse("/app/ns/html/main/grouprequestreportplt_en.htm",dbPtr->doc,0);
	}
   

}
 
    if(lt_page_content==NULL){
			 sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    		 system(caLabel);
    		 lt_dbfree(dbPtr);
             return 0;
	}else{
	    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	    sprintf(caLabel,"%sflownum%d.htm",caTempDir,lUnit);
		iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
	  /*生成html格式报告*/
   }
  lt_dbfree(dbPtr);
  return 0;
}
 

/*系统出口流量*/    //暂注掉。。 
int outflow(char date1[36],char date2[36],char caTempDir[256]){

    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    // char caTempDir[128];
    char caTempFile[128];
    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
    } sCount[35];

    int lUnit;
    unsigned long iMaxItem;
    int lSize;
    unsigned long long lUbytes,lDbytes,lMaxBytes,lBaseX,lBaseY;
    int      i,iMax;
    long lStep,lTime,lTime1,lTime2,lTime0,lHight,lWidth;
    char caSdate[32],caSTime[32],caTime[32],caItem[16];
    char caEdate[32];
    char caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    char caLabel[256];
    char caTemp[18192];
    void *pJpg;
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char *reportname;
    char *lt_page_content;
    int  iFd;
  
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    printf("caSdate::%s ;; caEdate::%s \n",date1,date2);
    
    memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
    
    sprintf(caSTime,"%s","00:00:00");
    
    tempCursor=NULL;
    lTime = time(0);
    
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,caSTime);
    }
    else {
        lTime1 = 0;
    }
    
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"24:00:00");
    }
    else {
        lTime2 = lTime;
    }
     
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
   
      /*天*/
        lStep = 24 * 3600;
        if(lTime1 == 0) {
            lTime1 = lTime2-31*lStep;
           
        }
        if(lTime2>(lTime1+31*lStep)){
        	lTime2=lTime1+31*lStep;
        }
        // lTime2=lTime1+31*lStep;
        if(lTime2>lTime){
					lTime2=lTime;
				}
        lTime0 = lTime1;
        iMaxItem = 32;
        for(i=0;i<iMaxItem;i++) {
	    			nasCvtStime(lTime0,caSdate,caTime);
            memcpy(sCount[i].caItem,caSdate+8,2);
            lTime0 = lTime0 + lStep;
				    sCount[i].lDbytes=0;
				    sCount[i].lUbytes=0;
        }
        sprintf(sqlBuf,"select substring(sdate,7,2),avg(ubytes),avg(dbytes) \
                                   from nasflowlog \
                                   where rtime >= %lu and rtime <= %lu \
                                   group by sdate",
                                   lTime1,lTime2);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
       
    if(tempCursor == NULL) {
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		system(caLabel);/*建立目录和正在处理标志*/    	 
            return 0;
    }
 
    lMaxBytes = 1;
    iMax = 0;
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
        sprintf(caItem,"%s",tempRow[0]);
				lUbytes=atoll(tempRow[1]);
        lDbytes=atoll(tempRow[2]);
        if(lMaxBytes <= (lUbytes + lDbytes) / 20) {
           lMaxBytes = (lUbytes + lDbytes) / 20;
        }
        for(i=0;i<iMaxItem;i++) {
           if(strcmp(sCount[i].caItem,caItem)==0) {
                if(iMax < i+1) iMax = i+1;
                sCount[i].lUbytes = lUbytes / 20;
                sCount[i].lDbytes = lDbytes / 20;
                break;
           }
        }
        tempRow= ltDbFetchRow(tempCursor);
    }
    
    ltDbCloseCursor(tempCursor);
    iMaxItem = iMax;
    /* 结果输出  */

/*  生成图表  */
    
    lBaseX = lMaxBytes / 20;
    lHight = 380;
    lWidth = 650;
    nasCvtStime(lTime1,caSdate1,caTime1);
    nasCvtStime(lTime2,caSdate2,caTime2);
    caTime1[5]=0;
    caTime2[5]=0;
    sprintf(caLabel,"%s %s--%s %s",caSdate1,caTime1,caSdate2,caTime2);
    strcpy(caTemp,"<?xml version=\"1.0\"?>\n<db>\n");
    ltStrAddF(caTemp,"<pic>\n<height>%d</height>\n<width>%d</width>\n<sample>3</sample>\n",lHight,lWidth);
    ltStrAddF(caTemp,"<title>%s</title>\n<style>%s</style>\n<piconline>3</piconline>\n",caLabel,"2");
    ltStrAddF(caTemp,"<lbase>70</lbase>\n<rbase>70</rbase>\n<tbase>30</tbase>\n<bbase>50</bbase>\n");
    ltStrAddF(caTemp,"<maxvalue>%llu</maxvalue>\n</pic>\n",(unsigned long long)(lMaxBytes+1));

  
    ltStrAddF(caTemp,"<xbase>\n<points>%d</points>\n",iMaxItem+1);
    for(i=0;i<iMaxItem;i++) {
        ltStrAddF(caTemp,"<pointv>%s</pointv>\n",sCount[i].caItem);
    }
    if(lUnit==1){
    	ltStrAddF(caTemp,"<pointv>%s</pointv>\n","Day");
    }else{
			ltStrAddF(caTemp,"<pointv>%s</pointv>\n","Hour");
    }

    ltStrAddF(caTemp,"</xbase>\n");        

    ltStrAddF(caTemp,"<ybase>\n<points>%d</points>\n",20);
    for(i=0;i<20;i++) {
        lBaseY = (i+1)*lBaseX;
        ltStrAddF(caTemp,"<pointv>%lluB</pointv>\n",lBaseY);
    }
    ltStrAddF(caTemp,"</ybase>\n");        

    ltStrAddF(caTemp,"<sample color=\"0\" title=\"%s\">\n","Up");
    for(i=0;i<iMaxItem;i++) {
        
        lBaseY = sCount[i].lUbytes ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);

    ltStrAddF(caTemp,"</sample>\n");
    
    ltStrAddF(caTemp,"<sample color=\"1\" title=\"%s\">\n","Down");
    for(i=0;i<iMaxItem;i++) {
        lBaseY = sCount[i].lDbytes ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);

    ltStrAddF(caTemp,"</sample>\n");
    ltStrAddF(caTemp,"<sample color=\"2\" title=\"%s\">\n","Total");
    for(i=0;i<iMaxItem;i++) {
        lBaseY = (sCount[i].lDbytes + sCount[i].lUbytes) ;
        ltStrAddF(caTemp,"<value>%llu</value>\n",lBaseY);
    }
    ltStrAddF(caTemp,"<value>0</value>\n",0);
    ltStrAddF(caTemp,"</sample>\n");
    ltStrAddF(caTemp,"</db>\n");
   //printf("caTemp==%s",caTemp);
    

    sprintf(caTempFile,"%soutflow1.jpg",caTempDir);
    //printf("%s\n",caTempDir);
    pJpg=drawJPG(caTemp,3,&lSize);
    saveJPG(pJpg,lSize, caTempFile);
    //freeJPG(pJpg);
    dbPtr=lt_dbinit();
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.jpg");

     sprintf(caTempFile,"%soutflow2.jpg",caTempDir);

     pJpg=drawJPG(caTemp,1,&lSize);
     saveJPG(pJpg,lSize, caTempFile);
     lt_dbput_rootvars(dbPtr,1,"file2","outflow2.jpg");
     
     printf(caLabel,"%s %s--%s %s",caSdate1,caTime1,caSdate2,caTime2);
     lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
     lt_dbput_rootvars(dbPtr,1,"reportname",reportname);  
     
     lt_dbput_rootvars(dbPtr,1,"xunit","Day");
       
    tablePtr=lt_dbput_table(dbPtr,"list2");
    for(i=0;i<iMaxItem;i++) {
        lt_dbput_recordvars(tablePtr,4,
				"mytime",LT_TYPE_STRING,sCount[i].caItem,
				"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
				"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
				"abyte",LT_TYPE_LONGLONG,sCount[i].lUbytes+sCount[i].lDbytes);
    }
     
    lt_page_content=ltPltFileParse("/app/ns/html/reportplt/dailyreportpart3.htm",dbPtr->doc,0);
    
    if(lt_page_content==NULL){ 
			  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
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
    char       xName[64];
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    long lTime,lTime1,lTime2;
    char caLabel[128];
    int  sType;
    
    struct xs_group{
    	char strGroup[128];
        int  idGroup;
		};

    char       oldname[256];
	  struct xs_group xmyGroup[256]; /*最多256个部门*/
	  
	  char reportname[64];
    char *lt_page_content;
    int  iFd;
    
	memset(oldname,0,sizeof(oldname));
	  
	sprintf(sqlBuf,"select groupid,groupname from nasgroup");
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor == NULL) {
			printf("数据为空，不能生成报告！\n");
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
   
  //  sType=atol( ltMsgGetVar_s(ltMsgPk,"stype") );   // 排序的 。。。
    sType=1;  

	sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);

    tempCursor=NULL;
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }
    else {
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }
    else {
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
            /// 生成该报告目录错误 
            printf("make dir error");
            return 0;
        }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
   
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);

/*top30 访问次数的排序*/
if(oType==1){sprintf(reportname,"全部汇总");

  	if(gid==-1){  
      if(sType==2){
					sprintf(sqlBuf,"select 'All User', host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from nashttpreport \
			where sdate >= '%s' and sdate <= '%s' group by host order by acount desc limit 0,50",caSdate1,caSdate2);
			}else if(sType==1){
					sprintf(sqlBuf,"select  'All User',host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort from nashttpreport \
			where sdate >= '%s' and sdate <= '%s' group by host order by abyte desc limit 0,50",caSdate1,caSdate2);
			}
    }else{
    		if(userid==-1){

						if(sType==2){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by c.groupid, a.host order by acount desc limit 0,30",caSdate1,caSdate2,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid  and b.groupid=%d  group by c.groupid, a.host order by abyte desc limit 0,30",caSdate1,caSdate2,gid);
						}
						
						
    		}else{
    		
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by acount desc limit 0,30",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by abyte desc limit 0,30",caSdate1,caSdate2,userid);
						}
    				 
    	  } 
   }

}else if(oType==2){sprintf(reportname,"部门汇总");
	
  	if(gid==-1){
      	if(sType==2){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid group by c.groupid, a.host order by c.groupid,acount desc ",caSdate1,caSdate2);
				}else if(sType==1){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid  group by c.groupid, a.host order by c.groupid,abyte desc ",caSdate1,caSdate2);
			  }
    }else{
    		if(userid==-1){

						if(sType==2){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by c.groupid, a.host order by acount desc ",caSdate1,caSdate2,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select c.groupname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid  and b.groupid=%d  group by c.groupid, a.host order by abyte desc ",caSdate1,caSdate2,gid);
						}
					 	
    		}else{
    		
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by abyte desc ",caSdate1,caSdate2,userid);
						} 
    	 } 
   }

}else if(oType==3){sprintf(reportname,"用户汇总");
	
  	if(gid==-1){
      	if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid group by a.userid, a.host order by a.userid, acount desc ",caSdate1,caSdate2);
				}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid  group by a.userid, a.host order by a.userid,abyte desc ",caSdate1,caSdate2);
			  }
    }else{
    		if(userid==-1){

						if(sType==2){
								sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid and b.groupid=%d group by a.userid, a.host order by a.userid,acount desc ",caSdate1,caSdate2,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b,nasgroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=c.groupid  and b.groupid=%d  group by a.userid, a.host order by a.userid,abyte desc ",caSdate1,caSdate2,gid);
						}
						
						
    		}else{
    		
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d  group by b.userid, a.host order by abyte desc ",caSdate1,caSdate2,userid);
						} 
    	  } 
   }
}else if(oType==4){sprintf(reportname,"日期汇总");
	
  	if(gid==-1){
      if(sType==2){
					sprintf(sqlBuf,"select a.sdate, host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,a.urlsort from nashttpreport a  \
			where sdate >= '%s' and sdate <= '%s' group by a.sdate,host order by a.sdate,acount desc ",caSdate1,caSdate2);
			}else if(sType==1){
					sprintf(sqlBuf,"select  a.sdate,host ,sum(bytes)/1024 as abyte,sum(ctime) as atime, sum(lcount) as acount,a.urlsort from nashttpreport a\
			where sdate >= '%s' and sdate <= '%s' group by  a.sdate,host order by  a.sdate,abyte desc ",caSdate1,caSdate2);
			}
    }else{
    		if(userid==-1){

						if(sType==2){
								sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid  and b.groupid=%d group by a.sdate, a.host order by a.sdate,acount desc ",caSdate1,caSdate2,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a,nasuser b  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid=%d  group by a.sdate, a.host order by a.sdate,abyte desc ",caSdate1,caSdate2,gid);
						} 
					 
    		}else{
    		
    		    if(sType==2){
								sprintf(sqlBuf,"select  a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a  \
						where a.sdate >= '%s' and a.sdate <= '%s' and  a.userid=%d  group by a.sdate, a.host order by a.sdate,acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from nashttpreport a  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=%d  group by a.sdate, a.host order by a.sdate,abyte desc ",caSdate1,caSdate2,userid);
						}  
    	  } 
   } 
}

printf("sql:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor == NULL) {
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		system(caLabel);/*建立目录和正在处理标志*/
            return 0;
    }
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"gb2312");
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
						"usort",LT_TYPE_STRING,_ltPubInfo->urlSortName[atol(tempRow[5])].sortname);
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
				"usort",LT_TYPE_STRING,_ltPubInfo->urlSortName[atol(tempRow[5])].sortname);
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
	   lt_page_content=ltPltFileParse("/app/ns/html/main/webreportplt.htm",dbPtr->doc,0);
	}else if(lang==1){//Eng
		lt_page_content=ltPltFileParse("/app/ns/html/main/webreportplt_en.htm",dbPtr->doc,0);
	}
    
  if(lt_page_content==NULL){
			  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    		  system(caLabel);/*建立目录和正在处理标志*/
    		 //ltMsgFree(ltMsgPk);
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
   
    return 0;
}



