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
#include <dirent.h>
#include <gdfontmb.h>

#include <ctype.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <stdarg.h>
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
	 	 		char topSrvNameList[MAX_TOP_SRVNUM];
	 	 		nasDbUserInfo *tmpdbuserinfo;
	 	 		for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
	 		 	tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
       	if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
       		memset(topSrvNameList,0,sizeof(topSrvNameList));
					memcpy(topSrvNameList,tmpdbuserinfo->topSrvNameList,MAX_TOP_SRVNUM);
       	}}
	 	 		for(i=0;i<MAX_TOP_SRVNUM;i++){
				   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
				   		if(topSrvNameList[i]==48){
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								}
							}else if(topSrvNameList[i]==49){
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
	 	 if(ltMsgGetVar_s(ltMsgPk,"uid")!=NULL){
	   		uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 	 }

 		 int i;
  	 char policyid[129];
 		 nasDbUserInfo *tmpdbuserinfo;
		 memset(policyid,0,sizeof(policyid));
 		 for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
     if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
     			memcpy(policyid,tmpdbuserinfo->policyid,128);
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
									 sprintf(tmpstr,"{id:'%s',qtip:'%s',text:'%s',leaf:true}",tempRow[0],tempRow[2],tempRow[1]);
									 iHead=0;
								}else{
									 sprintf(tmpstr,",{id:'%s',qtip:'%s',text:'%s',leaf:true}",tempRow[0],tempRow[2],tempRow[1]);
								}
				   	}else if(type==1){
				       	if(policyid[atol(tempRow[0])]==49){
						       		if(iHead==1){
												 sprintf(tmpstr,"{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:true}",tempRow[0],tempRow[2],tempRow[1]);
												 iHead=0;
											}else{
												 sprintf(tmpstr,",{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:true}",tempRow[0],tempRow[2],tempRow[1]);
											}
				       	}else if(policyid[atol(tempRow[0])]==48){
						       		if(iHead==1){
												 sprintf(tmpstr,"{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:false}",tempRow[0],tempRow[2],tempRow[1]);
												 iHead=0;
											}else{
												 sprintf(tmpstr,",{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:false}",tempRow[0],tempRow[2],tempRow[1]);
											}
				       	}
				   	}else if(type==2){
				   			if(iHead==1){
									 sprintf(tmpstr,"{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:true}",tempRow[0],tempRow[2],tempRow[1]);
									 iHead=0;
								}else{
									 sprintf(tmpstr,",{id:'%s',qtip:'%s',text:'%s',leaf:true,checked:true}",tempRow[0],tempRow[2],tempRow[1]);
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
       	 	char moreflag[33],caSel[16];
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
       	 	memset(moreflag,0,sizeof(moreflag));
					memcpy(moreflag,tmpdbuserinfo->moreflag,32);
       	 	int i;
       	 	for(i=0;i<32;i++){
       	 		memset(caSel,0,sizeof(caSel));
       	 		sprintf(caSel,"c%d",i);
       	 		if(moreflag[i]==49){
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"true");
       	 		}else if(moreflag[i]==48){
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"false");
       	 			}
       	 	}
       	 	//QOS 状态
       	 	char sqlBuf[128];
       	 	ltTablePtr tablePtr;
       	 	ltDbCursor *tempCursor;
					LT_DBROW tempRow;
					memset(str,0,sizeof(str));
       	 	sprintf(str,"qsel%d",tmpdbuserinfo->ratelimit);
       	 	tablePtr=lt_dbput_table(dbPtr,"qoslist");
					sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
					tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor!=NULL){
								tempRow= ltDbFetchRow(tempCursor);
								while(tempRow!=NULL){
										lt_dbput_recordvars(tablePtr,3,
											"qosid",LT_TYPE_STRING,tempRow[0],
											"str",LT_TYPE_STRING,"selected",
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
					char policyid[129],topSrvNameList[MAX_TOP_SRVNUM];
       	 	memset(policyid,0,sizeof(policyid));
					memcpy(policyid,tmpdbuserinfo->policyid,128);
       	 	memset(topSrvNameList,0,sizeof(topSrvNameList));
					memcpy(topSrvNameList,tmpdbuserinfo->topSrvNameList,MAX_TOP_SRVNUM);
  				lt_dbput_rootvars(dbPtr,9,
  					"username",tmpdbuserinfo->username,
  					"dispname",tmpdbuserinfo->dispname,
  					"Certificate_code",tmpdbuserinfo->Certificate_code,
  					"email",tmpdbuserinfo->bindemail,
  					"ip",strip,
  					"mac",tmpdbuserinfo->bindmac,
  					"policyid",policyid,
  					"moreflag",moreflag,
  					"topSrvNameList",topSrvNameList
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
    int Certificate_type,sex,ratelimit,ratetype,userflag,accountnum,jftype;
		char username[32],dispname[32],Certificate_code[32],password[32],bindemail[64],bindmac[32],moreflag[33],policyid[129],topsrvnamelist[MAX_TOP_SRVNUM],strip[32];
		unsigned int endtime,addtime,bindip,newuserid;
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
		memset(policyid,0,sizeof(policyid));
		memset(moreflag,0,sizeof(moreflag));
		memset(topsrvnamelist,1,sizeof(topsrvnamelist));
		memset(strip,0,sizeof(strip));
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
	 	int groupid=0;
	 	int level,iii;
	 	int  pgid[_MAX_GROUPLEVEL];
	 	for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
      	pgid[iii]=0;
    }
	 	if(strcmp(ltMsgGetVar_s(ltMsgPk,"groupid"),"")==0){
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
		sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 	sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
		sprintf(topsrvnamelist,"%s",ltMsgGetVar_s(ltMsgPk,"qosid"));
    sprintf(username,"%s",ltMsgGetVar_s(ltMsgPk,"username"));
    sprintf(dispname,"%s",ltMsgGetVar_s(ltMsgPk,"dispname"));
    Certificate_type=atol(ltMsgGetVar_s(ltMsgPk,"Certificate_type"));
    sprintf(Certificate_code,"%s",ltMsgGetVar_s(ltMsgPk,"Certificate_code"));
    sex=atol(ltMsgGetVar_s(ltMsgPk,"sex"));
    ltMd5Encrypto(ltMsgGetVar_s(ltMsgPk,"password"),strlen(ltMsgGetVar_s(ltMsgPk,"password")),password);
    sprintf(bindemail,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    if(strcmp(ltMsgGetVar_s(ltMsgPk,"ip"),"")!=0){
    	bindip=ntohl(inet_addr(ltMsgGetVar_s(ltMsgPk,"ip")));
  	}else {
  		bindip=0;
  	}
    sprintf(bindmac,"%s",ltMsgGetVar_s(ltMsgPk,"mac"));
    ratelimit=atol(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
    ratetype=atol(ltMsgGetVar_s(ltMsgPk,"solo"));
    jftype=atol(ltMsgGetVar_s(ltMsgPk,"billing"));
    accountnum=atol(ltMsgGetVar_s(ltMsgPk,"money"));
    endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
		addtime=time(0);
	  userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));
		//内存更新
		 nasDbUserInfo item;
				memset(&item,0,sizeof(nasDbUserInfo));
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
        memcpy(item.topSrvNameList,topsrvnamelist,MAX_TOP_SRVNUM);
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
        item.jftype=jftype;
        item.accountnum=accountnum;
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
     //newuserid=msaNewInsertDbUser(_ltPubInfo,item);
     printf("newuserid:%d \n",newuserid);
		 lt_dbput_rootvars(dbPtr,1,"gid","0");
   	 lt_dbput_rootvars(dbPtr,1,"level","0");
		 ltMsgPk->msgpktype=1;
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);
	   ltMsgFree(ltMsgPk);
	   lt_dbfree(dbPtr);
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
      			break;
      	 }
      }
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,1,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml");
    ltMsgFree(ltMsgPk);
    return 0;
}else if(type==1){
		for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
       if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==id){
       		char sqlBuf[2048];
			    int Certificate_type,sex,ratelimit,ratetype,userflag,accountnum,jftype;
					char username[32],dispname[32],Certificate_code[32],bindemail[64],bindmac[32],moreflag[33],policyid[129],topsrvnamelist[MAX_TOP_SRVNUM];
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
			    jftype=atol(ltMsgGetVar_s(ltMsgPk,"billing"));
			    accountnum=atol(ltMsgGetVar_s(ltMsgPk,"money"));
			    endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
				  sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
			    sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
			    sprintf(topsrvnamelist,"%s",ltMsgGetVar_s(ltMsgPk,"qosid"));
					userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));
				 	sprintf(sqlBuf,"update msauser set username='%s',dispname='%s',Certificate_type=%d,Certificate_code='%s',sex=%d,bindemail='%s',bindip='%u',bindmac='%s',ratelimit=%d,ratetype=%d,moreflag='%s',policyid='%s',locktime=%d,useflags=%d,jftype=%d,accountnum=%d where userid=%d",username,dispname,Certificate_type,Certificate_code,sex,bindemail,bindip,bindmac,ratelimit,ratetype,moreflag,policyid,endtime,userflag,jftype,accountnum,id);

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
					tmpdbuserinfo->bindip=jftype;
					tmpdbuserinfo->bindip=accountnum;
					sprintf(tmpdbuserinfo->username,"%s",username);
					sprintf(tmpdbuserinfo->dispname,"%s",dispname);
					sprintf(tmpdbuserinfo->Certificate_code,"%s",Certificate_code);
					sprintf(tmpdbuserinfo->bindemail,"%s",bindemail);
					sprintf(tmpdbuserinfo->bindmac,"%s",bindmac);
					memcpy(tmpdbuserinfo->policyid,policyid,128);
					memcpy(tmpdbuserinfo->moreflag,moreflag,32);
					memcpy(tmpdbuserinfo->topSrvNameList,topsrvnamelist,MAX_TOP_SRVNUM);

				  msaShowClientList(confd,ltMsgPk,lt_MMHead);
			    return 0;
       }
    }
		}
    return -1;
}

