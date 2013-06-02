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

unsigned long msaCvtLongTime(char *caSdate,char *caTime)
{
    struct tm *tm;
    char caTemp[16],*p;
    unsigned long lTime;   
    lTime = time(0);
    tm =localtime(&lTime);
    p = ltStrGetWord(caSdate,caTemp,6,"/");
    if(*p == '/' && !ltStrIsSpaces(caTemp)) {
        tm->tm_year = atol(caTemp) - 1900;
        p = ltStrGetWord(p+1,caTemp,6,"/");
        if(*p == '/' && !ltStrIsSpaces(caTemp)) {
            tm->tm_mon = atol(caTemp) - 1;
            p++;
            if(!ltStrIsSpaces(p)) {
                tm->tm_mday = atol(p);
            }
        }
    }else{    
        p = ltStrGetWord(caSdate,caTemp,4,"-");
                       if(*p == '-' && !ltStrIsSpaces(caTemp)) {
                           tm->tm_year = 2000+atol(caTemp) - 1900;
                           p = ltStrGetWord(p+1,caTemp,6,"-");
                           if(*p == '-' && !ltStrIsSpaces(caTemp)) {
                               tm->tm_mon = atol(caTemp) - 1;
                               p++;
                               if(!ltStrIsSpaces(p)) {
                                   tm->tm_mday = atol(p);
                               }
                           }
                       }
       }   
    if(!ltStrIsSpaces(caTime)) {
        p = ltStrGetWord(caTime,caTemp,6,":");
        if(*p == ':' && !ltStrIsSpaces(caTemp)) {
            tm->tm_hour = atol(caTemp);
            p = ltStrGetWord(p+1,caTemp,16,":");
            if(*p == ':' && !ltStrIsSpaces(caTemp)) {
                tm->tm_min = atol(caTemp);
                p++;
                if(!ltStrIsSpaces(p)) {
                    tm->tm_sec = atol(p);
                }
            }
        }
    }
    lTime = mktime(tm);
    return lTime;
}

int msaCvtStime(unsigned long lTime,char *caDate,char *caTime)
{
    struct tm *tm1;
    tm1 = localtime(&lTime);
    sprintf(caDate,"%04d/%02d/%02d",tm1->tm_year + 1900,tm1->tm_mon+1,tm1->tm_mday);
    sprintf(caTime,"%02d:%02d:%02d",tm1->tm_hour,tm1->tm_min,tm1->tm_sec);
    return 0;
}

