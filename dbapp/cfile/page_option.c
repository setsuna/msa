#include <stdlib.h>
#include <unistd.h>
#include <signal.h>  
#include <stdio.h>  
#include <fcntl.h>
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
#include <string.h>

#include "nas.h"
  
char dbName[64];

char dbUser[64];

char dbPass[64];
 

/*到定义模板页面*/



int toreporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
  ltDbCursor *tempCursor;

  LT_DBROW tempRow;

  char sqlBuf[1024];  

  ltDbHeadPtr dbPtr;

  ltTablePtr tablePtr;

  ltDbConn *G_DbCon;

  sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));

  sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));

  sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));

  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);

  if (G_DbCon==NULL){

		printf("report:can't open db\n");
		_exit(0);

 }
 
  char *gid;
 
  gid=ltMsgGetVar_s(ltMsgPk,"gid");
 
  if(gid==NULL){

  	gid="-1";

  }



  dbPtr=lt_dbinit();

  sprintf(sqlBuf,"select groupid,groupname from nasgroup ");

  tablePtr=lt_dbput_table(dbPtr,"glist");

  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

		if(tempCursor!=NULL){


							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){
 
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

  	  sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);

      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

	  if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

								    lt_dbput_recordvars(tablePtr,2,

									"uid",LT_TYPE_STRING,tempRow[0],

									"uname",LT_TYPE_STRING,tempRow[1]);

								    tempRow= ltDbFetchRow(tempCursor);

							}

							ltDbCloseCursor(tempCursor);

			}

}

  //lt_dbput_rootvars(dbPtr,2,"sdate",ltMsgGetVar_s(ltMsgPk,"sdate"),"edate",ltMsgGetVar_s(ltMsgPk,"edate"));
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportstylesetting.htm",dbPtr->head,0);
 
  ltMsgFree(ltMsgPk);
 
  lt_dbfree(dbPtr);
  


  return 0;

}



/*
 
接收页面的数据 保存到文件 
 
*/



int reporttemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
  char caLabel[256];
 
  char *depart;
  
  char *employee;
  char *topusernum;
  char reportname[40];
 
  int  gid;

  char *ppp;

  int  userid;
 
  char element[50];

  char checkb[10];
  
  int deployscope;

  char templname[100];     
 
  char caTempDir[123];

  long tempindex;
  
  char temptype[256];

  char tempdir[64];

  char email[64];

  char language[4];
  
  char createDate[36];

  long lTime;
 
  tempindex=time(0);
 
  depart=ltMsgGetVar_s(ltMsgPk,"groupid");
 
  employee=ltMsgGetVar_s(ltMsgPk,"user");
  topusernum=ltMsgGetVar_s(ltMsgPk,"topusernum");
  userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
 
  gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
 
  if( ltMsgGetVar_s(ltMsgPk,"templname")==NULL){
 
     sprintf(templname,"%s","");
 
  }else{
 
     ppp=ltMsgGetVar_s(ltMsgPk,"templname");

     memset(templname,0,sizeof(templname));

     strutf82gb(ppp,templname,strlen(ppp));
 
  } 
 
  printf("templname:%s\n",templname);
  
  if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
 
		sprintf(reportname,"%s","");
 
  }else{
 
        sprintf(reportname,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
 
  } 
 

  memset(element,0,sizeof(element)); memset(checkb,0,sizeof(checkb));
 
  memset(element,'0',50);            memset(checkb,'0',10);
 
  sprintf(element,ltMsgGetVar_s(ltMsgPk,"element"));

  sprintf(checkb,ltMsgGetVar_s(ltMsgPk,"checkb"));
 
  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
 
       sprintf(email,"%s",""); 
 
  }else{
 
       sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
 
  }

 memset(language,0,sizeof(language));

 sprintf(language,"%s",ltMsgGetVar_s(ltMsgPk,"language"));
 
 printf("depart:%s\n",depart);

 printf("employee:%s\n",employee);

 printf("element:%s\n",element);

 printf("checkb:%s\n",checkb);

 printf("templname:%s\n",templname);printf("email:%s\n",email);
 printf("language:%s\n",language);
 
 

 lTime = time(0);

 nasTimeGetDate(createDate,lTime);
 
 deployscope=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope"));
 

 if(deployscope==0){//每天生成报告
 
   sprintf(caTempDir,"/app/ns/conlog/report/page_data/%ld/",tempindex);  

   sprintf(temptype,"每天自动生成报告");

   sprintf(tempdir,"dailyreport");
 
   if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {

          /// 生成该报告目录错误 

          printf("mkdir daily report dir error!\n");

          return 0;
 
         }
 
   }  
 
  sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
  
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'reportname=%s' >> %sinfo.ini",reportname,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);
  
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",0,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
  
  system(caLabel);

  sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
  system(caLabel);
  
 
 }if(deployscope==1){//每周 
  
	  sprintf(caTempDir,"/app/ns/conlog/report/page_data/%ld/",tempindex); 

	  sprintf(temptype," 每周自动生成报告");

	  sprintf(tempdir,"dailyreport");
 
      if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {

          /// 生成该报告目录错误 

          printf("mkdir daily report dir error!\n");

          return 0;

         }

       } 
 
  sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
 
  system(caLabel);  
 
  sprintf(caLabel,"/bin/echo 'reportname=%s' >> %sinfo.ini",reportname,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);
  
  system(caLabel); 
  
  sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);
  
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);
  
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",1,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
 
  system(caLabel);

  sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
  system(caLabel);
 
 }if(deployscope==2){//立即生成 
 
   sprintf(temptype,"手动生成报告");

   sprintf(tempdir,"instantreport");
 
   sprintf(caTempDir,"/app/ns/conlog/report/page_data/%ld/",tempindex); 

  
   if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
        /// 生成该报告目录错误 
          printf("mkdir daily report dir error!\n");

          return 0;

         }

    } 

   sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);

   system(caLabel);    

   sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",2,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  
   system(caLabel);
   
   sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
   system(caLabel);

 } 
 
 // toreporttemplate(confd,ltMsgPk,lt_MMHead);

 ltDbHeadPtr dbPtr;
 
 dbPtr=lt_dbinit();
 
 lt_db_htmlpage(dbPtr,"gb2312");
 
 lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportstylesetting.htm",dbPtr->head,0);
 
 ltMsgFree(ltMsgPk); 
  
 return 0;
 
}



int ccmps(nasDir* a,nasDir* b)

{

   int myret;

   myret=a->dirTime-b->dirTime;

   if(myret!=0){
 		return myret;	
   }

   return strcmp(a->caDirName,b->caDirName);

} 

 



/*显示列表*/



int instantreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	ltDbHeadPtr dbPtr;
 
	ltTablePtr tablePtr;
 
	char caFile1[256];
 
 	char cDate[128];
 
	char reportname[64];
 
	char reportuser[64];
 
    struct stat st;
 
    char   *reportdir;
 
    int len=0; //sum of file names 
 
    nasDir *fnames;
 
//if(checkRight(confd,ltMsgPk,13,lt_MMHead)==-1){
 
//		return -1;
 
