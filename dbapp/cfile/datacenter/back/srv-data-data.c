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

#define SRVCSV_PATH   "/datacenter/msa/report/msaexport/srvrecord/srvexport.csv" 
#define SRVXLS_PATH  "/datacenter/msa/report/msaexport/srvrecord/srvexport.xls"
#define HTTPCSV_PATH  "/datacenter/msa/report/msaexport/httprecord/httpexport.csv" 
#define HTTPXLS_PATH  "/datacenter/msa/report/msaexport/httprecord/httpexport.xls" 
/*
 MSA:数据中心
     详细访问记录
         服务访问记录
         阻断访问记录
         HTTP访问记录
 功能：页面链接调用函数
 */
int msaShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 int    showtype=0;
	 char   caFile1[256];
	 int    len=0;
	 int    myindex=0;
	 msasDir *fnames;
	 ltTablePtr  tablePtr;
	 if(ltMsgGetVar_s(ltMsgPk,"showtype")){
	 	showtype=atoi(ltMsgGetVar_s(ltMsgPk,"showtype"));
	 }
   
   sprintf(caFile1,"%s",_datacenterdir);
   fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); 
	 }
     
   dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
	 tablePtr=lt_dbput_table(dbPtr,"datelist");
	 while(len--){
   	if(strlen(fnames[len].caDirName)==8 && fnames[len].caDirName[0]=='1'){
   		if(myindex==0){
   			lt_dbput_recordvars(tablePtr,2,
   			"strdate",LT_TYPE_STRING,fnames[len].caDirName,
   			"dsel",LT_TYPE_STRING,"selected");
   			myindex++;
   		}else{
   			lt_dbput_recordvars(tablePtr,2,
   			"strdate",LT_TYPE_STRING,fnames[len].caDirName,
   			"dsel",LT_TYPE_STRING,"");
   		}
   	}
   }
	 
	 
   ltMsgPk->msgpktype=1;
   if(showtype==1){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
	 }else if(showtype==2){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/BlockAccessList.htm",dbPtr->head,0);
	 }else if(showtype==3){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/HttpAccessList.htm",dbPtr->head,0);
	 }
	 ltMsgFree(ltMsgPk);
	 
	 if(fnames){
 		free(fnames);
   }
   
   
	 return 0;   
}


/*
 MSA:数据中心
     服务访问记录搜索
     服务阻断记录搜索
     http访问记录搜索
 功能:页面链接调用函数
 */
int msaShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 int    showtype=0;
	 char   caFile1[256];
	 int    len=0;//目录个数
	 int    iii;
	 char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 msasDir *fnames;
	 ltTablePtr  tablePtr;
	 
	  //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	 }
		
	 
	 if(ltMsgGetVar_s(ltMsgPk,"showtype")){
	 	showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	 }
   
   char *sGroups=NULL;
   sGroups=ltMsgGetVar_s(ltMsgPk,"managergroup");
   if(!sGroups){
  	sGroups="999999";
   }
	
   sprintf(caFile1,"%s",_datacenterdir);
   fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); 
	 }
      	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
	 tablePtr=lt_dbput_table(dbPtr,"datelist");
	 while(len--){
   	if(strlen(fnames[len].caDirName)==8 && fnames[len].caDirName[0]=='1' ){
   		lt_dbput_recordvars(tablePtr,1,"strdate",LT_TYPE_STRING,fnames[len].caDirName);
   	}
   }
   
   
   //一级服务
   tablePtr=lt_dbput_table(dbPtr,"srvlist");
   for(iii=0;iii<MAX_TOP_SRVNUM;iii++){
	   	if(strlen(_ltPubInfo->topSrvName[iii].srvname)>0){
	   	 lt_dbput_recordvars(tablePtr,2,
	   	 "srvid",LT_TYPE_LONG,iii,
	   	 "srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
	    }else{
	     break;	
	    }
   }
   
   //二级服务
   tablePtr=lt_dbput_table(dbPtr,"twosrvlist");
   for(iii=0;iii<MAX_SYS_SRVNUM;iii++){
   	  if(strlen(_ltPubInfo->ltService[iii].srvName)>0){
   	  	lt_dbput_recordvars(tablePtr,2,
   	     "twosrvid",LT_TYPE_LONG,iii,
   	     "twosrvname",LT_TYPE_STRING,_ltPubInfo->ltService[iii].srvName);
   	  }else{
   	   break;	
   	  }
   	 
   }
   
   
   //虚拟通道
   tablePtr=lt_dbput_table(dbPtr,"vpathlist");
   for(iii=0;iii<_MAXVPATHNUM;iii++){
   	 if(strlen(_ltPubInfo->_VPathList[iii].vPathName)>0){
   	 	lt_dbput_recordvars(tablePtr,2,
   		 "vpathid",LT_TYPE_LONG,iii,
   		 "vpathname",LT_TYPE_STRING,_ltPubInfo->_VPathList[iii].vPathName);  	 	
   	 }else{
   	   break;	
   	 }
   	 
   }


   ltMsgPk->msgpktype=1;
   if(showtype==1){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipbacksearchlist.htm",dbPtr->head,0);
	 }else if(showtype==2){
	 	//匹配规则
	 	 tablePtr=lt_dbput_table(dbPtr,"matchrulelist");
	   for(iii=0;iii<_MAX_SRVRULE_NUM;iii++){
	   	if(strlen(_ltPubInfo->_Srvrulelist[iii].rulename)>0){
	   		lt_dbput_recordvars(tablePtr,2,
	   		 "matchruleid",LT_TYPE_LONG,iii,
	   	 	 "matchrulename",LT_TYPE_STRING,_ltPubInfo->_Srvrulelist[iii].rulename);
	   	}
	   	 
	   }
	 	
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipblocksearchlist.htm",dbPtr->head,0);
	 }else if(showtype==3){
	 	 tablePtr=lt_dbput_table(dbPtr,"urltypelist"); 	 
	 	 sprintf(sqlBuf,"select sortid,sorname from nasurlsort");
		 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		 if(tempCursor!=NULL){
				   tempRow=ltDbFetchRow(tempCursor);
				   	while(tempRow!=NULL){
                lt_dbput_recordvars(tablePtr,2,
				   		 "urltypeid",LT_TYPE_LONG,atol(tempRow[0]),
				   	 	 "urltypename",LT_TYPE_STRING,tempRow[1]);
								tempRow=ltDbFetchRow(tempCursor);						
					  }
				   ltDbCloseCursor(tempCursor);
		 }
	 	
	 	
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/httpbacksearchlist.htm",dbPtr->head,0);
	 }
	 
	 ltMsgFree(ltMsgPk);
	 
	 if(fnames){//释放内存
 		free(fnames);
   }
   
	 return 0;
   

}



/*
 MSA:数据中心
         详细访问记录
     		 服务访问记录
         服务阻断记录
 功能:列表显示本段时间访问记录调用函数
 */
int msaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char   caFile1[256];
  char   caFile2[256];
  long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  char  caTmpp11[1024];
  int   fd;
  int   totalnum;
  int   nownum;
  int   iCount;
  int   inum;
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  char  fangX[32];
  char  sipadd[32];
  char  mipadd[32];
  char  sport[32];
  char  dport[32];
  char  protocol[32]; 
  int   iReturn;
  char  *caUrl;
	iReturn=-1;
  static char inbuf[4096];
  static stralloc line = {0};
  buffer ss;
  int    match;
  int    kkkk;
  int    jjjj;
  int    iiii;
  
  
  if(ltMsgGetVar_s(ltMsgPk,"url")){
  	caUrl=ltMsgGetVar_s(ltMsgPk,"url");
  }else{
  	caUrl="";
  }
  printf("url:%s\n",caUrl);
	sprintf(caFile1,"%s/%s.csv",_datacenterdir,caUrl);
	printf("caFile1:%s\n",caFile1);
	iReturn=access(caFile1,F_OK);
  if(iReturn!=0){
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 
		 return 0;
  } 

  lRowNum=100; 
  lSumRec=0;
  lCount=0; 
  lStartRec=0;
  lNum=0;
	
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
   
  sprintf(caFile2,"%s/%s.count",_datacenterdir,caUrl);
	iReturn=access(caFile2,F_OK);
  if(iReturn!=0){//不存在file2
     fd=open(caFile1,O_RDONLY | O_NDELAY); 
		 if(fd<1){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 
				 return 0;
		 }
		 totalnum=0;
		 iCount=read(fd,inbuf,8192); 
		 while(iCount){ 
		     for(inum=0;inum<iCount;inum++){   /*统计有读取多少行记录*/
		     		if(inbuf[inum]=='\n'){
		     		   totalnum++; 
		     		}
		     }
		     iCount=read(fd,inbuf,8192);
		 } 
		 close(fd);
  }else{
    FILE  *fp; 
    char ch; 
    char res[12]; 
    char ress[32]; 
    memset(res,0,sizeof(res));
    memset(ress,0,sizeof(ress)); 
  	fp=fopen(caFile2,"r");
  	ch=fgetc(fp); 
		while(ch!=EOF){  
			  if(ch=='\r' || ch=='\n' || ch==' ' ){
			  	break;
			  }
				sprintf(res,"%c",ch); 
				strcat(ress,res); //字符拼接
				ch=fgetc(fp);
		}  
    fclose(fp);    
    totalnum=atol(ress);
  }
  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 
				 return 0;
  }
  nownum=0;

  memset(tempStr,0,sizeof(tempStr));

  
  nownum=0;
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		nownum++;
		if(nownum>=lStartRec){
			striptrailingwhitespace(&line);
			xIndex++;
			iiii=0;
      iIndex=0;
      kkkk=0;
      if(xIndex==1){
        stralloc_cats(&strTemp,"{");  
      }else{
        stralloc_cats(&strTemp,",{");  
      }
     //Userid,用户名,显示名，一级部门，2级部门，3级部门，4级部门，5级部门，传输方向，协议，源ip,自身mac,目的ip,源端口，目的端口，开始连接日期，开始连接时间，上行流量，下行流量、数据包数目、一级服务，2级服务，虚拟通道，关键字  
			for(jjjj=0;jjjj<line.len;jjjj++){
				 if(line.s[jjjj]=='\t'){				  
					 iIndex++;
           if(iIndex==1){
           	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);				          
			     }else if(iIndex==4){//一级部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s__",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==5){//二级部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==6){//三级部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==7){//四级部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==8){//五级部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s\"",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==9){//传输方向
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strcmp(caTmpp11,"0")==0){
			         	 sprintf(fangX,"%s","->");
			         }else{
			         	 sprintf(fangX,"%s","<-");
			         }			 
					 }else if(iIndex==10){//协议
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(protocol,0,sizeof(protocol));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(protocol,"%s",caTmpp11);					 
					 }else if(iIndex==11){//源IP
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sipadd,0,sizeof(sipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sipadd,"%s",caTmpp11);									 
					 }else if(iIndex==13){//目标IP
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(mipadd,0,sizeof(mipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(mipadd,"%s",caTmpp11);							 
					 }else if(iIndex==14){//源端口
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sport,0,sizeof(sport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sport,"%s",caTmpp11);									 
					 }else if(iIndex==15){//UDP:IP:源端口->目的端口
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(dport,0,sizeof(dport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(dport,"%s",caTmpp11);
			         sprintf(tempStr,",\"v%d\":\"%s:%s:%s%s%s:%s\"",iIndex,protocol,sipadd,sport,fangX,mipadd,dport);
			         stralloc_cats(&strTemp,tempStr);										 
					 }else{
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 
					 }	
					 
					 
					           
					 jjjj++;
					 iiii=jjjj;
					 while(line.s[jjjj]=='\t'){
					 	iIndex++;
				    sprintf(tempStr,",\"v%d\":\"\"",iIndex);
				    stralloc_cats(&strTemp,tempStr);
				    jjjj++;
					  iiii=jjjj;
					 }
			   }
			  
		  }
		  
		  
			
		  {  	
		  	iIndex++;
		  	printf("iIndex:%d\n",iIndex);
	      memset(caTmpp11,0,sizeof(caTmpp11));
		    memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);  
		    sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
		    stralloc_cats(&strTemp,tempStr);
		  }	
		
		  stralloc_cats(&strTemp,"}"); 
		 
		  if(nownum==(lRowNum+lStartRec)){
     		break;
      } 
     
    }
    
  }
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp); 
	printf("%s\n",strTemp.s);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	
	
	return 0;
     		
}


