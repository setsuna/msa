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


#define JAVACMD "/app/ns/java/jdk1.6.0_01/bin/java -cp /app/ns/java/poi/poi-3.7-20101029.jar:./ "
#define BINCMD	"/app/msa/bin"



/*
 MSA:��������
     ��ϸ���ʼ�¼
         ������ʼ�¼
         ��Ϸ��ʼ�¼
 ���ܣ�ҳ�����ӵ��ú���
 */
int msaShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	  ltDbHeadPtr dbPtr;
		int    showtype=0;
		char   caFile1[256];
		int    len=0;
		int    myindex=0;
	 
	  long  lTime;
	  char *caSdate=NULL;
	  char sDate[36];
	  char selDate[36];
	  char selTime[36];
	  
    memset(sDate,0,sizeof(sDate));
    memset(selDate,0,sizeof(selDate));
    memset(selTime,0,sizeof(selTime));
	  msasDir *fnames;
	  ltTablePtr  tablePtr;
	 
	 if(ltMsgGetVar_s(ltMsgPk,"showtype")){
	 	showtype=atoi(ltMsgGetVar_s(ltMsgPk,"showtype"));
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"sdate")){
	 	caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
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
   
	  lTime = time(0);
	  nasTimeGetDate(sDate,lTime);	
	  nasCvtStime(lTime,selDate,selTime);	
	  
	  if(selTime[0]=='0'){
	  	selTime[0]=selTime[1];
	  	selTime[1]='\0';
	  }else{
	    selTime[2]='\0';
	  }
		
		if(caSdate==NULL){
			lt_dbput_rootvars(dbPtr,1,"seldate",sDate);
		}else{
			lt_dbput_rootvars(dbPtr,1,"seldate",caSdate);
		}
    
    lt_dbput_rootvars(dbPtr,1,"seltime",selTime);


   ltMsgPk->msgpktype=1;
   if(showtype==1){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
	 }else if(showtype==2){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/BlockAccessList.htm",dbPtr->head,0);
	 }
	 ltMsgFree(ltMsgPk);

	 if(fnames){
 		free(fnames);
   }


	 return 0;
}


/*
 MSA:��������
     ������ʼ�¼����
     ������ϼ�¼����
 ����:ҳ�����ӵ��ú���
 */
int msaShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 int    showtype=0;
	 char   caFile1[256];
	 int    len=0;//Ŀ¼����
	 int    iii;

	 msasDir *fnames;
	 char *sDate=NULL;
	 ltTablePtr  tablePtr;

	 if(ltMsgGetVar_s(ltMsgPk,"showtype")){
	 	showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"sdate")){
	 	sDate=ltMsgGetVar_s(ltMsgPk,"sdate");
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
   //����ͨ��
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

   lt_dbput_rootvars(dbPtr,1,"seldate",sDate);

   ltMsgPk->msgpktype=1;
   if(showtype==1){
	 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipbacksearchlist.htm",dbPtr->head,0);
	 }else if(showtype==2){
	 	//ƥ�����
	 	 tablePtr=lt_dbput_table(dbPtr,"matchrulelist");
	   for(iii=0;iii<_MAX_SRVRULE_NUM;iii++){
		   	if(strlen(_ltPubInfo->_Srvrulelist[iii].rulename)>0){
		   		lt_dbput_recordvars(tablePtr,2,
		   		 "matchruleid",LT_TYPE_LONG,_ltPubInfo->_Srvrulelist[iii].ruleid,
		   	 	 "matchrulename",LT_TYPE_STRING,_ltPubInfo->_Srvrulelist[iii].rulename);
		   	}
	   }

	 	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/ipblocksearchlist.htm",dbPtr->head,0);
	 }

	 ltMsgFree(ltMsgPk);

	 if(fnames){
 		free(fnames);
   }

	 return 0;


}



/*
 MSA:��������
     		 ������ʼ�¼
         ������ϼ�¼
 ����:�б���ʾ����ʱ����ʼ�¼���ú���
 */
int msaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char   caFile1[256];
  char   caFile2[256];
  long   lRowNum,lStartRec,lSumRec,lCount,lNum;
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
  char  dtime[32];
  int   iReturn;
  char  *caUrl;

  static char inbuf[4096];
  static stralloc line = {0};
  buffer ss;
  int    match;
  int    kkkk;
  int    jjjj;
  int    iiii;
  
  lRowNum=100;
  lSumRec=0;
  lCount=0;
  lStartRec=0;
  lNum=0;
  iReturn=-1;
  
  memset(caFile1,0,sizeof(caFile1));
  memset(caFile2,0,sizeof(caFile2));
  memset(caTmpp,0,sizeof(caTmpp));
  memset(caTmpp11,0,sizeof(caTmpp11));
  
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


  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }

  totalnum=0;
  sprintf(caFile2,"%s/%s.count",_datacenterdir,caUrl);
	iReturn=access(caFile2,F_OK);
  if(iReturn!=0){//������file2
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
		     for(inum=0;inum<iCount;inum++){   /*ͳ���ж�ȡ�����м�¼*/
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
				strcat(ress,res); //�ַ�ƴ��
				ch=fgetc(fp);
		}
    fclose(fp);
    totalnum=atol(ress);
  }

  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  fd=open(caFile1,O_RDONLY | O_NDELAY);
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");
				 ltMsgFree(ltMsgPk);

				 return 0;
  }
  
  nownum=0;
  memset(tempStr,0,sizeof(tempStr));
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
     //Userid,�û���,��ʾ����һ�����ţ�2�����ţ�3�����ţ�4�����ţ�5�����ţ����䷽��Э�飬Դip,����mac,Ŀ��ip,Դ�˿ڣ�Ŀ�Ķ˿ڣ���ʼ�������ڣ���ʼ����ʱ�䣬�����������������������ݰ���Ŀ��һ������2����������ͨ�����ؼ���		
			for(jjjj=0;jjjj<line.len;jjjj++){
				 if(line.s[jjjj]=='\t'){
					 iIndex++;
           if(iIndex==1){
           	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
			     }else if(iIndex==4){//һ������
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(tempStr,",\"v%d\":\"%s",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 }else if(iIndex==9){//���䷽��
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strcmp(caTmpp11,"0")==0){
			         	 sprintf(fangX,"%s","->");
			         }else{
			         	 sprintf(fangX,"%s","<-");
			         }
					 }else if(iIndex==10){//Э��
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(protocol,0,sizeof(protocol));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(protocol,"%s",caTmpp11);
					 }else if(iIndex==11){//ԴIP
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sipadd,0,sizeof(sipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sipadd,"%s",caTmpp11);
					 }else if(iIndex==13){//Ŀ��IP
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(mipadd,0,sizeof(mipadd));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(mipadd,"%s",caTmpp11);
					 }else if(iIndex==14){//Դ�˿�
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(sport,0,sizeof(sport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(sport,"%s",caTmpp11);
					 }else if(iIndex==15){//UDP:IP:Դ�˿�->Ŀ�Ķ˿�
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
					 	   memset(dport,0,sizeof(dport));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         sprintf(dport,"%s",caTmpp11);
			         memset(tempStr,0,sizeof(tempStr));
			         sprintf(tempStr,",\"v%d\":\"%s:%s:%s%s%s:%s\"",iIndex,protocol,sipadd,sport,fangX,mipadd,dport);
			         stralloc_cats(&strTemp,tempStr);
					 }else if(iIndex==16){
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         memset(dtime,0,sizeof(dtime));
			         sprintf(dtime,"%s",caTmpp11);	         
					 }else if(iIndex==17){
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         memset(tempStr,0,sizeof(tempStr));
			         sprintf(tempStr,",\"v%d\":\"%s    %s\"",iIndex,dtime,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 }else{
					 	   memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         memset(tempStr,0,sizeof(tempStr));
			         sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);
			         stralloc_cats(&strTemp,tempStr);
					 }



					 jjjj++;
					 iiii=jjjj;
					 while(line.s[jjjj]=='\t'){
					 	iIndex++;
					  if(iIndex==5){//��������
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strlen(caTmpp11)>0){
			         	 sprintf(tempStr,"__%s",caTmpp11);
			         	 stralloc_cats(&strTemp,tempStr);
			         }		         
					  }else if(iIndex==6){//��������
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strlen(caTmpp11)>0){
			         	 sprintf(tempStr,"__%s",caTmpp11);
			         	 stralloc_cats(&strTemp,tempStr);
			         }		        
					  }else if(iIndex==7){//�ļ�����
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strlen(caTmpp11)>0){
			         	 sprintf(tempStr,"__%s",caTmpp11);
			         	 stralloc_cats(&strTemp,tempStr);
			         }			        
					  }else if(iIndex==8){//�弶����
					 		 memset(caTmpp11,0,sizeof(caTmpp11));
			         memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
			         if(strlen(caTmpp11)>0){
			         	 sprintf(tempStr,"__%s\"",caTmpp11);
			         }else{
			           sprintf(tempStr,"%s\"",caTmpp11);
			         }
			         stralloc_cats(&strTemp,tempStr);
					  }
				    jjjj++;
					  iiii=jjjj;
					 }
			   }

		  }



		  {
		  	iIndex++;
	      memset(caTmpp11,0,sizeof(caTmpp11));
		    memcpy(caTmpp11,&line.s[iiii],jjjj-iiii);
		    memset(tempStr,0,sizeof(tempStr));
		    if(strlen(caTmpp11)>2){
		    	sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTmpp11);   	
		    }else{
		      sprintf(tempStr,",\"v%d\":\"\"",iIndex);	
		    }
		    		    		 
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
 MSA:��������
     		 ������ʼ�¼����
 ����:��������ѯ����������|ֱ�ӵ�������ʱ����������
 */
int msaipbacksearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

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
	char   **vv;
	int    srvid;
	int    twosrvid;
	int    vpath;
	int    matchrule;

	char   caTempDir[256];
	char   tmpdir[64];
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
	char   caLine[1024];

	
	memset(caLine,0,sizeof(caLine));
	memset(sDate,0,sizeof(sDate));
	if(ltMsgGetVar_s(ltMsgPk,"showtype")){
		showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	}else{
		showtype=0;
	}
	
	if(ltMsgGetVar_s(ltMsgPk,"msa.login.account")){
		manageruser=ltMsgGetVar_s(ltMsgPk,"msa.login.account");
	}else{
		manageruser="";
	}
	printf("manageruser:%s\n",manageruser);
	nasTimeFormat(sDate,(time(0)-1800));
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
		  	gid=atoi(ltMsgGetVar_s(ltMsgPk,"gid"));//����id;
	}	
	

	printf("gname:%s\n",ltMsgGetVar_s(ltMsgPk,"gname"));	
	if(ltMsgGetVar_s(ltMsgPk,"glevel")){
		  	glevel=atoi(ltMsgGetVar_s(ltMsgPk,"glevel"));//���ż���
		  	if(glevel==0 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){//һ������
		  		   if(strcmp(ltMsgGetVar_s(ltMsgPk,"gname"),"DefaultGroup")==0){
		  		   	vv[4]="defaultGroup";  
		  		   }		  		       
		  	}else if(glevel==1 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){//��������
		  		   vv[5]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else if(glevel==2 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){//��������
		  	     vv[6]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else if(glevel==3 && ltMsgGetVar_s(ltMsgPk,"gname")!=NULL){//�ļ�����
		  		   vv[7]=ltMsgGetVar_s(ltMsgPk,"gname");
		  	}else{
		  		   vv[8]=ltMsgGetVar_s(ltMsgPk,"gname");//�弶����
		  	}
	}
	
	
	if(showtype==1){//������ʼ�¼����
		  //Userid,�û���,��ʾ����һ�����ţ�2�����ţ�3�����ţ�4�����ţ�5�����ţ����䷽��Э�飬Դip,����mac,Ŀ��ip,Դ�˿ڣ�Ŀ�Ķ˿ڣ���ʼ�������ڣ���ʼ����ʱ�䣬�����������������������ݰ���Ŀ��һ������2����������ͨ�����ؼ���		
			if(ltMsgGetVar_s(ltMsgPk,"sip")){
				vv[11]=ltMsgGetVar_s(ltMsgPk,"sip");//ԴIP 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"dip")){
				vv[13]=ltMsgGetVar_s(ltMsgPk,"dip");//Ŀ��IP
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"dport")){
				vv[15]=ltMsgGetVar_s(ltMsgPk,"dport");//Ŀ�Ķ˿�
			}
		
			if(ltMsgGetVar_s(ltMsgPk,"selsrv")){
				srvid=atoi(ltMsgGetVar_s(ltMsgPk,"selsrv"));//һ������ 
				if(srvid>-1 && srvid<MAX_TOP_SRVNUM){
			  	vv[21]=_ltPubInfo->topSrvName[srvid].srvname;
			  }
			}
		

		if(ltMsgGetVar_s(ltMsgPk,"twoselsrv")){
		  twosrvid=atoi(ltMsgGetVar_s(ltMsgPk,"twoselsrv"));//�������� 
			if(twosrvid>-1 && twosrvid<_ltPubInfo->lServiceNum){
		  	vv[22]=_ltPubInfo->ltService[twosrvid].srvName;
		  }
	  }
	  
	  if(ltMsgGetVar_s(ltMsgPk,"vpath")){
			vpath=atoi(ltMsgGetVar_s(ltMsgPk,"vpath"));//����ͨ��
      if(vpath>-1 && vpath<_MAXVPATHNUM){
      	 vv[23]=_ltPubInfo->_VPathList[vpath].vPathName;
      }
		}

		
		
	}else if(showtype==2){//������ϼ�¼����
		//Userid,�û���,��ʾ����һ�����ţ�2�����ţ�3�����ţ�4�����ţ�5�����ţ����䷽��Э�飬Դip,����mac,Ŀ��ip,Դ�˿ڣ�Ŀ�Ķ˿ڣ���ʼ�������ڣ���ʼ����ʱ�䣬������ݰ���Ŀ��һ������2������ƥ���������ͨ�����ؼ��� 	
		if(ltMsgGetVar_s(ltMsgPk,"sip")){
			vv[11]=ltMsgGetVar_s(ltMsgPk,"sip");//ԴIP 
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dip")){
			vv[13]=ltMsgGetVar_s(ltMsgPk,"dip");//Ŀ��IP
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"dport")){
			vv[15]=ltMsgGetVar_s(ltMsgPk,"dport");//Ŀ�Ķ˿�
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"selsrv")){
			srvid=atoi(ltMsgGetVar_s(ltMsgPk,"selsrv"));//һ������ 
			if(srvid>-1 && srvid<MAX_TOP_SRVNUM){
		  	vv[19]=_ltPubInfo->topSrvName[srvid].srvname;
		  }
		}
		

		if(ltMsgGetVar_s(ltMsgPk,"twoselsrv")){
		  twosrvid=atoi(ltMsgGetVar_s(ltMsgPk,"twoselsrv"));//�������� 
			if(twosrvid>-1 && twosrvid<_ltPubInfo->lServiceNum){
		  	vv[20]=_ltPubInfo->ltService[twosrvid].srvName;
		  }
	  }
	  
	  if(ltMsgGetVar_s(ltMsgPk,"matchrule")){
			matchrule=atoi(ltMsgGetVar_s(ltMsgPk,"matchrule"));//ƥ�����
      if(matchrule>-1 && matchrule<_MAX_SRVRULE_NUM){
      	 vv[21]=_ltPubInfo->_Srvrulelist[matchrule].rulename;
      }
		}
	  
	  if(ltMsgGetVar_s(ltMsgPk,"vpath")){
			vpath=atoi(ltMsgGetVar_s(ltMsgPk,"vpath"));//����ͨ��
      if(vpath>-1 && vpath<_MAXVPATHNUM){
      	 vv[22]=_ltPubInfo->_VPathList[vpath].vPathName;
      }
		}
		
		
	}
	 
	 //��ѯ����
	 if(ltMsgGetVar_s(ltMsgPk,"searchconX")){
	 	 searchconX=ltMsgGetVar_s(ltMsgPk,"searchconX");
	 }else{
	 	 searchconX="";
	 }
	 
	 
	 //�Ƿ񵼳�
	 if(ltMsgGetVar_s(ltMsgPk,"exportaction")){
	 	 exportaction=ltMsgGetVar_s(ltMsgPk,"exportaction");
	 }else{
	 	 exportaction=NULL;
	 }
	  
	  //������������
	  if(ltMsgGetVar_s(ltMsgPk,"searchname")){
	  	searchname=ltMsgGetVar_s(ltMsgPk,"searchname");
	  }else{
	  	searchname=" ";
	  }
	  

    if(exportaction==NULL){//��ѯ    	 
  	    if(showtype==1){
		  	   sprintf(caTempDir,"%s/ipsearch/",_datacenterdir);
		    }else if(showtype==2){
		  	   sprintf(caTempDir,"%s/ipblocksearch/",_datacenterdir);
		    }
		
				if(ltFileIsExist(caTempDir)!=1) {//�ж��Ƿ���ڴ�Ŀ¼
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					  ltMsgFree(ltMsgPk);
					  return 0;
					}
				}
		
				if(showtype==1){
					sprintf(caTempDir,"%s/ipsearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
				}else if(showtype==2){
					sprintf(caTempDir,"%s/ipblocksearch/%s_%lu/",_datacenterdir,manageruser,ltStrGetId());
				}
		
				if(ltFileIsExist(caTempDir)!=1){
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					  ltMsgFree(ltMsgPk);
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
		    	memset(tmpgname,0,sizeof(tmpgname));
		    	sprintf(tmpgname,"%s","|������:");
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
					
					sprintf(caCmd,"|ԴIP:%s",vv[11]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
			
					sprintf(caCmd,"|Ŀ��IP:%s",vv[13]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
					
					sprintf(caCmd,"|Ŀ�Ķ˿�:%s",vv[15]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
				  
				  char oneservice[32];
				  sprintf(oneservice,"%s","|һ������:");
				  strgb2utf8(oneservice,utf8Cmd,strlen(oneservice));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[21]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char twoservice[32];
					sprintf(twoservice,"%s","|��������:");
				  strgb2utf8(twoservice,utf8Cmd,strlen(twoservice));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[22]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
					char tmpvpath[32];
					sprintf(tmpvpath,"%s","|����ͨ��:");
					strgb2utf8(tmpvpath,utf8Cmd,strlen(tmpvpath));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[23]);
					write(fdwrite,caCmd,strlen(caCmd));
									
					
				}else if(showtype==2){
					char tmpgname[32];
					memset(tmpgname,0,sizeof(tmpgname));
		    	sprintf(tmpgname,"%s","|������:");
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
					
					
					sprintf(caCmd,"|ԴIP:%s",vv[11]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
			
					sprintf(caCmd,"|Ŀ��IP:%s",vv[13]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
					
					sprintf(caCmd,"|Ŀ�Ķ˿�:%s",vv[15]);
					strgb2utf8(caCmd,utf8Cmd,strlen(caCmd));
					write(fdwrite,utf8Cmd,strlen(utf8Cmd));
				  
				   char oneservice[32];
				  sprintf(oneservice,"%s","|һ������:");
				  strgb2utf8(oneservice,utf8Cmd,strlen(oneservice));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[19]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char twoservice[32];
					sprintf(twoservice,"%s","|��������:");
				  strgb2utf8(twoservice,utf8Cmd,strlen(twoservice));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[20]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					char matchrule[32];
					sprintf(matchrule,"%s","|ƥ�����:");
				  strgb2utf8(matchrule,utf8Cmd,strlen(matchrule));
				  sprintf(caCmd,"%s%s",utf8Cmd,vv[21]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
					char tmpvpath[32];
					sprintf(tmpvpath,"%s","|����ͨ��:");
					strgb2utf8(tmpvpath,utf8Cmd,strlen(tmpvpath));
					sprintf(caCmd,"%s%s",utf8Cmd,vv[22]);
					write(fdwrite,caCmd,strlen(caCmd));
					
					
				}
					
				if(searchconX){
						if(searchconX[0] ){
							char keyword[32];
							sprintf(keyword,"%s","|�ؼ���:");
						  strgb2utf8(keyword,utf8Cmd,strlen(keyword));
							sprintf(caCmd,"%s%s",utf8Cmd,searchconX);
							write(fdwrite,caCmd,strlen(caCmd));
						}
				}
				
		    close(fdwrite);
  
    }
    
	  if(showtype==1){
	 		 sprintf(caFile1,"%s/%s/iplog/iplog-%s.csv",_datacenterdir,seldate,seltime);
	 	}else if(showtype==2){
	 		 sprintf(caFile1,"%s/%s/ipblock/ipblocklog-%s.csv",_datacenterdir,seldate,seltime);
		}
		
		printf("file:%s\n",caFile1);
	
    iReturn = access(caFile1,F_OK);
    if(iReturn!=0){
		    ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");						
				ltMsgFree(ltMsgPk);
				 			 
				return 0;
    }
   
    fd=open(caFile1,O_RDONLY | O_NDELAY);
    if(fd<1){
        ltMsgPk->msgpktype=1;
		   	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
		    ltMsgFree(ltMsgPk);		    
		    return 0;
    }
    
    
   if(exportaction==NULL){//��������������
			  sprintf(caFile2,"echo '1'>%s/proccess.ini",caTempDir);//��ʾ��ǰĿ¼��proccess.ini�ļ�
			  system(caFile2);
			  sprintf(caFile2,"%ssearch.csv",caTempDir);
			  fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
			  if(fdwrite == (-1)) {
					 ltMsgPk->msgpktype=1;
					 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					 ltMsgFree(ltMsgPk);
					 close(fd);				 
					 return 0;
				}
	  }else{//ֱ�ӵ�����ʱ������
			  memset(tmpdir,0,sizeof(tmpdir)); 
		    sprintf(tmpdir,"%lu",ltStrGetId());
		    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
				if(ltFileIsExist(caTempDir)!=1){
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					  ltMsgFree(ltMsgPk);
					  return 0;
					}
				}
				
	 		 	sprintf(caFile2,"%ssearch.csv",caTempDir);
			  fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
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
			if(nownum>=100000){
				break;
			}
			if (getln(&ss,&line,&match,'\n') == -1){
				break;
			}
			if (!match && !line.len) {
				break; 
			}		
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
					 iIndex++;						 
					 if(vv[iIndex]){					    					 	
					   if(vv[iIndex][0]){ 
							   if(memmem(&line.s[iiii],jjjj-iiii,vv[iIndex],strlen(vv[iIndex]))){ 
							   			nownum++; 		  
		 									write(fdwrite,line.s,line.len);	  
								 	 	 	kkkk=1;
						 	 	 	    break;
							   }
					   }
					 }	 	 
					 printf("jjjj:%d\n",jjjj);
					 jjjj++;
					 iiii=jjjj;
					 printf("iiii:%d\n",iiii);
			  }			
			}

			if(kkkk==1){
	      continue;
	    } 
	  
	  
    }
  
    close(fd);
    close(fdwrite);        
    sprintf(caFile2,"echo '%d'>%ssearch.count",nownum,caTempDir);
    system(caFile2);
    sprintf(caFile2,"rm -f %sproccess.ini",caTempDir);
    system(caFile2);
    

	  memset(caLine,0,sizeof(caLine));   
    if(showtype==1){
	 		 sprintf(caLine,"%sIplogConvert %s search",JAVACMD,caTempDir);	   
	 	}else if(showtype==2){
	 		 sprintf(caLine,"%sIpblockConvert %s search",JAVACMD,caTempDir);	   
		}
		chdir(BINCMD);
		system(caLine);
        
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);		
   	ltMsgFree(ltMsgPk);
   	
    return 0;
  
}