//	}
 
    reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
 
	dbPtr=lt_dbinit();
 
	lt_db_htmlpage(dbPtr,"gb2312");
 
	tablePtr=lt_dbput_table(dbPtr,"list");
 
	sprintf(caFile1,"/app/ns/conlog/report/%s/",reportdir); 
 
    fnames=mydirlist(caFile1,&len);
 
    if(fnames){
 
 	   qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序
 
    }
 
    while(len--) //free all allocated memory blocks

   {

 	      //printf("%s\n",fnames[len].caDirName);
 
						memset(reportname,0,sizeof(reportname));
 
					    memset(reportuser,0,sizeof(reportuser));
 
					    memset(cDate,0,sizeof(cDate));
 
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
 
					    bcCnfGetValue_s(caFile1,"date",cDate);
 
					    bcCnfGetValue_s(caFile1,"name",reportname);
 
					    bcCnfGetValue_s(caFile1,"user",reportuser);
 
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
 
					    if(stat(caFile1,&st) == 0){
 
					    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
 
					    	lt_dbput_recordvars(tablePtr,4,
 
								"reportname",LT_TYPE_STRING,reportname,
 
								"reportstate",LT_TYPE_STRING,caFile1,
 
								"bfile",LT_TYPE_STRING,fnames[len].caDirName,
 
								"reportdir",LT_TYPE_STRING,reportdir);
 
								continue;
 
					    }
 
					    lt_dbput_recordvars(tablePtr,4,
 
								"reportname",LT_TYPE_STRING,reportname,
 
								"reportstate",LT_TYPE_STRING,cDate,
 
								"bfile",LT_TYPE_STRING,fnames[len].caDirName,
 
								"reportdir",LT_TYPE_STRING,reportdir);
  
 }
 
    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportlist-instant.htm",dbPtr->head,0);
 
	if(fnames){

 		free(fnames);

    }
  
	ltMsgFree(ltMsgPk);
 
	lt_dbfree(dbPtr);
 
	return 0;
 
}





/*删除*/



int inreportdel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	char  caCmd[256];

	char   *reportdir;

	char   *filedir;

    int  typ;

	ltDbHeadPtr dbPtr;
 
    dbPtr=lt_dbinit();

    reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");

    filedir=ltMsgGetVar_s(ltMsgPk,"file");
 
    if(reportdir && filedir){

 
	    if(strlen(reportdir)>1 && strlen(filedir)>1){
 
			sprintf(caCmd,"rm -Rf /app/ns/conlog/report/%s/%s",reportdir,filedir);
 
			system(caCmd);
 
	    }else{
 
			ltMsgPk->msgpktype=1;
 
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");		
 
	   }
 
	}else{
 
			ltMsgPk->msgpktype=1;
 
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");		
 
   }

 

 //  instantreportlist(confd,ltMsgPk,lt_MMHead);

//toreportlist(confd,ltMsgPk,lt_MMHead);

 

 typ=atoi(ltMsgGetVar_s(ltMsgPk,"typ")); //printf("...%d\n",typ);

 if(typ==0){  //每天报告
 
   lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/dailyreport.htm",dbPtr->head,0);
 

 }if(typ==1){  //每天汇总
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/daygroupreport.htm",dbPtr->head,0);
 

 }if(typ==2){  //计费
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/jfreportlist.htm",dbPtr->head,0);
 
 }if(typ==3){  //立即生成 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportlist-instant.htm",dbPtr->head,0);
 

 }if(typ==4){  //每周 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/weekreportlist.htm",dbPtr->head,0);
 

 }if(typ==5){  //每日 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/dayreportlist.htm",dbPtr->head,0);
 

 }if(typ==6){  //网络流量 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/connectflowlist.htm",dbPtr->head,0);
 

 } if(typ==7){  //网络次数 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/connectreportlist.htm",dbPtr->head,0);
 

 }if(typ==8){  //出口流量 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/nsoutflowlist.htm",dbPtr->head,0);

  
 }if(typ==9){  //服务协议 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/srvreportlist.htm",dbPtr->head,0);
 

 }if(typ==10){  //用户时间 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/usertimereportlist.htm",dbPtr->head,0);
 
 }if(typ==11){  //访问拦截 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/webblockreportlist.htm",dbPtr->head,0);
 

 }if(typ==12){  //模板 
 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reporttemplatelist.htm",dbPtr->head,0);
 
 } 

   ltMsgFree(ltMsgPk);

   lt_dbfree(dbPtr);

   return 0;

 

}





/*到报告页面，查询部门|人员信息到下拉框*/

int toreports(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)

{
 
  ltDbCursor *tempCursor;

  LT_DBROW tempRow;

  char sqlBuf[1024]; 

  ltDbHeadPtr dbPtr;

  ltTablePtr tablePtr;

  char *gid;
 

  gid=ltMsgGetVar_s(ltMsgPk,"gid");



  if(gid==NULL){

  	gid="-1";

  }
 
  dbPtr=lt_dbinit();
 
  sprintf(sqlBuf,"select groupid,groupname from nasgroup ");
 
  tablePtr=lt_dbput_table(dbPtr,"glist");

		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

		if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){
 
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
 
  	sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);
 
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

			if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

								lt_dbput_recordvars(tablePtr,2,
 

									"uid",LT_TYPE_STRING,tempRow[0],
 

									"uname",LT_TYPE_STRING,tempRow[1]);
 

								tempRow= ltDbFetchRow(tempCursor);

							}

							ltDbCloseCursor(tempCursor);

			}

 
  }

		 
  lt_dbput_rootvars(dbPtr,2,"sdate",ltMsgGetVar_s(ltMsgPk,"sdate"),"edate",ltMsgGetVar_s(ltMsgPk,"edate"));



  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportmain2.htm",dbPtr->head,0);



  ltMsgFree(ltMsgPk);



  lt_dbfree(dbPtr);



  return 0;



}



//查询对应部门的人员 

 int selectPer(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)

{



  ltDbCursor *tempCursor;

  LT_DBROW tempRow;

  char sqlBuf[1024];

  ltDbHeadPtr dbPtr;

  ltTablePtr tablePtr;

  char *gid;

  int type;

  

  type=atoi(ltMsgGetVar_s(ltMsgPk,"gid"));

  gid=ltMsgGetVar_s(ltMsgPk,"gid");

  if(gid==NULL){

  	gid="-1";

  }

 

  dbPtr=lt_dbinit();

  sprintf(sqlBuf,"select groupid,groupname from nasgroup ");



  if(type==1){

	 tablePtr=lt_dbput_table(dbPtr,"glist1");

		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

		if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){



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





	tablePtr=lt_dbput_table(dbPtr,"ulist1");

  

    if(strcmp(gid,"-1")!=0){



  	  sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);



      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

			if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

								lt_dbput_recordvars(tablePtr,2,



									"uid",LT_TYPE_STRING,tempRow[0],



									"uname",LT_TYPE_STRING,tempRow[1]);



								tempRow= ltDbFetchRow(tempCursor);

							}

							ltDbCloseCursor(tempCursor);

			}

		}

   }if(type==2){

		tablePtr=lt_dbput_table(dbPtr,"glist2");

		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

		if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){
 
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





  	tablePtr=lt_dbput_table(dbPtr,"ulist2"); 

    if(strcmp(gid,"-1")!=0){

  	  sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);
 
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

			if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

								lt_dbput_recordvars(tablePtr,2,

									"uid",LT_TYPE_STRING,tempRow[0],

									"uname",LT_TYPE_STRING,tempRow[1]);

								tempRow= ltDbFetchRow(tempCursor);

							}

							ltDbCloseCursor(tempCursor);

			}

		}



   }if(type==3){

		 tablePtr=lt_dbput_table(dbPtr,"glist3");

		 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   

		 if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

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



	  tablePtr=lt_dbput_table(dbPtr,"ulist3");
 
      if(strcmp(gid,"-1")!=0){
 
  	    sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);
 
        tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);

		if(tempCursor!=NULL){

							tempRow= ltDbFetchRow(tempCursor);

							while(tempRow){

								lt_dbput_recordvars(tablePtr,2,
 
									"uid",LT_TYPE_STRING,tempRow[0],
 
									"uname",LT_TYPE_STRING,tempRow[1]);

 
								tempRow= ltDbFetchRow(tempCursor);

							}

							ltDbCloseCursor(tempCursor);

		}
 
		}

  }

 

  lt_dbput_rootvars(dbPtr,2,"sdate",ltMsgGetVar_s(ltMsgPk,"sdate"),"edate",ltMsgGetVar_s(ltMsgPk,"edate"));

 
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportmain3.htm",dbPtr->head,0);
 
  ltMsgFree(ltMsgPk);

 
  lt_dbfree(dbPtr);
 
  return 0;



}



int toreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	ltDbHeadPtr dbPtr;

	ltTablePtr tablePtr;

	char caFile1[256];

 	char cDate[128];

    char flowInfo[30000],flowInfodata[3000];

	char reportname[64];

	char reportuser[64];char reportstate[123];

    struct stat st;

    char   *reportdir;

	int len=0; //sum of file names 

	nasDir *fnames;

	char   *filedir;

	char  caCmd[256];



//if(checkRight(confd,ltMsgPk,13,lt_MMHead)==-1){

//		return -1;

//	}
 
   reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
 
   if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){

		filedir=ltMsgGetVar_s(ltMsgPk,"file");

		sprintf(caCmd,"rm -Rf /app/ns/conlog/report/%s/%s",reportdir,filedir);

        system(caCmd);

	}
 

	dbPtr=lt_dbinit();
 
	lt_db_htmlpage(dbPtr,"gb2312");
 
	tablePtr=lt_dbput_table(dbPtr,"list");
 
	sprintf(caFile1,"/app/ns/conlog/report/%s/",reportdir);

	printf("caFile1::%s\n",caFile1);  

  
    fnames=mydirlist(caFile1,&len);
 
    if(fnames){

	    qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

    }
 
    sprintf(flowInfo,"[ ");  // printf("len::%d\n",len);
 
     while(len--) //free all allocated memory blocks

    {

			     //	printf("%s\n",fnames[len].caDirName);
 
						memset(reportname,0,sizeof(reportname));
 
					    memset(reportuser,0,sizeof(reportuser));
 
					    memset(cDate,0,sizeof(cDate));
 
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
  
					    bcCnfGetValue_s(caFile1,"date",cDate);
 
					    bcCnfGetValue_s(caFile1,"name",reportname);
 
					    bcCnfGetValue_s(caFile1,"user",reportuser);
 
						sprintf(reportstate,"报告已生成");
 
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
 
					    if(stat(caFile1,&st) == 0){
 
					    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
 
							sprintf(reportstate,caFile1);

					    }  
 
					    sprintf(caFile1,"/app/ns/conlog/report/%s/%s/proccess.ini",reportdir,fnames[len].caDirName);

					    if(stat(caFile1,&st) == 0){
 
								sprintf(reportstate,"报告正在生成中");
 
					    }
 
					    lt_dbput_recordvars(tablePtr,4,
 
								"reportname",LT_TYPE_STRING,reportname,
 
								"reportstate",LT_TYPE_STRING,cDate,
  
								"bfile",LT_TYPE_STRING,fnames[len].caDirName,
 
								"reportdir",LT_TYPE_STRING,reportdir);
 
                        sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,reportdir,fnames[len].caDirName,reportstate);

                        //    printf("%s\n",flowInfodata);                

						sprintf(flowInfodata,"%s,",flowInfodata);
 
						strcat(flowInfo,flowInfodata);
 
 }

if(len!=0){
  flowInfo[strlen(flowInfo)-1]='\0'; 
}
 strcat(flowInfo,"]");

 printf("%s\n",flowInfo);  
 
 ltMsgPk->msgpktype=1;
 
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\n","lthtml",LT_TYPE_STRING,flowInfo);
 
 if(fnames){
 
 	free(fnames);

 }
 
 ltMsgFree(ltMsgPk);
 
 lt_dbfree(dbPtr);
 
 return 0;
 
}



int toreporttemplatelist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){



char caFile1[256];

ltDbHeadPtr dbPtr;

ltTablePtr tablePtr;

nasDir *fnames;

int len=0;

char tempdir[128],tempdirs[128];

char flowInfo[30000],flowInfodata[3000];

char reportname[64];

char reporttype[64];

char reportdir[64];

char createDate[36];

char typ[10],typs[10];

struct stat st;

char   *filedir;

char  caCmd[256];

int    iPid;

char **binqqargs;

 
    filedir=ltMsgGetVar_s(ltMsgPk,"file");
 
	dbPtr=lt_dbinit();
 
	lt_db_htmlpage(dbPtr,"gb2312");

 

	tablePtr=lt_dbput_table(dbPtr,"list");
 

    if((ltMsgGetVar_s(ltMsgPk,"filed"))!=NULL && (ltMsgGetVar_s(ltMsgPk,"reportdir"))!=NULL){   //delete reports of the template

		filedir=ltMsgGetVar_s(ltMsgPk,"filed");

		sprintf(caCmd,"rm -Rf /app/ns/conlog/report/%s/%s",ltMsgGetVar_s(ltMsgPk,"reportdir"),filedir);

        system(caCmd);

        sleep(1);

    }

    if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){    //delete

		filedir=ltMsgGetVar_s(ltMsgPk,"file");

		sprintf(caCmd,"rm -Rf /app/ns/conlog/report/page_data/%s",filedir);

        system(caCmd);

        sleep(1);

    }
 
/*	if((ltMsgGetVar_s(ltMsgPk,"typ"))!=NULL && (ltMsgGetVar_s(ltMsgPk,"tempDir"))!=NULL){

			    sprintf(typs,ltMsgGetVar_s(ltMsgPk,"typ"));

			    sprintf(tempdirs,ltMsgGetVar_s(ltMsgPk,"tempDir"));

				printf("type:: %s,tempindex:: %s\n",typs,tempdirs);  

				chdir("/app/ns/bin");

 
				iPid=fork();
 
				if(iPid==0){

				binqqargs = (char **) malloc(2 * sizeof(char *));

				binqqargs[0]= "dailyreporttemplate";

				binqqargs[1]= typs; 

						 

				binqqargs[2]= tempdirs; 

					  	 

				binqqargs[3]= 0;

					  	  	  

				execv(*binqqargs,binqqargs); 

						 

				die(0);

				}

				wait(NULL); 

 

			   printf("report is ok!!!\n");



  }*/

  

  

  sprintf(reportdir,"/app/ns/conlog/report/page_data/");



  sprintf(caFile1,"/app/ns/conlog/report/page_data/");



  fnames=mydirlist(caFile1,&len);



  if(fnames){

 	  qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

  }



  memset(flowInfo,0,sizeof(flowInfo));

  sprintf(flowInfo,"[ ");

  while(len--) //free all allocated memory blocks 

 {

						memset(reportname,0,sizeof(reportname));

					    memset(reporttype,0,sizeof(reporttype));

					    memset(typ,0,sizeof(typ));

						memset(tempdir,0,sizeof(tempdir));



						sprintf(caFile1,"/app/ns/conlog/report/page_data/%s/info.ini",fnames[len].caDirName);

					    bcCnfGetValue_s(caFile1,"tempdir",tempdir);

					    bcCnfGetValue_s(caFile1,"templname",reportname);

					    bcCnfGetValue_s(caFile1,"temptype",reporttype);

					    bcCnfGetValue_s(caFile1,"type",typ);

						bcCnfGetValue_s(caFile1,"createDate",createDate);



						sprintf(caFile1,"/app/ns/conlog/report/page_data/%s/errmsg.ini",fnames[len].caDirName);

					    if(stat(caFile1,&st) == 0){



					    	sprintf(caFile1,"<a href=\"/report/page_data/%s/errmsg.ini\">Error:生成报告错误!</a>",fnames[len].caDirName);

					    	lt_dbput_recordvars(tablePtr,6,

										"reportname",LT_TYPE_STRING,reportname,

										 "tempdir",LT_TYPE_STRING,tempdir,

										"tempInfo",LT_TYPE_STRING,caFile1,

										"bfile",LT_TYPE_STRING,fnames[len].caDirName,

										"reportdir",LT_TYPE_STRING,reportdir,

										"typ",LT_TYPE_STRING,typ 

							);

							continue;

					    }  
 

					    lt_dbput_recordvars(tablePtr,6,

								"reportname",LT_TYPE_STRING,reportname,

								"tempdir",LT_TYPE_STRING,tempdir,

								"tempInfo",LT_TYPE_STRING,reporttype,

								"bfile",LT_TYPE_STRING,fnames[len].caDirName,

								"reportdir",LT_TYPE_STRING,reportdir,

								"typ",LT_TYPE_STRING,typ 

						);

						sprintf(flowInfodata,"['%s','%s','%s','%s','%s','%s','%s']",reportname,reporttype,reportdir,fnames[len].caDirName,tempdir,typ,createDate); 

						sprintf(flowInfodata,"%s,",flowInfodata);
  
						strcat(flowInfo,flowInfodata);  

					} 
					if(len!=0){
					  flowInfo[strlen(flowInfo)-1]='\0'; 
					}

                    strcat(flowInfo,"]");

       printf("flowInfodata:%s\n",flowInfodata);
 

     // lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reporttemplatelist.htm",dbPtr->head,0);

     ltMsgPk->msgpktype=1;

	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\n","lthtml",LT_TYPE_STRING,flowInfo);
 

	 if(fnames){

 		free(fnames);

     }
  
	ltMsgFree(ltMsgPk);

	lt_dbfree(dbPtr);
 
    return 0;

}
   

