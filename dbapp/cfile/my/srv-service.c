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
//显示规则模板
int showService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}

	 char sqlBuf[128];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"qoslist");
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							lt_dbput_recordvars(tablePtr,3,
							  "qosid",LT_TYPE_STRING,tempRow[0],
								"qosname",LT_TYPE_STRING,tempRow[1]);
						    tempRow=ltDbFetchRow(tempCursor);
					}
			ltDbCloseCursor(tempCursor);
		}
		int i;
		i=0;
		char strid[8];
		memset(strid,0,sizeof(strid));
		tablePtr=lt_dbput_table(dbPtr,"jjrlist");
		for(i=0;i<NAS_MAXJJR_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_jjrtimelist[i].jjrid);
				if(strcmp(_ltPubInfo->_jjrtimelist[i].jjrname,"")!=0){
					lt_dbput_recordvars(tablePtr,2,
					  "jjrid",LT_TYPE_STRING,strid,
						"jjrname",LT_TYPE_STRING,_ltPubInfo->_jjrtimelist[i].jjrname);
				}
		}
		tablePtr=lt_dbput_table(dbPtr,"schlist");
		memset(strid,0,sizeof(strid));
		for(i=0;i<NAS_MAXSCH_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_schtimelist[i].schid);
				if(strcmp(_ltPubInfo->_schtimelist[i].schname,"")!=0){
					lt_dbput_recordvars(tablePtr,2,
					  "schid",LT_TYPE_STRING,strid,
						"schname",LT_TYPE_STRING,_ltPubInfo->_schtimelist[i].schname);
				}
		}
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/serviceAdd.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则添加
int addService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr;
	 char rulename[32],ruleurl[128],sqlstr[2048],policy[129],group[1001],g1[251],g2[251],g3[251],g4[251];
	 char ruleid[8];
	 int tttid,lCount;
	 float temprio;
	 temprio=0;
	 lCount=0;
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 for(tttid=1;tttid<_MAX_SRVRULE_NUM;tttid++){
	 					memset(sqlstr,0,sizeof(sqlstr));
		        sprintf(sqlstr,"select count(*) from nassrvrule where ruleid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
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
   //优先级别查找
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"%s","select rulerate from nassrvrule order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
	 if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								temprio=atoi(tempRow[0])+1.0;
							}
							ltDbCloseCursor(tempCursor);
	 }
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
	 ruleschedule=atoi(ltMsgGetVar_s(ltMsgPk,"ruleschedule"));
	 rulejjr=atoi(ltMsgGetVar_s(ltMsgPk,"rulejjr"));
	 sprintf(rulename,"%s",ltMsgGetVar_s(ltMsgPk,"rulename"));
	 sprintf(ruleurl,"%s",ltMsgGetVar_s(ltMsgPk,"ruleurl"));
	 sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));
 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nassrvrule(ruleid,ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr,rulename,ruleurl,group0,group1,group2,group3,policyid,rulerate) values (%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s',%f)",lCount,ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr,rulename,ruleurl,g1,g2,g3,g4,policy,temprio);
	 ltDbExecSql(G_DbCon,sqlstr);
	 sprintf(ruleid,"%d",lCount);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,ruleid);
   ltMsgFree(ltMsgPk);
   return 0;
}
//规则修改
int showEditService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char group[1001],rulesrv[501],contenttype[751];
	 char sqlBuf[128];
	 int myratelimit,ruleschedule,rulejjr;
	 char str[16];
	 myratelimit=-1;
	 ruleschedule=-1;
	 rulejjr=-1;
	 ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								memset(group,0,sizeof(group));
								sprintf(group,"%s%s%s%s",tempRow[8],tempRow[9],tempRow[10],tempRow[11]);

								memset(str,0,sizeof(str));
								sprintf(str,"ruleflag%s",tempRow[2]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(str,0,sizeof(str));
								sprintf(str,"logaction%s",tempRow[14]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(str,0,sizeof(str));
								sprintf(str,"ruleaction%s",tempRow[3]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(str,0,sizeof(str));
								sprintf(str,"conaction%s",tempRow[15]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(rulesrv,0,sizeof(rulesrv));
								sprintf(rulesrv,"%s%s",tempRow[6],tempRow[7]);
								memset(contenttype,0,sizeof(contenttype));
								sprintf(contenttype,"%s%s%s",tempRow[19],tempRow[20],tempRow[21]);
								lt_dbput_rootvars(dbPtr,7,
			       	 		"rulename",tempRow[1],
			       	 		"policy",tempRow[12],
			       	 		"group",group,
			       	 		"ruleurl",tempRow[18],
			       	 		"rulesrv",rulesrv,
			       	 		"urlsort",tempRow[16],
			       	 		"contenttype",contenttype
       	 				);
       	 				myratelimit=atoi(tempRow[13]);
       	 				ruleschedule=atoi(tempRow[4]);
       	 				rulejjr=atoi(tempRow[5]);
							}
							ltDbCloseCursor(tempCursor);
						}
						char qossel[32];
						char schedulesel[32];
						char jjrsel[32];
						tablePtr=lt_dbput_table(dbPtr,"qoslist");
						memset(sqlBuf,0,sizeof(sqlBuf));
						sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
						tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor!=NULL){
									tempRow=ltDbFetchRow(tempCursor);
									while(tempRow!=NULL){
											memset(qossel,0,sizeof(qossel));
											if(atoi(tempRow[0])==myratelimit){
													sprintf(qossel,"%s","selected");
											}
											lt_dbput_recordvars(tablePtr,3,
											  "qosid",LT_TYPE_STRING,tempRow[0],
												"sel",LT_TYPE_STRING,qossel,
												"qosname",LT_TYPE_STRING,tempRow[1]);
										    tempRow=ltDbFetchRow(tempCursor);
									}
									ltDbCloseCursor(tempCursor);
						}
		int i;
		i=0;
		char strid[8];
		memset(strid,0,sizeof(strid));
		tablePtr=lt_dbput_table(dbPtr,"jjrlist");
		for(i=0;i<NAS_MAXJJR_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_jjrtimelist[i].jjrid);
				if(strcmp(_ltPubInfo->_jjrtimelist[i].jjrname,"")!=0){
					memset(jjrsel,0,sizeof(jjrsel));
					if(_ltPubInfo->_jjrtimelist[i].jjrid==rulejjr){
							sprintf(jjrsel,"%s","selected");
					}
					lt_dbput_recordvars(tablePtr,3,
					  "jjrid",LT_TYPE_STRING,strid,
					  "jjrsel",LT_TYPE_STRING,jjrsel,
						"jjrname",LT_TYPE_STRING,_ltPubInfo->_jjrtimelist[i].jjrname);
				}
		}
		tablePtr=lt_dbput_table(dbPtr,"schlist");
		memset(strid,0,sizeof(strid));
		for(i=0;i<NAS_MAXSCH_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_schtimelist[i].schid);
				if(strcmp(_ltPubInfo->_schtimelist[i].schname,"")!=0){
					memset(schedulesel,0,sizeof(schedulesel));
					if(_ltPubInfo->_schtimelist[i].schid==ruleschedule){
													sprintf(schedulesel,"%s","selected");
					}
					lt_dbput_recordvars(tablePtr,3,
					  "schid",LT_TYPE_STRING,strid,
					  "schsel",LT_TYPE_STRING,schedulesel,
						"schname",LT_TYPE_STRING,_ltPubInfo->_schtimelist[i].schname);
				}
		}
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/SrvRulelistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑规则
int msaEditSrvRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int ruleid,ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr;
	 char ruleurl[128],sqlstr[4096],policy[129],group[1001],g1[251],g2[251],g3[251],g4[251],ruleurlsort[101],rulesrv[501],rulesrv0[251],rulesrv1[251],contenttype[751],contenttype1[251],contenttype2[251],contenttype3[251];
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 memset(ruleurlsort,0,sizeof(ruleurlsort));
	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
	 ruleschedule=atoi(ltMsgGetVar_s(ltMsgPk,"ruleschedule"));
	 rulejjr=atoi(ltMsgGetVar_s(ltMsgPk,"rulejjr"));
	 sprintf(ruleurl,"%s",ltMsgGetVar_s(ltMsgPk,"ruleurl"));
	 sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 sprintf(ruleurlsort,"%s",ltMsgGetVar_s(ltMsgPk,"urlsort"));
	 sprintf(rulesrv,"%s",ltMsgGetVar_s(ltMsgPk,"rulesrv"));
	 sprintf(contenttype,"%s",ltMsgGetVar_s(ltMsgPk,"contenttype"));

	 memset(rulesrv0,0,sizeof(rulesrv0));
	 memset(rulesrv1,0,sizeof(rulesrv1));
 	 memcpy(rulesrv0,rulesrv,250);
	 memcpy(rulesrv1,rulesrv+250,250);


	 memset(contenttype1,0,sizeof(contenttype1));
	 memset(contenttype2,0,sizeof(contenttype2));
	 memset(contenttype3,0,sizeof(contenttype3));
 	 memcpy(contenttype1,contenttype,250);
	 memcpy(contenttype2,contenttype+250,250);
	 memcpy(contenttype3,contenttype+500,250);

	 memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));
 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"update nassrvrule set ruleflag=%d,ruleaction=%d,logaction=%d,conaction=%d,qosid=%d,ruleschedule=%d,rulejjr=%d,rulename='%s',ruleurl='%s',group0='%s',group1='%s',group2='%s',group3='%s',policyid='%s',ruleurlsort='%s',rulesrv0='%s',rulesrv1='%s',contenttype1='%s',contenttype2='%s',contenttype3='%s' where ruleid=%d",ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr,ltMsgGetVar_s(ltMsgPk,"rulename"),ruleurl,g1,g2,g3,g4,policy,ruleurlsort,rulesrv0,rulesrv1,contenttype1,contenttype2,contenttype3,ruleid);
	 printf("SQL: %s  \n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
//规则列表
int msaSrvRuleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		sprintf(sqlStr,"delete from nassrvrule where ruleid=%s",ruleid);
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
	 sprintf(sqlStr,"%s","select ruleid,rulename,ruleflag,ruleaction,rulerate,logaction,conaction,qosid from nassrvrule order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s,\"logaction\":%s,\"conaction\":%s,\"qosname\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],_ltPubInfo->_QosList[atoi(tempRow[7])].qosname);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s,\"logaction\":%s,\"conaction\":%s,\"qosname\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],_ltPubInfo->_QosList[atoi(tempRow[7])].qosname);
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
//编辑webpost
int msaEditWebpost(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlBuf[128],str[128];
	 char *addflag,*delflag;
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into nasruleurl values(%s,%s,%s,'%s')",ruleid,ltMsgGetVar_s(ltMsgPk,"utf8"),ltMsgGetVar_s(ltMsgPk,"action"),caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from nasruleurl where ruleid=%s and caurl='%s'",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nasruleurl where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				memset(str,0,sizeof(str));
				if(atoi(tempRow[1])==1){
					sprintf(str,"|%s","UTF8");
				}
				switch(atoi(tempRow[2])){
					case 0:
					sprintf(str,"%s|webGet",str);
					break;
					case 1:
					sprintf(str,"%s|webPost",str);
					break;
					case 2:
					sprintf(str,"%s|All",str);
					break;
				}
				lt_dbput_recordvars(tablePtr,2,
				"type",LT_TYPE_STRING,str,
				"caurl",LT_TYPE_STRING,tempRow[3]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/webpost.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑weburl
int msaEditWeburl(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into nasruleurl values(%s,'%s')",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from nasruleurl where ruleid=%s and caurl='%s'",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nasruleurl where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,1,
				"caurl",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/weburl.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑mudip userip
int msaEditip(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 char url[64];
	 memset(url,0,sizeof(url));
	 if(strcmp(ltMsgGetVar_s(ltMsgPk,"table"),"nassrvruledip")==0){
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/users/mudip.htm");
	 }else if(strcmp(ltMsgGetVar_s(ltMsgPk,"table"),"nasruleip")==0){
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/users/userip.htm");
	 }
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *ipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	 		char *ipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	 		char *table=ltMsgGetVar_s(ltMsgPk,"table");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into %s values(%s,'%s','%s')",table,ruleid,ipstart,ipend);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *ipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	 		char *ipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	 		char *table=ltMsgGetVar_s(ltMsgPk,"table");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from %s where ruleid=%s and ipstart='%s' and ipend='%s'",table,ruleid,ipstart,ipend);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from %s where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"table"),ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,
				"ipstart",LT_TYPE_STRING,tempRow[1],
				"ipend",LT_TYPE_STRING,tempRow[2]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示选择用户树
int ShowSelectUserTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char url[64];
	 memset(url,0,sizeof(url));
	 sprintf(url,"/app/msa/msa/htmlplt/users/%s.htm",ltMsgGetVar_s(ltMsgPk,"name"));
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   lt_dbput_rootvars(dbPtr,1,"groupid",ltMsgGetVar_s(ltMsgPk,"groupid"));
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//部门可配置用户
int ruleGroupUserlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int groupid,level;
	 int allcount,k,iii;
	 k=0;
	 iii=0;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 groupid=atoi(ltMsgGetVar_s(ltMsgPk,"groupid"));
	 level=_ltPubInfo->_GroupList[groupid].level;
	 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 nasDbUserInfo *tmpdbuserinfo;
	 for(iii=0;iii<_ltPubInfo->_dbUserInfoHead.bufnum;iii++){
		 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,iii);
	   if(tmpdbuserinfo!=NULL){
	   		if(tmpdbuserinfo->groupid[level]==groupid){
	     			if(k==0){
	     	  	 sprintf(caTmpp,"{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\"}",tmpdbuserinfo->userid,tmpdbuserinfo->username,tmpdbuserinfo->dispname);
	     	     stralloc_cats(&strTemp,caTmpp);
		     	  }else{
		     	  	 sprintf(caTmpp,",{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\"}",tmpdbuserinfo->userid,tmpdbuserinfo->username,tmpdbuserinfo->dispname);
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
//策略已配置用户列表
int serviceUserlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	  delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *userid=ltMsgGetVar_s(ltMsgPk,"userid");
	 		sprintf(sqlStr,"delete from nasruleuser where ruleid=%s and userid=%s",ruleid,userid);
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
	 sprintf(sqlStr,"select count(*) from nasruleuser where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
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
	 sprintf(sqlStr,"select * from nasruleuser where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
	 nasDbUserInfo *tmpdbuserinfo;
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
	 		 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,atoi(tempRow[1]));
       		if(tmpdbuserinfo!=NULL){
       				if(k==0){
			     	  	 sprintf(caTmpp,"{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\"}",tempRow[1],tmpdbuserinfo->username,tmpdbuserinfo->dispname);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\"}",tempRow[1],tmpdbuserinfo->username,tmpdbuserinfo->dispname);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
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
//配置用户
int addruleuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlstr[1024];
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 char *userid=ltMsgGetVar_s(ltMsgPk,"userid");
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select * from nasruleuser where userid=%s and ruleid=%s",userid,ruleid);
   tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
					   ltMsgPk->msgpktype=1;
						 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{success:false,data:{msg:'unsuccessful'}}");
						 ltMsgFree(ltMsgPk);
						 return 0;
			  }
       	ltDbCloseCursor(tempCursor);
   }

	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nasruleuser values(%s,%s)",ruleid,userid);
	 ltDbExecSql(G_DbCon,sqlstr);
	 ltDbClose(G_DbCon);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{success:true,data:{msg:'add successful'}}");
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//规则的URL库修改
int ShowUrlSort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int i;
	 char ruleurlsort[101],str[8];
	 memset(ruleurlsort,0,sizeof(ruleurlsort));
	 i=0;
	 char sqlstr[1024];
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));

	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select ruleurlsort from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       			sprintf(ruleurlsort,"%s",tempRow[0]);
   					lt_dbput_rootvars(dbPtr,1,"ruleurlsort",tempRow[0]);
						tempRow= ltDbFetchRow(tempCursor);
			  }
       	ltDbCloseCursor(tempCursor);
   }
   for(i=0;i<100;i++){
        memset(str,0,sizeof(str));
				sprintf(str,"chksort%d",i);
				if(ruleurlsort[i]==49){
					 lt_dbput_rootvars(dbPtr,1,str,"checked");
				}
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/urladdress.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则的URL库修改
int ShowAddress(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int i;
	 char rulesrv[511],str[8];
	 memset(rulesrv,0,sizeof(rulesrv));
	 i=0;
	 char sqlstr[1024];
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select rulesrv0,rulesrv1 from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
   					sprintf(rulesrv,"%s%s",tempRow[0],tempRow[1]);
						tempRow= ltDbFetchRow(tempCursor);
			  }
       	ltDbCloseCursor(tempCursor);
   }
   lt_dbput_rootvars(dbPtr,1,"rulesrv",rulesrv);
   for(i=0;i<500;i++){
        memset(str,0,sizeof(str));
				sprintf(str,"chksort%d",i);
				if(rulesrv[i]==49){
					 lt_dbput_rootvars(dbPtr,1,str,"checked");
				}
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/serviceaddress.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑keyword
int editkeyword(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlBuf[128],page[128],code[16];
	 char *addflag,*delflag,*type;
	 addflag=NULL;
	 delflag=NULL;
	 type=NULL;
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 memset(code,0,sizeof(code));
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 	 type=ltMsgGetVar_s(ltMsgPk,"type");
	 }
	 if(addflag){
	 		int lCount;
	 		lCount=0;
	 		char *host=ltMsgGetVar_s(ltMsgPk,"host");
	 		char *keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
	 		char *utf8=ltMsgGetVar_s(ltMsgPk,"utf8");
	 		char *web=ltMsgGetVar_s(ltMsgPk,"web");

	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"select max(kid)+1 from naskeyword");
      tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
      if(tempCursor!=NULL){
         tempRow= ltDbFetchRow(tempCursor);
         if(tempRow!=NULL){
                  lCount=atoi(tempRow[0]);
         }
         ltDbCloseCursor(tempCursor);
			}
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into naskeyword values(%d,'%s','%s',%s,%s,%s)",lCount,host,keyword,utf8,web,type);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *id=ltMsgGetVar_s(ltMsgPk,"id");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from naskeyword where kid=%s",id);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }

	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from naskeyword where type=%s",type);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				memset(code,0,sizeof(code));
				if(atoi(tempRow[3])==1){
						sprintf(code,"%s-","utf8");
				}
				if(atoi(tempRow[4])==1){
						sprintf(code,"%s-web",code);
				}
				lt_dbput_recordvars(tablePtr,4,
					"id",LT_TYPE_STRING,tempRow[0],
					"host",LT_TYPE_STRING,tempRow[1],
					"keyword",LT_TYPE_STRING,tempRow[2],
					"code",LT_TYPE_STRING,code);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
	 if(atoi(type)==0){
	 		sprintf(page,"%s","/app/msa/msa/htmlplt/users/formkeyword.htm");
	 }else if(atoi(type)==1){
	 		sprintf(page,"%s","/app/msa/msa/htmlplt/users/webkeyword.htm");
	 }
   lt_dbput_rootvars(dbPtr,1,"type",type);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,page,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//msaService 树
int msaServiceLoop(stralloc *strTemp,int srvnum,char rulesrv[]){
	char tmpstr[512];
	int iHead,i;
	i=0;
	iHead=1;
 	for(i=0;i<MAX_SYS_SRVNUM;i++){
 			if(_ltPubInfo->ltService[i].topSrv==srvnum&&strcmp(_ltPubInfo->ltService[i].srvName,"")!=0){
				if(iHead==1){
		 			sprintf(tmpstr,"%s",",children: [");
				  stralloc_cats(strTemp,tmpstr);
					if(rulesrv[i]==49){
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"true");
					}else{
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"false");
				  }
					iHead=0;
				}else{
					if(rulesrv[i]==49){
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"true");
					}else{
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"false");
				  }
			  }
			  stralloc_cats(strTemp,tmpstr);
				stralloc_cats(strTemp,"}");
			}
	}

	if(iHead==1){
  	  sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			return 0;
  }
	stralloc_cats(strTemp,"]");
	return 0;
}

int msaltServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	sprintf(_ltPubInfo->ltService[0].srvName,"%s","000000000");
	_ltPubInfo->ltService[0].topSrv=0;

	sprintf(_ltPubInfo->ltService[1].srvName,"%s","111111111");
	_ltPubInfo->ltService[1].topSrv=1;

	sprintf(_ltPubInfo->ltService[2].srvName,"%s","222222222");
	_ltPubInfo->ltService[2].topSrv=2;


	sprintf(_ltPubInfo->ltService[3].srvName,"%s","3333333333");
	_ltPubInfo->ltService[3].topSrv=3;


	char tmpstr[512],rulesrv[501];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	sprintf(rulesrv,"%s",ltMsgGetVar_s(ltMsgPk,"rulesrv"));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_TOP_SRVNUM;j++){
 			if(strcmp(_ltPubInfo->topSrvName[j].srvname,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{text: '%s',checked:false",_ltPubInfo->topSrvName[j].srvname);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{text: '%s',checked:false",_ltPubInfo->topSrvName[j].srvname);
				  }
				  stralloc_cats(&strTemp,tmpstr);
					msaServiceLoop(&strTemp,j,rulesrv);
					stralloc_cats(&strTemp,"}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}
//static int msaSortContentType(const void *p1,const void *p2)
//{
//    userlist *gname1,*gname2;
//    gname1 = (userlist *)p1;
//    gname2 = (userlist *)p2;
//    return (gname2->groupname - gname1->groupname);
//}


//contentTypeTree 树
static char inbuf[512];
static stralloc line = {0};
int contentTypeTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char tmpstr[512],caTempDir[128],contenttype[751];
	stralloc strTemp;
  int fd,match,k,treetype;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
	sprintf(contenttype,"%s",ltMsgGetVar_s(ltMsgPk,"contenttype"));
	treetype=atoi(ltMsgGetVar_s(ltMsgPk,"treetype"));
	buffer ss;
	char *pppp;
  char strContentType[64];
  char tempstr[64];
  sprintf(caTempDir,"%s","/app/msa/bin/msaContentType");
	if(ltFileIsExist(caTempDir)!=1) {
      return 0;
  }
  fd = open_read(caTempDir);
  if (fd == -1)
  {
      return 0;
  }
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
  for(k=0;k<_MAX_CONTENTTYPE_NUM;k++){
  		 memset(tempstr,0,sizeof(tempstr));
			 if (getln(&ss,&line,&match,'\n') == -1) {
			 		  break;
			 }
			 if (!match && !line.len) {
			 	   break;
			 }
			 striptrailingwhitespace(&line);
			 if (!stralloc_0(&line)) {
			 		  break;
			 }
			 sprintf(strContentType,"%s",line.s);
			 pppp=strchr(strContentType,',');
			 if(!pppp){
			 	  continue;
			 }
			 *pppp='\0';
			 pppp++;
			 sprintf(tempstr,"%s",pppp);
			 pppp=strchr(tempstr,',');
			 if(!pppp){
			 	  continue;
			 }
			 *pppp='\0';
			 pppp++;
			 if(atoi(pppp)!=treetype){
			 		continue;
			 }
			 printf("strContentType :%s  \n",strContentType);
			 printf("%d \n   %c \n",atoi(strContentType),contenttype[atoi(strContentType)]);
			 if(iHead==1){
						if(contenttype[atoi(strContentType)]==49){
							sprintf(tmpstr,"{id: '%s',text: '%s',checked:%s,leaf:true",strContentType,tempstr,"true");
						}else{
							sprintf(tmpstr,"{id: '%s',text: '%s',checked:%s,leaf:true",strContentType,tempstr,"false");
						}
					  iHead=0;
			 }else{
						if(contenttype[atoi(strContentType)]==49){
						  sprintf(tmpstr,",{id: '%s',text: '%s',checked:%s,leaf:true",strContentType,tempstr,"true");
						}else{
						  sprintf(tmpstr,",{id: '%s',text: '%s',checked:%s,leaf:true",strContentType,tempstr,"false");
						}
			 }
			 stralloc_cats(&strTemp,tmpstr);
			 stralloc_cats(&strTemp,"}");
	}
	close(fd);
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}
//ruleurlsort 树
int msaUrlsortLoop(stralloc *strTemp,int srvnum,char urlsort[]){
	char tmpstr[512];
	int iHead,i;
	i=0;
	iHead=1;
 	for(i=0;i<NAS_MAXURLSORT_NUM;i++){
 			if(_ltPubInfo->_urlSortlist[i].toptype==srvnum&&strcmp(_ltPubInfo->_urlSortlist[i].sortname,"")!=0){
				if(iHead==1){
		 			sprintf(tmpstr,"%s",",children: [");
				  stralloc_cats(strTemp,tmpstr);
					if(urlsort[i]==49){
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->_urlSortlist[i].sortname,"true");
					}else{
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->_urlSortlist[i].sortname,"false");
				  }
					iHead=0;
				}else{
					if(urlsort[i]==49){
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->_urlSortlist[i].sortname,"true");
					}else{
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->_urlSortlist[i].sortname,"false");
				  }
			  }
			  stralloc_cats(strTemp,tmpstr);
				stralloc_cats(strTemp,"}");
			}
	}

	if(iHead==1){
  	  sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			return 0;
  }
	stralloc_cats(strTemp,"]");
	return 0;
}

int msaUrlsortTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	sprintf(_ltPubInfo->_topurlSortlist[0].topsortname,"%s","sort0");
	_ltPubInfo->_topurlSortlist[0].topsortid=0;
	sprintf(_ltPubInfo->_topurlSortlist[1].topsortname,"%s","sort1");
	_ltPubInfo->_topurlSortlist[1].topsortid=1;
	sprintf(_ltPubInfo->_topurlSortlist[2].topsortname,"%s","sort2");
	_ltPubInfo->_topurlSortlist[2].topsortid=2;
	sprintf(_ltPubInfo->_topurlSortlist[3].topsortname,"%s","sort3");
	_ltPubInfo->_topurlSortlist[3].topsortid=3;
	
	
	_ltPubInfo->_urlSortlist[0].sortid=0;
	_ltPubInfo->_urlSortlist[0].toptype=0;
	sprintf(_ltPubInfo->_urlSortlist[0].sortname,"%s","000000000");

	_ltPubInfo->_urlSortlist[1].sortid=1;
	_ltPubInfo->_urlSortlist[1].toptype=1;
	sprintf(_ltPubInfo->_urlSortlist[1].sortname,"%s","111111111");
	
	_ltPubInfo->_urlSortlist[2].sortid=2;
	_ltPubInfo->_urlSortlist[2].toptype=2;
	sprintf(_ltPubInfo->_urlSortlist[2].sortname,"%s","2222222222");
	
	_ltPubInfo->_urlSortlist[3].sortid=3;
	_ltPubInfo->_urlSortlist[3].toptype=3;
	sprintf(_ltPubInfo->_urlSortlist[3].sortname,"%s","3333333333");