/*
MSA:��������
    ��ϸ���ʼ�¼
        ������ϼ�¼����
����:��¼�������������룬�Լ�gird�����¼ɾ������
*/
int msaNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	stralloc strTemp;
  char caTmpp[2048];
  char caFile1[256];
  int  fileproc;
  struct stat st;
  msasDir *fnames;
  char   *manageruser;
  char   *caDelFile;
  char   *caSdate;
  char   sname[256];
  char   sDate[256];
  char   suser[256];
  char   stype[256];
  char   searchcon[256];
  int    iii;
  int    jjj;
  int    allnum;
  int    len=0; 
  int    showtype;
	if(ltMsgGetVar_s(ltMsgPk,"showtype")){
			showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));	
	}
	if(ltMsgGetVar_s(ltMsgPk,"msa.login.account")){
			manageruser=ltMsgGetVar_s(ltMsgPk,"msa.login.account");
	}
  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
  caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");

  if(showtype==1){
  	sprintf(caFile1,"%s/ipsearch/",_datacenterdir);
  }else if(showtype==2){
  	sprintf(caFile1,"%s/ipblocksearch/",_datacenterdir);
  }
	if(caDelFile){ 
	       char caCmd[256];
	       if(strlen(caDelFile)>1){	           
	           if(showtype==1){
						  	sprintf(caCmd,"rm -Rf %s/ipsearch/%s",_datacenterdir,caDelFile);
						 }else if(showtype==2){
						  	sprintf(caCmd,"rm -Rf %s/ipblocksearch/%s",_datacenterdir,caDelFile);
						 }
	           system(caCmd);
	       }
	       sleep(1);
	}

 fnames=mydirlist(caFile1,&len);
 if(fnames){
        qsort(fnames,len,sizeof(msasDir),dirtimecmp); //���ļ�������ĸ����
 }

 allnum=0;
 
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 stralloc_cats(&strTemp,caTmpp);
 jjj=0;
 while(len--) 
 {               
              iii=strlen(manageruser);
              printf("iii:%d\n",iii);
              printf("caDirName:%s\n",fnames[len].caDirName);
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
						  	sprintf(caFile1,"%s/ipsearch/%s/proccess.ini",_datacenterdir,fnames[len].caDirName);
						  }else if(showtype==2){
						  	sprintf(caFile1,"%s/ipblocksearch/%s/proccess.ini",_datacenterdir,fnames[len].caDirName);
						  }
              if(stat(caFile1,&st) == 0){ 
                    fileproc=1; 
              }
              if(showtype==1){
						  	sprintf(caFile1,"%s/ipsearch/%s/info.ini",_datacenterdir,fnames[len].caDirName);
						  }else if(showtype==2){
						  	sprintf(caFile1,"%s/ipblocksearch/%s/info.ini",_datacenterdir,fnames[len].caDirName);
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
 
 printf("json:%s\n",strTemp.s);
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
 ltMsgFree(ltMsgPk);
 
 return 0;
}



