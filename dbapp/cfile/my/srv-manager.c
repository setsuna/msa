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
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char     caRight[128];
	long inum;
	char caTime[40];
  char *strUserName;
  rightype=0;
  
  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	}
  //ltDbClose(G_DbCon);
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
	ltDbClose(G_DbCon);
	return 0;
 }
 ltDbClose(G_DbCon);
 return 0;
}
/*
*密码修改判断
*/
int ltPassIsOk(char *InStr){
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
	char sqlBuf[2048];
	char *strUserName;
	char *strPassword;
  char newPass[50],g1[256],g2[256],g3[256],g4[256],g5[256];

	char sIp[24];
	char caMsg[256];
	int  passisok;
	char *bindip;
  int  actiontype;
  char *strUserRight;
  char *strGroupManager;

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	}
  //ltDbClose(G_DbCon);
  
  actiontype=0;
	passisok=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));

	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
      strUserRight=ltMsgGetVar_s(ltMsgPk,"userright");
			strGroupManager=ltMsgGetVar_s(ltMsgPk,"groupmanager");
	if(actiontype==1){/*add*/
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
			sprintf(sqlBuf,"delete from msaadmuser where name='%s' ",	strUserName);
			ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"delete %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==3){/*change power*/
			sprintf(sqlBuf,"update msaadmuser set lright='%s' where name='%s' ",strUserRight,strUserName);//sRight
			ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"update %s right success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==4){/*change password indfo*/
			memset(newPass,0,sizeof(newPass));
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
			sprintf(sqlBuf,"update msaadmuser set bindip='%s'where name='%s' ",bindip,strUserName);
		  ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"update %s ip success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==6){/*change group*/
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

			sprintf(sqlBuf,"update msaadmuser set grouplist1='%s',grouplist2='%s',grouplist3='%s',grouplist4='%s',grouplist5='%s' where name='%s' ",g1,g2,g3,g4,g5,strUserName);
			ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"update %s group success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==7){
		  memset(newPass,0,sizeof(newPass));
			passisok=0;
		  passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr,confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			sprintf(sqlBuf,"update msaadmuser set password='%s',bindip='%s' where name='%s' ",newPass,bindip,strUserName);
			ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"change %s password and ip success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}
	ltDbClose(G_DbCon);
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
//强制下线
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
int msaAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
    char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);			
				return 0;
		}
	  //ltDbClose(G_DbCon);
  
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
       sprintf(caFileName,"admlogexport-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   sprintf(strBuf,"select name,sdate,loglevel,subject from msaadmlog where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){
			     	sprintf(caTmpp,"%s,%s,%s,%s\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
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
    sprintf(caTmpp,"{\"totalCount\":\"%lu\",\"Results\":[",lCount);
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
   ltDbClose(G_DbCon);
	 ltMsgFree(ltMsgPk);
	 return 0;

}
/*
*设备重启
*/
int msaSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
	  
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);

    sprintf(caMsg,"%s reboot system success.",strUserName);
	  if(G_DbCon){
		  logWrite(ltMsgPk,caMsg,9);
		  ltDbClose(G_DbCon);
	  }
	  system("/app/msa/bin/msashutdownall");
    system("/bin/reboot");
    system("/bin/reboot");
    return 0;
}
/*
*设备关机
*/
int msaSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
		
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);

    sprintf(caMsg,"%s shutdown system success.",strUserName);
	  if(G_DbCon){
		  logWrite(ltMsgPk,caMsg,9);
		  ltDbClose(G_DbCon);
	  }
	  system("/app/msa/bin/msashutdownall");
    system("/sbin/poweroff");
    system("/sbin/poweroff");
    system("/sbin/init 0");
    return 0;
}

int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char *caipstart;
    char *caipend;
    char *canetflag;
    char *casnmpnum;
    char *canohashch;
    char *cahomeurl;
    char *cagroupid;
    char *capolicyid;

	  char caStr[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    char sqlBuf[2048];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caMsg[256];
    long lCount;
    char *caipdesp;
    
    char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
				ltMsgFree(ltMsgPk);			
				return 0;
		}
	  //ltDbClose(G_DbCon);
	    
    sprintf(sqlBuf,"select count(*) from nasNetArea ");
    lCount =0;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
            tempRow= ltDbFetchRow(tempCursor);
            if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
            }
            ltDbCloseCursor(tempCursor);
    }
    if(lCount == 0) {
            lCount=1;
    }else{
            sprintf(sqlBuf,"select max(id)+1 from nasNetArea");
            lCount = 1;
            tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
            if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             if(tempRow!=NULL){
                       lCount=atol(tempRow[0]);
             }
             ltDbCloseCursor(tempCursor);
      }
    }
		caipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	     if(!caipstart){
	         caipstart="";
		 }
		caipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	     if(!caipend){
	         caipend="";
		 }
		canetflag=ltMsgGetVar_s(ltMsgPk,"netflag");
	     if(!canetflag){
	         canetflag="";
		 }
		casnmpnum=ltMsgGetVar_s(ltMsgPk,"snmpnum");
	     if(!casnmpnum){
	         casnmpnum="";
		 }
		casnmpnum=ltMsgGetVar_s(ltMsgPk,"snmpnum");
	     if(!casnmpnum){
	         casnmpnum="";
		 }
		canohashch=ltMsgGetVar_s(ltMsgPk,"nohashcheck");
	     if(!canohashch){
	         canohashch="";
		 }
		cahomeurl=ltMsgGetVar_s(ltMsgPk,"homeurl");
	     if(!cahomeurl){
	         cahomeurl="";
		 }
		cagroupid=ltMsgGetVar_s(ltMsgPk,"groupid");
	     if(!cagroupid){
	         cagroupid="";
		 }
		capolicyid=ltMsgGetVar_s(ltMsgPk,"policyid");
	     if(!capolicyid){
	         capolicyid="";
		 }
		 printf("capolicyid:%s\n",capolicyid);
		 caipdesp=ltMsgGetVar_s(ltMsgPk,"ipdesp");
		 if(!caipdesp){
	         caipdesp="";
		 }

   sprintf(sqlBuf,"insert into nasNetArea(\
                  id,ipstart,ipend,netflag,ipdesp,snmpnum,\
                  nohashcheck,homeurl,groupid,policyid)values\
                  (%ld,'%s','%s',%s,'%s',%s,%ld,'%s',%ld,%s)",lCount,caipstart,caipend,canetflag,caipdesp,casnmpnum,atol(canohashch),cahomeurl,atol(cagroupid),capolicyid);
   printf("sqlBuf:%s\n",sqlBuf);
   ltDbExecSql(G_DbCon,sqlBuf);
	 snprintf(caMsg,255,"%s","add nasNetArea success.");
	 logWrite(ltMsgPk,caMsg,3);
   memset(caStr,0,sizeof(caStr));
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}

/*实现删除功能*/
int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
     char strBuf[1024];
     ltDbCursor *tempCursor;
     LT_DBROW tempRow;
     char caTmpp[512];
     long lRowNum,lStartRec,lSumRec,lCount,lNum;
     int k;

	   char caWhere[512];
	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
     
     char caMsg[256];
     
     
     char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
				ltMsgFree(ltMsgPk);			
				return 0;
		}
    
     dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 char *my_id;
				 my_id=ltMsgGetVar_s(ltMsgPk,"del_id");
				 sprintf(sqlBuf,"delete from nasnetArea where id='%s'",my_id);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete nasNetArea success.");
				 logWrite(ltMsgPk,caMsg,3);
			}
			 ltMsgPk->msgpktype=1;
		   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
		   ltMsgFree(ltMsgPk);
		   return 0;
		}


    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0;
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    sprintf(strBuf,"select count(*) from nasNetArea where %s '",caWhere);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select id,ipstart,ipend,netflag,ipdesp,snmpnum,nohashcheck,homeurl,groupid,policyid from nasNetArea limit %lu offset %lu ",lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"id\":\"%s\",\"ipstart\":\"%s\",\"ipend\":\"%s\",\"netflag\":\"%s\",\"snmpnum\":\"%s\",\"homeurl\":\"%s\",\"ipdesp\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[5],tempRow[7],tempRow[4]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"id\":\"%s\",\"ipstart\":\"%s\",\"ipend\":\"%s\",\"netflag\":\"%s\",\"snmpnum\":\"%s\",\"homeurl\":\"%s\",\"ipdesp\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[5],tempRow[7],tempRow[4]);
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
   ltDbClose(G_DbCon);
   return 0;
}
/*
port info
*/
int ltportinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char strDesc[30];
	char myDesc[32];
	char myLocal[32];
	char strProt[20];
	char strLocal[30];
	int iNum;
	char sNum[20];

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);			
				return 0;
	}
	//ltDbClose(G_DbCon);
	  
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"portinfolist");
	iNum=0;

  sprintf(sqlBuf,"select id,lPortStart,lPortEnd,lFlag,lProp,isLocal from nasPortInfo order by lPortStart asc,lFlag desc");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			iNum++;
				if(atol(tempRow[3])==10){
					sprintf(myDesc,"%s","禁止访问");//禁止访问
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==9){
					sprintf(myDesc,"%s","免于监控");//免于监控
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==3){
					sprintf(myDesc,"%s","进行监控");//进行监控
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==2){
					sprintf(myDesc,"%s","监控并记录");//监控并记录
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}
				sprintf(strProt,"%s","unknown");
				if(atol(tempRow[4])==1){
					sprintf(strProt,"%s","TCP");
				}else if(atol(tempRow[4])==2){
					sprintf(strProt,"%s","UDP");
				}

				if(atol(tempRow[5])==0){
					sprintf(myLocal,"%s","远端端口");//远端端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}else if(atol(tempRow[5])==1){
					sprintf(myLocal,"%s","本地端口");//本地端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}else if(atol(tempRow[5])==2){
					sprintf(myLocal,"%s","双向端口");//双向端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}



			lt_dbput_recordvars(tablePtr,7,
				"num",LT_TYPE_LONG,iNum,
				"listindex",LT_TYPE_STRING,tempRow[0],
				"bport",LT_TYPE_STRING,tempRow[1],
				"eport",LT_TYPE_STRING,tempRow[2],
				"listdesc",LT_TYPE_STRING,strDesc,
				"listprot",LT_TYPE_STRING,strProt,
				"strlocal",LT_TYPE_STRING,strLocal);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(sNum,"%d",iNum);
	lt_dbput_rootvars(dbPtr,1,"allnum",sNum);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/portinfo.htm",dbPtr->head,0);

  ltDbClose(G_DbCon);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}
int ltportinfoaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char sqlBuf[1024];
		int  actiontype;
		ltDbCursor *tempCursor;
		LT_DBROW tempRow;
	  char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);			
				return 0;
		}
	//ltDbClose(G_DbCon);
		actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
		if(actiontype==1){/*add*/
				long lCount=0;
				sprintf(sqlBuf,"select count(*) from nasPortInfo ");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
            tempRow= ltDbFetchRow(tempCursor);
            if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
            }
            ltDbCloseCursor(tempCursor);
        }
				if(lCount == 0) {
					lCount=1;
				}else{
		      sprintf(sqlBuf,"select max(id)+1 from nasPortInfo");
		      lCount = 1;
		      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		      if(tempCursor!=NULL){
		       tempRow= ltDbFetchRow(tempCursor);
		       if(tempRow!=NULL){
		                 lCount=atol(tempRow[0]);
		       }
		       ltDbCloseCursor(tempCursor);
		      }
		    }
				sprintf(sqlBuf,"insert into nasPortInfo values (%ld,%s,%s,%s,%s,%s)",lCount,
					ltMsgGetVar_s(ltMsgPk,"bport"),
					ltMsgGetVar_s(ltMsgPk,"eport"),
					ltMsgGetVar_s(ltMsgPk,"netflag"),
					ltMsgGetVar_s(ltMsgPk,"netprot"),
					ltMsgGetVar_s(ltMsgPk,"islocal"));
				ltDbExecSql(G_DbCon,sqlBuf);
	  }else if (actiontype==2){/*del*/
	        int allNum,i;
	        char sNum[30];
	        allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	        for(i=1;i<allNum+1;i++){
	        	sprintf(sNum,"checkbox%d",i);
						sprintf(sqlBuf,"delete from nasPortInfo where id = %s",ltMsgGetVar_s(ltMsgPk,sNum));
						ltDbExecSql(G_DbCon,sqlBuf);
					}
		}
		ltDbClose(G_DbCon);
		ltportinfolist(confd,ltMsgPk,lt_MMHead);
		return 0;
}
int ltapplyportinfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\"> please try 20 seconds later..........</body></html>");
	ltMsgFree(ltMsgPk);
	system("killall -1 gnmmainproc");
	return 0;

}
//功能参数
int ltappFunctionok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *mysupportEmail,*mysupportForm,*myonlineusertype,*mynoautoadduser,*myattprev,*mynopicblock,*mydefaultuserflag;//功能参数
  char *mymaxusertimeout,*mymaxuseronlietime,*myuserproccesstime,*mymaxtcpsessiontimeout,*mytcpproccesstime,*mytimerate,*myflowrate;//系统参数
  
  char *myipmon,*myhttpmon,*myipreportmon,*myhttpreportmon,*mysmtpmon,*myformmon,*mypop3mon,*myblockmon,*myimmon,*myfilemon,*mysearchmon,*myusermon;//监视内容
  char *myuserkeepday,*myvidkeepday,*mynomondisk,*myrecordkeepday,*mymailkeepday,*myaimkeepday,*myformkeepday,*myhistorykeepday,*myloglevel;//配置参数
  
  char *myauthport,*mystrAuthIP,*mydenyUrl,*mysupportTCP,*mysupportUDP;   //验证服务参数
  
  
  //snmp参数
  char *mysnmpebable;
  char *mysnmpcommunity,*mysnmpsip,*mysnmpoid,*mysnmpv;
  char *mysnmpcommunity2,*mysnmpsip2,*mysnmpoid2,*mysnmpv2;
  char *mysnmpcommunity3,*mysnmpsip3,*mysnmpoid3,*mysnmpv3;
  char *mysnmpcommunity4,*mysnmpsip4,*mysnmpoid4,*mysnmpv4;
  char *mysnmpcommunity5,*mysnmpsip5,*mysnmpoid5,*mysnmpv5;
  char *mysnmpcommunity6,*mysnmpsip6,*mysnmpoid6,*mysnmpv6;


	int  actiontype;
	if(ltMsgGetVar_s(ltMsgPk,"actiontype")!=NULL){
		actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
		char caCmdCon[128];
		if(actiontype==1){//监视内容	
			  mysupportEmail=ltMsgGetVar_s(ltMsgPk,"supportEmail");
			  mysupportForm=ltMsgGetVar_s(ltMsgPk,"supportForm");
			  myonlineusertype=ltMsgGetVar_s(ltMsgPk,"onlineusertype");
			  mynoautoadduser=ltMsgGetVar_s(ltMsgPk,"noautoadduser");
			  myattprev=ltMsgGetVar_s(ltMsgPk,"attprev");
			  mynopicblock=ltMsgGetVar_s(ltMsgPk,"nopicblock");
			  mydefaultuserflag=ltMsgGetVar_s(ltMsgPk,"defaultuserflag");
  
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportEmail=%s' > %s",mysupportEmail,"/etc/msa/msa/option/supportEmail");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportForm=%s' > %s",mysupportForm,"/etc/msa/msa/option/supportForm");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'onlineusertype=%s' > %s",myonlineusertype,"/etc/msa/msa/option/onlineusertype");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'noautoadduser=%s' > %s",mynoautoadduser,"/etc/msa/msa/option/noautoadduser");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'attprev=%s' > %s",myattprev,"/etc/msa/msa/option/attprev");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'nopicblock=%s' > %s",mynopicblock,"/etc/msa/msa/option/nopicblock");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'defaultuserflag=%s' > %s",mydefaultuserflag,"/etc/msa/msa/option/defaultuserflag");
				system(caCmdCon);

		}else if(actiontype==2){//配置参数
			  mymaxusertimeout=ltMsgGetVar_s(ltMsgPk,"maxusertimeout");
			  mymaxuseronlietime=ltMsgGetVar_s(ltMsgPk,"maxuseronlietime");
			  myuserproccesstime=ltMsgGetVar_s(ltMsgPk,"userproccesstime");
			  mymaxtcpsessiontimeout=ltMsgGetVar_s(ltMsgPk,"maxtcpsessiontimeout");
			  mytcpproccesstime=ltMsgGetVar_s(ltMsgPk,"tcpproccesstime");
				mytimerate=ltMsgGetVar_s(ltMsgPk,"timerate");
				myflowrate=ltMsgGetVar_s(ltMsgPk,"flowrate");
	
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxusertimeout=%s' > %s",mymaxusertimeout,"/etc/msa/msa/option/maxusertimeout");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxuseronlietime=%s' > %s",mymaxuseronlietime,"/etc/msa/msa/option/maxuseronlietime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'userproccesstime=%s' > %s",myuserproccesstime,"/etc/msa/msa/option/userproccesstime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxtcpsessiontimeout=%s' > %s",mymaxtcpsessiontimeout,"/etc/msa/msa/option/maxtcpsessiontimeout");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'tcpproccesstime=%s' > %s",mytcpproccesstime,"/etc/msa/msa/option/tcpproccesstime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'timerate=%s' > %s",mytimerate,"/etc/msa/msa/option/timerate");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'flowrate=%s' > %s",myflowrate,"/etc/msa/msa/option/flowrate");
				system(caCmdCon);
				
		}else if(actiontype==3){//监视内容	
			  myipmon=ltMsgGetVar_s(ltMsgPk,"ipmon");
			  myhttpmon=ltMsgGetVar_s(ltMsgPk,"httpmon");
			  myipreportmon=ltMsgGetVar_s(ltMsgPk,"ipreportmon");
			  myhttpreportmon=ltMsgGetVar_s(ltMsgPk,"httpreportmon");
			  mysmtpmon=ltMsgGetVar_s(ltMsgPk,"smtpmon");
			  myformmon=ltMsgGetVar_s(ltMsgPk,"formmon");
			  mypop3mon=ltMsgGetVar_s(ltMsgPk,"pop3mon");
			  myblockmon=ltMsgGetVar_s(ltMsgPk,"blockmon");
			  myimmon=ltMsgGetVar_s(ltMsgPk,"immon");
			  myfilemon=ltMsgGetVar_s(ltMsgPk,"filemon");
			  mysearchmon=ltMsgGetVar_s(ltMsgPk,"searchmon");
			  myusermon=ltMsgGetVar_s(ltMsgPk,"usermon");
  
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipmon=%s' > %s",myipmon,"/etc/msa/msa/option/ipmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'httpmon=%s' > %s",myhttpmon,"/etc/msa/msa/option/httpmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipreportmon=%s' > %s",myipreportmon,"/etc/msa/msa/option/ipreportmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'httpreportmon=%s' > %s",myhttpreportmon,"/etc/msa/msa/option/httpreportmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'smtpmon=%s' > %s",mysmtpmon,"/etc/msa/msa/option/smtpmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'formmon=%s' > %s",myformmon,"/etc/msa/msa/option/formmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'pop3mon=%s' > %s",mypop3mon,"/etc/msa/msa/option/pop3mon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'blockmon=%s' > %s",myblockmon,"/etc/msa/msa/option/blockmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'immon=%s' > %s",myimmon,"/etc/msa/msa/option/immon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'filemon=%s' > %s",myfilemon,"/etc/msa/msa/option/filemon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'searchmon=%s' > %s",mysearchmon,"/etc/msa/msa/option/searchmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'usermon=%s' > %s",myusermon,"/etc/msa/msa/option/usermon");
				system(caCmdCon);
				
		}else if(actiontype==4){//配置参数				      
	      mynomondisk=ltMsgGetVar_s(ltMsgPk,"nomondisk");
				myrecordkeepday=ltMsgGetVar_s(ltMsgPk,"recordkeepday");
				mymailkeepday=ltMsgGetVar_s(ltMsgPk,"mailkeepday");
				myaimkeepday=ltMsgGetVar_s(ltMsgPk,"aimkeepday");
				myvidkeepday=ltMsgGetVar_s(ltMsgPk,"vidkeepday");
				myuserkeepday=ltMsgGetVar_s(ltMsgPk,"userkeepday");
				myformkeepday=ltMsgGetVar_s(ltMsgPk,"formkeepday");
				myhistorykeepday=ltMsgGetVar_s(ltMsgPk,"historykeepday");
			  myloglevel=ltMsgGetVar_s(ltMsgPk,"loglevel");
  
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'nomondisk=%s' > %s",mynomondisk,"/etc/msa/msa/option/nomondisk");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'recordkeepday=%s' > %s",myrecordkeepday,"/etc/msa/msa/option/recordkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'mailkeepday=%s' > %s",mymailkeepday,"/etc/msa/msa/option/mailkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'aimkeepday=%s' > %s",myaimkeepday,"/etc/msa/msa/option/aimkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'vidkeepday=%s' > %s",myvidkeepday,"/etc/msa/msa/option/vidkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'userkeepday=%s' > %s",myuserkeepday,"/etc/msa/msa/option/userkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'formkeepday=%s' > %s",myformkeepday,"/etc/msa/msa/option/formkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'historykeepday=%s' > %s",myhistorykeepday,"/etc/msa/msa/option/historykeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'loglevel=%s' > %s",myloglevel,"/etc/msa/msa/option/loglevel");
				system(caCmdCon);
		}else if(actiontype==5){//snmp参数
			  mysnmpebable=ltMsgGetVar_s(ltMsgPk,"snmpebable");
			  mysnmpcommunity=ltMsgGetVar_s(ltMsgPk,"snmpcommunity");
			  mysnmpsip=ltMsgGetVar_s(ltMsgPk,"snmpsip");
			  mysnmpoid=ltMsgGetVar_s(ltMsgPk,"snmpoid");
			  mysnmpv=ltMsgGetVar_s(ltMsgPk,"snmpv");
			  
			  mysnmpcommunity2=ltMsgGetVar_s(ltMsgPk,"snmpcommunity2");
			  mysnmpsip2=ltMsgGetVar_s(ltMsgPk,"snmpsip2");
			  mysnmpoid2=ltMsgGetVar_s(ltMsgPk,"snmpoid2");
			  mysnmpv2=ltMsgGetVar_s(ltMsgPk,"snmpv2");
			  mysnmpcommunity3=ltMsgGetVar_s(ltMsgPk,"snmpcommunity3");
			  mysnmpsip3=ltMsgGetVar_s(ltMsgPk,"snmpsip3");
			  mysnmpoid3=ltMsgGetVar_s(ltMsgPk,"snmpoid3");
			  mysnmpv3=ltMsgGetVar_s(ltMsgPk,"snmpv3");
			  mysnmpcommunity4=ltMsgGetVar_s(ltMsgPk,"snmpcommunity4");
			  mysnmpsip4=ltMsgGetVar_s(ltMsgPk,"snmpsip4");
			  mysnmpoid4=ltMsgGetVar_s(ltMsgPk,"snmpoid4");
			  mysnmpv4=ltMsgGetVar_s(ltMsgPk,"snmpv4");
			  mysnmpcommunity5=ltMsgGetVar_s(ltMsgPk,"snmpcommunity5");
			  mysnmpsip5=ltMsgGetVar_s(ltMsgPk,"snmpsip5");
			  mysnmpoid5=ltMsgGetVar_s(ltMsgPk,"snmpoid5");
			  mysnmpv5=ltMsgGetVar_s(ltMsgPk,"snmpv5");
			  mysnmpcommunity6=ltMsgGetVar_s(ltMsgPk,"snmpcommunity6");
			  mysnmpsip6=ltMsgGetVar_s(ltMsgPk,"snmpsip6");
			  mysnmpoid6=ltMsgGetVar_s(ltMsgPk,"snmpoid6");
			  mysnmpv6=ltMsgGetVar_s(ltMsgPk,"snmpv6");
			  
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpebable=%s' > %s",mysnmpebable,"/etc/msa/msa/option/snmpebable");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity=%s' > %s",mysnmpcommunity,"/etc/msa/msa/option/snmpcommunity");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip=%s' > %s",mysnmpsip,"/etc/msa/msa/option/snmpsip");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid=%s' > %s",mysnmpoid,"/etc/msa/msa/option/snmpoid");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv=%s' > %s",mysnmpv,"/etc/msa/msa/option/snmpv");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity2=%s' > %s",mysnmpcommunity2,"/etc/msa/msa/option/snmpcommunity2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip2=%s' > %s",mysnmpsip2,"/etc/msa/msa/option/snmpsip2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid2=%s' > %s",mysnmpoid2,"/etc/msa/msa/option/snmpoid2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv2=%s' > %s",mysnmpv2,"/etc/msa/msa/option/snmpv2");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity3=%s' > %s",mysnmpcommunity3,"/etc/msa/msa/option/snmpcommunity3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip3=%s' > %s",mysnmpsip3,"/etc/msa/msa/option/snmpsip3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid3=%s' > %s",mysnmpoid3,"/etc/msa/msa/option/snmpoid3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv3=%s' > %s",mysnmpv3,"/etc/msa/msa/option/snmpv3");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity4=%s' > %s",mysnmpcommunity4,"/etc/msa/msa/option/snmpcommunity4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip4=%s' > %s",mysnmpsip4,"/etc/msa/msa/option/snmpsip4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid4=%s' > %s",mysnmpoid4,"/etc/msa/msa/option/snmpoid4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv4=%s' > %s",mysnmpv4,"/etc/msa/msa/option/snmpv4");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity5=%s' > %s",mysnmpcommunity5,"/etc/msa/msa/option/snmpcommunity5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip5=%s' > %s",mysnmpsip5,"/etc/msa/msa/option/snmpsip5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid5=%s' > %s",mysnmpoid5,"/etc/msa/msa/option/snmpoid5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv5=%s' > %s",mysnmpv5,"/etc/msa/msa/option/snmpv5");
				system(caCmdCon);
				
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity6=%s' > %s",mysnmpcommunity6,"/etc/msa/msa/option/snmpcommunity6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip6=%s' > %s",mysnmpsip6,"/etc/msa/msa/option/snmpsip6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid6=%s' > %s",mysnmpoid6,"/etc/msa/msa/option/snmpoid6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv6=%s' > %s",mysnmpv6,"/etc/msa/msa/option/snmpv6");
				system(caCmdCon);
		}else if(actiontype==6){//验证服务参数
			  myauthport=ltMsgGetVar_s(ltMsgPk,"authport");
			  mystrAuthIP=ltMsgGetVar_s(ltMsgPk,"strAuthIP");
			  mydenyUrl=ltMsgGetVar_s(ltMsgPk,"denyUrl");
			  mysupportTCP=ltMsgGetVar_s(ltMsgPk,"supportTCP");
			  mysupportUDP=ltMsgGetVar_s(ltMsgPk,"supportUDP");
			  
			  
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'authport=%s' > %s",myauthport,"/etc/msa/msa/option/authport");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'strAuthIP=%s' > %s",mystrAuthIP,"/etc/msa/msa/option/strAuthIP");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'denyUrl=%s' > %s",mydenyUrl,"/etc/msa/msa/option/denyUrl");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportTCP=%s' > %s",mysupportTCP,"/etc/msa/msa/option/supportTCP");
				system(caCmdCon);
	  		memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportUDP=%s' > %s",mysupportUDP,"/etc/msa/msa/option/supportUDP");
				system(caCmdCon);
		}
	}

	ltappFunction(confd,ltMsgPk,lt_MMHead);
	return 0;

}

