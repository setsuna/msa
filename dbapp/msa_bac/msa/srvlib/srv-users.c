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
//策略
int msaServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		 int i,type,uid;
		 char tmpstr[512];
		 char tempname[32];
	   stralloc strTemp;
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   type=0;
	   uid=-1;
	   strTemp.s=0;	   
	   if(ltMsgGetVar_s(ltMsgPk,"uid")!=NULL){
	   		uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 	 }
	   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	   		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	 }
	   stralloc_cats(&strTemp,"[");
	   if(type==0){
			   for(i=0;i<MAX_TOP_SRVNUM;i++){
				   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
							if(iHead==1)
							{
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}
							else
							{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
							stralloc_cats(&strTemp,tmpstr);
			   }}
	 	 }else if(type==1){
	 	 		char topSrvNameList[64];
	 	 		nasDbUserInfo *tmpdbuserinfo;
	 	 		for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
	 		 	tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
       	if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){       			
       			sprintf(topSrvNameList,"%s",tmpdbuserinfo->topSrvNameList);
       	}}
	 	 		for(i=0;i<MAX_TOP_SRVNUM;i++){
				   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
				   		if(topSrvNameList[i]==49){
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								}
							}else{
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								}
							}								
							stralloc_cats(&strTemp,tmpstr);
			   			}}
	 	 }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}
int msaPolicyTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){	
	   char sqlBuf[512];
	   char tmpstr[512];
	   int type,uid;
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   stralloc strTemp;
	   int iHead;
	   type=0;
	   uid=-1;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   strTemp.s=0;
	   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	   		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	 }
	 	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	   		uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 	 }
	 	 
 		 int i;
  	 char policyid[128];
 		 nasDbUserInfo *tmpdbuserinfo;
 		 
 		 for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
     if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
     			sprintf(policyid,"%s",tmpdbuserinfo->policyid);
					break;
     		}
   	 }
	 	 
	   stralloc_cats(&strTemp,"[");
	   sprintf(sqlBuf,"select pid,pname,pdesc from msapolicy order by pid asc");
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	   	if(tempCursor!=NULL)
	   	{
	       tempRow= ltDbFetchRow(tempCursor);
	       while(tempRow!=NULL)
			   {
				   	if(type==0){
					   		if(iHead==1){
									 sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);
									 iHead=0;
								}else{
									 sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);
								}
								
				   	}else if(type==1){
				   			printf("11111111111111111111111\n");
				   			printf("UID:%d\n",uid);
				       	if(policyid[atol(tempRow[0])]==49){
						       		if(iHead==1){
												 sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true,checked: true}",tempRow[0],tempRow[2],tempRow[1]);
												 iHead=0;
											}else{
												 sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true,checked: true}",tempRow[0],tempRow[2],tempRow[1]);
											}
				       	}else if(policyid[atol(tempRow[0])]==48){
						       		if(iHead==1){
												 sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true,checked: false}",tempRow[0],tempRow[2],tempRow[1]);
												 iHead=0;
											}else{
												 sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true,checked: false}",tempRow[0],tempRow[2],tempRow[1]);
											}
				       	}
				   	}
				   	stralloc_cats(&strTemp,tmpstr);
						tempRow= ltDbFetchRow(tempCursor);
			   }
	        ltDbCloseCursor(tempCursor);
	    }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}