/*
MSA:��������
    ��ϸ���ʼ�¼
        ������ʼ�¼
        ������ϼ�¼
����:���ر�ʱ���ļ�
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
MSA:��������
				�Ƿ�����XLS�ļ��ж�
*/
int msaIfCreatXlsOK(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char caFile[512];
	 char *tempdir=NULL;
	 int iReturn;
	 iReturn=-1;
	 memset(caFile,0,sizeof(caFile));
	 memset(tempdir,0,sizeof(tempdir));
	 if(ltMsgGetVar_s(ltMsgPk,"tempdir")!=NULL){
	 		tempdir=ltMsgGetVar_s(ltMsgPk,"tempdir");
	 }else{
	 	  tempdir="";
	 }
	 sprintf(caFile,"%s/%s/download.xls",_TMPDIR,tempdir);
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
MSA:��������
		��ʱͨ�ż�¼
				MSN,Yahoo,QQ,�Ա�,����,skype
����:ҳ�����ӵ��ú���
*/
int ltimshowwebquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;

  char strBuf[512];
	char *gid;
	char *uid;
	char *host;
	char *sdate;
	char *osort;
	char *orderby;
  char *sFlag;
  char fWhere[128];
  char tempDate[32];
	char ToDate[32];
	memset(tempDate,0,sizeof(tempDate));
	memset(ToDate,0,sizeof(ToDate));
 
  int  Num=0;
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
  uid=ltMsgGetVar_s(ltMsgPk,"uid");
  host=ltMsgGetVar_s(ltMsgPk,"host");
  sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
  orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
  sFlag=ltMsgGetVar_s(ltMsgPk,"flag");
  if(!gid){
	 		gid="";
	}
	if(!uid){
	 		uid="";
	}

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

	if(sFlag){
  	sprintf(fWhere,"tse%s",sFlag);
  	lt_dbput_rootvars(dbPtr,1,fWhere," selected");
  }

  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=1 order by sdate desc");
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	ToDate[0]=sdate[0];
	ToDate[1]=sdate[1];
	ToDate[2]=sdate[2];
	ToDate[3]=sdate[3];
	ToDate[4]='-';
	ToDate[5]=sdate[4];
	ToDate[6]=sdate[5];
	ToDate[7]='-';
	ToDate[8]=sdate[6];
	ToDate[9]=sdate[7];
	ToDate[10]='\0';
  lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,sdate,"strdate",LT_TYPE_STRING,ToDate);
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//�����м�¼
			Num++;		
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
			if(strcmp(tempRow[0],sdate)!=0){
         lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,tempRow[0],"strdate",LT_TYPE_STRING,ToDate);
			}
      tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);
  
  lt_dbput_rootvars(dbPtr,1,"gid",gid);
  lt_dbput_rootvars(dbPtr,1,"uid",uid);
  lt_dbput_rootvars(dbPtr,1,"myhost",host);
  lt_dbput_rootvars(dbPtr,1,"myflag",sFlag);
  lt_dbput_rootvars(dbPtr,1,"mysdate",sdate);
  lt_dbput_rootvars(dbPtr,1,"myosort",osort);
  lt_dbput_rootvars(dbPtr,1,"orderby",orderby);
  
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msaimsearb.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);

  return 0;
}


