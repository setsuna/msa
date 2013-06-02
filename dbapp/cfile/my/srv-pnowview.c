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
int msaloopgrouptree(stralloc *strTemp,int level,int pid);
//虚拟通道
typedef struct pathFlowInfo_s {
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
    char     strupflow[32];
    char     strdownflow[32];
    char     alllasttime[32];
}pathFlowInfo;
//部门服务
typedef struct groupDetailFlowInfo_s{
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
    char     alllasttime[32];
}groupDetailFlowInfo;
//虚拟通道服务
typedef struct pathDetailFlowInfo_s{
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
    char     alllasttime[32];
}pathDetailFlowInfo;
//虚拟通道
int msaNowPathList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    pathFlowInfo  _pathFlowInfo[_MAXVPATHNUM];
    unsigned long long lMaxBytes;
    int      j,allcount,k;
    char caTmpp[2048];
    stralloc strTemp;
    unsigned long long allflow,allsrvflow;
    int      iFd;
    unsigned int lTime;
    int      mysec;
    char     caTemp[256];
    char     nowTime[32];
    
    memset((void *)_pathFlowInfo,0,sizeof(pathFlowInfo)*_MAXVPATHNUM);
    lMaxBytes=0;

	  lTime=time(0);
    mysec=lTime%60;
    if(mysec<2 || mysec>58 ){
    	sleep(4);
    }
	  lTime=time(0);
    mysec=lTime%60;
	  allcount=0;
	  allflow=0;
	  j=0;
	  
	  
   memset(nowTime,0,sizeof(nowTime));
   //加一个锁    
	 for(j=0;j<_MAXVPATHNUM;j++){
      allsrvflow=0; 
			allsrvflow=_ltPubInfo->_VPathList[j].nowFlowInfo.allflow[0]+_ltPubInfo->_VPathList[j].nowFlowInfo.allflow[1];
			if(allsrvflow>0)
		  {
				allcount++;
			}else{
				continue;
			}
			_pathFlowInfo[j].flowbps=(allsrvflow*8)/mysec;

			if(_pathFlowInfo[j].flowbps>1024000){
				 sprintf(_pathFlowInfo[j].strflowbps,"%0.3fM",_pathFlowInfo[j].flowbps/1024000.00);
			}else{
				 sprintf(_pathFlowInfo[j].strflowbps,"%0.3fK",_pathFlowInfo[j].flowbps/1024.00);
			}
			allflow=allflow+_pathFlowInfo[j].flowbps;
			_pathFlowInfo[j].daytime=_ltPubInfo->_VPathList[j].dayFlowInfo.alltime;
			sprintf(_pathFlowInfo[j].strdaytime,"%s",time2str(_pathFlowInfo[j].daytime));
			
			_pathFlowInfo[j].dayflow=_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[0]+_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[1];
			if(_pathFlowInfo[j].dayflow>1024000){
				 sprintf(_pathFlowInfo[j].strdayflow,"%0.3fM",_pathFlowInfo[j].dayflow/1024000.00);
			}else{
				 sprintf(_pathFlowInfo[j].strdayflow,"%0.3fK",_pathFlowInfo[j].dayflow/1024.00);
			}		
  		if(_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[0]>1024000){
  			 sprintf(_pathFlowInfo[j].strupflow,"%0.3fM",_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[0]/1024000.00);
  		}else{
  			 sprintf(_pathFlowInfo[j].strupflow,"%0.3fK",_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[0]/1024.00);
  		}
  		if(_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[1]>1024000){
  			 sprintf(_pathFlowInfo[j].strdownflow,"%0.3fM",_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[1]/1024000.00);
  		}else{
  			 sprintf(_pathFlowInfo[j].strdownflow,"%0.3fK",_ltPubInfo->_VPathList[j].dayFlowInfo.allflow[1]/1024.00);
  		}
  		
      nasTimeFormat(nowTime,_ltPubInfo->_VPathList[j].dayFlowInfo.alllasttime);
  		sprintf(_pathFlowInfo[j].alllasttime,"%s",nowTime);
  		//sprintf(_pathFlowInfo[j].alllasttime,"%s",time2str(_ltPubInfo->_VPathList[j].dayFlowInfo.alllasttime));   		
   }
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);//6
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   for(j=0;j<_MAXVPATHNUM;j++){
     if(_pathFlowInfo[j].flowbps>0){
      if(k==0){
			  sprintf(caTmpp,"{\"pname\":\"%s\",\"srvflowu\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%ld\",\"strdaytime\":\"%s\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"upflow\":\"%s\",\"downflow\":\"%s\",\"lasttime\":\"%s\"}",	   
				_ltPubInfo->_VPathList[j].vPathName, //虚拟通道名称
				_pathFlowInfo[j].flowbps,
				_pathFlowInfo[j].strflowbps,//速率bps
				(((float)_pathFlowInfo[j].flowbps)*100)/allflow,//占比
	     	_pathFlowInfo[j].daytime,
				_pathFlowInfo[j].strdaytime,//当天总访问时间
				_pathFlowInfo[j].dayflow,
				_pathFlowInfo[j].strdayflow,//当天通道总流量
 	  	 	_pathFlowInfo[j].strupflow,//上行流量
 	  	 	_pathFlowInfo[j].strdownflow,//下行流量
 	  	 	_pathFlowInfo[j].alllasttime);//最后访问时间
			  stralloc_cats(&strTemp,caTmpp);
	    }else{
 	  	  sprintf(caTmpp,",{\"pname\":\"%s\",\"srvflowu\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%ld\",\"strdaytime\":\"%s\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"upflow\":\"%s\",\"downflow\":\"%s\",\"lasttime\":\"%s\"}",
 	  	  _ltPubInfo->_VPathList[j].vPathName, //虚拟通道名称
 	  	  _pathFlowInfo[j].flowbps,
 	  	  _pathFlowInfo[j].strflowbps,
	 		  (((float)_pathFlowInfo[j].flowbps)*100)/allflow,
	     	_pathFlowInfo[j].daytime,
	 		  _pathFlowInfo[j].strdaytime,
	 		  _pathFlowInfo[j].dayflow,
   	    _pathFlowInfo[j].strdayflow,
 	  	 	_pathFlowInfo[j].strupflow,
 	  	 	_pathFlowInfo[j].strdownflow,
 	  	 	_pathFlowInfo[j].alllasttime);
 	      stralloc_cats(&strTemp,caTmpp);
 	    }
	    k++;
	  }
   }

     stralloc_cats(&strTemp,"]}");
     stralloc_0(&strTemp);
 
	 sprintf(caTemp,"%s/data/path.json",_msahtmldir);	
	 iFd = open(caTemp,O_WRONLY | O_CREAT | O_TRUNC);
	 if(iFd >0 ) {
		      write(iFd, strTemp.s, strTemp.len );	
		      close(iFd);
	 }
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
  return 0;
  
}
//虚拟通道服务模板显示
int msaPathDetail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char *pathid;
	pathid=ltMsgGetVar_s(ltMsgPk,"pathid");
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"pathid",pathid);
  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/nowview/xunitd.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