//显示策略用户列表模板
int msaPolicyList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *gid;
   ltDbHeadPtr dbPtr;
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"gid",gid);
   lt_dbput_rootvars(dbPtr,1,"policy","policy");
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示批量策略模板
int msaPolicyApply(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[128];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
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
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policyApply.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//用户列表
int msaClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *delflag;
	 char *gid;
	 char *gnmlevel;
	 int  level;
   char sqlStr[1024];
   char sqlStr1[1024];
   char caTmpp[1024];
	 char strip[32];
   memset(strip,0,sizeof(strip));
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
	 if(level<0 || level>=_MAX_GROUPLEVEL){
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
   //策略用户列表
   if(strcmp(ltMsgGetVar_s(ltMsgPk,"policyflag"),"policy")==0){
	 		int iii;
	 		char policyid[129];
	 		nasDbUserInfo *tmpdbuserinfo;
	 		for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
       if(tmpdbuserinfo!=NULL){
       		memset(policyid,0,sizeof(policyid));
       		memcpy(policyid,tmpdbuserinfo->policyid,128);
				  struct in_addr s;
					s.s_addr = htonl(tmpdbuserinfo->bindip);
					sprintf(strip,"%s",inet_ntoa(s));
       		//printf("POLICYID[]%d \n",policyid[atol(gid)]);
       		if(policyid[atol(gid)]==49){
	       			if(k==0){
		     	  	 sprintf(caTmpp,"{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%d\",\"bindemail\":\"%s\"}",tmpdbuserinfo->userid,tmpdbuserinfo->username,tmpdbuserinfo->dispname,strip,tmpdbuserinfo->bindmac,tmpdbuserinfo->lasttime,tmpdbuserinfo->bindemail);
		     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%d\",\"bindemail\":\"%s\"}",tmpdbuserinfo->userid,tmpdbuserinfo->username,tmpdbuserinfo->dispname,strip,tmpdbuserinfo->bindmac,tmpdbuserinfo->lasttime,tmpdbuserinfo->bindemail);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
     				 	k++;
       		 }
       	 }
      }
      stralloc_cats(&strTemp,"]}");
	 		stralloc_0(&strTemp);
	 		ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
			ltMsgFree(ltMsgPk);
			return 0;
	 }
//部门用户
   k=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr1);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
	 			memset(strip,0,sizeof(strip));
			  struct in_addr s;
       	while(tempRow!=NULL){
					s.s_addr = htonl(atoll(tempRow[3]));
					sprintf(strip,"%s",inet_ntoa(s));
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"bindemail\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],strip,tempRow[4],tempRow[5],tempRow[6]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"bindemail\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],strip,tempRow[4],tempRow[5],tempRow[6]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//修改密码
int msaUpdatePwd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *oldpwd,*newpwd;
	 int iii,uid;
	 stralloc strTemp;
	 strTemp.s=0;
	 char caTmpp[1024];
	 char password[32],newmd5pwd[32],sql[128],oldmd5pwd[32];
	 oldpwd=ltMsgGetVar_s(ltMsgPk,"oldpwd");
	 newpwd=ltMsgGetVar_s(ltMsgPk,"newpwd");
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 nasDbUserInfo *tmpdbuserinfo;
	 for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
 		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
     if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
     		sprintf(password,"%s",tmpdbuserinfo->password);
     		ltMd5Encrypto(oldpwd,strlen(oldpwd),oldmd5pwd);
     		if(strcmp(password,oldmd5pwd)==0){
     				ltMd5Encrypto(newpwd,strlen(newpwd),newmd5pwd);
     				sprintf(tmpdbuserinfo->password,"%s",newmd5pwd);
     				sprintf(sql,"update msauser set password='%s' where userid=%d",newmd5pwd,uid);
     				ltDbExecSql(G_DbCon,sql);
						ltDbClose(G_DbCon);
     				sprintf(caTmpp,"{success:true}");
     				stralloc_cats(&strTemp,caTmpp);
     		}else{
     				sprintf(caTmpp,"{success:false,errors:[{id:'oldpwd',msg:'Old password is incorrect!'}]}");
     				stralloc_cats(&strTemp,caTmpp);
     	 	}
   		}
   }
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//移动用户到部门
int msaUpdateGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int uid,gid,iii;
	 char sqlBuf[1024];
	 int level;
	 nasDbUserInfo *tmpdbuserinfo;
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
	 ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   int pgid[_MAX_GROUPLEVEL];
 	 for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
    	pgid[iii]=0;
   }
   sprintf(sqlBuf,"select pid,level from msagroup where id=%d",gid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor!=NULL){
			 tempRow= ltDbFetchRow(tempCursor);
			 if(tempRow!=NULL){
			 	level=atol(tempRow[1]);
			 	pgid[level]=gid;
			 	if(level>0){
				msaloop(level,atol(tempRow[0]),pgid);
				}
			 }
		 ltDbCloseCursor(tempCursor);
	  }
	for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
			memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"update msauser set groupid%d=%d where userid=%d",iii,pgid[iii],uid);
	 		ltDbExecSql(G_DbCon,sqlBuf);
  }
  for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
	 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
      if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
       	 tmpdbuserinfo->groupid[0]=pgid[0];
       	 tmpdbuserinfo->groupid[1]=pgid[1];
       	 tmpdbuserinfo->groupid[2]=pgid[2];
       	 tmpdbuserinfo->groupid[3]=pgid[3];
       	 tmpdbuserinfo->groupid[4]=pgid[4];
      }
  }
  ltDbClose(G_DbCon);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
  ltMsgFree(ltMsgPk);
  return 0;
}
//导入导出  部门导入
int msaGroupInport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/groupimport.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
//读取文件
//static void striptrailingwhitespace(sa)
//stralloc *sa;
//{
// while (sa->len > 0)
//   switch(sa->s[sa->len - 1])
//    {
//     case '\n': case ' ': case '\t': case '\r':
//       --sa->len;
//       break;
//     default:
//       return;
//    }
//}
static char inbuf[512];
static stralloc line = {0};
void catch_alarm (int sig)
{
  _exit(0);
}
ltDbConn *G_DbCon; /*各个包处理进程的数据库连接*/
int msaGroupInputAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	buffer ss;
  int fd;
  int match;

  char groupname[32];
  char sip[32];
  char dip[512];
	char caTempDir[128];
  ltDbCursor *tempCursor;
	LT_DBROW tempRow;

	char sqlBuf[2048];
  char alias[2048];

  int  groupid;

  char dbUser[64];
  char dbPass[64];
  char dbName[64];
  int  i,k;
  int  xlen;
  char field[32];
  memset(field,0,sizeof(field));
