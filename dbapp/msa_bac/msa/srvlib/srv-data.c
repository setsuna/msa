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
	 int    showtype;
	 char   caFile1[256];
	 int    len=0;//目录文件个数
	 int    myindex=0;
	 msasDir *fnames;
	 ltTablePtr  tablePtr;
   showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
   printf("%d\n",showtype);
   sprintf(caFile1,"%s",_datacenterdir);
   printf("%s\n",caFile1);
   fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	 }
     
   dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");//页面初始化
	 
   //打印到页面参数
	 tablePtr=lt_dbput_table(dbPtr,"datelist");
	 while(len--) //free all allocated memory blocks
   {
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
	 
	 if(fnames){//释放内存
 		free(fnames);
   }
	 return 0;
   
}

/*
 *function:服务访问记录、服务阻断记录、http访问记录:点击节点链接函数
 */
int msaShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 int    showtype;
	 char   caFile1[256];
	 int    len=0;//目录个数
	 int    iii;
	 //char   sqlBuf[1024];
	 //LT_DBROW tempRow;
	 //ltDbCursor *tempCursor;
	 msasDir *fnames;
	 ltTablePtr  tablePtr;
   showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
   char *sGroups=NULL;
   sGroups=ltMsgGetVar_s(ltMsgPk,"managergroup");//预留cookis
   if(!sGroups){
  	sGroups="999999";
   }
	
   sprintf(caFile1,"%s",_datacenterdir);//目录
   printf("%s\n",caFile1);
   fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	 }
      	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
	 tablePtr=lt_dbput_table(dbPtr,"datelist");
	 while(len--) //free all allocated memory blocks
   {
   	 
   	printf("%d\n",len);
   	printf("%s\n",fnames[len].caDirName);
   	if(strlen(fnames[len].caDirName)==8 && fnames[len].caDirName[0]=='1' ){
   		lt_dbput_recordvars(tablePtr,1,"strdate",LT_TYPE_STRING,fnames[len].caDirName);
   	}
   }
   
   tablePtr=lt_dbput_table(dbPtr,"srvlist");
   for(iii=0;iii<_ltPubInfo->lServiceNum;iii++){
   	 lt_dbput_recordvars(tablePtr,2,"srvid",LT_TYPE_LONG,iii,
   	 "srvname",LT_TYPE_STRING,_ltPubInfo->ltService[iii].srvName);
   	 printf("%d\n",iii);
   	 printf("%s\n",_ltPubInfo->ltService[iii].srvName);
   }