/*显示用户信息模板*/
int msaTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{ 
	 int uid,iii;
	 nasDbUserInfo *tmpdbuserinfo;
	 ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
       if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
       		char userid[32],ratelimit[32],locktime[32],lLimitBytes[64],lLimitTimes[32],lAllUpFlow[64],lAllDownFlow[64],lAllTime[32],lLimitBytesw[64],lLimitTimesw[32],lAllUpFloww[64],lAllDownFloww[64],lAllTimew[32],lLimitBytesm[64],lLimitTimesm[32],lAllUpFlowm[64],lAllDownFlowm[64],accountnum[32],maxpackage[32],maxBytes[32],maxudp[32], maxtcp[32],maxudpp[32],maxtcpp[32],maxupp[32],maxdownp[32], maxsp[32],maxnc[32]; 
	 				char seconds[32];
	 				nasCvtStime(tmpdbuserinfo->locktime,locktime,seconds);
       		sprintf(userid,"%d",tmpdbuserinfo->userid);
					sprintf(ratelimit,"%d",tmpdbuserinfo->ratelimit);
					sprintf(lLimitBytes,"%lld",tmpdbuserinfo->lLimitBytes);
					sprintf(lLimitTimes,"%d",tmpdbuserinfo->lLimitTimes);
					sprintf(lAllUpFlow,"%lld",tmpdbuserinfo->lAllUpFlow);
					sprintf(lAllDownFlow,"%lld",tmpdbuserinfo->lAllDownFlow);
					sprintf(lAllTime,"%d",tmpdbuserinfo->lAllTime);
					sprintf(lLimitBytesw,"%lld",tmpdbuserinfo->lLimitBytesw);
					sprintf(lLimitTimesw,"%d",tmpdbuserinfo->lLimitTimesw);
					sprintf(lAllUpFloww,"%lld",tmpdbuserinfo->lAllUpFloww);
					sprintf(lAllDownFloww,"%lld",tmpdbuserinfo->lAllDownFloww);
					sprintf(lAllTimew,"%d",tmpdbuserinfo->lAllTimew);
					sprintf(lLimitBytesm,"%lld",tmpdbuserinfo->lLimitBytesm);
					sprintf(lLimitTimesm,"%d",tmpdbuserinfo->lLimitTimesm);
					sprintf(lAllUpFlowm,"%lld",tmpdbuserinfo->lAllUpFlowm);
					sprintf(lAllDownFlowm,"%lld",tmpdbuserinfo->lAllDownFlowm);
					sprintf(accountnum,"%d",tmpdbuserinfo->accountnum);
					sprintf(maxpackage,"%d",tmpdbuserinfo->maxpackage);
					sprintf(maxBytes,"%d",tmpdbuserinfo->maxBytes);
					sprintf(maxudp,"%d",tmpdbuserinfo->maxudp);
					sprintf(maxtcp,"%d",tmpdbuserinfo->maxtcp);
					sprintf(maxudpp,"%d",tmpdbuserinfo->maxudpp);
					sprintf(maxtcpp,"%d",tmpdbuserinfo->maxtcpp);
					sprintf(maxupp,"%d",tmpdbuserinfo->maxupp);
					sprintf(maxdownp,"%d",tmpdbuserinfo->maxdownp);
					sprintf(maxsp,"%d",tmpdbuserinfo->maxsp);
					sprintf(maxnc,"%d",tmpdbuserinfo->maxnc);
       	 	lt_dbput_rootvars(dbPtr,28,
       	 		"userid",userid,
						"ratelimit",ratelimit,
						"locktime",locktime,
						"lLimitBytes",lLimitBytes,
						"lLimitTimes",lLimitTimes,
						"lAllUpFlow",lAllUpFlow,
						"lAllDownFlow",lAllDownFlow,
						"lAllTime",lAllTime,
						"lLimitBytesw",lLimitBytesw,
						"lLimitTimesw",lLimitTimesw,
						"lAllUpFloww",lAllUpFloww,
						"lAllDownFloww",lAllDownFloww,
						"lAllTimew",lAllTimew,
						"lLimitBytesm",lLimitBytesm,
						"lLimitTimesm",lLimitTimesm,
						"lAllUpFlowm",lAllUpFlowm,
						"lAllDownFlowm",lAllDownFlowm,
						"accountnum",accountnum,
						"maxpackage",maxpackage,
						"maxBytes",maxBytes,
						"maxudp",maxudp,
						"maxtcp",maxtcp,
						"maxudpp",maxudpp,
						"maxtcpp",maxtcpp,
						"maxupp",maxupp,
						"maxdownp",maxdownp,
						"maxsp",maxsp,
						"maxnc",maxnc
       	 	);
       	 	char str[32];
       	 	char moreflag[32],caSel[16];
       	 	sprintf(str,"sex%d",tmpdbuserinfo->sex);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	
       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"idcard%d",tmpdbuserinfo->Certificate_type);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	
       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"jftype%d",tmpdbuserinfo->jftype);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	
       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"ratetype%d",tmpdbuserinfo->ratetype);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	
       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"useflags%d",tmpdbuserinfo->useflags);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	
       	 	sprintf(moreflag,"%s",tmpdbuserinfo->moreflag);
       	 	int i;
       	 	for(i=0;i<32;i++){
       	 		sprintf(caSel,"c%d",i);
       	 		if(moreflag[i]==49){
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"true");
       	 		}else {
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"false");
       	 			}
       	 	}
       	 	//QOS 状态
       	 	char sqlBuf[128];
       	 	ltTablePtr tablePtr;
       	 	ltDbCursor *tempCursor;
					LT_DBROW tempRow;
       	 	tablePtr=lt_dbput_table(dbPtr,"qoslist");
					sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
					tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor!=NULL){
								tempRow= ltDbFetchRow(tempCursor);
								while(tempRow!=NULL){
										lt_dbput_recordvars(tablePtr,3,
											"qosid",LT_TYPE_STRING,tempRow[0],
											"qosname",LT_TYPE_STRING,tempRow[1]);
									tempRow= ltDbFetchRow(tempCursor);
								}
								ltDbCloseCursor(tempCursor);
					}
       	 	ltDbClose(G_DbCon);
       	 	
       	 	char strip[32];
					struct in_addr s;
					s.s_addr = htonl(tmpdbuserinfo->bindip);
					sprintf(strip,"%s",inet_ntoa(s));

  				lt_dbput_rootvars(dbPtr,8,
  					"username",tmpdbuserinfo->username,
  					"dispname",tmpdbuserinfo->dispname,
  					"Certificate_code",tmpdbuserinfo->Certificate_code,
  					"email",tmpdbuserinfo->bindemail,
  					"ip",strip,
  					"mac",tmpdbuserinfo->bindmac,
  					"policyid",tmpdbuserinfo->policyid,
  					"moreflag",tmpdbuserinfo->moreflag,
  					"topSrvNameList",tmpdbuserinfo->topSrvNameList
  					);
  					break;
	     }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/msaterminfodetail.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