/*
MSA:数据中心
    详细访问记录
        服务访问记录
        服务阻断记录
        HTTP访问记录
功能:下载本时段文件
*/
int msaBackListDownLoadCSV(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char *downLoadFile;
    char cafile[512];
    int  fd;

    static char inbuf[4096];
	  static stralloc line = {0};
		buffer ss;
	  int    match;
	  int    jjjj;
  
    if(ltMsgGetVar_s(ltMsgPk,"downLoadFile")){
    	 downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
    }else{
    	 downLoadFile=NULL;
    } 
    
    if(!downLoadFile){
    	downLoadFile="";
    	ltMsgPk->msgpktype=2;
			lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.csv","contenttype","Application/MS-DOWNLOAD");
		  write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
		  
		  return 0;
    }
    
    sprintf(cafile,"%s/%s",_datacenterdir,downLoadFile);
    
    fd=open(cafile,O_RDONLY | O_NDELAY); 
		if(fd<1){
		    ltMsgPk->msgpktype=2;
				lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"nothisfile.csv","contenttype","Application/MS-DOWNLOAD");
			  write(confd,"this file isn't exist!",strlen("this file isn't exist!"));
			  
			  return 0;
		}
		
		ltMsgPk->msgpktype=2;
		lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"download.csv","contenttype","Application/MS-DOWNLOAD");
		

	  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
		for (;;){
			if (getln(&ss,&line,&match,'\n') == -1) break;
			if (!match && !line.len) { break; }
	
			for(jjjj=0;jjjj<line.len;jjjj++){
					 if(line.s[jjjj]=='\t'){
					  line.s[jjjj]=',';
				   }
			}
			write(confd,line.s,line.len);
	  }
	  
	 close(fd);
   ltMsgFree(ltMsgPk);   
    
   return 0;
 
 }
 

/*
MSA:数据中心
				详细访问记录
				    HTTP访问记录
功能:列表显示本时段访问记录调用函数
*/
int HttpmsaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char   caFile1[256];
  char   caFile2[256];
  long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  char  caTmpp11[1024];
  int   fd;
  int   totalnum;
  int   nownum;

  int   iCount;
  int   inum;
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  int   iReturn;
  char  *caUrl;
	
  static char inbuf[4096];
  static stralloc line = {0};
  buffer ss;
  int    match;
  int    kkkk;
  int    jjjj;
  int    iiii;
  
  
  if(ltMsgGetVar_s(ltMsgPk,"url")){
  	caUrl=ltMsgGetVar_s(ltMsgPk,"url");
  }else{
  	caUrl="";
  }
  
	
	sprintf(caFile1,"%s/%s.csv",_datacenterdir,caUrl);
	iReturn=access(caFile1,F_OK);
  if(iReturn!=0){
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 
		 return 0;
  } 

  lRowNum=100; 
  lSumRec=0;
  lCount=0; 
  lStartRec=0;
  lNum=0;
	
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
   
  sprintf(caFile2,"%s/%s.count",_datacenterdir,caUrl);
	iReturn=access(caFile2,F_OK);
  if(iReturn!=0){//不存在file2
     fd=open(caFile1,O_RDONLY | O_NDELAY); 
		 if(fd<1){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 
				 return 0;
		 }
		 totalnum=0;
		 iCount=read(fd,inbuf,8192); 
		 while(iCount){ 
		     for(inum=0;inum<iCount;inum++){                /*统计有读取多少行记录*/
		     		if(inbuf[inum]=='\n'){
		     		   totalnum++; 
		     		}
		     }
		     iCount=read(fd,inbuf,8192);
		 } 
		 close(fd);
  }else{
    FILE  *fp; 
    char ch; 
    char res[12]; 
    char ress[32]; 
    memset(res,0,sizeof(res));
    memset(ress,0,sizeof(ress)); 
  	fp=fopen(caFile2,"r");
  	ch=fgetc(fp); 
		while(ch!=EOF){  
			  if(ch=='\r' || ch=='\n' || ch==' ' ){
			  	break;
			  }
				sprintf(res,"%c",ch); 
				strcat(ress,res); //字符拼接
				ch=fgetc(fp);
		}  
    fclose(fp);    
    totalnum=atol(ress);
  }
  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 
				 return 0;
  }
  nownum=0;

  memset(tempStr,0,sizeof(tempStr));
  
  nownum=0;
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		nownum++;
		if(nownum>=lStartRec){
			striptrailingwhitespace(&line);
			xIndex++;
			iiii=0;
      iIndex=0;
      kkkk=0;
      if(xIndex==1){
        stralloc_cats(&strTemp,"{");  
      }else{
        stralloc_cats(&strTemp,",{");  
      }
      
			for(jjjj=0;jjjj<line.len;jjjj++){
				 if(line.s[jjjj]=='\t'){
					 iIndex++;
           if(iIndex==1){
           	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);				          
			     }else if(iIndex==4){//部门
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s__",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==5){
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==6){
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==7){
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s__",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else if(iIndex==8){
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"%s\"",caTmpp11);
			         stralloc_cats(&strTemp,tempStr);	
					 }else{
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 
					 }			
					 
					 
					           
					 jjjj++;
					 iiii=jjjj;
					 while(line.s[jjjj]=='\t'){
					 	iIndex++;
				    sprintf(tempStr,",\"v%d\":\"\"",iIndex);// \" 转义字符双引号 
				    stralloc_cats(&strTemp,tempStr);
				    jjjj++;
					  iiii=jjjj;
					 }
			  }
			  
		  }
		  
		  
			
		  { 
		  	iIndex++;
	      memset(caTmpp11,0,sizeof(caTmpp11));
		    memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
		    printf("caTmpp11:%s\n",caTmpp11);
		    sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
		    stralloc_cats(&strTemp,tempStr);
		  }	
		
		  stralloc_cats(&strTemp,"}"); 
		 
		  if(nownum==(lRowNum+lStartRec)){
     		break;
      } 
     
    }
    
  }
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp); 
	
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	
	
	return 0;
     		
}



/*
MSA:数据中心
				每天访问汇总记录
						服务汇总记录
功能：服务汇总记录动态链接页面调用函数
*/
int ltshowservicequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
	char *gid;
  int iii;
  
  char strBuf[512];
  int Num=0;
  
  //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	 }

  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
       
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select distinct sdate from msasrvreport");
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录			
			Num++;	
			if(Num==1){
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,tempRow[0]);
			}else{
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,tempRow[0]);
			}    				              
    tempRow=ltDbFetchRow(tempCursor);	
		}		
	}
	ltDbCloseCursor(tempCursor); 
	
	tablePtr=lt_dbput_table(dbPtr,"srvlist");	
	for(iii=0;iii<25;iii++){
			lt_dbput_recordvars(tablePtr,2,
				"srvid",LT_TYPE_LONG,iii,
				"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
	}
	
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msaservicehead.htm",dbPtr->head,0);	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
	return 0;
}


/*
MSA:数据中心
        每天访问汇总记录
        		HTTP汇总记录
功能:HTTP汇总记录动态链接页面调用函数
*/
int ltwebshowwebsitequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int iii;
	iii=0;
	
	char sqlBuf[2048];
	char *gid;
  int Num=0;
   //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	 }
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
   
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	memset(sqlBuf,0,sizeof(sqlBuf));
  Num=0;
  sprintf(sqlBuf,"select distinct sdate from msauserhttpreport");
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录			
			Num++;	
			if(Num==1){
       lt_dbput_recordvars(tablePtr,2,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,tempRow[0]);
			}else{
       lt_dbput_recordvars(tablePtr,2,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,tempRow[0]);
			}    				              
    tempRow=ltDbFetchRow(tempCursor);	
		}		
	}
	ltDbCloseCursor(tempCursor);
	

	  //http服务分类
   tablePtr=lt_dbput_table(dbPtr,"srvlist");//http服务分类1到103
   for(iii=1;iii<104;iii++){
   	  if(strlen(_ltPubInfo->ltService[iii].srvName)>0){
   	  	lt_dbput_recordvars(tablePtr,2,
   	     "srvid",LT_TYPE_LONG,_ltPubInfo->ltService[iii].srvid,
   	     "srvname",LT_TYPE_STRING,_ltPubInfo->ltService[iii].srvName);
   	  }else{
   	   break;	
   	  }
   	 
   }
	
	
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msawebsitehead.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