printf("11111111111\n  ");
	char tmpstr[512],urlsort[101];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	sprintf(urlsort,"%s",ltMsgGetVar_s(ltMsgPk,"urlsort"));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<NAS_MAXTOPURLSORT_NUM;j++){
 			if(strcmp(_ltPubInfo->_topurlSortlist[j].topsortname,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{text: '%s',checked:false",_ltPubInfo->_topurlSortlist[j].topsortname);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{text: '%s',checked:false",_ltPubInfo->_topurlSortlist[j].topsortname);
				  }
				  stralloc_cats(&strTemp,tmpstr);
					msaUrlsortLoop(&strTemp,j,urlsort);
					stralloc_cats(&strTemp,"}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}
//添加防火墙规则
int showFWrulepg(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 		char caTempDir[128];
		  int fd,match;
			buffer ss;
			char *pppp;
		  char str[512];
		  
			
		 char sqlBuf[128];
	   ltDbHeadPtr dbPtr;
	   ltTablePtr tablePtr;
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
		 dbPtr=lt_dbinit();
	   lt_db_htmlpage(dbPtr,"utf-8");
		 tablePtr=lt_dbput_table(dbPtr,"qoslist");
		 memset(sqlBuf,0,sizeof(sqlBuf));
		 sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
		 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		 if(tempCursor!=NULL){
						tempRow=ltDbFetchRow(tempCursor);
						while(tempRow!=NULL){
								lt_dbput_recordvars(tablePtr,3,
								  "qosid",LT_TYPE_STRING,tempRow[0],
									"qosname",LT_TYPE_STRING,tempRow[1]);
							    tempRow=ltDbFetchRow(tempCursor);
						}
				ltDbCloseCursor(tempCursor);
		 }
		 
		 
	   ltTablePtr tablePtr1;
	   ltTablePtr tablePtr2;
		 tablePtr1=lt_dbput_table(dbPtr,"inputdev");
		 tablePtr2=lt_dbput_table(dbPtr,"outputdev");
		 sprintf(caTempDir,"%s","/datacenter/tmp/tmpdevinfo");
		 if(ltFileIsExist(caTempDir)!=1) {
	      return 0;
	   }
	   fd = open_read(caTempDir);
	   if (fd == -1)
	   {
	      return 0;
	   }
	   buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	   for(;;){
				 if (getln(&ss,&line,&match,'\n') == -1) {
				 		  break;
				 }
				 if (!match && !line.len) {
				 	   break;
				 }
				 striptrailingwhitespace(&line);
				 if (!stralloc_0(&line)) {
				 		  break;
				 }
				 sprintf(str,"%s",line.s);
				 pppp=strchr(str,'e');
				 if(!pppp){
				 	  continue;
				 }
				 pppp--;
				 *pppp='\0';
				 pppp++;
			 	 sprintf(str,"%s",pppp);
				 pppp=strchr(str,':');
				 if(!pppp){
				 	  continue;
				 }
				 *pppp='\0';
				 if(str[0]=='e'&&str[1]=='t'&&str[2]=='h'){
				 		lt_dbput_recordvars(tablePtr1,1,"name",LT_TYPE_STRING,str);
				 		lt_dbput_recordvars(tablePtr2,1,"name",LT_TYPE_STRING,str);
				 }
		 }
		 close(fd);
		 
	   ltMsgPk->msgpktype=1;
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/policycenter/addRule.htm",dbPtr->head,0);
	   ltMsgFree(ltMsgPk);
	   lt_dbfree(dbPtr);
	   return 0;
}
//规则添加
int addFWrule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 int ruleflag,ruleaction,qosid;
	 char rulename[32],sqlstr[2048],lips[16],lipe[16],rips[16],ripe[16],finputdev[16],foutputdev[16];
	 char ruleid[8];
	 char *type;
	 int tttid,lCount,id,fprotocol,lportl,lportu,rportl,rportu;
	 float temprio;
	 temprio=0;
	 lCount=0;
	 id=-1;
	 type=NULL;
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
	 sprintf(rulename,"%s",ltMsgGetVar_s(ltMsgPk,"rulename"));
	 sprintf(lips,"%s",ltMsgGetVar_s(ltMsgPk,"lips"));
	 sprintf(lipe,"%s",ltMsgGetVar_s(ltMsgPk,"lipe"));
	 sprintf(rips,"%s",ltMsgGetVar_s(ltMsgPk,"rips"));
	 sprintf(ripe,"%s",ltMsgGetVar_s(ltMsgPk,"ripe"));
	 sprintf(finputdev,"%s",ltMsgGetVar_s(ltMsgPk,"finputdev"));
	 sprintf(foutputdev,"%s",ltMsgGetVar_s(ltMsgPk,"foutputdev"));
	 fprotocol=atoi(ltMsgGetVar_s(ltMsgPk,"fprotocol"));
	 lportl=atoi(ltMsgGetVar_s(ltMsgPk,"lportl"));
	 lportu=atoi(ltMsgGetVar_s(ltMsgPk,"lportu"));
	 rportl=atoi(ltMsgGetVar_s(ltMsgPk,"rportl"));
	 rportu=atoi(ltMsgGetVar_s(ltMsgPk,"rportu"));
	 if(ltMsgGetVar_s(ltMsgPk,"type")){
	 		type=ltMsgGetVar_s(ltMsgPk,"type");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"ruleid")){
	 		id=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 }
	 if(type){
	 		 memset(sqlstr,0,sizeof(sqlstr));
			 sprintf(sqlstr,"update nasfirewall set ruleflag=%d,ruleaction=%d,qosid=%d,rulename='%s',lips='%s',lipe='%s',rips='%s',ripe='%s',finputdev='%s',foutputdev='%s',fprotocol=%d,lportl=%d,lportu=%d,rportl=%d,rportu=%d where ruleid=%d",ruleflag,ruleaction,qosid,rulename,lips,lipe,rips,ripe,finputdev,foutputdev,fprotocol,lportl,lportu,rportl,rportu,id);
			 printf("%s \n",sqlstr);
			 ltDbExecSql(G_DbCon,sqlstr);
			 ltDbClose(G_DbCon);
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,0);
		   ltMsgFree(ltMsgPk);
		   return 0;
	 }
	 for(tttid=1;tttid<MAX_FIREWALL_NUM;tttid++){
	 					memset(sqlstr,0,sizeof(sqlstr));
		        sprintf(sqlstr,"select count(*) from nasfirewall where ruleid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
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
   //优先级别查找
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"%s","select rulerate from nasfirewall order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
	 if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								temprio=atoi(tempRow[0])+1.0;
							}
							ltDbCloseCursor(tempCursor);
	 }
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nasfirewall(ruleid,ruleflag,ruleaction,qosid,rulename,rulerate,lips,lipe,rips,ripe,finputdev,foutputdev,fprotocol,lportl,lportu,rportl,rportu) values (%d,%d,%d,%d,'%s',%f,'%s','%s','%s','%s','%s','%s',%d,%d,%d,%d,%d)",lCount,ruleflag,ruleaction,qosid,rulename,temprio,lips,lipe,lips,ripe,finputdev,foutputdev,fprotocol,lportl,lportu,rportl,rportu);
	 printf("%s \n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 sprintf(ruleid,"%d",lCount);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,ruleid);
   ltMsgFree(ltMsgPk);
   return 0;
}
//防火墙规则列表
int msaFWruleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		sprintf(sqlStr,"delete from nasfirewall where ruleid=%s",ruleid);
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
	 sprintf(sqlStr,"%s","select count(*) from nasfirewall");
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
	 sprintf(sqlStr,"%s","select ruleid,rulename,ruleflag,ruleaction,rulerate,qosid from nasfirewall order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s,\"qosname\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],_ltPubInfo->_QosList[atoi(tempRow[5])].qosname);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s,\"qosname\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],_ltPubInfo->_QosList[atoi(tempRow[5])].qosname);
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
//防火墙规则修改
int editPWpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
		  char *dbUser;
		  char *dbPass;
		  char *dbName;
		  dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon!=NULL){
				//printf("db connect ok\n");
			}else{
				fprintf(stderr,"db connect error\n");
			}
			 char sqlBuf[128],tempin[64],output[64],sel[16];
			 int myratelimit,fprotocol;
			 char str[16];
		 	 char caTempDir[128];
			 int fd,match;
			 buffer ss;
			 char *pppp;
			 myratelimit=-1;
			 ltDbHeadPtr dbPtr;
			 ltTablePtr  tablePtr;
		   dbPtr=lt_dbinit();
		   lt_db_htmlpage(dbPtr,"utf-8");
		   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
		   lt_dbput_rootvars(dbPtr,1,"type","type:'update',");
		   LT_DBROW tempRow;
			 ltDbCursor *tempCursor;
			 ltTablePtr tablePtr1;
			 ltTablePtr tablePtr2;
			 tablePtr1=lt_dbput_table(dbPtr,"inputdev");
			 tablePtr2=lt_dbput_table(dbPtr,"outputdev");
			 memset(str,0,sizeof(str));
			 memset(sqlBuf,0,sizeof(sqlBuf));
			 memset(tempin,0,sizeof(tempin));
			 memset(output,0,sizeof(output));
			 sprintf(sqlBuf,"select * from nasfirewall where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow){
							memset(str,0,sizeof(str));
							sprintf(str,"ruleflag%s",tempRow[2]);
							lt_dbput_rootvars(dbPtr,1,str,"selected");
							memset(str,0,sizeof(str));
							sprintf(str,"ruleaction%s",tempRow[3]);
							lt_dbput_rootvars(dbPtr,1,str,"selected");
							lt_dbput_rootvars(dbPtr,9,
								"rulename",tempRow[1],
								"lips",tempRow[9],
								"lipe",tempRow[10],
								"lportl",tempRow[11],
								"lportu",tempRow[12],
								"rips",tempRow[13],
								"ripe",tempRow[14],
								"rportl",tempRow[15],
								"rportu",tempRow[16]
							);
							myratelimit=atoi(tempRow[4]);
							fprotocol=atoi(tempRow[8]);
							sprintf(tempin,"%s",tempRow[6]);
							sprintf(output,"%s",tempRow[7]);
					}
					ltDbCloseCursor(tempCursor);
			 }
			char qossel[32];
			tablePtr=lt_dbput_table(dbPtr,"qoslist");
			memset(sqlBuf,0,sizeof(sqlBuf));
			sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
						tempRow=ltDbFetchRow(tempCursor);
						while(tempRow!=NULL){
								memset(qossel,0,sizeof(qossel));
								if(atoi(tempRow[0])==myratelimit){
										sprintf(qossel,"%s","selected");
								}
								lt_dbput_recordvars(tablePtr,3,
								  "qosid",LT_TYPE_STRING,tempRow[0],
									"sel",LT_TYPE_STRING,qossel,
									"qosname",LT_TYPE_STRING,tempRow[1]);
							    tempRow=ltDbFetchRow(tempCursor);
						}
						ltDbCloseCursor(tempCursor);
			}
		 ltDbClose(G_DbCon);
		 sprintf(caTempDir,"%s","/datacenter/tmp/tmpdevinfo");
		 if(ltFileIsExist(caTempDir)!=1) {
	      return 0;
	   }
	   fd = open_read(caTempDir);
	   if (fd == -1)
	   {
	      return 0;
	   }
	   buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	   
	   for(;;){
				 if (getln(&ss,&line,&match,'\n') == -1) {
				 		  break;
				 }
				 if (!match && !line.len) {
				 	   break;
				 }
				 striptrailingwhitespace(&line);
				 if (!stralloc_0(&line)) {
				 		  break;
				 }
				 sprintf(str,"%s",line.s);
				 pppp=strchr(str,'e');
				 if(!pppp){
				 	  continue;
				 }
				 pppp--;
				 *pppp='\0';
				 pppp++;
			 	 sprintf(str,"%s",pppp);
				 pppp=strchr(str,':');
				 if(!pppp){
				 	  continue;
				 }
				 *pppp='\0';
				 if(str[0]=='e'&&str[1]=='t'&&str[2]=='h'){
				 			printf("tempin:%s\n",tempin);
				 			printf("output:%s\n",output);
				 		memset(sel,0,sizeof(sel));
				 		if(strcmp(tempin,str)==0){
				 				sprintf(sel,"%s","selected");
				 			printf("sel:%s\n",sel);
				 		}
				 		lt_dbput_recordvars(tablePtr1,2,
				 				"sel",LT_TYPE_STRING,sel,
				 				"name",LT_TYPE_STRING,str
				 		);
				 		memset(sel,0,sizeof(sel));
				 		if(strcmp(output,str)==0){
				 				sprintf(sel,"%s","selected");
				 			printf("sel:%s\n",sel);
				 		}
				 		lt_dbput_recordvars(tablePtr2,2,
				 				"sel",LT_TYPE_STRING,sel,
				 				"name",LT_TYPE_STRING,str
				 		);
				 }
		 }
		 close(fd);
		 ltMsgPk->msgpktype=1;
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/policycenter/addRule.htm",dbPtr->head,0);
	   ltMsgFree(ltMsgPk);
	   lt_dbfree(dbPtr);
	   return 0;
}
//防火墙规则应用
int appFireWallRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char caLabel[128],sqlBuf[128];
		int head;
		head=0;
    LT_DBROW tempRow;
	  ltDbCursor *tempCursor;
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select finputdev,foutputdev,fprotocol,lips,lipe,lportl,lportu,rips,ripe,rportl,rportu,ruleaction,qosid from nasfirewall");
		printf("%s \n",sqlBuf);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							if(head==0){
								sprintf(caLabel,"%s","echo 'c1' > /tmp/baochuang/firewall");
						    system(caLabel);
						    sprintf(caLabel,"%s","echo '1' >> /tmp/baochuang/firewall");
						    system(caLabel);
						    sprintf(caLabel,"echo '%s|%s|' >> /tmp/baochuang/firewall",tempRow[0],tempRow[1]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%02ld' >> /tmp/baochuang/firewall",atol(tempRow[2]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%s%s' >> /tmp/baochuang/firewall",tempRow[3],tempRow[4]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld%05ld' >> /tmp/baochuang/firewall",atol(tempRow[5]),atol(tempRow[6]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%s%s' >> /tmp/baochuang/firewall",tempRow[7],tempRow[8]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld%05ld' >> /tmp/baochuang/firewall",atol(tempRow[9]),atol(tempRow[10]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%02ld' >> /tmp/baochuang/firewall",atol(tempRow[11]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld' >> /tmp/baochuang/firewall",atol(tempRow[12]));
						    system(caLabel);
								tempRow=ltDbFetchRow(tempCursor);
								head=1;
							}else{
						    sprintf(caLabel,"%s","echo '1' >> /tmp/baochuang/firewall");
						    system(caLabel);
						    sprintf(caLabel,"echo '%s|%s|' >> /tmp/baochuang/firewall",tempRow[0],tempRow[1]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%02ld' >> /tmp/baochuang/firewall",atol(tempRow[2]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%s%s' >> /tmp/baochuang/firewall",tempRow[3],tempRow[4]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld%05ld' >> /tmp/baochuang/firewall",atol(tempRow[5]),atol(tempRow[6]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%s%s' >> /tmp/baochuang/firewall",tempRow[7],tempRow[8]);
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld%05ld' >> /tmp/baochuang/firewall",atol(tempRow[9]),atol(tempRow[10]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%02ld' >> /tmp/baochuang/firewall",atol(tempRow[11]));
						    system(caLabel);
						    sprintf(caLabel,"echo '%05ld' >> /tmp/baochuang/firewall",atol(tempRow[12]));
						    system(caLabel);
								tempRow=ltDbFetchRow(tempCursor);
							}
					}
					ltDbCloseCursor(tempCursor);
		}
	  ltDbClose(G_DbCon);
		
		
		
		
    
    
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,0);
    ltMsgFree(ltMsgPk);
    return 0;
}