/*显示添加用户模板*/
int msaShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[128];
  ltDbHeadPtr dbPtr; 
  ltTablePtr tablePtr;
  dbPtr=lt_dbinit();
  ltDbCursor *tempCursor;
	LT_DBROW tempRow;
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"id",ltMsgGetVar_s(ltMsgPk,"id"));
  
  tablePtr=lt_dbput_table(dbPtr,"qoslist");
	sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
						lt_dbput_recordvars(tablePtr,3,
							"qosid",LT_TYPE_STRING,tempRow[0],
							"qosname",LT_TYPE_STRING,tempRow[1]);
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
	}
  ltDbClose(G_DbCon);
  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/msaterminfonew.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}
//
int msaloop(int level,int nowpid,int *pgid){
					 ltDbCursor *tempCursor1;
	 				 LT_DBROW tempRow1;
           int  nowglevel;
           int  iii;
           char sqlstr[128];
           nowglevel=0;
					 char *dbUser;
					 char *dbPass;
					 char *dbName;
					 ltDbConn *Tmp_DbCon;
					 dbName=_ltPubInfo->_dbname;
				 	 dbUser=_ltPubInfo->_dbuser;
					 dbPass=_ltPubInfo->_dbpass;
					 Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
					 if (Tmp_DbCon==NULL){
						return -1;
					 }
           sprintf(sqlstr,"select pid,level from msaGroup where id=%d",nowpid);
           tempCursor1=ltDbOpenCursor(Tmp_DbCon,sqlstr);
           if(tempCursor1){
                    tempRow1= ltDbFetchRow(tempCursor1);
                    if(tempRow1){
                       nowglevel=atol(tempRow1[1]);
                    }
                    ltDbCloseCursor(tempCursor1);
           }
           if(nowglevel>=_MAX_GROUPLEVEL){
                    return -1;
           }
           for(iii=nowglevel;iii>-1;iii--){
                    sprintf(sqlstr,"select pid from msaGroup where id=%d and level=%d",nowpid,iii);
                    tempCursor1=ltDbOpenCursor(Tmp_DbCon,sqlstr);
                    if(tempCursor1){
                       tempRow1= ltDbFetchRow(tempCursor1);
                       if(tempRow1){
                          pgid[iii]=nowpid;
                          nowpid=atol(tempRow1[0]);
                       }
                       ltDbCloseCursor(tempCursor1);
                    }
           }           
           ltDbClose(Tmp_DbCon);
           return 0;
         }