/**
 *function:record page show view
 **/
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
   			printf("%s\n",fnames[len].caDirName);
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
 *function:(服务访问记录、服务阻断记录、http访问记录)搜索 点击节点定向连接到页面函数
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
		 printf("sqlBuf=%s\n",sqlBuf);
		 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		 if(tempCursor!=NULL){
				   tempRow= ltDbFetchRow(tempCursor);
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
 *function:服务访问记录节点：列表显示本段时间访问记录
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
  //char  linebuf[2048];
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  char  fangX[32];
  char  sipadd[32];
  char  mipadd[32];
  char  sport[32];
  char  dport[32];
  //char  *pppp;
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
  
  printf("url:%s\n",caUrl);
	
	sprintf(caFile1,"%s/%s.csv",_datacenterdir,caUrl);
	printf("caFile1:%s\n",caFile1);
	iReturn=access(caFile1,F_OK);
	printf("iReturn1:%d\n",iReturn);
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
	printf("iReturn2:%d\n",iReturn);
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
 // memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  //pppp=linebuf;
  
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
					 }else if(iIndex==9){//传输方向
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strcmp(caTmpp11,"0")==0){
			         	sprintf(fangX,"%s","->");
			         }else{
			         	sprintf(fangX,"%s","<-");
			         }			 
					 }else if(iIndex==10){//源ip
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sipadd,0,sizeof(sipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sipadd,"%s",caTmpp11);					 
					 }else if(iIndex==11){//mac
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);								 
					 }else if(iIndex==12){//目ip
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(mipadd,0,sizeof(mipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(mipadd,"%s",caTmpp11);							 
					 }else if(iIndex==13){//源端口
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sport,0,sizeof(sport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sport,"%s",caTmpp11);							 
					 }else if(iIndex==14){//目端口
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(dport,0,sizeof(dport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(dport,"%s",caTmpp11);
			         sprintf(tempStr,",\"v%d\":\"%s:%s%s%s:%s\"",iIndex,sipadd,sport,fangX,mipadd,dport);
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
	
	printf("testjson:%s\n",strTemp.s);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	
	
	return 0;
     		
}




int HttpmsaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
  //char  linebuf[2048];
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  //char  *pppp;
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
  
  printf("url:%s\n",caUrl);
	
	sprintf(caFile1,"%s/%s.csv",_datacenterdir,caUrl);
	printf("caFile1:%s\n",caFile1);
	iReturn=access(caFile1,F_OK);
	printf("iReturn1:%d\n",iReturn);
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
	printf("iReturn2:%d\n",iReturn);
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
 // memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  //pppp=linebuf;
  
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
	
	printf("testjson:%s\n",strTemp.s);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	
	
	return 0;
     		
}




/*服务访问记录节点：下载本时段文件*/
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
    printf("%s\n",downLoadFile);
    
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
 	
 	
 	
/*function:按条件查询并保存数据|直接导出本段时间所搜数据*/
int msaipbacksearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	char  caCmd[256];
	char  utf8Cmd[256];
	char   *seldate;
	char   *seltime;
	char   *searchconX;
	char   caFile1[256];
	char   caFile2[256];
	int    iReturn;
	char   cpOut[256];
	int    fd;
	int    nownum;
	int    iIndex=0;
	int    maxv;
	char   **vv;//*(*vv) 指向指针的指针
	int    srvid;
	int    twosrvid;
	int    vpath;
	int    matchrule;
	long    urltype;
	char   caTempDir[256];
	char   *searchname;
	int    gid;
	char   *manageruser;
	int    fdwrite=0;
	char   sDate[32];
	int    kkkk;
	int    jjjj;
	int    iiii;
	static char inbuf[4096];
	static stralloc line = {0};
	buffer ss;
	int    match;
	char   *exportaction;
	int    showtype;
	int    glevel;
	char sqlBuf[1024];
	ltDbCursor *tempCursor;
  LT_DBROW tempRow;
	
	memset(sqlBuf,0,sizeof(sqlBuf));
	memset(sDate,0,sizeof(sDate));
	if(ltMsgGetVar_s(ltMsgPk,"showtype")){
		showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	}else{
		showtype=0;
	}
	
	if(ltMsgGetVar_s(ltMsgPk,"manageruser")){
		manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	}else{
		manageruser="";
	}
	printf("manageruser:%s\n",manageruser);
	
	nasTimeFormat(sDate,time(0));
	maxv=32;
	vv=(char **)malloc(maxv*sizeof(char *));
	for(iIndex=0;iIndex<maxv;iIndex++){
		vv[iIndex]=NULL;
	}

	iIndex=0;
	memset(cpOut,0,256);


	if(ltMsgGetVar_s(ltMsgPk,"seldate")){
		seldate=ltMsgGetVar_s(ltMsgPk,"seldate");
	}else{
		seldate="";
	}
	
	if(ltMsgGetVar_s(ltMsgPk,"seltime")){
		seltime=ltMsgGetVar_s(ltMsgPk,"seltime");
	}else{
		seltime="";
	}
	
	
	if(ltMsgGetVar_s(ltMsgPk,"gid")){
		  	gid=atoi(ltMsgGetVar_s(ltMsgPk,"gid"));//部门id;
	}	
		
	if(ltMsgGetVar_s(ltMsgPk,"glevel")){
		  	glevel=atoi(ltMsgGetVar_s(ltMsgPk,"glevel"));//部门id;
		  	if(glevel==0 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
		  	     vv[4]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else if(glevel==1 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
		  		   vv[5]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else if(glevel==2 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
		  	     vv[6]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else if(glevel==3 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
		  		   vv[7]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else{
		  		   vv[8]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}
	}
	
	if(showtype==1){
	
		
		if(ltMsgGetVar_s(ltMsgPk,"sip")){
			vv[10]=ltMsgGetVar_s(ltMsgPk,"sip");//源IP 
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dip")){
			vv[12]=ltMsgGetVar_s(ltMsgPk,"dip");//目标IP
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dport")){
			vv[14]=ltMsgGetVar_s(ltMsgPk,"dport");//目的端口
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"selsrv")){
			srvid=atoi(ltMsgGetVar_s(ltMsgPk,"selsrv"));//一级服务 
			if(srvid>-1 && srvid<MAX_TOP_SRVNUM){
		  	vv[20]=_ltPubInfo->topSrvName[srvid].srvname;
		  }
		}
		

		if(ltMsgGetVar_s(ltMsgPk,"twoselsrv")){
		  twosrvid=atoi(ltMsgGetVar_s(ltMsgPk,"twoselsrv"));//二级服务 
			if(twosrvid>-1 && twosrvid<_ltPubInfo->lServiceNum){
		  	vv[21]=_ltPubInfo->ltService[twosrvid].srvName;
		  }
	  }
	  
	  if(ltMsgGetVar_s(ltMsgPk,"vpath")){
			vpath=atoi(ltMsgGetVar_s(ltMsgPk,"vpath"));//虚拟通道
      if(vpath>-1 && vpath<_MAXVPATHNUM){
      	 vv[22]=_ltPubInfo->_VPathList[vpath].vPathName;
      }
		}

		
		
	}else if(showtype==2){
//		if(ltMsgGetVar_s(ltMsgPk,"gid")){
//		  	gid=atoi(ltMsgGetVar_s(ltMsgPk,"gid"));//部门id;
//		}	
//		
//		if(ltMsgGetVar_s(ltMsgPk,"glevel")){
//		  	glevel=atoi(ltMsgGetVar_s(ltMsgPk,"glevel"));//部门id;
//		  	if(glevel==0 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
//		  	     vv[4]=ltMsgGetVar_s(ltMsgPk,"gname");
//		  	}else if(glevel==1 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
//		  		   vv[5]=ltMsgGetVar_s(ltMsgPk,"gname");
//		  	}else if(glevel==2 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
//		  	     vv[6]=ltMsgGetVar_s(ltMsgPk,"gname");
//		  	}else if(glevel==3 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){
//		  		   vv[7]=ltMsgGetVar_s(ltMsgPk,"gname");
//		  	}else{
//		  		   vv[8]=ltMsgGetVar_s(ltMsgPk,"gname");
//		  	}
//		}
		
		if(ltMsgGetVar_s(ltMsgPk,"sip")){
			vv[10]=ltMsgGetVar_s(ltMsgPk,"sip");//源IP 
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dip")){
			vv[12]=ltMsgGetVar_s(ltMsgPk,"dip");//目标IP
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dport")){
			vv[14]=ltMsgGetVar_s(ltMsgPk,"dport");//目的端口
		}
		
			if(ltMsgGetVar_s(ltMsgPk,"selsrv")){
			srvid=atoi(ltMsgGetVar_s(ltMsgPk,"selsrv"));//一级服务 
			if(srvid>-1 && srvid<MAX_TOP_SRVNUM){
		  	vv[18]=_ltPubInfo->topSrvName[srvid].srvname;
		  }
		}
		

		if(ltMsgGetVar_s(ltMsgPk,"twoselsrv")){
		  twosrvid=atoi(ltMsgGetVar_s(ltMsgPk,"twoselsrv"));//二级服务 
			if(twosrvid>-1 && twosrvid<_ltPubInfo->lServiceNum){
		  	vv[19]=_ltPubInfo->ltService[twosrvid].srvName;
		  }
	  }
	  
	  if(ltMsgGetVar_s(ltMsgPk,"matchrule")){
			matchrule=atoi(ltMsgGetVar_s(ltMsgPk,"matchrule"));//匹配规则
      if(matchrule>-1 && matchrule<_MAX_SRVRULE_NUM){
      	 vv[20]=_ltPubInfo->_Srvrulelist[matchrule].rulename;
      }
		}
	  
	  if(ltMsgGetVar_s(ltMsgPk,"vpath")){
			vpath=atoi(ltMsgGetVar_s(ltMsgPk,"vpath"));//虚拟通道
      if(vpath>-1 && vpath<_MAXVPATHNUM){
      	 vv[21]=_ltPubInfo->_VPathList[vpath].vPathName;
      }
		}
		
		
	}else if(showtype==3){

		if(ltMsgGetVar_s(ltMsgPk,"sip")){
			vv[9]=ltMsgGetVar_s(ltMsgPk,"sip");//源IP 
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dip")){
			vv[10]=ltMsgGetVar_s(ltMsgPk,"dip");//目标IP
		}
		
		
			//url分类
		if(ltMsgGetVar_s(ltMsgPk,"urltype")){
				urltype=atol(ltMsgGetVar_s(ltMsgPk,"urltype"));
				sprintf(sqlBuf,"select sorname from nasurlsort where sortid=%ld",urltype);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						 tempRow= ltDbFetchRow(tempCursor);
						 vv[18]=tempRow[0];
				}
				ltDbCloseCursor(tempCursor);
		}
		
		//url地址
		if(ltMsgGetVar_s(ltMsgPk,"host")){
			vv[19]=ltMsgGetVar_s(ltMsgPk,"host");
		}
		
	
		
		
		
	}
	 
	 //查询条件
	 if(ltMsgGetVar_s(ltMsgPk,"searchconX")){
	 	 searchconX=ltMsgGetVar_s(ltMsgPk,"searchconX");
	 }else{
	 	 searchconX=" ";
	 }
	 
	 printf("searchconX:%s\n",searchconX);
	 
	 //是否导出
	 if(ltMsgGetVar_s(ltMsgPk,"exportaction")){
	 	 exportaction=ltMsgGetVar_s(ltMsgPk,"exportaction");
	 }else{
	 	 exportaction=NULL;
	 }
	  
	  //搜索保存名字
	  if(ltMsgGetVar_s(ltMsgPk,"searchname")){
	  	searchname=ltMsgGetVar_s(ltMsgPk,"searchname");
	  }else{
	  	searchname=" ";
	  }
	  

    if(exportaction==NULL){//不导出
    	 
  	    if(showtype==1){
		  	   sprintf(caTempDir,"/datacenter/msa/ipsearch/");
		    }else if(showtype==2){
		  	   sprintf(caTempDir,"/datacenter/msa/ipblocksearch/");
		    }else if(showtype==3){
		  	   sprintf(caTempDir,"/datacenter/msa/httpsearch/");
		    }
		
				if(ltFileIsExist(caTempDir)!=1) {
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						return 0;
					}
				}
		
				if(showtype==1){
					sprintf(caTempDir,"%s/ipsearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
				}else if(showtype==2){
					sprintf(caTempDir,"%s/ipblocksearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
				}else if(showtype==3){
					sprintf(caTempDir,"%s/httpsearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
				}
		
				if(ltFileIsExist(caTempDir)!=1){
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						return 0;
					}
				}
		
		    sprintf(caFile2,"%s/info.ini",caTempDir);
				fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
				if(fdwrite == (-1)) {
					 ltMsgPk->msgpktype=1;
					 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					 ltMsgFree(ltMsgPk);
					 return 0;
				}
				
				sprintf(caCmd,"sname=%s\n",searchname);
				write(fdwrite,caCmd,strlen(caCmd));
				sprintf(caCmd,"sdate=%s\n",sDate);
				write(fdwrite,caCmd,strlen(caCmd));
				sprintf(caCmd,"suser=%s\n",manageruser);
				write(fdwrite,caCmd,strlen(caCmd));
				sprintf(caCmd,"stype=%s\n","1");
				write(fdwrite,caCmd,strlen(caCmd));	
				sprintf(caCmd,"%s","searchcon=");
				write(fdwrite,caCmd,strlen(caCmd));
				sprintf(caCmd,"%s_%s",seldate,seltime);
				write(fdwrite,caCmd,strlen(caCmd));
		
		    if(showtype==1){
		    	char tmpgname[32];
		    	sprintf(tmpgname,"%s","|部门名:");
		    	strgb2utf8(tmpgname,utf8Cmd,strlen(tmpgname));
		    	
		    	if(vv[4]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[4]);
		    	}else if(vv[5]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[5]);
		    	}else if(vv[6]!=NULL){
		    	  sprintf(caCmd,"%s%s",utf8Cmd,vv[6]);
		    	}else if(vv[7]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[7]);
		    	}else{
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[8]);
		    	}															
					write(fdwrite,caCmd,strlen(caCmd));
					
					sprintf(caCmd,"|源IP:%s",vv[10]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
			
					sprintf(caCmd,"|目标IP:%s",vv[12]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
					
					sprintf(caCmd,"|目的端口:%s",vv[14]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
				  
				  char oneservice[32];
				  sprintf(oneservice,"%s","|一级服务:");
				  strgb2utf8(oneservice,utf8Cmd,strlen(oneservice));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[20]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char twoservice[32];
					sprintf(twoservice,"%s","|二级服务:");
				  strgb2utf8(twoservice,utf8Cmd,strlen(twoservice));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[21]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
					char tmpvpath[32];
					sprintf(tmpvpath,"%s","|虚拟通道:");
					strgb2utf8(tmpvpath,utf8Cmd,strlen(tmpvpath));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[22]);
					write(fdwrite,caCmd,strlen(caCmd));
									
					
				}else if(showtype==2){
					char tmpgname[32];
		    	sprintf(tmpgname,"%s","|部门名:");
		    	strgb2utf8(tmpgname,utf8Cmd,strlen(tmpgname));
		    	
		    	if(vv[4]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[4]);
		    	}else if(vv[5]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[5]);
		    	}else if(vv[6]!=NULL){
		    	  sprintf(caCmd,"%s%s",utf8Cmd,vv[6]);
		    	}else if(vv[7]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[7]);
		    	}else{
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[8]);
		    	}															
					write(fdwrite,caCmd,strlen(caCmd));
					
					
					sprintf(caCmd,"|源IP:%s",vv[10]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
			
					sprintf(caCmd,"|目标IP:%s",vv[12]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
					
					sprintf(caCmd,"|目的端口:%s",vv[14]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
				  
				   char oneservice[32];
				  sprintf(oneservice,"%s","|一级服务:");
				  strgb2utf8(oneservice,utf8Cmd,strlen(oneservice));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[18]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char twoservice[32];
					sprintf(twoservice,"%s","|二级服务:");
				  strgb2utf8(twoservice,utf8Cmd,strlen(twoservice));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[19]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char matchrule[32];
					sprintf(matchrule,"%s","|匹配规则:");
				  strgb2utf8(matchrule,utf8Cmd,strlen(matchrule));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[20]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
					char tmpvpath[32];
					sprintf(tmpvpath,"%s","|虚拟通道:");
					strgb2utf8(tmpvpath,utf8Cmd,strlen(tmpvpath));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[21]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
				}else if(showtype==3){
					char tmpgname[32];
		    	sprintf(tmpgname,"%s","|部门名:");
		    	strgb2utf8(tmpgname,utf8Cmd,strlen(tmpgname));
		    	
		    	if(vv[4]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[4]);
		    	}else if(vv[5]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[5]);
		    	}else if(vv[6]!=NULL){
		    	  sprintf(caCmd,"%s%s",utf8Cmd,vv[6]);
		    	}else if(vv[7]!=NULL){
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[7]);
		    	}else{
		    		sprintf(caCmd,"%s%s",utf8Cmd,vv[8]);
		    	}															
					write(fdwrite,caCmd,strlen(caCmd));
					
					sprintf(caCmd,"|源IP:%s",vv[9]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
			
					sprintf(caCmd,"|目标IP:%s",vv[10]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
				  
          //url分类
          char URLtype[32];
					sprintf(URLtype,"%s","|URL分类:");
				  strgb2utf8(URLtype,utf8Cmd,strlen(URLtype));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[18]);
					write(fdwrite,caCmd,strlen(caCmd));
          
          //url地址
          char URLadd[32];
					sprintf(URLadd,"%s","|URL地址:");
				  strgb2utf8(URLadd,utf8Cmd,strlen(URLadd));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[19]);
					write(fdwrite,caCmd,strlen(caCmd));
          
          
				  
				}
					
				if(searchconX){
						if(searchconX[0] ){
							char keyword[32];
							sprintf(keyword,"%s","|关键字:");
						  strgb2utf8(keyword,utf8Cmd,strlen(keyword));
							sprintf(caCmd,"%s%s",utf8Cmd,searchconX);
							write(fdwrite,caCmd,strlen(caCmd));
						}
				}
				
		  close(fdwrite);
  
    }else{//导出
  	     ltMsgPk->msgpktype=2;
	       lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"search.csv","contenttype","Application/MS-DOWNLOAD");
    }
    
	  if(showtype==1){
	 		 sprintf(caFile1,"%s/%s/iplog/iplog-%s.csv",_datacenterdir,seldate,seltime);
	 	}else if(showtype==2){
	 		 sprintf(caFile1,"%s/%s/ipblock/ipblocklog-%s.csv",_datacenterdir,seldate,seltime);
		}else if(showtype==3){	  	
	 		 sprintf(caFile1,"%s/%s/httplog/httplog-%s.csv",_datacenterdir,seldate,seltime);
		}
	
    iReturn = access(caFile1,F_OK);
    if(iReturn!=0){
		  	if(exportaction==NULL){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				}else{
				 write(confd,"open file error",strlen("open file error"));
				}
				
				ltMsgFree(ltMsgPk);
				return 0;
    }
   
    fd=open(caFile1,O_RDONLY | O_NDELAY);
    printf("myfd:%d\n",fd);
    if(fd<1){
        if(exportaction==NULL){
            ltMsgPk->msgpktype=1;
		   			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
				}else{
				 	write(confd,"open file error",strlen("open file error"));
				}
		    ltMsgFree(ltMsgPk);
		    
		    return 0;
    }
    
   if(exportaction==NULL){
       //保存数据执行
		  sprintf(caFile2,"echo '1'>%s/proccess.ini",caTempDir);//显示当前目录下proccess.ini文件
		  system(caFile2);
		  sprintf(caFile2,"%ssearch.csv",caTempDir);
		  fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
		  printf("myfdwrite:%d\n",fdwrite);
		  if(fdwrite == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 close(fd);
				 
				 return 0;
			}
	  }
	  
	nownum=0;
	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;){
			if(nownum>=100000){printf("copy end...\n");break;}
			if (getln(&ss,&line,&match,'\n') == -1){printf("copy end...\n");break;}
			if (!match && !line.len) {printf("copy end ...\n"); break; }		
			if(searchconX){
					if(searchconX[0]){
					   if(!memmem(line.s,line.len,searchconX,strlen(searchconX))){
					     	continue;
					   }
					}	
			} 
					
	    iiii=0;
	    iIndex=0;
	    kkkk=0;
			for(jjjj=0;jjjj<line.len;jjjj++){
				if(line.s[jjjj]=='\t'){
					if(exportaction){						 
					 line.s[jjjj]=',';
					}
					 iIndex++;						 
					 if(vv[iIndex]){         
					 	 if(vv[iIndex][0]){ 
					 	 	 if(!memmem(&line.s[iiii],jjjj-iiii,vv[iIndex],strlen(vv[iIndex]))){
						 	 	 	kkkk=1;//没有匹配退出标记
						 	 	 	break;
					 	 	 }
					 	 }	
					 }       
					 jjjj++;
					 iiii=jjjj;
			  }
			}
			
			
		  nownum++; 	  
		  
		  if(exportaction==NULL){
		  	  write(fdwrite,line.s,line.len);
		  }else{
		      write(confd,line.s,line.len);
		  }
		  
			if(kkkk==1){
				continue; 	
			} 
		  
	    
  }
    
	if(exportaction==NULL){
    close(fd);
    close(fdwrite);
    sprintf(caFile2,"echo '%d'>%ssearch.count",nownum,caTempDir);
    system(caFile2);
    sprintf(caFile2,"rm -f %sproccess.ini",caTempDir);
    system(caFile2);
    
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");		
	}	
	
	ltMsgFree(ltMsgPk);
	
	
	printf("end.........................................\n");
	
  return 0;
  
}