//某模板下的所有报告

int templatereportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	ltDbHeadPtr dbPtr;

	ltTablePtr tablePtr; 

	char caFile1[256];

 	char cDate[128];

    char flowInfo[30000],flowInfodata[3000];

	char reportname[64];

	char reportuser[64];char reportstate[123];

    struct stat st;

    char   *reportdir;

    char   *reportdir2;

    int len=0; //sum of file names 

    nasDir *fnames;

    char   *filedir;

    char  caCmd[256];
 
    filedir=ltMsgGetVar_s(ltMsgPk,"file");

    reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir"); 
 
	if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){                    //delete

		filedir=ltMsgGetVar_s(ltMsgPk,"file");

		sprintf(caCmd,"rm -Rf /app/ns/conlog/report/%s/%s",reportdir,filedir);

        system(caCmd);
 
	}
 

	dbPtr=lt_dbinit();

	lt_db_htmlpage(dbPtr,"gb2312");

	tablePtr=lt_dbput_table(dbPtr,"list");

	sprintf(caFile1,"/app/ns/conlog/report/%s/",reportdir);

    printf("caFile1%s\n",caFile1);

    fnames=mydirlist(caFile1,&len);

 

   if(fnames){

 	  qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

   }

 

   sprintf(flowInfo,"[ ");         printf("len::%d\n",len);
   


/*  if(len==0){

	 strcat(flowInfo,"[' ',' ',' ',' ',' '] ");

  }else{

*/

   while(len--) //free all allocated memory blocks

   {

			       	printf("%s\n",fnames[len].caDirName);

	   				    memset(reportname,0,sizeof(reportname));
 
					    memset(reportuser,0,sizeof(reportuser));
 
					    memset(cDate,0,sizeof(cDate));
  
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);
 
					    bcCnfGetValue_s(caFile1,"date",cDate);
 
					    bcCnfGetValue_s(caFile1,"name",reportname);
  
					    bcCnfGetValue_s(caFile1,"user",reportuser);
 
						sprintf(reportstate,"报告已生成");
 
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);
 
					    if(stat(caFile1,&st) == 0){
  
					    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);

							sprintf(reportstate,caFile1);
  
					    }
 

						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/proccess.ini",reportdir,fnames[len].caDirName);

					    if(stat(caFile1,&st) == 0){
 
								sprintf(reportstate,"报告正在生成中");
 
					    }



					    lt_dbput_recordvars(tablePtr,4,
 
								"reportname",LT_TYPE_STRING,reportname,
 
								"reportstate",LT_TYPE_STRING,cDate,
 
								"bfile",LT_TYPE_STRING,fnames[len].caDirName,
 
								"reportdir",LT_TYPE_STRING,reportdir);
 

                      sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,reportdir,fnames[len].caDirName,reportstate);
 
					  sprintf(flowInfodata,"%s,",flowInfodata);
 
					  strcat(flowInfo,flowInfodata);
 
  } 

//  } 
if(len!=0){
  flowInfo[strlen(flowInfo)-1]='\0'; 
}
 strcat(flowInfo,"]"); 
  

//  printf("%s\n",flowInfo);

// printf("==========%d\n",strlen(flowInfo));

// printf("==========%s\n",flowInfo[strlen(flowInfo)-1]);

    printf("flowInfo===%s\n",flowInfo);	 
 
	ltMsgPk->msgpktype=1;
 
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\n","lthtml",LT_TYPE_STRING,flowInfo);
  
  	if(fnames){ 
 		 free(fnames); 
    }
 
	ltMsgFree(ltMsgPk);
 
	lt_dbfree(dbPtr);
 
	return 0;

}





int templatereportlista(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

 

	ltDbHeadPtr dbPtr;

	ltTablePtr tablePtr;

	char caFile1[256];

 	char cDate[128];

    char flowInfo[30000],flowInfodata[3000];

	char reportname[64];

	char reportuser[64];

    struct stat st;

    char   *reportdir;

    char   *reportdir2;

    int len=0; //sum of file names 

    nasDir *fnames;

 

    reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir"); 

 

	dbPtr=lt_dbinit();



	lt_db_htmlpage(dbPtr,"gb2312");

 

	tablePtr=lt_dbput_table(dbPtr,"list");



	sprintf(caFile1,"/app/ns/conlog/report/%s/",reportdir);



    printf("caFile1%s\n",caFile1);



    fnames=mydirlist(caFile1,&len);



   if(fnames){

 	 qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

   }

 

   printf("len:::::%d\n",len);

   while(len--) //free all allocated memory blocks

   {

			//	printf("%s\n",fnames[len].caDirName);



						memset(reportname,0,sizeof(reportname));



					    memset(reportuser,0,sizeof(reportuser));



					    memset(cDate,0,sizeof(cDate));



						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/info.ini",reportdir,fnames[len].caDirName);



					    bcCnfGetValue_s(caFile1,"date",cDate);



					    bcCnfGetValue_s(caFile1,"name",reportname);



					    bcCnfGetValue_s(caFile1,"user",reportuser);



						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/errmsg.ini",reportdir,fnames[len].caDirName);



					    if(stat(caFile1,&st) == 0){



					    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);



					    	lt_dbput_recordvars(tablePtr,4,



								"reportname",LT_TYPE_STRING,reportname,



								"reportstate",LT_TYPE_STRING,caFile1,



								"bfile",LT_TYPE_STRING,fnames[len].caDirName,



								"reportdir",LT_TYPE_STRING,reportdir);



								continue;

					     }

 

					    lt_dbput_recordvars(tablePtr,4,
 

								"reportname",LT_TYPE_STRING,reportname,
 

								"reportstate",LT_TYPE_STRING,cDate,

 
								"bfile",LT_TYPE_STRING,fnames[len].caDirName,
 
								"reportdir",LT_TYPE_STRING,reportdir);
 

   }  

	   lt_dbput_rootvars(dbPtr,1,"reportdirs",reportdir);
 
	   lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/templatereportlist.htm",dbPtr->head,0);
  
	   ltMsgFree(ltMsgPk);
   
	   lt_dbfree(dbPtr);
 
	   return 0;

}