/*
MSA:数据中心
    每天访问汇总记录
    		服务汇总记录
功能:服务汇总记录条件查询,导出,删除参数 调用函数
*/
int ltservicequeryhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;   
    int  intGid,glevel=0;	 
    char sqlBuf[1024],strBuf[1024];
    char caWhere[512],delWhere[512];   
    char *caUser=NULL; 
    char *caService=NULL;
    char *caDtype=NULL; 
    char *caDdd=NULL;
    char *caUnit=NULL;
    char *caBef=NULL;
    char *caSdate=NULL;
    char *caBtype=NULL;
    char *caBbyte=NULL; 
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *gid=NULL;
    char caTmpp[2048];
    char group_name[256],user_name[256],servic_name[256],date_name[256],flow_name[256];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    stralloc strTemp;
    strTemp.s=0;
    memset(group_name,0,sizeof(group_name));
	  memset(servic_name,0,sizeof(servic_name));
	  memset(date_name,0,sizeof(date_name));
	  memset(flow_name,0,sizeof(flow_name));
	  memset(user_name,0,sizeof(user_name));
    printf("start checkkkkk......................\n");  
   	//数据库连接
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				fprintf(stderr,"db connect error\n");
				return 0;
		 }

    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     iAction=0;
    }else{
      iAction=atol(m_action);
    }
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); 
    caService=ltMsgGetVar_s(ltMsgPk,"service");
    caDtype=ltMsgGetVar_s(ltMsgPk,"dtype");  
    caDdd=ltMsgGetVar_s(ltMsgPk,"ddd");
    caUnit=ltMsgGetVar_s(ltMsgPk,"unit");  // 天。周。月
    caBef=ltMsgGetVar_s(ltMsgPk,"bef");    //以内 以前
    
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    
    caBtype=ltMsgGetVar_s(ltMsgPk,"btype");   //流量范围
    caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");   //字节
    
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");       //排序方式
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
    
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
	  intGid=atol(gid);
	  /*获取部门管理级别*/	  
	  sprintf(strBuf,"select level,name from msagroup where id=%d",intGid);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				sprintf(group_name,"%s",tempRow[1]);
			}
		}
		ltDbCloseCursor(tempCursor); 
			
  
		lRowNum=100; 
		lSumRec=0;
		lCount=0; 
		lStartRec=0;
		lNum=0; 
			  	
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			 lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){
			 lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}
		
	
		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));
	
		 if(strcmp(gid,"-1")){//不等于-1
			   if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
			       sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
			       sprintf(user_name,"%s","Allusers");
				 }else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  为b表
						 sprintf(delWhere,"userid = %s",caUser); 
				 }
		 }
		 
		 if(strcmp(gid,"-1")==0){
			  if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"%s","2=2");
			       sprintf(delWhere,"%s","2=2");
			       sprintf(group_name,"%s","Allgroup");
				}
		 }   
		 
     if(caService!=NULL){
	    if(strlen(caService)>0) {
          if(strcmp(caService,"-1")){
		        	sprintf(caWhere,"%s and (b.service='%s')",caWhere,caService);
		        	sprintf(delWhere,"%s and (service ='%s')",delWhere,caService);
			    }else{
			    	  sprintf(servic_name,"%s","Allservice");
			    }
	    }
     }    
      
    if(strcmp(caDtype,"unit2") == 0){ /*时间*/
    	if((caSdate!=NULL)){  		
	        if(strlen(caSdate)>0) {
	            if(strlen(caSdate)==10) {
	            	sprintf(caWhere,"%s and (b.sdate = '%s')",caWhere,caSdate);
	            	sprintf(delWhere,"%s and (sdate = '%s')",delWhere,caSdate);
	             }            
	        }
			}
    }

    if(strcmp(caBtype,"unit2") == 0)  { /* 流量范围  */
    	  sprintf(caWhere,"%s and (b.dbytes > '%s')",caWhere,caBbyte);
    	  sprintf(delWhere,"%s and (dbytes > '%s')",delWhere,caBbyte);
    	  sprintf(flow_name,"%s",caBbyte);
    }else{
    	  sprintf(flow_name,"%s","All");
    } 
          
    if(iAction==2){/*del*/   	 
				if(strlen(delWhere)>0){
				     sprintf(sqlBuf,"delete from msasrvreport where %s ",delWhere);
				     ltDbExecSql(G_DbCon,sqlBuf);
		    }
		    
			  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				ltMsgFree(ltMsgPk);
				
				return 0;
    	
    }else if(iAction==1){/*export*/	
	    int  lTime;
	    char sDate[36];
	    char caLine[8012];
	    char caFile[256];		
	    memset(sDate,0,sizeof(sDate));
	    memset(caFile,0,sizeof(caFile));
	    lTime = time(0);
			nasTimeGetDate(sDate,lTime);		 			 	//nasTimeLongFormat nasTimeGetDate		
			if(strlen(caWhere)>0){
				  if(caOrder!=NULL){
					    if( strlen(caOrder)>0 ) {
					    	if(strcmp(caOrder,"nosort")!=0) {
							        if(strcmp(caOrder,"date")==0) {
							            sprintf(caWhere,"%s order by b.sdate ",caWhere);
							        }else if(strcmp(caOrder,"userss") == 0) {
							            sprintf(caWhere,"%s order by b.userid ",caWhere);
							        }else if(strcmp(caOrder,"service") == 0) {
							            sprintf(caWhere,"%s order by b.service ",caWhere);
							        }else if(strcmp(caOrder,"bytes") == 0) {
							            sprintf(caWhere,"%s order by b.dbytes ",caWhere);
							        } 
					        
						          if(strlen(caOrderby)) {
						            if(caOrderby[0] == '1') {   
			            	    	 if(strcmp(caOrder,"date")==0){
							            		ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"service")==0){
												   	  ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"dbytes")==0){
												   	  ltStrAddF(caWhere," desc");
												   }else{
												   	  ltStrAddF(caWhere," desc");
												   }
						            }else{
					                 if(strcmp(caOrder,"date")==0) {
									            ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"service")==0){
												   	  ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"dbytes")==0){
												   	  ltStrAddF(caWhere," asc");
												   }else{
												   	  ltStrAddF(caWhere," asc");
												   }          
						            } 
						          }
					         }
					    }   
			     }	
         }
        
		    int fd=0;
		    int iReturn=-1;
		    int flag=0;
		    sprintf(caFile,"%s",SRVCSV_PATH);
		    iReturn=access(caFile,F_OK);
		    if(iReturn==0){
		      sprintf(caFile,"rm -f %s",SRVCSV_PATH);
		      system(caFile);
		    }
				fd = open(caFile,O_WRONLY | O_CREAT, 0644);
				if(fd == (-1)) {
						ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
						ltMsgFree(ltMsgPk);
				
			 		  return 0;
				}
	
	     sprintf(sqlBuf,"select a.dispname,b.sdate,b.ubytes,b.dbytes,b.ctime,b.onlinetime,b.service,a.userid,c.name from msasrvreport b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",glevel,caWhere,lRowNum,lStartRec);
	     tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		   if(tempCursor!=NULL){
			    tempRow= ltDbFetchRow(tempCursor);
		       	while(tempRow!=NULL){
		       		flag++;
		       		char sTime[32];
		       		char ubytes[128];
		       		char dbytes[128];
			        memset(sTime,0,sizeof(sTime));
			        memset(ubytes,0,sizeof(ubytes));
			        memset(dbytes,0,sizeof(dbytes));
			  	    sprintf(sTime,"%ld",atol(tempRow[5])/3600);
					    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
					    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
					    sprintf(ubytes,"%lldKb",atoll(tempRow[2])/1024);
					    sprintf(dbytes,"%lldKb",atoll(tempRow[3])/1024);
							sprintf(caLine,"%d,%s,%s,%s,%s,%s,%s,%ld,%s\r\n",flag,
											tempRow[0],//用户名
											tempRow[8],//部门名称
											tempRow[1],//日期
											_ltPubInfo->topSrvName[atol(tempRow[6])].srvname,
											ubytes,//ubytes
											dbytes,//dbytes
											atol(tempRow[4]),//ctime
											sTime);
							
							if(strcmp(user_name,"Allusers")!=0){//单个用户
								sprintf(user_name,"%s",tempRow[0]);
							}
							
							if(strcmp(servic_name,"Allservice")!=0){
								sprintf(servic_name,"%s",_ltPubInfo->topSrvName[atol(tempRow[6])].srvname);
							}
							
							write(fd,caLine,strlen(caLine));								
							if(flag>=10000){//不超过一万行
							   break;	
							}					
							tempRow=ltDbFetchRow(tempCursor);
						}
		     ltDbCloseCursor(tempCursor);
		   }
		   close(fd);
		   
		   memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"/app/ns/java/jdk1.6.0_01/bin/java -Duser.language=zh -Dfile.encoding=GBK -cp /app/ns/java/poi/poi-3.7-20101029.jar:/app/ns/java/poi/javacsv.jar:./ Toexcel %d %s %s %s %s %s",flag,group_name,user_name,servic_name,sDate,flow_name);
			 chdir("/app/msa/bin");
			 system(caLine);
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
			 ltMsgFree(ltMsgPk);
			    		  				
		   return 0;
    }
    
    
		  sprintf(strBuf,"select count(*) from msasrvreport b left join msauser a on a.userid=b.userid where %s",caWhere);
		  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
			}
			ltDbCloseCursor(tempCursor);
 
     if(caOrder!=NULL){
	    if( strlen(caOrder)>0 ) {
	    	if(strcmp(caOrder,"nosort")!=0) {
			    if(strcmp(caOrder,"date")==0) {
			           sprintf(caWhere,"%s order by b.sdate ",caWhere);
			    }else if(strcmp(caOrder,"service") == 0){
						     sprintf(caWhere,"%s order by b.service ",caWhere);
					}else if(strcmp(caOrder,"bytes") == 0){
			           sprintf(caWhere,"%s order by b.dbytes ",caWhere);
			    }else if(strcmp(caOrder,"userss") == 0){
						     sprintf(caWhere,"%s order by b.userid ",caWhere);
					}
					
	        if(strlen(caOrderby)){
		            if(caOrderby[0] == '1') {   
		            	  if(strcmp(caOrder,"date")==0) {
						            ltStrAddF(caWhere," desc");
									  }else if(strcmp(caOrder,"service")==0){
									   	       ltStrAddF(caWhere," desc");
									  }else if(strcmp(caOrder,"dbytes")==0){
									   	        ltStrAddF(caWhere," desc");
									  }else{
									   	       ltStrAddF(caWhere," desc");
									  }
		            }else{
	                  if(strcmp(caOrder,"date")==0) {
					            ltStrAddF(caWhere," asc");
								    }else if(strcmp(caOrder,"service")==0){
								   	         ltStrAddF(caWhere," asc");
								    }else if(strcmp(caOrder,"dbytes")==0){
								   	         ltStrAddF(caWhere," asc");
								    }else{
								   	        ltStrAddF(caWhere," asc");
								   	}          
		            } 
				 }
	      }
	     }   
    }	
  //json拼接
  sprintf(sqlBuf,"select a.dispname,b.sdate,b.ubytes,b.dbytes,b.ctime,b.onlinetime,b.service,a.userid,c.name from msasrvreport b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",glevel,caWhere,lRowNum,lStartRec);
	sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
	stralloc_cats(&strTemp,caTmpp);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
	     tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
					  lNum++;
						if(lNum==1){
							        char sTime[32];
							        memset(sTime,0,sizeof(sTime));
							  	    sprintf(sTime,"%ld",atol(tempRow[5])/3600);
									    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
									    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
								      sprintf(caTmpp,"{\"mygroup\":\"%s\",\"uname\":\"%s\",\"mydate\":\"%s\",\"sflow\":\"%lld\",\"xflow\":\"%lld\",\"counts\":\"%ld\",\"times\":\"%s\",\"myservice\":\"%s\",\"uid\":\"%ld\"}",
								      				tempRow[8],//GroupName
															tempRow[0],//dispname
															tempRow[1],//sdate
															atoll(tempRow[2])/1024,//ubytes
															atoll(tempRow[3])/1024,//dbytes
															atol(tempRow[4]),//ctime
															sTime,//onlinetime
															_ltPubInfo->topSrvName[atol(tempRow[6])].srvname,//service
															atol(tempRow[7])//userid
									);											  
								 stralloc_cats(&strTemp,caTmpp);								
						}else{
							        char sTime[32];
								      memset(sTime,0,sizeof(sTime));
								  	  sprintf(sTime,"%ld",atol(tempRow[5])/3600);
									    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
									    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
							        sprintf(caTmpp,",{\"mygroup\":\"%s\",\"uname\":\"%s\",\"mydate\":\"%s\",\"sflow\":\"%lld\",\"xflow\":\"%lld\",\"counts\":\"%ld\",\"times\":\"%s\",\"myservice\":\"%s\",\"uid\":\"%ld\"}",
                            tempRow[8],//GroupName
														tempRow[0],//dispname
														tempRow[1],//sdate
														atoll(tempRow[2])/1024,//ubytes
														atoll(tempRow[3])/1024,//dbytes
														atol(tempRow[4]),//ctime
														sTime,//onlinetime
														_ltPubInfo->topSrvName[atol(tempRow[6])].srvname,//service
														atol(tempRow[7])//userid
									);						  
								  stralloc_cats(&strTemp,caTmpp);	
						 }
						tempRow= ltDbFetchRow(tempCursor);
			}
     ltDbCloseCursor(tempCursor);
   }

  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp);
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);

	return 0;

}