/*记录搜索，数据载入，以及gird表格记录删除功能*/
int msaNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	stralloc strTemp;
  char caTmpp[2048];
  int  iii;
  int  allnum;
  char caFile1[256];

  char *caDelFile;
  int  fileproc;
  struct stat st;
  int len=0; //sum of file names 
  msasDir *fnames;
  char   *manageruser;
  int    jjj;
  char   sname[256];
  char   sDate[256];
  char   suser[256];
  char   stype[256];
  char   searchcon[256];
 
  int    showtype;
	
	showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));	
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
  if(showtype==1){
  	sprintf(caFile1,"%s","/datacenter/msa/ipsearch/");
  }else if(showtype==2){
  	sprintf(caFile1,"%s","/datacenter/msa/ipblocksearch/");
  }else if(showtype==3){
  	sprintf(caFile1,"%s","/datacenter/msa/httpsearch/");
  }
	if(caDelFile){ 
	       char caCmd[256];
	       if(strlen(caDelFile)>1){	           
	           if(showtype==1){
						  	sprintf(caCmd,"rm -Rf /datacenter/msa/ipsearch/%s",caDelFile);
						 }else if(showtype==2){
						  	sprintf(caCmd,"rm -Rf /datacenter/msa/ipblocksearch/%s",caDelFile);
						 }else if(showtype==3){
						  	sprintf(caCmd,"rm -Rf /datacenter/msa/httpsearch/%s",caDelFile);
						 }
	           printf("caCmd:%s\n",caCmd);
	           system(caCmd);
	       }
	       sleep(1);
	}

 fnames=mydirlist(caFile1,&len);
 if(fnames){
        qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
 }

 allnum=0;
 
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 stralloc_cats(&strTemp,caTmpp);

 jjj=0;
 while(len--) //free all allocated memory blocks
 {               
              iii=strlen(manageruser);
						 	if(strncmp(manageruser,fnames[len].caDirName,iii)){
						 		continue;
						  }
              jjj++;
              memset(sname,0,sizeof(sname));
              memset(sDate,0,sizeof(sDate));
              memset(suser,0,sizeof(suser));
              memset(stype,0,sizeof(stype));
              memset(searchcon,0,sizeof(searchcon));
							fileproc=0;
							if(showtype==1){
						  	sprintf(caFile1,"/datacenter/msa/ipsearch/%s/proccess.ini",fnames[len].caDirName);
						  }else if(showtype==2){
						  	sprintf(caFile1,"/datacenter/msa/ipblocksearch/%s/proccess.ini",fnames[len].caDirName);
						  }else if(showtype==3){
						  	sprintf(caFile1,"/datacenter/msa/httpsearch/%s/proccess.ini",fnames[len].caDirName);
						  }
              if(stat(caFile1,&st) == 0){ 
                    fileproc=1; 
              }
              if(showtype==1){
						  	sprintf(caFile1,"/datacenter/msa/ipsearch/%s/info.ini",fnames[len].caDirName);
						  }else if(showtype==2){
						  	sprintf(caFile1,"/datacenter/msa/ipblocksearch/%s/info.ini",fnames[len].caDirName);
						  }else if(showtype==3){
						  	sprintf(caFile1,"/datacenter/msa/httpsearch/%s/info.ini",fnames[len].caDirName);
						  }
              
              bcCnfGetValue_s(caFile1,"sname",sname);
              bcCnfGetValue_s(caFile1,"sdate",sDate);
              bcCnfGetValue_s(caFile1,"suser",suser);
              bcCnfGetValue_s(caFile1,"stype",stype);
              bcCnfGetValue_s(caFile1,"searchcon",searchcon);
              memset(caTmpp,0,sizeof(caTmpp));
              
							if(jjj==1){
								sprintf(caTmpp,"{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
									sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"The search was generating or when you make a mistake ":"Search success "); 
				        stralloc_cats(&strTemp,caTmpp);			
              }else{
                sprintf(caTmpp,",{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
									sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"The search was generating or when you make a mistake ":"Search success ");  
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





/*POP3，SMTP邮件节点点击链接函数*/
int ltmsamail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  
  char strBuf[512];
  char *mailtype;
  int Num=0;
  
  if(ltMsgGetVar_s(ltMsgPk,"mailtype")){
  	mailtype=ltMsgGetVar_s(ltMsgPk,"mailtype");
  }else{
  	mailtype="";
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
	printf("strBuf::%s\n",strBuf);
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
	printf("strBuf::%s\n",strBuf);
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
说明：当日邮件查询 包括：发送，接收，带附件，大于1M，符合报警策略，符合关键字报警 
      条件构造：并将条件打印到页面模板参数
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
    	 printf("gid=%s\n",gid);
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"user")){//默认为用户
    	 caUser=ltMsgGetVar_s(ltMsgPk,"user");
       printf("caUser=%s\n",caUser);  
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"flags")){//邮件状态
    	caFlag=ltMsgGetVar_s(ltMsgPk,"flags");
      printf("caFlag=%s\n",caFlag); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"mtype")){//邮件分类
    	caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");    
    }else{
    	caMtype=NULL;
    }
     
    if(ltMsgGetVar_s(ltMsgPk,"subject")){
    	 caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
       printf("caSubject=%s\n",caSubject);
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"dip")){
    	 caDip=ltMsgGetVar_s(ltMsgPk,"dip");
       printf("caDip=%s\n",caDip);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"from")){//发送
    	 caFrom=ltMsgGetVar_s(ltMsgPk,"from");
       printf("caFrom=%s\n",caFrom);
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"to")){//接受
    	 caTo=ltMsgGetVar_s(ltMsgPk,"to");
       printf("caTo=%s\n",caTo);    
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabdate")){
    	tabDate=ltMsgGetVar_s(ltMsgPk,"tabdate");
      printf("tabDate=%s\n",tabDate); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"btype")){
    	caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
      printf("caBtype=%s\n",caBtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"bbyte")){
    	caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
      printf("caBbyte=%s\n",caBbyte);
    }else{
    	caBbyte=NULL;
    }
                 
    if(ltMsgGetVar_s(ltMsgPk,"fbtype")){
    		caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
        printf("caFbtype=%s\n",caFbtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"fbbyte")){
    	caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
		  printf("caFbbyte=%s\n",caFbbyte);  
    }else{
    	caFbbyte=NULL;
    }     

    if(ltMsgGetVar_s(ltMsgPk,"content")){
    	caContent=ltMsgGetVar_s(ltMsgPk,"content");
      printf("caContent=%s\n",caContent); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"osort")){
    	 caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
       printf("caOrder=%s\n",caOrder);
    }    
   
    if(ltMsgGetVar_s(ltMsgPk,"orderby")){
    	 caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
       printf("caOrderby=%s\n",caOrderby);   
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

    printf("myday=%s\n",myday);   
    
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

  printf("Where:%s\n",caWhere);
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
说明：当日邮件grid数据显示json构造函数，页面采用json数据存储器
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
			 printf("tabname::%s\n",tabname);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
    	 tabtype=atoi(ltMsgGetVar_s(ltMsgPk,"tabtype"));
			 printf("tabtype::%d\n",tabtype);
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
		printf("strBuf:%s\n",strBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			    sprintf(sqltablename,"%s%s",tempRow[1],tempRow[0]);
          printf("sqltablename::%s\n",sqltablename);
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
    printf("strBuf:%s\n",strBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			  tempRow= ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   lCount=atol(tempRow[0]);
			  }
		}
		
		ltDbCloseCursor(tempCursor);	 
		
     printf("lCount=%ld\n",lCount);

   //json生成
	 sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,a.username from %s  b left join msauser a on (a.userid =b.userid) where %s limit %d offset %d",sqltablename,caWhere,100,0);//limit:返回lRowNum行  offset 说明在开始返回行之前忽略多少行
   printf("sqlBuf=%s\n",sqlBuf);
    	
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
		
		printf("json=%s\n",strTemp.s);
		
		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);		
		
		return 0; 
    
    
 
}