int ltappFunction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char str[16];
	//监视内容
  int myipmon,myhttpmon,myipreportmon,myhttpreportmon,mysmtpmon,myformmon,mypop3mon,myblockmon,myimmon,myfilemon,mysearchmon,myusermon;
	//显示方式
	char mynomondisk[32],myrecordkeepday[32],mymailkeepday[32],myaimkeepday[32],myformkeepday[32],myhistorykeepday[32],myloglevel[32];
	char myvidkeepday[32],myuserkeepday[32];
	//功能参数
	int mysupportEmail,mysupportForm,myonlineusertype,mynoautoadduser,myattprev,mynopicblock;
	char mydefaultuserflag[32];
	//系统参数
  char mymaxusertimeout[32],mymaxuseronlietime[32],myuserproccesstime[32],mytimerate[32],myflowrate[32],mymaxtcpsessiontimeout[32],mytcpproccesstime[32];
  //验证服务参数
  int mysupportTCP,mysupportUDP;
  char myauthport[256],mystrAuthIP[32],mydenyUrl[256];
  //snmp参数
  int mysnmpebable;
  char mysnmpcommunity[256],mysnmpsip[32],mysnmpoid[32],mysnmpv[256];
  char mysnmpcommunity2[256],mysnmpsip2[32],mysnmpoid2[32],mysnmpv2[256];
  char mysnmpcommunity3[256],mysnmpsip3[32],mysnmpoid3[32],mysnmpv3[256];
  char mysnmpcommunity4[256],mysnmpsip4[32],mysnmpoid4[32],mysnmpv4[256];
  char mysnmpcommunity5[256],mysnmpsip5[32],mysnmpoid5[32],mysnmpv5[256];
  char mysnmpcommunity6[256],mysnmpsip6[32],mysnmpoid6[32],mysnmpv6[256];
	
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	//监视内容
	myipmon=bcCnfGetValueL("/etc/msa/msa","option","ipmon");
	myhttpmon=bcCnfGetValueL("/etc/msa/msa","option","httpmon");
	myipreportmon=bcCnfGetValueL("/etc/msa/msa","option","ipreportmon");
	myhttpreportmon=bcCnfGetValueL("/etc/msa/msa","option","httpreportmon");
	mysmtpmon=bcCnfGetValueL("/etc/msa/msa","option","smtpmon");
	myformmon=bcCnfGetValueL("/etc/msa/msa","option","formmon");
	mypop3mon=bcCnfGetValueL("/etc/msa/msa","option","pop3mon");
	myblockmon=bcCnfGetValueL("/etc/msa/msa","option","blockmon");
	myimmon=bcCnfGetValueL("/etc/msa/msa","option","immon");
	myfilemon=bcCnfGetValueL("/etc/msa/msa","option","filemon");
	mysearchmon=bcCnfGetValueL("/etc/msa/msa","option","searchmon");
	myusermon=bcCnfGetValueL("/etc/msa/msa","option","usermon");

	memset(str,0,sizeof(str));//1
	sprintf(str,"ipmonchk%d",myipmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//2
	sprintf(str,"httpmonchk%d",myhttpmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//3
	sprintf(str,"ipreportmonchk%d",myipreportmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//4
	sprintf(str,"httpreportmonchk%d",myhttpreportmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//5
	sprintf(str,"smtpmonchk%d",mysmtpmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//6
	sprintf(str,"formmonchk%d",myformmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//7
	sprintf(str,"pop3monchk%d",mypop3mon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//8
	sprintf(str,"blockmonchk%d",myblockmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//9
	sprintf(str,"immonchk%d",myimmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//10
	sprintf(str,"filemonchk%d",myfilemon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//11
	sprintf(str,"searchmonchk%d",mysearchmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//12
	sprintf(str,"usermonchk%d",myusermon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");
	
	//功能参数
	mysupportEmail=bcCnfGetValueL("/etc/msa/msa","option","supportEmail");
	mysupportForm=bcCnfGetValueL("/etc/msa/msa","option","supportForm");
	myonlineusertype=bcCnfGetValueL("/etc/msa/msa","option","onlineusertype");
	mynoautoadduser=bcCnfGetValueL("/etc/msa/msa","option","noautoadduser");
	myattprev=bcCnfGetValueL("/etc/msa/msa","option","attprev");
	mynopicblock=bcCnfGetValueL("/etc/msa/msa","option","nopicblock");	
	bcCnfGetValue_s("/etc/msa/msa/option/defaultuserflag","defaultuserflag",mydefaultuserflag);

	memset(str,0,sizeof(str));
	sprintf(str,"supportEmailchk%d",mysupportEmail);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"supportFormchk%d",mysupportForm);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"onlineusertypechk%d",myonlineusertype);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"noautoadduserchk%d",mynoautoadduser);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"attprevchk%d",myattprev);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"nopicblockchk%d",mynopicblock);
	lt_dbput_rootvars(dbPtr,1,str,"checked");
	
	memset(str,0,sizeof(str));
 	sprintf(str,"defaultuserflag%ssel",mydefaultuserflag);
 	lt_dbput_rootvars(dbPtr,1,str,"selected");


 	//系统参数
	memset(mymaxusertimeout,0,sizeof(mymaxusertimeout));
	bcCnfGetValue_s("/etc/msa/msa/option/maxusertimeout","maxusertimeout",mymaxusertimeout);
	lt_dbput_rootvars(dbPtr,1,"maxusertimeout",mymaxusertimeout);

	memset(mymaxuseronlietime,0,sizeof(mymaxuseronlietime));
	bcCnfGetValue_s("/etc/msa/msa/option/maxuseronlietime","maxuseronlietime",mymaxuseronlietime);
	lt_dbput_rootvars(dbPtr,1,"maxuseronlietime",mymaxuseronlietime);

	memset(myuserproccesstime,0,sizeof(myuserproccesstime));
	bcCnfGetValue_s("/etc/msa/msa/option/userproccesstime","userproccesstime",myuserproccesstime);
	lt_dbput_rootvars(dbPtr,1,"userproccesstime",myuserproccesstime);

	memset(mymaxtcpsessiontimeout,0,sizeof(mymaxtcpsessiontimeout));
	bcCnfGetValue_s("/etc/msa/msa/option/maxtcpsessiontimeout","maxtcpsessiontimeout",mymaxtcpsessiontimeout);
	lt_dbput_rootvars(dbPtr,1,"maxtcpsessiontimeout",mymaxtcpsessiontimeout);

	memset(mytcpproccesstime,0,sizeof(mytcpproccesstime));
	bcCnfGetValue_s("/etc/msa/msa/option/tcpproccesstime","tcpproccesstime",mytcpproccesstime);
	lt_dbput_rootvars(dbPtr,1,"tcpproccesstime",mytcpproccesstime);

	memset(mytimerate,0,sizeof(mytimerate));
	bcCnfGetValue_s("/etc/msa/msa/option/timerate","timerate",mytimerate);
	lt_dbput_rootvars(dbPtr,1,"timerate",mytimerate);

	memset(myflowrate,0,sizeof(myflowrate));
	bcCnfGetValue_s("/etc/msa/msa/option/flowrate","flowrate",myflowrate);
	lt_dbput_rootvars(dbPtr,1,"flowrate",myflowrate);
	
  //验证服务参数
	memset(myauthport,0,sizeof(myauthport));
	bcCnfGetValue_s("/etc/msa/msa/option/authport","authport",myauthport);
	lt_dbput_rootvars(dbPtr,1,"authport",myauthport);
  
	memset(mystrAuthIP,0,sizeof(mystrAuthIP));
	bcCnfGetValue_s("/etc/msa/msa/option/strAuthIP","strAuthIP",mystrAuthIP);
	lt_dbput_rootvars(dbPtr,1,"strAuthIP",mystrAuthIP);
  
	memset(mydenyUrl,0,sizeof(mydenyUrl));
	bcCnfGetValue_s("/etc/msa/msa/option/denyUrl","denyUrl",mydenyUrl);
	lt_dbput_rootvars(dbPtr,1,"denyUrl",mydenyUrl);

	mysupportTCP=bcCnfGetValueL("/etc/msa/msa","option","supportTCP");
	memset(str,0,sizeof(str));//3
	sprintf(str,"supportTCPchk%d",mysupportTCP);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	mysupportUDP=bcCnfGetValueL("/etc/msa/msa","option","supportUDP");
	memset(str,0,sizeof(str));//4
	sprintf(str,"supportUDPchk%d",mysupportUDP);
	lt_dbput_rootvars(dbPtr,1,str,"checked");
  
  
	//记录保存参数
	memset(mynomondisk,0,sizeof(mynomondisk));//6
	bcCnfGetValue_s("/etc/msa/msa/option/nomondisk","nomondisk",mynomondisk);
	lt_dbput_rootvars(dbPtr,1,"nomondisk",mynomondisk);

	memset(myrecordkeepday,0,sizeof(myrecordkeepday));//7
	bcCnfGetValue_s("/etc/msa/msa/option/recordkeepday","recordkeepday",myrecordkeepday);
	lt_dbput_rootvars(dbPtr,1,"recordkeepday",myrecordkeepday);

	memset(mymailkeepday,0,sizeof(mymailkeepday));//9
	bcCnfGetValue_s("/etc/msa/msa/option/mailkeepday","mailkeepday",mymailkeepday);
	lt_dbput_rootvars(dbPtr,1,"mailkeepday",mymailkeepday);

	memset(myaimkeepday,0,sizeof(myaimkeepday));//10
	bcCnfGetValue_s("/etc/msa/msa/option/aimkeepday","aimkeepday",myaimkeepday);
	lt_dbput_rootvars(dbPtr,1,"aimkeepday",myaimkeepday);

	memset(myformkeepday,0,sizeof(myformkeepday));//13
	bcCnfGetValue_s("/etc/msa/msa/option/formkeepday","formkeepday",myformkeepday);
	lt_dbput_rootvars(dbPtr,1,"formkeepday",myformkeepday);

	memset(myhistorykeepday,0,sizeof(myhistorykeepday));//14
	bcCnfGetValue_s("/etc/msa/msa/option/historykeepday","historykeepday",myhistorykeepday);
	lt_dbput_rootvars(dbPtr,1,"historykeepday",myhistorykeepday);

 
  memset(myvidkeepday,0,sizeof(myvidkeepday));//虚拟身份保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/vidkeepday","vidkeepday",myvidkeepday);
	lt_dbput_rootvars(dbPtr,1,"vidkeepday",myvidkeepday);

	memset(myuserkeepday,0,sizeof(myuserkeepday));//用户保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/userkeepday","userkeepday",myuserkeepday);
	lt_dbput_rootvars(dbPtr,1,"userkeepday",myuserkeepday);
	
 	bcCnfGetValue_s("/etc/msa/msa/option/loglevel","loglevel",myloglevel);//审计记录记录级别	
	memset(str,0,sizeof(str));
 	sprintf(str,"loglevel%ssel",myloglevel);
 	lt_dbput_rootvars(dbPtr,1,str,"selected");
	
	//snmp参数
	mysnmpebable=bcCnfGetValueL("/etc/msa/msa","option","snmpebable");
	memset(str,0,sizeof(str));//启动SNMP学习功能
	sprintf(str,"snmpebablechk%d",mysnmpebable);
	lt_dbput_rootvars(dbPtr,1,str,"checked");
		
	memset(mysnmpcommunity,0,sizeof(mysnmpcommunity));//SNMP1的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity","snmpcommunity",mysnmpcommunity);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity",mysnmpcommunity);
		
	memset(mysnmpsip,0,sizeof(mysnmpsip));//SNMP1的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip","snmpsip",mysnmpsip);
	lt_dbput_rootvars(dbPtr,1,"snmpsip",mysnmpsip);
		
	memset(mysnmpoid,0,sizeof(mysnmpoid));//SNMP1的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid","snmpoid",mysnmpoid);
	lt_dbput_rootvars(dbPtr,1,"snmpoid",mysnmpoid);
		
	memset(mysnmpv,0,sizeof(mysnmpv));//SNMP1的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv","snmpv",mysnmpv);
	lt_dbput_rootvars(dbPtr,1,"snmpv",mysnmpv);
		
	memset(mysnmpcommunity2,0,sizeof(mysnmpcommunity2));//SNMP2的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity2","snmpcommunity2",mysnmpcommunity2);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity2",mysnmpcommunity2);
		
	memset(mysnmpsip2,0,sizeof(mysnmpsip2));//SNMP2的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip2","snmpsip2",mysnmpsip2);
	lt_dbput_rootvars(dbPtr,1,"snmpsip2",mysnmpsip2);
		
	memset(mysnmpoid2,0,sizeof(mysnmpoid2));//SNMP2的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid2","snmpoid2",mysnmpoid2);
	lt_dbput_rootvars(dbPtr,1,"snmpoid2",mysnmpoid2);
		
	memset(mysnmpv2,0,sizeof(mysnmpv2));//SNMP2的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv2","snmpv2",mysnmpv2);
	lt_dbput_rootvars(dbPtr,1,"snmpv2",mysnmpv2);
		
	memset(mysnmpcommunity3,0,sizeof(mysnmpcommunity3));//SNMP3的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity3","snmpcommunity3",mysnmpcommunity3);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity3",mysnmpcommunity3);
		
	memset(mysnmpsip3,0,sizeof(mysnmpsip3));//SNMP3的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip3","snmpsip3",mysnmpsip3);
	lt_dbput_rootvars(dbPtr,1,"snmpsip3",mysnmpsip3);
		
	memset(mysnmpoid3,0,sizeof(mysnmpoid3));//SNMP3的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid3","snmpoid3",mysnmpoid3);
	lt_dbput_rootvars(dbPtr,1,"snmpoid3",mysnmpoid3);
		
	memset(mysnmpv3,0,sizeof(mysnmpv3));//SNMP3的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv3","snmpv3",mysnmpv3);
	lt_dbput_rootvars(dbPtr,1,"snmpv3",mysnmpv3);
		
	memset(mysnmpcommunity4,0,sizeof(mysnmpcommunity4));//SNMP4的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity4","snmpcommunity4",mysnmpcommunity4);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity4",mysnmpcommunity4);
		
	memset(mysnmpsip4,0,sizeof(mysnmpsip4));//SNMP4的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip4","snmpsip4",mysnmpsip4);
	lt_dbput_rootvars(dbPtr,1,"snmpsip4",mysnmpsip4);
		
	memset(mysnmpoid4,0,sizeof(mysnmpoid4));//SNMP4的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid4","snmpoid4",mysnmpoid4);
	lt_dbput_rootvars(dbPtr,1,"snmpoid4",mysnmpoid4);
		
	memset(mysnmpv4,0,sizeof(mysnmpv4));//SNMP4的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv4","snmpv4",mysnmpv4);
	lt_dbput_rootvars(dbPtr,1,"snmpv4",mysnmpv4);
		
	memset(mysnmpcommunity5,0,sizeof(mysnmpcommunity5));//SNMP5的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity5","snmpcommunity5",mysnmpcommunity5);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity5",mysnmpcommunity5);
		
	memset(mysnmpsip5,0,sizeof(mysnmpsip5));//SNMP5的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip5","snmpsip5",mysnmpsip5);
	lt_dbput_rootvars(dbPtr,1,"snmpsip5",mysnmpsip5);
		
	memset(mysnmpoid5,0,sizeof(mysnmpoid5));//SNMP5的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid5","snmpoid5",mysnmpoid5);
	lt_dbput_rootvars(dbPtr,1,"snmpoid5",mysnmpoid5);
		
	memset(mysnmpv5,0,sizeof(mysnmpv5));//SNMP5的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv5","snmpv5",mysnmpv5);
	lt_dbput_rootvars(dbPtr,1,"snmpv5",mysnmpv5);
		
	memset(mysnmpcommunity6,0,sizeof(mysnmpcommunity6));//SNMP6的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity6","snmpcommunity6",mysnmpcommunity6);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity6",mysnmpcommunity6);
		
	memset(mysnmpsip6,0,sizeof(mysnmpsip6));//SNMP6的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip6","snmpsip6",mysnmpsip6);
	lt_dbput_rootvars(dbPtr,1,"snmpsip6",mysnmpsip6);
		
	memset(mysnmpoid6,0,sizeof(mysnmpoid6));//SNMP5的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid6","snmpoid6",mysnmpoid6);
	lt_dbput_rootvars(dbPtr,1,"snmpoid6",mysnmpoid6);
		
	memset(mysnmpv6,0,sizeof(mysnmpv6));//SNMP6的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv6","snmpv6",mysnmpv6);
	lt_dbput_rootvars(dbPtr,1,"snmpv6",mysnmpv6);

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/appFunction.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

/*
**邮件分类加载页面
*/
int msamailGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/mail.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
**添加新的记录
*/
int ltmailfilteraction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
 		char *canum;
 		char *carname;
 		char *camailfrom;
 		char *camailto;
 		char *camailsub;
 		char *camailcont;
 		char *camailsize;
 		char *caattname;
 		char *caattsize;
 		char *caflags;
 		char *casflags;
	  char *casmail;
 		char *carnum;
 		char *camaction;

	  char caStr[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    char sqlBuf[2048];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caMsg[256];
    long lCount;
    
	  char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
    
    sprintf(sqlBuf,"select count(*) from msamailfilter");
    lCount =0;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
            tempRow= ltDbFetchRow(tempCursor);
            if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
            }
            ltDbCloseCursor(tempCursor);
    }
    if(lCount == 0) {
            lCount=1;
    }else{
            sprintf(sqlBuf,"select max(rid)+1 from msamailfilter");
            lCount = 1;
            tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
            if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             if(tempRow!=NULL){
                       lCount=atol(tempRow[0]);
             }
             ltDbCloseCursor(tempCursor);
      }
    }

		canum=ltMsgGetVar_s(ltMsgPk,"num");
	     if(!canum){
	         canum="";
		 }
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
	     if(!carname){
	         carname="";
		 }
		camailfrom=ltMsgGetVar_s(ltMsgPk,"mailfrom");
	     if(!camailfrom){
	         camailfrom="";
		 }
		camailto=ltMsgGetVar_s(ltMsgPk,"mailto");
	     if(!camailto){
	         camailto="";
		 }
		camailsub=ltMsgGetVar_s(ltMsgPk,"mailsub");
	     if(!camailsub){
	         camailsub="";
		 }
		camailcont=ltMsgGetVar_s(ltMsgPk,"mailcont");
	     if(!camailcont){
	         camailcont="";
		 }
		camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
	     if(!camailsize){
	         camailsize="";
		 }
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
	     if(!caattname){
	         caattname="";
		 }
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
	     if(!caattsize){
	         caattsize="";
		 }
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
	     if(!caflags){
	         caflags="";
		 }
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
	     if(!casflags){
	         casflags="";
		 }
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
	     if(!casmail){
	         casmail="";
		 }
		carnum=ltMsgGetVar_s(ltMsgPk,"rnum");
	     if(!carnum){
	         carnum="";
		 }
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
	     if(!camaction){
	         camaction="";
		 }
   sprintf(sqlBuf,"insert into msamailfilter(rid,num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction)values (%ld,%ld,'%s','%s','%s','%s','%s',%ld,'%s',%ld,%ld,%ld,'%s',%ld,%ld)",
                   					lCount,
                            atol(ltMsgGetVar_s(ltMsgPk,"num")),
                            ltMsgGetVar_s(ltMsgPk,"rname"),
                            ltMsgGetVar_s(ltMsgPk,"mailfrom"),
                            ltMsgGetVar_s(ltMsgPk,"mailto"),
                            ltMsgGetVar_s(ltMsgPk,"mailsub"),
                            ltMsgGetVar_s(ltMsgPk,"mailcont"),
                            atol(ltMsgGetVar_s(ltMsgPk,"mailsize")),
                            ltMsgGetVar_s(ltMsgPk,"attname"),
                            atol(ltMsgGetVar_s(ltMsgPk,"attsize")),
                            atol(ltMsgGetVar_s(ltMsgPk,"flags")),
                            atol(ltMsgGetVar_s(ltMsgPk,"sflags")),
                            ltMsgGetVar_s(ltMsgPk,"smail"),
                            atol(ltMsgGetVar_s(ltMsgPk,"rnum")),
                            atol(ltMsgGetVar_s(ltMsgPk,"maction")));
   ltDbExecSql(G_DbCon,sqlBuf);
	 snprintf(caMsg,255,"%s","add msamailfilter success.");
	 logWrite(ltMsgPk,caMsg,3);
   memset(caStr,0,sizeof(caStr));
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}
/*
**显示记录  根据传的参数删除记录
*/
int ltmailfilterlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		char *carid;
		char *canum;
 		char *carname;
 		char *camailfrom;
 		char *camailto;
 		char *camailsub;
 		char *camailcont;
 		char *camailsize;
 		char *caattname;
 		char *caattsize;
 		char *caflags;
 		char *casflags;
	  char *casmail;
 		char *carnum;
 		char *camaction;

	  char caWhere[512];
	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  char caMsg[256];
	  lt_db_htmlpage(dbPtr,"utf-8");

    carid=ltMsgGetVar_s(ltMsgPk,"rid");
    canum=ltMsgGetVar_s(ltMsgPk,"num");
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
		camailfrom=ltMsgGetVar_s(ltMsgPk,"mailfrom");
		camailto=ltMsgGetVar_s(ltMsgPk,"mailto");
		camailsub=ltMsgGetVar_s(ltMsgPk,"mailsub");
		camailcont=ltMsgGetVar_s(ltMsgPk,"mailcont");
		camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
		carnum=ltMsgGetVar_s(ltMsgPk,"rnum");
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		
		char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);			
			return 0;
		}
		
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from msamailfilter where rid='%s'",carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete msamailfilter success.");
			}
			if(cadeflag==2)//编辑
			{
				 sprintf(sqlBuf,"update msamailfilter set num='%s',rname='%s',mailfrom='%s',mailto='%s',mailsub='%s',mailcont='%s',mailsize='%s',attname='%s',attsize='%s',flags='%s',sflags='%s',smail='%s',rnum='%s',maction='%s' where rid='%s'",canum,carname,camailfrom,camailto,camailsub,camailcont,camailsize,caattname,caattsize,caflags,casflags,casmail,carnum,camaction,carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","update msamailfilter success.");
			}

				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
		}
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");

	  if(carid && carid[0] )
		{
			sprintf(caWhere,"%s and rid like '%c%s%c'",caWhere,'%',carid,'%');
		}
	  if(canum && canum[0] )
		{
			sprintf(caWhere,"%s and num like '%c%s%c'",caWhere,'%',canum,'%');
		}
	  if(carname && carname[0] )
		{
			sprintf(caWhere,"%s and rname like '%c%s%c'",caWhere,'%',carname,'%');
		}
	  if(camailfrom && camailfrom[0] )
		{
			sprintf(caWhere,"%s and mailfrom like '%c%s%c'",caWhere,'%',camailfrom,'%');
		}
	  if(camailto && camailto[0] )
		{
			sprintf(caWhere,"%s and mailto like '%c%s%c'",caWhere,'%',camailto,'%');
		}
	  if(camailsub && camailsub[0] )
		{
			sprintf(caWhere,"%s and mailsub like '%c%s%c'",caWhere,'%',camailsub,'%');
		}
	  if(camailcont && camailcont[0] )
		{
			sprintf(caWhere,"%s and mailcont like '%c%s%c'",caWhere,'%',camailcont,'%');
		}
	  if(camailsize && camailsize[0] )
		{
			sprintf(caWhere,"%s and mailsize like '%c%s%c'",caWhere,'%',camailsize,'%');
		}
	  if(caattname && caattname[0] )
		{
			sprintf(caWhere,"%s and attname like '%c%s%c'",caWhere,'%',caattname,'%');
		}
	  if(caattsize && caattsize[0] )
		{
			sprintf(caWhere,"%s and attsize like '%c%s%c'",caWhere,'%',caattsize,'%');
		}
	  if(caflags && caflags[0] )
		{
			sprintf(caWhere,"%s and flags like '%c%s%c'",caWhere,'%',caflags,'%');
		}
	  if(casflags && casflags[0] )
		{
			sprintf(caWhere,"%s and sflags like '%c%s%c'",caWhere,'%',casflags,'%');
		}
	  if(casmail && casmail[0] )
		{
			sprintf(caWhere,"%s and smail like '%c%s%c'",caWhere,'%',casmail,'%');
		}
	  if(carnum && carnum[0] )
		{
			sprintf(caWhere,"%s and rnum like '%c%s%c'",caWhere,'%',carnum,'%');
		}
	  if(camaction && camaction[0] )
		{
			sprintf(caWhere,"%s and maction like '%c%s%c'",caWhere,'%',camaction,'%');
		}

    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0;
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }

    sprintf(strBuf,"select count(*) from msamailfilter where %s '",caWhere);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select rid,num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction from msamailfilter where %s  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"mailfrom\":\"%s\",\"mailto\":\"%s\",\"mailsub\":\"%s\",\"mailcont\":\"%s\",\"mailsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"rnum\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"mailfrom\":\"%s\",\"mailto\":\"%s\",\"mailsub\":\"%s\",\"mailcont\":\"%s\",\"mailsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"rnum\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
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
   ltDbClose(G_DbCon);
   return 0;
}
/*
*邮件详情对话框  根据rid查询
*/
int msaShowMail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction from msamailfilter where rid='%s'",my_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);
						 
						 char str[16];
						 memset(str,0,sizeof(str));
						 sprintf(str,"num%ssel",tempRow[0]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 lt_dbput_rootvars(dbPtr,1,"rname",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"mailfrom",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"mailto",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"mailsub",tempRow[4]);
						 lt_dbput_rootvars(dbPtr,1,"mailcont",tempRow[5]);
						 lt_dbput_rootvars(dbPtr,1,"mailsize",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"attname",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"attsize",tempRow[8]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"flags%ssel",tempRow[9]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"%s",str);

						 memset(str,0,sizeof(str));
						 sprintf(str,"sflags%ssel",tempRow[10]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"smail",tempRow[11]);
						 lt_dbput_rootvars(dbPtr,1,"rnum",tempRow[12]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"maction%ssel",tempRow[13]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateMailPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}
/*
**表单分类加载页面
*/
int msaformGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/form.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
**添加新的记录
*/
int ltformfilteraction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	char *canum;
	char *carname;
	char *caformfrom;
	char *caformhost;
	char *caformurl;
	char *caformlcont;
	char *camailsize;
	char *caattname;
	char *caattsize;
	char *caflags;
	char *casflags;
  char *casmail;
	char *camaction;
  
  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error");
			ltMsgFree(ltMsgPk);			
			return 0;
	}
  
  char sqlBuf[2048];
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  char caMsg[256];
  long lCount;
  sprintf(sqlBuf,"select count(*) from msaformfilter");
  lCount =0;
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
  }
  if(lCount == 0) {
          lCount=1;
  }else{
        sprintf(sqlBuf,"select max(rid)+1 from msaformfilter");
        lCount = 1;
        tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
        if(tempCursor!=NULL){
        	tempRow= ltDbFetchRow(tempCursor);
         	if(tempRow!=NULL){
                   lCount=atol(tempRow[0]);
         	}
         	ltDbCloseCursor(tempCursor);
				}
  }

	canum=ltMsgGetVar_s(ltMsgPk,"num");
     if(!canum){
         canum="";
	 }
	carname=ltMsgGetVar_s(ltMsgPk,"rname");
     if(!carname){
         carname="";
	 }
	caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
     if(!caformfrom){
         caformfrom="";
	 }
	caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
     if(!caformhost){
         caformhost="";
	 }
	caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
     if(!caformurl){
         caformurl="";
	 }
	caformlcont=ltMsgGetVar_s(ltMsgPk,"formlcont");
     if(!caformlcont){
         caformlcont="";
	 }
	camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
     if(!camailsize){
         camailsize="";
	 }
	caattname=ltMsgGetVar_s(ltMsgPk,"attname");
     if(!caattname){
         caattname="";
	 }
	caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
     if(!caattsize){
         caattsize="";
	 }
	caflags=ltMsgGetVar_s(ltMsgPk,"flags");
     if(!caflags){
         caflags="";
	 }
	casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
     if(!casflags){
         casflags="";
	 }
	casmail=ltMsgGetVar_s(ltMsgPk,"smail");
     if(!casmail){
         casmail="";
	 }
	camaction=ltMsgGetVar_s(ltMsgPk,"maction");
     if(!camaction){
         camaction="";
	 }
 sprintf(sqlBuf,"insert into msaformfilter(rid,num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction)values(%ld,%ld,'%s','%s','%s','%s','%s',%ld,'%s',%ld,%ld,%ld,'%s',%ld)",
                 					lCount,
                          atol(ltMsgGetVar_s(ltMsgPk,"num")),
                          ltMsgGetVar_s(ltMsgPk,"rname"),
                          ltMsgGetVar_s(ltMsgPk,"formfrom"),
                          ltMsgGetVar_s(ltMsgPk,"formhost"),
                          ltMsgGetVar_s(ltMsgPk,"formurl"),
                          ltMsgGetVar_s(ltMsgPk,"formlcont"),
                          atol(ltMsgGetVar_s(ltMsgPk,"formsize")),
                          ltMsgGetVar_s(ltMsgPk,"attname"),
                          atol(ltMsgGetVar_s(ltMsgPk,"attsize")),
                          atol(ltMsgGetVar_s(ltMsgPk,"flags")),
                          atol(ltMsgGetVar_s(ltMsgPk,"sflags")),
                          ltMsgGetVar_s(ltMsgPk,"smail"),
                          atol(ltMsgGetVar_s(ltMsgPk,"maction")));
 ltDbExecSql(G_DbCon,sqlBuf);
 snprintf(caMsg,255,"%s","add msaformfilter success.");
 logWrite(ltMsgPk,caMsg,3);
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
 ltMsgFree(ltMsgPk);
 ltDbClose(G_DbCon);
 return 0;
}