/////系统自动生成报告列表

int toautoreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	char caFile1[256];
 	char cDate[128];
  char flowInfo[30000],flowInfodata[3000];
	char reportname[64];
	char reportuser[64];char reportstate[123];
    struct stat st;
    char   *reportdir;
    int len=0,i,j=0; //sum of file names 
    nasDir *fnames;
    nasDir *fnames2;
    char lastDir[25];
    char   *filedir;
    char  caCmd[256];


   reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");
   filedir=ltMsgGetVar_s(ltMsgPk,"file");
   if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){
	    filedir=ltMsgGetVar_s(ltMsgPk,"file");
	    sprintf(caCmd,"rm -Rf /app/ns/conlog/report/%s/%s",ltMsgGetVar_s(ltMsgPk,"reportdird"),filedir);
      system(caCmd);
      sleep(1);
   }

  sprintf(caFile1,"/app/ns/conlog/report/%s/",reportdir);
  fnames=mydirlist(caFile1,&len);
  if(fnames){
 	   qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序
  }

  printf("%d个模板。。\n",len); 

  memset(flowInfo,0,sizeof(flowInfo));

  sprintf(flowInfo,"[ "); 
  
  for(i=0;i<len;i++) { 
  	
   sprintf(caFile1,"/app/ns/conlog/report/%s/%s/",reportdir,fnames[i].caDirName);
   printf("caFile1:%s\n",caFile1);
   sprintf(lastDir,"%s",fnames[i].caDirName); 
   fnames2=mydirlist(caFile1,&j); 
   if(fnames){
 	   qsort(fnames2,j,sizeof(nasDir),ccmps); 
   }
   printf("%s 下有 %d个报告\n",caFile1,j);
   while(j--) //free all allocated memory blocks
   {			//	printf("%s\n",fnames[len].caDirName);
						memset(reportname,0,sizeof(reportname));
					  memset(reportuser,0,sizeof(reportuser));
					  memset(cDate,0,sizeof(cDate));			
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s/%s/info.ini",reportdir,lastDir,fnames2[j].caDirName);
					  bcCnfGetValue_s(caFile1,"date",cDate);
					  bcCnfGetValue_s(caFile1,"name",reportname);
					  bcCnfGetValue_s(caFile1,"user",reportuser);
						sprintf(reportstate,"报告已生成");
						sprintf(caFile1,"/app/ns/conlog/report/%s/%s%s/errmsg.ini",reportdir,lastDir,fnames2[j].caDirName);
					  if(stat(caFile1,&st) == 0){
					    	sprintf(caFile1,"<a href=\"/report/%s/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,lastDir,fnames2[j].caDirName);
					    	sprintf(reportstate,caFile1);
					  }
					  sprintf(caFile1,"/app/ns/conlog/report/%s/%s/%s/proccess.ini",reportdir,lastDir,fnames2[j].caDirName);
					  if(stat(caFile1,&st) == 0){					    	 
								sprintf(reportstate,"报告正在生成中");
					  }
						printf("status::%s\n",reportstate);
            sprintf(flowInfodata,"['%s','%s','%s','%s','%s']",reportname,cDate,lastDir,fnames2[j].caDirName,reportstate);
						sprintf(flowInfodata,"%s,",flowInfodata);
						strcat(flowInfo,flowInfodata);
   }
}
if(len!=0){
	 flowInfo[strlen(flowInfo)-1]='\0'; 
}
	 strcat(flowInfo,"]"); 
	 ltMsgPk->msgpktype=1;

	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\n","lthtml",LT_TYPE_STRING,flowInfo);

  	  if(fnames){
		 		 free(fnames);
      }
      if(fnames2){
 				 free(fnames2);
      }


	 ltMsgFree(ltMsgPk);

	 return 0;

}
 
int toreportcreate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){ 
	char *tempIndex; 
    ltDbHeadPtr dbPtr; 
    dbPtr=lt_dbinit(); 
	tempIndex=ltMsgGetVar_s(ltMsgPk,"tempindex"); 
    lt_dbput_rootvars(dbPtr,1,"tempindex",tempIndex);  
    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportcreat.htm",dbPtr->head,0);  
    ltMsgFree(ltMsgPk); 
    lt_dbfree(dbPtr); 
    return 0;
 
}
 
int reportcreate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
	char caLabel[256];

    char caSdates[63]; 

    char caEdates[63]; 

	char reportname[40];

	char createDate[63];

	char caTempDir[123];

	char tempindex[64];

	int type;

	int    iPid;

    char **binqqargs;

	ltDbHeadPtr dbPtr;

	long lTime;

	struct stat st;
    char caFile1[256];
    char reportstate[123],repname[1024];
    nasDir *fnames;
    int len=0;
   // char   *filedir;
 
    dbPtr=lt_dbinit();
 
	lTime=time(0);

	nasTimeGetDate(createDate,lTime);
	
 	if(atoi( ltMsgGetVar_s(ltMsgPk,"st"))==0){
  
    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL || ltMsgGetVar_s(ltMsgPk,"edate")==NULL){

			sprintf(caSdates,"%s",createDate);

            sprintf(caEdates,"%s",createDate);

    }else{

    	    sprintf(caSdates,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));

			sprintf(caEdates,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));

    } 

 

   if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){

			sprintf(reportname,"%s","");

   }else{

    	    char *ppp;

    	    ppp=ltMsgGetVar_s(ltMsgPk,"reportname");

			memset(reportname,0,sizeof(reportname));

			strutf82gb(ppp,reportname,strlen(ppp));

   } 

 

   sprintf(tempindex,"%s",ltMsgGetVar_s(ltMsgPk,"tempindex"));

   sprintf(caTempDir,"/app/ns/conlog/report/page_data/%s/",tempindex);

   sprintf(caLabel,"/bin/echo 'sdate=%s' > %ssdate",caSdates,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'edate=%s' > %sedate",caEdates,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'rpname=%s' > %srpname",reportname,caTempDir);

   system(caLabel);  
 

	// 调用生成报告的主函数。。。。。。。。。。。。。。。。。。。。传入 type 和 tempindex

	type=atoi(ltMsgGetVar_s(ltMsgPk,"type")); 
	printf("repname:%s,type:: %d,tempindex:: %s\n",reportname,type,tempindex);   
	chdir("/app/ns/bin"); 
	iPid=fork(); 
 