/*
MSA:��������
				��ʱͨ�ż�¼
				    MSN,Yahoo,�Ա�,����,qq,skype
����:��¼���ݲ�ѯ ���� ɾ��
*/
int ltwebqueryima(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char strBuf[1024];
    char delWhere[512];
    char caWhere[512];
    char *caSdate=NULL;
    char *caHost=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;
    char *flag=NULL;
    char *gid=NULL;
    int intGid,glevel,gpid;
    char caTmpp[2048];
    char tabname[256]; 
    char group_name[256];
    char user_name[256];
    char servic_name[256];
    char key_name[256];

    memset(group_name,0,sizeof(group_name));
	  memset(servic_name,0,sizeof(servic_name));
	  memset(key_name,0,sizeof(key_name));
	  memset(user_name,0,sizeof(user_name));

    stralloc strTemp;
    strTemp.s=0;

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
    flag=ltMsgGetVar_s(ltMsgPk,"flag");//ͨ������
    caHost=ltMsgGetVar_s(ltMsgPk,"host");//host�ؼ���
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");// ����ʽ
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");//�� ��
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");  
    sprintf(tabname,"msaimlog%s",caSdate);
    sprintf(servic_name,"%s","-");
    
    intGid=atol(gid);
    glevel=0;gpid=0;
    
    
		lRowNum=100; /*ÿҳ������*/
		lSumRec=0;/*������*/
		lCount=0;
		lStartRec=0;
		lNum=0; /*��ҳʣ�������*/

		if(ltMsgGetVar_s(ltMsgPk,"limit")){
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}


		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));
		/*��ȡ���Ź�������*/
		sprintf(strBuf,"select level,pid,name from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				gpid=atol(tempRow[1]);
				sprintf(group_name,"%s",tempRow[2]);
			}
		}
		ltDbCloseCursor(tempCursor);

    if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"%s","2=2");//��ѯ�����û�
			       sprintf(delWhere,"%s","2=2");
			       sprintf(group_name,"%s","Allgroup");
				}
		}

		if(strcmp(gid,"-1")){//������-1
			  if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//�������������û�
			       sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
			       sprintf(user_name,"%s","Allusers");
				}else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//��ѯ�����û�
						 sprintf(delWhere,"userid = %s",caUser);
				}
		}
		if(flag!=NULL){//ͨ������
	    if(strcmp(flag,"0")!=0){//flag:0 ������
	    		sprintf(caWhere,"%s and (b.flags = '%s') ",caWhere,flag);
	        sprintf(delWhere,"%s and (flags='%s') ",delWhere,flag);	
	    }
	  }
    if(caHost!=NULL){//�ؼ��ִ���
	    if(strlen(caHost)>0) {
	        sprintf(caWhere,"%s and (b.imcon like '%c%s%c') ",caWhere,'%',caHost,'%');
	        sprintf(delWhere,"%s and (imcon like '%c%s%c') ",delWhere,'%',caHost,'%');
	        sprintf(key_name,"%s",caHost);
	    }else{
	    	  sprintf(key_name,"%s","-");
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
    	    char caTempDir[256];
			    char caUrl1[200];
			    char tmpdir[128];
			    int  lTime;
			    char sDate[36];
			    char caFile[256];
			    char caLine[2048];
			    int fd=0;
	    	  int flag=0;
	   			char gname[512];
		   		int  pid;
		   		int  i;
		   		pid=0;
		   		memset(caFile,0,sizeof(caFile));
   	      memset(gname,0,sizeof(gname));
			    memset(sDate,0,sizeof(sDate));
			    lTime = time(0);
				  nasTimeGetDate(sDate,lTime);
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
		 			
	 		memset(tmpdir,0,sizeof(tmpdir)); 
	    sprintf(tmpdir,"%lu",ltStrGetId());
	    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
			if(ltFileIsExist(caTempDir)!=1){
				if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
					ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				  ltMsgFree(ltMsgPk);
				  return 0;
				}
			}
			
 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
		  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		  if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 close(fd);				 
				 return 0;
			} 	
			
			sprintf(strBuf,"select a.dispname,b.sdate,b.stime,b.imfrom,b.imto,b.imcon,b.flags,c.name from %s b left join msauser a on a.userid=b.userid  left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
			tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
					 tempRow=ltDbFetchRow(tempCursor);
				   while(tempRow!=NULL){	
				   	    flag++;		   	   
				   	    if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[7]);
		             }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		             }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		             }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		             }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	             	  
		             }	
				   	   
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
							
							sprintf(servic_name,"%s",caUrl1);
						  sprintf(caLine,"%d|%s|%s|%s|%s|%s|%s|%s|%s\r\n",flag,
						    gname,//GroupName
								tempRow[0],//dispname
								tempRow[1],//sdate
								tempRow[2],//stime
								tempRow[3],//imfrom
								tempRow[4],//imto
								tempRow[5],//imcon
								caUrl1);
								
						if(strcmp(user_name,"Allusers")!=0){//�����û�
								sprintf(user_name,"%s",tempRow[0]);
						}
						 write(fd,caLine,strlen(caLine));
						 tempRow= ltDbFetchRow(tempCursor);
					 }
				   ltDbCloseCursor(tempCursor);
			}
			
		   memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"%sImToexcel %s %s %s %s %s %s search",JAVACMD,group_name,user_name,servic_name,sDate,key_name,caTempDir);
			 close(fd);	
			 chdir(BINCMD);
			 system(caLine);
			 
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
			 ltMsgFree(ltMsgPk);
			 
			 return 0;

    }
  	sprintf(strBuf,"select count(*) from %s b  left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s ",tabname,glevel,caWhere);
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
	       char gname[512];
		   	 int  pid;
		   	 int  i;
		   	 pid=0;
   	     memset(gname,0,sizeof(gname));
   	     memset(caUrl,0,sizeof(caUrl));
	       lNum++;       
				 if(glevel==0){
         	  sprintf(gname,"%s",tempRow[8]);
         }else if(glevel==1){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	break;
         	  	  }
         	  }   

         }else if(glevel==2){
           	for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	           	  
         }else if(glevel==3){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	
         }else if(glevel==4){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	             	  
         }	
        
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
					     gname,tempRow[7],tempRow[0],tempRow[1], tempRow[2],tempRow[3],tempRow[4],tempRow[5],caUrl);
						   stralloc_cats(&strTemp,caTmpp);
				}else{
					  sprintf(caTmpp,",{\"mygroup\":\"%s\",\"uid\":\"%s\",\"uname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myimfrom\":\"%s\",\"myimto\":\"%s\",\"myimcon\":\"%s\",\"myflags\":\"%s\"}",
					     gname,tempRow[7],tempRow[0],tempRow[1], tempRow[2],tempRow[3],tempRow[4],tempRow[5],caUrl);
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
MSA:��������
				�ļ������¼
����:�ļ������¼���ڵ�����ҳ����ú���
*/
int ltmsafiletranquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltTablePtr  tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;

	char strBuf[512];
	char *gid;
	char *uid;
	char *host;
	char *sdate;
	char *osort;
	char *orderby;
	char *sFlag;
	char fWhere[128];
	char tempDate[32];
	char ToDate[32];
	memset(tempDate,0,sizeof(tempDate));
	memset(ToDate,0,sizeof(ToDate));
	int Num=0;

	//���ݿ�����
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
	uid=ltMsgGetVar_s(ltMsgPk,"uid");
  host=ltMsgGetVar_s(ltMsgPk,"host");
  sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
  orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
	sFlag=ltMsgGetVar_s(ltMsgPk,"trantype");
	if(!gid){
	 		gid="";
	}
	if(!uid){
	 		uid="";
	}

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

	if(sFlag){
		sprintf(fWhere,"tse%s",sFlag);
		lt_dbput_rootvars(dbPtr,1,fWhere," selected");
	}

	memset(strBuf,0,sizeof(strBuf));
	Num=0;
	sprintf(strBuf,"select  sdate,tabname from msaloglist where tabtype=3  order by sdate desc");
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	ToDate[0]=sdate[0];
	ToDate[1]=sdate[1];
	ToDate[2]=sdate[2];
	ToDate[3]=sdate[3];
	ToDate[4]='-';
	ToDate[5]=sdate[4];
	ToDate[6]=sdate[5];
	ToDate[7]='-';
	ToDate[8]=sdate[6];
	ToDate[9]=sdate[7];
	ToDate[10]='\0';
	lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,sdate,"strdate",LT_TYPE_STRING,ToDate);
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
		tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//�����м�¼
			Num++;		
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
			if(strcmp(tempRow[0],sdate)!=0){
         lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,tempRow[0],"strdate",LT_TYPE_STRING,ToDate);
			}
		
			tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);
	

  lt_dbput_rootvars(dbPtr,1,"gid",gid);
  lt_dbput_rootvars(dbPtr,1,"uid",uid);
  lt_dbput_rootvars(dbPtr,1,"myhost",host);
  lt_dbput_rootvars(dbPtr,1,"myflag",sFlag);
  lt_dbput_rootvars(dbPtr,1,"mysdate",sdate);
  lt_dbput_rootvars(dbPtr,1,"myosort",osort);
  lt_dbput_rootvars(dbPtr,1,"orderby",orderby);
  
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msafiletransearch.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);

	return 0;
}