/*
**显示记录  根据传的参数删除记录
*/
int ltformfilterlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		 char *carid;
		 char *canum;
 		 char *carname;
 		 char *caformfrom;
 		 char *caformhost;
 		 char *caformurl;
 		 char *caformlcont;
 		 char *caformsize;
 		 char *caattname;
 		 char *caattsize;
 		 char *caflags;
 		 char *casflags;
	   char *casmail;
 		 char *camaction;

	   char caWhere[512];
	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
	   dbPtr=lt_dbinit();
	   char caMsg[256];
	   lt_db_htmlpage(dbPtr,"utf-8");

    carid=ltMsgGetVar_s(ltMsgPk,"rid");
    canum=ltMsgGetVar_s(ltMsgPk,"num");
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
		caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
		caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
		caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
		caformlcont=ltMsgGetVar_s(ltMsgPk,"formlcont");
		caformsize=ltMsgGetVar_s(ltMsgPk,"formsize");
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
    //根据传的参数判断删除以及执行删除
    
    char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
    
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from msaformfilter where rid='%s'",carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete msaformfilter success.");
			}
			if(cadeflag==2)//编辑
			{
				 sprintf(sqlBuf,"update msaformfilter set num='%s',rname='%s',formfrom='%s',formhost='%s',formurl='%s',formlcont='%s',formsize='%s',attname='%s',attsize='%s',flags='%s',sflags='%s',smail='%s',maction='%s' where rid='%s'",canum,carname,caformfrom,caformhost,caformurl,caformlcont,caformsize,caattname,caattsize,caflags,casflags,casmail,camaction,carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","update msaformfilter success.");
			}
				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
		}
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");

	  if(carid && carid[0] )
		{
			sprintf(caWhere,"%s and rid like '%c%s%c'",caWhere,'%',carid,'%');
		}
	  if(canum && canum[0] )
		{
			sprintf(caWhere,"%s and num like '%c%s%c'",caWhere,'%',canum,'%');
		}
	  if(carname && carname[0] )
		{
			sprintf(caWhere,"%s and rname like '%c%s%c'",caWhere,'%',carname,'%');
		}
	  if(caformfrom && caformfrom[0] )
		{
			sprintf(caWhere,"%s and formfrom like '%c%s%c'",caWhere,'%',caformfrom,'%');
		}
	  if(caformhost && caformhost[0] )
		{
			sprintf(caWhere,"%s and formhost like '%c%s%c'",caWhere,'%',caformhost,'%');
		}
	  if(caformurl && caformurl[0] )
		{
			sprintf(caWhere,"%s and formurl like '%c%s%c'",caWhere,'%',caformurl,'%');
		}
	  if(caformlcont && caformlcont[0] )
		{
			sprintf(caWhere,"%s and formlcont like '%c%s%c'",caWhere,'%',caformlcont,'%');
		}
	  if(caformsize && caformsize[0] )
		{
			sprintf(caWhere,"%s and formsize like '%c%s%c'",caWhere,'%',caformsize,'%');
		}
	  if(caattname && caattname[0] )
		{
			sprintf(caWhere,"%s and attname like '%c%s%c'",caWhere,'%',caattname,'%');
		}
	  if(caattsize && caattsize[0] )
		{
			sprintf(caWhere,"%s and attsize like '%c%s%c'",caWhere,'%',caattsize,'%');
		}
	  if(caflags && caflags[0] )
		{
			sprintf(caWhere,"%s and flags like '%c%s%c'",caWhere,'%',caflags,'%');
		}
	  if(casflags && casflags[0] )
		{
			sprintf(caWhere,"%s and sflags like '%c%s%c'",caWhere,'%',casflags,'%');
		}
	  if(casmail && casmail[0] )
		{
			sprintf(caWhere,"%s and smail like '%c%s%c'",caWhere,'%',casmail,'%');
		}
	  if(camaction && camaction[0] )
		{
			sprintf(caWhere,"%s and maction like '%c%s%c'",caWhere,'%',camaction,'%');
		}

    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0;
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }

    sprintf(strBuf,"select count(*) from msaformfilter where %s ",caWhere);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select rid,num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction from msaformfilter where %s and flags=0  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"formlcont\":\"%s\",\"formsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13]);
						stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"formlcont\":\"%s\",\"formsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13]);
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
   ltDbClose(G_DbCon);
   return 0;
}