/*
MSA:数据中心
				是否生成XLS文件判断
*/
int msaIfCreatXlsOK(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char caFile[64];
	 int iReturn;
	 iReturn=-1;
	 memset(caFile,0,sizeof(caFile));
	 if(strcmp(ltMsgGetVar_s(ltMsgPk,"type"),"0")==0){
	 	 sprintf(caFile,"%s",SRVXLS_PATH);//服务汇总xls
	 }else{
	   sprintf(caFile,"%s",HTTPXLS_PATH);//http汇总xls
	 }
	 
   
	 iReturn=access(caFile,F_OK);
  
   if(iReturn==0){
   	 ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
   }else{
   	 ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
   }
	 ltMsgFree(ltMsgPk);	 
	 return 0;

}




/*
MSA:数据中心
				每天访问汇总记录
						HTTP汇总记录
功能:HTTP汇总条件导出，查询，删除
*/
int ltwebsitequeryhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    
    LT_DBROW tempRow;
    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//删除条件
    char caWhere[512];
    
    char *caDtype=NULL; 
    char *caHost=NULL;
    char *caSdate=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;  
    char *service=NULL;
    char *caBtype=NULL;//流量范围
    char *caBbyte=NULL;//字节
    char *gid=NULL;
    int intGid,glevel=0;	
    char caTmpp[2048];
    char group_name[256],user_name[256],servic_name[256],date_name[256],flow_name[256];
    
    stralloc strTemp;
    strTemp.s=0;
    memset(group_name,0,sizeof(group_name));
	  memset(servic_name,0,sizeof(servic_name));
	  memset(date_name,0,sizeof(date_name));
	  memset(flow_name,0,sizeof(flow_name));
	  memset(user_name,0,sizeof(user_name));
    
    	//数据库连接
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				fprintf(stderr,"db connect error\n");
				return 0;
		 }
    
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
      iAction=0;
    }else{
      iAction=atol(m_action);
    }
    
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); //uid
   
    caHost=ltMsgGetVar_s(ltMsgPk,"host");//host关键字
     
    service=ltMsgGetVar_s(ltMsgPk,"service");//url分类
    
    caDtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    
    caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
    caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
    
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降   
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    intGid=atol(gid);

    /*获取部门管理级别*/	  
		sprintf(strBuf,"select level,name from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				sprintf(group_name,"%s",tempRow[1]);
			}
	  }
		ltDbCloseCursor(tempCursor); 
  
		lRowNum=100; /*每页的行数*/
		lSumRec=0;/*总行数*/
		lCount=0; //
		lStartRec=0;//
		lNum=0; /*该页剩余的行数*/

			
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));}//相当于结束位置
		if(ltMsgGetVar_s(ltMsgPk,"start")){ lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));}//相当于起始位置

		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));
    
		if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
	       sprintf(caWhere,"%s","2=2");//查询所有用户
	       sprintf(delWhere,"%s","2=2");
	       sprintf(group_name,"%s","Allgroup");
			}
		}	
	  if(strcmp(gid,"-1")){//不等于-1
	   	if(strcmp(caUser,"-1")==0){
	        sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
	        sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
	        sprintf(user_name,"%s","Allusers");
			}else{
			    sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  为b表
				  sprintf(delWhere,"userid = %s",caUser); 
			}
	  }     

    if(caHost!=NULL){
	    if(strlen(caHost)>0) {
	        sprintf(caWhere,"%s and (b.host like '%c%s%c') ",caWhere,'%',caHost,'%');
	        sprintf(delWhere,"%s and (host like '%c%s%c') ",delWhere,'%',caHost,'%');
	    }
    }  
    if(service!=NULL){
	    if(strcmp(service,"-1")){
	        sprintf(caWhere,"%s and (b.urlsort = '%s') ",caWhere,service);
	        sprintf(delWhere,"%s and (urlsort ='%s') ",delWhere,service);
	    }else{
			    sprintf(servic_name,"%s","Allservice");
			}
    }  
    if(strcmp(caDtype,"unit2") == 0){ /* 时间范围  */
    	if((caSdate!=NULL)){    		
		        if(strlen(caSdate)>0) {
		            if(strlen(caSdate)==10) {
		            	sprintf(caWhere,"%s and (b.sdate = '%s')",caWhere,caSdate);
		            	sprintf(delWhere,"%s and (sdate = '%s')",delWhere,caSdate);
		             }            
		        }
			}
    } 
    if(strcmp(caBtype,"unit2")==0){ //流量范围
    	  sprintf(caWhere,"%s and (b.bytes >%ld)",caWhere,atol(caBbyte));
    	  sprintf(delWhere,"%s and (bytes >%ld)",delWhere,atol(caBbyte));
    	  sprintf(flow_name,"%s",caBbyte);
    }else{
    	  sprintf(flow_name,"%s","All");
    }   
    if(iAction==2){/*del*/    	 
				if( strlen(delWhere)>0){
				    sprintf(sqlBuf,"delete from msauserhttpreport where %s ",delWhere);
				    ltDbExecSql(G_DbCon,sqlBuf);
		    }
		    
			  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				ltMsgFree(ltMsgPk);
				return 0;
    }else if(iAction==1){/*export*/			    
					int  lTime;
			    char sDate[36];
			    char caLine[8012];
			    char caFile[256];		
			    memset(sDate,0,sizeof(sDate));
			    memset(caFile,0,sizeof(caFile));
			    lTime = time(0);
					nasTimeGetDate(sDate,lTime);	
				  if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    	if(strcmp(caOrder,"nosort")!=0) {
				        if(strcmp(caOrder,"sdate")==0) {//按日期排序
				            sprintf(caWhere,"%s order by b.sdate ",caWhere);
				        }else if(strcmp(caOrder,"userid") == 0) {//按用户id排序
				            sprintf(caWhere,"%s order by b.userid ",caWhere);
				        }else if(strcmp(caOrder,"host") == 0) {//按站点
				            sprintf(caWhere,"%s order by b.host ",caWhere);
				        }else if(strcmp(caOrder,"bytes") == 0) {//按流量
				            sprintf(caWhere,"%s order by b.bytes ",caWhere);
				        }else if(strcmp(caOrder,"lcount") == 0) {//按连接数
				            sprintf(caWhere,"%s order by b.lcount ",caWhere);
				        }else if(strcmp(caOrder,"ctime") == 0) {//按访问时长
				            sprintf(caWhere,"%s order by b.ctime ",caWhere);
				        }
				        if(strlen(caOrderby)) {
						           if(caOrderby[0] == '1') {   
						            	 if(strcmp(caOrder,"sdate")==0) {
										            	 ltStrAddF(caWhere," desc");								            	 
												   }else if(strcmp(caOrder,"userid")==0){
												   	       ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"host")==0){
												   	        ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"bytes")==0){
												   	        ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"lcount")==0){
												   	        ltStrAddF(caWhere," desc");
												   }else if(strcmp(caOrder,"ctime")==0){
												   	        ltStrAddF(caWhere," desc");
												   }
						           }else{
						            	 if(strcmp(caOrder,"sdate")==0) {
										               ltStrAddF(caWhere," asc");	
												   }else if(strcmp(caOrder,"userid")==0){
												   	       ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"host")==0){
												   	        ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"bytes")==0){
												   	        ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"lcount")==0){
												   	        ltStrAddF(caWhere," asc");
												   }else if(strcmp(caOrder,"ctime")==0){
												   	        ltStrAddF(caWhere," asc");
												   }           
						            } 
						    	}     
				       }
				     }   
			    }		
			 
		  int fd=0;
	    int iReturn=-1;
	    int flag=0;
	    sprintf(caFile,"%s",HTTPCSV_PATH);
	    iReturn=access(caFile,F_OK);
	    if(iReturn==0){
	      sprintf(caFile,"rm -f %s",HTTPCSV_PATH);
	      system(caFile);
	    }
			fd = open(caFile,O_WRONLY | O_CREAT, 0644);
			if(fd == (-1)) {
					ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					ltMsgFree(ltMsgPk);
			
		 		  return 0;
			}   
			 
			sprintf(sqlBuf,"select a.dispname,b.host,b.sdate,b.bytes,b.lcount,b.ctime,b.urlsort,c.name from msauserhttpreport b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",glevel,caWhere,lRowNum,lStartRec);	
			printf("sqlBufaaa:%s\n",sqlBuf);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		  if(tempCursor!=NULL){
						tempRow=ltDbFetchRow(tempCursor);
		       	while(tempRow!=NULL){
		       		flag++;
		       		char sTime[32];
		       		char bytes[128];
			        memset(sTime,0,sizeof(sTime));
			        memset(bytes,0,sizeof(bytes));
			  	    sprintf(sTime,"%ld",atol(tempRow[5])/3600);
					    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
					    sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
					    sprintf(bytes,"%lldKb",atoll(tempRow[3])/1024);
					    
							sprintf(caLine,"%d,%s,%s,%s,%s,%s,%ld,%s,%s\r\n",
									flag,//序号
									tempRow[0],//用户名
									tempRow[7],//部门
									tempRow[2],//日期
									tempRow[1],//域名
									bytes,//流量
									atol(tempRow[4]),//访问次数
									sTime,//连接时间
									_ltPubInfo->ltService[atol(tempRow[6])].srvName//http服务分类
							);		
							
							if(strcmp(user_name,"Allusers")!=0){//单个用户
								sprintf(user_name,"%s",tempRow[0]);
							}
							
							if(strcmp(servic_name,"Allservice")!=0){
								sprintf(servic_name,"%s",_ltPubInfo->topSrvName[atol(tempRow[6])].srvname);
							}
								
							write(fd,caLine,strlen(caLine));	
							if(flag>=10000){//不超过一万行
							   break;	
							}										
							tempRow= ltDbFetchRow(tempCursor);
						}
		        ltDbCloseCursor(tempCursor);
		   }		
			 
			 
			 memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"/app/ns/java/jdk1.6.0_01/bin/java -Duser.language=zh -Dfile.encoding=GBK -cp /app/ns/java/poi/poi-3.7-20101029.jar:/app/ns/java/poi/javacsv.jar:./ HttpToexcel %d %s %s %s %s %s",flag,group_name,user_name,servic_name,sDate,flow_name);
			 chdir("/app/msa/bin");
			 system(caLine);
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
			 ltMsgFree(ltMsgPk);
			    		  				
		   return 0;   	   	
		   
    }


    sprintf(strBuf,"select count(*) from msauserhttpreport b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s",glevel,caWhere);  
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
		}
		ltDbCloseCursor(tempCursor);
	
	  if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    			if(strcmp(caOrder,"nosort")!=0) {
								        if(strcmp(caOrder,"sdate")==0) {//按日期排序
								            sprintf(caWhere,"%s order by b.sdate ",caWhere);
								        }else if(strcmp(caOrder,"userid") == 0) {//按用户id排序
								            sprintf(caWhere,"%s order by b.userid ",caWhere);
								        }else if(strcmp(caOrder,"host") == 0) {//按站点
								            sprintf(caWhere,"%s order by b.host ",caWhere);
								        }else if(strcmp(caOrder,"bytes") == 0) {//按流量
								            sprintf(caWhere,"%s order by b.bytes ",caWhere);
								        }else if(strcmp(caOrder,"lcount") == 0) {//按连接数
								            sprintf(caWhere,"%s order by b.lcount ",caWhere);
								        }else if(strcmp(caOrder,"ctime") == 0) {//按访问时长
								            sprintf(caWhere,"%s order by b.ctime ",caWhere);
								        }
				        
				          if(strlen(caOrderby)) {
						            if(caOrderby[0] == '1') {   
						            	   if(strcmp(caOrder,"sdate")==0) {
										            		 ltStrAddF(caWhere," desc");							            		 
													   }else if(strcmp(caOrder,"userid")==0){
													   	       ltStrAddF(caWhere," desc");
													   }else if(strcmp(caOrder,"host")==0){
													   	        ltStrAddF(caWhere," desc");
													   }else if(strcmp(caOrder,"bytes")==0){
													   	        ltStrAddF(caWhere," desc");
													   }else if(strcmp(caOrder,"lcount")==0){
													   	        ltStrAddF(caWhere," desc");
													   }else if(strcmp(caOrder,"ctime")==0){
													   	        ltStrAddF(caWhere," desc");
													   }
						            }else{
				            	       if(strcmp(caOrder,"date")==0) {
								            				  ltStrAddF(caWhere," asc");
													   }else if(strcmp(caOrder,"userid")==0){
													   	        ltStrAddF(caWhere," asc");
													   }else if(strcmp(caOrder,"host")==0){
													   	        ltStrAddF(caWhere," asc");
													   }else if(strcmp(caOrder,"bytes")==0){
													   	        ltStrAddF(caWhere," asc");
													   }else if(strcmp(caOrder,"lcount")==0){
													   	        ltStrAddF(caWhere," asc");
													   }else if(strcmp(caOrder,"ctime")==0){
													   	        ltStrAddF(caWhere," asc");
													   }
						            	           
						            } 
						        }
				    		}
				   	}   
		}	


  //json构造
	sprintf(sqlBuf,"select a.dispname,b.sdate,b.host,b.bytes,b.lcount,b.ctime,b.urlsort,a.userid,c.name from msauserhttpreport b,msauser a,msagroup c where a.userid =b.userid and a.groupid%d=c.id and %s limit %lu offset %lu",glevel,caWhere,lRowNum,lStartRec);
	printf("sqlBuf:%s\n",sqlBuf);
	sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
	stralloc_cats(&strTemp,caTmpp);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	lNum=0;	
   if(tempCursor!=NULL){
	     tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
					  lNum++;
						if(lNum==1){
							    char sTime[32];
							    memset(sTime,0,sizeof(sTime));
							  	sprintf(sTime,"%ld",atol(tempRow[5])/3600);
									sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
									sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
									sprintf(caTmpp,"{\"mygroup\":\"%s\",\"uname\":\"%s\",\"mydate\":\"%s\",\"myurl\":\"%s\",\"flow\":\"%lld\",\"counts\":\"%ld\",\"times\":\"%s\",\"sorts\":\"%s\",\"uid\":\"%ld\"}",
											tempRow[8],//GroupName
											tempRow[0],//a.dispname
											tempRow[1],//b.sdate
											tempRow[2],//b.host
											atoll(tempRow[3])/1024,//b.bytes
											atol(tempRow[4]),//b.lcount
											sTime,//b.ctime
											_ltPubInfo->ltService[atol(tempRow[6])].srvName,//b.urlsort
											atol(tempRow[7])//a.userid
											);
								      stralloc_cats(&strTemp,caTmpp);
								
						}else{
							   char sTime[32];
							    memset(sTime,0,sizeof(sTime));
							  	sprintf(sTime,"%ld",atol(tempRow[5])/3600);
									sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])/60);
									sprintf(sTime,"%s:%ld",sTime,atol(tempRow[5])%60);
								  sprintf(caTmpp,",{\"mygroup\":\"%s\",\"uname\":\"%s\",\"mydate\":\"%s\",\"myurl\":\"%s\",\"flow\":\"%lld\",\"counts\":\"%ld\",\"times\":\"%s\",\"sorts\":\"%s\",\"uid\":\"%ld\"}",
											tempRow[8],//GroupName
											tempRow[0],//uname
											tempRow[1],//mydate
											tempRow[2],//myurl
											atoll(tempRow[3])/1024,//flow
											atol(tempRow[4]),//counts
											sTime,//times
											_ltPubInfo->ltService[atol(tempRow[6])].srvName,
											atol(tempRow[7])//a.userid
											);
								   		stralloc_cats(&strTemp,caTmpp);
		
						 }
						tempRow= ltDbFetchRow(tempCursor);
			}
     ltDbCloseCursor(tempCursor);
   }
	stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp);
   
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	
	return 0;

}


