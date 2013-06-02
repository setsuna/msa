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

static char *time2str(int itime);

static char *time2str(int itime)
{
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




/* 
网络出口流向分析 流量分析   上行，下行，总共
*/
int msaoutflowproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    char caTempDir[256];
    char caTempFile[256];
    int lUnit;
    unsigned long long lUbytes,lDbytes,lMaxBytes;
    int      iMax;
    long lStep,lTime,lTime1,lTime2;
    char caSdate[32],caSTime[32],caItem[16];
    char caEdate[32];
    char caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    char caLabel[256];
    char caTemp[18192];

    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char *reportname;
    char *lt_page_content;
    int  iFd;
    char templname[256];
    char email[25];
    int  srcfd,j;
    char caTempDataDir[256];
	  char stime1[24],stime2[24],stime3[24],stime4[24];
    int time1,time2,time3,time4;
	  char strBuf[1024];

	  memset(strBuf,0,sizeof(strBuf));

    time1=0;
    time2=0;
    time3=0;
    time4=0;
    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }

printf("caSdate: %s\n",caSdate);   
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			  sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
printf("caEdate: %s\n",caEdate);

	  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			  sprintf(email,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }

	  if( ltMsgGetVar_s(ltMsgPk,"time1")==NULL){
			  sprintf(stime1,"%s","");
    }else{
    		sprintf(stime1,"%s",ltMsgGetVar_s(ltMsgPk,"time1"));
			  time1=atoi(stime1);
    }

	  if( ltMsgGetVar_s(ltMsgPk,"time11")==NULL){
			  sprintf(stime2,"%s","");
    }else{
    		sprintf(stime2,"%s",ltMsgGetVar_s(ltMsgPk,"time11"));
			  time2=atoi(stime2); 
    }

	  if( ltMsgGetVar_s(ltMsgPk,"time2")==NULL){
			 sprintf(stime3,"%s","");
    }else{
    	 sprintf(stime3,"%s",ltMsgGetVar_s(ltMsgPk,"time2"));
			 time3=atoi(stime3);
    } 
    
	  if( ltMsgGetVar_s(ltMsgPk,"time22")==NULL){
			sprintf(stime4,"%s","");
    }else{
    	sprintf(stime4,"%s",ltMsgGetVar_s(ltMsgPk,"time22"));
			time4=atoi(stime4); 
    } 

	 if((strcmp(stime1," ")!=0 && strcmp(stime2," ")!=0)){ 
		 sprintf(strBuf,"and (mintue  between %d and %d)",time1,time2);
	 }
	
	 if(strcmp(stime3," ")!=0 && strcmp(stime4," ")!=0){
		 sprintf(strBuf," and ((mintue  between %d and %d) or (mintue  between %d and %d))",time1,time2,time3,time4);
	 }
	 

    sprintf(caSTime,"%s","00:00:00");   
    lUnit=atol(ltMsgGetVar_s(ltMsgPk,"punit"));
    printf("lUnit::   %d\n",lUnit);
    tempCursor=NULL;
    lTime = time(0);
  
    
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,caSTime);
        printf("#：%lu\n",lTime1);
    }
    else {
        lTime1 = 0;
    }  
  
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
         printf("#：%lu\n",lTime2);
    }
    else {
        lTime2 = lTime;
    }
    
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    
    if(reportname==NULL){
    	reportname="********";
    }else{
      memset(templname,0,sizeof(templname));
      strgb2utf8(reportname,templname,strlen(reportname));
      reportname=templname;
    }  
    
    sprintf(caTempDir,"/datacenter/msa/report/reportoutflow/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
			      lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");		
			      ltMsgFree(ltMsgPk);
            return 0;
        }
    }
    nasCvtStime(lTime,caSdate1,caTime1);  
    caTime1[5]=0;
    
    sprintf(caLabel,"/bin/echo 'date=%s %s' > %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'name=%s' >> %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/  
    sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
           
    dbPtr=lt_dbinit();
	  tablePtr=lt_dbput_table(dbPtr,"list2");
	  lMaxBytes = 1;
	  iMax = 0;
    if(lUnit==1) {/*天*/
	        lStep = 24 * 3600;//一天共计多少秒
	        if(lTime1 == 0) {
	            lTime1 = lTime2-31*lStep;     
	        }
	        if(lTime2>(lTime1+31*lStep)){
	        	lTime2=lTime1+31*lStep;
	        }
        	if(lTime2>lTime){
			      lTime2=lTime;
		      }  
          nasTimeGetDate(caSdate1,lTime1);
          nasTimeGetDate(caSdate2,lTime2);
        printf("caSdate1: %s\n",caSdate1);
        printf("caSdate1:%s\n",caSdate2);
        sprintf(sqlBuf,"select sdate,avg(ubytes),avg(dbytes)\
        from msatimereport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' %s\
        group by sdate order by sdate asc",caSdate1,caSdate2,strBuf);     
printf("sql: %s\n",sqlBuf);
            tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	      if(tempCursor == NULL) {
						  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			    		system(caLabel);/*建立目录和正在处理标志*/
			    		ltMsgFree(ltMsgPk);
			        return 0;
			  }
			
		    tempRow= ltDbFetchRow(tempCursor);
		    while(tempRow!=NULL){
		          sprintf(caItem,"%s",tempRow[0]);
				      lUbytes=atoll(tempRow[1])/1024;
		          lDbytes=atoll(tempRow[2])/1024;
			        if( lMaxBytes <= (lUbytes + lDbytes) ) {
			           lMaxBytes = lUbytes + lDbytes;
			        }        
                   lt_dbput_recordvars(tablePtr,4,
							"mytime",LT_TYPE_STRING,caItem,//时间点
							"ubyte",LT_TYPE_LONGLONG,lUbytes,//平均上行速率
							"dbyte",LT_TYPE_LONGLONG,lDbytes,//平均下行速率
							"abyte",LT_TYPE_LONGLONG,lUbytes+lDbytes);//平均总速率
		            tempRow= ltDbFetchRow(tempCursor);
		    }		    
		    ltDbCloseCursor(tempCursor);
    }        


	    sprintf(caTempDataDir,"%sdata",caTempDir);
	    printf("#:%s\n",caTempDataDir);
	    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	    if(srcfd == (-1)) {
	  	      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}
    {/*hour*/
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
		    
          nasTimeGetDate(caSdate1,lTime1);
          nasTimeGetDate(caSdate2,lTime2);
        printf("caSdate1: %s\n",caSdate1);
        printf("caSdate1:%s\n",caSdate2);
         sprintf(sqlBuf,"select sdate,hours,mintue,avg(ubytes),avg(dbytes)\
           from msatimereport  where  substring(sdate,3,8) >='%s'  and substring(sdate,3,8) <='%s'  %s \
           group by  sdate, hours,mintue order by sdate asc, hours asc,mintue asc;",caSdate1,caSdate1,strBuf);
printf("sql: %s\n",sqlBuf);
           tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
           if(tempCursor == NULL){
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		    		system(caLabel);/*建立目录和正在处理标志*/
		    		ltMsgFree(ltMsgPk);
		        	return 0;
		    } 
		    tempRow= ltDbFetchRow(tempCursor);
		    j=0;
		    while(tempRow!=NULL){
		        sprintf(caItem,"%s %s:%s",tempRow[0],tempRow[1],tempRow[2]);
		        lUbytes=atoll(tempRow[3])/1024;
		        lDbytes=atoll(tempRow[4])/1024;
		        if( lMaxBytes <= (lUbytes + lDbytes) ) {
		           lMaxBytes = lUbytes + lDbytes;
		        }
 				 if(lUnit==2){//按小时汇总
		                lt_dbput_recordvars(tablePtr,4,
									"mytime",LT_TYPE_STRING,caItem,
									"ubyte",LT_TYPE_LONGLONG,lUbytes,
									"dbyte",LT_TYPE_LONGLONG,lDbytes,
									"abyte",LT_TYPE_LONGLONG,lUbytes+lDbytes);
						}
						if( (j%24)==0 ){
			           sprintf(caTemp,"%s,%llu,%llu,%llu\r\n",
			        	caItem,
			        	lUbytes+lDbytes,
			        	lDbytes,lUbytes);
			            write(srcfd,caTemp,strlen(caTemp));	
			      }else{
			      	sprintf(caTemp,"%s,%llu,%llu,%llu\r\n",
			        	"",
			        	lUbytes+lDbytes,
			        	lDbytes,lUbytes);
			        write(srcfd, caTemp, strlen(caTemp));	
			      }
			      j++;
		        tempRow= ltDbFetchRow(tempCursor);
		    }
		    
		    ltDbCloseCursor(tempCursor);

    }     
    lMaxBytes=lMaxBytes*1.3;
    sprintf(caTempFile,"%soutflow1.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msa/htmlplt/Scripts/outflowplt.pl   %s %s %llu ",caTempDataDir,caTempFile,lMaxBytes);
    system(caTemp);
    
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");     
    sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msa/htmlplt/Scripts/outflowpltline.pl  %s %s %llu ",caTempDataDir,caTempFile,lMaxBytes);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file2","outflow2.gif");

    nasCvtStime(lTime1,caSdate1,caTime1);
    nasCvtStime(lTime2,caSdate2,caTime2);
    sprintf(caLabel,"%s %s--%s %s",caSdate1,caTime1,caSdate2,caTime2);
    printf(caLabel,"%s %s--%s %s\n",caSdate1,caTime1,caSdate2,caTime2);
    lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
    lt_dbput_rootvars(dbPtr,1,"reportname",reportname);  
    if(lUnit==1){
  	     lt_dbput_rootvars(dbPtr,1,"xunit","Day");
    }else{
 	     lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
    }
        
    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/outflowplt.htm",dbPtr->doc,0);
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
        if(iFd >0){
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
	  /*生成html格式报告*/
  }
  
    chdir(caTempDir);
    sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    system(caLabel);
    system("tar -cvzf report.tgz *");
	sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	system(caLabel);
	sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	system(caLabel);
