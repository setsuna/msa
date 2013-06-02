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

/*
*在线管理员
*/
int msaAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int i,rightype;
	char caSel[32];
	char sqlBuf[1024],groupmanager[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char     caRight[128];
	long inum;
	char caTime[40];
  char *strUserName;
  rightype=0;
  strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
  if(!strUserName){
  	strUserName="";
  }
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"list");
  sprintf(sqlBuf,"select name,lright,lasttime,bindip,substr(grouplist1,1,200),substr(grouplist2,1,200),substr(grouplist3,1,200),substr(grouplist4,1,200),substr(grouplist5,1,200) from msaadmuser");
	inum=0;
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			inum++;
			nasTimeFormat(caTime,atol(tempRow[2]));
			lt_dbput_recordvars(tablePtr,3,
				"num",LT_TYPE_LONG,inum,
				"logname",LT_TYPE_STRING,tempRow[0],
				"lasttime",LT_TYPE_STRING,caTime);
				
			if(strcmp(strUserName,tempRow[0])==0){
				lt_dbput_rootvars(dbPtr,1,"logname",tempRow[0]);
				memset(caRight,0,sizeof(caRight));
				sprintf(caRight,"%s",tempRow[1]);
				lt_dbput_rootvars(dbPtr,1,"userright",caRight);
				for(i=0;i<9;i++){
					sprintf(caSel,"c%d",i+1);
					if(caRight[i]=='1'){
						lt_dbput_rootvars(dbPtr,1,caSel,"true");
					}else{
						lt_dbput_rootvars(dbPtr,1,caSel,"false");
						}
				}
				lt_dbput_rootvars(dbPtr,1,"bindip",tempRow[3]);
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	if(ltMsgGetVar_s(ltMsgPk,"rightype")!=NULL){
		rightype=atol(ltMsgGetVar_s(ltMsgPk,"rightype"));
		}
	
	if(rightype==0){
			lt_dbput_rootvars(dbPtr,8,
                			 "c1","false",
                       "c2","false",
                       "c3","false",
                       "c4","false",
                       "c5","false",
                       "c6","false",
                       "c7","false",
                       "c8","false"
                       );
    lt_dbput_rootvars(dbPtr,1,"grouptype","1");
    }else {
    	lt_dbput_rootvars(dbPtr,1,"grouptype","3");
    	}
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/adminuser.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}
/*
*密码修改判断
*/
int ltPassIsOk(char *InStr)
{
   register int i;
   int   findD;
   int   findS;
   findD=0;
   findS=0;
   for(i=0;InStr[i]!=0;i++) {
     if(InStr[i]>'9'||InStr[i]<'0') {
         findS=1;
     }else{
       findD=1;
     }
   }
   if(findD && findS){
   			return 1;
   }
   return 0;
}

/*
*管理员操作
*/
int msaAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[1024];
	char *strUserName;
	char *strPassword;
  char newPass[50],g1[256],g2[256],g3[256],g4[256],g5[256];
	//char sRight[100];

	int  i;
	char caSel[20];
	char sIp[24];
	char caMsg[256];
	int  passisok;
	char *bindip;
  int  actiontype;
  char *strUserRight;
  char *strGroupManager;
  
  
  actiontype=0;
	passisok=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));
	//memset(sRight,0,sizeof(sRight));

	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));

	if(actiontype==1){/*add*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			if(!bindip){
				bindip="";
			}
			passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
  			
				strUserRight=ltMsgGetVar_s(ltMsgPk,"userright");
				strGroupManager=ltMsgGetVar_s(ltMsgPk,"groupmanager");
				memset(g1,0,sizeof(g1));
			  memset(g2,0,sizeof(g2));
			  memset(g3,0,sizeof(g3));
			  memset(g4,0,sizeof(g4));
				memset(g5,0,sizeof(g5));
				memcpy(g1,strGroupManager,200);
				memcpy(g2,strGroupManager+200,200);
				memcpy(g3,strGroupManager+400,200);
				memcpy(g4,strGroupManager+600,200);
				memcpy(g5,strGroupManager+800,200);
   			sprintf(sqlBuf,"insert into msaadmuser(name,password,lright,lasttime,bindip,grouplist1,grouplist2,grouplist3,grouplist4,grouplist5)values ('%s','%s','%s',%ld,'%s','%s','%s','%s','%s','%s')",
					strUserName,
					newPass,
					strUserRight,//sRight
					time(0),bindip,g1,g2,g3,g4,g5);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"add new user %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==2){/*del*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			sprintf(sqlBuf,"delete from msaadmuser where name='%s' ",	strUserName);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"delete %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==3){/*update info*/
			memset(strUserRight,0,sizeof(strUserRight));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			memset(strUserRight,'0',9);	
			for(i=0;i<9;i++){
				sprintf(caSel,"chr%d",i+1);
				if(ltMsgGetVar_s(ltMsgPk,caSel)!=NULL){
					strUserRight[i]='1';
				}
			}
			strUserRight=atol(ltMsgGetVar_s(ltMsgPk,"userright"));
			int myRight;
  			sprintf(myRight,"%d",strUserRight);//%d代表strUserRight  这句是把strUserRight打成文本付给myRight
			sprintf(sqlBuf,"update msaadmuser set lright='%s' where name='%s' ",myRight,strUserName);//sRight
		  ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"update %s right success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
			
	}else if(actiontype==4){/*change password indfo*/
			memset(newPass,0,sizeof(newPass));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			passisok=0;
		  passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr,confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			sprintf(sqlBuf,"update msaadmuser set password='%s' where name='%s' ",newPass,strUserName);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"change %s password success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==5){/*change ip*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			sprintf(sqlBuf,"update msaadmuser set bindip='%s'where name='%s' ",bindip,strUserName);
		  ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"update %s ip success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}
	msaAdminUserList(confd,ltMsgPk,lt_MMHead);
	return 0;
}