//通道服务详细情况
int msaPathDetailList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  pathDetailFlowInfo  _pathDetailFlowInfo[MAX_TOP_SRVNUM];
    int      j,allcount,k;
    char 		 caTmpp[2048];
    stralloc strTemp;
    unsigned long long allflow,allsrvflow;
    int      iFd;
    unsigned int lTime;
    int      mysec;
    char     caTemp[256];
    char     nowTime[32];
    int     pathid;
	  pathid=atoi(ltMsgGetVar_s(ltMsgPk,"pathid"));
	  memset((void *)_pathDetailFlowInfo,0,sizeof(pathDetailFlowInfo)*MAX_TOP_SRVNUM);
	  
	  lTime=time(0);
    mysec=lTime%60;
    if(mysec<2 || mysec>58 ){
    	sleep(4);
    }
    lTime=time(0);
    mysec=lTime%60;
    allcount=0;
    allflow=0;
    memset(nowTime,0,sizeof(nowTime));
	  //加一个锁
	  for(j=0;j<MAX_TOP_SRVNUM;j++){
				allsrvflow=0;
				  allsrvflow+=_ltPubInfo->_VPathList[pathid].nowFlowInfo.srvflow[j];
	      if(allsrvflow>0){
	     		allcount++;
	     	}else{
	     	  continue;
	     	}
    		_pathDetailFlowInfo[j].flowbps=(allsrvflow*8)/mysec;
    		if(_pathDetailFlowInfo[j].flowbps>1024000){
    			 sprintf(_pathDetailFlowInfo[j].strflowbps,"%0.3fM",_pathDetailFlowInfo[j].flowbps/1024000.00);
    		}else{
    			 sprintf(_pathDetailFlowInfo[j].strflowbps,"%0.3fK",_pathDetailFlowInfo[j].flowbps/1024.00);
    		}
    		allflow=allflow+_pathDetailFlowInfo[j].flowbps;//////j  一个通道的服务流量累加

    		_pathDetailFlowInfo[j].daytime=_ltPubInfo->_VPathList[pathid].dayFlowInfo.srvtime[j];
    		sprintf(_pathDetailFlowInfo[j].strdaytime,"%s",time2str(_pathDetailFlowInfo[j].daytime));
    		_pathDetailFlowInfo[j].dayflow=_ltPubInfo->_VPathList[pathid].dayFlowInfo.srvflow[j];
    		if(_pathDetailFlowInfo[j].dayflow>1024000){
    			 sprintf(_pathDetailFlowInfo[j].strdayflow,"%0.3fM",_pathDetailFlowInfo[j].dayflow/1024000.00);
    		}else{
    			 sprintf(_pathDetailFlowInfo[j].strdayflow,"%0.3fK",_pathDetailFlowInfo[j].dayflow/1024.00);
    		}
        nasTimeFormat(nowTime,_ltPubInfo->_VPathList[j].dayFlowInfo.alllasttime);
    		sprintf(_pathDetailFlowInfo[j].alllasttime,"%s",nowTime);
	 }
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;	
				
   for(j=0;j<MAX_TOP_SRVNUM;j++){
   	if(_pathDetailFlowInfo[j].flowbps>0){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"srvname\":\"%s\",\"srvflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"strdaytime\":\"%s\",\"daytime\":\"%ld\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname,//服务名称
	     	  	 	_pathDetailFlowInfo[j].flowbps,
	     	  	 	_pathDetailFlowInfo[j].strflowbps,
	     	  	 	(((float)_pathDetailFlowInfo[j].flowbps)*100)/allflow,
	     	  	 	_pathDetailFlowInfo[j].strdaytime,
	     	  	 	_pathDetailFlowInfo[j].daytime,
	     	  	 	_pathDetailFlowInfo[j].dayflow,
	     	  	 	_pathDetailFlowInfo[j].strdayflow,
			 	  	 	_pathDetailFlowInfo[j].alllasttime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"srvname\":\"%s\",\"srvflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"strdaytime\":\"%s\",\"daytime\":\"%ld\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname,//服务名称
	     	  	 	_pathDetailFlowInfo[j].flowbps,
	     	  	 	_pathDetailFlowInfo[j].strflowbps,
	     	  	 	(((float)_pathDetailFlowInfo[j].flowbps)*100)/allflow,
	     	  	 	_pathDetailFlowInfo[j].strdaytime,
	     	  	 	_pathDetailFlowInfo[j].daytime,
	     	  	 	_pathDetailFlowInfo[j].dayflow,
	     	  	 	_pathDetailFlowInfo[j].strdayflow,
			 	  	 	_pathDetailFlowInfo[j].alllasttime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
	  }
   }
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 sprintf(caTemp,"%s/data/pathdetail.json",_msahtmldir);

	 iFd = open(caTemp,O_WRONLY | O_CREAT | O_TRUNC);
	 if(iFd >0 ) {
		      write(iFd, strTemp.s, strTemp.len );
		      close(iFd);
	 }
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//部门主线树
int msaloopgrouptree(stralloc *strTemp,int level,int pid){
	char tmpstr[512]; 
	int iHead;
	int j;
	iHead=1;
	level++;
 
	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				sprintf(tmpstr,"%s",",children: [");
			  stralloc_cats(strTemp,tmpstr);
				sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:'%s'",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name,"false");
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:'%s'",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name,"false");
		  }			
			stralloc_cats(strTemp,tmpstr);
			pid=_ltPubInfo->_GroupList[j].groupid;
			
			if(level<5){
				msaloopgrouptree(strTemp,level,pid);
			}
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
int msaGroupTreeView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char tmpstr[512];
	int level,pid;
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));

	pid=0;
	level=0;
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
	
 	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',checked:'%s'",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name,"false");
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',checked:'%s'",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name,"false");
		  }
		  stralloc_cats(&strTemp,tmpstr);	
			pid=_ltPubInfo->_GroupList[j].groupid;	
			if(level<5){
				msaloopgrouptree(&strTemp,level,pid);
			}
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
//部门服务模板显示
int msaGroupDetail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char *groupid;
	groupid=ltMsgGetVar_s(ltMsgPk,"groupid");
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"groupid",groupid);
  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/nowview/syssrvlistgroup.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