int msaShowForm(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction from msaformfilter where rid='%s'",my_id);

			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);
						 
						 char str[16];
						 memset(str,0,sizeof(str));
						 sprintf(str,"num%ssel",tempRow[0]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 lt_dbput_rootvars(dbPtr,1,"rname",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"formfrom",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"formhost",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"formurl",tempRow[4]);
						 lt_dbput_rootvars(dbPtr,1,"formlcont",tempRow[5]);
						 lt_dbput_rootvars(dbPtr,1,"formsize",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"attname",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"attsize",tempRow[8]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"flags%ssel",tempRow[9]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"%s",str);

						 memset(str,0,sizeof(str));
						 sprintf(str,"sflags%ssel",tempRow[10]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"smail",tempRow[11]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"maction%ssel",tempRow[12]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateFormPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}

/*
**时间策略加载页面
*/
int ltschinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char mytime[400];
    memset(mytime,0,sizeof(mytime));
    
    char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
    
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"infolist");
    sprintf(sqlBuf,"select schid,schname,schdesc,schtime,schtime1 from nasschedule");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
              tempRow= ltDbFetchRow(tempCursor);
              while(tempRow!=NULL){
                       sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);                
                       lt_dbput_recordvars(tablePtr,4,
                                "schid",LT_TYPE_STRING,tempRow[0],
                                "schname",LT_TYPE_STRING,tempRow[1],
                                "schdesc",LT_TYPE_STRING,tempRow[2],
                                "schtime",LT_TYPE_STRING,mytime);
                       tempRow= ltDbFetchRow(tempCursor);
              }
              ltDbCloseCursor(tempCursor);
    }
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/schedit.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);
    ltDbClose(G_DbCon);
    return 0;
}
/*
**添加新的时间策略
*/
int ltaddsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   long lCount=0;
   
   	 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
   
   sprintf(sqlBuf,"select count(*) from nasschedule");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
   }
   if(lCount == 0) {
         lCount=1;
   }else{
    sprintf(sqlBuf,"select max(schid)+1 from nasschedule");
    lCount = 1;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
     tempRow= ltDbFetchRow(tempCursor);
     if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
     }
     ltDbCloseCursor(tempCursor);
		}
   }       
    sprintf(sqlBuf,"insert into nasschedule values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf); 
    snprintf(caMsg,255,"%s","add nasschedule success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltschinfolist(confd,ltMsgPk,lt_MMHead);
 return 0;
}
/*
**修改时间策略
*/
int ltupdatesch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
	 }
   
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   sprintf(sqlBuf,"update nasschedule set schname='%s',schdesc='%s',schtime='%s',schtime1='%s' where schid=%s",
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"schid"));
   ltDbExecSql(G_DbCon,sqlBuf); 
   snprintf(caMsg,255,"%s","update nasschedule success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltschinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除时间策略
*/
int ltdelsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
        char sqlBuf[1048];
        char caMsg[256];
        char *dbUser;
				 char *dbPass;
				 char *dbName;
				 dbName=_ltPubInfo->_dbname;
				 dbUser=_ltPubInfo->_dbuser;
				 dbPass=_ltPubInfo->_dbpass;
				 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
				 }
        sprintf(sqlBuf,"delete from nasschedule where schid in (%s) ",
        ltMsgGetVar_s(ltMsgPk,"checkbox"));
        ltDbExecSql(G_DbCon,sqlBuf); 
			  snprintf(caMsg,255,"%s","delete nasschedule success.");
				logWrite(ltMsgPk,caMsg,3);
				ltDbClose(G_DbCon);
        ltschinfolist(confd,ltMsgPk,lt_MMHead);
        return 0;
}
/*
**节假日加载页面
*/
int ltjjrinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	 char sqlBuf[1024];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char mytime[400];
   
   char *dbUser;
				 char *dbPass;
				 char *dbName;
				 dbName=_ltPubInfo->_dbname;
				 dbUser=_ltPubInfo->_dbuser;
				 dbPass=_ltPubInfo->_dbpass;
				 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
				 }
   
   memset(mytime,0,sizeof(mytime));
