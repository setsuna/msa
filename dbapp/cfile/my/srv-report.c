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




/*自定义分析中服务协议分析节点调用函数
功能：返回页面*/
int msashowsrvreportpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbHeadPtr dbPtr; 
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");//页面初始化
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/srvreport.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);
 
    return 0;
    		   
}


/*自定义分析中的网络流量分析节点
功能：返回页面*/
int msashowconnectflowpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");//页面初始化
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/connectflow.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);

    return 0;
    
    
    	
    	
}



/*自定义分析中的网络次数分析
功能：返回页面*/
int msashowconnectreportpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");//页面初始化

    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/connectreport.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);

    return 0;
    	
    	
    	
}



/*自定义分析中的网站访问分析
功能：返回页面*/
int msashowwebreportpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");//页面初始化

    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/reportcenter/webreport.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);
 
    return 0;	
    
    	
    		
}






/*用户时间分析 报告生成调用函数*/
int msausertimereportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;   
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    long lTime,lTime1,lTime2; 
    char *reportname;
    char *lt_page_content;
    int  iFd;
    char caTempDir[256];
 
    char caLabel[256];
    char templname[256];
	char email[25];
	 
    iFd=-1;
    
    
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }else{
      memset(templname,0,sizeof(templname));
      strgb2utf8(reportname,templname,strlen(reportname));
      reportname=templname;
    }
 printf("reportname: %s\n",reportname);
 
	  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			  sprintf(email,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
  printf("email: %s\n",email);  
  
    sprintf(caTempDir,"/datacenter/msa/report/timereport");
    if(ltFileIsExist(caTempDir)!=1) {//创建目录
    	mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }  
    sprintf(caTempDir,"/datacenter/msa/report/timereport/%lu/",ltStrGetId());
printf("caTempDir: %s\n",caTempDir);  
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
 			      lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,0);//生成报告错误		
			      ltMsgFree(ltMsgPk);
            return 0; 
        } 
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
	
    nasTimeGetDate(caSdate1,lTime); //获取当前日期
    sprintf(caLabel,"/bin/echo 'date=%s' > %sinfo.ini",caSdate1,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'name=%s' >> %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
//sprintf(caLabel,"/bin/echo 'user=%d' >> %sinfo.ini",userid,caTempDir);//ltMsgGetVar_s(ltMsgPk,"manageruser")
//system(caLabel);/*建立目录标志*/ 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	 sprintf(caLabel,"/bin/cp  /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp  /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp  /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp  /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp  /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);  
    
    printf("dateAaaaa: %s\n",caSdate1);
    printf("dateBbbbb: %s\n",caSdate2);

    int      groupnum;
    int      i;
    char   caLine[30000];    
    int  oldgroupid;
	int  oldsrvid;
	int  xgroupid;
	char oldDate[64];
	char xName[256];
	/*定义部门信息结构体*/
    typedef struct gname_s {
//		    int             gid;
//		    int             glevel;
//		    char          gname[50];
		    long long    gflow;
		    int             gtime;
		} nasGname;
    nasGname gnamelist[1000];

    for(i=0;i<_MAX_ALLGRPNUM;i++){/*结构体初始化*/
    	gnamelist[i].gflow=0;
    	gnamelist[i].gtime=0;    	
    } 
    
   
	  /*将部门名称显示到  部门网络汇总模板页面 
	     部门有五个级别0,1,2,3,4
	     msagroup表处理字段id
	     msauser表处理字段groupid0,groupid1,groupid2,groupid3,groupid4
	  */
	  groupnum=0;	  
	  sprintf(sqlBuf,"select id,name,level from msagroup order by id asc ");	 	 
	  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);/*链接数据库获取光标*/
	  if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    	  system(caLabel);/*建立目录和正在处理标志*/
	    	  ltMsgFree(ltMsgPk);
	          return 0;
	  }
	  
	  dbPtr=lt_dbinit();/*页面初始化*/
      lt_db_htmlpage(dbPtr,"utf-8");//页面编码设置
      tablePtr=lt_dbput_table(dbPtr,"glist");/*开始模板参数*/
	  tempRow= ltDbFetchRow(tempCursor);
      while(tempRow!=NULL){/*将部门信息存入结构体*/     	
      	_ltPubInfo->_GroupList[groupnum].groupid=atol(tempRow[0]);
      	_ltPubInfo->_GroupList[groupnum].level=atoi(tempRow[2]);
      	sprintf(_ltPubInfo->_GroupList[groupnum].name,"%s",tempRow[1]);
      	sprintf(xName,"<a href='g%d.htm'>%s</a>", 
			      	_ltPubInfo->_GroupList[groupnum].groupid,
			      	_ltPubInfo->_GroupList[groupnum].name);
//		      	gnamelist[groupnum].glevel=atoi(tempRow[2]);
//		    	gnamelist[groupnum].gid=atol(tempRow[0]);
//		    	sprintf(_ltPubInfo->_GroupList[groupnum].name,"%s",tempRow[1]);
//		    	sprintf(xName,"<a href='g%d.htm'>%s</a>", gnamelist[groupnum].gid,gnamelist[groupnum].gname);		    	
		    	lt_dbput_recordvars(tablePtr,1, "groupname",LT_TYPE_STRING,xName);
		    	tempRow= ltDbFetchRow(tempCursor);
		    	 groupnum++;		    	 
		    	  if(groupnum>1000){
		    		 break;
		    	 }
		    	 
//		    	 if(groupnum>000){
//		    		 break;
//		    	 }
     }
     
	 ltDbCloseCursor(tempCursor);/*关闭数据库*/
	 
	  oldgroupid=-1;
	  oldsrvid=-1;
	  memset(oldDate,0,sizeof(oldDate));	 
	  sprintf(sqlBuf,"select sum(onlinetime) as atime,sdate,a.userid,c.id from msaaccountreport a,msauser b ,msagroup c\
					 where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid%d=c.id \
					 group by sdate,a.userid,ctime,c.id, b.groupid%d order by sdate,b.groupid%d,ctime desc",
					 caSdate1,caSdate2,
					 _ltPubInfo->_GroupList[groupnum].level,
					 _ltPubInfo->_GroupList[groupnum].level,
					 _ltPubInfo->_GroupList[groupnum].level
					 );