/*
说明：全部邮件记录查询,记录删除,记录导出HTML
采用iAction参数判断是执行查询，还是删除，还是记录导出HTML
修改日期：11-1-26
修改人：刘彬*/
int ltmsamailquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
	  ltDbHeadPtr dbPtr;
	  ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char caWhere[512];
    char *caUser=NULL; 
    char *gid=NULL;
    char *glevel=NULL;
    char *caFlag=NULL;
    char *caMtype=NULL;  
    char *caSubject=NULL;
    char *caDip=NULL;
    char *caFrom=NULL;
    char *caTo=NULL;
    char *tabname=NULL; 
    char *caBtype=NULL;
    char *caBbyte=NULL;
    char *caFbtype=NULL;
    char *caFbbyte=NULL;
    char *caContent=NULL;
    char *caMaxtime=NULL;
    char *caAttchk=NULL; 
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *tabtype=NULL;
    char caTmpp[2048];
    char sqltablename[64];
    stralloc strTemp;
    strTemp.s=0; 
      
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
    
    dbPtr=lt_dbinit();   	 
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     iAction=0;
    }else{
      iAction=atol(m_action);
    }
    printf("iAction=%d\n",iAction);    
    
    if(ltMsgGetVar_s(ltMsgPk,"groupid")){
    	gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    	printf("gid=%s\n",gid);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"glevel")){
    	glevel=ltMsgGetVar_s(ltMsgPk,"glevel");
      printf("glevel=%s\n",glevel);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"user")){
    	 caUser=ltMsgGetVar_s(ltMsgPk,"user");
       printf("caUser=%s\n",caUser);  
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"flags")){
    	caFlag=ltMsgGetVar_s(ltMsgPk,"flags");
      printf("caFlag=%s\n",caFlag); 
    }

    if(ltMsgGetVar_s(ltMsgPk,"mtype")){
    	 caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");
       printf("caMtype=%s\n",caMtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"subject")){
    	 caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
       printf("caSubject=%s\n",caSubject);
    }

    if(ltMsgGetVar_s(ltMsgPk,"dip")){
    	 caDip=ltMsgGetVar_s(ltMsgPk,"dip");
       printf("caDip=%s\n",caDip);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"from")){
    	 caFrom=ltMsgGetVar_s(ltMsgPk,"from");
       printf("caFrom=%s\n",caFrom);
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"to")){
    	 caTo=ltMsgGetVar_s(ltMsgPk,"to");
       printf("caTo=%s\n",caTo);  
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabname")){
    	tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
      printf("tabname=%s\n",tabname);
    }else{
    	tabname="";
    }
     
    if(ltMsgGetVar_s(ltMsgPk,"btype")){
    	caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
      printf("caBtype=%s\n",caBtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"bbyte")){
    	caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
      printf("caBbyte=%s\n",caBbyte);  
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"fbtype")){
    	caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
      printf("caFbtype=%s\n",caFbtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"fbbyte")){
    	 caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
       printf("caFbbyte=%s\n",caFbbyte);
    }
       
    if(ltMsgGetVar_s(ltMsgPk,"content")){
    	caContent=ltMsgGetVar_s(ltMsgPk,"content");
      printf("caContent=%s\n",caContent); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"maxtime")){//查询时长
    	caMaxtime=ltMsgGetVar_s(ltMsgPk,"maxtime");
      printf("caMaxtime=%s\n",caMaxtime); 
    }else{
    	caMaxtime="";
    }
    

    if(ltMsgGetVar_s(ltMsgPk,"attchk")){//搜索附件内容
    	caAttchk=ltMsgGetVar_s(ltMsgPk,"attchk");
      printf("caAttchk=%s\n",caAttchk);
    }
              
    if(ltMsgGetVar_s(ltMsgPk,"osort")){
    	caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
      printf("caOrder=%s\n",caOrder);
    }          
    
    if(ltMsgGetVar_s(ltMsgPk,"orderby")){
    	caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
      printf("caOrderby=%s\n",caOrderby);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
    	tabtype=ltMsgGetVar_s(ltMsgPk,"tabtype");   //升 降
      printf("tabtype=%s\n",tabtype);
    }
        

		  
		lRowNum=100; /*每页的行数*/
		lSumRec=0;/*总行数*/
		lCount=0;
		lStartRec=0;
	
		
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}

		memset(caWhere,0,sizeof(caWhere));

		if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			   sprintf(caWhere,"%s","2=2");//查找整个部门用户
			}
		}

		if(strcmp(gid,"-1")!=0){//不等于-1
			if(strcmp(caUser,"-1")==0){
			    sprintf(caWhere,"a.groupid%s=%s",glevel,gid);//查找整个部门用户
			}else{
					sprintf(caWhere,"b.userid=%s and a.groupid%s=%s",caUser,glevel,gid);//msasearchkey
		  }
		}     
			

   /*邮件状态*/
   if(strcmp(caFlag,"-1")==0){/*全部邮件*/
   		sprintf(caWhere,"%s and (b.flags<2) ",caWhere);
   }else{
   		sprintf(caWhere,"%s and (b.flags=%s)",caWhere,caFlag);
   }
   

   /*邮件分类*/
	 if(strcmp(caMtype,"-1")!=0){
	 	 sprintf(strBuf,"select rname from msamailfilter where rid=%s",caMtype);
	 	 printf("strBuf:%s\n",strBuf);
		 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		 if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   sprintf(caWhere,"%s and (b.mtype='%s') ",caWhere,tempRow[0]);
			  }
		 }
		 ltDbCloseCursor(tempCursor);	 
	 }
  
   if(caSubject!=NULL){
	    if(strlen(caSubject)>0) {
	        sprintf(caWhere,"%s and (b.mailsub like '%c%s%c')",caWhere,'%',caSubject,'%');
	    }
   }
    
   if(caDip!=NULL){
	    if(strlen(caDip)>0) {
	        sprintf(caWhere,"%s and (b.dip = '%s')",caWhere,caDip);
	    }
   }
    
   if(caFrom!=NULL){
	    if(strlen(caFrom)>0) {
	        sprintf(caWhere,"%s and (b.mailfrom='%s')",caWhere,caFrom);
	    }
   }
    
   if(caTo!=NULL){
	    if(strlen(caTo)>0) {
	        sprintf(caWhere,"%s and (b.mailto='%s')",caWhere,caTo);
	    }
   }


   if(strcmp(caBtype,"unit2") == 0) { /* 邮件大小*/
    	   sprintf(caWhere,"%s and (b.mailsize >%s)",caWhere,caBbyte);
   }
    
   if(strcmp(caFbtype,"unit2") == 0) { /* 附件大小*/
    	  sprintf(caWhere,"%s and (b.mailasize >%s)",caWhere,caFbbyte);
   }
   
   
   if(strlen(tabtype)>0){
   	    memset(sqltablename,0,sizeof(sqltablename));
   	    sprintf(strBuf,"select sdate,tabname,tabtype from msaloglist where tabtype=%s and sdate='%s'",tabtype,tabname);      
				printf("strBuf:%s\n",strBuf);
				tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
				if(tempCursor!=NULL){
					  tempRow=ltDbFetchRow(tempCursor);
					  if(tempRow!=NULL){
					    sprintf(sqltablename,"%s%s",tempRow[1],tempRow[0]);
		          printf("sqltablename::%s\n",sqltablename);
					  }
				}
				
				ltDbCloseCursor(tempCursor);	
   }
   
   
	 sprintf(strBuf,"select count(*) from %s b left join msauser a on a.userid=b.userid where %s",sqltablename,caWhere);      
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	 if(tempCursor!=NULL){
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL){
		   lCount=atol(tempRow[0]);
		  }
	 }
	 ltDbCloseCursor(tempCursor);	 

	 if(caOrder!=NULL){
		    if( strlen(caOrder)>0 ){
				    	if(strcmp(caOrder,"nosort")!=0){
						        if(strcmp(caOrder,"date")==0){
						            sprintf(caWhere,"%s order by b.sdate ",caWhere);
						        }else if(strcmp(caOrder,"from") == 0){
						            sprintf(caWhere,"%s order by b.mailfrom ",caWhere);
						        }else if(strcmp(caOrder,"to") == 0){
						            sprintf(caWhere,"%s order by b.mailto ",caWhere);
						        }else if(strcmp(caOrder,"dip") == 0){
												sprintf(caWhere,"%s order by b.dip ",caWhere);
							    	}else if(strcmp(caOrder,"size") == 0){
						            sprintf(caWhere,"%s order by b.mailsize ",caWhere);
						        }else if(strcmp(caOrder,"user") == 0){
						            sprintf(caWhere,"%s order by b.userid ",caWhere);
						        }
						        
						        if(strlen(caOrderby)){
							          if(caOrderby[0] == '1'){   		            	  
							              sprintf(caWhere,"%s desc",caWhere);
							          }else{
							          	  sprintf(caWhere,"%s asc",caWhere);
							          }
									  }
				    }
		   }   
   }	
  
    
    /*记录删除*/
		if(iAction==1){
			sprintf(sqlBuf,"delete from %s where %s ",sqltablename,caWhere);
      printf("sqlBuf:%s\n",sqlBuf);
		  ltDbExecSql(G_DbCon,sqlBuf);  
		
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
			ltMsgFree(ltMsgPk);
			
			return 0;
			
		}

    
		/*记录导出HTML*/
		if(iAction==2){
			      char xHead[1024];
			      char *lt_page_content=NULL;
			      char *ppp;
				    int  iii;
				    dbPtr=lt_dbinit();   	
		    					    
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
				        
				    sprintf(xHead,"%s","Content-disposition: attachment; filename=\"mailexport.htm\"\nContent-type: application/octet-stream\nContent-transfer-encoding:BINARY\n\n");		    
			      lt_dbput_rootvars(dbPtr,4,"tabname",tabname,"where",caWhereU,"iAction",tabtype,"ipadd","192.168.0.60");//ip地址为暂时为固定ip	      
				    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/datacenter/mailrecordexport.htm",dbPtr->doc,0);
				    
				    if(lt_page_content==NULL){
				     	return -1;
				    }
				    iii=0;
				    ppp=lt_page_content;
				    for(iii=0;iii<30;iii++){
				    	if(*ppp!='<'){
				    		*ppp=' ';
				    	}else{
				    		break;
				      }
				    }
			
				    ppp=strrchr(lt_page_content,'>');
			      if(ppp){
			   		  *(ppp+1)='\0';
			      }

				  ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,xHead,"lthtml",LT_TYPE_STRING,lt_page_content);
				  free(lt_page_content);		    
			    ltMsgFree(ltMsgPk);
			    lt_dbfree(dbPtr);
							
				  return 0; 
				
			      
			  }

