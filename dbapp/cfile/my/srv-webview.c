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
#include <sys/sem.h>
#include "msa.h"


static int msaSortHttpBufByByte(const void *p1,const void *p2)
{
    nasHttpBuffer *ps1,*ps2;
    ps1 = (nasHttpBuffer *)p1;
    ps2 = (nasHttpBuffer *)p2;    
    return (ps2->lBytes - ps1->lBytes);
}

int msaWebListInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 long lRowNum,lStartRec;
    
   struct sembuf lock={0,-1,SEM_UNDO};
   struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
   unsigned int MSAS_MAXHASHNUM;
   shm_hash_link_HttpBufferindex *_gnm_hash_index;
   shm_hash_link_HttpBuffer      *_gnm_hash_content;
   shm_hash_link_HttpBuffer      *walker;
   long indexWalker;
   int b;
   int j;
   int k;
   char caTmpp[2048];
   stralloc strTemp;

   
   nasHttpBuffer  *nasHashItemInfo;
   int jjj;  
   nasHttpBuffer  *_TmpHttpBuffer;
   
   lRowNum=100;
   lStartRec=0;
     
   if(ltMsgGetVar_s(ltMsgPk,"start")){
     	lStartRec=atoi(ltMsgGetVar_s(ltMsgPk,"start"));
   }
   if(ltMsgGetVar_s(ltMsgPk,"limit")){
     	lRowNum=atoi(ltMsgGetVar_s(ltMsgPk,"limit"));
   }

   
   jjj=0;
   _TmpHttpBuffer=(nasHttpBuffer  *)malloc(sizeof(nasHttpBuffer)*_ltPubInfo->_httpbufferhead.bufnum);
   memset((char *)_TmpHttpBuffer,0,sizeof(nasHttpBuffer)*_ltPubInfo->_httpbufferhead.bufnum);
   
   char  *p=(char *)_ltPubInfo;

   //p=(char *)_ltPubInfo;
   _gnm_hash_index=(shm_hash_link_HttpBufferindex *)(p+sizeof(msaPubInfo)+_ltPubInfo->_httpbufferhead.bufptr);
   _gnm_hash_content=(shm_hash_link_HttpBuffer *)(p+sizeof(msaPubInfo)+_ltPubInfo->_httpbufferhead.conbufptr);
   MSAS_MAXHASHNUM=_ltPubInfo->_httpbufferhead.bufnum;

 

  for(b=0;b<MSAS_MAXHASHNUM;b++){
	 
	   semop(_Gsemid[HTTPBUFSEMID],&lock,1);
	   indexWalker=_gnm_hash_index[b].next;
	   semop(_Gsemid[HTTPBUFSEMID],&unlock,1);
	   if(indexWalker==-1){
	            continue;
	   }
	   walker = &_gnm_hash_content[indexWalker];
	   for (; ; ) {
	        nasHashItemInfo=&walker->item;
	        if( nasHashItemInfo->lBytes>0 ){                                          
	            memcpy(&_TmpHttpBuffer[jjj],nasHashItemInfo,sizeof(nasHttpBuffer));
	            jjj++;	
	        }
	        semop(_Gsemid[HTTPBUFSEMID],&lock,1);
	        if(walker->next == -1){
	              semop(_Gsemid[HTTPBUFSEMID],&unlock,1);
	              break;
	        }
	
	        indexWalker = walker->next;
	        semop(_Gsemid[HTTPBUFSEMID],&unlock,1);
	        walker = &_gnm_hash_content[indexWalker];
	  }

 
  } 
   //排序
   qsort(_TmpHttpBuffer,jjj,sizeof(nasHttpBuffer),msaSortHttpBufByByte);
   //一个一个出来构建json 
   