/*
MSA:数据中心
		即时通信记录
				MSN,Yahoo,QQ,淘宝,飞信,skype
功能:页面链接调用函数
*/
int ltimshowwebquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  
  char strBuf[512];
	char *gid;
  char *sFlag;
  char fWhere[128];
  int Num=0;
  
	//数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	 }
	 
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  sFlag=ltMsgGetVar_s(ltMsgPk,"flag"); 
  
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8"); 
	
	if(sFlag){
  	sprintf(fWhere,"tse%s",sFlag);
  	lt_dbput_rootvars(dbPtr,1,fWhere," selected ");
  }
  
  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=%s",sFlag);
  printf("sql:%s\n",strBuf);
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录			
			Num++;	
			char tempDate[32];
			char ToDate[32];		
			memset(tempDate,0,sizeof(tempDate));
			memset(ToDate,0,sizeof(ToDate));
			sprintf(tempDate,"%s",tempRow[0]);
			ToDate[0]=tempDate[0];
			ToDate[1]=tempDate[1];
			ToDate[2]=tempDate[2];
			ToDate[3]=tempDate[3];
			ToDate[4]='-';
			ToDate[5]=tempDate[4];
			ToDate[6]=tempDate[5];
			ToDate[7]='-';
			ToDate[8]=tempDate[6];
			ToDate[9]=tempDate[7];
			ToDate[10]='\0';
			printf("aaa:%s\n",tempRow[0]);
			printf("bbb:%s\n",ToDate);
			if(Num==1){
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,ToDate);
			}else{
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],        
       "strdate",LT_TYPE_STRING,ToDate);
			}    				              
    tempRow=ltDbFetchRow(tempCursor);	
		}		
	}
	ltDbCloseCursor(tempCursor); 
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msaimsearb.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
  return 0;
}