printf("Where:%s\n",caWhere);
//json构造
	lNum=0;
	sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
  stralloc_cats(&strTemp,caTmpp);  
  sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,a.username from %s b left join msauser a on (a.userid =b.userid) where %s limit %ld offset %ld",sqltablename,caWhere,lRowNum,lStartRec);//limit:返回lRowNum行  offset 说明在开始返回行之前忽略多少行
  printf("sqlBuf=%s\n",sqlBuf);
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





/*内容查询*/
int ltmsamailcontentquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    char sqlBuf[1024];
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caWhere[512];
    char *caUser=NULL; 
    char *gid=NULL;
    char *glevel=NULL;
    char *gname=NULL;
    char *caFlag=NULL;
    char *caMtype=NULL;   
    char *caSubject=NULL;
    char *caDip=NULL;
    char *caFrom=NULL;
    char *caTo=NULL;
    char *tabname=NULL;  
    char *caBtype=NULL;
    char *caBbyte=NULL;
    char *caFbtype=NULL;
    char *caFbbyte=NULL;    
    char *caContent=NULL;
    char *attchk=NULL;   
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *caMaxtime=NULL;
    char *tabtype=NULL;
    char sqltablename[64];
    char searchcon[2048];
	  char utf8Cmd[256];
    int  iNum;
    char sDate[64],sTime[64];
    char caTempDir[128];
    char *caMsgFile;
    char caFile1[256];
    char caFile2[256];
	  char caCmd[2048];
    struct stat st;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;	 
    int  iReturn;
    int  pid;
    int  fdwrite=0;
    int  maxtime=1200;    
    char caTmpp[2048];
    struct dirent *d;
    FILE *fp;
	  DIR *dir;	   
	  
	  memset(searchcon,0,sizeof(searchcon));
	  
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
	  
	  if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
    	tabtype=ltMsgGetVar_s(ltMsgPk,"tabtype"); 
      printf("tabtype=%s\n",tabtype);
    }
	       
    
    if(ltMsgGetVar_s(ltMsgPk,"groupid")){
    	gid=ltMsgGetVar_s(ltMsgPk,"groupid"); 	
    	printf("gid:%s\n",gid);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"gname")){
    	gname=ltMsgGetVar_s(ltMsgPk,"gname"); 	
    	printf("gname:%s\n",gname);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"glevel")){
    	glevel=ltMsgGetVar_s(ltMsgPk,"glevel");
      printf("glevel=%s\n",glevel);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"user")){
    	 caUser=ltMsgGetVar_s(ltMsgPk,"user");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"flags")){
    	caFlag=ltMsgGetVar_s(ltMsgPk,"flags");
      printf("caFlag=%s\n",caFlag); 
    }

    if(ltMsgGetVar_s(ltMsgPk,"mtype")){
    	 caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");
       printf("caMtype=%s\n",caMtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"subject")){
    	 caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
       printf("caSubject=%s\n",caSubject);
    }

    if(ltMsgGetVar_s(ltMsgPk,"dip")){
    	 caDip=ltMsgGetVar_s(ltMsgPk,"dip");
       printf("caDip=%s\n",caDip);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"from")){
    	 caFrom=ltMsgGetVar_s(ltMsgPk,"from");
       printf("caFrom=%s\n",caFrom);
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"to")){
    	 caTo=ltMsgGetVar_s(ltMsgPk,"to");
       printf("caTo=%s\n",caTo);  
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabname")){
    	tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
      printf("tabname=%s\n",tabname);
    }else{
    	tabname="";
    }
     
    if(ltMsgGetVar_s(ltMsgPk,"btype")){
    	caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
      printf("caBtype=%s\n",caBtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"bbyte")){
    	caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
      printf("caBbyte=%s\n",caBbyte);  
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"fbtype")){
    	caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
      printf("caFbtype=%s\n",caFbtype);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"fbbyte")){
    	 caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
       printf("caFbbyte=%s\n",caFbbyte);
    }
       
    if(ltMsgGetVar_s(ltMsgPk,"content")){
    	caContent=ltMsgGetVar_s(ltMsgPk,"content");
      printf("caContent=%s\n",caContent); 
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"maxtime")){//查询时长
    	caMaxtime=ltMsgGetVar_s(ltMsgPk,"maxtime");
    }
    

    if(ltMsgGetVar_s(ltMsgPk,"attchk")){//搜索附件内容
    	attchk=ltMsgGetVar_s(ltMsgPk,"attchk");
      printf("attchk=%s\n",attchk);
    }
              
    if(ltMsgGetVar_s(ltMsgPk,"osort")){
    	caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
      printf("caOrder=%s\n",caOrder);
    }          
    
    if(ltMsgGetVar_s(ltMsgPk,"orderby")){
    	caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
      printf("caOrderby=%s\n",caOrderby);
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
		
   if(strlen(tabtype)>0){
    memset(sqltablename,0,sizeof(sqltablename));
    memset(strBuf,0,sizeof(strBuf));
    sprintf(strBuf,"select sdate,tabname,tabtype from msaloglist where tabtype=%s and sdate='%s'",tabtype,tabname);      
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			    sprintf(sqltablename,"%s%s",tempRow[1],tempRow[0]);
          printf("sqltablename::%s\n",sqltablename);
			  }
		}
		
		ltDbCloseCursor(tempCursor);	
   }
		

 pid=fork();//子线程
 printf("pid:%d\n",pid);
 if(pid==0){
		  char *dbUser;
		  char *dbPass;
		  char *dbName; 
	    dbName=_ltPubInfo->_dbname;
		  dbUser=_ltPubInfo->_dbuser;
		  dbPass=_ltPubInfo->_dbpass;	
		  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	    memset(caWhere,0,sizeof(caWhere));
	    strcpy(caWhere,"");
	    alarm(maxtime);
	  	if(strcmp(gid,"-1")==0){
	  			char tmpgname[32];
		    	sprintf(tmpgname,"%s","部门名:");
		    	strgb2utf8(tmpgname,utf8Cmd,strlen(tmpgname));
		      sprintf(searchcon,"%s%s",utf8Cmd,"AllGroup");
					if(strcmp(caUser,"-1")==0){
				      sprintf(caWhere,"%s","2=2");
				      char tempuname[32];
				      sprintf(tempuname,"%s","|用户名:");
		    	    strgb2utf8(tempuname,utf8Cmd,strlen(tempuname));
		    	    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"AllUser");			       
					}
		  }
		
			if(strcmp(gid,"-1")!=0){
				  char tmpgname[32];
		    	sprintf(tmpgname,"%s","部门名:");
		    	strgb2utf8(tmpgname,utf8Cmd,strlen(tmpgname));
		    	sprintf(searchcon,"%s%s",utf8Cmd,gname);
		      
			  if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%s=%s",glevel,gid);
			       char tempuname[32];
				     sprintf(tempuname,"%s","|用户名:");
		    	   strgb2utf8(tempuname,utf8Cmd,strlen(tempuname));
		    	   sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"AllUser");		
				}else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%s=%s",caUser,glevel,gid);
					   char tempuname[32];
				     sprintf(tempuname,"%s","|用户名:");
		    	   strgb2utf8(tempuname,utf8Cmd,strlen(tempuname));
		    	   memset(strBuf,0,sizeof(strBuf));
		    	   sprintf(strBuf,"select username,dispname from msauser where userid=%s",caUser);
					 	 printf("strBuf:%s\n",strBuf);
						 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
						 if(tempCursor!=NULL){
							  tempRow=ltDbFetchRow(tempCursor);
							  if(tempRow!=NULL){
							   sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,tempRow[1]);	
							  }
						 }
						 ltDbCloseCursor(tempCursor);	 
					   
				}
			}
      
	   /*邮件状态*/
	   if(strcmp(caFlag,"-1")==0){/*全部邮件*/
	   		sprintf(caWhere,"%s and (b.flags<2) ",caWhere);
	   		char tempflag[32];
	   		sprintf(tempflag,"%s","|类型:");
		    strgb2utf8(tempflag,utf8Cmd,strlen(tempflag));
		    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"All");	
	   }else{
	   		sprintf(caWhere,"%s and (b.flags=%s)",caWhere,caFlag);
	   		char tempflag[32];
	   		sprintf(tempflag,"%s","|类型:");
		    strgb2utf8(tempflag,utf8Cmd,strlen(tempflag));
		    if(strcmp(caFlag,"0")){
		      sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"POP3");	
		    }else{
		    	sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"SMTP");	
		    }
		      		
	   }

	
	  /*邮件分类*/
		 if(strcmp(caMtype,"-1")!=0){
		 	 memset(strBuf,0,sizeof(strBuf));
		 	 sprintf(strBuf,"select rname from msamailfilter where rid=%s",caMtype);
			 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			 if(tempCursor!=NULL){
				  tempRow=ltDbFetchRow(tempCursor);
				  if(tempRow!=NULL){
				   sprintf(caWhere,"%s and (b.mtype='%s') ",caWhere,tempRow[0]);
				   char tempMtype[32];
				   sprintf(tempMtype,"%s","|分类:");
		       strgb2utf8(tempMtype,utf8Cmd,strlen(tempMtype));
				   sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,tempRow[0]);	
				  }
			 }
			 ltDbCloseCursor(tempCursor);	 
		 }
	

	  
    if(caSubject!=NULL){
	    if(strlen(caSubject)>0) {
	        sprintf(caWhere,"%s and (b.mailsub like '%c%s%c')",caWhere,'%',caSubject,'%');
	        char tempsubject[32];
	        sprintf(tempsubject,"%s","|主题包括:");
		      strgb2utf8(tempsubject,utf8Cmd,strlen(tempsubject));
				  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caSubject);	
	    }
    }
    
     if(caDip!=NULL){
	    if(strlen(caDip)>0) {
	        sprintf(caWhere,"%s and (b.dip = '%s')",caWhere,caDip);
	        char tempdip[32];
	        sprintf(tempdip,"%s","|远端IP:");
		      strgb2utf8(tempdip,utf8Cmd,strlen(tempdip));
				  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caDip);	
	    }
    }
    
    if(caFrom!=NULL){
	    if(strlen(caFrom)>0) {
	        sprintf(caWhere,"%s and (b.mailfrom='%s')",caWhere,caFrom);
	        char tempfrom[32];
	        sprintf(tempfrom,"%s","|发件人:");
		      strgb2utf8(tempfrom,utf8Cmd,strlen(tempfrom));
				  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caFrom);	
	    }
    }
    
    if(caTo!=NULL){
	    if(strlen(caTo)>0) {
	        sprintf(caWhere,"%s and (b.mailto='%s')",caWhere,caTo);
	        char tempto[32];
	        sprintf(tempto,"%s","|收件人:");
		      strgb2utf8(tempto,utf8Cmd,strlen(tempto));
				  sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caTo);	
	    }
    }
    
    
    //选择日期
    if(strlen(tabname)>0){
    	char tempdate[32];
   	 sprintf(tempdate,"%s","|选择日期:");
     strgb2utf8(tempdate,utf8Cmd,strlen(tempdate));
	   sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,tabname);	
    }
     
    
    
    if(strcmp(caBtype,"unit2") == 0)  { /*邮件大小*/
    	 sprintf(caWhere,"%s and (b.mailsize >%s)",caWhere,caBbyte);   	
    	 char tempbtype[32];
   	   sprintf(tempbtype,"%s","|邮件大小:");
       strgb2utf8(tempbtype,utf8Cmd,strlen(tempbtype));
	     sprintf(searchcon,"%s%s>%s",searchcon,utf8Cmd,caBbyte);	  
    }else{
    	 char tempbtype[32];
   	   sprintf(tempbtype,"%s","|邮件大小:");
       strgb2utf8(tempbtype,utf8Cmd,strlen(tempbtype));
	     sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"all");	
    }
    
    if(strcmp(caFbtype,"unit2") == 0)  { /*附件大小*/
    	  sprintf(caWhere,"%s and (b.mailasize >%s)",caWhere,caFbbyte);
    	  char tempfbtype[32];
   	    sprintf(tempfbtype,"%s","|附件大小:");
        strgb2utf8(tempfbtype,utf8Cmd,strlen(tempfbtype));
	      sprintf(searchcon,"%s%s>0",searchcon,utf8Cmd);	
    }else{
	    	 char tempfbtype[32];
	   	   sprintf(tempfbtype,"%s","|附件大小:");
	       strgb2utf8(tempfbtype,utf8Cmd,strlen(tempfbtype));
		     sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"all");	
    }
    
    //内容包括
    if(strlen(caContent)>0){
    	char tempcontent[32];
   	  sprintf(tempcontent,"%s","|内容包括:");
      strgb2utf8(tempcontent,utf8Cmd,strlen(tempcontent));
	    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caContent);	
    }
    
    //查询时常
    if(strlen(caMaxtime)>0){
    	char tempmaxtime[32];
   	  sprintf(tempmaxtime,"%s","|查询时长:");
      strgb2utf8(tempmaxtime,utf8Cmd,strlen(tempmaxtime));
	    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,caMaxtime);	
    }
    
    //是否搜索附件内容
    if(strcmp(attchk,"1")==0){
    	char tempattchk[32];
   	  sprintf(tempattchk,"%s","|搜索附件内容:");
      strgb2utf8(tempattchk,utf8Cmd,strlen(tempattchk));
	    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"Yes");	
    }else{
    	char tempattchk[32];
   	  sprintf(tempattchk,"%s","|搜索附件内容:");
      strgb2utf8(tempattchk,utf8Cmd,strlen(tempattchk));
	    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"No");	
    }
    
    
    if(caOrder!=NULL){
			   if( strlen(caOrder)>0 ) {
				    	 if(strcmp(caOrder,"nosort")!=0) {
				    	    char temporder[32];
				    	 	  sprintf(temporder,"%s","|是否排序:");
						      strgb2utf8(temporder,utf8Cmd,strlen(temporder));
							    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"Yes");	
				    	 	
					        if(strcmp(caOrder,"date")==0) {
					            sprintf(caWhere,"%s order by b.sdate ",caWhere);
					            char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"date");	
					        }else if(strcmp(caOrder,"from") == 0) {
					            sprintf(caWhere,"%s order by b.mailfrom ",caWhere);
					            char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"SenderAddress");
					        }else if(strcmp(caOrder,"to") == 0) {
					            sprintf(caWhere,"%s order by b.mailto ",caWhere);
					            char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"WriteAddress");
					        }else if(strcmp(caOrder,"dip") == 0) {
								      sprintf(caWhere,"%s order by b.dip ",caWhere);
								      char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"RemoteIP");
						      }else if(strcmp(caOrder,"size") == 0) {
					            sprintf(caWhere,"%s order by b.mailsize ",caWhere);
					            char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"MailSize");
					        }else if(strcmp(caOrder,"user") == 0) {
					            sprintf(caWhere,"%s order by b.userid ",caWhere);
					            char temporderby[32];
					            sprintf(temporderby,"%s","|分组方式:");
					            strgb2utf8(temporderby,utf8Cmd,strlen(temporderby));
					            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"User");
					        }
					        if(strlen(caOrderby)) {
					            if(caOrderby[0] == '1') {   				            	  
					                sprintf(caWhere,"%s desc",caWhere);
					                char temppaixu[32];
							            sprintf(temppaixu,"%s","|排序方式:");
							            strgb2utf8(temppaixu,utf8Cmd,strlen(temppaixu));
							            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"Descending");
					            }else{
					            	  sprintf(caWhere,"%s asc",caWhere);
					            	  char temppaixu[32];
							            sprintf(temppaixu,"%s","|排序方式:");
							            strgb2utf8(temppaixu,utf8Cmd,strlen(temppaixu));
							            sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"Ascending");
					            }
						    	}
				       }else{
				       	  char temporder[32];
				    	 	  sprintf(temporder,"%s","|是否排序:");
						      strgb2utf8(temporder,utf8Cmd,strlen(temporder));
							    sprintf(searchcon,"%s%s%s",searchcon,utf8Cmd,"No");	
				       }
			   }   
    }	
    
   printf("Where:%s\n",caWhere);  
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");//返回系统正在处理信息
   nasCvtStime(time(0),sDate,sTime);
   
   if(atoi(tabtype)==5){
   	sprintf(caTempDir,"/datacenter/msa/mailsearch/%lu/",ltStrGetId());
   }
   if(atoi(tabtype)==4){
   	sprintf(caTempDir,"/datacenter/msa/webmailsearch/%lu/",ltStrGetId());
   }
   if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {      
            return 0;//生成该报告目录错误 
        }
   }
   
   
   char *manageruser=NULL;
   if(ltMsgGetVar_s(ltMsgPk,"manageruser")){
		 manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	 }
	 
  sprintf(caFile1,"%sinfo.ini",caTempDir);
  printf("infourl:%s\n",caFile1);
	fdwrite = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if(fdwrite == (-1)) {
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		 ltMsgFree(ltMsgPk);
		 return 0;
	}

  printf("ini file start ................\n");
	sprintf(caCmd,"skey=%s\n",caContent);
  write(fdwrite,caCmd,strlen(caCmd));
  sprintf(caCmd,"sdate=%s_%s\n",sDate,sTime);
  write(fdwrite,caCmd,strlen(caCmd));
  sprintf(caCmd,"suser=%s\n",manageruser);
  write(fdwrite,caCmd,strlen(caCmd));
  sprintf(caCmd,"stype=%s\n","1");
  write(fdwrite,caCmd,strlen(caCmd));
  sprintf(caCmd,"scon=%s\n",searchcon);
  write(fdwrite,caCmd,strlen(caCmd));
  close(fdwrite);
  
  int tmpfdwrite=0;
  sprintf(caFile2,"%sproccess.ini",caTempDir);
  printf("proccessurl:%s\n",caFile2);
	tmpfdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if(tmpfdwrite == (-1)) {
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	
	sprintf(caCmd,"%s","1");
  write(tmpfdwrite,caCmd,strlen(caCmd));
  close(tmpfdwrite);
   
  sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,a.username from %s b left join msauser a on (a.userid =b.userid) where %s limit %ld offset %ld",sqltablename,caWhere,lRowNum,lStartRec);//limit:返回lRowNum行  offset 说明在开始返回行之前忽略多少行
  printf("sqlBuf:%s\n",sqlBuf);
   iNum=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
   	
		    tempRow=ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){		
       		 
						 caMsgFile=tempRow[1];
						 printf("caMsgFile:%s\n",caMsgFile);
						 int i;
						 for(i=0;i<strlen(caMsgFile);i++){
						 	    if(caMsgFile[i]==' '){
						 	    	caMsgFile[i]='\0';
						 	    }
						 }
						 iReturn=stat(caMsgFile ,&st);
				     if(iReturn!=0){
				        tempRow=ltDbFetchRow(tempCursor);
				        continue;
				     }

				     if(S_ISDIR(st.st_mode)==0){
				    	  tempRow=ltDbFetchRow(tempCursor);
						    continue;
				     }	
										  
				   	 dir=opendir(caMsgFile); 			   	 
				   	 if(dir){		   		
				   		   		
					   	  while((d=readdir(dir))){
					   	  	 
					   	  	 if(strcmp(d->d_name,".")==0) continue;
					    		 if(strcmp(d->d_name,"..")==0) continue;
					   	  	//有附件				   	  	 
					   	  	if(atoi(attchk)==1){
					   	  	   if(strcmp(d->d_name,"key.txt")==0){
					   	  	   	 sprintf(caFile1,"%s/%s", caMsgFile, d->d_name);
					   	  	   	 printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv%s\n",caFile1);
					   	  	   	 fp=fopen(caFile1,"r");
//					   	  	   	 char *tmpfile=NULL;
//					   	  	   	 fgets(tmpfile,strlen(caContent)+1,fp);
//					   	  	   	 printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr%s\n",tmpfile);
//					   	  	   	 while(tmpfile!=NULL){
//					   	  	   	 	  if(strstr(tmpfile,caContent)!=0){
//					   	  	   	 	      break;
//					   	  	   	    }
//					   	  	   	    fgets(tmpfile,strlen(caContent)+1,fp);		
//					   	  	   	 }					   	  	   	 						   	  	   						   	  	   	 
					   	  	   	 if(ltFileSkipUntilStr(fp,caContent,strlen(caContent))<0){
					   	  	   	 	  continue;
					   	  	   	 }				   	  	   	 
					   	  	   	 fclose(fp);
					   	  	   	 int wjson=0;
					   	  	   	 sprintf(caFile1,"%ssearch.csv",caTempDir);
					   	  	   	 printf("searchurl:%s\n",caFile1);
					   	  	   	 wjson = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
											 if(wjson == (-1)) {
													 ltMsgPk->msgpktype=1;
													 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
													 ltMsgFree(ltMsgPk);
													 return 0;
											 }
											 
											 //将数据写入文件
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
						            
						            					            
											  sprintf(caTmpp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
											    tempRow[0],tempRow[11],tempRow[12],tempRow[1],tempRow[2],tempRow[3],tempRow[4],
							            tempRow[5],mailaSize,mailSize,tempRow[8],tempRow[9],tempRow[10]);		
					   	  	   	  write(wjson,caTmpp,strlen(caTmpp));
					   	  	   	  close(wjson);
					   	  	   	  					   	  	   	  
					   	  	   	  //记录数据
					   	  	   	  iNum++;
					   	  	   }
					   	  	}
					   	  	
					   	  }
				   	 }
				     closedir(dir);					
						 tempRow=ltDbFetchRow(tempCursor);
			}
			
     ltDbCloseCursor(tempCursor);
        
   }
   
   
    int wrnum=0;
	 	sprintf(caFile1,"%ssearch.count",caTempDir);
	 	wrnum = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(wrnum == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 return 0;
		}
    sprintf(caTmpp,"%d",iNum);
    write(wrnum,caTmpp,strlen(caTmpp));
		close(wrnum);
		
		sprintf(caFile2,"rm -f %sproccess.ini",caTempDir);
    system(caFile2);


	 ltMsgFree(ltMsgPk);	 
	 _exit(0);//退出子进程
    
 }
  
  
  printf("end function .................................\n");
  
  return 0;
  
}



