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

typedef struct srvruleInfo_s {
	  char     strSrvRuleName[64];
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
    unsigned long long   dayflowu;//上行流量
    char     strdayflowu[32];
    unsigned long long   dayflowd;//下行流量
    char     strdayflowd[32];
    char     lasttime[32];
    
}srvruleInfo;


int msaNowSrvruleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    srvruleInfo  _sruleListInfo[_MAX_SRVRULE_NUM];
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
        
	 memset((void *)_sruleListInfo,0,sizeof(srvruleInfo)*_MAX_SRVRULE_NUM);
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
	 for(j=0;j<_MAX_SRVRULE_NUM;j++){ 	 	 	       
            allsrvflow=0;
            //当前总流量
            allsrvflow=_ltPubInfo->_Srvrulelist[j].nowFlowInfo.allflow[0]+_ltPubInfo->_Srvrulelist[j].nowFlowInfo.allflow[1];
            if(allsrvflow>0){
                allcount++;
            }else{
                continue;
            }
	 	 	      
	 	 	      //服务策略名称 
	 	 	      sprintf(_sruleListInfo[j].strSrvRuleName,"%s",_ltPubInfo->_Srvrulelist[j].rulename);
		    		//当前某服务策略的流速
		    		_sruleListInfo[j].flowbps=(allsrvflow*8)/mysec; 		
		    		//当前流速转换为字符类型	    		
		    		if(_sruleListInfo[j].flowbps>1024000){
		    			 sprintf(_sruleListInfo[j].strflowbps,"%0.3fM",_sruleListInfo[j].flowbps/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strflowbps,"%0.3fK",_sruleListInfo[j].flowbps/1024.00);
		    		}
		    		//总流速
		    		allflow=allflow+_sruleListInfo[j].flowbps;		    			
		    		//当天流量访问时间	    	
	 	 	       _sruleListInfo[j].daytime=_ltPubInfo->_Srvrulelist[j].dayFlowInfo.alltime;
		    		//时间转为字符类型		    		
		    		 sprintf(_sruleListInfo[j].strdaytime,"%s",time2str(_sruleListInfo[j].daytime));
		    		 //当天总流量		    		
	 	 	      _sruleListInfo[j].dayflow=_ltPubInfo->_Srvrulelist[j].dayFlowInfo.allflow[0]+_ltPubInfo->_Srvrulelist[j].dayFlowInfo.allflow[1];
	 	 	       //总流量转为字符类型		    	   
		    		if(_sruleListInfo[j].dayflow>1024000){
		    			 sprintf(_sruleListInfo[j].strdayflow,"%0.3fM",_sruleListInfo[j].dayflow/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strdayflow,"%0.3fK",_sruleListInfo[j].dayflow/1024.00);
		    		}
		    		//当天上行流量
		    		_sruleListInfo[j].dayflowu=_ltPubInfo->_Srvrulelist[j].dayFlowInfo.allflow[0];
		    		//上行流量转为字符类型
		    		if(_sruleListInfo[j].dayflowu>1024000){
		    			 sprintf(_sruleListInfo[j].strdayflowu,"%0.3fM",_sruleListInfo[j].dayflowu/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strdayflowu,"%0.3fK",_sruleListInfo[j].dayflowu/1024.00);
		    		}
		    		//当天下行流量
		    		_sruleListInfo[j].dayflowd=_ltPubInfo->_Srvrulelist[j].dayFlowInfo.allflow[1];
		    		//下行流量转为字符类型
		    		if(_sruleListInfo[j].dayflowd>1024000){
		    			 sprintf(_sruleListInfo[j].strdayflowd,"%0.3fM",_sruleListInfo[j].dayflowd/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strdayflowd,"%0.3fK",_sruleListInfo[j].dayflowd/1024.00);
		    		}
		    		
		    		//最后访问时间
		    		 nasTimeFormat(nowTime,_ltPubInfo->_Srvrulelist[j].dayFlowInfo.alllasttime);
		    		 sprintf(_sruleListInfo[j].lasttime,"%s",nowTime);
		    		
   
      }
    
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   for(j=0;j<_MAX_SRVRULE_NUM;j++){
   	if(_sruleListInfo[j].flowbps>0 ){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"srvrulename\":\"%s\",\"flowbps\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_sruleListInfo[j].strSrvRuleName, 
	     	  	 	_sruleListInfo[j].flowbps,
	     	  	 	_sruleListInfo[j].strflowbps,
	     	  	 	(((float)_sruleListInfo[j].flowbps)*100)/allflow,//占比
	     	  	 	_sruleListInfo[j].daytime,
	     	  	 	_sruleListInfo[j].strdaytime,
	     	  	 	_sruleListInfo[j].dayflow,
	     	  	 	_sruleListInfo[j].strdayflow,
	     	  	 	_sruleListInfo[j].dayflowu,//上流量                      
              _sruleListInfo[j].strdayflowu,
              _sruleListInfo[j].dayflowd,                      
              _sruleListInfo[j].strdayflowd,//下流量
              _sruleListInfo[j].lasttime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"srvrulename\":\"%s\",\"flowbps\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_sruleListInfo[j].strSrvRuleName, 
	     	  	 	_sruleListInfo[j].flowbps,
	     	  	 	_sruleListInfo[j].strflowbps,
	     	  	 	(((float)_sruleListInfo[j].flowbps)*100)/allflow,//占比
	     	  	 	_sruleListInfo[j].daytime,
	     	  	 	_sruleListInfo[j].strdaytime,
	     	  	 	_sruleListInfo[j].dayflow,
	     	  	 	_sruleListInfo[j].strdayflow,
	     	  	 	_sruleListInfo[j].dayflowu,//上流量                      
              _sruleListInfo[j].strdayflowu,
              _sruleListInfo[j].dayflowd,                      
              _sruleListInfo[j].strdayflowd,//下流量
              _sruleListInfo[j].lasttime
              );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
	  }
   }    	
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
	 sprintf(caTemp,"%s/data/rule.json",_msahtmldir);
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


int msaToSrvRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    srvruleInfo  _sruleListInfo[MAX_TOP_SRVNUM];
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
    
    char *ruleid;
    int rid;
    ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
    rid=atol(ruleid);
    printf("rid:%d",rid);
   
	  memset((void *)_sruleListInfo,0,sizeof(srvruleInfo)*MAX_TOP_SRVNUM);
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
	 for(j=0;j<MAX_TOP_SRVNUM;j++){
			 	    allsrvflow=0;					
					  allsrvflow=_ltPubInfo->_Srvrulelist[rid].nowFlowInfo.srvflow[j];
			      if(allsrvflow>0  ){
			     		allcount++;
			     	}else{
			     	  continue;
			     	}
	  	          
            //当前某服务策略速率
		    		 _sruleListInfo[j].flowbps=(allsrvflow*8)/mysec; 		
		    	  //速率转为字符类型 		
		    		if(_sruleListInfo[j].flowbps>1024000){
		    			 sprintf(_sruleListInfo[j].strflowbps,"%0.3fM",_sruleListInfo[j].flowbps/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strflowbps,"%0.3fK",_sruleListInfo[j].flowbps/1024.00);
		    		}
		    		//总流速
		    		allflow=allflow+_sruleListInfo[j].flowbps;
		    			
		    		//该策略通道当天访问总时间    	
	 	 	      _sruleListInfo[j].daytime=_ltPubInfo->_Srvrulelist[rid].dayFlowInfo.alltime;		    				    		
		    		sprintf(_sruleListInfo[j].strdaytime,"%s",time2str(_sruleListInfo[j].daytime));
		    		//该策略通道最后访问时间
		    		nasTimeFormat(nowTime,_ltPubInfo->_Srvrulelist[rid].dayFlowInfo.alllasttime);
            sprintf(_sruleListInfo[j].lasttime,"%s",nowTime);
	 	 	      
	 	 	      //该策略通道当天总流量  
	 	 	      _sruleListInfo[j].dayflow=_ltPubInfo->_Srvrulelist[rid].dayFlowInfo.allflow[0]+_ltPubInfo->_Srvrulelist[rid].dayFlowInfo.allflow[1];
		    	   
		    		if(_sruleListInfo[j].dayflow>1024000){
		    			 sprintf(_sruleListInfo[j].strdayflow,"%0.3fM",_sruleListInfo[j].dayflow/1024000.00);
		    		}else{
		    			 sprintf(_sruleListInfo[j].strdayflow,"%0.3fK",_sruleListInfo[j].dayflow/1024.00);
		    		}
		    		
   
      }
      
   
   

   
    
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
  

   
   for(j=0;j<MAX_TOP_SRVNUM;j++){
   	if(_sruleListInfo[j].flowbps>0 ){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"srvname\":\"%s\",\"rflowu\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname, 
	     	  	 	_sruleListInfo[j].flowbps,
	     	  	 	_sruleListInfo[j].strflowbps,
	     	  	 	(((float)_sruleListInfo[j].flowbps)*100)/allflow,
	     	  	 	_sruleListInfo[j].daytime,
	     	  	 	_sruleListInfo[j].strdaytime,
	     	  	 	_sruleListInfo[j].dayflow,
	     	  	 	_sruleListInfo[j].strdayflow,
	     	  	 	_sruleListInfo[j].lasttime
	     	  	 	);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"srvname\":\"%s\",\"rflowu\":\"%llu\",\"strflowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"lasttime\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname, 
	     	  	 	_sruleListInfo[j].flowbps,
	     	  	 	_sruleListInfo[j].strflowbps,
	     	  	 	(((float)_sruleListInfo[j].flowbps)*100)/allflow,
	     	  	 	_sruleListInfo[j].daytime,
	     	  	 	_sruleListInfo[j].strdaytime,
	     	  	 	_sruleListInfo[j].dayflow,
	     	  	 	_sruleListInfo[j].strdayflow,
	     	  	 	_sruleListInfo[j].lasttime
	     	  	 	);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
	  }
   }    	
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
 printf("torulejson: %s\n",strTemp.s);
	 sprintf(caTemp,"%s/data/torule.json",_msahtmldir);
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


//策略通道相关服务
int msaRuleHtml(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
  int  ruleid;
  ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
  if(ruleid<0 || ruleid>=_MAX_SRVRULE_NUM){
     ruleid=0;
  }

  ltDbHeadPtr dbPtr;
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
  lt_dbput_rootvars(dbPtr,1,"text",_ltPubInfo->_Srvrulelist[ruleid].rulename);
 
  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/nowview/srvruletd1.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);

  return 0;
  
}