//  reporttempstatus(tempindex,reportname,ltMsgPk,confd); 

	if(iPid==0){ 

			binqqargs = (char **) malloc(2 * sizeof(char *));

			binqqargs[0]= "dailyreporttemplate";

			binqqargs[1]= "2";

			binqqargs[2]= tempindex; 

			binqqargs[3]= 0;

			execv(*binqqargs,binqqargs);   
			 

			die(0);

	}
  
	wait(NULL);   
    printf("report is ok!!!\n"); 
   
	char flowInfo[1000];
 
	sprintf(flowInfo,"%s","{success:true}");
 

	ltMsgPk->msgpktype=1;

	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\r\n","lthtml",LT_TYPE_STRING,flowInfo);
	
   }else{

   sprintf(tempindex,"%s",ltMsgGetVar_s(ltMsgPk,"tempindex"));	  
   sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/",tempindex);
   printf("repname:%s,caFile1:: %s\n",ltMsgGetVar_s(ltMsgPk,"reportname"),caFile1);   
   fnames=mydirlist(caFile1,&len);
   if(fnames){

	    qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

     }
  while(len--){
    
     memset(repname,0,sizeof(repname));
	 memset(reportstate,0,sizeof(reportstate));
     
      sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/%s/info.ini",tempindex,fnames[len].caDirName);
     
      bcCnfGetValue_s(caFile1,"name",repname);

	  if(strcmp(ltMsgGetVar_s(ltMsgPk,"reportname"),repname)==0){
    
         sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/%s/proccess.ini",tempindex,fnames[len].caDirName);

	     if(stat(caFile1,&st) == 0){    
 
		 	sprintf(reportstate,"%s","0"); printf("==============proceeing\n");
 
	     }else{
			 sprintf(reportstate,"%s","1");printf("==============over\n");
		 }
	  break;

	  }
	 
	 }
	 printf("%s\n",reportstate);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\r\n","lthtml",LT_TYPE_STRING,reportstate);

  }
    
	ltMsgFree(ltMsgPk);

    lt_dbfree(dbPtr); 

    return 0;

}  
 
int reporttempstatus(char *tempindex,char *reportname,ltMsgHead *ltMsgPk,int confd){   
    struct stat st;
    char caFile1[256];
    char reportstate[123],repname[1024];
    nasDir *fnames;
    int len=0;

  // sprintf(tempindex,"%s",ltMsgGetVar_s(ltMsgPk,"tempindex"));	  
   sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/",tempindex);
   printf("repname:%s,caFile1:: %s\n",reportname,caFile1);   
   fnames=mydirlist(caFile1,&len);
   if(fnames){

	    qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

     }
  while(len--){
    
     memset(repname,0,sizeof(repname));
	 memset(reportstate,0,sizeof(reportstate));
     
      sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/%s/info.ini",tempindex,fnames[len].caDirName);
     
      bcCnfGetValue_s(caFile1,"name",repname);

	  if(strcmp(reportname,repname)==0){
    
         sprintf(caFile1,"/app/ns/conlog/report/instantreport/%s/%s/proccess.ini",tempindex,fnames[len].caDirName);

	     if(stat(caFile1,&st) == 0){    
 
		 	sprintf(reportstate,"%s","0"); printf("==============proceeing\n");
 
	     }else{
			 sprintf(reportstate,"%s","1");printf("==============over\n");
		 }
	  break;

	  }
	 
	 }
	 printf("%s\n",reportstate);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\r\n","lthtml",LT_TYPE_STRING,reportstate);

} 
 

int search(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){   

  char *content;

  char *date;

  char *type;

  char *curPage;

  int iCurPage;

  char currentPage[123],totalPage[256],totalNum[256];

  char flowInfo[900000];

  char indexPath[256];

  ltDbHeadPtr dbPtr;

  char caFile[236];

  ltTablePtr tablePtr;

  struct stat st;

  int len=0; //sum of file names 

  nasDir *fnames;

  char title[1026],contents[1026],url[1026];

  char caFile1[256],caFile2[256],caFile3[256];

  FILE *fp;    

  char ch;

  char res[1026];

  char ress[1026];

  int    iPid;

  char  caCmd[256];

  char toName1[1026];

  char toName2[1026]; 

  dbPtr=lt_dbinit();

 

  content=ltMsgGetVar_s(ltMsgPk,"myname");

  date=ltMsgGetVar_s(ltMsgPk,"sdate");

  type=ltMsgGetVar_s(ltMsgPk,"types");

  curPage=ltMsgGetVar_s(ltMsgPk,"CurPg");

  if(curPage==NULL){

       iCurPage=1;

  }else{  

       iCurPage=atoi(curPage);

  }

 printf("===curpage::%d\n",iCurPage);

  sprintf(indexPath,"/app/mnt/localback/%s/%sindex",date,type);

  printf("indexPath::%s\n",indexPath);

  printf("search : %s\n",content);



 //调用JAVA程序返回所有数据

 chdir("/app/ns/v2008/class");

 iPid=fork();

 if(iPid==0){

	 sprintf(caCmd,"java com.seven.getData %s %s %s %d ",content,date,type,iCurPage);

	 printf(" caCmd ::%s\n",caCmd);

	 system(caCmd);

 }

 wait(NULL); 
 

 sprintf(caFile,"/app/mnt/localback/pagebak/page.ini"); 

 bcCnfGetValue_s(caFile,"0",currentPage); 

 bcCnfGetValue_s(caFile,"1",totalPage); 

 bcCnfGetValue_s(caFile,"2",totalNum); 
 
 printf("currentPage:%d /%s ,totalPage:%s ,totalNum:%s\n",iCurPage,currentPage,totalPage,totalNum);

    
  	lt_db_htmlpage(dbPtr,"gb2312");
 
	tablePtr=lt_dbput_table(dbPtr,"list");
 
	sprintf(caFile1,"/app/mnt/localback/databak/"); 
 
    fnames=mydirlist(caFile1,&len);
 
    if(fnames){
 
 	   qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序
 
    }



    while(len--) //free all allocated memory blocks

   { 
 
					 	memset(title,0,sizeof(title));
 
					    memset(contents,0,sizeof(contents));
 
					    memset(url,0,sizeof(url));
 
						sprintf(caFile1,"/app/mnt/localback/databak/%s/title.txt",fnames[len].caDirName);
 
			           // bcCnfGetValue_s(caFile1,"title",title);
 
						fp=fopen(caFile1,"r");

						if(fp==NULL){

							printf("no file"); 

							exit(1);

						}

						ch=fgetc(fp);

						memset(res,0,sizeof(res));

						memset(ress,0,sizeof(ress));

						while(ch!=EOF){

							sprintf(res,"%c",ch);

							strcat(ress,res);

							ch=fgetc(fp);

						

						}  //printf("%s\n",ress);

						fclose(fp);

						sprintf(title,ress);
 
						sprintf(caFile2,"/app/mnt/localback/databak/%s/content.txt",fnames[len].caDirName);

						bcCnfGetValue_s(caFile2,"content",contents);

 
						sprintf(caFile3,"/app/mnt/localback/databak/%s/url.txt",fnames[len].caDirName);

					    bcCnfGetValue_s(caFile3,"url",url);

						 //  printf("caFile1:%s\n",caFile1);

                     //   printf("content:::%s\n",contents);  
 
					    memset(toName1,0,sizeof(toName1));

						strutf82gb(title,toName1,strlen(title));

						//printf("title:::%s\n",title);

					//	printf(".......%s\n",toName1);

						sprintf(title,toName1);



						memset(toName2,0,sizeof(toName2));

						strutf82gb(contents,toName2,strlen(contents));

						sprintf(contents,toName2);

						//contents=toName2; 

					 

					    lt_dbput_recordvars(tablePtr,3,
  
								"title",LT_TYPE_STRING,title,
 
								"contents",LT_TYPE_STRING,contents, 
 
								"url",LT_TYPE_STRING,url);
 
 }
  
  lt_dbput_rootvars(dbPtr,3,"CurPg",currentPage,"TotPg",totalPage,"allcount",totalNum);

  lt_dbput_rootvars(dbPtr,3,"myname",content,"sdate",date,"types",type);
 

  if(strcmp(type,"mail")==0){        

	  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/mailResult.htm",dbPtr->head,0);

  }else{

	  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/formResult.htm",dbPtr->head,0);

  } 
  
  ltMsgFree(ltMsgPk);  

  lt_dbfree(dbPtr);  

  return 0;     

}  
 