////邮件内容查询数据列表/////
int nasformsearchlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
	printf("nasformsearchlist start .......................\n");
	char  caFile1[256];
  stralloc strTemp;
  int   lNum;
  char  caTmpp[2048];
  char  caCmd[256];
  char  mydir[256];
	char  *filedir;
	int   mytype;
	int   len=0; 
	struct stat st; 
  msasDir *fnames;
  
  mytype=0;
  memset(mydir,0,sizeof(mydir));
  
  if(ltMsgGetVar_s(ltMsgPk,"mytype")){
  	 mytype=atol(ltMsgGetVar_s(ltMsgPk,"mytype"));
  }
 
  printf("mytype:%d\n",mytype);
  
  if(mytype==5){
  	sprintf(mydir,"%s","mailsearch");
  }
  if(mytype==4){
  	sprintf(mydir,"%s","webmailsearch");
  }
  
  filedir=ltMsgGetVar_s(ltMsgPk,"file");//删除参数
  if(filedir){
  	if(strlen(filedir)>1){
			sprintf(caCmd,"rm -Rf /datacenter/msa/%s/%s",mydir,filedir);
			system(caCmd);
		}
	}
	
	sprintf(caFile1,"/datacenter/msa/%s/",mydir);


 fnames=mydirlist(caFile1,&len);
 if(fnames){
 	qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
 }
 
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 stralloc_cats(&strTemp,caTmpp);
 lNum=0;
 while(len--){
 	      int fileproc=-1;
			  char   sname[256];
			  char   sDate[256];
			  char   suser[256];
			  char   stype[256];
			  char   searchcon[2048];
			  memset(sname,0,sizeof(sname));
        memset(sDate,0,sizeof(sDate));
        memset(suser,0,sizeof(suser));
        memset(stype,0,sizeof(stype));
        memset(searchcon,0,sizeof(searchcon));

				sprintf(caFile1,"/datacenter/msa/%s/%s/info.ini",mydir,fnames[len].caDirName);
				printf("caFile1:%s\n",caFile1);
		    bcCnfGetValue_s(caFile1,"skey",sname);
        bcCnfGetValue_s(caFile1,"sdate",sDate);
        bcCnfGetValue_s(caFile1,"suser",suser);
        bcCnfGetValue_s(caFile1,"stype",stype);
        bcCnfGetValue_s(caFile1,"scon",searchcon);
        printf("searchcon:%s\n",searchcon);
				
			  sprintf(caFile1,"/datacenter/msa/%s/%s/proccess.ini",mydir,fnames[len].caDirName);
			  printf("caFile1:%s\n",caFile1);
		    if(stat(caFile1,&st) == 0){
		    	fileproc=1; 
		    }else{
		      fileproc=0; 
		    } 			
		    memset(caTmpp,0,sizeof(caTmpp));
		    lNum++;
		    
			  if(lNum==1){
					sprintf(caTmpp,"{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\",\"delfun\":\"%s\",\"mytype\":\"%d\"}",
						                sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"The search was generating or when you make a mistake ":"Search success ","nasformsearchlist",mytype); 
										        stralloc_cats(&strTemp,caTmpp);			
	      }else{
	        sprintf(caTmpp,",{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\",\"delfun\":\"%s\",\"mytype\":\"%d\"}",
						                sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"The search was generating or when you make a mistake ":"Search success ","nasformsearchlist",mytype);  
	                          stralloc_cats(&strTemp,caTmpp);			
	      }  

 }
 
  stralloc_cats(&strTemp,"]}");
  stralloc_0(&strTemp); 
	
	
	printf("myjson:%s\n",strTemp.s);
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	
	if(fnames){
 		free(fnames);
  }
  
  ltMsgFree(ltMsgPk);
  
  printf("nasformsearchlist end ........\n");
  
	return 0;
	

}