printf("sqlBuf2 :%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor == NULL) {
		      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    	  system(caLabel);/*建立目录和正在处理标志*/
	    	  ltMsgFree(ltMsgPk);	    	  
	          return 0;
	  }
	 tempRow= ltDbFetchRow(tempCursor);
	 tablePtr=lt_dbput_table(dbPtr,"llist");   
     while(tempRow!=NULL){
	    	if( strcmp(oldDate,tempRow[1])!=0){//day change 全变了，新的一行加时间   		
		    		if( oldDate[0]!='\0' ){//写入旧的一行  第一次运行跳出此条件
			    		  	 for(i=0;i<_MAX_ALLGRPNUM;i++){
//						  		  	if(_ltPubInfo->_GroupList[i].groupid==-1){
//						  		  		break;
//						  		  	}
			    		  			sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);
						     }
			    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
			    			memset(caLine,0,sizeof(caLine));
			    			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
			    			//printf("caLine2:　%s\n",caLine);
			    			for(i=0;i<_MAX_ALLGRPNUM;i++){//时间	  	
//				    		  		if(_ltPubInfo->_GroupList[i].groupid==-1){
//						  		  		break;
//						  		  	}
			    		  		sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
						    }
		    				lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		    			 	memset(caLine,0,sizeof(caLine));
		    			 	sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
			    			for(i=0;i<_MAX_ALLGRPNUM;i++){//平均时间
//					    		  	if(_ltPubInfo->_GroupList[i].groupid==-1){
//					    		  		break;
//					    		  	}
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
		    		
		    		for(i=0;i<_MAX_ALLGRPNUM;i++){
					    	gnamelist[i].gflow=0;
				    	    gnamelist[i].gtime=0;
				    }
			    
		    		sprintf(oldDate,"%s",tempRow[1]);
		    		memset(caLine,0,sizeof(caLine));
		    		sprintf(caLine,"<tr height=19 style='height:14.25pt'>\r\n \
							  <td rowspan=3 height=57 class=xl488922 style='border-bottom:.5pt solid black;\r\n \
							  height:42.75pt;border-top:none'>%s</td> \r\n \
							  <td class=xl248922 width=81 style='border-top:none;border-left:none;\r\n \
							  width:61pt'>上网人数</td>",oldDate);
		    		xgroupid=atol(tempRow[3]);
		    		
		    		for(i=0;i<_MAX_ALLGRPNUM;i++){
			    			if(_ltPubInfo->_GroupList[i].groupid==xgroupid){
			    				gnamelist[i].gflow++;
					    		gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
					    		break;
			    			}
					  }
					  
	        }else{//keep day    		
	    	  	    xgroupid=atol(tempRow[3]);
	    		  	for(i=0;i<_MAX_ALLGRPNUM;i++){
			    			if(_ltPubInfo->_GroupList[i].groupid==xgroupid){
			    				gnamelist[i].gflow++;
					    		gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
					    		break;
			    			}
				    }
	        }
	        
           tempRow= ltDbFetchRow(tempCursor);
    }    
    
	 ltDbCloseCursor(tempCursor);//关闭


	  if( oldDate[0]!='\0' ){//写入旧的一行   第一次执行区域
    		   for(i=0;i<_MAX_ALLGRPNUM;i++){   		  	
//	    		  	if(_ltPubInfo->_GroupList[i].groupid==-1){
//	    		  		break;
//	    		  	}
    		  		sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);			   
			    }
			   
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
    			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
    			for(i=0;i<_MAX_ALLGRPNUM;i++){//时间
//	    		  	if(_ltPubInfo->_GroupList[i].groupid==-1){
//	    		  		break;
//	    		  	}
    		  		sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
			    }
			    
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine); 
		
    			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
    			for(i=0;i<_MAX_ALLGRPNUM;i++){//平均  	
//		    		  	if(_ltPubInfo->_GroupList[i].groupid==-1){
//		    		  		break;
//		    		  	}
		    		  	if(gnamelist[i].gflow){
		    		  		sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime/gnamelist[i].gflow));
		    		  	}else{
		    		  	 	sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>0:0:0</td>",caLine);
		    		    }
			    }
    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
      }
      

	  lt_dbput_rootvars(dbPtr,3,"reportname",reportname,"bdate",caSdate1,"edate",caSdate2);

	  lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usertimeplt.htm",dbPtr->doc,0);//报告种类：部门网络时间汇总	  	 
	  if(lt_page_content==NULL){//判断解析模板
				  sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    		  system(caLabel);//建立目录和正在处理标志
	    		  ltMsgFree(ltMsgPk);
	    		  lt_dbfree(dbPtr);
	              return 0;
	  }else{			
			  lt_page_content=strstr(lt_page_content,"<html>");
			  sprintf(caLabel,"%sindex.htm",caTempDir);
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
			 if(iFd >0) {
			          write(iFd,lt_page_content,strlen(lt_page_content));
			    }
	     	close(iFd);

	   }


//单个用户流向显示和上网时间显示
//   流量汇总和时间汇总
//   服务汇总报表生成程序  	
printf("====================================================================================\n");
		char  oldname[128];
	  long long *srvflow;
		int       *srvrequest;
		long long usersrvflow;
		int       usersrvrequest;
		int       iTemp;

	  srvflow=(long long *)malloc(sizeof(long long)*(MAX_TOP_SRVNUM+1));
	  srvrequest=(int *)malloc(sizeof(int)*(MAX_TOP_SRVNUM+1));  
      memset(sqlBuf,0,sizeof(sqlBuf));
  
 //生成网络服务汇总报表
// int j;
// for(j=0;j<100;j++){ //时间
//	      printf("bbbj= %d,level: %d\n",j,_ltPubInfo->_GroupList[j].level);
//	      printf("aaaj= %d,gid: %d\n",j,_ltPubInfo->_GroupList[j].groupid);
//}
for(i=0;i<_MAX_ALLGRPNUM;i++){ //时间
//		 if(_ltPubInfo->_GroupList[i].groupid==-1){		 
//		    		  break;
//		  }
		  usersrvflow=0;
		  usersrvrequest=0;
	      for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
	   			srvflow[iTemp]=0;
	    		srvrequest[iTemp]=0;
	      }
	      memset(sqlBuf,0,sizeof(sqlBuf));	      