//   //lServiceNum:暂无
//   tablePtr=lt_dbput_table(dbPtr,"glist");
//   if(strcmp(sGroups,"999999")==0){
//   	sprintf(sqlBuf,"select id,name from msagroup");
//   	printf("%s\n",sqlBuf);
//   	lt_dbput_recordvars(tablePtr,2,
//							"gid",LT_TYPE_STRING,"999",
//							"gname",LT_TYPE_STRING,"All Department");//所有部门 用中文出现乱码
//							
//   }else{
//    sprintf(sqlBuf,"select id,name from msagroup where id in (%s)",sGroups);
//   }
//   
//	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
//	 if(tempCursor!=NULL){
//		tempRow= ltDbFetchRow(tempCursor);
//		while(tempRow!=NULL){
//			  printf("gid=%s\n",tempRow[0]);
//			  printf("gname=%s\n",tempRow[1]);
//				lt_dbput_recordvars(tablePtr,2,
//							"gid",LT_TYPE_STRING,tempRow[0],
//							"gname",LT_TYPE_STRING,tempRow[1]);
//			  tempRow= ltDbFetchRow(tempCursor);
//		}
//		ltDbCloseCursor(tempCursor);
//	 }

   ltMsgPk->msgpktype=1;
   if(showtype==1){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipbacksearchlist.htm",dbPtr->head,0);
	 }else if(showtype==2){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipblocksearchlist.htm",dbPtr->head,0);
	 }else if(showtype==3){
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
int msaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

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
  char  linebuf[2048];
  int   iIndex=0;
  int   xIndex=0;
  char  tempStr[512];
  char  *pppp;
  int   iReturn;
  char  *caUrl;
	
  static char inbuf[4096];
  static stralloc line = {0};
  buffer ss;
  int    match;
  int    kkkk;
  int    jjjj;
  int    iiii;
  
  
  caUrl=ltMsgGetVar_s(ltMsgPk,"url");
  printf("%s\n",caUrl);
	if(!caUrl){
	 	 caUrl="";
	}
	
	sprintf(caFile1,"%s/%s.csv",_datacenterdir,caUrl);
	printf("caFile1=%s\n",caFile1);
	iReturn = access(caFile1,F_OK);
	printf("iReturn=%d\n",iReturn);
   if(iReturn!=0){
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 return 0;
    } 

  lRowNum=100; /*每页的行数*/
  lSumRec=0;/*总行数*/
  lCount=0; 
  lStartRec=0;
  lNum=0; /*该页剩余的行数*/
	
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
   
  sprintf(caFile2,"%s/%s.count",_datacenterdir,caUrl);
  printf("caFile2:%s\n",caFile2); 
	iReturn = access(caFile2,F_OK);
  if(iReturn!=0) {
     /*读取rowline*/  
     fd=open(caFile1,O_RDONLY | O_NDELAY); 
		 if(fd<1){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
		 }
		 totalnum=0;
		 iCount=read(fd,inbuf,8192); 
		 printf("iCount=%d\n",iCount);
		 while(iCount){ 
		     for(inum=0;inum<iCount;inum++){
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
    memset(res,0,sizeof(res)); //sizeof(res)：字节大小
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
    printf("read :%s %s %d \n",caFile2,ress,totalnum); 
  }
  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  printf("fd=%d\n",fd);
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
         }
  nownum=0;
  memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  pppp=linebuf;
  
  nownum=0;
  xIndex=0;
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for (;;)
  {
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		nownum++;
		if(nownum>=lStartRec){
			striptrailingwhitespace(&line);  //去掉尾部\n | \r
			iIndex=0;
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
			         printf("iIndex==1-->tempStr=%s\n",tempStr);
			         stralloc_cats(&strTemp,tempStr);				          
			     }else{
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         printf("tempStr=%s\n",tempStr);
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
		  if(nownum==(lRowNum+lStartRec)){//lStartRec-->start lRowNum---->limit
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
  
    downLoadFile=ltMsgGetVar_s(ltMsgPk,"downLoadFile");
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
		for (;;)
	  {
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
	char   caTempDir[256];
	char   *searchname;
	int    gid;
	char   cpOut1[256];
	char   *manageruser;
	int    fdwrite=0;
	char   sDate[32];
	int    kkkk;
	int    jjjj;
	int    iiii;
	static char inbuf[4096];//
	static stralloc line = {0};//
	buffer ss;
	int    match;
	char   *exportaction;
	int    showtype;
	
	showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	printf("%d\n",showtype);
	memset(sDate,0,sizeof(sDate));
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	nasTimeFormat(sDate,time(0));
	printf("sDate=%s\n",sDate);
	maxv=32;
	vv=(char **)malloc(maxv*sizeof(char *));
	for(iIndex=0;iIndex<maxv;iIndex++){
		vv[iIndex]=NULL;
	}

	iIndex=0;
	memset(cpOut,0,256);
	gid=999;
	seldate=ltMsgGetVar_s(ltMsgPk,"seldate");
	printf("seldate=%s\n",seldate);
	seltime=ltMsgGetVar_s(ltMsgPk,"seltime");
	printf("seltime=%s\n",seltime);
	
	if(showtype==1){
		if(ltMsgGetVar_s(ltMsgPk,"gid")){
		  	gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));//gid为：groupid部门分组
		  	if(gid!=999){
		  		vv[15]=ltMsgGetVar_s(ltMsgPk,"gid");//iplog为24  替换  15
		  			printf("gid=%s\n",vv[15]);
		  	}
		}
		vv[8]=ltMsgGetVar_s(ltMsgPk,"sip");//源IP
		printf("sip=%s\n",vv[8]);
		vv[9]=ltMsgGetVar_s(ltMsgPk,"dip");//目标IP
		printf("dip=%s\n",vv[9]);
		vv[11]=ltMsgGetVar_s(ltMsgPk,"dport");//目的端口
		printf("dport=%s\n",vv[11]);
		srvid=atol(ltMsgGetVar_s(ltMsgPk,"selsrv"));//服务类型
		printf("srvid=%d\n",srvid);	  
		if(srvid>-1 && srvid<_ltPubInfo->lServiceNum){
		  	vv[12]=_ltPubInfo->ltService[srvid].srvName;
		  	printf("vv[12]=%s\n",vv[12]);
		}
	}else if(showtype==2){
		if(ltMsgGetVar_s(ltMsgPk,"gid")){
		  	gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));//部门
		  	if(gid!=999){
		  		vv[16]=ltMsgGetVar_s(ltMsgPk,"gid");
		  	}
		}
		vv[6]=ltMsgGetVar_s(ltMsgPk,"sip");
		printf("sip=%s\n",vv[6]);
		vv[7]=ltMsgGetVar_s(ltMsgPk,"dip");
		printf("dip=%s\n",vv[7]);
		vv[11]=ltMsgGetVar_s(ltMsgPk,"dport");
		printf("dport=%s\n",vv[11]);
		srvid=atol(ltMsgGetVar_s(ltMsgPk,"selsrv"));
		printf("srvid=%d\n",srvid);
		if(srvid>-1 && srvid<_ltPubInfo->lServiceNum){//lServiceNum：结构体中暂无此成员
		  	vv[12]=_ltPubInfo->ltService[srvid].srvName;
		  	printf("vv[12]=%s\n",vv[12]);
		}
	}else if(showtype==3){
		if(ltMsgGetVar_s(ltMsgPk,"gid")){
		  	gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
		  	if(gid!=999){
		  		vv[14]=ltMsgGetVar_s(ltMsgPk,"gid");
		  		printf("vv[14]=%s",vv[14]);
		  	}
		}
		vv[7]=ltMsgGetVar_s(ltMsgPk,"sip");
		printf("vv[7]=%s\n",vv[7]);
		vv[9]=ltMsgGetVar_s(ltMsgPk,"dip");
		printf("vv[9]=%s\n",vv[9]);
		vv[11]=ltMsgGetVar_s(ltMsgPk,"host");
	}
    searchconX=ltMsgGetVar_s(ltMsgPk,"searchconX");
    printf("searchconX=%s\n",searchconX);

	exportaction=NULL;
	exportaction=ltMsgGetVar_s(ltMsgPk,"exportaction");
	printf("exportaction=%s\n",exportaction);
	memset(cpOut1,0,256);
	searchname=ltMsgGetVar_s(ltMsgPk,"searchname");
	printf("searchname=%s\n",searchname);

    if(exportaction==NULL){
  	    if(showtype==1){
		  	sprintf(caTempDir,"/datacenter/msa/ipsearch/");
		}else if(showtype==2){
		  	sprintf(caTempDir,"/datacenter/msa/ipblocksearch/");
		}else if(showtype==3){
		  	sprintf(caTempDir,"/datacenter/msa/httpsearch/");
		}
		
		if(ltFileIsExist(caTempDir)!=1) {
			if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				/// 生成该报告目录错误 
				return 0;
			}
		}
		if(showtype==1){
			sprintf(caTempDir,"%s/ipsearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
			printf("%s\n",caTempDir);
		}else if(showtype==2){
			sprintf(caTempDir,"%s/ipblocksearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
			printf("%s\n",caTempDir);
		}else if(showtype==3){
			sprintf(caTempDir,"%s/httpsearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
			printf("%s\n",caTempDir);
		}
		if(ltFileIsExist(caTempDir)!=1) {
			if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				/// 生成该报告目录错误 
				return 0;
			}
		}
		
		sprintf(caFile2,"%s/info.ini",caTempDir);
		fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);//fdwrite文件指针
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
					"lthtml",LT_TYPE_STRING,"0");		
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
					if(vv[15]){//部门
						printf("vv15=%s\n",vv[15]);
						if(vv[15][0]){
							printf("vv[15][0]=%c\n",vv[15][0]);
							sprintf(caCmd,"|GroupID:%s",vv[15]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					
					if(vv[8]){//源IP
						printf("vv8=%s\n",vv[8]);
						if(vv[8][0]){
							sprintf(caCmd,"|SourceIP:%s",vv[8]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					if(vv[9] ){//目标IP
						if(vv[9][0]){
							sprintf(caCmd,"|PurposeIP:%s",vv[9]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				    if(vv[11]){//端口
						if(vv[11][0]){
							sprintf(caCmd,"|DestinationPort:%s",vv[11]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				    if(vv[12]){
						if(vv[12][0] ){
							sprintf(caCmd,"|Service:%s",vv[12]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				}else if(showtype==2){
					if(vv[16]){
						if(vv[16][0]){
							sprintf(caCmd,"|GroupID:%s",vv[16]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					
					if(vv[6]){
						if(vv[6][0]){
							sprintf(caCmd,"|SourceIP:%s",vv[6]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					if(vv[7]){
						if(vv[7][0]){
							sprintf(caCmd,"|PurposeIP:%s",vv[7]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				    if(vv[11]){
						if(vv[11][0] ){
							sprintf(caCmd,"|DestinationPort:%s",vv[11]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				    if(vv[12]){
						if(vv[12][0]){
							sprintf(caCmd,"|Service:%s",vv[12]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				}else if(showtype==3){
					if(vv[14]){
						if(vv[14][0] ){
							sprintf(caCmd,"|GroupID:%s",vv[14]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					
					if(vv[7]){
						if(vv[7][0] ){
							sprintf(caCmd,"|SourceIP:%s",vv[7]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
					if(vv[9]){
						if(vv[9][0] ){
							sprintf(caCmd,"|PurposeIP:%s",vv[9]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				    if(vv[11]){
						if(vv[11][0] ){
							sprintf(caCmd,"|URL:%s",vv[11]);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
					}
				  
				}
					
				if(searchconX){
						if(searchconX[0] ){
							sprintf(caCmd,"|KeyWord:%s",searchconX);
							printf("caCm=%s\n",caCmd);
							write(fdwrite,caCmd,strlen(caCmd));
						}
				}
		  close(fdwrite);
  
    }else{
  	     ltMsgPk->msgpktype=2;
	     lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"search.csv","contenttype","Application/MS-DOWNLOAD");
    }
  if(showtype==1){
 		 sprintf(caFile1,"%s/%s/iplog/iplog-%s.csv",_datacenterdir,seldate,seltime);
		 printf("caFile1=%s\n",caFile1);
 	}else if(showtype==2){
 		 sprintf(caFile1,"%s/%s/ipblock/ipblocklog-%s.csv",_datacenterdir,seldate,seltime);
 		 printf("caFile1=%s\n",caFile1);
	}else if(showtype==3){
 		 sprintf(caFile1,"%s/%s/httplog/httplog-%s.csv",_datacenterdir,seldate,seltime);
 		 printf("caFile1=%s\n",caFile1);
	}
	
    iReturn = access(caFile1,F_OK); //判断iplog目录中是否存在iplog-10.cvs文件
    printf("iReturn=%d\n",iReturn);//iReturn=0
    if(iReturn!=0){
				  	if(exportaction==NULL){
				     ltMsgPk->msgpktype=1;
						 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
								"lthtml",LT_TYPE_STRING,"0");		
						}else{
						 write(confd,"open file error",strlen("open file error"));
						}
				ltMsgFree(ltMsgPk);
				return 0;
    }
   
    fd=open(caFile1,O_RDONLY | O_NDELAY); //打开iplog目录iplog-10.cvs文件
    printf("caFile1=%s\n",caFile1);//fd=5
    if(fd<1){
        if(exportaction==NULL){
            ltMsgPk->msgpktype=1;
		   			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
							"lthtml",LT_TYPE_STRING,"0");
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
		  printf("fdwrite=%d\n",fdwrite);
			  if(fdwrite == (-1)) {
					 ltMsgPk->msgpktype=1;
					 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
							"lthtml",LT_TYPE_STRING,"0");		
					 ltMsgFree(ltMsgPk);
					 close(fd);
					 return 0;
				}
	}
	nownum=0;
	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;){
		if(nownum>=100000){break;}
		if (getln(&ss,&line,&match,'\n') == -1){break;}
		if (!match && !line.len) { break; }		
	  //printf("line.s=%s\n",line.s);
		if(searchconX){//判断是否有匹配关键字
						if(searchconX[0]){
						   if(!memmem(line.s,line.len,searchconX,strlen(searchconX))){
						     	 	 	continue;//如果没匹配的关键字则继续for循环
						   }
						}	
					} 
				
    iiii=0;
    iIndex=0;
    kkkk=0;
		for(jjjj=0;jjjj<line.len;jjjj++){
			if(line.s[jjjj]=='\t'){
				//printf("jjjj=%d\n",jjjj);
				//printf("line.len=%d\n",line.len);
				if(exportaction){						 
				 line.s[jjjj]=',';
				}
				 iIndex++;//iIndex变为1						 
				 //找出符合条件的数据   
				// printf("vv[iIndex]1=%s\n",vv[iIndex]);  
				 //vv[8]满足条件
//				 if(vv[iIndex]){//jjjj-iiii 13 strlen(vv[iIndex]) 12    
//				 	printf("vv[iIndex]2=%s\n",vv[iIndex]); 
//				 	printf("iiii=%d\n",iiii);             
//				 	 if(vv[iIndex][0]){ 
//				 	 	 if(!memmem(&line.s[iiii],jjjj-iiii,vv[iIndex],strlen(vv[iIndex]))){//判断line.s是否包含vv[iIndex]
//					 	 	 	kkkk=1;
//					 	 	 	break;//没有找到匹配的退出for循环
//				 	 	 }
//				 	 }	
//				 }        
				 jjjj++;
				 iiii=jjjj;
		    }
		}
		
		if(kkkk==1){continue; } 
	  nownum++; 
	  
	  if(exportaction==NULL){
	  	  write(fdwrite,line.s,line.len);
	    }else{
	      write(confd,line.s,line.len);
	    }
  }//for
    
	if(exportaction==NULL){
	    close(fd);
	    close(fdwrite);
	    sprintf(caFile2,"echo '%d'>%ssearch.count",nownum,caTempDir);
	    printf("caFile2=%s\n",caFile2);
	    system(caFile2);
	    sprintf(caFile2,"rm -f %sproccess.ini",caTempDir);
	    printf("caFile2=%s\n",caFile2);
	    system(caFile2);
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
					"lthtml",LT_TYPE_STRING,"1");		
	}	
	
	ltMsgFree(ltMsgPk);
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
 // int    listtype;
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
						  	printf("%s\n",caFile1);
						  }else if(showtype==2){
						  	sprintf(caFile1,"/datacenter/msa/ipblocksearch/%s/proccess.ini",fnames[len].caDirName);
						  	printf("%s\n",caFile1);
						  }else if(showtype==3){
						  	sprintf(caFile1,"/datacenter/msa/httpsearch/%s/proccess.ini",fnames[len].caDirName);
						  	printf("%s\n",caFile1);
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


/*服务记录汇总链接页面函数*/
int ltshowservicequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char sqlBuf[2048];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
  char *sGroups=NULL;
  char *gid;
  char *caGroupid;
	char *caUser;
	char caWhere[1024];
  char caWhereU[1024];
  int  lWhere;
  unsigned int  lTime1;
  char xDate[32];
  int  iii;
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  if(gid==NULL){
    	gid="-1";
  }
  sGroups=ltMsgGetVar_s(ltMsgPk,"managergroup");
  if(!sGroups){
  	sGroups="999999";
  }
  memset(caWhere,0,sizeof(caWhere));
  caGroupid=ltMsgGetVar_s(ltMsgPk,"groupid");
  caUser=ltMsgGetVar_s(ltMsgPk,"user");
    

	    if(strcmp(sGroups,"999999")==0){
					if(strlen(caGroupid)>0) {
						if(strcmp(caGroupid,"-1")){
					        	if(strcmp(caUser,"-1")==0){
							        	sprintf(caWhere," and a.groupid = %s",caGroupid);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
						}
			    }
			}else{
					          
					          
					        	//sprintf(caWhere," and a.groupid = %s",sGroups);
					        	if(strcmp(caUser,"-1")==0 ){
							        	sprintf(caWhere," and a.id in (%s)",sGroups);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
					
			}
			if(strcmp(sGroups,"999999")==0){
				sprintf(sqlBuf,"select id,name from msagroup");
			}else{
				sprintf(sqlBuf,"select id,name from msagroup where id in (%s)",sGroups);
			}
	
	
  
	 lTime1 = time(0) -  86400L;
	 memset(xDate,0,sizeof(xDate));
	 nasTimeFormat(xDate,lTime1);
	 sprintf(caWhere,"%s and b.sdate >= '%s'",caWhere,xDate);           
	
	memset(caWhereU,0,sizeof(caWhereU));
	if(strlen(caWhere)>0){
		ltEncode64(caWhere, strlen(caWhere), caWhereU, 1024, &lWhere);
		for(lWhere=0;lWhere<strlen(caWhereU);lWhere++){
	                if(caWhereU[lWhere]=='+'){
	                        caWhereU[lWhere]='*';
	                }
	                if(caWhereU[lWhere]=='/'){
	                        caWhereU[lWhere]='#';
	                }
	  }
  }

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"glist");
  if(strcmp(sGroups,"999999")==0){
				 lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,"-1",
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,"All");
	}

  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			 if(strcmp(gid,tempRow[0])==0){
									 lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,tempRow[1]);
								 }else{
								 		lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," ",
										"gname",LT_TYPE_STRING,tempRow[1]);
				}	
			
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	 tablePtr=lt_dbput_table(dbPtr,"ulist");
   if(strcmp(gid,"-1")!=0){
    	sprintf(sqlBuf,"select userid,dispname from msauser where groupid0='%s' order by dispname ",gid);
    	printf("%s\n",sqlBuf);
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							while(tempRow){
							
								if(strcmp(caUser,tempRow[0])==0){
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," selected ");
								}else{
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," ");
							  }	
								
								tempRow= ltDbFetchRow(tempCursor);
							}
							ltDbCloseCursor(tempCursor);
			}
	}
	
	tablePtr=lt_dbput_table(dbPtr,"srvlist");
	
	for(iii=0;iii<25;iii++){
			lt_dbput_recordvars(tablePtr,2,
				"srvid",LT_TYPE_LONG,iii,
				"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
	}
	
	if(strlen(caWhereU)>0){
  		sprintf(caWhere,"where=%s",caWhereU);
  		lt_dbput_rootvars(dbPtr,1,"where",caWhere);
  }
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msaservicehead.htm",dbPtr->head,0);
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


/*HTTP服务汇总节点调用函数*/
int ltwebshowwebsitequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char sqlBuf[2048];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
  char *sGroups=NULL;
  char *gid;
  char *caGroupid;
	char *caUser;
	char caWhere[512];
  char caWhereU[512];
  int  lWhere;
  unsigned int  lTime1;
  char xDate[32];
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  if(gid==NULL){
    	gid="-1";
  }
  sGroups=ltMsgGetVar_s(ltMsgPk,"managergroup");
  if(!sGroups){
  	sGroups="999999";
  }
  memset(caWhere,0,sizeof(caWhere));
  caGroupid=ltMsgGetVar_s(ltMsgPk,"groupid");
  caUser=ltMsgGetVar_s(ltMsgPk,"user");
    

	    if(strcmp(sGroups,"999999")==0){
					if(strlen(caGroupid)>0) {
						if(strcmp(caGroupid,"-1")){
					        	if(strcmp(caUser,"-1")==0){
							        	sprintf(caWhere," and a.id = %s",caGroupid);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
						}
			    }
			}else{
					
					        	//sprintf(caWhere," and a.groupid = %s",sGroups);
					        	if(strcmp(caUser,"-1")==0 ){
							        	sprintf(caWhere," and a.id in (%s)",sGroups);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
					
			}
			if(strcmp(sGroups,"999999")==0){
				sprintf(sqlBuf,"select id,name from msagroup");
			}else{
				sprintf(sqlBuf,"select id,name from msagroup where id in (%s)",sGroups);
			}
	
	
  
	 lTime1 = time(0) -  86400L;
	 memset(xDate,0,sizeof(xDate));
	 nasTimeFormat(xDate,lTime1);
	 sprintf(caWhere,"%s and b.sdate >= '%s'",caWhere,xDate);           
	
	memset(caWhereU,0,sizeof(caWhereU));
	if(strlen(caWhere)>0){
		ltEncode64(caWhere, strlen(caWhere), caWhereU, 512, &lWhere);
		for(lWhere=0;lWhere<strlen(caWhereU);lWhere++){
	                if(caWhereU[lWhere]=='+'){
	                        caWhereU[lWhere]='*';
	                }
	                if(caWhereU[lWhere]=='/'){
	                        caWhereU[lWhere]='#';
	                }
	  }
  }

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"glist");


  if(strcmp(sGroups,"999999")==0){
				lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,"-1",
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,"All");
	}
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			 if(strcmp(gid,tempRow[0])==0){
									 lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,tempRow[1]);
								 }else{
								 		lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," ",
										"gname",LT_TYPE_STRING,tempRow[1]);
				}	
			
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	 tablePtr=lt_dbput_table(dbPtr,"ulist");
   if(strcmp(gid,"-1")!=0){
    	sprintf(sqlBuf,"select userid,dispname from msauser where groupid0='%s' order by dispname ",gid);
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							while(tempRow){
							
								if(strcmp(caUser,tempRow[0])==0){
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," selected ");
								}else{
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," ");
							  }	
								
								tempRow= ltDbFetchRow(tempCursor);
							}
							ltDbCloseCursor(tempCursor);
			}
	}
	
	tablePtr=lt_dbput_table(dbPtr,"srvlist");
	sprintf(sqlBuf,"select sortid,sorname from msaurlsort");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
							
			lt_dbput_recordvars(tablePtr,2,
				"srvid",LT_TYPE_STRING,tempRow[0],
				"srvname",LT_TYPE_STRING,tempRow[1]);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	if(strlen(caWhereU)>0){
  		sprintf(caWhere,"where=%s",caWhereU);
  		lt_dbput_rootvars(dbPtr,1,"where",caWhere);
  }
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msawebsitehead.htm",dbPtr->head,0);
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


/*MSN,QQ记录节点链接调用函数*/
int ltimshowwebquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char sqlBuf[2048];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;

  char *sGroups=NULL;
  char *gid=NULL;
  
  char *caGroupid;
	char *caUser;
	char *tflags;
	char caWhere[512];
  char caWhereU[512];
  int  lWhere;
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  printf("gid=%s\n",gid);
  if(gid==NULL){
    	gid="-1";
  }
  
  sGroups=ltMsgGetVar_s(ltMsgPk,"managergroup");
  printf("sGroups=%s\n",sGroups);
  if(!sGroups){
  	sGroups="999999";
  }
  tflags=ltMsgGetVar_s(ltMsgPk,"tflags");
  printf("tflags=%s\n",tflags);  
  caGroupid=ltMsgGetVar_s(ltMsgPk,"groupid");
  printf("caGroupid=%s\n",caGroupid);  
  caUser=ltMsgGetVar_s(ltMsgPk,"user");
  printf("caUser=%s\n",caUser);  
  memset(caWhere,0,sizeof(caWhere));
  memset(caWhereU,0,sizeof(caWhereU));
      printf("1111111111111111111\n");
	    if(strcmp(sGroups,"999999")==0){
					if(strlen(caGroupid)>0) {
						if(strcmp(caGroupid,"-1")){
					        	if(strcmp(caUser,"-1")==0){
							        	sprintf(caWhere," and a.id = %s",caGroupid);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
						}
			    }
			}else{
					        	//sprintf(caWhere," and a.groupid = %s",sGroups);
					        	if(strcmp(caUser,"-1")==0 ){
							        	sprintf(caWhere," and a.id in (%s)",sGroups);
							      }else{
							      		sprintf(caWhere," and b.userid = %s",caUser);
							      }
			}
			printf("2222222222222222222222\n");
			if(strcmp(sGroups,"999999")==0){
				sprintf(sqlBuf,"select id,name from msagroup");
			}else{
				sprintf(sqlBuf,"select id,name from msagroup where id in (%s) ",sGroups);
			}
	  
	    printf("3333333\n");
		  if(tflags){
		  	if(atol(tflags)!=0){
		  		sprintf(caWhere," and b.useflags = %s",tflags);
		  	}
		  }else{
		    tflags="0";
		    sprintf(caWhere," and b.useflags = %s",tflags);
		  }

			memset(caWhereU,0,sizeof(caWhereU));
			printf("4444444\n");
			if(strlen(caWhere)>0){
				ltEncode64(caWhere, strlen(caWhere), caWhereU, 512, &lWhere);
				for(lWhere=0;lWhere<strlen(caWhereU);lWhere++){
			                if(caWhereU[lWhere]=='+'){
			                        caWhereU[lWhere]='*';
			                }
			                if(caWhereU[lWhere]=='/'){
			                        caWhereU[lWhere]='#';
			                }
			  }
		  }
 
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"UTF-8");
	tablePtr=lt_dbput_table(dbPtr,"glist");
  if(strcmp(sGroups,"999999")==0){
				lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,"-1",
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,"所有");
	}
	
	printf("555555\n");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){						
				if(strcmp(gid,tempRow[0])==0){
									 lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,tempRow[1]);
			  }else{
								 		lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," ",
										"gname",LT_TYPE_STRING,tempRow[1]);
				}	
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	printf("666666\n");
	tablePtr=lt_dbput_table(dbPtr,"ulist");
  if(strcmp(gid,"-1")!=0){
    	sprintf(sqlBuf,"select userid,dispname from msauser where groupid0='%s' order by dispname ",gid);
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							while(tempRow){
								if(strcmp(caUser,tempRow[0])==0){
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," selected ");
								}else{
									lt_dbput_recordvars(tablePtr,3,
										"uid",LT_TYPE_STRING,tempRow[0],
										"uname",LT_TYPE_STRING,tempRow[1],
										"usel",LT_TYPE_STRING," ");
							  }
								tempRow= ltDbFetchRow(tempCursor);
							}
							ltDbCloseCursor(tempCursor);
			}
	}
	
  printf("	tflags:%s\n",tflags);
	if(tflags){
  	sprintf(caWhere,"tse%s",tflags);
  	lt_dbput_rootvars(dbPtr,1,caWhere," selected ");
  }
	
	if(strlen(caWhereU)>0){
  		sprintf(caWhere,"where=%s",caWhereU);
  		lt_dbput_rootvars(dbPtr,1,"where",caWhere);
  }
  
  printf("777777\n");
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msaimsearb.htm",dbPtr->head,0);	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

//关键字搜索
int ltmsasearchkeyquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;

    char *gid;
    int iii;
    int intGid,intGlevel;
    nasDbUserInfo *tmpdbuserinfo;
    char *glevel;

  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  glevel=ltMsgGetVar_s(ltMsgPk,"level"); 
   printf("gid:%s\n",gid);
  printf("glevel:%s\n",glevel);
  if(gid==NULL){
  	   gid="-1";
  	}
  
   intGlevel=atol(glevel);
   intGid=atol(gid);
   printf("intGid:%d\n",intGid);  
    printf("intGlevel:%d\n",intGlevel);
    
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");	
	if(intGid!=-1){
			printf("333333333333\n");
			tablePtr=lt_dbput_table(dbPtr,"ulist");
                 for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
				          tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii ); 
				          if(tmpdbuserinfo!=NULL){				          	
				          	    if(tmpdbuserinfo->groupid[intGlevel]==intGid){
							          lt_dbput_recordvars(tablePtr,3,
																"uid",LT_TYPE_LONG,tmpdbuserinfo->userid,
																"uname",LT_TYPE_STRING,tmpdbuserinfo->username,
																"usel",LT_TYPE_STRING," selected ");
																printf("tmpdbuserinfo->userid:%d\n",tmpdbuserinfo->userid);
																printf("tmpdbuserinfo->userid:%s\n",tmpdbuserinfo->username);
										printf("99999999999999\n");						
	                                  }
				          	
				          	}			       
                    }
			
			}
	 
    printf("44444444444\n");
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msasearchkeyhead.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
	return 0;
}


//关键字列表表单提交函数、查询、条件导出、删除
int ltmsasearchkeyhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
  
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    unsigned long lTime,L,lTime1;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
   // int intGlevel;
    int  iAction=0;
    
    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//删除条件
    char caWhere[512],xDate[30];
    
    char *caDtype=NULL; 
    //char *caGroupid=	NULL;
    char *caHost=NULL;
    char *caSdate=NULL;
    char *caEdate=NULL;
    char *searchkey=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *caDdd=NULL;
    char *caUnit=NULL;
    char *caBef=NULL;
    char *m_action=NULL;
    char *caSdate3=NULL;
    char *caStime=NULL;
    char *caEtime=NULL;
    char *caUser=NULL; 
    //char *glevel=NULL;

    char caTmpp[2048];
    stralloc strTemp;
    strTemp.s=0;
 
//    glevel=ltMsgGetVar_s(ltMsgPk,"level"); //部门级别
//    intGlevel=atol(glevel);
    
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     iAction=0;
    }else{
      iAction=atol(m_action);
    }
    printf("iAction=%d\n",iAction);
    
//    caGroupid=ltMsgGetVar_s(ltMsgPk,"groupid");//option value="[$gid$]"  name groupid部门名字
//    printf("caGroupid=%s\n",caGroupid);   
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); //option value="[$uid$]" name user
     printf("caUser=%s\n",caUser);
   
    caHost=ltMsgGetVar_s(ltMsgPk,"host");
    printf("caHost=%s\n",caHost);
    
    searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
    printf("searchkey=%s\n",searchkey);
    
    caDtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    printf("caDtype=%s\n",caDtype);
    
    caDdd=ltMsgGetVar_s(ltMsgPk,"ddd");
    printf("caDdd=%s\n",caDdd);
    caUnit=ltMsgGetVar_s(ltMsgPk,"unit");  // 天。周。月
    printf("caUnit=%s\n",caUnit);
    caBef=ltMsgGetVar_s(ltMsgPk,"bef"); //以内 以前
    printf("caBef=%s\n",caBef);
    
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    printf("Sdate=%s\n",caSdate);
    caEdate=ltMsgGetVar_s(ltMsgPk,"edate");
    printf("Edate=%s\n",caEdate);
    
    caSdate3=ltMsgGetVar_s(ltMsgPk,"sdate3");   //day 
    printf("Sdate3=%s\n",caSdate3);
    caStime=ltMsgGetVar_s(ltMsgPk,"stime");   //hour
    printf("caStime=%s\n",caStime);
    caEtime=ltMsgGetVar_s(ltMsgPk,"etime");
    printf("caEtime=%s\n",caEtime);
    
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // 排序方式
    printf("caOrder=%s\n",caOrder);
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //升 降
    printf("caOrderby=%s\n",caOrderby);
    
  
	  lRowNum=100; /*每页的行数*/
	  lSumRec=0;/*总行数*/
	  lCount=0; //
	  lStartRec=0;//
	  lNum=0; /*该页剩余的行数*/
	  
	  char Shour[32];//
	  char Ehour[32];//
	  unsigned int sTime3,eTime3,sTime,eTime;	//存储转换为整型的日期
	 sprintf(Shour,"%s:00:00",caStime);//开始小时
	 sprintf(Ehour,"%s:00:00",caEtime);//结束小时
	 sTime3= msaCvtLongTime(caSdate3,Shour);//将开始时间转换为整型
	 eTime3= msaCvtLongTime(caSdate3,Ehour);//将结束时间转换为整型
	 printf("sTime3=%u\n",sTime3);
	 printf("eTime3=%u\n",eTime3);

    //选择按从  某年某月某日 到   某年某月某日
	 sTime= msaCvtLongTime(caSdate,"00:00:00");//将开始时间转换为整型
	 eTime= msaCvtLongTime(caEdate,"23:00:00");//将结束时间转换为整型
	 printf("sTime=%u\n",sTime);
	 printf("eTime=%u\n",eTime);
	
		
if(ltMsgGetVar_s(ltMsgPk,"limit")){ lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));}
if(ltMsgGetVar_s(ltMsgPk,"start")){ lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));}
		
    memset(caWhere,0,sizeof(caWhere));
    memset(delWhere,0,sizeof(delWhere));
	
	  
    if(strcmp(caUser,"-1")==0){//没有选择部门
           sprintf(caWhere,"%s","2=2");
           sprintf(delWhere,"%s","2=2");
        }else{
            sprintf(caWhere,"b.userid = %s",caUser);//msasearchkey  为b表
            sprintf(delWhere,"userid = %s",caUser);
          }   
printf("1111:%s\n",caWhere);
printf("delWhere111:%s\n",delWhere);
    if(caHost!=NULL){//HOST关键字判断
	     if(strlen(caHost)>0) {//
	         sprintf(caWhere,"%s and (b.host like '%c%s%c') ",caWhere,'%',caHost,'%');
	         sprintf(delWhere,"%s and (host like '%c%s%c') ",delWhere,'%',caHost,'%');
	     }
    }
printf("222:%s\n",caWhere);
printf("delWhere222:%s\n",delWhere);
    if(searchkey!=NULL){//搜索关键字
	     if(strlen(searchkey)>0) {
	         sprintf(caWhere,"%s and (b.searchkey like '%c%s%c') ",caWhere,'%',searchkey,'%');
	         sprintf(delWhere,"%s and (searchkey like '%c%s%c') ",delWhere,'%',searchkey,'%');
	     }
    } 
printf("333:%s\n",caWhere);
printf("delWhere333:%s\n",delWhere);
    if(strcmp(caDtype,"unit") == 0) { /* 按天数计1天、周、月：以内、以前*/
	        lTime = time(0);//获取当前时间
	        printf("lTime=%u\n",lTime);
	        if(caDdd!=NULL){
	            if(strlen(caDdd)>0) { /*计算几天以内*/
			             L= atol(caDdd);         
			             if(caUnit[0] == '1') { /*天*/
			                 lTime1 = lTime - L* 86400L; 
			                 printf("lTime1:%u\n",lTime1);            
			             }else if(caUnit[0] == '2'){ /*周*/
			                 lTime1 = lTime - L* 7 * 86400L;          
			                 printf("lTime1:%u\n",lTime1);       
			             }else {//月
			                 lTime1 = lTime - L* 30 * 86400L;   
			                 printf("lTime1:%u\n",lTime1);               
			             }
			             nasTimeFormat(xDate,lTime1);
			             printf("xDate:%s\n",xDate);//10-12-16 18:14:41
			             xDate[8]='\0';//10-10-10 八位
			             unsigned int date1,date2;			            
			             date1= msaCvtLongTime(xDate,"00:00:00");
			             date2= msaCvtLongTime(xDate,"23:00:00");
			             printf("date1=%u\n",date1);
			             printf("date2=%u\n",date2);
			             if(caBef[0] == '1') { /* 以内 */
			                 sprintf(caWhere,"%s and  b.ltime >= %u",caWhere,date1);/* 以内 */
			                 sprintf(delWhere,"%s and ltime >= %u",delWhere,date1);/* 以内 */
			             }else { /* 以前  */
			                 sprintf(caWhere,"%s and b.ltime <= %u",caWhere,date2); /* 以前  */
			                 sprintf(delWhere,"%s and ltime<= %u",delWhere,date2); /* 以前  */
			             }         
	          } 
	      }           
    }else if(strcmp(caDtype,"unit3") == 0){ /* 时间范围在2010-12-08从00-23*/
		        if((caSdate3!=NULL) ){      
			             if(strlen(caSdate3)==8){
					              sprintf(caWhere," and b.ltime = %u",sTime3);//
					              sprintf(delWhere," and ltime = %u",sTime3);
					              if( strlen(caEtime)==2 && strlen(caStime)==2 ){
					               sprintf(caWhere," and b.ltime> %u",sTime3);
					               sprintf(caWhere," and b.ltime< %u",eTime3);
					               sprintf(delWhere," and ltime > %u",sTime3);
					               sprintf(delWhere," and ltime < %u",eTime3);//时间：00-23
					              }
			             }
		         }    
    }else if(strcmp(caDtype,"unit2") == 0){ /* 时间范围从2010-12-10到2010-12-20*/
    	      printf("unit2\n");
						     if( (caSdate!=NULL) && (caEdate!=NULL)){	                          							 
						         if(strlen(caSdate)>0) {
						             if(strlen(caSdate)==8) {
						              sprintf(caWhere," and b.ltime >= %u",sTime);
						              sprintf(delWhere," and ltime >= %u",sTime);
						              }
						         }
						         if(strlen(caEdate)>0) {
						              if(strlen(caEdate)==8) {
						              sprintf(caWhere," and b.ltime <= %u",eTime);
						              sprintf(delWhere," and ltime <= %u",eTime);
						              }
						         }
						   }
    }
printf("444:%s\n",caWhere);
printf("delWhere444:%s\n",delWhere);
   
    if(iAction==2){ /*del条件删除操作*/ 
    	 printf("delWhere:%s\n",delWhere);
         if( strlen(delWhere)>0){
		        sprintf(sqlBuf,"delete from msasearchkey where %s ",delWhere);
		        printf("sqlbuf:%s\n",sqlBuf);
		        ltDbExecSql(G_DbCon,sqlBuf);
          }  
          printf("wwwwwwwwwwww\n");
		    ltMsgPk->msgpktype=1;
		    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
		       "lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: the http log you select has been deleted! </body></html>");  
		    ltMsgFree(ltMsgPk);
		    printf("gggggggggggggggggg\n");
		    return 0;     
    }else if(iAction==1){/*export导出*/       
			       //int  srcfd;
			       char caBack[128];
			       int  lTime;
			       char sDate[36];
			       char caLine[8012];
			       memset(sDate,0,sizeof(sDate));
			       lTime = time(0);//获取当前时间
			       nasTimeFormat(sDate,lTime); 
			       sDate[8]='\0';
			       printf("%s\n",sDate);
			       sprintf(caBack,"%s-searchexport.csv",sDate);//文件路径
			       //sprintf(caBack,"/datacenter/msa/%s-searchexport.csv",sDate);//文件路径
			       //srcfd=open(caBack,O_WRONLY | O_EXCL | O_CREAT,0644);//open 文件包含多种模式包括例如O_WRONLY | O_EXCL | O_CREAT		       
			 ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caBack,"contenttype","Application/MS-DOWNLOAD");
//       printf("srcfd=%d\n",srcfd);
//		       if(srcfd<1){//不存在此文件
//				       ltMsgPk->msgpktype=1;
//				       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
//				       "lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: Can't open export file! </body></html>");  
//				        ltMsgFree(ltMsgPk);
//				        return 0;    
//		       }        
//	     ltMsgPk->msgpktype=1;
//	     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
//	       "lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: 导出正在进行中，请在管理-备份页面中观察导出进度并下载！ </body></html>");  
//	     ltMsgFree(ltMsgPk);
  
		 if(caOrder!=NULL){
			        if( strlen(caOrder)>0 ){
				         if(strcmp(caOrder,"nosort")!=0) {
						            if(strcmp(caOrder,"date")==0) {
						                sprintf(caWhere,"%s order by b.ltime ",caWhere);
						            }else if(strcmp(caOrder,"userid") == 0) {
						                sprintf(caWhere,"%s order by b.userid ",caWhere);
						             }
				             
						            if(strlen(caOrderby)) {
						                if(caOrderby[0] == '1') {   
						                   if(strcmp(caOrder,"date")==0) {
						                       ltStrAddF(caWhere," desc,b.ltime desc");
						                    }else{ 
						                       ltStrAddF(caWhere," desc ");
						                   }
						                }else{
						                    if(strcmp(caOrder,"date")==0) {
						                        ltStrAddF(caWhere," asc,b.ltime asc ");
						                    }else{ 
						                        ltStrAddF(caWhere," asc "); 
						                    }
						                } 
						            }
				         }
			       }   
		       } 
		 printf("caWhere:%s\n",caWhere);       
		sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey from msasearchkey b left join msauser a on a.userid=b.userid where %s limit %d",caWhere,50000);	 
	     printf("sqlbuf:%s\n",sqlBuf);
	     tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	     if(tempCursor!=NULL){
	     tempRow=ltDbFetchRow(tempCursor);
		   while(tempRow!=NULL){
		   	             memset(xDate,0,sizeof(xDate));
				        nasTimeFormat(xDate,tempRow[2]);
                        printf("xDate:%s\n",xDate);//10-12-16 18:14:41
						sprintf(caLine,"%s,%s,%s,%s,%s\n",tempRow[0],tempRow[1],xDate,tempRow[3]); 
						 printf("tempRow[0]=%s\n",tempRow[0]);
						printf("tempRow[1]=%s\n",tempRow[1]);
						printf("atol(tempRow[2])=%d\n",atol(tempRow[2]));
						printf("tempRow[3]=%s\n",tempRow[3]);
						
						write(confd,caLine,strlen(caLine));				
						//write(srcfd,caLine,strlen(caLine));//写到文件          
						tempRow= ltDbFetchRow(tempCursor);
		   }
	     ltDbCloseCursor(tempCursor);
	   }
	   // close(srcfd);  
	    return 0;      
   }
    

	  sprintf(strBuf,"select count(*) from msasearchkey b left join msauser a on a.userid=b.userid where %s",caWhere);      
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
      
printf("5555:%s\n",caWhere);  
	if(caOrder!=NULL){
				     if( strlen(caOrder)>0 ){
					      if(strcmp(caOrder,"nosort")!=0) {
							         if(strcmp(caOrder,"date")==0) {
							                sprintf(caWhere,"%s order by b.ltime ",caWhere);
							                printf("aaaaa:%s\n",caWhere);
							         }else if(strcmp(caOrder,"userid") == 0){
							                sprintf(caWhere,"%s order by b.userid ",caWhere);
							                 printf("bbbb:%s\n",caWhere);
							         }
							         
							         if(strlen(caOrderby)){
								                if(caOrderby[0] == '1') {   
								                   if(strcmp(caOrder,"date")==0) {
								                      ltStrAddF(caWhere,"desc,b.ltime desc");
								                       printf("cccccc:%s\n",caWhere);
								                    }else{
								                    	 ltStrAddF(caWhere,"desc");
								                    	  printf("tttttt:%s\n",caWhere);
								                    } 
								                }else{
								                     if(strcmp(caOrder,"date")==0) {
								                        ltStrAddF(caWhere," asc,b.ltime asc");
								                         printf("eeeeeeee:%s\n",caWhere);
								                     }else{
								                	    ltStrAddF(caWhere,"asc");
								                	     printf("ggggg:%s\n",caWhere);
								                	}   
								                } 
							         }
					         
					     }
				    }   
		    } 		    

 
printf("6666:%s\n",caWhere);
//json生成
	sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
   
    sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey from msasearchkey b left join msauser a on (a.userid =b.userid) where %s limit %lu offset %lu",caWhere,lRowNum,lStartRec);
    printf("sqlBuf=%s\n",sqlBuf);
    	
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		   tempRow= ltDbFetchRow(tempCursor);
		   	while(tempRow!=NULL){
		   		printf("5555555555\n");
					  lNum++;
						if(lNum==1){
							        memset(xDate,0,sizeof(xDate));
							        nasTimeFormat(xDate,tempRow[2]);
			                        printf("xDate:%s\n",xDate);//10-12-16 18:14:41
								  sprintf(caTmpp,"{\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\"}",
														    tempRow[0],
															tempRow[1],
															xDate,//atol(tempRow[2]),
															tempRow[3]);
															printf("tempRow[0]=%s\n",tempRow[0]);
															printf("tempRow[1]=%s\n",tempRow[1]);
															printf("atol(tempRow[2])=%d\n",atol(tempRow[2]));
															printf("tempRow[3]=%s\n",tempRow[3]);
								  stralloc_cats(&strTemp,caTmpp);					
						}else{
							printf("77777777777\n");
							       memset(xDate,0,sizeof(xDate));
							       nasTimeFormat(xDate,tempRow[2]);
			                        printf("xDate:%s\n",xDate);//10-12-16 18:14:41
							    sprintf(caTmpp,"{\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\"}",
															tempRow[0],
															tempRow[1],
															xDate,//atol(tempRow[2]),
															tempRow[3]);	
														   printf("tempRow[0]=%s\n",tempRow[0]);
															printf("tempRow[1]=%s\n",tempRow[1]);
															printf("atol(tempRow[2])=%d\n",atol(tempRow[2]));
															printf("tempRow[3]=%s\n",tempRow[3]);
								  stralloc_cats(&strTemp,caTmpp);
						 }
						tempRow= ltDbFetchRow(tempCursor);
			}
		 ltDbCloseCursor(tempCursor);
		}
		stralloc_cats(&strTemp,"]}");	
		stralloc_0(&strTemp);
		printf("mmmmmmmmmmmmmmm\n");		
		
		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);
		
		return 0; 
		
		
		
}