//部门服务详细情况
int msaGroupDetailList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  groupDetailFlowInfo  _groupDetailFlowInfo[MAX_TOP_SRVNUM];
    int      j,allcount,k;
    char 		 caTmpp[2048];
    stralloc strTemp;
    unsigned long long allflow,allsrvflow;
    int      iFd;
    unsigned int lTime;
    int      mysec;
    char     caTemp[256];
    char     nowTime[32];
    int     groupid;
	  groupid=atoi(ltMsgGetVar_s(ltMsgPk,"groupid"));
	  memset((void *)_groupDetailFlowInfo,0,sizeof(groupDetailFlowInfo)*MAX_TOP_SRVNUM);
	  
	  lTime=time(0);
    mysec=lTime%60;
    if(mysec<2 || mysec>58 ){
    	sleep(4);
    }
    lTime=time(0);
    mysec=lTime%60;
    allcount=0;
    allflow=0;
    
    memset(nowTime,0,sizeof(nowTime));
    //加一个锁
	  for(j=0;j<MAX_TOP_SRVNUM;j++){
				allsrvflow=0;
				  allsrvflow+=_ltPubInfo->_GroupList[groupid].nowFlowInfo.srvflow[j];
	      if(allsrvflow>0){
	     		allcount++;
	     	}else{
	     	  continue;
	     	}
    		_groupDetailFlowInfo[j].flowbps=(allsrvflow*8)/mysec;
    		if(_groupDetailFlowInfo[j].flowbps>1024000){
    			 sprintf(_groupDetailFlowInfo[j].strflowbps,"%0.3fM",_groupDetailFlowInfo[j].flowbps/1024000.00);
    		}else{
    			 sprintf(_groupDetailFlowInfo[j].strflowbps,"%0.3fK",_groupDetailFlowInfo[j].flowbps/1024.00);
    		}
    		allflow=allflow+_groupDetailFlowInfo[j].flowbps;//////j  一个部门的服务流量累加

    		_groupDetailFlowInfo[j].daytime=_ltPubInfo->_GroupList[groupid].dayFlowInfo.srvtime[j];
    		sprintf(_groupDetailFlowInfo[j].strdaytime,"%s",time2str(_groupDetailFlowInfo[j].daytime));
    		_groupDetailFlowInfo[j].dayflow=_ltPubInfo->_GroupList[groupid].dayFlowInfo.srvflow[j];
    		if(_groupDetailFlowInfo[j].dayflow>1024000){
    			 sprintf(_groupDetailFlowInfo[j].strdayflow,"%0.3fM",_groupDetailFlowInfo[j].dayflow/1024000.00);
    		}else{
    			 sprintf(_groupDetailFlowInfo[j].strdayflow,"%0.3fK",_groupDetailFlowInfo[j].dayflow/1024.00);
    		}
        nasTimeFormat(nowTime,_ltPubInfo->_GroupList[j].dayFlowInfo.alllasttime);
    		sprintf(_groupDetailFlowInfo[j].alllasttime,"%s",nowTime);
	 }
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;	
				
   for(j=0;j<MAX_TOP_SRVNUM;j++){
   	if(_groupDetailFlowInfo[j].flowbps>0){//_groupDetailFlowInfo[groupid].flowbps>0
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"srvname\":\"%s\",\"srvflowu\":\"%s\",\"flowbps\":\"%llu\",\"flowp\":\"%0.3f\",\"strdaytime\":\"%s\",\"daytime\":\"%ld\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname,//服务名称
	     	  	 	_groupDetailFlowInfo[j].strflowbps,
	     	  	 	_groupDetailFlowInfo[j].flowbps,
	     	  	 	(((float)_groupDetailFlowInfo[j].flowbps)*100)/allflow,
	     	  	 	_groupDetailFlowInfo[j].strdaytime,
	     	  	 	_groupDetailFlowInfo[j].daytime,
	     	  	 	_groupDetailFlowInfo[j].dayflow,
	     	  	 	_groupDetailFlowInfo[j].strdayflow,
			 	  	 	_groupDetailFlowInfo[j].alllasttime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"srvname\":\"%s\",\"srvflowu\":\"%s\",\"flowbps\":\"%llu\",\"flowp\":\"%0.3f\",\"strdaytime\":\"%s\",\"daytime\":\"%ld\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname,//服务名称
	     	  	 	_groupDetailFlowInfo[j].strflowbps,
	     	  	 	_groupDetailFlowInfo[j].flowbps,
	     	  	 	(((float)_groupDetailFlowInfo[j].flowbps)*100)/allflow,
	     	  	 	_groupDetailFlowInfo[j].strdaytime,
	     	  	 	_groupDetailFlowInfo[j].daytime,
	     	  	 	_groupDetailFlowInfo[j].dayflow,
	     	  	 	_groupDetailFlowInfo[j].strdayflow,
			 	  	 	_groupDetailFlowInfo[j].alllasttime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
	  }
   }
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 sprintf(caTemp,"%s/data/groupdetail.json",_msahtmldir);

	 iFd = open(caTemp,O_WRONLY | O_CREAT | O_TRUNC);
	 if(iFd >0 ) {
		      write(iFd, strTemp.s, strTemp.len );
		      close(iFd);
	 }
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