/*
MSA:��������
				�ļ������¼
����:�ļ������¼��ѯ��������ɾ��
*/
int ltmsafilequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//ɾ������
    char caWhere[512];
    char *caSdate=NULL;
    char *caHost=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;
    char *flag=NULL;
    char *gid=NULL;
    int  intGid,glevel,gpid;
    char caTmpp[2048];
    char tabname[256];//�ļ������¼����
    
    char group_name[256];
    char user_name[256];
    char servic_name[256];
    char key_name[256];

    memset(group_name,0,sizeof(group_name));
	  memset(servic_name,0,sizeof(servic_name));
	  memset(key_name,0,sizeof(key_name));
	  memset(user_name,0,sizeof(user_name));

    stralloc strTemp;
    strTemp.s=0;

		//���ݿ�����
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
    flag=ltMsgGetVar_s(ltMsgPk,"flag");//����
    caHost=ltMsgGetVar_s(ltMsgPk,"host");//host�ؼ���
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");// ����ʽ
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");//�� ��
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    intGid=atol(gid);
    glevel=0;gpid=0;    
    sprintf(tabname,"msafilelog%s",caSdate);
    sprintf(servic_name,"%s","-");
    
    lRowNum=100; /*ÿҳ������*/
		lSumRec=0;/*������*/
		lCount=0;
		lStartRec=0;
		lNum=0; /*��ҳʣ�������*/
    
		if(ltMsgGetVar_s(ltMsgPk,"limit")){
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}
	
		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));

		/*��ȡ���Ź�������*/
		sprintf(strBuf,"select name,level,pid from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					glevel=atol(tempRow[1]);
					gpid=atol(tempRow[2]);
					sprintf(group_name,"%s",tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);


	if(strcmp(gid,"-1")==0){
				if(strcmp(caUser,"-1")==0){
					   sprintf(caWhere,"%s","2=2");//��ѯ�����û�
					   sprintf(delWhere,"%s","2=2");
					   sprintf(group_name,"%s","Allgroup");
				}
	}

	if(strcmp(gid,"-1")){//������-1
		   if(strcmp(caUser,"-1")==0){
			    sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//�������������û�
			    sprintf(delWhere,"a.groupid%d=%d",glevel,intGid);
			    sprintf(user_name,"%s","Allusers");
			}else{
					sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  Ϊb��
					sprintf(delWhere,"userid =%s",caUser);
			}
	}

	if(flag!=NULL){//�������ʹ���
		if(strcmp(flag,"0")){
			sprintf(caWhere,"%s and (b.tantype = '%s') ",caWhere,flag);
			sprintf(delWhere,"%s and (tantype='%s') ",delWhere,flag);
		}
	}

	 if(caHost!=NULL){//�ؼ��ִ���
		if(strlen(caHost)>0) {
			sprintf(caWhere,"%s and (b.contenttype like '%c%s%c') ",caWhere,'%',caHost,'%');
			sprintf(delWhere,"%s and (contenttype like '%c%s%c') ",delWhere,'%',caHost,'%');
			sprintf(key_name,"%s",caHost);
		}else{
			sprintf(key_name,"-");
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
    			char caTempDir[512];
    	    char tmpdir[128];
			    int  lTime;
			    char sDate[36];
			    char caFile[256];
			    char caLine[2048];
			    int fd=0;
	    	  int flag=0;
	   			char gname[512];
		   		int  pid;
		   		int  i;
		   		pid=0;
		   		memset(caFile,0,sizeof(caFile));
   	      memset(gname,0,sizeof(gname));
			    memset(sDate,0,sizeof(sDate));
			    lTime = time(0);
				  nasTimeGetDate(sDate,lTime);

          
				  if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    	if(strcmp(caOrder,"nosort")!=0){
				        if(strcmp(caOrder,"date")==0){//����������
				            sprintf(caWhere,"%s order by b.stime ",caWhere);
				        }else if(strcmp(caOrder,"filename") == 0){//���û�id����
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
		      
	 		memset(tmpdir,0,sizeof(tmpdir)); 
	    sprintf(tmpdir,"%lu",ltStrGetId());
	    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
			if(ltFileIsExist(caTempDir)!=1){
				if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
					ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				  ltMsgFree(ltMsgPk);
				  return 0;
				}
			}
				
 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
		  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		  if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 close(fd);				 
				 return 0;
			} 	  
		      
			memset(sqlBuf,0,sizeof(sqlBuf));
	    sprintf(sqlBuf,"select a.dispname,b.tantype,b.stime,b.filename,b.fileurl,b.contenttype,c.name from %s b left join msauser a on a.userid=b.userid  left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		  if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);					
		       	while(tempRow!=NULL){
		       			 flag++;
		       		
		       		   if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[6]);
		             }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[6]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		             }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[6]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		             }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[6]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		             }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[6]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	             	  
		             }	
		       		
							sprintf(caLine,"%d|%s|%s|%s|%s|%s|%s|%s\r\n",
							        flag,
							        gname,//����
											tempRow[0],//�û���								
											tempRow[1],//��������
											tempRow[2],//����ʱ��
											tempRow[3],//�ļ���
											tempRow[4],//�ļ�λ��
											tempRow[5] //�ļ�����
							);
							
						 sprintf(servic_name,"%s",tempRow[1]);
						 if(strcmp(user_name,"Allusers")!=0){//�����û�
								sprintf(user_name,"%s",tempRow[0]);
						 }
							
							write(fd,caLine,strlen(caLine));
							tempRow= ltDbFetchRow(tempCursor);
						}
		     ltDbCloseCursor(tempCursor);
		  }
				  
			 memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"%sFilelogToexcel %s %s %s %s %s %s search",JAVACMD,group_name,user_name,servic_name,sDate,key_name,caTempDir);
		   close(fd);	
			 chdir(BINCMD);
			 system(caLine);
			 
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
			 ltMsgFree(ltMsgPk);
				  
			 return 0;
    }

    memset(strBuf,0,sizeof(strBuf));
    sprintf(strBuf,"select count(*) from %s b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s ",tabname,glevel,caWhere);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	  if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
	  }
    ltDbCloseCursor(tempCursor);

    //����
     if(caOrder!=NULL){
				    if( strlen(caOrder)>0 ) {
				    	if(strcmp(caOrder,"nosort")!=0) {
					        if(strcmp(caOrder,"date")==0){//����������					        	  
					            sprintf(caWhere,"%s order by b.stime ",caWhere);
					        }else if(strcmp(caOrder,"filename") == 0) {//���û�id����
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

    //json����
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select a.dispname,b.tantype,b.stime,b.filename,b.fileurl,b.contenttype,a.userid,c.name from %s b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
		sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
		stralloc_cats(&strTemp,caTmpp);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		lNum=0;
    if(tempCursor!=NULL){
	     tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
					  lNum++;
		   			char gname[512];
			   		int  pid;
			   		int  i;
			   		pid=0;
	   	      memset(gname,0,sizeof(gname));
	   	      
					  if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[7]);
		             }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		             }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		             }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		             }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	             	  
		             }	
					  
						if(lNum==1){
									sprintf(caTmpp,"{\"uname\":\"%s\",\"mytantype\":\"%s\",\"mystime\":\"%s\",\"myfilename\":\"%s\",\"myfileurl\":\"%s\",\"mycontenttype\":\"%s\",\"userid\":\"%s\",\"mygroup\":\"%s\",\"mysdate\":\"%s\"}",
											tempRow[0],
											tempRow[1],
											tempRow[2],
											tempRow[3],
											tempRow[4],
											tempRow[5],
											tempRow[6],
											gname,
											caSdate
											);
								   stralloc_cats(&strTemp,caTmpp);
						}else{
								sprintf(caTmpp,",{\"uname\":\"%s\",\"mytantype\":\"%s\",\"mystime\":\"%s\",\"myfilename\":\"%s\",\"myfileurl\":\"%s\",\"mycontenttype\":\"%s\",\"userid\":\"%s\",\"mygroup\":\"%s\",\"mysdate\":\"%s\"}",
											tempRow[0],
											tempRow[1],
											tempRow[2],
											tempRow[3],
											tempRow[4],
											tempRow[5],
											tempRow[6],
											gname,
											caSdate
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
MSA:��������
����:�û�combox��json����
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

	  /*��ȡ���Ź�������*/
	  sprintf(strBuf,"select level from msagroup where id=%d",intGid);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
			}
		}
	  ltDbCloseCursor(tempCursor);

  /*�����¼��Ŀ*/
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


	/*ѡ���ź������û�json����*/
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



