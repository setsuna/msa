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

typedef struct xFlowInfo_s {
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long hourtime; 
    char     strhourtime[32];
    unsigned long long   hourflow; 
    char     strhourflow[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
}xFlowInfo;


int msaNowSrvList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    
    xFlowInfo  _xFlowInfo[MAX_TOP_SRVNUM];;
    unsigned long long lMaxBytes;
    int      j,allcount,k;
    char caTmpp[2048];
    stralloc strTemp;
    unsigned long long allflow;
    int      iFd;
    unsigned int lTime;
    int      mysec;
    char     caTemp[256];
        
	 memset((void *)_xFlowInfo,0,sizeof(xFlowInfo)*MAX_TOP_SRVNUM);
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
    
//¼ÓÒ»¸öËø    
	 for(j=0;j<MAX_TOP_SRVNUM;j++){
	  	
	  	      if(_ltPubInfo->_AllInfo.nowFlowInfo.srvflow[j]>0  ){
			     		allcount++;
			     	}else{
			     	  continue;	
			     	}
		    		_xFlowInfo[j].flowbps=(_ltPubInfo->_AllInfo.nowFlowInfo.srvflow[j]*8)/mysec;
		    		if(_xFlowInfo[j].flowbps>1024000){
		    			 sprintf(_xFlowInfo[j].strflowbps,"%0.2fM",_xFlowInfo[j].flowbps/1024000.00);
		    		}else{
		    			 sprintf(_xFlowInfo[j].strflowbps,"%0.2fK",_xFlowInfo[j].flowbps/1024.00);
		    		}
		    		allflow=allflow+_xFlowInfo[j].flowbps;
		    		
		    		_xFlowInfo[j].hourtime=_ltPubInfo->_AllInfo.hourFlowInfo.srvtime[j];
		    		sprintf(_xFlowInfo[j].strhourtime,"%s",time2str(_xFlowInfo[j].hourtime));
		    		_xFlowInfo[j].hourflow=_ltPubInfo->_AllInfo.hourFlowInfo.srvflow[j];
		    		if(_xFlowInfo[j].hourflow>1024000){
		    			 sprintf(_xFlowInfo[j].strhourflow,"%0.2fM",_xFlowInfo[j].hourflow/1024000.00);
		    		}else{
		    			 sprintf(_xFlowInfo[j].strhourflow,"%0.2fK",_xFlowInfo[j].hourflow/1024.00);
		    		}
		    		
		    		_xFlowInfo[j].daytime=_ltPubInfo->_AllInfo.dayFlowInfo.srvtime[j];
		    		sprintf(_xFlowInfo[j].strdaytime,"%s",time2str(_xFlowInfo[j].daytime));
		    		_xFlowInfo[j].dayflow=_ltPubInfo->_AllInfo.dayFlowInfo.srvflow[j];
		    		if(_xFlowInfo[j].dayflow>1024000){
		    			 sprintf(_xFlowInfo[j].strdayflow,"%0.2fM",_xFlowInfo[j].dayflow/1024000.00);
		    		}else{
		    			 sprintf(_xFlowInfo[j].strdayflow,"%0.2fK",_xFlowInfo[j].dayflow/1024.00);
		    		}
		    		
   }
    
    
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   for(j=0;j<MAX_TOP_SRVNUM;j++){
   	if(_xFlowInfo[j].flowbps>0 ){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"srvname\":\"%s\",\"srvflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.2f%c\",\"hourtime\":\"%s\",\"hourflowu\":\"%llu\",\"hourflow\":\"%s\",\"daytime\":\"%s\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname, 
	     	  	 	_xFlowInfo[j].flowbps,_xFlowInfo[j].strflowbps,
	     	  	 	(((float)_xFlowInfo[j].flowbps)*100)/allflow,'%',
	     	  	 	_xFlowInfo[j].strhourtime,_xFlowInfo[j].hourflow,_xFlowInfo[j].strhourflow,
	     	  	 	_xFlowInfo[j].strdaytime,_xFlowInfo[j].dayflow,_xFlowInfo[j].strdayflow);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"srvname\":\"%s\",\"srvflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.2f%c\",\"hourtime\":\"%s\",\"hourflowu\":\"%llu\",\"hourflow\":\"%s\",\"daytime\":\"%s\",\"dayflowu\":\"%llu\",\"dayflow\":\"%s\"}",
	     	  	 	_ltPubInfo->topSrvName[j].srvname, 
	     	  	 	_xFlowInfo[j].flowbps,_xFlowInfo[j].strflowbps,
	     	  	 	(((float)_xFlowInfo[j].flowbps)*100)/allflow,'%',
	     	  	 	_xFlowInfo[j].strhourtime,_xFlowInfo[j].hourflow,_xFlowInfo[j].strhourflow,
	     	  	 	_xFlowInfo[j].strdaytime,_xFlowInfo[j].dayflow,_xFlowInfo[j].strdayflow);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
	  }
   }    	
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
 
	 sprintf(caTemp,"%s/data/srv.json",_msahtmldir);
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