//添加用户
int msaTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
	  char sqlBuf[2048];
    int maxUser_Id,Certificate_type,sex,ratelimit,ratetype,userflag;
		char username[32],dispname[32],Certificate_code[32],password[32],bindemail[64],bindmac[32],moreflag[32],policyid[256],topsrvnamelist[64];
		unsigned int endtime,addtime,bindip;
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
	  maxUser_Id=msaGetFreeUserid(_ltPubInfo);
	  printf("MAXUSERID:%d \n",maxUser_Id);
	 	int groupid=0;
	 	int level,iii;
	 	int  pgid[_MAX_GROUPLEVEL];
	 	for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
                       pgid[iii]=0;
           }
	 	if(ltMsgGetVar_s(ltMsgPk,"groupid")!=NULL){
	 		groupid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
	 	}
	 		 sprintf(sqlBuf,"select pid,level from msagroup where id=%d",groupid);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			  if(tempCursor!=NULL){
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL){
					 	level=atol(tempRow[1]);
					 	pgid[level]=groupid;
					 	if(level>0){
						msaloop(level,atol(tempRow[0]),pgid);
						}
					 }
				 ltDbCloseCursor(tempCursor);
			  }
		
		sprintf(topsrvnamelist,"%s",ltMsgGetVar_s(ltMsgPk,"qosid"));
    sprintf(username,"%s",ltMsgGetVar_s(ltMsgPk,"username"));
    sprintf(dispname,"%s",ltMsgGetVar_s(ltMsgPk,"dispname"));
    Certificate_type=atol(ltMsgGetVar_s(ltMsgPk,"Certificate_type"));
    sprintf(Certificate_code,"%s",ltMsgGetVar_s(ltMsgPk,"Certificate_code"));
    sex=atol(ltMsgGetVar_s(ltMsgPk,"sex"));
    ltMd5Encrypto(ltMsgGetVar_s(ltMsgPk,"password"), strlen(ltMsgGetVar_s(ltMsgPk,"password")),password);
    sprintf(bindemail,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    if(strcmp(ltMsgGetVar_s(ltMsgPk,"ip"),"")!=0){
    	bindip=ntohl(inet_addr(ltMsgGetVar_s(ltMsgPk,"ip")));
  	}else {
  		bindip=0;
  	}
    sprintf(bindmac,"%s",ltMsgGetVar_s(ltMsgPk,"mac"));
    ratelimit=atol(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
    ratetype=atol(ltMsgGetVar_s(ltMsgPk,"solo"));
    endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
		addtime=time(0);
	  sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
    sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
		userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));
	 	sprintf(sqlBuf,"insert into msauser(userid,username,dispname,Certificate_type,Certificate_code,sex,password,bindemail,bindip,\
bindmac,ratelimit,ratetype,addtime,\
groupid0,groupid1,groupid2,groupid3,groupid4,moreflag,policyid,locktime,useflags,topsrvnamelist)values(%d,'%s','%s',%d,'%s',%d,'%s','%s','%d','%s',%d,%d,%d,\
%d,%d,%d,%d,%d,'%s','%s',%d,%d,'%s')",maxUser_Id,username,dispname,Certificate_type,Certificate_code,sex,password,bindemail,\
bindip,bindmac,ratelimit,ratetype,addtime,pgid[0],pgid[1],pgid[2],pgid[3],pgid[4],moreflag,policyid,endtime,userflag,topsrvnamelist);
	 printf("SQL:%s\n",sqlBuf);
	 ltDbExecSql(G_DbCon,sqlBuf);
		 ltDbClose(G_DbCon);
		//内存更新
		 nasDbUserInfo item;
		 if(maxUser_Id<_ltPubInfo->_dbUserInfoHead.bufnum){
				memset(&item,0,sizeof(nasDbUserInfo));
				item.userid=maxUser_Id;
        snprintf(item.username,32,"%s",username);
        snprintf(item.dispname,32,"%s",dispname);
        item.Certificate_type=Certificate_type;
        snprintf(item.Certificate_code,24,"%s",Certificate_code);
        item.sex=sex;
        snprintf(item.password,32,"%s",password);
        item.groupid[0]=pgid[0];
        item.groupid[1]=pgid[1];
        item.groupid[2]=pgid[2];
        item.groupid[3]=pgid[3];
        item.groupid[4]=pgid[4];
        snprintf(item.bindemail,64,"%s",bindemail);
        item.bindip=bindip;
        if(strlen(bindmac)==17 || strlen(bindmac)==12){
                   nasCvtMacI(bindmac,item.bindmac);
        }
        item.ratelimit=ratelimit;
        item.ratetype=(unsigned char)ratetype;
				item.addtime=(unsigned int)addtime;
        item.locktime=(unsigned int)endtime;        
        memcpy(item.moreflag,moreflag,32);
        memcpy(item.policyid,policyid,128);
        item.useflags=(unsigned char)userflag;
        
        item.lasttime=0;
        item.lLimitBytes=0;
        item.lLimitTimes=0;
        item.lAllUpFlow=0;
        item.lAllDownFlow=0;
				item.lAllTime=0;
        item.lLimitBytesw=0;
				item.lLimitTimesw=0;
        item.lAllUpFloww=0;
        item.lAllDownFloww=0;
        item.lAllTimew=0;
        item.lLimitBytesm=0;
				item.lLimitTimesm=0;
        item.lAllUpFlowm=0;
        item.lAllDownFlowm=0;
        item.lAllTimem=0;
        item.jftype=0;
        item.accountnum=0;
        item.maxpackage=0;
        item.maxBytes=0;
        item.maxudp=0;
        item.maxtcp=0;
        item.maxudpp=0;
        item.maxtcpp=0;
        item.maxupp=0;
        item.maxdownp=0;
        item.maxsp=0;
        item.maxnc=0;
        if(msaHashInsertDbUser(_ltPubInfo, &item)!=0){
            printf("insert dbuserinfo errro;%s\n",item.username);
        }
		 }
		 lt_dbput_rootvars(dbPtr,1,"groupid",0);
  	 ltMsgPk->msgpktype=1;
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/msaterminfonew.htm",dbPtr->head,0);
     ltMsgFree(ltMsgPk);
     return 0;
}
//修改用户
int msaTerminfoUp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		int type,id,iii;		
		nasDbUserInfo *tmpdbuserinfo;
		type=-1;
		id=-1;
		if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	}
	 	if(ltMsgGetVar_s(ltMsgPk,"userid")!=NULL){
	 		id=atol(ltMsgGetVar_s(ltMsgPk,"userid"));
	 	}
		if(type==0){
		printf("^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ ^_^ \n");
			unsigned int maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc;
			unsigned long long lLimitBytes,lLimitBytesw,lLimitBytesm;
			maxudp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxudp"));
			lLimitTimes=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimes"));
			maxtcp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxtcp"));
			maxudpp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxudpp"));
			lLimitTimesw=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw"));
			maxtcpp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxtcpp"));
			maxupp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxupp"));
			lLimitTimesm=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm"));
			maxdownp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxdownp"));
			maxpackage=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxpackage"));
			maxsp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxsp"));
			maxBytes=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxBytes"));
			maxnc=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxnc"));
			
			lLimitBytes=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytes"));
			lLimitBytesw=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw"));
			lLimitBytesm=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm"));
			char sqlstr[1024];
			sprintf(sqlstr,"update msauser set maxudp=%d,lLimitTimes=%d,maxtcp=%d,maxudpp=%d,lLimitTimesw=%d,maxtcpp=%d,maxupp=%d,lLimitTimesm=%d,maxdownp=%d,maxpackage=%d,maxsp=%d,maxBytes=%d,maxnc=%d,lLimitBytes=%lld,lLimitBytesw=%lld,lLimitBytesm=%lld where userid=%d",maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc,lLimitBytes,lLimitBytesw,lLimitBytesm,id);
			printf("SQL%s\n",sqlstr);
			ltDbExecSql(G_DbCon,sqlstr);
			ltDbClose(G_DbCon);
			
			//内存更新
	 		for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
       if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==id){
      			tmpdbuserinfo->maxudp=maxudp;
      			tmpdbuserinfo->lLimitTimes=lLimitTimes;
      			tmpdbuserinfo->maxtcp=maxtcp;
      			tmpdbuserinfo->maxudpp=maxudpp;
      			tmpdbuserinfo->lLimitTimesw=lLimitTimesw;
      			tmpdbuserinfo->maxtcpp=maxtcpp;
      			tmpdbuserinfo->maxupp=maxupp;
      			tmpdbuserinfo->lLimitTimesm=lLimitTimesm;
      			tmpdbuserinfo->maxdownp=maxdownp;
      			tmpdbuserinfo->maxpackage=maxpackage;
      			tmpdbuserinfo->maxsp=maxsp;
      			tmpdbuserinfo->maxBytes=maxBytes;
      			tmpdbuserinfo->maxnc=maxnc;
      			tmpdbuserinfo->lLimitBytes=lLimitBytes;
      			tmpdbuserinfo->lLimitBytesw=lLimitBytesw;
      			tmpdbuserinfo->lLimitBytesm=lLimitBytesm;
      	 }
      }			
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,1,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml");	
    ltMsgFree(ltMsgPk);
}else if(type==1){
		printf(">_< >_< >_< >_< >_< >_< >_< >_< >_< >_<\n");
		for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
       if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==id){
       		char sqlBuf[2048];
			    int Certificate_type,sex,ratelimit,ratetype,userflag;
					char username[32],dispname[32],Certificate_code[32],bindemail[64],bindmac[32],moreflag[32],policyid[256];
					unsigned int endtime,bindip;
					sprintf(username,"%s",ltMsgGetVar_s(ltMsgPk,"username"));
			    sprintf(dispname,"%s",ltMsgGetVar_s(ltMsgPk,"dispname"));
			    Certificate_type=atol(ltMsgGetVar_s(ltMsgPk,"Certificate_type"));
			    sprintf(Certificate_code,"%s",ltMsgGetVar_s(ltMsgPk,"Certificate_code"));
			    sex=atol(ltMsgGetVar_s(ltMsgPk,"sex"));
			    sprintf(bindemail,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
			    if(strcmp(ltMsgGetVar_s(ltMsgPk,"ip"),"")!=0){
			    	bindip=ntohl(inet_addr(ltMsgGetVar_s(ltMsgPk,"ip")));
			  	}else {
			  		bindip=0;
			  	}
			    sprintf(bindmac,"%s",ltMsgGetVar_s(ltMsgPk,"mac"));
			    ratelimit=atol(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
			    ratetype=atol(ltMsgGetVar_s(ltMsgPk,"solo"));
			    endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
				  sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
			    sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
					userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));
				 	sprintf(sqlBuf,"update msauser set username='%s',dispname='%s',Certificate_type=%d,Certificate_code='%s',sex=%d,bindemail='%s',bindip='%u',bindmac='%s',ratelimit=%d,ratetype=%d,moreflag='%s',policyid='%s',locktime=%d,useflags=%d where userid=%d",username,dispname,Certificate_type,Certificate_code,sex,bindemail,bindip,bindmac,ratelimit,ratetype,moreflag,policyid,endtime,userflag,id);
				 	printf("SQL:%s\n",sqlBuf);
				 	ltDbExecSql(G_DbCon,sqlBuf);
				 	ltDbClose(G_DbCon);
				 	
				 	//内存更新
			    tmpdbuserinfo->Certificate_type=Certificate_type;
				  tmpdbuserinfo->sex=sex;
					tmpdbuserinfo->ratelimit=ratelimit;
					tmpdbuserinfo->ratetype=ratetype;
					tmpdbuserinfo->useflags=userflag;
					tmpdbuserinfo->locktime=endtime;
					tmpdbuserinfo->bindip=bindip;
					sprintf(tmpdbuserinfo->username,"%s",username);
					sprintf(tmpdbuserinfo->dispname,"%s",dispname);
					sprintf(tmpdbuserinfo->Certificate_code,"%s",Certificate_code);
					sprintf(tmpdbuserinfo->bindemail,"%s",bindemail);
					sprintf(tmpdbuserinfo->bindmac,"%s",bindmac);
					sprintf(tmpdbuserinfo->moreflag,"%s",moreflag);
					sprintf(tmpdbuserinfo->policyid,"%s",policyid);
       }
    }
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,1,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml");
    ltMsgFree(ltMsgPk);
		}
    return -1;
}
//显示部门用户列表模板
int msaShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	 char *gid;
	 char *gnmlevel;
	 int  level;
   ltDbHeadPtr dbPtr;
   char  strlevel[32];
   
   gnmlevel=ltMsgGetVar_s(ltMsgPk,"level");
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 level=0;
	 if(!gnmlevel){
	 		gnmlevel="0";	
	 }
	 if(!gid){
	 		gid="";	
	 }
	 level=atol(gnmlevel);
	 if(level<0 || level>=_MAX_GROUPLEVEL ){
	 	 level=0;
	 }
   sprintf(strlevel,"%d",level);
	 
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   
   lt_dbput_rootvars(dbPtr,1,"gid",gid);
   lt_dbput_rootvars(dbPtr,1,"level",strlevel);
   
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
	 
}