//    if(argv[2]){
//    	sprintf(field,"%s",field);
//    }else{
  sprintf(field,"%s","11111111111111");
    //}
	printf("field:%s\n",field);
  fd = open_read(caTempDir);
  if (fd == -1)
  {
      printf("open user error\n");
      return 0;
  }
	sprintf(dbName,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbname"));
  sprintf(dbUser,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbuser"));
  sprintf(dbPass,"%s",bcCnfGetValueS("/etc/gnm/conf","db","dbpass"));
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (G_DbCon==NULL){
	          printf ("Couldn't open db connection.\n");
	           return 0;
	}
 	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	memset(groupname,0,sizeof(groupname));
	memset(sip,0,sizeof(sip));
	memset(dip,0,sizeof(dip));
  for (;;)
  {
	   if (getln(&ss,&line,&match,'\n') == -1) break;
	   if (!match && !line.len) { close(fd); return 1; }
	   striptrailingwhitespace(&line);
	   if (!stralloc_0(&line)) break;
	   if(line.len<5){
      // printf("update nasuser set username='%s',dispname='%s',group='%s',policyname='%s'\n",username,dispname,groupname,policyname);
       continue;
	   }
		   memset(groupname,0,sizeof(groupname));
       memset(sip,0,sizeof(sip));
       memset(dip,0,sizeof(dip));
 			 sprintf(alias,"%s",line.s);
 			xlen=strlen(alias);
			k=0;
			for(i=0;i<xlen;i++){
				if( alias[i]==',' ){
					break;
				}
				else{
					groupname[k]=alias[i];
					k++;
				}
			}
	    if(field[1]=='1'){
				k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						sip[k]=alias[i];
						k++;
					}
				}
		  }
	    if(field[2]=='1'){
			  k=0;
				for(i=i+1;i<xlen;i++){
					if( alias[i]==',' ){
						break;
					}
					else{
						dip[k]=alias[i];
						k++;
					}
				}
		  }
     //	vssf(line.s,"%s,%s,%s,%s,%s,%s,%s\r\n", username, dispname,groupname,pname,uip,umac,umail);
     	printf("1:%s  2:%s  3:%s\r\n", groupname,sip,dip);
      groupid=0;
			sprintf(sqlBuf, "select groupid from nasgroup where groupname='%s'",groupname);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						//printf("group %s exist\n",groupname);
						groupid=atol(tempRow[0]);

					}
					ltDbCloseCursor(tempCursor);
					sprintf(sqlBuf, "delete from nasgroupip where groupid='%d' ",groupid);
					ltDbExecSql(G_DbCon,sqlBuf);
					if( strlen(sip)>0 && strlen(dip)>0 ){
						  sprintf(sqlBuf,"insert into nasgroupip values(%d,%u,'%s',%u,'%s') ",
								groupid, ntohl(inet_addr(sip)),sip,ntohl(inet_addr(dip)),dip);
					    printf("%s\n",sqlBuf);
						 ltDbExecSql(G_DbCon,sqlBuf);
				  }
			}
			if(groupid==0){
				sprintf(sqlBuf, "select max(groupid)+1  from nasgroup ");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							groupid=atol(tempRow[0]);
						}
						ltDbCloseCursor(tempCursor);
				}
				sprintf(sqlBuf,"insert into nasgroup (groupid,groupname,grouphomeurl) values (%d,'%s','')",groupid,groupname);
				printf("%s\n",sqlBuf);
				ltDbExecSql(G_DbCon,sqlBuf);
				if( strlen(sip)>0 && strlen(dip)>0 ){
					sprintf(sqlBuf,"insert into nasgroupip values(%d,%u,'%s',%u,'%s') ",
							groupid, ntohl(inet_addr(sip)),sip,ntohl(inet_addr(dip)),dip);
				  printf("%s\n",sqlBuf);
					 ltDbExecSql(G_DbCon,sqlBuf);
				}
			}
	   //sleep(1);
  }
  close(fd);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,0);
	ltMsgFree(ltMsgPk);
	return 0;
}
//文件上传
int groupinportok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char tmpfold[64];
  char caTempDir[128];
  char caFile1[128];
  char fnames[512];
  int  filesize;
  DIR *dir;
  struct dirent *d;
  struct stat st;
  stralloc strTemp;
	strTemp.s=0;
	char caTmpp[1024];
	memset(caTmpp,0,sizeof(caTmpp));
	 /*得到WEB传上来的变量*/
	filesize=0;
	memset(caFile1,0,sizeof(caFile1));
	memset(fnames,0,sizeof(fnames));
	memset(tmpfold,0,sizeof(tmpfold));
	memset(caTempDir,0,sizeof(caTempDir));
  sprintf(caTempDir,"%s","/app/ns/upload");
  if(ltFileIsExist(caTempDir)!=1){
			sprintf(caTmpp,"{success:false,message:'Path1 error!'}");
			stralloc_cats(&strTemp,caTmpp);
      stralloc_0(&strTemp);
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
			ltMsgFree(ltMsgPk);
			return 0;
  }
  if(ltMsgGetVar_s(ltMsgPk,"tmpfold")){
  		sprintf(tmpfold,ltMsgGetVar_s(ltMsgPk,"tmpfold"));
  }
  if(strlen(tmpfold)<1){
	    sprintf(caTmpp,"{success:false,message:'Path2 error!'}");
			stralloc_cats(&strTemp,caTmpp);
      stralloc_0(&strTemp);
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
			ltMsgFree(ltMsgPk);
			return 0;
  }
  sprintf(caTempDir,"%s/%s/",caTempDir,tmpfold);
  if(ltFileIsExist(caTempDir)!=1){
      sprintf(caTmpp,"{success:false,message:'Path3 error!'}");
			stralloc_cats(&strTemp,caTmpp);
      stralloc_0(&strTemp);
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
			ltMsgFree(ltMsgPk);
			return 0;
  }
  if ( dir = opendir(caTempDir)) {
        while(d = readdir(dir)) {
            if (d->d_name[0] == '.') continue;
               sprintf(caFile1, "%s/%s", caTempDir, d->d_name);
               if( strcmp(d->d_name,"grouplist.txt")==0){
                   if (stat(caFile1,&st) == 0 && S_ISREG(st.st_mode)) {
                   	break;
                   }
                }
             }
   }
   sprintf(caTmpp,"{success:true,message:'Bingo!'}");
	 stralloc_cats(&strTemp,caTmpp);
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//策略组列表
int msaListUserPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *sid=ltMsgGetVar_s(ltMsgPk,"sid");
	 		sprintf(sqlStr,"delete from msauserpolicy where sid='%s'",sid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select sid,policyname,ratelimit,ratetype,prio from msauserpolicy order by prio DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//优先级别修改
int msaUpdatePrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *before,*after,*table;
	 char sql[128];
	 int type;
	 type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 before=ltMsgGetVar_s(ltMsgPk,"before");
	 after=ltMsgGetVar_s(ltMsgPk,"after");
	 if(type==0){
	 		sprintf(sql,"update %s set prio=%s where sid=%s",table,after,before);
	 }else if(type==1){
	 		sprintf(sql,"update %s set rulerate=%s where ruleid=%s",table,after,before);
	 }
	 ltDbExecSql(G_DbCon,sql);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",LT_TYPE_STRING,0);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//策略组添加
int msaAddPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char policy[129],policyname[64],topSrvNameList[36],moreflag[32],group0[255],group1[255],group2[255],group3[255],lLimitBytesw[64],lLimitBytes[64],lLimitBytesm[64],maxBytes[64],accountnum[32];
	 int lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc;
	 char sqlBuf[2048];
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
	 memset(policy,0,sizeof(policy));
	 memset(moreflag,0,sizeof(moreflag));
	 memset(topSrvNameList,0,sizeof(topSrvNameList));
	 long lCount;
	 int tttid;
   lCount=0;
   ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   for(tttid=1;tttid<100;tttid++){
		        sprintf(sqlBuf,"select count(*) from msauserpolicy where sid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if( atol(tempRow[0])==0){
									lCount=tttid;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
   }
	 lLimitTimes=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimes"));
	 lLimitTimesw=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw"));
	 lLimitTimesm=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm"));
	 jftype=atol(ltMsgGetVar_s(ltMsgPk,"jftype"));
	 maxpackage=atol(ltMsgGetVar_s(ltMsgPk,"maxpackage"));
	 maxudp=atol(ltMsgGetVar_s(ltMsgPk,"maxudp"));
	 maxtcp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcp"));
	 maxudpp=atol(ltMsgGetVar_s(ltMsgPk,"maxudpp"));
	 maxtcpp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcpp"));
	 maxupp=atol(ltMsgGetVar_s(ltMsgPk,"maxupp"));
	 maxdownp=atol(ltMsgGetVar_s(ltMsgPk,"maxdownp"));
	 maxsp=atol(ltMsgGetVar_s(ltMsgPk,"maxsp"));
	 maxnc=atol(ltMsgGetVar_s(ltMsgPk,"maxnc"));
	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 sprintf(policyname,"%s",ltMsgGetVar_s(ltMsgPk,"policyname"));
	 sprintf(topSrvNameList,"%s",ltMsgGetVar_s(ltMsgPk,"topSrvNameList"));
	 sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"moreflag"));
	 sprintf(lLimitBytesw,"%s",ltMsgGetVar_s(ltMsgPk,"lLimitBytesw"));
	 sprintf(lLimitBytes,"%s",ltMsgGetVar_s(ltMsgPk,"lLimitBytes"));
	 sprintf(lLimitBytesm,"%s",ltMsgGetVar_s(ltMsgPk,"lLimitBytesm"));
	 sprintf(maxBytes,"%s",ltMsgGetVar_s(ltMsgPk,"maxBytes"));
	 sprintf(accountnum,"%s",ltMsgGetVar_s(ltMsgPk,"accountnum"));
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"insert into msauserpolicy (policyid,policyname,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,sid)values \
('%s','%s','%s','%s',%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",policy,policyname,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,lCount);
	 printf("%s \n",sqlBuf);
	 ltDbExecSql(G_DbCon,sqlBuf);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,1,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml");
   ltMsgFree(ltMsgPk);
   return 0;
}
//编辑策略组
int msaEditPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int sid;
	 char sqlBuf[1024];
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   sid=atol(ltMsgGetVar_s(ltMsgPk,"sid"));
   sprintf(sqlBuf,"select * from msauserpolicy where sid=%d",sid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								lt_dbput_rootvars(dbPtr,23,
			       	 		"policyname",tempRow[1],
									"ratelimit",tempRow[3],
									"ratetype",tempRow[4],
									"topsrvnamelist",tempRow[5],
									"llimitbytes",tempRow[6],
									"llimittimes",tempRow[7],
									"llimitbytesw",tempRow[8],
									"llimittimesw",tempRow[9],
									"llimitbytesm",tempRow[10],
									"llimittimesm",tempRow[11],
									"moreflag",tempRow[12],
									"accountnum",tempRow[14],
									"maxpackage",tempRow[15],
									"maxbytes",tempRow[16],
									"maxudp",tempRow[17],
									"maxtcp",tempRow[18],
									"maxudpp",tempRow[19],
									"maxtcpp",tempRow[20],
									"maxupp",tempRow[21],
									"maxdownp",tempRow[22],
									"maxsp",tempRow[23],
									"maxnc",tempRow[24],
									"policyid",tempRow[25]
       	 				);
							}
							ltDbCloseCursor(tempCursor);
						}
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policylistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则列表
int msaSrvRuleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		sprintf(sqlStr,"delete from nassrvrule where ruleid='%s'",ruleid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from nassrvrule");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select ruleid,rulename,ruleflag,ruleaction,rulerate from nassrvrule order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//编辑规则
int msaEditSrvRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *type;
	 type=NULL;
	 char url[32];
	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 	 type=ltMsgGetVar_s(ltMsgPk,"type");
	 }
	 sprintf(url,"/app/msa/msa/htmlplt/users/%s.htm",type);
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//模板添加
int addMouldList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char sqlStr[1024];
	 int addtype;
	 addtype=0;
	 if(ltMsgGetVar_s(ltMsgPk,"addtype")!=NULL){
	 	 addtype=atol(ltMsgGetVar_s(ltMsgPk,"addtype"));
	 }
	 if(addtype==0){
	 	 sprintf(sqlStr,"insert into %s values (%s,'%s','%s')",ltMsgGetVar_s(ltMsgPk,"table"),ltMsgGetVar_s(ltMsgPk,"ruleid"),ltMsgGetVar_s(ltMsgPk,"first"),ltMsgGetVar_s(ltMsgPk,"second"));
	 }else if(addtype==1){
	 	 sprintf(sqlStr,"insert into %s values (%s,'%s')",ltMsgGetVar_s(ltMsgPk,"table"),ltMsgGetVar_s(ltMsgPk,"ruleid"),ltMsgGetVar_s(ltMsgPk,"url"));
	 }
	 printf("%s \n",sqlStr);
	 ltDbExecSql(G_DbCon,sqlStr);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,1,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml");
   ltMsgFree(ltMsgPk);
   return 0;
}
//模板列表
int mouldList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount,k;
	 char *table;
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 k=0;
	 int delflag,ruleid;
	 delflag=-1;
	 ruleid=-1;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=atoi(ltMsgGetVar_s(ltMsgPk,"delflag"));
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"ruleid")!=NULL){
	 	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 }
	 memset(sqlStr,0,sizeof(sqlStr));
	 if(delflag!=-1){
		 if(delflag==0){
		 		char *ipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
		 		char *ipend=ltMsgGetVar_s(ltMsgPk,"ipend");
		 		sprintf(sqlStr,"delete from %s where ipstart='%s' and ipend='%s'",table,ipstart,ipend);
		 		printf("%s \n",sqlStr);
		 		ltDbExecSql(G_DbCon,sqlStr);
		 } else if (delflag==1){
				char *type=ltMsgGetVar_s(ltMsgPk,"type");
				char *url=ltMsgGetVar_s(ltMsgPk,"url");
		 		sprintf(sqlStr,"delete from %s where %s='%s'",table,type,url);
		 		printf("%s \n",sqlStr);
		 		ltDbExecSql(G_DbCon,sqlStr);
		 }
	 }
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select count(*) from %s",table);
	 printf("SQL %s \n",sqlStr);
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select * from %s where ruleid=%d",table,ruleid);
	 printf("SQL %s \n",sqlStr);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
		 		printf("1111111 \n");
       	while(tempRow!=NULL){
       		if(k==0){
       			 printf("222222 \n");
			     	  	sprintf(caTmpp,"{\"first\":\"%s\",\"second\":\"%s\"}",tempRow[1],tempRow[2]);
			     	    stralloc_cats(&strTemp,caTmpp);
			     	 printf("5555555 \n");
	     	  }else{
						 printf("666666666 \n");
			     	  	sprintf(caTmpp,",{\"first\":\"%s\",\"second\":\"%s\"}",tempRow[1],tempRow[2]);
	     	     		stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  printf("loop \n");
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
		 			printf("77777777 \n");
       	}
       	ltDbCloseCursor(tempCursor);
   }

		 printf("88888888 \n");
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//服务策略组添加
//int add(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//	 int ruleid,ruleflag,ruleaction,
//	 char rulename[32],
//	 
//}