/*
MSA:数据中心
				即时通信记录
				    MSN,Yahoo,淘宝,飞信,qq,skype
功能:记录数据查询 导出 删除
*/
int ltwebqueryima(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    
    char strBuf[1024];
    char delWhere[512];//删除条件
    char caWhere[512];
    
    char *caDtype=NULL; 
    char *caSdate=NULL;
    char *caHost=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;  
    char *flag=NULL;//类型标记
    char *gid=NULL;
    int intGid,glevel=0;	
    char caTmpp[2048];
    char tabname[256];//通信记录表名
    
    stralloc strTemp;
    strTemp.s=0;
    
	    //数据库连接
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				fprintf(stderr,"db connect error\n");
				return 0;
		 }
    
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
      iAction=0;
    }else{
      iAction=atol(m_action);
    }
    
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); //uid
    flag=ltMsgGetVar_s(ltMsgPk,"flag");//url分类
    caHost=ltMsgGetVar_s(ltMsgPk,"host");//host关键字
    caDtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    intGid=atol(gid);
	  
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
			}
		if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
			}
	
		lRowNum=100; /*每页的行数*/
		lSumRec=0;/*总行数*/
		lCount=0; 
		lStartRec=0;
		lNum=0; /*该页剩余的行数*/
					
		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));		
		/*获取部门管理级别*/	  
		sprintf(strBuf,"select level from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
			}
		}
		ltDbCloseCursor(tempCursor);  

		/*判断此天是否有文件传输记录表*/
		memset(strBuf,0,sizeof(strBuf));
		sprintf(strBuf,"select tabname,sdate from msaloglist where sdate='%ld' and tabtype=%s",atol(caSdate),flag);		
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){//当天有记录		    
				    sprintf(tabname,"%s%s",tempRow[0],tempRow[1]);
				}		
			}
		ltDbCloseCursor(tempCursor); 		
		if(strlen(tabname)==0){	  
					  return 0;  
		}
		
    if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"%s","2=2");//查询所有用户
			       sprintf(delWhere,"%s","2=2");//
				}
		}
		
		if(strcmp(gid,"-1")){//不等于-1
			  if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
			       sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
				}else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//查询单个用户
						 sprintf(delWhere,"userid = %s",caUser); 
				}
		}        
		if(flag!=NULL){//传输类型处理
	    if(strcmp(flag,"-1") ){
	        sprintf(caWhere,"%s and (b.flags = '%s') ",caWhere,flag);
	        sprintf(delWhere,"%s and (flags='%s') ",delWhere,flag);
	    }
	  }
    if(caHost!=NULL){//关键字处理
	    if(strlen(caHost)>0) {
	        sprintf(caWhere,"%s and (b.imcon like '%c%s%c') ",caWhere,'%',caHost,'%');
	        sprintf(delWhere,"%s and (imcon like '%c%s%c') ",delWhere,'%',caHost,'%');
	    }
    }
       
    if(iAction==2){/*del*/  	 
				if(strlen(delWhere)>0){
				    sprintf(strBuf,"delete from %s where %s ",tabname,delWhere);
				    ltDbExecSql(G_DbCon,strBuf);
		    }
			  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				ltMsgFree(ltMsgPk);
				return 0;
    }else if(iAction==1){/*export*/		    
			    char caUrl1[200];//
			    int  lTime;
			    char sDate[36];
			    char caLine[8012];			
			    memset(sDate,0,sizeof(sDate));
			    lTime = time(0);
				  nasTimeFormat(sDate,lTime);  
			 
					if(caOrder!=NULL){
					    if(strlen(caOrder)>0){
							    	if(strcmp(caOrder,"nosort")!=0) {
									        if(strcmp(caOrder,"date")==0) {
									            sprintf(caWhere,"%s order by b.sdate ",caWhere);
									        }else if(strcmp(caOrder,"user") == 0) {
									            sprintf(caWhere,"%s order by b.userid ",caWhere);
									        }
									        if(strlen(caOrderby)) {
									            if(caOrderby[0] == '1') {   				            	  
													       ltStrAddF(caWhere," desc");								         
									            }else{
									              ltStrAddF(caWhere," asc");				               
									            } 
									        }
							    }
					   }   
		 			}	
		 
		 
			sprintf(strBuf,"select a.dispname,b.sdate,b.stime,b.imfrom,b.imto,b.imcon,b.flags,c.name from %s b left join msauser a on a.userid=b.userid  left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);

			ltMsgPk->msgpktype=2;
			lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"imexport.csv","contenttype","Application/MS-DOWNLOAD");
			tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
				 tempRow=ltDbFetchRow(tempCursor);
			   while(tempRow!=NULL){
					  if(atol(tempRow[6])==1){
							sprintf(caUrl1,"Msn");
						}else if(atol(tempRow[6])==2){
							sprintf(caUrl1,"Yahoo");
						}else if(atol(tempRow[6])==3){
							sprintf(caUrl1,"QQ");
						}else if(atol(tempRow[6])==4){
							sprintf(caUrl1,"TaoBao");
						}else if(atol(tempRow[6])==5){
							sprintf(caUrl1,"FeiXin");
						}else if(atol(tempRow[6])==6){
							sprintf(caUrl1,"Skype");
						}else {
							sprintf(caUrl1,"???");
						}				
					  sprintf(caLine,"%s,%s,%s,%s,%s,%s,%s,%s\r\n",
					    tempRow[7],//GroupName
							tempRow[0],//dispname
							tempRow[2],//sdate
							tempRow[3],//stime
							tempRow[4],//imfrom
							tempRow[5],//imto
							tempRow[6],//imcon
							caUrl1);
					 write(confd,caLine,strlen(caLine));	
					 tempRow= ltDbFetchRow(tempCursor);
				}
			     ltDbCloseCursor(tempCursor);
			}
		  ltMsgFree(ltMsgPk);
			return 0;
    	
    }
  	sprintf(strBuf,"select count(*) from msaimlog b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s",glevel,caWhere);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
		}
		ltDbCloseCursor(tempCursor);   

    if(caOrder!=NULL){
			    if(strlen(caOrder)>0){
					    	if(strcmp(caOrder,"nosort")!=0) {
								        if(strcmp(caOrder,"date")==0){
								            sprintf(caWhere,"%s order by b.sdate ",caWhere);
								        }else if(strcmp(caOrder,"user") == 0) {
								            sprintf(caWhere,"%s order by b.userid ",caWhere);
								        }
								        if(strlen(caOrderby)) {
								            if(caOrderby[0] == '1'){   				            	  
												       ltStrAddF(caWhere," desc");							         
								            }else{
								              ltStrAddF(caWhere," asc");				               
								            } 
											  }
					        }
			   }   
    }	    	
	 sprintf(strBuf,"select a.dispname,b.sdate,b.stime,b.imfrom,b.imto,b.imcon,b.flags,a.userid,c.name from %s b  left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
        char caUrl[32];     
        lNum++;						
				if(atol(tempRow[6])==1){
					sprintf(caUrl,"Msn");
				}else if(atol(tempRow[6])==2){
					sprintf(caUrl,"Yahoo");
				}else if(atol(tempRow[6])==3){
					sprintf(caUrl,"QQ");
				}else if(atol(tempRow[6])==4){
					sprintf(caUrl,"TaoBao");
				}else if(atol(tempRow[6])==5){
					sprintf(caUrl,"FeiXin");
				}else if(atol(tempRow[6])==6){
					sprintf(caUrl,"Skype");
				}else {
					sprintf(caUrl,"???");
				}	
				
        if(lNum==1){ 
					  sprintf(caTmpp,"{\"mygroup\":\"%s\",\"uid\":\"%s\",\"uname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myimfrom\":\"%s\",\"myimto\":\"%s\",\"myimcon\":\"%s\",\"myflags\":\"%s\"}",
					     tempRow[8],tempRow[7],tempRow[0],tempRow[1], tempRow[2],tempRow[3],tempRow[4],tempRow[5],caUrl);
						   stralloc_cats(&strTemp,caTmpp);
				}else{					    
					  sprintf(caTmpp,",{\"mygroup\":\"%s\",\"uid\":\"%s\",\"uname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myimfrom\":\"%s\",\"myimto\":\"%s\",\"myimcon\":\"%s\",\"myflags\":\"%s\"}",
					     tempRow[8],tempRow[7],tempRow[0],tempRow[1], tempRow[2],tempRow[3],tempRow[4],tempRow[5],caUrl);
						   stralloc_cats(&strTemp,caTmpp);
			  }       
		    tempRow= ltDbFetchRow(tempCursor);
	}
  ltDbCloseCursor(tempCursor);
 }
   
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);


		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);

	return 0;
}



/*
MSA:数据中心
				文件传输记录
功能:文件传输记录各节点链接页面调用函数
*/
int ltmsafiletranquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  
  char strBuf[512];
	char *gid;
  char *sFlag;
  char fWhere[128];
  int Num=0;
  
  //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	 }

  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  sFlag=ltMsgGetVar_s(ltMsgPk,"trantype");
  	
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
  
  if(sFlag){
  	sprintf(fWhere,"tse%s",sFlag);
  	lt_dbput_rootvars(dbPtr,1,fWhere," selected ");
  }
  
  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select tablename,sdate from msafileloglist where tabtype=%s",sFlag);
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录			
			Num++;	
			char tempDate[32];
			char ToDate[32];		
			memset(tempDate,0,sizeof(tempDate));
			memset(ToDate,0,sizeof(ToDate));
			sprintf(tempDate,"%s",tempRow[1]);
			ToDate[0]=tempDate[0];
			ToDate[1]=tempDate[1];
			ToDate[2]=tempDate[2];
			ToDate[3]=tempDate[3];
			ToDate[4]='-';
			ToDate[5]=tempDate[4];
			ToDate[6]=tempDate[5];
			ToDate[7]='-';
			ToDate[8]=tempDate[6];
			ToDate[9]=tempDate[7];
			ToDate[10]='\0';
			if(Num==1){
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[1],        
       "strdate",LT_TYPE_STRING,ToDate);
			}else{
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[1],        
       "strdate",LT_TYPE_STRING,ToDate);
			}    				              
    tempRow=ltDbFetchRow(tempCursor);	
		}		
	}
	ltDbCloseCursor(tempCursor); 

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msafiletransearch.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
	return 0;
}