/*
MSA:��������
				���������¼
				    �ؼ����б�
����:�ؼ����б�����ҳ����ú���
*/
int ltmsasearchkeyquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	LT_DBROW tempRow;
	ltDbCursor *tempCursor;
  char strBuf[512];
	char *gid;
	char *uid;
	char *host;
	char *sdate;
	char *osort;
	char *orderby;
	char *searchkey;
  int Num;
  
  char tempDate[32];
	char ToDate[32];
	memset(tempDate,0,sizeof(tempDate));
	memset(ToDate,0,sizeof(ToDate));
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
	uid=ltMsgGetVar_s(ltMsgPk,"uid");
  host=ltMsgGetVar_s(ltMsgPk,"host");
  sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
  orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
	searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
	
  if(!gid){
	 		gid="";
	}
	if(!uid){
	 		uid="";
	}

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
  
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=7 order by sdate desc");
	tablePtr=lt_dbput_table(dbPtr,"datelist");	
	ToDate[0]=sdate[0];
	ToDate[1]=sdate[1];
	ToDate[2]=sdate[2];
	ToDate[3]=sdate[3];
	ToDate[4]='-';
	ToDate[5]=sdate[4];
	ToDate[6]=sdate[5];
	ToDate[7]='-';
	ToDate[8]=sdate[6];
	ToDate[9]=sdate[7];
	ToDate[10]='\0';
	lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,sdate,"strdate",LT_TYPE_STRING,ToDate);
	
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//�����м�¼
			  Num++;
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
					
			  if(strcmp(tempRow[0],sdate)!=0){
           lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,tempRow[0],"strdate",LT_TYPE_STRING,ToDate);
			  }			  
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);
	
	lt_dbput_rootvars(dbPtr,1,"gid",gid);
  lt_dbput_rootvars(dbPtr,1,"uid",uid);
  lt_dbput_rootvars(dbPtr,1,"myhost",host);
  lt_dbput_rootvars(dbPtr,1,"mysearchkey",searchkey);
  lt_dbput_rootvars(dbPtr,1,"mysdate",sdate);
  lt_dbput_rootvars(dbPtr,1,"myosort",osort);
  lt_dbput_rootvars(dbPtr,1,"orderby",orderby);
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msasearchkeyhead.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}