//用户列表
int msaClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	 char *delflag;
	 char *upflag;
	 char *gid;
	 char *gnmlevel;
	 int  level;
   char sqlStr[1024];
   char sqlStr1[1024];
   char caTmpp[1024];
   stralloc strTemp;
   int  allcount;
   ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   int  k;
   delflag=NULL;
   gnmlevel=ltMsgGetVar_s(ltMsgPk,"level");
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 	}
  	if(ltMsgGetVar_s(ltMsgPk,"upflag")!=NULL){
  	 upflag=ltMsgGetVar_s(ltMsgPk,"upflag");
  	}
//	 if(delflag){
//	 	  unsigned int lIpB;
//	 	  char     caMac[6];
//	 		char *uid=ltMsgGetVar_s(ltMsgPk,"uid");
//	 		/*ip|mac*/
//	 		lIpB=0;
//	 		memset(caMac,0,6);
//	 		sprintf(sqlStr,"select uid,username,dispname,ipadd,macadd,lasttime,email,telphone from msauser where uid='%s'",uid);
//	 		tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
//	    if(tempCursor!=NULL){
//		      tempRow= ltDbFetchRow(tempCursor);
//	       	if(tempRow!=NULL){
//	       		lIpB=ntohl(inet_addr(tempRow[4]));
//	       		nasCvtMacI(tempRow[5],caMac);	
//	       	}
//	       	ltDbCloseCursor(tempCursor);
//	    }
//	 		sprintf(sqlStr,"delete from msauserinfo where uid='%s'",uid);
//	 		ltDbExecSql(G_DbCon,sqlStr);
//	 }
	 if(delflag){
	 		char *uid=ltMsgGetVar_s(ltMsgPk,"uid");
	 		sprintf(sqlStr,"delete from msauser where userid='%s'",uid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 level=0;
	 if(!gnmlevel){
	 		gnmlevel="0";	
	 }
	 if(!gid){
	 		gid="";	
	 }
	 level=atol(gnmlevel);
	 if(level<0 || level>=_MAX_GROUPLEVEL ){
	 	 level=0;
	 }
	 memset(sqlStr,0,sizeof(sqlStr));
	 if(strlen(gid)<1){
	 		sprintf(sqlStr,"%s","select count(*) from msauser");
	 		sprintf(sqlStr1,"%s","select userid,username,dispname,bindip,bindmac,lasttime,bindemail from msauser");
	 }else{
	    sprintf(sqlStr,"select count(*) from msauserinfo where groupid%d=%s",level,gid);
	    sprintf(sqlStr1,"select userid,username,dispname,bindip,bindmac,lasttime,bindemail from msauser where groupid%d=%s",level,gid);
	 }
   allcount=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr1);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"bindemail\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"bindemail\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
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