//   if(checkRight(confd,ltMsgPk,21)==-1){//权限判断
//             return -1;
//   }
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   tablePtr=lt_dbput_table(dbPtr,"infolist");
   sprintf(sqlBuf,"select jjrid,jjrname,jjrdesc,jjrtime,jjrtime1 from nasjjr");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             while(tempRow!=NULL){
                      sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);                
                      lt_dbput_recordvars(tablePtr,4,
                               "jjrid",LT_TYPE_STRING,tempRow[0],
                               "jjrname",LT_TYPE_STRING,tempRow[1],
                               "jjrdesc",LT_TYPE_STRING,tempRow[2],
                               "jjrtime",LT_TYPE_STRING,mytime);
                      tempRow= ltDbFetchRow(tempCursor);
             }
             ltDbCloseCursor(tempCursor);
   }
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/jjredit.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 ltDbClose(G_DbCon);
	 return 0;
}


/*
**添加新的节假日
*/
int ltaddjjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   long lCount=0;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	 }
   
   sprintf(sqlBuf,"select count(*) from nasjjr");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
   }
   if(lCount == 0) {
         lCount=1;
   }else{
    sprintf(sqlBuf,"select max(jjrid)+1 from nasjjr");
    lCount = 1;
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
     tempRow= ltDbFetchRow(tempCursor);
     if(tempRow!=NULL){
               lCount=atol(tempRow[0]);
     }
     ltDbCloseCursor(tempCursor);
		}
   }       
    sprintf(sqlBuf,"insert into nasjjr values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf); 
    snprintf(caMsg,255,"%s","add nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}

/*
**修改节假日
*/
int ltupdatejjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	 }
   
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   sprintf(sqlBuf,"update nasjjr set jjrname='%s',jjrdesc='%s',jjrtime='%s',jjrtime1='%s' where jjrid=%s",
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"jjrid"));
   ltDbExecSql(G_DbCon,sqlBuf); 
   snprintf(caMsg,255,"%s","update nasjjr success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除节假日
*/
int ltdeljjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sqlBuf[1048];
    char caMsg[256];
    char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
	  dbUser=_ltPubInfo->_dbuser;
	  dbPass=_ltPubInfo->_dbpass;
	  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	  if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);			
							return 0;
	  }
    sprintf(sqlBuf,"delete from nasjjr where jjrid in (%s) ",
    ltMsgGetVar_s(ltMsgPk,"checkbox"));
    ltDbExecSql(G_DbCon,sqlBuf);  
    snprintf(caMsg,255,"%s","delete nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
    ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}