/*
MSA:��������
				���������¼
				    �ؼ����б�
����:������ѯ��������ɾ���ĵ��ú���
*/
int ltmsasearchkeyhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char delWhere[512];//ɾ������
    char caWhere[512],xDate[30];
    char *caHost=NULL;
    char *caSdate=NULL;
    char *searchkey=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *caUser=NULL;
    char *gid=NULL;
    char tabname[256];
    int  intGid,glevel,gpid;
    char caTmpp[2048];
    
    char group_name[256];
    char user_name[256];
    char host_name[256];
    char key_name[256];

    memset(group_name,0,sizeof(group_name));
	  memset(host_name,0,sizeof(host_name));
	  memset(key_name,0,sizeof(key_name));
	  memset(user_name,0,sizeof(user_name));
    
    stralloc strTemp;
    strTemp.s=0;
    
		//���ݿ�����
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
    caHost=ltMsgGetVar_s(ltMsgPk,"host");
    searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");   // ����ʽ
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");   //�� ��
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    sprintf(tabname,"msasearchkey%s",caSdate);
    intGid=atol(gid);
    glevel=0;gpid=0;

		/*��ȡ���Ź�������*/
		sprintf(strBuf,"select level,pid,name from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				gpid=atol(tempRow[1]);
				sprintf(group_name,"%s",tempRow[2]);
			}
		}
		ltDbCloseCursor(tempCursor);
		

		lRowNum=100; /*ÿҳ������*/
		lSumRec=0;/*������*/
		lCount=0; //
		lStartRec=0;//
		lNum=0; /*��ҳʣ�������*/


		if(ltMsgGetVar_s(ltMsgPk,"limit")){
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}//�൱�ڽ���λ��
		if(ltMsgGetVar_s(ltMsgPk,"start")){
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}//�൱����ʼλ��

		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));

		if(strcmp(gid,"-1")==0){
				if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"%s","2=2");
			       sprintf(delWhere,"%s","2=2");
			       sprintf(group_name,"%s","Allgroup");
				}
		}
		
		if(strcmp(gid,"-1")){//������-1
			   if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);
			       sprintf(delWhere,"%s","2=2");
			       sprintf(user_name,"%s","Allusers");
				}else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//msasearchkey  Ϊb��
						 sprintf(delWhere,"userid = %s",caUser);
				}
		}


    if(caHost!=NULL){//HOST�ؼ����ж�
	     if(strlen(caHost)>0) {
	         sprintf(caWhere,"%s and (b.host like '%c%s%c') ",caWhere,'%',caHost,'%');
	         sprintf(delWhere,"%s and (host like '%c%s%c') ",delWhere,'%',caHost,'%');
	         sprintf(host_name,"%s",caHost);
	     }else{
	     	   sprintf(host_name,"%s","-");
	     }
    }

    if(searchkey!=NULL){//�����ؼ���
	     if(strlen(searchkey)>0) {
	         sprintf(caWhere,"%s and (b.searchkey like '%c%s%c') ",caWhere,'%',searchkey,'%');
	         sprintf(delWhere,"%s and (searchkey like '%c%s%c') ",delWhere,'%',searchkey,'%');
	         sprintf(key_name,"%s",searchkey);
	     }else{
	     	   sprintf(key_name,"%s","-");
	     }
    }


    if(iAction==2){ /*del����ɾ������*/
		          if( strlen(delWhere)>0){
				         sprintf(strBuf,"delete from %s where %s ",tabname,delWhere);
				         ltDbExecSql(G_DbCon,sqlBuf);
		          }
				      ltMsgPk->msgpktype=1;
				      lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n", "lthtml",LT_TYPE_STRING,"0");
				      ltMsgFree(ltMsgPk);
				      return 0; 
    }else if(iAction==1){/*export����*/
    	        char caTempDir[512];
    	        char tmpdir[128];
			        int  lTime;
			       	char sDate[36];
					    char caFile[256];
					    char caLine[2048];
					    int  fd=0;
			    	  int  flag=0;
			   			char gname[512];
				   		int  pid;
				   		int  i;
				   		pid=0;
				   		memset(caFile,0,sizeof(caFile));
		   	      memset(gname,0,sizeof(gname));
					    memset(sDate,0,sizeof(sDate));
					    lTime = time(0);
						  nasTimeGetDate(sDate,lTime);
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
								                       ltStrAddF(caWhere," desc");
								                    }else{
								                       ltStrAddF(caWhere," desc ");
								                    }
								                }else{
								                    if(strcmp(caOrder,"date")==0) {
								                        ltStrAddF(caWhere," asc");
								                    }else{
								                        ltStrAddF(caWhere," asc ");
								                    }
								                }
								            }
						         }
					        }
						 }
						 
							memset(tmpdir,0,sizeof(tmpdir)); 
					    sprintf(tmpdir,"%lu",ltStrGetId());
					    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
							if(ltFileIsExist(caTempDir)!=1){
								if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
									ltMsgPk->msgpktype=1;
								  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
								  ltMsgFree(ltMsgPk);
								  return 0;
								}
							}
							
				 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
						  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
						  if(fd == (-1)) {
								 ltMsgPk->msgpktype=1;
								 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
								 ltMsgFree(ltMsgPk);
								 close(fd);				 
								 return 0;
							} 	
            
						 sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey,c.name from %s b left join msauser a on (a.userid=b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
				     tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				     if(tempCursor!=NULL){
						     tempRow=ltDbFetchRow(tempCursor);
							   while(tempRow!=NULL){
							   	   memset(xDate,0,sizeof(xDate));
									   nasTimeFormat(xDate,atol(tempRow[2]));
									   flag++;
		    	   				 if(glevel==0){
						         	 sprintf(gname,"%s",tempRow[4]);
						         }else if(glevel==1){
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
						         	  	  	break;
						         	  	  }
						         	  }   
						
						         }else if(glevel==2){
						           	for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	break;
						         	  	  }
						         	  }	           	  
						         }else if(glevel==3){
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }	
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	break;
						         	  	  }
						         	  }	
						         }else if(glevel==4){
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }	
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
						         	  	  	break;
						         	  	  }
						         	  }	
						         	  
						         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
						         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
						         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
						         	  	  	break;
						         	  	  }
						         	  }	             	  
						         }	
						       
						        if(strcmp(user_name,"Allusers")!=0){//�����û�
												sprintf(user_name,"%s",tempRow[0]);
										}  
										sprintf(caLine,"%d|%s|%s|%s|%s|%s\r\n",flag,gname,tempRow[0],tempRow[1],xDate,tempRow[3]);
										write(fd,caLine,strlen(caLine));
										
										tempRow=ltDbFetchRow(tempCursor);
							   }
						     ltDbCloseCursor(tempCursor);
	   				 }
	   				 
	   				 memset(caLine,0,sizeof(caLine));
					   sprintf(caLine,"%sKeywordToexcel %s %s %s %s %s %s search",JAVACMD,group_name,user_name,host_name,sDate,key_name,caTempDir);
						 close(fd);			
						 chdir(BINCMD);
						 system(caLine);
			 
						 ltMsgPk->msgpktype=1;
						 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
						 ltMsgFree(ltMsgPk);
	   				
	   				 return 0;
    }

	  sprintf(strBuf,"select count(*) from %s b  left join msauser a on (a.userid=b.userid) left join msagroup c on a.groupid%d=c.id where %s ",tabname,glevel,caWhere);
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
			      if(strcmp(caOrder,"nosort")!=0) {
					         if(strcmp(caOrder,"date")==0) {
					                sprintf(caWhere,"%s order by b.ltime ",caWhere);
					         }else if(strcmp(caOrder,"userid") == 0){
					                sprintf(caWhere,"%s order by b.userid ",caWhere);
					         }

					         if(strlen(caOrderby)){
						                if(caOrderby[0] == '1') {
						                   	if(strcmp(caOrder,"date")==0) {
						                       ltStrAddF(caWhere,"desc");
						                    }else{
						                    	 ltStrAddF(caWhere,"desc");
						                    }
						                }else{
						                     if(strcmp(caOrder,"date")==0) {
						                        ltStrAddF(caWhere,"asc");
						                     }else{
						                	    ltStrAddF(caWhere,"asc");
						                	   }
						                }
					         }

			     }
		     }
		}

		//json����
		sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   	stralloc_cats(&strTemp,caTmpp);
    sprintf(sqlBuf,"select a.dispname,b.host,b.ltime,b.searchkey,c.name,a.userid from %s b left join msauser a on (a.userid=b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		   tempRow= ltDbFetchRow(tempCursor);
		   	while(tempRow!=NULL){
		   			 lNum++;		   			 
		   			 memset(xDate,0,sizeof(xDate));
					   nasTimeFormat(xDate,atol(tempRow[2]));
					   char gname[512];
				   	 int  pid;
				   	 int  i;
				   	 pid=0;
		   	     memset(gname,0,sizeof(gname));
	   				 if(glevel==0){
		         	 sprintf(gname,"%s",tempRow[4]);
		         }else if(glevel==1){
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
		         	  	  	break;
		         	  	  }
		         	  }   
		
		         }else if(glevel==2){
		           	for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	break;
		         	  	  }
		         	  }	           	  
		         }else if(glevel==3){
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }	
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//���һ����������
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	break;
		         	  	  }
		         	  }	
		         }else if(glevel==4){
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,tempRow[4]);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }	
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		         	  	  	break;
		         	  	  }
		         	  }	
		         	  
		         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		         	  	  	sprintf(gname,"%s_%s",_ltPubInfo->_GroupList[i].name,gname);
		         	  	  	break;
		         	  	  }
		         	  }	             	  
		         }	

						if(lNum==1){ 
							 		sprintf(caTmpp,"{\"uid\":\"%s\",\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\",\"mygroup\":\"%s\"}", tempRow[5],
							 		tempRow[0],tempRow[1],xDate,tempRow[3],gname);
							  	stralloc_cats(&strTemp,caTmpp);
						}else{
						      sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\",\"host\":\"%s\",\"sdate\":\"%s\",\"searchkey\":\"%s\",\"mygroup\":\"%s\"}", tempRow[5],
						      tempRow[0],tempRow[1],xDate,tempRow[3],gname);
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