//	      printf("aaai= %d,level: %d\n",i,_ltPubInfo->_GroupList[i].level);
//	      printf("bbbi= %d,gid: %d\n",i,_ltPubInfo->_GroupList[i].groupid);
    	  sprintf(sqlBuf,"select b.username ,service,sum(ubytes+dbytes)/1024+1 as abyte ,sum(onlinetime) as arequest from msasrvreport a,msauser b where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid%d=%d \
    	   group by a.userid,b.username,service order by a.userid,abyte desc ",caSdate1,caSdate2,_ltPubInfo->_GroupList[i].level,_ltPubInfo->_GroupList[i].groupid);		 
		// printf("sql: %s\n",sqlBuf);
		 
		  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		  if(tempCursor == NULL) {
			     sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		    	 system(caLabel);
		         return 0;
		  }

		  dbPtr=lt_dbinit();
		  lt_db_htmlpage(dbPtr,"utf-8");
		  tablePtr=lt_dbput_table(dbPtr,"glist");//构造部门信息列表
		  for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
		   	  lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iTemp].srvname);
		   	 // printf("srvname: %s\n",_ltPubInfo->topSrvName[iTemp].srvname);
		  }


		  tablePtr=lt_dbput_table(dbPtr,"llist");
		  tempRow= ltDbFetchRow(tempCursor);
	 	  memset(oldname,0,sizeof(oldname));
	      while(tempRow!=NULL){	      
	    	    if(strcmp(oldname,tempRow[0])!=0){//新的一行	    	    		
							  if(oldname[0]!='\0'){//新的一行开始写
		    							sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s</td>\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,usersrvflow);
										for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
													if(_ltPubInfo->topSrvName[iTemp].srvname==NULL){
																											break;
																										}
										    		sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
										  }														
									      sprintf(caLine,"%s</tr>",caLine);
									     
								  	      lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);			  	  
		    							sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s</td>\r\n\
		    							<td class=xl25724 style='border-top:none;border-left:none'>%s</td>","",time2str(usersrvrequest));					
										for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
										 	if(_ltPubInfo->topSrvName[iTemp].srvname==NULL){
															break;
														}
										     sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%d</td>",caLine,srvrequest[iTemp]);
										}									
									   sprintf(caLine,"%s</tr>",caLine);
								  	   lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);				  	  
							  }
							  
							  sprintf(oldname,"%s",tempRow[0]);
							  usersrvflow=0;
							  usersrvrequest=0;
							  for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
							   		srvflow[iTemp]=0;
							    	srvrequest[iTemp]=0;
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
							  	sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s</td>\r\n\
							  	 <td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,usersrvflow);								
								for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
	                                    sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
								}
	                            //sprintf(caLine,"%s</tr>",caLine);
							  	lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);		
							  	 
	    						sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s</td>\r\n\
	    						 <td class=xl25724 style='border-top:none;border-left:none'>%s</td>","",time2str(usersrvrequest));
								for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
	                            		sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%d</td>",caLine,srvrequest[iTemp]);
								}
								//sprintf(caLine,"%s</tr>",caLine);
							  	lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		 }
	  lt_dbput_rootvars(dbPtr,2,"reportname",reportname,"bdate",caSdate1);
	  lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/usersrvplt.htm",dbPtr->doc,0);
	  if(lt_page_content==NULL){	  	
	    		lt_dbfree(dbPtr);
	             return 0;
		}else{		
			   lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
			   sprintf(caLabel,"%sg%d.htm",caTempDir,_ltPubInfo->_GroupList[i].groupid);
			   //printf("caLabel: %s\n",caLabel);
			   iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
			    if(iFd >0) {
			          write(iFd,lt_page_content,strlen(lt_page_content));
			    }
   				close(iFd);
	    }
	    lt_dbfree(dbPtr);
}  	  

	  chdir(caTempDir);
      sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
      system(caLabel);
	  system("tar -cvzf report.tgz *");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel); 	  	  
//	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
//	  system(caLabel);	    	  
	  ltMsgFree(ltMsgPk); 	  
	  lt_dbfree(dbPtr);
	  
	  printf("%s end ..............................................\n",__FUNCTION__);
 
      return 0;
 		
}



/*网络流量分析 开始生成报告函数调用*/
int msaconnectflowproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    printf("%s start-----------------------------\n",__FUNCTION__);
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    char caTempDir[128];

    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    } sCount[35];

    int lUnit;
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
    
    char templname[256];
    int  gid;
    int  userid;
    
    char caEdate[32];
    char *reportname;
    char *lt_page_content;
    int  iFd;
    int glevel=0;
    unsigned long long *lTop;//??
    int  iTemp;
    char  oldgname[128];
    char  egname[128];
    int  topuser=100;
    unsigned long long  lMax;
    userid=0;
    char email[25];
	//char stime1[24],stime2[24],stime3[24],stime4[24];
	//int time1,time2,time3,time4;
	char strBuf[1024];
	//,strBuf2[1024];
	//int iIndex=0;

	memset(strBuf,0,sizeof(strBuf));

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
printf("email: %s\n",email);
	userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
printf("userid: %d\n",userid);
printf("gid: %d\n",gid);

    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }
    else {
        lTime1 = 0;
    }
 printf("aaaalTime: %ld\n",lTime);   
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"24:00:00");
    }
    else {
        lTime2 = lTime;
    }
printf("aaaalTime2: %ld\n",lTime2);      
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }else{

      memset(templname,0,sizeof(templname));
      strgb2utf8(reportname,templname,strlen(reportname));
      reportname=templname;
    }
    
    
    /*获取部门管理级别*/	  
sprintf(strBuf,"select level from msagroup where id=%d",gid);
printf("strBuf::%s\n",strBuf);
tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			printf("tempRow[0]::%s\n",tempRow[0]);
			glevel=atol(tempRow[0]);
		}
	}
ltDbCloseCursor(tempCursor);
printf("glevel::%d\n",glevel);  
    
    
    sprintf(caTempDir,"/datacenter/msa/report/flowreport/%lu/",ltStrGetId());
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
//sprintf(caLabel,"/bin/echo 'user=%s' >> %sinfo.ini",ltMsgGetVar_s(ltMsgPk,"manageruser"),caTempDir);
//system(caLabel);/*建立目录标志*/
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
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		

    lUnit=atol(ltMsgGetVar_s(ltMsgPk,"punit"));