/*
 *function:服务访问记录节点：列表显示本段时间访问记录
 */
int msaMailconLook(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
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
  char  *tabtype;
	
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
  if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
  	tabtype=ltMsgGetVar_s(ltMsgPk,"tabtype");
  }else{
  	tabtype="";
  }
  
 
	if(atoi(tabtype)==5){
		sprintf(caFile1,"%s/mailsearch/%s/search.csv",_datacenterdir,caUrl);
	}
	if(atoi(tabtype)==4){
		sprintf(caFile1,"%s/webmailsearch/%s/search.csv",_datacenterdir,caUrl);
	}

	printf("caFile1:%s\n",caFile1);
	iReturn=access(caFile1,F_OK);
	printf("iReturn1:%d\n",iReturn);
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
  
  if(atoi(tabtype)==5){
		 sprintf(caFile2,"%s/mailsearch/%s/search.count",_datacenterdir,caUrl);
	}
	if(atoi(tabtype)==4){
		 sprintf(caFile2,"%s/webmailsearch/%s/search.count",_datacenterdir,caUrl);
	}
	iReturn=access(caFile2,F_OK);
	printf("iReturn2:%d\n",iReturn);
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
	
	printf("testjson:%s\n",strTemp.s);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	
	
	printf("end msaMailconLook....................\n");
	
	return 0;
     		
}