int searchQuery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){ 

    char flowInfo[900000];

    char indexPath[256];

     ltDbHeadPtr dbPtr;

    char caFile[236];

     dbPtr=lt_dbinit();
 
	lt_dbput_rootvars(dbPtr,3,"CurPg","1","TotPg","1","allcount","0"); 
 
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/mailResult.htm",dbPtr->head,0);
 
	ltMsgFree(ltMsgPk);

	lt_dbfree(dbPtr);

	return 0;

}


//模板编辑
int reporttempledit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
  char caFile[256];
   
  char gids[24];

  char userids[24];

  char temptypes[256];
 
  long  gid;
  
  long  userid;

  long temptype;
 
  char element[50];

  char checkb[10];
  
  char templname[100];     
   
  char email[64];
  char topusernum[64];
  char language[4];
  
  ltDbHeadPtr dbPtr;
  ltTablePtr tablePtr; 
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  char sqlBuf[1024];

  ltDbConn *G_DbCon;

  sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
  sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
  sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
  if (G_DbCon==NULL){
		printf("report:can't open db\n");
		_exit(0);
 }

  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"gb2312");
 
   sprintf(caFile,"/app/ns/conlog/report/page_data/%s/info.ini",ltMsgGetVar_s(ltMsgPk,"tempdir"));
  printf("caFile::%s\n",caFile);
   memset(element,0,sizeof(element)); memset(checkb,0,sizeof(checkb));
 
   memset(element,'0',50);            memset(checkb,'0',10);
  memset(topusernum,0,sizeof(topusernum));
    bcCnfGetValue_s(caFile,"element",element); 
    bcCnfGetValue_s(caFile,"checkb",checkb); 
    bcCnfGetValue_s(caFile,"gid",gids);  
    bcCnfGetValue_s(caFile,"userid",userids);
	bcCnfGetValue_s(caFile,"email",email);
	bcCnfGetValue_s(caFile,"templname",templname);
	bcCnfGetValue_s(caFile,"type",temptypes);
	bcCnfGetValue_s(caFile,"language",language);
  bcCnfGetValue_s(caFile,"topusernum",topusernum);  
    
	gid=atol(gids);
	userid=atol(userids);
	temptype=atol(temptypes);
  
    printf("element::%s\n",element);
  	printf("checkb::%s\n",checkb);
	printf("gid::%d\n",gid);
	printf("userid::%d\n",userid);
	printf("templname::%s\n",templname);
 	printf("temptype::%d\n",temptype);
	printf("email::%s\n",email);  
	printf("language::%s\n",language);

    sprintf(sqlBuf,"select groupid,groupname from nasgroup ");printf("sqlBuf1::%s\n",sqlBuf);
    tablePtr=lt_dbput_table(dbPtr,"glist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);    
		if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
						    while(tempRow!=NULL){
								 if(strcmp(gids,tempRow[0])==0){ 
									 lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," selected ",
										"gname",LT_TYPE_STRING,tempRow[1]);

								 }else{
								 		lt_dbput_recordvars(tablePtr,3,
										"gid",LT_TYPE_STRING,tempRow[0],
										"sel",LT_TYPE_STRING," ",
										"gname",LT_TYPE_STRING,tempRow[1]);
								 }printf("::%s\n",tempRow[1]);
								 tempRow= ltDbFetchRow(tempCursor);       
		                     }
							 ltDbCloseCursor(tempCursor); 
		}
 
     tablePtr=lt_dbput_table(dbPtr,"ulist"); 
    if(strcmp(gids,"-1")!=0){
    	sprintf(sqlBuf,"select userid,username from nasuser where groupid='%s' ",gid);
        printf("sqlBuf::%s\n",sqlBuf);
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
						   tempRow= ltDbFetchRow(tempCursor);
						   while(tempRow!=NULL){
							  if(strcmp(userids,tempRow[0])==0){  
								lt_dbput_recordvars(tablePtr,3,
									"uid",LT_TYPE_STRING,tempRow[0],
									"sel2",LT_TYPE_STRING,"selected",
									"uname",LT_TYPE_STRING,tempRow[1]);
							  }else{
								 lt_dbput_recordvars(tablePtr,3,
									"uid",LT_TYPE_STRING,tempRow[0],
									 "sel2",LT_TYPE_STRING," ",
									"uname",LT_TYPE_STRING,tempRow[1]);
							  }
								tempRow= ltDbFetchRow(tempCursor);
			               }
						   ltDbCloseCursor(tempCursor); 
			} 	
	}
   
    if(temptype==0){
	  lt_dbput_rootvars(dbPtr,1,"se1","checked");
   } if(temptype==1){
	  lt_dbput_rootvars(dbPtr,1,"se2","checked");
   } if(temptype==2){
	  lt_dbput_rootvars(dbPtr,1,"se3","checked");
   }

if(checkb[0]=='1'){
	lt_dbput_rootvars(dbPtr,1,"b1","checked");
}if(checkb[1]=='1'){
	lt_dbput_rootvars(dbPtr,1,"b2","checked");
}if(checkb[2]=='1'){
	lt_dbput_rootvars(dbPtr,1,"b3","checked");
}if(checkb[3]=='1'){
	lt_dbput_rootvars(dbPtr,1,"b4","checked");
}if(checkb[4]=='1'){
	lt_dbput_rootvars(dbPtr,1,"b5","checked");
} 

if(element[0]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr1","checked");
}if(element[1]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr2","checked");
}if(element[2]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr3","checked");
}if(element[3]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr4","checked");
}if(element[4]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr5","checked");
}if(element[5]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr6","checked");
}if(element[6]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr7","checked");
}if(element[7]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr8","checked");
}if(element[8]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr9","checked");
}if(element[9]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr10","checked");
}if(element[10]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr11","checked");
}if(element[11]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr12","checked");
}if(element[12]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr13","checked");
}if(element[13]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr14","checked");
}if(element[14]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr15","checked");
}if(element[15]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr16","checked");
}if(element[16]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr17","checked");
}if(element[17]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr18","checked");
}if(element[18]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr19","checked");
}if(element[19]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr20","checked");
}if(element[20]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr21","checked");
}if(element[21]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr22","checked");
}if(element[22]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr23","checked");
}if(element[23]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr24","checked");
}if(element[24]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr25","checked");
}if(element[25]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr26","checked");
}if(element[26]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr27","checked");
}if(element[27]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr28","checked");
}if(element[28]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr29","checked");
}if(element[29]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr30","checked");
}if(element[30]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr31","checked");
}if(element[31]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr32","checked");
}if(element[32]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr33","checked");
}if(element[33]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr34","checked");
}if(element[34]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr35","checked");
}if(element[35]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr36","checked");
}if(element[36]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr37","checked");
}if(element[37]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr38","checked");
}if(element[38]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr39","checked");
}if(element[39]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr40","checked");
}if(element[40]=='1'){
	lt_dbput_rootvars(dbPtr,1,"chr41","checked");
} 

if(atoi(language)==0){
    lt_dbput_rootvars(dbPtr,1,"san1","selected");
}if(atoi(language)==1){
   lt_dbput_rootvars(dbPtr,1,"san2","selected");
}

 lt_dbput_rootvars(dbPtr,4,"topusernum",topusernum,"templname",templname,"email",email,"tempid",ltMsgGetVar_s(ltMsgPk,"tempdir"));
 
 lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reportstylesettingedit.htm",dbPtr->head,0);
 
 ltMsgFree(ltMsgPk); 
  
 return 0;
 
}

/*修改模板*/ 

int reporttemplateupdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){ 
 
  char caLabel[256];
 
  char *depart;
  
  char *employee;  
 
  char reportname[40];
 
  int  gid;

  char *ppp;

  int  userid;
 
  char element[50];

  char checkb[10];
  
  int deployscope;

  char templname[100];     
 
  char caTempDir[123];

  char *tempindex;
  
  char temptype[256];

  char tempdir[64];

  char email[64];
  
  char language[4];
  
  char createDate[36];
  char *topusernum;
  long lTime;
 
  tempindex=ltMsgGetVar_s(ltMsgPk,"tempid");
 topusernum=ltMsgGetVar_s(ltMsgPk,"topusernum");
  depart=ltMsgGetVar_s(ltMsgPk,"groupid");
 
  employee=ltMsgGetVar_s(ltMsgPk,"user");
 
  userid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
 
  gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
 
  if( ltMsgGetVar_s(ltMsgPk,"templname")==NULL){
 
      sprintf(templname,"%s","");
 
  }else{
 
     ppp=ltMsgGetVar_s(ltMsgPk,"templname");

     memset(templname,0,sizeof(templname));

     strutf82gb(ppp,templname,strlen(ppp));
 
  } 
 
  printf("templname:%s\n",templname);
  
  if( ltMsgGetVar_s(ltMsgPk,"reportname")==NULL){
 
		sprintf(reportname,"%s","");
 
  }else{
 
        sprintf(reportname,"%s",ltMsgGetVar_s(ltMsgPk,"reportname"));
 
  } 
 

  memset(element,0,sizeof(element)); memset(checkb,0,sizeof(checkb));
 
  memset(element,'0',50);            memset(checkb,'0',10);
 
  sprintf(element,ltMsgGetVar_s(ltMsgPk,"element"));

  sprintf(checkb,ltMsgGetVar_s(ltMsgPk,"checkb"));
 
  if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
 
       sprintf(email,"%s",""); 
 
  }else{
 
       sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
 
  }
 
 memset(language,0,sizeof(language));
 sprintf(language,ltMsgGetVar_s(ltMsgPk,"language"));

 printf("depart:%s\n",depart);

 printf("employee:%s\n",employee);
 
 printf("element:%s\n",element);

 printf("checkb:%s\n",checkb);

 printf("templname:%s\n",templname);printf("email:%s\n",email);
 printf("language:%s\n",language);
 
 

 lTime = time(0);

 nasTimeGetDate(createDate,lTime);
 
 deployscope=atoi(ltMsgGetVar_s(ltMsgPk,"deployscope"));
 

 if(deployscope==0){//每天生成报告
 
   sprintf(caTempDir,"/app/ns/conlog/report/page_data/%s/",tempindex);  

   sprintf(temptype,"每天自动生成报告");

   sprintf(tempdir,"dailyreport");
 
   if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {

          /// 生成该报告目录错误 

          printf("mkdir daily report dir error!\n");

          return 0;
 
         }
 
   }  
 
  sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
  
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'reportname=%s' >> %sinfo.ini",reportname,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);
  
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",0,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
  
  system(caLabel);

  sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
   system(caLabel);
 
 }if(deployscope==1){//每周 
  
	  sprintf(caTempDir,"/app/ns/conlog/report/page_data/%s/",tempindex); 

	  sprintf(temptype," 每周自动生成报告");

	  sprintf(tempdir,"dailyreport");
 
      if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {

          /// 生成该报告目录错误 

          printf("mkdir daily report dir error!\n");

          return 0;

         }

       } 
 
  sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);
 
  system(caLabel);  
 
  sprintf(caLabel,"/bin/echo 'reportname=%s' >> %sinfo.ini",reportname,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);
 
  system(caLabel); 
 
  sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);
  
  system(caLabel); 
  
  sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);
  
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);
  
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",1,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);
 
  system(caLabel);
 
  sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
 
  system(caLabel);

  sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  
  system(caLabel);
  
  sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
   system(caLabel);
 
 }if(deployscope==2){//立即生成 
 
   sprintf(temptype,"手动生成报告");

   sprintf(tempdir,"instantreport");
 
   sprintf(caTempDir,"/app/ns/conlog/report/page_data/%s/",tempindex); 

  
   if(ltFileIsExist(caTempDir)!=1) {

         if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
        /// 生成该报告目录错误 
          printf("mkdir daily report dir error!\n");

          return 0;

         }

    } 

   sprintf(caLabel,"/bin/echo 'templname=%s' > %sinfo.ini",templname,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'gid=%d' >> %sinfo.ini",gid,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'userid=%d' >> %sinfo.ini",userid,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'element=%s' >> %sinfo.ini",element,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'checkb=%s' >> %sinfo.ini",checkb,caTempDir);

   system(caLabel);    

   sprintf(caLabel,"/bin/echo 'temptype=%s' >> %sinfo.ini",temptype,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'tempdir=%s' >> %sinfo.ini",tempdir,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'type=%d' >> %sinfo.ini",2,caTempDir);

   system(caLabel); 

   sprintf(caLabel,"/bin/echo 'createDate=%s' >> %sinfo.ini",createDate,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);

   system(caLabel);

   sprintf(caLabel,"/bin/echo 'language=%s' >> %sinfo.ini",language,caTempDir);
  
   system(caLabel);
   
   sprintf(caLabel,"/bin/echo 'topusernum=%s' >> %sinfo.ini",topusernum,caTempDir);
   system(caLabel);

 } 

 printf("caTempDir::%s\n",caTempDir);
  
 ltDbHeadPtr dbPtr;
 
 dbPtr=lt_dbinit();  
 
 lt_db_htmlpage(dbPtr,"gb2312");    
 
 lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/reporttemplatelist.htm",dbPtr->head,0);
 
 ltMsgFree(ltMsgPk); 
  
 return 0;
 
}


int reportstatus(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  
	ltDbHeadPtr dbPtr;
 
	struct stat st;
    char caFile1[256];
    char reportstate[123],repname[1024],repdir[1024];
    nasDir *fnames;
    int len=0;
   
    dbPtr=lt_dbinit();
   
   sprintf(repdir,"%s",ltMsgGetVar_s(ltMsgPk,"repdir"));	  
   sprintf(caFile1,"/app/ns/conlog/report/%s/",repdir);

   printf("repname:%s,caFile1:: %s\n",ltMsgGetVar_s(ltMsgPk,"reportname"),caFile1);   

   fnames=mydirlist(caFile1,&len);
    if(fnames){

	    qsort(fnames,len,sizeof(nasDir),ccmps); //给文件名按字母排序

     }
    while(len--){
    
     memset(repname,0,sizeof(repname));
	 memset(reportstate,0,sizeof(reportstate));
     
      sprintf(caFile1,"/app/ns/conlog/report/%s/%s/info.ini",repdir,fnames[len].caDirName);
  
      bcCnfGetValue_s(caFile1,"name",repname);
 printf("%s\n",repname);  
	  if(strcmp(ltMsgGetVar_s(ltMsgPk,"reportname"),repname)==0){
    
         sprintf(caFile1,"/app/ns/conlog/report/%s/%s/proccess.ini",repdir,fnames[len].caDirName);

	      if(stat(caFile1,&st) == 0){    
 
		 	sprintf(reportstate,"%s","0"); printf("0==============proceeing\n");
 
	      }else{
			 sprintf(reportstate,"%s","1");printf("1==============over\n");
		  }
	      break;

	  }
	 
	 }
	 
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=gb2312\r\n","lthtml",LT_TYPE_STRING,reportstate);
  
	ltMsgFree(ltMsgPk);

    lt_dbfree(dbPtr); 

    return 0;

}