printf("lUnit: %d\n",lUnit);
    tempCursor=NULL;
    iMaxItem = 32;
    dbPtr=NULL;
    lt_page_content=NULL;
    lTop=NULL;
    if(lUnit==1) {/*天*/   	
		        memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
		        lStep = 24 * 3600;//按秒计算一天
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
		         printf("lTime0: %ld\n",lTime0);
		        iMaxItem = 32;
		        for(i=0;i<iMaxItem;i++) {
			    	nasCvtStime(lTime0,caSdate,caTime);
		            memcpy(sCount[i].caItem,caSdate+8,2);		            
		            lTime0 = lTime0 + lStep;
		        }
		        nasTimeGetDate(caSdate1,lTime1);
		        nasTimeGetDate(caSdate2,lTime2);
		        nasTimeGetDate(caTime1,lTime1);
			      nasTimeGetDate(caTime2,lTime2);
		    printf("caTime1: %s\n",caTime1);
	      printf("caTime2: %s\n",caTime2);
		        caTime1[2]=0;
			      caTime2[2]=0;			    
	      printf("caSdate1: %s\n",caSdate1);
	      printf("caSdate2: %s\n",caSdate2);
	      printf("hour--caTime1: %s\n",caTime1);
	      printf("hour--caTime2: %s\n",caTime2);
				if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
						sprintf(caSdate,"%s","");
				}else{
				    	sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
				}
	      
				 if(gid==-1){
		        			sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) \
									from msatimereport where ((sdate > '%s') or(sdate = '%s' and hours>='%s')) and \
									((sdate < '%s') or(sdate = '%s' and hours<='%s')) \
									 group by sdate ",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
		         }else{
		          	 if(userid!=-1){
		       	    	       	     sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) \
												from msatimereport \
												where userid = %d and ((sdate > '%s') or(sdate = '%s' and hours>='%s')) and \
												((sdate < '%s') or(sdate = '%s' and hours<='%s'))\
												group by sdate ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
						}else{
							 sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) \
												from msatimereport a,msauser b \
												where a.userid = b.userid and b.groupid%d=%d and ((sdate > '%s') or(sdate = '%s' and hours>='%s')) and \
												((sdate < '%s') or(sdate = '%s' and hours<='%s'))\
												group by sdate ",glevel,gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
					  }       
		         }
		      
		      printf("sqlA: %s\n",sqlBuf);
		      tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

		    }else if(lUnit==2) {/*用户*/
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
						sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1, c.name\
						    from msasrvreport a, msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'\
							and a.userid = b.userid and b.groupid%d=c.id\
							group by a.userid,b.dispname,c.name order by abyte desc offset 0 limit %d ",caSdate1,caSdate2,glevel,topuser);
				 }else{
				 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1,c.name\
				 		    from msasrvreport a,msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s' \
							and a.userid = b.userid and b.groupid%d=c.id and b.groupid%d=%d\
							group by a.userid,b.dispname,c.name order by abyte desc offset 0 limit %d ",caSdate1,caSdate2,glevel,glevel,gid,topuser);
				 }
			      printf("strBuf::%s\n",sqlBuf);
		       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);	     
		    }else if(lUnit==3) {/*用户上流量排序*/
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
						sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.name\
						    from msasrvreport a,msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
							and a.userid = b.userid and b.groupid%d=c.id\
							group by a.userid,b.dispname,c.name order by abyte desc offset  0 limit %d ",caSdate1,caSdate2,glevel,topuser);
				 }else{
				 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.name\
				 		 from msasrvreport a,msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'\
						 and a.userid = b.userid and b.groupid%d=c.id and b.groupid%d=%d\
						 group by a.userid,b.dispname,c.name order by abyte desc offset 0 limit %d ",caSdate1,caSdate2,glevel,glevel,gid,topuser);
				 }
			   printf("strBuf::%s\n",sqlBuf);
		       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		     
		    }else if(lUnit==4) {/*用户下流量排序*/
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
						sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.name\
						    from msasrvreport a,msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'\
							and a.userid = b.userid and b.groupid%d=c.id\
							group by a.userid,b.dispname,c.name order by abyte desc offset 0 limit %d ",caSdate1,caSdate2,glevel,topuser);
				 }else{
				 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.name\
				 		 from msasrvreport a,msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
						 and a.userid = b.userid and b.groupid%d=c.id and b.groupid%d=%d  \
						 group by a.userid,b.dispname,c.name order by abyte desc offset 0 limit %d ",caSdate1,caSdate2,glevel,glevel,gid,topuser);
				 }
			      printf("strBuf::%s\n",sqlBuf);
		       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		     
		    }else if(lUnit==5){
			  nasTimeGetDate(caSdate1,lTime1);
			  nasTimeGetDate(caSdate2,lTime2);
			  memset(oldgname,0,sizeof(oldgname));
			  memset(egname,0,sizeof(egname));
		  	sprintf(sqlBuf,"select c.name,c.name, \
			sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)\
			from msasrvreport a,msauser b ,msagroup c \
			where a.sdate >= '%s' and a.sdate <= '%s'  \
			and a.userid = b.userid and  b.groupid%d=c.id\
			group by c.name order by mybytes desc  ",caSdate1,caSdate2,glevel);
		   printf("strBuf::%s\n",sqlBuf);
		  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		  
		    }else if(lUnit==6){
			  nasTimeGetDate(caSdate1,lTime1);
			  nasTimeGetDate(caSdate2,lTime2);
			  memset(oldgname,0,sizeof(oldgname));
			  memset(egname,0,sizeof(egname));
		  
			sprintf(sqlBuf,"select b.username,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)\
						from msasrvreport a,msauser b ,msagroup c \
						where a.sdate >= '%s' and a.sdate <= '%s'  \
						and a.userid = b.userid and b.groupid%d=c.id\
						group by b.username,b.groupid%d,c.name order by b.groupid%d, mybytes desc  ",caSdate1,caSdate2,glevel,glevel,glevel);
		   printf("strBuf::%s\n",sqlBuf);
		  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		
		    }else if(lUnit==7){
	  nasTimeGetDate(caSdate1,lTime1);
	  nasTimeGetDate(caSdate2,lTime2);
	  memset(oldgname,0,sizeof(oldgname));
	  memset(egname,0,sizeof(egname));
  
	sprintf(sqlBuf,"select c.name,a.sdate,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime)\
				from msasrvreport a,msauser b ,msagroup c \
				where a.sdate >= '%s' and a.sdate <= '%s'  \
				and a.userid = b.userid and  b.groupid%d=c.id\
				group by c.name,a.sdate order by a.sdate,mybytes desc  ",caSdate1,caSdate2,glevel);
   printf("strBuf::%s\n",sqlBuf);
  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }
        
    if(tempCursor == NULL) {
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		system(caLabel);/*建立目录和正在处理标志*/
    		ltMsgFree(ltMsgPk);
            return 0;
    }

    if(lUnit==1 || lUnit==2 ){ 
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
		   printf("caLabel: %s\n",caLabel);
		   lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
		   lt_dbput_rootvars(dbPtr,1,"reportname",reportname); 
	   
		   if(lUnit==1){
		    	lt_dbput_rootvars(dbPtr,1,"xunit","Day");
		   }else{
				lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
		   }
	    
	   	   tablePtr=lt_dbput_table(dbPtr,"list2");  
			
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
		        for(i=0;i<iMaxItem;i++){
		           if(strcmp(sCount[i].caItem,caItem)==0) { 
		                sCount[i].lUbytes = lUbytes / 1024;
		                sCount[i].lDbytes = lDbytes / 1024;
						lt_dbput_recordvars(tablePtr,4,
						"mytime",LT_TYPE_STRING,sCount[i].caItem,
						"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
						"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
						"abyte",LT_TYPE_LONGLONG,sCount[i].lUbytes+sCount[i].lDbytes);	
				        sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes+sCount[i].lDbytes,sCount[i].lDbytes,sCount[i].lUbytes);	
				        write(iFd, caLabel, strlen(caLabel));	
		                break;
		           }
		        } 
		        tempRow= ltDbFetchRow(tempCursor);
		    }	    
		    ltDbCloseCursor(tempCursor);	
		    close(iFd); 
			 
	     printf("caLabelCC: %s\n",caLabel);
         sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowplt.pl  %sdata.ini %sflowreport1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 printf("caLabelA: %s\n",caLabel);
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file1","flowreport1.jpg");

		 sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/outflowpltline.pl %sdata.ini %sflowreport2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		 printf("caLabelB: %s\n",caLabel);
		 system(caLabel);	
		 lt_dbput_rootvars(dbPtr,1,"file2","flowreport2.jpg");	 	     	    
	     lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/flowreportplt.htm",dbPtr->doc,0);
	
		 }else if(lUnit==2 || lUnit==3  || lUnit==4 )   { 
			
			iTemp=0;
		    lMax=0;
		    dbPtr=lt_dbinit();
		    lt_db_htmlpage(dbPtr,"utf-8");
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
					  if(lUnit==2){
							lTop[iTemp]=atoll(tempRow[2]);
					  }else if(lUnit==3){
							lTop[iTemp]=atoll(tempRow[3]);
					  }else if(lUnit==4){
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
				    		ltMsgFree(ltMsgPk);
				        return 0;
				}	
				for(i=0;i<iTemp;i++) {
		
		        ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
		        sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
				write(iFd, caLabel, strlen(caLabel));	
		 
		    }
				close(iFd);	
			 
				sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv1.pl  %sdata.ini %sflowreport.gif %llu ",caTempDir,caTempDir,lMax);
				system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreport.gif\">" );
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
		        sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
						write(iFd, caLabel, strlen(caLabel));	
		    }
		
				close(iFd);	
					 
				sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));
		
				system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"file1","flowreport1.jpg"); 
		 
		
			}  
			 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userreportplt.htm",dbPtr->doc,0);
			
		 }else if(lUnit==5 || lUnit==6  || lUnit==7 )   { 
		printf("================================================\n");
		  dbPtr=lt_dbinit();
	      lt_db_htmlpage(dbPtr,"utf-8");
	      tablePtr=lt_dbput_table(dbPtr,"flowlist");
	    
	    tempRow= ltDbFetchRow(tempCursor);
	    while(tempRow!=NULL){
				if(lUnit==5 ){
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
	    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/groupreportplt.htm",dbPtr->doc,0);
	 }
  
   if(lt_page_content==NULL){
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    		system(caLabel);//建立目录和正在处理标志
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
	  //生成html格式报告
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


printf("%s  end..............\n",__FUNCTION__);  

  return 0;




}




//网络次数分析  开始生成报告调用函数
int msaconnectreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	printf("%s start -----------------------\n",__FUNCTION__);
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    char caTempDir[256];
	ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char templname[256];
    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    } sCount[35];

    int lUnit;
    unsigned long iMaxItem=0;

    unsigned long long lUbytes,lMaxBytes;
    int      i,iMax;
    long lStep,lTime,lTime1,lTime2,lTime0;
    char caSdate[32],caItem[16];//caTime[32]
    char caSdate1[32],caSdate2[32],caTime1[32];//caTime2[32]
    char caLabel[256];
    char caTemp[8192];

    int   gid;
    int   glevel=0;
    int    userid=0;;
    char caEdate[32];
    char *reportname;
    char *lt_page_content=NULL;
    int  iFd;
    unsigned long long *lTop=NULL;
    int  iTemp;
    char  oldgname[128];
	char  egname[128];
    int  topuser=100;
    unsigned long long  lMax=0;
	char email[25];
	//char stime1[24],stime2[24],stime3[24],stime4[24];
	//int time1,time2,time3,time4;
	char strBuf[1024];//strBuf2[1024];

	memset(strBuf,0,sizeof(strBuf));

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }

printf("caSdate:  %s\n",caSdate);

    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
printf("caEdate:  %s\n",caEdate);

	if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			sprintf(email,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
printf("email:  %s\n",email);

	userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
 printf("userid:  %d\n",userid);
  printf("gid:  %d\n",gid);

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
    
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }else{

      memset(templname,0,sizeof(templname));
      strgb2utf8(reportname,templname,strlen(reportname));
      reportname=templname;
    }
    
    
            /*获取部门管理级别*/	  
	sprintf(strBuf,"select level from msagroup where id=%d",gid);
	printf("strBuf::%s\n",strBuf);
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				printf("tempRow[0]::%s\n",tempRow[0]);
				glevel=atol(tempRow[0]);
			}
		}
	ltDbCloseCursor(tempCursor);
	printf("glevel::%d\n",glevel);  
    
    sprintf(caTempDir,"/datacenter/msa/report/connectreport/%lu/",ltStrGetId());
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
//    sprintf(caLabel,"/bin/echo 'user=%s' >> %sinfo.ini",ltMsgGetVar_s(ltMsgPk,"manageruser"),caTempDir);
//    system(caLabel);/*建立目录标志*/
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
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/connectreportlist.htm\" >察看新报告?</a></body></html>");		

    lUnit=atol(ltMsgGetVar_s(ltMsgPk,"punit"));
   printf("lUnit:%d\n",lUnit);
    tempCursor=NULL;
		dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");