/*
MSA:数据中心
				文件传输记录
功能:文件传输记录查询，导出，删除
*/
int ltmsafilequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//删除条件
    char caWhere[512];
    char *caSdate=NULL;
    char *caHost=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;  
    char *flag=NULL;
    char *gid=NULL;
    int intGid,glevel=0;	
    char caTmpp[2048];
    char tabname[256];//文件传输记录表名
    
    stralloc strTemp;
    strTemp.s=0;
    
  	 //数据库连接
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				fprintf(stderr,"db connect error\n");
				return 0;
		 }
    
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     iAction=0;
    }else{
      iAction=atol(m_action);
    } 
	  caUser=ltMsgGetVar_s(ltMsgPk,"user"); //uid    
    flag=ltMsgGetVar_s(ltMsgPk,"flag");//类型
    caHost=ltMsgGetVar_s(ltMsgPk,"host");//host关键字
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");// 排序方式
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");//升 降  
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    intGid=atol(gid);  

		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
			}
		if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
			}
	
		lRowNum=100; /*每页的行数*/
		lSumRec=0;/*总行数*/
		lCount=0; 
		lStartRec=0;
		lNum=0; /*该页剩余的行数*/
					
		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));

		/*获取部门管理级别*/	  
		sprintf(strBuf,"select level from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					glevel=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);

		/*判断此天是否有文件传输记录表*/
		memset(strBuf,0,sizeof(strBuf));
		sprintf(strBuf,"select tablename,sdate from msafileloglist where tabtype=%s",flag);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){//当天有记录		    
				    sprintf(tabname,"%s%s",tempRow[0],tempRow[1]);
				}		
			}
		ltDbCloseCursor(tempCursor); 

				if(strlen(tabname)==0){	  
					  return 0;  
				}

        if(strcmp(gid,"-1")==0){
					if(strcmp(caUser,"-1")==0){
					       sprintf(caWhere,"%s","2=2");//查询所有用户
					       sprintf(delWhere,"%s","2=2");//
						}
				}
				 
				if(strcmp(gid,"-1")){//不等于-1
					   if(strcmp(caUser,"-1")==0){
					       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
					       sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
						}else{
							    sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  为b表
								  sprintf(delWhere,"userid =%s",caUser); 
							}
					}
				
				 if(flag!=NULL){//传输类型处理
			    if(strcmp(flag,"-1") ){
			        sprintf(caWhere,"%s and (b.tantype = '%s') ",caWhere,flag);
			        sprintf(delWhere,"%s and (tantype='%s') ",delWhere,flag);
			    }
		     }
		
		     if(caHost!=NULL){//关键字处理
			    if(strlen(caHost)>0) {
			        sprintf(caWhere,"%s and (b.contenttype like '%c%s%c') ",caWhere,'%',caHost,'%');
			        sprintf(delWhere,"%s and (contenttype like '%c%s%c') ",delWhere,'%',caHost,'%');
			    }
		     }
    
    
    
    if(iAction==2){/*del*/    	 
				if( strlen(delWhere)>0){
				    sprintf(sqlBuf,"delete from %s where %s ",tabname,delWhere);
				    ltDbExecSql(G_DbCon,sqlBuf);
		        }
			  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				ltMsgFree(ltMsgPk);
				return 0;
    }else if(iAction==1){/*export*/	
			    char caLine[8012];	
				  if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    	if(strcmp(caOrder,"nosort")!=0){
				        if(strcmp(caOrder,"sdate")==0){//按日期排序
				            sprintf(caWhere,"%s order by b.stime ",caWhere);
				        }else if(strcmp(caOrder,"filename") == 0){//按用户id排序
				            sprintf(caWhere,"%s order by b.filename ",caWhere);
				        }
				        
			          if(strlen(caOrderby)){
				            if(caOrderby[0] == '1') {   
		            	     if(strcmp(caOrder,"date")==0) {
						                   ltStrAddF(caWhere," desc");
										   }else if(strcmp(caOrder,"filename")==0){
										   	       ltStrAddF(caWhere," desc");
										   }
				            }else{
		            	     if(strcmp(caOrder,"date")==0) {
						                   ltStrAddF(caWhere," asc");
										   }else if(strcmp(caOrder,"filename")==0){
										   	       ltStrAddF(caWhere," asc");
										   }         
				            } 
					      }
						        
				      }
			      }   
		      }	
			    
					memset(sqlBuf,0,sizeof(sqlBuf));			    
			    sprintf(sqlBuf,"select a.dispname,b.tantype,b.stime,b.filename,b.fileurl,b.contenttype from %s b left join msauser a on a.userid=b.userid where %s limit %lu offset %lu",tabname,caWhere,lRowNum,lStartRec);		
			
			    ltMsgPk->msgpktype=2;
					lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"filetranexport.csv","contenttype","Application/MS-DOWNLOAD");			
					tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				  if(tempCursor!=NULL){
								tempRow= ltDbFetchRow(tempCursor);
				       	while(tempRow!=NULL){
									sprintf(caLine,"%s,%s,%s,%s,%s,%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5]);			
									write(confd,caLine,strlen(caLine));										
									tempRow= ltDbFetchRow(tempCursor);
								}
				     ltDbCloseCursor(tempCursor);
				  }		
			return 0;    	   	
    }
    
    memset(strBuf,0,sizeof(strBuf));
    sprintf(strBuf,"select count(*) from %s b left join msauser a on a.userid=b.userid where %s",tabname,caWhere);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	  if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
	  }
    ltDbCloseCursor(tempCursor);
    
    //排序
     if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    	if(strcmp(caOrder,"nosort")!=0) {
					        if(strcmp(caOrder,"sdate")==0){//按日期排序
					            sprintf(caWhere,"%s order by b.stime ",caWhere);
					        }else if(strcmp(caOrder,"filename") == 0) {//按用户id排序
					            sprintf(caWhere,"%s order by b.filename ",caWhere);
					        }
				        
				          if(strlen(caOrderby)) {
						            if(caOrderby[0] == '1') {   
						            	     if(strcmp(caOrder,"date")==0) {
										                   ltStrAddF(caWhere,"desc");
														   }else if(strcmp(caOrder,"filename")==0){
														   	       ltStrAddF(caWhere,"desc");
														   }
						            }else{
						            	     if(strcmp(caOrder,"date")==0) {
										                   ltStrAddF(caWhere," asc");
														   }else if(strcmp(caOrder,"filename")==0){
														   	       ltStrAddF(caWhere," asc");
														   }
						            	           
						            } 
						        }    
				    	}
				   	}   
		}	
    
    //json构造
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select a.dispname,b.tantype,b.stime,b.filename,b.fileurl,b.contenttype,a.userid from %s b left join msauser a on a.userid=b.userid where %s limit %lu offset %lu",tabname,caWhere,lRowNum,lStartRec);
		sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
		stralloc_cats(&strTemp,caTmpp);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		lNum=0;
    if(tempCursor!=NULL){
	     tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
					  lNum++;
						if(lNum==1){
									sprintf(caTmpp,"{\"uname\":\"%s\",\"mytantype\":\"%s\",\"mystime\":\"%s\",\"myfilename\":\"%s\",\"myfileurl\":\"%s\",\"mycontenttype\":\"%s\",\"userid\":\"%s\"}",
											tempRow[0],
											tempRow[1],
											tempRow[2],
											tempRow[3],
											tempRow[4],
											tempRow[5],
											tempRow[6]);
								   stralloc_cats(&strTemp,caTmpp);								
						}else{
								sprintf(caTmpp,",{\"uname\":\"%s\",\"mytantype\":\"%s\",\"mystime\":\"%s\",\"myfilename\":\"%s\",\"myfileurl\":\"%s\",\"mycontenttype\":\"%s\",\"userid\":\"%s\"}",
											tempRow[0],
											tempRow[1],
											tempRow[2],
											tempRow[3],
											tempRow[4],
											tempRow[5],
											tempRow[6]);
								   stralloc_cats(&strTemp,caTmpp);		
						 }
						tempRow= ltDbFetchRow(tempCursor);
			}
     ltDbCloseCursor(tempCursor);
   }

	stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp);
   
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	
	return 0;
}



/*
MSA:数据中心
				POP3/SMTP邮件
功能:当日邮件查询 
包括:发送，接收，带附件，大于1M，符合报警策略，符合关键字报警 
条件构造:并将条件打印到页面模板参数
*/
int ltmsadaymailquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    char caWhere[512];
    char myday[32];
    char *caUser=NULL; 
    char *gid=NULL;
    char *caFlag=NULL;
    char *caMtype=NULL; 
    char *caSubject=NULL;
    char *caDip=NULL;
    char *caFrom=NULL;
    char *caTo=NULL;
    char *caBtype=NULL;
    char *caBbyte=NULL;
    char *caFbtype=NULL;
    char *caFbbyte=NULL;
    char *caContent=NULL;
    char *tabDate=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    ltDbHeadPtr dbPtr;

    if(ltMsgGetVar_s(ltMsgPk,"action")){
    	 m_action=ltMsgGetVar_s(ltMsgPk,"action");
    }
   
    
    if(ltMsgGetVar_s(ltMsgPk,"groupid")){//默认为所有部门
    	 gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"user")){//默认为用户
    	 caUser=ltMsgGetVar_s(ltMsgPk,"user");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"flags")){//邮件状态
    	caFlag=ltMsgGetVar_s(ltMsgPk,"flags");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"mtype")){//邮件分类
    	caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");    
    }else{
    	caMtype=NULL;
    }
     
    if(ltMsgGetVar_s(ltMsgPk,"subject")){
    	 caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"dip")){
    	 caDip=ltMsgGetVar_s(ltMsgPk,"dip");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"from")){//发送
    	 caFrom=ltMsgGetVar_s(ltMsgPk,"from");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"to")){//接受
    	 caTo=ltMsgGetVar_s(ltMsgPk,"to"); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabdate")){
    	tabDate=ltMsgGetVar_s(ltMsgPk,"tabdate");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"btype")){
    	caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"bbyte")){
    	caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
    }else{
    	caBbyte=NULL;
    }
                 
    if(ltMsgGetVar_s(ltMsgPk,"fbtype")){
    		caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"fbbyte")){
    	caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
    }else{
    	caFbbyte=NULL;
    }     

    if(ltMsgGetVar_s(ltMsgPk,"content")){
    	caContent=ltMsgGetVar_s(ltMsgPk,"content");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"osort")){
    	 caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
    }    
   
    if(ltMsgGetVar_s(ltMsgPk,"orderby")){
    	 caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
    }
   

		myday[0]=tabDate[0];
		myday[1]=tabDate[1];
		myday[2]=tabDate[2];
		myday[3]=tabDate[3];
		myday[4]=tabDate[5];
		myday[5]=tabDate[6];
		myday[6]=tabDate[8];
		myday[7]=tabDate[9];
		myday[8]='\0';

    
		memset(caWhere,0,sizeof(caWhere));
		dbPtr=lt_dbinit();	
		lt_db_htmlpage(dbPtr,"utf-8"); 

		
		if(strcmp(gid,"-1")==0 && strcmp(caUser,"-1")==0){
			 sprintf(caWhere,"%s","2=2");//所有用户
		}
    

	  if(atoi(caFlag)==-1){
	   		sprintf(caWhere,"%s and (b.flags<2)",caWhere);
	  }else{
	   		sprintf(caWhere,"%s and (b.flags=%d)",caWhere,atoi(caFlag));
	  }



		if(caMtype!=NULL){
			  if(atoi(caMtype)==1){
			  	sprintf(caWhere,"%s and b.mtype=''",caWhere);
			  }
				
		}

    if(caSubject!=NULL){
	    if(strlen(caSubject)>0) {
	        sprintf(caWhere,"%s and b.mailsub like '%c%s%c'",caWhere,'%',caSubject,'%');
	    }
    }
    
      
    if(caDip!=NULL){
	    if(strlen(caDip)>0) {
	        sprintf(caWhere,"%s and b.dip = '%s'",caWhere,caDip);
	    }
    }
      
    if(caFrom!=NULL){
	    if(strlen(caFrom)>0) {
	        sprintf(caWhere,"%s and b.mailfrom='%s'",caWhere,caFrom);
	    }
    }
    
    
    if(caTo!=NULL){
	    if(strlen(caTo)>0) {
	        sprintf(caWhere,"%s and b.mailto='%s'",caWhere,caTo);
	    }
    }


    if(tabDate!=NULL){
	    if(strlen(tabDate)>0) {
	        sprintf(caWhere,"%s and substring(b.sdate,1,10)='%s'",caWhere,tabDate);
	    }
    }


    if(strcmp(caBtype,"unit1") == 0) { /*邮件大小*/
    	  if(atol(caBbyte)==1){
    	  	sprintf(caWhere,"%s and (b.mailsize >1024000)",caWhere);
    	  }	  
    }
        
        
    if(strcmp(caFbtype,"unit2") == 0) { /*附件大小*/
    	  if(strlen(caFbbyte)>0){
    	  	sprintf(caWhere,"%s and (b.mailasize >%s)",caWhere,caFbbyte);
    	  }    	 
    }

	 if(caOrder!=NULL){
	    if( strlen(caOrder)>0 ) {
	    	if(strcmp(caOrder,"nosort")!=0) {
	        if(strcmp(caOrder,"date")==0) {
	            sprintf(caWhere,"%s order by b.sdate ",caWhere);
	        }else if(strcmp(caOrder,"from") == 0) {
	            sprintf(caWhere,"%s order by b.mailfrom ",caWhere);
	        }else if(strcmp(caOrder,"to") == 0) {
	            sprintf(caWhere,"%s order by b.mailto ",caWhere);
	        }else if(strcmp(caOrder,"dip") == 0) {
				      sprintf(caWhere,"%s order by b.dip ",caWhere);
				  }else if(strcmp(caOrder,"size") == 0) {
	            sprintf(caWhere,"%s order by b.mailsize ",caWhere);
	        }else if(strcmp(caOrder,"user") == 0) {
	            sprintf(caWhere,"%s order by b.userid ",caWhere);
	        }
	        if(strlen(caOrderby)) {
	            if(caOrderby[0] == '1') {   		            	  
	                sprintf(caWhere,"%s desc",caWhere);
	            }
				  }
	    }
	   }   
    }			    

  //将where条件打印到页面模板参数
  char caWhereU[512];
  int  lWhere;
  memset(caWhereU,0,sizeof(caWhereU));
	ltEncode64(caWhere, strlen(caWhere), caWhereU, 512, &lWhere);
	for(lWhere=0;lWhere<strlen(caWhereU);lWhere++){
                if(caWhereU[lWhere]=='+'){
                        caWhereU[lWhere]='*';
                }
                if(caWhereU[lWhere]=='/'){
                        caWhereU[lWhere]='#';
                }
  }