//   jjj=3;
//    
// 	 	_TmpHttpBuffer[0].lStartTime=1040;
//	 	_TmpHttpBuffer[0].lLastTime=2000;
//	 	_TmpHttpBuffer[0].lBytes=88;
//	 	sprintf(_TmpHttpBuffer[0].caHost,"%s","192.168.0.10");
//
//	 	
//		_TmpHttpBuffer[1].lStartTime=1200;
//	 	_TmpHttpBuffer[1].lLastTime=2400;
//	 	_TmpHttpBuffer[1].lBytes=388;
//	 	sprintf(_TmpHttpBuffer[1].caHost,"%s","192.168.0.120");
//
//	 	
//	 	_TmpHttpBuffer[2].lStartTime=1100;
//	 	_TmpHttpBuffer[2].lLastTime=2300;
//	 	_TmpHttpBuffer[2].lBytes=188;
//	 	sprintf(_TmpHttpBuffer[2].caHost,"%s","192.168.0.110");

   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",jjj);
   stralloc_cats(&strTemp,caTmpp);
   k=0;

   if(lStartRec>(jjj-1)){
   	  lStartRec=jjj-1;
   }
   if(lStartRec<0){
   	  lStartRec=0;
   }
   if((lStartRec+lRowNum)>jjj){
   	  lRowNum=jjj-lStartRec;
   }
   
   
   for(j=lStartRec;j<(lStartRec+lRowNum);j++){
   	      char strip[32];
   	      memset(strip,0,sizeof(strip));
   				struct in_addr s;
   				char   caSTime[32];
   				char   caETime[32];
   				char   caTimes[32];
   				char   caByte[32];
					s.s_addr = htonl(_TmpHttpBuffer[j].lIp);
					sprintf(strip,"%s",inet_ntoa(s));
					memset(caSTime,0,sizeof(caSTime));
       		memset(caETime,0,sizeof(caETime));
       		memset(caTimes,0,sizeof(caTimes));
       		memset(caByte,0,sizeof(caByte));
       		nasTimeLongFormat(caSTime,_TmpHttpBuffer[j].lStartTime);
       		nasTimeLongFormat(caETime,_TmpHttpBuffer[j].lLastTime);	
       		sprintf(caTimes,"%s",time2str(_TmpHttpBuffer[j].lTime));
       		
       		if(_TmpHttpBuffer[j].lBytes>1024000){
		    			 sprintf(caByte,"%0.3fM",_TmpHttpBuffer[j].lBytes/1024000.00);
		    	}else{
		    			 sprintf(caByte,"%0.3fK",_TmpHttpBuffer[j].lBytes/1024.00);
		    	}
       		
   	
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"lStartTime\":\"%s\",\"lLastTime\":\"%s\",\"lBytes\":\"%s\",\"counts\":\"%lu\",\"times\":\"%s\",\"caHost\":\"%s\",\"ip\":\"%s\",\"urlsort\":\"%s\"}",
	     	  	 	caSTime, 
	     	  	 	caETime, 
	     	  	 	caByte, 
	     	  	 	_TmpHttpBuffer[j].lCount,
	     	  	 	caTimes,
	     	  	 	_TmpHttpBuffer[j].caHost,
	     	  	 	strip,_ltPubInfo->_urlSortlist[_TmpHttpBuffer[j].cUrlSort].sortname
	     	  	 	);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"lStartTime\":\"%s\",\"lLastTime\":\"%s\",\"lBytes\":\"%s\",\"counts\":\"%lu\",\"times\":\"%s\",\"caHost\":\"%s\",\"ip\":\"%s\",\"urlsort\":\"%s\"}",
	     	  	 	caSTime, 
	     	  	 	caETime, 
	     	  	 	caByte, 
	     	  	 	_TmpHttpBuffer[j].lCount,
	     	  	 	caTimes,
	     	  	 	_TmpHttpBuffer[j].caHost,
	     	  	 	strip,_ltPubInfo->_urlSortlist[_TmpHttpBuffer[j].cUrlSort].sortname
	     	  	 	);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	k++;
   }    	
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 free(_TmpHttpBuffer);
	 return 0;
  
}