if(lUnit==3) {/*用户*/        
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
		  printf("caSdate1: %s\n",caSdate1);
		  printf("caSdate2: %s\n",caSdate2);
		  printf("topuser: %d\n",topuser);

			if(gid==-1){
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte, c.name from msasrvreport a, \
				msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
				and a.userid = b.userid and b.groupid%d=c.id \
				group by a.userid,b.dispname,c.name \
				order by abyte desc  offset 0 limit %d ",caSdate1,caSdate2,glevel,topuser);
				 }else{
				 		sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte,c.name from msasrvreport a, \
				msauser b,msagroup c where a.sdate >= '%s' and a.sdate <= '%s'  \
				and a.userid = b.userid and b.groupid%d=c.id and b.groupid%d=%d\
				group by a.userid,b.dispname,c.name \
				order by abyte desc  offset 0 limit %d  ",caSdate1,caSdate2,glevel,glevel,gid,topuser);
		    }
	    
		    printf("sql:%s\n",sqlBuf);
	       tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
     
 }else if(lUnit==6){
		 	nasTimeGetDate(caSdate1,lTime1);
		    nasTimeGetDate(caSdate2,lTime2);
		    memset(oldgname,0,sizeof(oldgname));
		    memset(egname,0,sizeof(egname));
			  printf("caSdate1: %s\n",caSdate1);
			  printf("caSdate2: %s\n",caSdate2);
			  printf("topuser: %d\n",topuser);
  
		 sprintf(sqlBuf,"select c.name,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime) \
					from msasrvreport a,msauser b ,msagroup c \
					where a.sdate >= '%s' and a.sdate <= '%s'  \
					and a.userid = b.userid and  b.groupid%d=c.id \
					group by c.id,c.name order by mybytes desc  ",caSdate1,caSdate2,glevel);
	 		 printf("sql:%s\n",sqlBuf);
 			 tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
  
}else if(lUnit==7){
		  nasTimeGetDate(caSdate1,lTime1);
		  nasTimeGetDate(caSdate2,lTime2);
		  memset(oldgname,0,sizeof(oldgname));
		  memset(egname,0,sizeof(egname));
  
		sprintf(sqlBuf,"select b.username,c.name,sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes \
			from msasrvreport a,msauser b ,msagroup c \
			where a.sdate >= '%s' and a.sdate <= '%s'  \
			and a.userid = b.userid and b.groupid%d=c.id \
			group by b.username,c.name,b.groupid%d order by b.groupid%d, mybytes desc  ",caSdate1,caSdate2,glevel,glevel,glevel);
		 printf("sql:%s\n",sqlBuf);
 		 tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

}else if(lUnit==8){
			nasTimeGetDate(caSdate1,lTime1);
		    nasTimeGetDate(caSdate2,lTime2);
		    memset(oldgname,0,sizeof(oldgname));
		    memset(egname,0,sizeof(egname));
		      printf("caSdate1: %s\n",caSdate1);
			  printf("caSdate2: %s\n",caSdate2);
			  printf("topuser: %d\n",topuser);
		  
			sprintf(sqlBuf,"select c.name,a.sdate, \
			sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes \
			from msasrvreport a,msauser b ,msagroup c \
			where a.sdate >= '%s' and a.sdate <= '%s'  \
			and a.userid = b.userid and  b.groupid%d=c.id\
			group by a.sdate,c.name order by a.sdate,mybytes desc  ",caSdate1,caSdate2,glevel);
			printf("sql:%s\n",sqlBuf);
			  tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);

}
     
    if(tempCursor == NULL) {
				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		    system(caLabel);/*建立目录和正在处理标志*/
    		    ltMsgFree(ltMsgPk);
                return 0;
    }

 if(lUnit==1 || lUnit==2 ){ 
		  //dbPtr=lt_dbinit();
	      sprintf(caLabel,"%sdata.ini",caTempDir);
	      iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		  if(iFd == (-1)) {
						sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
			    		system(caLabel);/*建立目录和正在处理标志*/
			    		ltDbCloseCursor(tempCursor);	
			            return 0;
			}		 
		   sprintf(caLabel,"%s--%s",caSdate,caSdate);
		   if(lUnit==1){  
		    	lt_dbput_rootvars(dbPtr,1,"xunit","Day");
		   }else{
				lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
		   }
		    
		    tablePtr=lt_dbput_table(dbPtr,"list2");
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
		                sCount[i].lUbytes = lUbytes ;
						lt_dbput_recordvars(tablePtr,2,
						"mytime",LT_TYPE_STRING,sCount[i].caItem,
						"request",LT_TYPE_LONGLONG,sCount[i].lUbytes);	
				        sprintf(caLabel,"%s,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes);
				        write(iFd, caLabel, strlen(caLabel));			
		                break;
		           }
		        } 
		        tempRow= ltDbFetchRow(tempCursor);
		    }  
          ltDbCloseCursor(tempCursor);
   	    close(iFd);	//关闭文件			 
		sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		system(caLabel);	 
		lt_dbput_rootvars(dbPtr,1,"file1","flowreport1.jpg");
  		 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/requestreportplt.htm",dbPtr->doc,0);

}else if(lUnit==3  ) {   
			 iTemp=0;
		    lMax=0;
		   
		    tablePtr=lt_dbput_table(dbPtr,"flowlist");	    
		    tempRow= ltDbFetchRow(tempCursor);
		    while(tempRow!=NULL){
						lt_dbput_recordvars(tablePtr,4,
									"iIndex",LT_TYPE_LONG,iTemp,
									"uname",LT_TYPE_STRING,tempRow[1],
									"lbyte",LT_TYPE_STRING,tempRow[2],
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
		    		ltMsgFree(ltMsgPk);
		        return 0;
		}	
		for(i=0;i<iTemp;i++) {
	        ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
	        sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
			write(iFd, caLabel, strlen(caLabel));			    
         }
		close(iFd);						  
		sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv1.pl  %sdata.ini %sflowreport.gif %llu ",caTempDir,caTempDir,lMax);
		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreport.gif\" width=\"623\" height=\"349\">" );
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
	        sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
			    write(iFd, caLabel, strlen(caLabel));	
	        }
			close(iFd);	 
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file1","flowreport1.jpg"); 
	 }	  
	 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userrequestreportplt.htm",dbPtr->doc,0);
	
}else if(lUnit==6 || lUnit==7  || lUnit==8 )  { 	
	    tablePtr=lt_dbput_table(dbPtr,"flowlist");  
	    tempRow= ltDbFetchRow(tempCursor);
	    while(tempRow!=NULL){
printf("===============================================================\n");	
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
	   sprintf(caLabel,"%s--%s",caSdate,caSdate);
	   lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
	   lt_dbput_rootvars(dbPtr,1,"reportname",reportname);       
	   lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/grouprequestreportplt.htm",dbPtr->doc,0);
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
  sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
  printf("change to mht :: %s\n",caLabel);
  system(caLabel);
  system("tar -cvzf report.tgz *");
  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
  system(caLabel);
  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
  system(caLabel);

  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);  
 
printf("%s end -----------------------\n",__FUNCTION__);

  return 0;
  
  
  
  
}  