//释放
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	
	printf("%s end ...........................\n",__FUNCTION__);
	
   return 0;
}




/* 
服务协议使用情况分析报表 上行，下行，总共
*/
int msasrvreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
    printf("%s START ...........................\n",__FUNCTION__);	
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char    oldname[256];
    char    caSrv[64];
   
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long lTime,lTime1,lTime2;
    int otype;
    char templname[256];

    int  userid;
    int   gid;   
    int   glevel=0; 
    int  srvindex;    
    char *reportname;
    char *lt_page_content=NULL;
    int  iFd;
    char caTempDir[256];
  
    char caLabel[256];
    unsigned long long lMaxBytes;
	char email[25];
	char strBuf[1024],tempBuf[1024],caWhere[1024];
	  memset(strBuf,0,sizeof(strBuf));
	  memset(tempBuf,0,sizeof(tempBuf));
	  memset(caWhere,0,sizeof(caWhere));
	 
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


    iFd=-1;

    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }else{     
        memset(templname,0,sizeof(templname));
        strgb2utf8(reportname,templname,strlen(reportname));
        reportname=templname;   
    }
   
    sprintf(caTempDir,"/datacenter/msa/report/srvreport/%lu/",ltStrGetId());
    printf("caTempDir:  %s\n",caTempDir);
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
      ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
			ltMsgFree(ltMsgPk);
       return 0;
       }

   }  
    userid=0;
    lMaxBytes=1;
    userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
    otype=atol(ltMsgGetVar_s(ltMsgPk,"otype"));
    tempCursor=NULL;
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }
    else{
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
	if(lTime1 > lTime) {//传进来的日期大于系统当前时间
         lTime1 = lTime;
    }
    if(lTime2>lTime){
		 lTime2=lTime;
	}
    nasTimeGetDate(caSdate1,lTime);

    sprintf(caLabel,"/bin/echo 'date=%s' > %sinfo.ini",caSdate1,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'name=%s' >> %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	  sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
  	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);

	//获取部门管理级别  
		sprintf(tempBuf,"select level from msagroup where id=%d",gid);
		printf("tempBuf::%s\n",tempBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,tempBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				printf("tempRow[0]::%s\n",tempRow[0]);
				glevel=atol(tempRow[0]);
			 }
		}
 		ltDbCloseCursor(tempCursor);

 if(otype==1){/*全部汇总*/
			  	if(gid==-1){
			        sprintf(sqlBuf,"select service,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest\
			         from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'\
			         group by service order by abyte desc ",caSdate1,caSdate2);     
			    }else{
			    		if(userid==-1){//某部门下所用用户
						    sprintf(sqlBuf,"select c.name ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,c.id\
						     from msasrvreport a,msauser b,msagroup c \
						     where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d\
						     group by c.name,c.id,service order by c.id,abyte desc ",caSdate1,caSdate2,glevel,glevel,gid);	
			    	  }else{//某部门下指定用户
			    			sprintf(sqlBuf,"select b.dispname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest\
			    			from msasrvreport a,msauser b where (substring(sdate,3,8)>='%s') and (substring(sdate,3,8)<='%s') and a.userid=b.userid and b.userid=%d\
			    			group by b.dispname,a.userid,service order by a.userid,abyte desc ",caSdate,caEdate,userid);
			    	  } 
			    }
			    printf("sqly: %s\n",sqlBuf);
}else if(otype==2){/*按部门汇总*/
			  if(gid==-1){
		         sprintf(sqlBuf,"select c.name ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,c.id\
		           from msasrvreport a,msauser b,msagroup c where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id\
		           group by c.name,c.id, c.name,service order by c.id,abyte desc ",caSdate1,caSdate2,glevel);
		      }else{
			  		 sprintf(sqlBuf,"select c.name ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest,c.id\
			  		  from msasrvreport a,msauser b,msagroup c\
			  		  where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d\
			  		   group by c.id,c.name,service order by c.id,abyte desc ",caSdate1,caSdate2,glevel,glevel,gid);
			  }
			  printf("sqlq: %s\n",sqlBuf);
}else if(otype==4){/*按日期汇总*/
			 if(gid==-1){
		         sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest\
		          from msasrvreport where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s'\
		          group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2);
			 }else{
				   	if(userid==-1){
				   		sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest\
				   		 from msasrvreport a,msauser b\
				   		 where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=%d\
				   		 group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2,glevel,gid);
				   	}else{
				   		sprintf(sqlBuf,"select sdate, service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(ctime) as arequest\
				   		 from msasrvreport a where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=%d\
				   		  group by sdate,service order by sdate asc,abyte desc ",caSdate1,caSdate2,userid);
				    }
			 }
		printf("sqle: %s\n",sqlBuf);
}else if(otype==3|| otype==7){/*3.按用户连接汇总 7.按部门人员汇总*/
			int i;
			int iTemp;
			long long *srvflow;
			int       *srvrequest;
			long long usersrvflow;
			int       usersrvrequest;
			char      caLine[25000];
			char      caggg[64];

	      if(gid==-1){
		   		sprintf(sqlBuf,"select b.dispname ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.name\
		   		 from msasrvreport a,msauser b,msagroup c\
		   		 where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id\
		   		  group by a.userid,b.dispname,c.name,service order by a.userid,abyte desc ",caSdate1,caSdate2,glevel);
		  }else{
		  		if(userid==-1){
	    			  sprintf(sqlBuf,"select b.username ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.name\
	    			   from msasrvreport a,msauser b,msagroup c\
	    			   where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=%d and b.groupid%d=c.id\
	    			    group by a.userid,b.username,c.name,service order by a.userid,abyte desc ",caSdate1,caSdate2,glevel,gid,glevel);	
	    		}else{
	    				sprintf(sqlBuf,"select b.username ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.name\
	    				 from msasrvreport a,msauser b,msagroup c\
	    				  where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.userid=%d and b.groupid%d=c.id\
	    				 group by a.userid,b.username,c.name,service order by a.userid,abyte desc ",caSdate1,caSdate2,userid,glevel);
	    	    } 
		   } 
 	  printf("sqlw:%s\n",sqlBuf);
 	  
	  srvflow=(long long *)malloc(sizeof(long long)*(MAX_TOP_SRVNUM+1));
	  srvrequest=(int *)malloc(sizeof(int)*(MAX_TOP_SRVNUM+1));
	  usersrvflow=0;
	  usersrvrequest=0;
	  
		 for(i=0;i<MAX_TOP_SRVNUM;i++){		
	    	srvflow[i]=0;
	    	srvrequest[i]=0;	
	    }
	    
 		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf); 		 
	  if(tempCursor == NULL) {
	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	  		system(caLabel);/*建立目录和正在处理标志*/
	  		ltMsgFree(ltMsgPk);
	        return 0;
	  }
		  
	  dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
	  tablePtr=lt_dbput_table(dbPtr,"glist");
	  for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){ 
	          if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)>0){ 
	          	 printf("srvname: %s\n",_ltPubInfo->topSrvName[iTemp].srvname);
	             lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iTemp].srvname);     
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
    						     sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,caggg,usersrvflow);
									}else if(otype==7){
    									sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
									}
									
									 for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){ 
											if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)>0){ 
													if(otype==3){
														sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
												    }else{
												        sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
												    }	
											}
									 }
									 sprintf(caLine,"%s</tr>",caLine);
						  	   lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
						  }
						  
						  sprintf(oldname,"%s",tempRow[0]);						 
						  sprintf(caggg,"%s",tempRow[4]);
						  printf("oldname: %s\n",oldname);
						  printf("caggg: %s\n",caggg);
						  usersrvflow=0;
						  usersrvrequest=0;
						  for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){ 	 
								    	srvflow[iTemp]=0;
								    	srvrequest[iTemp]=0;
						  }
				    	  iTemp=atol(tempRow[1]);
				    	  printf("iTemp: %d\n",iTemp);
				    	  srvflow[iTemp]=atol(tempRow[2]);//流量
						  srvrequest[iTemp]=atol(tempRow[3]);//请求次数
						  usersrvflow=usersrvflow+atol(tempRow[2]);
		  				  usersrvrequest=usersrvrequest+atol(tempRow[3]);
    	     }else{
	    	        iTemp=atol(tempRow[1]);
	    	        srvflow[iTemp]=atol(tempRow[2]);
					srvrequest[iTemp]=atol(tempRow[3]);
					usersrvflow=usersrvflow+atol(tempRow[2]);
			        usersrvrequest=usersrvrequest+atol(tempRow[3]);
    	    }
        tempRow= ltDbFetchRow(tempCursor);
    }
    
    ltDbCloseCursor(tempCursor);
    
    if(oldname[0]!='\0'){/*新的一行开始写*/
	        if(otype==3){
							sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldK</td>",oldname,caggg,usersrvflow);
					}else if(otype==7){
							sprintf(caLine,"<td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
					}
					
					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){ 
							if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)>0){ 
								 if(otype==3){
									sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
							     }else{
							     sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
							     }		
							}
					}
					
					sprintf(caLine,"%s</tr>",caLine);
					lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		}
		
		 lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
		 
		 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usersrvplt.htm",dbPtr->doc,0);
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
	    chdir(caTempDir);
	    sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
	    system(caLabel);
		  system("tar -cvzf report.tgz *");
		  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
		  system(caLabel);
		  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
		  system(caLabel);
		  ltMsgFree(ltMsgPk);
		  lt_dbfree(dbPtr);
		  return 0;
		  
}else if(otype==5 || otype==6 ){/*5:按部门日期流量汇总 6:按部门日期连接汇总*/
	   int  groupnum;
	   int  i;
	   char caLine[16396];
	   int  oldgroupid;
		 int  oldsrvid;
		 int  xgroupid;
		 char oldDate[64];
		 int iTemp;
		 typedef struct gname_s {
		    long long    gflow;
		    int             gtime;
		 } nasGname;
    	nasGname gnamelist[1000];
	    long long gflowlist[1000];
	    long long gtimelist[1000];
  
      for(i=0;i<_MAX_ALLGRPNUM;i++){/*结构体初始化*/
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
	  
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    		system(caLabel);/*建立目录和正在处理标志*/
	    		ltMsgFree(ltMsgPk);
	        return 0;
	  }
	  
	  dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"glist");
	  tempRow= ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){	  
	    	_ltPubInfo->_GroupList[groupnum].groupid=atol(tempRow[0]);
	    		printf("gid:%d\n",_ltPubInfo->_GroupList[groupnum].groupid);
	    	sprintf(_ltPubInfo->_GroupList[groupnum].name,"%s",tempRow[1]);
	    	lt_dbput_recordvars(tablePtr,1,"groupname",LT_TYPE_STRING,tempRow[1]);
	    	tempRow= ltDbFetchRow(tempCursor);
	    	groupnum++;
	  		if(groupnum>1000){
	    		 break;
	    	}
	    }
	  ltDbCloseCursor(tempCursor);
	  
	  oldgroupid=-1;
	  oldsrvid=-1;
	  memset(oldDate,0,sizeof(oldDate));
	  if(gid==-1){
       sprintf(sqlBuf,"select a.sdate as xdate,service,c.id ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.id\
        from msasrvreport a,msauser b,msagroup c\
        where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id\
        group by a.sdate,service,c.id order by xdate asc,service,c.id,abyte desc ",caSdate1,caSdate2,glevel);
	  }else{
	  	 sprintf(sqlBuf,"select a.sdate as xdate,service,c.id ,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest,c.id\
	  	 from msasrvreport a,msauser b,msagroup c\
	  	 where substring(sdate,3,8)>='%s' and substring(sdate,3,8)<='%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d\
	  	 group by a.sdate,service,c.id order by xdate asc,service,c.id,abyte desc ",caSdate1,caSdate2,glevel,glevel,gid);
	  }
	  printf("sqla: %s\n",sqlBuf);
	  
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    		system(caLabel);/*建立目录和正在处理标志*/
	    		ltMsgFree(ltMsgPk);
	        return 0;
	  }
	  
	  tablePtr=lt_dbput_table(dbPtr,"llist");
	  tempRow= ltDbFetchRow(tempCursor);  
      while(tempRow!=NULL){
    	if( strcmp(oldDate,tempRow[0])!=0){/*day change 全变了，新的一行加时间*/
    		if( oldDate[0]!='\0' ){/*写入旧的一行*/	
		    		    for(i=0;i<_MAX_ALLGRPNUM;i++){  		  	
			    		  	if(otype==5){
			    		  		sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[i].gflow);
						      }else{
						        sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
						      }
					    }
    					lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
    					
		    			if(otype==5){
		    				sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n<td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n<td class=xl2816118 style='border-top:none;border-left:none'>Totle Flow</td>");
						  }else if(otype==6){
		    				sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n<td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n<td class=xl2816118 style='border-top:none;border-left:none'>Totle Time</td>");
					    }
					    
 					    for(i=0;i<_MAX_ALLGRPNUM;i++){
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
    		for(i=0;i<_MAX_ALLGRPNUM;i++){
		        gnamelist[i].gflow=0;
		    	gnamelist[i].gtime=0;
		    	gflowlist[i]=0;
		    	gtimelist[i]=0;
		    }
		    		    
		    sprintf(caSrv,"%s","??");
		    iTemp=atol(tempRow[1]);//服务
			  sprintf(caSrv,"%s",_ltPubInfo->topSrvName[iTemp].srvname);
    		sprintf(oldDate,"%s",tempRow[0]);//日期
    		sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n<td class=xl5016118 style='border-top:none;border-left:none'>%s</td>\r\n<td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",tempRow[0],caSrv);
    		oldsrvid=atol(tempRow[1]);
    		xgroupid=atol(tempRow[2]);
    		for(i=0;i<_MAX_ALLGRPNUM;i++){
    			    gnamelist[i].gflow=atol(tempRow[3]);
		    		gnamelist[i].gtime=atol(tempRow[4]);
		    		gflowlist[i]=gflowlist[i]+gnamelist[i].gflow;
		    		gtimelist[i]=gtimelist[i]+gnamelist[i].gtime;
		    		break;
    		}
    				
    	}else{/*keep day*/
    		if( oldsrvid!=atol(tempRow[1]) ){/*服务变了,新的一行开始*/
					if( oldsrvid!=-1 ){
						for(i=0;i<_MAX_ALLGRPNUM;i++){
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
		  		  
	    		  	for(i=0;i<_MAX_ALLGRPNUM;i++){
				    	gnamelist[i].gflow=0;
				    	gnamelist[i].gtime=0;
				    }
			    
	    		  oldsrvid=atol(tempRow[1]);
	    		  xgroupid=atol(tempRow[2]);
	    		  memset(caLine,0,sizeof(caLine));
			      sprintf(caSrv,"%s","??");
				   iTemp=atol(tempRow[1]);
				   sprintf(caSrv,"%s",_ltPubInfo->topSrvName[iTemp].srvname);
	    		  sprintf(oldDate,"%s",tempRow[0]);
	    		  sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n<td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n<td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",caSrv);
	    		  oldsrvid=atol(tempRow[1]);
	    		  xgroupid=atol(tempRow[2]);
		    	    for(i=0;i<_MAX_ALLGRPNUM;i++){
		    			  if(_ltPubInfo->_GroupList[i].groupid==xgroupid){
		    					gnamelist[i].gflow=atoll(tempRow[3]);
					    		gnamelist[i].gtime=atol(tempRow[4]);
					    		gflowlist[i]=gflowlist[i]+gnamelist[i].gflow;
				    		    gtimelist[i]=gtimelist[i]+gnamelist[i].gtime;
				    		break;
		    			  }
				    }
			    
            }else{/*服务没变，增加部门*/
    	  	    xgroupid=atol(tempRow[2]);
    		    for(i=0;i<_MAX_ALLGRPNUM;i++){
	    			if(_ltPubInfo->_GroupList[i].groupid==xgroupid){
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
	  printf("ppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp\n");  
	  if( oldDate[0]!='\0' ){/*写入旧的一行*/		  		 
    		 	for(i=0;i<_MAX_ALLGRPNUM;i++){
					//	    		  	 if(_ltPubInfo->_GroupList[i].groupid==-1){
					//	    		  		break;
					//    		  	 }
	    		  	 if(otype==5){
	    		  		sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[i].gflow);
				      }else{
				        sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
				      }
			    }
			   
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
      }
	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvgroupdateplt.htm",dbPtr->doc,0);
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
	    }
	  lt_dbfree(dbPtr);
   	  chdir(caTempDir);
      sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
      system(caLabel);
	  system("tar -cvzf report.tgz *");  
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);    
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);   
	   printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n");  
	  ltMsgFree(ltMsgPk);
	  printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n");  
	  lt_dbfree(dbPtr);
	  
	  printf("%s end ...........................\n",__FUNCTION__);
	  
  return 0;
} 
	  
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor == NULL){
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
	    	  system(caLabel);/*建立目录和正在处理标志*/ 
	    	  ltMsgFree(ltMsgPk); 
	          return 0;
	  }
	  
	  if(otype==1){
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
	  
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    tempRow= ltDbFetchRow(tempCursor);
    memset(oldname,0,sizeof(oldname));
    srvindex=1;
    while(tempRow!=NULL){
    	    if(strcmp(oldname,tempRow[0])==0){
						if(otype==1){
								sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
						}else{
							    sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
						}
						
    	    	lt_dbput_recordvars(tablePtr,4,
           	"gname",LT_TYPE_STRING,"",
						"sname",LT_TYPE_STRING,caSrv,
						"sc",LT_TYPE_LONGLONG,atoll(tempRow[3]),
						"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]) 
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
					   int iTemp;
					   iTemp=0;
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
		    	   lt_dbput_recordvars(tablePtr,4,
		            "gname",LT_TYPE_STRING,caGname,
								"sname",LT_TYPE_STRING,caSrv,
								"sc",LT_TYPE_LONGLONG,atoll(tempRow[3]),
								"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2])
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
    
    if(otype==1){//画图
					  close(iFd);	 
					  sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %ssrvreport.gif %llu ",caTempDir,caTempDir,lMaxBytes);  //柱状      饼图：onlinesrv1.pl
					  system(caLabel);	
					  lt_dbput_rootvars(dbPtr,1,"piechart","srvreport.gif" );
	  }  

//////////////////////////////////////////////////////////解析页面 

	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);
	  if(otype==1){
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvreportpltall.htm",dbPtr->doc,0);
	  }else if(otype==2){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvreportpltdepart.htm",dbPtr->doc,0);
	  }else if(otype==4){
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/srvreportpltdate.htm",dbPtr->doc,0);
	  }

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

   	  chdir(caTempDir);
      sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
      system(caLabel);
	  system("tar -cvzf report.tgz *");  
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);    
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);   

	  ltMsgFree(ltMsgPk);

	  lt_dbfree(dbPtr);
	  
	  printf("%s end ...........................\n",__FUNCTION__);
	  
  return 0;

}



//用户计费报告 开始生成报告调用函数
int msajfreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{    
    printf("%s  START ...............................................\n",__FUNCTION__);
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char templname[256]; 
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    long lTime,lTime1,lTime2; 
    char caLabel[128];
    char caType[30];
    char tempBuf[255];
    int jfType;
    long account,accountnum,payaccount;
    char straccount[32];
    char straccountnum[32];
    char strpayaccount[32];
    char *reportname;
    char *lt_page_content;
    int  iFd;
    char caTempDir[256];
    
    int  userid;
    int   gid;   
    int   glevel=0; 

    char myddd[32];
    int myhour;
    int mymon;
    int mysec;
    int  pUint=1;
    int  accountflag=1; 
	char email[25];

    pUint=atol(ltMsgGetVar_s(ltMsgPk,"punit"));
    accountflag=atol(ltMsgGetVar_s(ltMsgPk,"accountflag"));

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
    
    
    userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
  
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

         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");

    if(reportname==NULL){
       reportname="********";
    }else{
      memset(templname,0,sizeof(templname));
      strgb2utf8(reportname,templname,strlen(reportname));
      reportname=templname;
    } 
    sprintf(caTempDir,"/datacenter/msa/report/jfreport/%lu/",ltStrGetId());

    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
             ltMsgPk->msgpktype=1;
             lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");               
             ltMsgFree(ltMsgPk);
             return 0;

        }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;   

    sprintf(caLabel,"/bin/echo 'date=%s %s' > %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'name=%s' >> %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'user=%s' >> %sinfo.ini",ltMsgGetVar_s(ltMsgPk,"manageruser"),caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/   
     sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/  
 
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");               
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
        
 	//获取部门管理级别  
		sprintf(tempBuf,"select level from msagroup where id=%d",gid);
		printf("tempBuf::%s\n",tempBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,tempBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				printf("tempRow[0]::%s\n",tempRow[0]);
				glevel=atol(tempRow[0]);
			 }
		}
 		ltDbCloseCursor(tempCursor);

  
/*top30 访问次数的排序*/

if(pUint==1){  
         if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s')  group by  b.userid order by a.username asc ",glevel,caSdate1,caSdate2);
         }else{
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s' and b.accountflag=%d )  group by  b.userid order by a.username asc ",glevel,caSdate1,caSdate2,accountflag);
  }

}else if(pUint==2){
         if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s')   group by  b.userid order by c.name ",glevel,caSdate1,caSdate2);
         }else{
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s' and b.accountflag=%d )   group by  b.userid order by c.name ",glevel,caSdate1,caSdate2,accountflag);
  }

}else if(pUint==3){
         if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s') group by  b.userid  order by aflow asc ",glevel,caSdate1,caSdate2);
         }else{
                  sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes) as flow,sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and b.sdate<='%s' and b.accountflag=%d ) group by  b.userid  order by aflow asc ",glevel,caSdate1,caSdate2,accountflag);

  }

}else if(pUint==4){
         if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s')  group by  b.userid order by aflow asc ",glevel,caSdate1,caSdate2);
         }else{
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s' and b.accountflag=%d )  group by  b.userid order by aflow asc ",glevel,caSdate1,caSdate2,accountflag);
  }

         

}else if(pUint==5){
         if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account)  from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s') group by  b.userid order by aflow asc ",glevel,caSdate1,caSdate2);
         }else{
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account)  from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s' and b.accountflag=%d ) group by  b.userid order by aflow asc ",glevel,caSdate1,caSdate2,accountflag);

  }

         

}else{
  if(accountflag==100){
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s') group by  b.userid order by a.username asc ",glevel,caSdate1,caSdate2);
         }else{
                   sprintf(sqlBuf,"select a.dispname,a.jftype,a.accountnum,c.name ,sum(b.ubytes+b.dbytes),sum(b.onlinetime),sum(b.account) from msaAccountReport b,msauser a,msagroup c where (a.userid=b.userid and  a.groupid%d=c.id and substring(b.sdate,3,8)>='%s' and substring(b.sdate,3,8)<='%s' and b.accountflag=%d ) group by  b.userid order by a.username asc ",glevel,caSdate1,caSdate2,accountflag);
  }

}
printf("sqlBuf::%s\n",sqlBuf);    
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

  if(tempCursor == NULL) {
               sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
               system(caLabel);/*建立目录和正在处理标志*/
               ltMsgFree(ltMsgPk);

               return 0;

   }

    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"list");
    tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    jfType=atol(tempRow[1]);
    account=atol(tempRow[6]);
    accountnum=atol(tempRow[2]);
    payaccount=0;

    if(jfType==0){
               sprintf(caType,"%s","不计费");
               payaccount=0;
    }else if(jfType==1){
               sprintf(caType,"%s","包月计费");
               payaccount=0;
    }else if(jfType==2){
               sprintf(caType,"%s","流量计费");
               payaccount=account;
    }else if(jfType==3){
               sprintf(caType,"%s","时间计费");
               payaccount=account;
    }else if(jfType==4){
               sprintf(caType,"%s","预付流量计费");
               if(accountnum>=0){
                        payaccount=0;
               }else{
                        payaccount=accountnum;
               }

    }else if(jfType==5){
               sprintf(caType,"%s","预付时间计费");
               if(accountnum>=0){
                        payaccount=0;
               }else{
                        payaccount=accountnum;
               }

    }
  

 //   sprintf(flow,"%lld", atoll(tempRow[4])/1000.000);
 
    sprintf(straccount,"%.2f",account/100.00);
    sprintf(straccountnum,"%.2f",accountnum/100.00);
    sprintf(strpayaccount,"%.2f",payaccount/100.00);
    memset(myddd,0,sizeof(myddd));
    myhour=atol(tempRow[5])/3600;
    mysec=atol(tempRow[5])%3600; 
    mymon=mysec/60;
    mysec=mysec%60;
    sprintf(myddd,"%02d:%02d:%02d",myhour,mymon,mysec);
    lt_dbput_recordvars(tablePtr,8,
                                     "uname",LT_TYPE_STRING,tempRow[0],
                                     "group",LT_TYPE_STRING,tempRow[3],
                                     "flow",LT_TYPE_LONGLONG,atoll(tempRow[4]),
                                     "time",LT_TYPE_STRING,myddd,
                                     "type",LT_TYPE_STRING,caType,
                                     "account",LT_TYPE_STRING,straccount,
                                     "accountnum",LT_TYPE_STRING,straccountnum,
                                     "payaccount",LT_TYPE_STRING,strpayaccount);
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
    sprintf(caLabel,"计费时间:  %s--%s",caSdate1,caSdate2);
    lt_dbput_rootvars(dbPtr,1,"lab",caLabel); 
    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/jfreportplt.htm",dbPtr->doc,0);
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

  chdir(caTempDir);
  sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
  printf("change to mht :: %s\n",caLabel);
  system(caLabel);
  system("tar -cvzf report.tgz *");
  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
  system(caLabel);
  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
  system(caLabel);
 
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  
  printf("%s  END ...............................................\n",__FUNCTION__);
  return 0;
}