/* 初始化时间服务页面  */
int lttimesetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   ltDbHeadPtr dbPtr;
   char ENABLENTP[24];
   char TIMEZONE[24];
   char ntpadd1[24];
   char ntpadd2[24];
   struct tm * sTim;
	 char   pFormat[24];
	 long   lTemp;
	 long   lTime;
	 char str[16];
	 
   char *pFile="/etc/msa/msa/time/settings";
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lTime=time(0);
   
   memset(TIMEZONE,0,sizeof(TIMEZONE));
   bcCnfGetValue_s(pFile,"TIMEZONE",TIMEZONE);
	 memset(str,0,sizeof(str));
	 sprintf(str,"timezone%ssel",TIMEZONE);
	 lt_dbput_rootvars(dbPtr,1,str,"selected");
	 
	 
   memset(ENABLENTP,0,sizeof(ENABLENTP));
   bcCnfGetValue_s(pFile,"ENABLENTP",ENABLENTP);
   if( strcmp(ENABLENTP,"YES")==0){
       lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL","checked");    
   }else{
       lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL"," ");         
   }
   memset(ntpadd1,0,sizeof(ntpadd1));
   bcCnfGetValue_s(pFile,"NTP_ADDR_1",ntpadd1);
   lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_1",ntpadd1 );
   memset(ntpadd2,0,sizeof(ntpadd2));
   bcCnfGetValue_s(pFile,"NTP_ADDR_2",ntpadd2);
   lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_2",ntpadd2);
   sTim = localtime(&lTime);
	 lTemp = sTim->tm_year;
	 if(lTemp > 100) lTemp = lTemp - 100;
	 if(lTemp<10){
	     sprintf(pFormat,"200%ld",lTemp);
	 }else{
       sprintf(pFormat,"20%ld",lTemp);
	 }
	 lt_dbput_rootvars(dbPtr,1,"year",pFormat);
	 lTemp = sTim->tm_mon + 1;
	 if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
	 }else{
       sprintf(pFormat,"%ld",lTemp);
	 }
   lt_dbput_rootvars(dbPtr,1,"month",pFormat);
   lTemp = sTim->tm_mday;
	 if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
	 }else{
       sprintf(pFormat,"%ld",lTemp);
   }
   lt_dbput_rootvars(dbPtr,1,"day",pFormat);
   lTemp = sTim->tm_hour;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);  
   }else{
       sprintf(pFormat,"%ld",lTemp);  
   }
   lt_dbput_rootvars(dbPtr,1,"day",pFormat);
   lTemp = sTim->tm_hour;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);  
   }else{
       sprintf(pFormat,"%ld",lTemp);  
   }
   lt_dbput_rootvars(dbPtr,1,"hour",pFormat);
   lTemp = sTim->tm_min;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);  
   }else{
       sprintf(pFormat,"%ld",lTemp);  
   }
   lt_dbput_rootvars(dbPtr,1,"minute",pFormat);
   lTemp = sTim->tm_sec;
	 if(lTemp<10){
	     sprintf(pFormat,"0%ld",lTemp);  
	 }else{
	     sprintf(pFormat,"%ld",lTemp);  
	 }
   lt_dbput_rootvars(dbPtr,1,"second",pFormat);
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/timeface.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
/*更改服务配置*/
int lttimeok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char    *pFile="/etc/msa/msa/time/settings";
   char    *ntpadd1;
   char    *ntpadd2;
   char    *TIMEZONE;
   char    *ENABLENTP;
   char    caCmdCon[256];
   ntpadd1=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_1");
   ntpadd2=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_2");
   TIMEZONE=ltMsgGetVar_s(ltMsgPk,"TIMEZONE");
   ENABLENTP=ltMsgGetVar_s(ltMsgPk,"ENABLENTP");
   if(ENABLENTP==NULL){
   ENABLENTP="NO";
   }
   system("/bin/cp /etc/msa/msa/time/settings /etc/msa/msa/time/settings.old");
   /*连接方式*/
   sprintf(caCmdCon,"echo 'TIMEZONE=%s' > %s",TIMEZONE,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'ENABLENTP=%s' >> %s",ENABLENTP,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'NTP_ADDR_1=%s' >> %s",ntpadd1,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'NTP_ADDR_2=%s' >> %s",ntpadd2,pFile);
   system(caCmdCon);
   if(strcmp(TIMEZONE,"-12")==0){//埃尼威托克岛，瓜加林岛代表
   		system("/bin/cp /usr/share/zoneinfo/Kwajalein /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-11")==0){//中途岛，萨摩亚群岛
   		system("/bin/cp /usr/share/zoneinfo/US/Samoa /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-10")==0){//夏威夷代表
   		system("/bin/cp /usr/share/zoneinfo/US/Hawaii /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-9")==0){//阿拉斯加代表
   		system("/bin/cp /usr/share/zoneinfo/US/Alaska /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-8")==0){//蒂华纳，加利福尼亚代表
   		system("/bin/cp /usr/share/zoneinfo/America/Tijuana /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-7")==0){//亚利桑那州(美国西南部的州)代表
   		system("/bin/cp /usr/share/zoneinfo/US/Arizona /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-6")==0){//中部时间（美国和加拿大）代表
   		system("/bin/cp /usr/share/zoneinfo/US/Central /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-5")==0){//波哥大，利马，里奥布朗库代表
   		system("/bin/cp /usr/share/zoneinfo/America/Bogota /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-4")==0){//大西洋时间（加拿大）代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Atlantic /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-3.5")==0){//纽芬兰代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Newfoundland /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-3")==0){//巴西利亚代表/////////////////////////////////
   		system("/bin/cp /usr/share/zoneinfo/Canada/Tokyo /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-2")==0){//中大西洋代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Atlantic /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-1")==0){//亚速尔群岛代表
   		system("/bin/cp /usr/share/zoneinfo/Atlantic/Azores /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"0")==0){//格林威治标准时：都柏林，爱丁堡，伦敦，里斯本
   		system("/bin/cp /usr/share/zoneinfo/Europe/Dublin /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"1")==0){//阿姆斯特丹，柏林，伯尔尼代表
   		system("/bin/cp /usr/share/zoneinfo/Europe/Berlin /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"2")==0){//雅典，布加勒斯特，伊斯坦布尔代表
   		system("/bin/cp /usr/share/zoneinfo/Europe/Athens /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"3")==0){//巴格达， 科威特， 利雅得代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Baghdad /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"4")==0){//阿布扎比，巴库，马斯喀特代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Baku /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"4.5")==0){//喀布尔代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Kabul /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"5")==0){//叶卡特琳堡，伊斯兰堡，卡拉奇，塔什干代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Yekaterinburg /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"5.5")==0){//新德里代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Dili /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"6")==0){//阿斯塔纳，达卡，阿拉木图，新西伯利亚代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Dhaka /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"7")==0){//曼谷，河内，雅加达代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Bangkok /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"8")==0){//北京，重庆，香港，乌鲁木齐，台北代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"9")==0){//日本，韩国代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Tokyo /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"9.5")==0){//阿德莱德，达尔文代表
   		system("/bin/cp /usr/share/zoneinfo/Australia/Adelaide /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"10")==0){//堪培拉，关岛，莫尔兹比港代表
   		system("/bin/cp /usr/share/zoneinfo/Australia/Canberra /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"11")==0){//马伽迪斯琴，索罗门群岛代表、、、、、、、、、、、、、、、、、、、、
   		system("/bin/cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"12")==0){//斐济，堪察加半岛，马绍尔群岛代表
   		system("/bin/cp /usr/share/zoneinfo/Pacific/Fiji /etc/localtime");
 	 }
   lttimesetting(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*更改时间*/
int lttimeset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char    *year;
   char    *month;
   char    *day;
   char    *hour;
   char    *minute;
   char    *second;
   char    caCmdCon[256];
   year=ltMsgGetVar_s(ltMsgPk,"year");
   month=ltMsgGetVar_s(ltMsgPk,"month");
   day=ltMsgGetVar_s(ltMsgPk,"day");
   hour=ltMsgGetVar_s(ltMsgPk,"hour");
   minute=ltMsgGetVar_s(ltMsgPk,"minute");
   second=ltMsgGetVar_s(ltMsgPk,"second");
   sprintf(caCmdCon,"/bin/date -s '%s-%s-%s'",year,month,day);
   system(caCmdCon);
   sprintf(caCmdCon,"/bin/date -s '%s:%s:%s'",hour,minute,second);
   system(caCmdCon);
   lttimesetting(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
*附加路由
*/
int msaRout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >System Upgrading......<br><a href=\"javascript:history.back(1);\" >BACK</a></body></html>"); 
	ltMsgFree(ltMsgPk);
	return 0;
}

/*
*表单记录级别
*/
int ltformlevelok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char caMsg[256];
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *mylevel1,*mylevel2,*mylevel3,*mylevel4,*mylevel5;//记录级别
  
  mylevel1=ltMsgGetVar_s(ltMsgPk,"level1");
  if(mylevel1==NULL){
    mylevel1="NO";
  }
  mylevel2=ltMsgGetVar_s(ltMsgPk,"level2");
  if(mylevel2==NULL){
    mylevel2="NO";
  }
  mylevel3=ltMsgGetVar_s(ltMsgPk,"level3");
  if(mylevel3==NULL){
    mylevel3="NO";
  }
  mylevel4=ltMsgGetVar_s(ltMsgPk,"level4");
  if(mylevel4==NULL){
    mylevel4="NO";
  }
  mylevel5=ltMsgGetVar_s(ltMsgPk,"level5");
  if(mylevel5==NULL){
    mylevel5="NO";
  }
  
		char caCmdCon[128];
    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level1=%s' > %s",mylevel1,"/etc/msa/msa/level/formlevel/level1");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level2=%s' > %s",mylevel2,"/etc/msa/msa/level/formlevel/level2");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level3=%s' > %s",mylevel3,"/etc/msa/msa/level/formlevel/level3");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level4=%s' > %s",mylevel4,"/etc/msa/msa/level/formlevel/level4");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level5=%s' > %s",mylevel5,"/etc/msa/msa/level/formlevel/level5");
		system(caCmdCon);
printf("test1...\n");	
	snprintf(caMsg,255,"%s","update formLevel setting success.");
	logWrite(ltMsgPk,caMsg,3);
	ltformlevel(confd,ltMsgPk,lt_MMHead);
	return 0;

}
int ltformlevel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char mylevel1[16];
	  char mylevel2[16];
	  char mylevel3[16];
	  char mylevel4[16];
	  char mylevel5[16];
		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		
		
		//记录级别
		memset(mylevel1,0,sizeof(mylevel1));
	  bcCnfGetValue_s("/etc/msa/msa/level/formlevel/level1","level1",mylevel1);
	  if(strcmp(mylevel1,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level1radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level1radio"," ");         
	  }
		
		memset(mylevel2,0,sizeof(mylevel2));
	  bcCnfGetValue_s("/etc/msa/msa/level/formlevel/level2","level2",mylevel2);
	  if(strcmp(mylevel2,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level2radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level2radio"," ");         
	  }
		
		memset(mylevel3,0,sizeof(mylevel3));
	  bcCnfGetValue_s("/etc/msa/msa/level/formlevel/level3","level3",mylevel3);
	  if(strcmp(mylevel3,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level3radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level3radio"," ");         
	  }
		
		memset(mylevel4,0,sizeof(mylevel4));
	  bcCnfGetValue_s("/etc/msa/msa/level/formlevel/level4","level4",mylevel4);
	  if(strcmp(mylevel4,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level4radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level4radio"," ");         
	  }
		
		memset(mylevel5,0,sizeof(mylevel5));
	  bcCnfGetValue_s("/etc/msa/msa/level/formlevel/level5","level5",mylevel5);
	  if(strcmp(mylevel5,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level5radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level5radio"," ");         
	  }
		
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/formlevel.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		return 0;

}

/*
*邮件记录级别
*/
int ltmaillevelok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char caMsg[256];
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *mylevel1,*mylevel2,*mylevel3,*mylevel4,*mylevel5;//记录级别
  
  mylevel1=ltMsgGetVar_s(ltMsgPk,"level1");
  if(mylevel1==NULL){
    mylevel1="NO";
  }
  mylevel2=ltMsgGetVar_s(ltMsgPk,"level2");
  if(mylevel2==NULL){
    mylevel2="NO";
  }
  mylevel3=ltMsgGetVar_s(ltMsgPk,"level3");
  if(mylevel3==NULL){
    mylevel3="NO";
  }
  mylevel4=ltMsgGetVar_s(ltMsgPk,"level4");
  if(mylevel4==NULL){
    mylevel4="NO";
  }
  mylevel5=ltMsgGetVar_s(ltMsgPk,"level5");
  if(mylevel5==NULL){
    mylevel5="NO";
  }
  
		char caCmdCon[128];
    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level1=%s' > %s",mylevel1,"/etc/msa/msa/level/maillevel/level1");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level2=%s' > %s",mylevel2,"/etc/msa/msa/level/maillevel/level2");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level3=%s' > %s",mylevel3,"/etc/msa/msa/level/maillevel/level3");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level4=%s' > %s",mylevel4,"/etc/msa/msa/level/maillevel/level4");
		system(caCmdCon);

    memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'level5=%s' > %s",mylevel5,"/etc/msa/msa/level/maillevel/level5");
		system(caCmdCon);
	snprintf(caMsg,255,"%s","update maillevel setting success.");
	logWrite(ltMsgPk,caMsg,3);
	ltmaillevel(confd,ltMsgPk,lt_MMHead);
	return 0;

}
int ltmaillevel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char mylevel1[16];
	  char mylevel2[16];
	  char mylevel3[16];
	  char mylevel4[16];
	  char mylevel5[16];
		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		
		
		//记录级别
		memset(mylevel1,0,sizeof(mylevel1));
	  bcCnfGetValue_s("/etc/msa/msa/level/maillevel/level1","level1",mylevel1);
	  if(strcmp(mylevel1,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level1radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level1radio"," ");         
	  }
		
		memset(mylevel2,0,sizeof(mylevel2));
	  bcCnfGetValue_s("/etc/msa/msa/level/maillevel/level2","level2",mylevel2);
	  if(strcmp(mylevel2,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level2radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level2radio"," ");         
	  }
		
		memset(mylevel3,0,sizeof(mylevel3));
	  bcCnfGetValue_s("/etc/msa/msa/level/maillevel/level3","level3",mylevel3);
	  if(strcmp(mylevel3,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level3radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level3radio"," ");         
	  }
		
		memset(mylevel4,0,sizeof(mylevel4));
	  bcCnfGetValue_s("/etc/msa/msa/level/maillevel/level4","level4",mylevel4);
	  if(strcmp(mylevel4,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level4radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level4radio"," ");         
	  }
		
		memset(mylevel5,0,sizeof(mylevel5));
	  bcCnfGetValue_s("/etc/msa/msa/level/maillevel/level5","level5",mylevel5);
	  if(strcmp(mylevel5,"YES")==0){
	     lt_dbput_rootvars(dbPtr,1,"level5radio","checked");    
	  }else{
	     lt_dbput_rootvars(dbPtr,1,"level5radio"," ");         
	  }
		
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/maillevel.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		return 0;
}

/*
*地址库设定
*/
int msaUrlSortList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   	int i;
		char caSel[91];
		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
	  char myurlStr[91];
		memset(myurlStr,0,sizeof(myurlStr));
	  bcCnfGetValue_s("/etc/msa/msa/url/url","url",myurlStr);
		if(myurlStr!=NULL){
			lt_dbput_rootvars(dbPtr,1,"url",myurlStr);
			for(i=0;i<91;i++){
				sprintf(caSel,"chksort%d",i);
				if(myurlStr[i]=='1'){
					lt_dbput_rootvars(dbPtr,1,caSel,"checked");
				}else{
					lt_dbput_rootvars(dbPtr,1,caSel," ");
				}
			}
			lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/nasurladdress.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
	  }
  return 0;
}

int msaUrlSortOK(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char caMsg[256];
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	char caCmdCon[91];
	char *myurlStr;//从页面取得
	myurlStr=ltMsgGetVar_s(ltMsgPk,"url");
	if(myurlStr!=NULL){
	  memset(caCmdCon,0,sizeof(caCmdCon));
		sprintf(caCmdCon,"echo 'url=%s' > %s",myurlStr,"/etc/msa/msa/url/url");
		system(caCmdCon);
		snprintf(caMsg,255,"%s","update url success.");
  }
  msaUrlSortList(confd,ltMsgPk,lt_MMHead);
  return 0;
}
/*加载服务类型页面*/
int msasrvlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/srv.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

//实现编辑更新  记录删除  grid 的json构造
int ltSrvlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		 char *casrvid;
		 char *casrvname;
 		 char *camatchDirect;
 		 char *camatchAgree;
 		 char *camatchportL;
 		 char *camatchportU;
 		 char *camatchload;
 		 char *casrvip;
 		 char *casrvarea;
 		 char *cadescmatch1;
 		 char *cadescmatch2;
 		 char *cadescmatch3;
 		 char *caascmatch1;
	   char *caascmatch2;
 		 char *caascmatch3;
 		 int srvascmatchtype;
 		 
 		 srvascmatchtype=0;

	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
		 
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
		 
	   dbPtr=lt_dbinit();
	   char caMsg[256];
	   lt_db_htmlpage(dbPtr,"utf-8");

    casrvid=ltMsgGetVar_s(ltMsgPk,"srvid");
    casrvname=ltMsgGetVar_s(ltMsgPk,"srvname");
		camatchDirect=ltMsgGetVar_s(ltMsgPk,"matchDirect");
		camatchAgree=ltMsgGetVar_s(ltMsgPk,"matchAgree");
		camatchportL=ltMsgGetVar_s(ltMsgPk,"matchportL");
		camatchportU=ltMsgGetVar_s(ltMsgPk,"matchportU");
		camatchload=ltMsgGetVar_s(ltMsgPk,"matchload");
		casrvip=ltMsgGetVar_s(ltMsgPk,"srvip");
		casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
		cadescmatch1=ltMsgGetVar_s(ltMsgPk,"srvdescmatch1");
		cadescmatch2=ltMsgGetVar_s(ltMsgPk,"srvdescmatch2");
		cadescmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
		caascmatch1=ltMsgGetVar_s(ltMsgPk,"srvascmatch1");
		caascmatch2=ltMsgGetVar_s(ltMsgPk,"srvascmatch2");
		caascmatch3=ltMsgGetVar_s(ltMsgPk,"srvascmatch3");
		if(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype")){
			srvascmatchtype=atoi(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype"));
		}
		
		
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL){
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1){
				 sprintf(sqlBuf,"delete from nassrv where srvid='%s'",casrvid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete nassrv success.");
			}
			if(cadeflag==2){//编辑
				char *myid;
				 myid==ltMsgGetVar_s(ltMsgPk,"srvid");
				 sprintf(sqlBuf,"update nassrv set srvname='%s',matchDirect='%s',matchAgree='%s',matchportL='%s',matchportU='%s',matchload='%s',srvip='%s',srvarea='%s',srvascmatchtype=%d,srvdescmatch1='%s',srvdescmatch2='%s',srvdescmatch3='%s',srvascmatch1='%s',srvascmatch2='%s',srvascmatch3='%s' where srvid='%s'",casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3,casrvid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","update nassrv success.");
			}
				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
		}

    lRowNum=20; /*每页的行数*/
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

    sprintf(strBuf,"select count(*) from nassrv");
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
    memset(strBuf,0,sizeof(strBuf));
    sprintf(strBuf,"select srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,srvip,srvarea,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3 from nassrv order by srvid  limit %lu offset %lu ",lRowNum,lStartRec);
    printf("strBuf:%s\n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL){
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL){
          if(k==0){
            sprintf(caTmpp,"{\"srvid\":\"%s\",\"srvname\":\"%s\",\"matchDirect\":\"%s\",\"matchAgree\":\"%s\",\"matchport\":\"%s-%s\",\"matchload\":\"%s\",\"srvip\":\"%s\",\"srvarea\":\"%s\",\"srvdescmatch1\":\"%s\",\"srvdescmatch2\":\"%s\",\"srvdescmatch3\":\"%s\",\"srvascmatch1\":\"%s\",\"srvascmatch2\":\"%s\",\"srvascmatch3\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
						stralloc_cats(&strTemp,caTmpp);
					}
					else{
				    sprintf(caTmpp,",{\"srvid\":\"%s\",\"srvname\":\"%s\",\"matchDirect\":\"%s\",\"matchAgree\":\"%s\",\"matchport\":\"%s-%s\",\"matchload\":\"%s\",\"srvip\":\"%s\",\"srvarea\":\"%s\",\"srvdescmatch1\":\"%s\",\"srvdescmatch2\":\"%s\",\"srvdescmatch3\":\"%s\",\"srvascmatch1\":\"%s\",\"srvascmatch2\":\"%s\",\"srvascmatch3\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
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
    ltDbClose(G_DbCon);
    return 0;
}



//实现显示编辑页面功能
int msaShowSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL)
	 {
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,srvip,srvarea,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3  from nassrv where srvid='%s'",my_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL){
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL){
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);
						 
						 char str[16];
						 lt_dbput_rootvars(dbPtr,1,"srvname",tempRow[0]);
						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"direct%ssel",tempRow[1]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"agree%ssel",tempRow[2]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
 						 
						 lt_dbput_rootvars(dbPtr,1,"matchportL",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"matchportU",tempRow[4]);
 						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"load%ssel",tempRow[5]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");
						 
						 lt_dbput_rootvars(dbPtr,1,"srvip",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"srvarea",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch1",tempRow[8]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch2",tempRow[9]);
						 lt_dbput_rootvars(dbPtr,1,"srvdescmatch3",tempRow[10]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch1",tempRow[11]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch2",tempRow[12]);
						 lt_dbput_rootvars(dbPtr,1,"srvascmatch3",tempRow[13]);
						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}


/*
**主页面 实现添加新记录功能
*/
int ltSrvaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	 char *casrvname;
	 long  camatchDirect;
	 long  camatchAgree;
	 char *camatchportL;
	 char *camatchportU;
	 long  camatchload;
	 char *casrvip;
	 char *casrvarea;
	 long cadescmatch1;
	 long cadescmatch2;
	 char *cadescmatch3;
	 long caascmatch1;
   long caascmatch2;
	 char *caascmatch3;
	 int srvascmatchtype;
 	 srvascmatchtype=0;
		
   char sqlBuf[2048];
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char caMsg[256];
   long lCount;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   
   sprintf(sqlBuf,"select count(*) from nassrv");
   lCount =0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
        tempRow= ltDbFetchRow(tempCursor);
        if(tempRow!=NULL){
           lCount=atol(tempRow[0]);
        }
        ltDbCloseCursor(tempCursor);
   }
   if(lCount == 0) {
          lCount=1;
   }else{
        sprintf(sqlBuf,"select max(srvid)+1 from nassrv");
        lCount = 1;
        tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
        if(tempCursor!=NULL){
	         tempRow= ltDbFetchRow(tempCursor);
	         if(tempRow!=NULL){
	                   lCount=atol(tempRow[0]);
	         }
	         ltDbCloseCursor(tempCursor);
        }
   }

   if(ltMsgGetVar_s(ltMsgPk,"srvname")){
   	 casrvname=ltMsgGetVar_s(ltMsgPk,"srvname");
   }else{
   	 casrvname=" ";
   }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchDirect")){
	 	 camatchDirect=atol(ltMsgGetVar_s(ltMsgPk,"matchDirect"));
	 }else{
	 	 camatchDirect=0;
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"matchAgree")){
	 	 camatchAgree=atol(ltMsgGetVar_s(ltMsgPk,"matchAgree"));
	 }else{
	 	 camatchAgree=0;
	 }
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchportL")){
	 	 camatchportL=ltMsgGetVar_s(ltMsgPk,"matchportL");
	 }else{
	 	 camatchportL=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"matchportU")){
	 	 camatchportU=ltMsgGetVar_s(ltMsgPk,"matchportU");
	 }else{
	 	 camatchportU=" ";
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"matchload")){
	 	 camatchload=atol(ltMsgGetVar_s(ltMsgPk,"matchload"));
	 }else{
	 	 camatchload=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvip")){
	 	 casrvip=ltMsgGetVar_s(ltMsgPk,"srvip");
	 }else{
	 	 casrvip=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvarea")){
	 	 casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
	 }else{
	 	 casrvarea=" "; 
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvarea")){
	 	 casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
	 }else{
	 	 casrvarea=" ";
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch1")){
	 	  cadescmatch1=atol(ltMsgGetVar_s(ltMsgPk,"srvdescmatch1"));
	 }else{
	 	  cadescmatch1=0;
	 }
	
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch2")){
	 	  cadescmatch2=atol(ltMsgGetVar_s(ltMsgPk,"srvdescmatch2"));
	 }else{
	 	  cadescmatch2=0;
	 }
	
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch3")){//字节是否在0到e
	 	 cadescmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
	 }else{
	 	 cadescmatch3="";
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatch1")){
	 	 caascmatch1=atol(ltMsgGetVar_s(ltMsgPk,"srvascmatch1"));
	 }else{
	 	 caascmatch1=0; 
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatch2")){
	 	 caascmatch2=atol(ltMsgGetVar_s(ltMsgPk,"srvascmatch2"));
	 }else{
	 	 caascmatch2=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvdescmatch3")){
	 	 caascmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
	 }else{
	 	 caascmatch3="";
	 }
	 
	 
	 
	 if(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype")){
			srvascmatchtype=atoi(ltMsgGetVar_s(ltMsgPk,"srvascmatchtype"));
	 }else{
	 	  srvascmatchtype=0;
	 }
	 
	 if(srvascmatchtype==0){//and操作
	 	    if(strlen(cadescmatch3)>0 && strlen(caascmatch3)>0){
	 	      sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,\
           srvip,srvarea,srvascmatchtype,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d,%ld,%ld,'%s',%ld,%ld,'%s')",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,
           casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3);
        }
	 	
	 }else{//or操作
	 	 if(strlen(cadescmatch3)>0 || strlen(caascmatch3)>0){
	 	 	   sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,matchportL,matchportU,matchload,\
           srvip,srvarea,srvascmatchtype,srvdescmatch1,srvdescmatch2,srvdescmatch3,srvascmatch1,srvascmatch2,srvascmatch3)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d,%ld,%ld,'%s',%ld,%ld,'%s')",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,
           casrvarea,srvascmatchtype,cadescmatch1,cadescmatch2,cadescmatch3,caascmatch1,caascmatch2,caascmatch3);
	 	 }else{
	 	 	  sprintf(sqlBuf,"insert into nassrv(srvid,srvname,matchDirect,matchAgree,\
	 	 	     matchportL,matchportU,matchload,srvip,srvarea,srvascmatchtype)\
           values(%ld,'%s',%ld,%ld,'%s','%s',%ld,'%s','%s',%d)",
           lCount,casrvname,camatchDirect,camatchAgree,camatchportL,camatchportU,camatchload,casrvip,casrvarea,srvascmatchtype);
	 	 }
	 	
	 
	 }
	 
  ltDbExecSql(G_DbCon,sqlBuf);
  snprintf(caMsg,255,"%s","add nassrv success.");
  logWrite(ltMsgPk,caMsg,3);
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
  ltMsgFree(ltMsgPk);
  ltDbClose(G_DbCon); 
  return 0;
}