//网站访问分析 开始生成报告
int msawebreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	printf("%s start-----------------------------\n",__FUNCTION__);
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    int iTemp;
    int        gid;
    int        userid;
    char       xName[64]; 
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    long lTime,lTime1,lTime2;
    char caLabel[128];
    int  oType;
    int  sType;
    char templname[256];   
//    struct xs_group{
//    	char strGroup[128];
//        int  idGroup;
//	};

    char       oldname[256];
	//struct xs_group xmyGroup[256]; /*最多256个部门*/	  
	char *reportname;
    char *lt_page_content;
    int  iFd;
    int glevel=0;
    char caTempDir[256]; 
	char email[25];
	//char stime1[24],stime2[24],stime3[24],stime4[24];
	//int time1,time2,time3,time4;
	char strBuf[1024];//strBuf2[1024];

	memset(strBuf,0,sizeof(strBuf));

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

	 if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			sprintf(email,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
 

	  memset(oldname,0,sizeof(oldname));
	  userid=0;
	  userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	  gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
printf("userid: %d\n",userid);
printf("gid: %d\n",gid);
	  
//	sprintf(sqlBuf,"select id,name from msagroup");
//    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
//    if(tempCursor == NULL) {
//			ltMsgPk->msgpktype=1;
//			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
//			ltMsgFree(ltMsgPk);
//            return 0;
//    }
//    tempRow= ltDbFetchRow(tempCursor);
//    iTemp=0;
//    while(tempRow!=NULL){
//    	xmyGroup[iTemp].idGroup=atol(tempRow[0]);
//    	sprintf(xmyGroup[iTemp].strGroup,"%s",tempRow[1]);
//    	iTemp++;
//    	if(iTemp>250){
//    		break;
//    	}
//    	tempRow= ltDbFetchRow(tempCursor);
//    }
//    ltDbCloseCursor(tempCursor);

    oType=atol( ltMsgGetVar_s(ltMsgPk,"otype") );
    sType=atol( ltMsgGetVar_s(ltMsgPk,"stype") );

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
    
        //获取部门管理级别	  
sprintf(strBuf,"select level from msagroup where id=%d",gid);
printf("strBuf::%s\n",strBuf);
tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			printf("tempRow[0]::%s\n",tempRow[0]);
			glevel=atol(tempRow[0]);
		}
	}
ltDbCloseCursor(tempCursor);
printf("glevel::%d\n",glevel);  
    
    sprintf(caTempDir,"/datacenter/msa/report/webreport/%lu/",ltStrGetId());
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
    system(caLabel);//建立目录标志
    sprintf(caLabel,"/bin/echo 'name=%s' >> %sinfo.ini",reportname,caTempDir);
    system(caLabel);//建立目录标志
//sprintf(caLabel,"/bin/echo 'user=%s' >> %sinfo.ini",ltMsgGetVar_s(ltMsgPk,"manageruser"),caTempDir);
//system(caLabel);//建立目录标志
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);//建立目录正在处理标志   
    sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/right-top.jpg  %s",caTempDir);
    system(caLabel);//建立目录正在处理标志
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/banner.jpg  %s",caTempDir);
    system(caLabel);//建立目录正在处理标志
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-1.jpg  %s",caTempDir);
    system(caLabel);//建立目录正在处理标志
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-2.jpg  %s",caTempDir);
    system(caLabel);//建立目录正在处理标志
	sprintf(caLabel,"/bin/cp /app/msa/msa/htmlplt/images/up-3.jpg  %s",caTempDir);
    system(caLabel);//建立目录正在处理标志
  

    ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		

    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
 