lt_dbput_rootvars(dbPtr,1,"where",caWhereU);
lt_dbput_rootvars(dbPtr,1,"tabname",myday);			
lt_dbput_rootvars(dbPtr,1,"iAction",m_action);	
lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/Daymail.htm",dbPtr->head,0);
ltMsgFree(ltMsgPk);
lt_dbfree(dbPtr);


return 0;
   

}



/*
MSA:数据中心
				POP3/SMTP邮件
功能:当日邮件grid数据显示json构造函数
*/
int ltmsadaymaillist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{  
    char sqlBuf[1024],strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caWhere[512];
    char *caSear=NULL;
    char caTmpp[2048];  
    char *tabname=NULL;
    char sqltablename[64];
    stralloc strTemp;
    strTemp.s=0;
    long lCount=0;
    int lNum=0;
    int tabtype=0;
    
    memset(strBuf,0,sizeof(strBuf));
    if(ltMsgGetVar_s(ltMsgPk,"tabname")){
    	 tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
    	 tabtype=atoi(ltMsgGetVar_s(ltMsgPk,"tabtype"));
    }
  
    
    if(ltMsgGetVar_s(ltMsgPk,"where")){
    	caSear=ltMsgGetVar_s(ltMsgPk,"where");
    }
    
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
		    
    sprintf(strBuf,"select sdate,tabname,tabtype from msaloglist where tabtype=%d and sdate='%s'",tabtype,tabname);      
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			    sprintf(sqltablename,"%s%s",tempRow[1],tempRow[0]);
			  }
		}
		
		ltDbCloseCursor(tempCursor);	
    
      
    if(caSear==NULL){    	
    	  ltMsgPk->msgpktype=3;  
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-Length: 0\nContent-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"");		
				ltMsgFree(ltMsgPk);			
				return 0;
				
	   
    }else if(strlen(caSear)==0){    	
    	  ltMsgPk->msgpktype=3;  
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-Length: 0\nContent-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"");		
				ltMsgFree(ltMsgPk);		
				return 0;     
    }else{
        int lWhere;
        memset(caWhere,0,sizeof(caWhere));      
	      for(lWhere=0;lWhere<strlen(caSear);lWhere++){
                if(caSear[lWhere]=='*'){
                        caSear[lWhere]='+';
                }
                if(caSear[lWhere]=='#'){
                        caSear[lWhere]='/';
                }
        }

        ltDecode64(caSear, strlen(caSear), caWhere, &lWhere);
    }

	  sprintf(strBuf,"select count(*) from %s  b left join msauser a on a.userid=b.userid where %s",sqltablename,caWhere);      
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			  tempRow= ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   lCount=atol(tempRow[0]);
			  }
		}
		
		ltDbCloseCursor(tempCursor);	 
		

   //json生成
	 sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,a.username from %s  b left join msauser a on (a.userid =b.userid) where %s limit %d offset %d",sqltablename,caWhere,100,0);//limit:返回lRowNum行  offset 说明在开始返回行之前忽略多少行
    	
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
		   tempRow=ltDbFetchRow(tempCursor);
		   	while(tempRow!=NULL){
		   		  lNum++;		  
		   		  
		   		  //M转换
		   		  char mailaSize[32];//附件
		   		  char mailSize[32];//邮件
		   		  memset(mailaSize,0,sizeof(mailaSize));
		   		  memset(mailSize,0,sizeof(mailSize));
		   		  if(atol(tempRow[6])>1024000){
               sprintf(mailaSize,"%0.3fM",atol(tempRow[6])/1024000.00);
            }else{
               sprintf(mailaSize,"%0.3fK",atol(tempRow[6])/1024.00);
            }
            
		   		  if(atol(tempRow[7])>1024000){
               sprintf(mailSize,"%0.3fM",atol(tempRow[7])/1024000.00);
            }else{
               sprintf(mailSize,"%0.3fK",atol(tempRow[7])/1024.00);
            }
		   		  
		   		  
		   		  
						if(lNum==1){
							sprintf(caTmpp,"{\"uname\":\"%s\",\"uid\":\"%s\",\"udispname\":\"%s\",\"mymailfile\":\"%s\",\"mymailfrom\":\"%s\",\"mymailto\":\"%s\",\"mymailsub\":\"%s\",\"mysdate\":\"%s\",\"mymailasize\":\"%s\",\"mymailsize\":\"%s\",\"myflags\":\"%s\",\"mymtype\":\"%s\",\"myip\":\"%s\"}",tempRow[0],tempRow[11],tempRow[12],tempRow[1],tempRow[2],tempRow[3],tempRow[4],
							tempRow[5],mailaSize,mailSize,tempRow[8],tempRow[9],tempRow[10]);
						  stralloc_cats(&strTemp,caTmpp);					
						}else{
							sprintf(caTmpp,",{\"uname\":\"%s\",\"uid\":\"%s\",\"udispname\":\"%s\",\"mymailfile\":\"%s\",\"mymailfrom\":\"%s\",\"mymailto\":\"%s\",\"mymailsub\":\"%s\",\"mysdate\":\"%s\",\"mymailasize\":\"%s\",\"mymailsize\":\"%s\",\"myflags\":\"%s\",\"mymtype\":\"%s\",\"myip\":\"%s\"}",tempRow[0],tempRow[11],tempRow[12],tempRow[1],tempRow[2],tempRow[3],tempRow[4],
							tempRow[5],mailaSize,mailSize,tempRow[8],tempRow[9],tempRow[10]);
							stralloc_cats(&strTemp,caTmpp);
						}
					
						tempRow= ltDbFetchRow(tempCursor);						
			}
			
		  ltDbCloseCursor(tempCursor);
		}
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);

		
		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);		
		
		return 0; 
    
    
 
}



/*
MSA:数据中心
				POP3,SMTP邮件
功能:邮件全文检索				
*/
int ltmsamail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  
  char strBuf[512];
  char *mailtype;
  int Num=0;
  
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
  
  if(ltMsgGetVar_s(ltMsgPk,"mailtype")){
  	mailtype=ltMsgGetVar_s(ltMsgPk,"mailtype");
  }else{
  	mailtype="";
  }
    
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8"); 
   
  if(strcmp(mailtype,"6") == 0 ){//邮件内容全查列表
  	
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/mailexp.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
  	
  }
  
  if(strcmp(mailtype,"4") == 0 || strcmp(mailtype,"5") == 0){//全部邮件检索	
  
  //分类
  memset(strBuf,0,sizeof(strBuf));
  sprintf(strBuf,"select rid,rname from msamailfilter");
	tablePtr=lt_dbput_table(dbPtr,"mtypelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
        tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){//当天有记录			
					Num++;			
					if(Num==1){
           lt_dbput_recordvars(tablePtr,3,
           "rid",LT_TYPE_STRING,tempRow[0],        
           "rname",LT_TYPE_STRING,tempRow[1]);
					}else{
           lt_dbput_recordvars(tablePtr,3,
           "rid",LT_TYPE_STRING,tempRow[0],        
           "rname",LT_TYPE_STRING,tempRow[1]);
					}    				              
        tempRow=ltDbFetchRow(tempCursor);	
				}		
			}
	ltDbCloseCursor(tempCursor); 
  
  
  	
  	
  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=%d",atoi(mailtype));
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
        tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){//当天有记录			
					Num++;	
					char tempDate[32];
					char ToDate[32];		
					memset(tempDate,0,sizeof(tempDate));
					memset(ToDate,0,sizeof(ToDate));
					sprintf(tempDate,"%s",tempRow[0]);
					ToDate[0]=tempDate[0];
					ToDate[1]=tempDate[1];
					ToDate[2]=tempDate[2];
					ToDate[3]=tempDate[3];
					ToDate[4]='-';
					ToDate[5]=tempDate[4];
					ToDate[6]=tempDate[5];
					ToDate[7]='-';
					ToDate[8]=tempDate[6];
					ToDate[9]=tempDate[7];
					ToDate[10]='\0';
					if(Num==1){
           lt_dbput_recordvars(tablePtr,3,
           "dayname",LT_TYPE_STRING,tempRow[0],        
           "strdate",LT_TYPE_STRING,ToDate);
					}else{
           lt_dbput_recordvars(tablePtr,3,
           "dayname",LT_TYPE_STRING,tempRow[0],        
           "strdate",LT_TYPE_STRING,ToDate);
					}    				              
        tempRow=ltDbFetchRow(tempCursor);	
				}		
			}
	 ltDbCloseCursor(tempCursor); 
	
	lt_dbput_rootvars(dbPtr,1,"tabtype",mailtype);	
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/allmailsearch.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
  }   
  
  return 0;

}


/*
MSA:数据中心
功能:用户combox的json构造 
*/
int msalocalStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  ltDbCursor *tempCursor;
    LT_DBROW tempRow;
	  stralloc strTemp;
    strTemp.s=0;	
	  char *gid;
    char caTmpp[2048],sqlBuf[1024],strBuf[1024];
    int intGid,glevel=0,lNum=0;
    long lCount=0;
    
    char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
		}
    
		memset(caTmpp,0,sizeof(caTmpp));
		memset(sqlBuf,0,sizeof(sqlBuf));
		memset(strBuf,0,sizeof(strBuf));

    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    intGid=atol(gid);

	  /*获取部门管理级别*/	  
	  sprintf(strBuf,"select level from msagroup where id=%d",intGid);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
			}
		}
	  ltDbCloseCursor(tempCursor);   

  /*计算记录数目*/
  memset(sqlBuf,0,sizeof(sqlBuf));
  sprintf(sqlBuf,"select count(*) from msauser a left join msagroup b on (a.groupid%d=b.id) where a.groupid%d=%d",glevel,glevel,intGid);
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			lCount=atol(tempRow[0]);
			lCount++;
		}
	}
	ltDbCloseCursor(tempCursor);
 

	/*选择部门后，生成用户json数据*/
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select a.userid,a.dispname from msauser a left join msagroup b on (a.groupid%d=b.id) where a.groupid%d=%d",glevel,glevel,intGid);
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
		    tempRow= ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){  
	        lNum++;						
	             if(lNum==1){              	
	             	  sprintf(caTmpp,"{\"uid\":\"%s\",\"uname\":\"%s\"}","-1","All USER");
						      stralloc_cats(&strTemp,caTmpp); 
						      
						      sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\"}",tempRow[0],tempRow[1]);				       
							    stralloc_cats(&strTemp,caTmpp);
					}else{					    
						      sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\"}",tempRow[0],tempRow[1]);
							    stralloc_cats(&strTemp,caTmpp);
	
				  }       
			    tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
   }
   
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
		

		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);

	  return 0;		

}