/*
int ltSrvaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char str[16];
	long lTime1,lTime2,lTime3,lTime4,lTime5,lTime6,lTime7;
  char nowTime1[512],nowTime2[512],nowTime3[512],nowTime4[512],nowTime5[512],nowTime6[512],nowTime7[512];
	//显示方式
	char mynomondisk[32],myrecordkeepday[32],mymailkeepday[32],myaimkeepday[32],myformkeepday[32],myhistorykeepday[32],myloglevel[32];
	char myvidkeepday[32],myuserkeepday[32];
	
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	lTime1=lTime2=lTime3=lTime4=lTime5=lTime6=lTime7=time(0);
  //显示方式
	memset(mynomondisk,0,sizeof(mynomondisk));//1
	bcCnfGetValue_s("/etc/msa/msa/option/nomondisk","nomondisk",mynomondisk);
	lt_dbput_rootvars(dbPtr,1,"nomondisk",mynomondisk);

	memset(myrecordkeepday,0,sizeof(myrecordkeepday));//2
	bcCnfGetValue_s("/etc/msa/msa/option/recordkeepday","recordkeepday",myrecordkeepday);
	lt_dbput_rootvars(dbPtr,1,"recordkeepday",myrecordkeepday);

	memset(mymailkeepday,0,sizeof(mymailkeepday));//3
	bcCnfGetValue_s("/etc/msa/msa/option/mailkeepday","mailkeepday",mymailkeepday);
	lt_dbput_rootvars(dbPtr,1,"mailkeepday",mymailkeepday);

	memset(myaimkeepday,0,sizeof(myaimkeepday));//4
	bcCnfGetValue_s("/etc/msa/msa/option/aimkeepday","aimkeepday",myaimkeepday);
	lt_dbput_rootvars(dbPtr,1,"aimkeepday",myaimkeepday);

	memset(myformkeepday,0,sizeof(myformkeepday));//5
	bcCnfGetValue_s("/etc/msa/msa/option/formkeepday","formkeepday",myformkeepday);
	lt_dbput_rootvars(dbPtr,1,"formkeepday",myformkeepday);

	memset(myhistorykeepday,0,sizeof(myhistorykeepday));//6
	bcCnfGetValue_s("/etc/msa/msa/option/historykeepday","historykeepday",myhistorykeepday);
	lt_dbput_rootvars(dbPtr,1,"historykeepday",myhistorykeepday);

 
  memset(myvidkeepday,0,sizeof(myvidkeepday));//虚拟身份保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/vidkeepday","vidkeepday",myvidkeepday);
	lt_dbput_rootvars(dbPtr,1,"vidkeepday",myvidkeepday);

	memset(myuserkeepday,0,sizeof(myuserkeepday));//用户保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/userkeepday","userkeepday",myuserkeepday);
	lt_dbput_rootvars(dbPtr,1,"userkeepday",myuserkeepday);
	
	memset(str,0,sizeof(str));//5等级
	bcCnfGetValue_s("/etc/msa/msa/option/loglevel","loglevel",myloglevel);
 	sprintf(str,"loglevel%ssel",myloglevel);


	nasTimeGetDate(nowTime1,lTime1-3600*24*atoi(myrecordkeepday));
	nasTimeGetDate(nowTime2,lTime2-3600*24*atoi(mymailkeepday));
	nasTimeGetDate(nowTime3,lTime3-3600*24*atoi(myaimkeepday));
	nasTimeGetDate(nowTime4,lTime4-3600*24*atoi(myformkeepday));
	nasTimeGetDate(nowTime5,lTime5-3600*24*atoi(myhistorykeepday));
	nasTimeGetDate(nowTime6,lTime6-3600*24*atoi(myvidkeepday));
	nasTimeGetDate(nowTime7,lTime7-3600*24*atoi(myuserkeepday));

printf("lTime1:%s\n",nowTime1);
printf("lTime2:%s\n",nowTime2);
printf("lTime3:%s\n",nowTime3);
printf("lTime4:%s\n",nowTime4);
printf("lTime5:%s\n",nowTime5);
printf("lTime6:%s\n",nowTime6);
printf("lTime7:%s\n",nowTime7);


	char strSql[1024];
	ltDbCursor *tempCursor;
  LT_DBROW tempRow; 
   //服务访问汇总记录/
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"delete from msasrvreport where sdate<'20%s'",nowTime1);
	//ltDbExecSql(G_DbCon,strSql);
printf("strSql1:%s\n",strSql);
	//邮件
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"select tabname,sdate from msaloglist where tabtype>=4 and tabtype<=5 and sdate<'20%s'",nowTime2);
printf("strSql3:%s\n",strSql);	
  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
  if(tempCursor!=NULL){
		   tempRow=ltDbFetchRow(tempCursor);
		    if(tempRow!=NULL) {                
           sprintf(strSql,"drop table %s%s",tempRow[0],tempRow[1]);
printf("strSql4:%s\n",strSql);	
           //ltDbExecSql(G_DbCon,strSql);
		   }
	}
  ltDbCloseCursor(tempCursor);
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"delete from msaloglist where tabtype>=4 and tabtype<=5 and sdate<'20%s'",nowTime2);
printf("strSql5:%s\n",strSql);	
  //ltDbExecSql(G_DbCon,strSql);		
  //即时通信
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"select tabname,sdate from msaloglist where tabtype=1 and sdate<'20%s'",nowTime3);
printf("strSql6:%s\n",strSql);	
  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
  if(tempCursor!=NULL){
		   tempRow=ltDbFetchRow(tempCursor);
		    if(tempRow!=NULL) {                
           sprintf(strSql,"drop table %s%s",tempRow[0],tempRow[1]);
printf("strSql7:%s\n",strSql);	
           //ltDbExecSql(G_DbCon,strSql);
		   }
	}
  ltDbCloseCursor(tempCursor);
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"delete from msaloglist where tabtype=1 and sdate<'20%s'",nowTime3);
printf("strSql8:%s\n",strSql);	
  //ltDbExecSql(G_DbCon,strSql);
  //表单监控
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"select tabname,sdate from msaloglist where tabtype=6 and sdate<'20%s'",nowTime4);
printf("strSql9:%s\n",strSql);	
  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
  if(tempCursor!=NULL){
		   tempRow=ltDbFetchRow(tempCursor);
		   if(tempRow!=NULL) { 
		   	   memset(strSql,0,sizeof(strSql));               
           sprintf(strSql,"drop table %s%s",tempRow[0],tempRow[1]);
printf("strSql10:%s\n",strSql);	
           //ltDbExecSql(G_DbCon,strSql);
		   }
	}
	ltDbCloseCursor(tempCursor);
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"delete from msaloglist where tabtype=6 and sdate<'20%s'",nowTime4);
printf("strSql11:%s\n",strSql);	
  //ltDbExecSql(G_DbCon,strSql);	
	//HTTP汇总记录/  
  memset(strSql,0,sizeof(strSql));  
  sprintf(strSql,"delete from msahttpreport where sdate<'20%s'",nowTime5);
	//ltDbExecSql(G_DbCon,strSql);
printf("strSql2:%s\n",strSql);	
  //用户轨迹
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"select tabname,sdate from msaloglist where tabtype=2 and sdate<'20%s'",nowTime6);
printf("strSql12:%s\n",strSql);	
  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
  if(tempCursor!=NULL){
		   tempRow=ltDbFetchRow(tempCursor);
		   if(tempRow!=NULL) { 
		   	   memset(strSql,0,sizeof(strSql));               
           sprintf(strSql,"drop table %s%s",tempRow[0],tempRow[1]);
printf("strSql13:%s\n",strSql);	
           //ltDbExecSql(G_DbCon,strSql);
		   }
	}
	ltDbCloseCursor(tempCursor);
  memset(strSql,0,sizeof(strSql));
  sprintf(strSql,"delete from msaloglist where tabtype=2 and sdate<'20%s'",nowTime6);
  //ltDbExecSql(G_DbCon,strSql);
printf("strSql14:%s\n",strSql);	
	ltDbClose(G_DbCon);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/appFunction.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}
*/