printf("caSdate1: %s\n",caSdate1);
printf("caSdate2: %s\n",caSdate2);
printf("oType: %d\n",oType);
printf("sType: %d\n",sType);
//top30 访问次数的排序
if(oType==1){
	printf("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");
  	if(gid==-1){  
      if(sType==2){
					sprintf(sqlBuf,"select host ,sum(bytes) as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort\
					 from msauserhttpreport \
					 where sdate >= '%s' and sdate <= '%s'\
					  group by host,urlsort order by acount desc offset 0 limit 50",caSdate1,caSdate2 );
			}else if(sType==1){
					sprintf(sqlBuf,"select host ,sum(bytes) as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort\
					 from msauserhttpreport \
					 where sdate >= '%s' and sdate <= '%s' \
					 group by host,urlsort order by abyte desc offset 0 limit 50",caSdate1,caSdate2);
			}
    }else{
    		if(userid==-1){
						if(sType==2){ 
								sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b,msagroup c  \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d\
								  group by c.id, a.host order by acount desc offset 0 limit 30",caSdate1,caSdate2,glevel,glevel,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,nasuser b,msagroup c  \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id  and b.groupid%d=%d\
								  group by c.id, a.host,a.urlsort order by abyte desc offset 0 limit 30",caSdate1,caSdate2,glevel,glevel,gid);
						}
    		}else{
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								    from msauserhttpreport a,msauser b  \
									where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d \
									group by b.dispname,b.userid, a.host,a.urlsort order by acount desc offset 0  limit 30",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b  \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d \
								 group by b.dispname,b.userid, a.host,a.urlsort  order by abyte desc offset 0 limit 30",caSdate1,caSdate2,userid);
						}
    	  } 
   }

}else if(oType==2){
	printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
  	if(gid==-1){
      	if(sType==2){
						sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
						  from msauserhttpreport a,msauser b,msagroup c  \
						where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id\
						 group by c.id, a.host order by c.id,acount desc ",caSdate1,caSdate2,glevel);
				}else if(sType==1){
								sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b,msagroup c  \
							      where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id\
								  group by c.id, a.host order by c.id,abyte desc ",caSdate1,caSdate2,glevel);
			  }
    }else{
    		if(userid==-1){
						if(sType==2){
								sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b,msagroup c  \
						          where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d\
						           group by c.id, a.host order by acount desc ",caSdate1,caSdate2,glevel,glevel,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b,msagroup c  \
						          where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id  and b.groupid%d=%d\
						          group by c.id, a.host order by abyte desc ",caSdate1,caSdate2,glevel,glevel,gid);
						}
    		}else{
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								from msauserhttpreport a,msauser b  \
								where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d\
								 group by b.userid, a.host,b.dispname,a.urlsort order by acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b  \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d\
								 group by b.userid, a.host,b.dispname,a.urlsort order by abyte desc ",caSdate1,caSdate2,userid);
						}
    	 } 
   }

}else if(oType==3){
	printf("4444444444444444444444444444444444444444444444444444444444444444444444444444444444\n");
  	if(gid==-1){
      	if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								     from msauserhttpreport a,msauser b  \
									 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid\
									 group by a.userid, a.host,b.dispname,a.urlsort order by a.userid, acount desc ",caSdate1,caSdate2);
				}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								 from msauserhttpreport a,msauser b \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid \
								 group by a.userid, a.host,b.dispname,a.urlsort order by a.userid,abyte desc ",caSdate1,caSdate2);
			  }
    }else{
    		if(userid==-1){
						if(sType==2){
								sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								    from msauserhttpreport a,msauser b,msagroup c  \
									where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.id and b.groupid%d=%d \
						 			group by a.userid, a.host, b.dispname,a.urlsort order by a.userid,acount desc ",caSdate1,caSdate2,glevel,glevel,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b,msagroup c  \
								  where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=c.groupid  and b.groupid%d=%d\
						  		  group by a.userid, a.host,b.dispname,a.urlsort order by a.userid,abyte desc ",caSdate1,caSdate2,glevel,glevel,gid);
						}
    		}else{
    		    if(sType==2){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b  \
						          where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d\
						          group by b.userid, a.host,b.dispname,a.urlsort order by acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  b.dispname,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a,msauser b  \
						          where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.userid=%d\
						          group by b.userid, a.host,b.dispname,a.urlsort order by abyte desc ",caSdate1,caSdate2,userid);
						}
    	  } 
   }
}else if(oType==4){
	printf("2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\n");
  	if(gid==-1){
      if(sType==2){
					sprintf(sqlBuf,"select a.sdate, host ,sum(bytes) as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort\
					 from msauserhttpreport a \
			        where sdate >= '%s' and sdate <= '%s' group by a.sdate,host order by a.sdate,acount desc ",caSdate1,caSdate2);
			}else if(sType==1){
					sprintf(sqlBuf,"select  a.sdate,host ,sum(bytes) as abyte,sum(ctime) as atime, sum(lcount) as acount,urlsort\
					 from msauserhttpreport a\
			        where sdate >= '%s' and sdate <= '%s' group by  a.sdate,host order by  a.sdate,abyte desc ",caSdate1,caSdate2);
			}
    }else{
    		if(userid==-1){
						if(sType==2){
								sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount ,a.urlsort\
								 from msauserhttpreport a,msauser b  \
								 where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid  and b.groupid%d=%d\
						 		group by a.urlsort,a.sdate, a.host order by a.sdate,acount desc ",caSdate1,caSdate2,glevel,gid);
						  }else if(sType==1){
								sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								    from msauserhttpreport a,msauser b \
									where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=b.userid and b.groupid%d=%d \
									group by a.urlsort,a.sdate, a.host order by a.sdate,abyte desc ",caSdate1,caSdate2,glevel,gid);
						}
    		}else{
    		    if(sType==2){
								sprintf(sqlBuf,"select  a.sdate,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a\
								 where a.sdate >= '%s' and a.sdate <= '%s' and  a.userid=%d\
								  group by a.sdate, a.host,a.urlsort order by a.sdate,acount desc ",caSdate1,caSdate2,userid);
						}else if(sType==1){
								sprintf(sqlBuf,"select  a.sdate,a.host ,sum(a.bytes) as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort\
								  from msauserhttpreport a\
								  where a.sdate >= '%s' and a.sdate <= '%s' and a.userid=%d\
								  group by a.sdate, a.host,a.urlsort order by a.sdate,abyte desc ",caSdate1,caSdate2,userid);
						}
    	  } 
   }
}

printf("sql:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor == NULL) {
		    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		system(caLabel);//建立目录和正在处理标志
    		ltMsgFree(ltMsgPk);
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
					"usort",LT_TYPE_STRING,_ltPubInfo->_urlSortlist[atol(tempRow[5])].sortname,
					"lbyte",LT_TYPE_LONG,atol(tempRow[2])/1024,
					"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
					"lrequest",LT_TYPE_STRING,tempRow[4]);
			  }
			  iTemp++;
			 }else{
			   sprintf(oldname,"%s",tempRow[0]);
			   sprintf(xName,"<b>%s</b>",oldname);
			    lt_dbput_recordvars(tablePtr,5,
				"uname",LT_TYPE_STRING,xName,
				"usort",LT_TYPE_STRING,"",
				"lbyte",LT_TYPE_STRING,"",
				"ltime",LT_TYPE_STRING,"",
				"lrequest",LT_TYPE_STRING,"");
			   lt_dbput_recordvars(tablePtr,5,
				"uname",LT_TYPE_STRING,tempRow[1],
				"usort",LT_TYPE_STRING,_ltPubInfo->_urlSortlist[atol(tempRow[5])].sortname,
				"lbyte",LT_TYPE_LONG,atol(tempRow[2])/1024,
				"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
				"lrequest",LT_TYPE_STRING,tempRow[4]);
			  iTemp=0;
			 }
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
    sprintf(caLabel,"Web Report:  %s--%s",caSdate1,caSdate2);
	 lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
	 lt_dbput_rootvars(dbPtr,1,"reportname",reportname);
  lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/webreportplt.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    		system(caLabel);//建立目录和正在处理标志
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
	  //生成html格式报告
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

printf("%s end-----------------------------\n",__FUNCTION__);
return 0;

 }
 