/*
*在线管理员列表
*/
int msaAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   stralloc strTemp;
   int  allcount;
   int  i,k;
   char caTmpp[512];
   
   allcount=0;
   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime)<7200)  ){
  			allcount++;   
  	}
  }
   
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;

   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime) < 7200)  ){
  			struct in_addr s;
  			char   xStatus[24];
  			s.s_addr = htonl(_ltPubInfo->admSession[i].lSip);
  			if(strcmp(_ltPubInfo->admSession[i].lright,"0000000000000000000000000000000000000000000")==0){
		  		sprintf(xStatus,"%s","Trying");
		  	}else{
		  		sprintf(xStatus,"%s","Success");
		    }
  			if(k==0){
	     	  	 sprintf(caTmpp,"{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}else{
	     	  	 sprintf(caTmpp,",{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}
	     	k++; 
  	}
  } 
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//强制下线////////////////////////////////////////////////////////////////////////////////////////////////////
int msaCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int  iindex;
	char strIndex[12];
	memset(strIndex,0,sizeof(strIndex));
  ltMsgGetSomeNVar(ltMsgPk,1,"xindex", LT_TYPE_STRING, 12, strIndex);
	iindex=atol(strIndex);
	if(iindex<20 && iindex>-1 ){
	     _ltPubInfo->admSession[iindex].lSip=0;
			_ltPubInfo->admSession[iindex].trytimes=0;
		  _ltPubInfo->admSession[iindex].lSid=0;
		  sprintf(_ltPubInfo->admSession[iindex].lright,"%s","0000000000000000000000000000000000000000000");		     
  }
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"SUCCESS"); 
	ltMsgFree(ltMsgPk);
	return 0;
}

/*
*管理员操作日志
*/
int msaAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
    char *caName;
    char *caSubject;
    char *caloglevel;
    char *cadtype;
    char *casdate;
    char *caedate;
    char *casdate3;
    char *castime;
    char *caetime;
    char caWhere[512];
    stralloc strTemp;
    char *exportaction;
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
    caloglevel=ltMsgGetVar_s(ltMsgPk,"loglevel");
    cadtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    casdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caedate=ltMsgGetVar_s(ltMsgPk,"edate");
    casdate3=ltMsgGetVar_s(ltMsgPk,"sdate3");
    castime=ltMsgGetVar_s(ltMsgPk,"stime");
    caetime=ltMsgGetVar_s(ltMsgPk,"etime");
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(caSubject && caSubject[0] ){
    	sprintf(caWhere,"%s and subject like '%c%s%c'",caWhere,'%',caSubject,'%');
    }
    if(caloglevel && caloglevel[0] && caloglevel[0]!='0' ){
    	sprintf(caWhere,"%s and  loglevel = '%s'",caWhere,caloglevel);
    }
    if(cadtype==NULL){
    	cadtype="0";
    }
    if( cadtype[0]=='1' ){
    	if(casdate && casdate[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s 00:00:00'",caWhere,casdate);
	    }
    	if(caedate && caedate[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s 00:00:00'",caWhere,caedate);
	    }
    }else if( cadtype[0]=='2' ){
    	if(casdate3 && casdate3[0] && castime && castime[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s %s'",caWhere,casdate3,castime);
	    }
    	if(casdate3 && casdate3[0] && caetime && caetime[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s %s'",caWhere,casdate3,caetime);
	    }
    }
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	 char caFileName[128];
       sprintf(caFileName,"admlogexport-%d.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   sprintf(strBuf,"select name,sdate,loglevel,subject from msaadmlog where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){		
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);	
			     	write(confd,caTmpp,strlen(caTmpp));
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=50; /*每页的行数*/
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
    sprintf(strBuf,"select count(*) from msaadmlog where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select name,sdate,loglevel,subject from msaadmlog where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){		
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"name\":\"%s\",\"sdate\":\"%s\",\"loglevel\":\"%s\",\"subject\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"name\":\"%s\",\"sdate\":\"%s\",\"loglevel\":\"%s\",\"subject\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }			
				tempRow= ltDbFetchRow(tempCursor);
				k++;
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
*设备重启
*/
int msaSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s reboot system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/msashutdownall");	
    system("/sbin/reboot");	  
    system("/sbin/reboot");
    return 0;
}
/*
*设备关机
*/
int msaSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s shutdown system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/msashutdownall");	
    system("/sbin/poweroff");
    system("/sbin/poweroff");	  
    system("/sbin/init 0");	   
    return 0;
}
