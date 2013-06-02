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


typedef struct _xgroupInfo_s {
    char     strgname[64];
    unsigned long long   flowbps; 
    char     strflowbps[32];
    unsigned long long   nowflow; 
    char     strnowflow[32];
    unsigned long daytime; 
    char     strdaytime[32];
    unsigned long long   dayflow; 
    char     strdayflow[32];
    unsigned long long   dayflowu;//上流量
    char     strdayflowu[32];
    unsigned long long   dayflowd;//下流量
    char     strdayflowd[32];
    char     lasttime[32];
}xgroupInfo;

 

int msaloopjson(xgroupInfo  *_gListInfo,int *iindex,int level,int groupid,unsigned int *allcount,int mysec,int glevel,int lTime);

 
int msaloopjson(xgroupInfo  *_gListInfo,int *iindex,int level,int groupid,unsigned int *allcount,int mysec,int glevel,int lTime)

{ 
   int j;
   int k;
   int i;
   long long allsrvflow;
   j=0;
   k=0;
   i=0;
   
   char     nowTime[32];
   memset(nowTime,0,sizeof(nowTime));
   

  for(j=0;j<_MAX_ALLGRPNUM;j++){      
              if(_ltPubInfo->_GroupList[j].pid==groupid && _ltPubInfo->_GroupList[j].level==(level+1)){
                      allsrvflow=0;
                      allsrvflow=_ltPubInfo->_GroupList[j].nowFlowInfo.allflow[0]+_ltPubInfo->_GroupList[j].nowFlowInfo.allflow[1];
                      if(allsrvflow>0){
                          (*allcount)++;
                      }else{
                          continue;
                      }
                      sprintf(_gListInfo[*iindex].strgname,"%s","_");
                      for(i=0;i<_ltPubInfo->_GroupList[j].level;i++){
                          sprintf(_gListInfo[*iindex].strgname,"%s_%s",_gListInfo[*iindex].strgname,_ltPubInfo->_GroupList[i].name);
                      }  

                      sprintf(_gListInfo[*iindex].strgname,"%s_%s",_gListInfo[*iindex].strgname,_ltPubInfo->_GroupList[j].name);
                      _gListInfo[*iindex].flowbps=(allsrvflow*8)/mysec;                                                                                  
                      if(_gListInfo[*iindex].flowbps>1024000){
                           sprintf(_gListInfo[*iindex].strflowbps,"%0.3fM",_gListInfo[*iindex].flowbps/1024000.00);
                      }else{
                           sprintf(_gListInfo[*iindex].strflowbps,"%0.3fK",_gListInfo[*iindex].flowbps/1024.00);
                      }
                      _gListInfo[*iindex].daytime=_ltPubInfo->_GroupList[*iindex].dayFlowInfo.alltime;
                      sprintf(_gListInfo[*iindex].strdaytime,"%s",time2str(_gListInfo[*iindex].daytime));
                      
                       //最后访问时间   
                       
                      nasTimeFormat(nowTime,_ltPubInfo->_GroupList[j].dayFlowInfo.alllasttime);               
                      sprintf(_gListInfo[*iindex].lasttime,"%s",nowTime);
                      
                      
                      
                      
                      _gListInfo[*iindex].dayflow=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[0]+_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[1];
                      if(_gListInfo[*iindex].dayflow>1024000){
                          sprintf(_gListInfo[*iindex].strdayflow,"%0.3fM",_gListInfo[*iindex].dayflow/1024000.00);
                      }else{
                          sprintf(_gListInfo[*iindex].strdayflow,"%0.3fK",_gListInfo[*iindex].dayflow/1024.00);
                      }
                          
                          
                            //上流量
                    _gListInfo[*iindex].dayflowu=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[0];
                    if(_gListInfo[*iindex].dayflowu>1024000){
                         sprintf(_gListInfo[*iindex].strdayflowu,"%0.3fM",_gListInfo[*iindex].dayflowu/1024000.00);
                    }else{
                         sprintf(_gListInfo[*iindex].strdayflowu,"%0.3fK",_gListInfo[*iindex].dayflowu/1024.00);

                    }
                    
                    
                    
                    //下流量
                    _gListInfo[*iindex].dayflowd=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[1];
                    if(_gListInfo[*iindex].dayflowd>1024000){
                         sprintf(_gListInfo[*iindex].strdayflowd,"%0.3fM",_gListInfo[*iindex].dayflowd/1024000.00);
                    }else{
                         sprintf(_gListInfo[*iindex].strdayflowd,"%0.3fK",_gListInfo[*iindex].dayflowd/1024.00);

                    }
                          
                          
                          

                    (*iindex)++;
                    if(_ltPubInfo->_GroupList[j].level<(glevel-1)){
                         msaloopjson(_gListInfo,iindex,_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,allcount,mysec,glevel,lTime);                
                    } 

              }     

   }

 

  return 0;

 

}
 

int msaNowGroupList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)

{

    xgroupInfo  _gListInfo[_MAX_ALLGRPNUM];
    xgroupInfo  _gTopListInfo[_MAX_ALLGRPNUM];
    
    unsigned long long lMaxBytes;
    int      j,allcount,k,iindex,itopindex;
    char caTmpp[2048];
    stralloc strTemp;
    unsigned long long allflow,allsrvflow;
    int      iFd;
    unsigned int lTime;
    int      mysec;
    char     caTemp[256];
    char     nowTime[32];
    int  glevel;
    if(ltMsgGetVar_s(ltMsgPk,"glevel")==NULL){
    	glevel=1;
    }else{
    	glevel=atoi(ltMsgGetVar_s(ltMsgPk,"glevel"));
    }
   
   memset((void *)_gListInfo,0,sizeof(xgroupInfo)*_MAX_ALLGRPNUM);
   memset((void *)_gTopListInfo,0,sizeof(xgroupInfo)*_MAX_ALLGRPNUM);
   
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
   allsrvflow=0;
   k=0;
   
   memset(nowTime,0,sizeof(nowTime));

   //加一个锁  
   iindex=0;
   itopindex=0;
   for(j=0;j<_MAX_ALLGRPNUM;j++){  	           
                 if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0){
                    allsrvflow=0;
                    allsrvflow=_ltPubInfo->_GroupList[j].nowFlowInfo.allflow[0]+_ltPubInfo->_GroupList[j].nowFlowInfo.allflow[1];
                    
                    if(allsrvflow>0){
                         allcount++;
                    }else{
                         continue;
                    }
                                       
                    sprintf(_gListInfo[iindex].strgname,"%s",_ltPubInfo->_GroupList[j].name);
                    _gListInfo[iindex].flowbps=(allsrvflow*8)/mysec;                                                           

                    if(_gListInfo[iindex].flowbps>1024000){
                          sprintf(_gListInfo[iindex].strflowbps,"%0.3fM",_gListInfo[iindex].flowbps/1024000.00);
                    }else{
                          sprintf(_gListInfo[iindex].strflowbps,"%0.3fK",_gListInfo[iindex].flowbps/1024.00);

                    }

                    //总流速
                    allflow=allflow+_gListInfo[iindex].flowbps;   
          
                    
                    //当天访问总时间          
                    _gListInfo[iindex].daytime=_ltPubInfo->_GroupList[j].dayFlowInfo.alltime;
                    sprintf(_gListInfo[iindex].strdaytime,"%s",time2str(_gListInfo[iindex].daytime));
                    //最后访问时间         
                    nasTimeFormat(nowTime,_ltPubInfo->_GroupList[j].dayFlowInfo.alllasttime);           
                    sprintf(_gListInfo[iindex].lasttime,"%s",nowTime);
                    
                    //总流量
                    _gListInfo[iindex].dayflow=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[0]+_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[1];

                    if(_gListInfo[j].dayflow>1024000){
                         sprintf(_gListInfo[iindex].strdayflow,"%0.3fM",_gListInfo[iindex].dayflow/1024000.00);
                    }else{
                         sprintf(_gListInfo[iindex].strdayflow,"%0.3fK",_gListInfo[iindex].dayflow/1024.00);

                    }
                    
                    
                    //上流量
                    _gListInfo[iindex].dayflowu=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[0];
                    if(_gListInfo[iindex].dayflowu>1024000){
                         sprintf(_gListInfo[iindex].strdayflowu,"%0.3fM",_gListInfo[iindex].dayflowu/1024000.00);
                    }else{
                         sprintf(_gListInfo[iindex].strdayflowu,"%0.3fK",_gListInfo[iindex].dayflowu/1024.00);

                    }
                    
                    
                    
                    //下流量
                    _gListInfo[iindex].dayflowd=_ltPubInfo->_GroupList[j].dayFlowInfo.allflow[1];
                    if(_gListInfo[iindex].dayflowd>1024000){
                         sprintf(_gListInfo[iindex].strdayflowd,"%0.3fM",_gListInfo[iindex].dayflowd/1024000.00);
                    }else{
                         sprintf(_gListInfo[iindex].strdayflowd,"%0.3fK",_gListInfo[iindex].dayflowd/1024.00);

                    }
                    
                    
                 
                    
                    memcpy(&(_gTopListInfo[itopindex]),&(_gListInfo[iindex]),sizeof(xgroupInfo));
                    itopindex++;
                    iindex++;
                    if(_ltPubInfo->_GroupList[j].level<(glevel-1)){
                           msaloopjson(_gListInfo,&iindex,_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,&allcount,mysec,glevel,lTime);             
                    }
                 
                 }    
                           
   }

   
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",itopindex);
   stralloc_cats(&strTemp,caTmpp);          
                              
   k=0;     
   for(j=0;j<itopindex;j++){                                                   

      if(k==0){
         sprintf(caTmpp,"{\"gname\":\"%s\",\"gflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
                      _gTopListInfo[j].strgname, 
                      _gTopListInfo[j].flowbps,
                      _gTopListInfo[j].strflowbps,
                      (((float)_gTopListInfo[j].flowbps)*100)/allflow,
                      _gTopListInfo[j].daytime,
                      _gTopListInfo[j].strdaytime,
                      _gTopListInfo[j].dayflow,
                      _gTopListInfo[j].strdayflow,
                      _gTopListInfo[j].dayflowu,//上流量                      
                      _gTopListInfo[j].strdayflowu,
                      _gTopListInfo[j].dayflowd,                      
                      _gTopListInfo[j].strdayflowd,//下流量
                      _gTopListInfo[j].lasttime);
         stralloc_cats(&strTemp,caTmpp);
      }else{
              sprintf(caTmpp,",{\"gname\":\"%s\",\"gflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
                      _gTopListInfo[j].strgname, 
                      _gTopListInfo[j].flowbps,
                      _gTopListInfo[j].strflowbps,
                      (((float)_gTopListInfo[j].flowbps)*100)/allflow,
                      _gTopListInfo[j].daytime,
                      _gTopListInfo[j].strdaytime,
                      _gTopListInfo[j].dayflow,
                      _gTopListInfo[j].strdayflow,
                      _gTopListInfo[j].dayflowu,//上流量                      
                      _gTopListInfo[j].strdayflowu,
                      _gTopListInfo[j].dayflowd,                      
                      _gTopListInfo[j].strdayflowd,//下流量
                      _gTopListInfo[j].lasttime);
         stralloc_cats(&strTemp,caTmpp);
      }
     k++;  
   } 

  stralloc_cats(&strTemp,"]}");
  stralloc_0(&strTemp); 
 //将一级部门的json写入到文件
  printf("myjson:%s\n",strTemp.s);
  sprintf(caTemp,"%s/data/gnv.json",_msahtmldir);
  iFd = open(caTemp,O_WRONLY | O_CREAT | O_TRUNC);
  if(iFd >0 ) {
                  write(iFd, strTemp.s, strTemp.len ); 
                  close(iFd);

  }



   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);          
                              
   k=0;     

   for(j=0;j<allcount;j++){                                                      
      if(k==0){
         sprintf(caTmpp,"{\"gname\":\"%s\",\"gflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
                      _gListInfo[j].strgname, 
                      _gListInfo[j].flowbps,
                      _gListInfo[j].strflowbps,
                      (((float)_gListInfo[j].flowbps)*100)/allflow,
                      _gListInfo[j].daytime,
                      _gListInfo[j].strdaytime,
                      _gListInfo[j].dayflow,
                      _gListInfo[j].strdayflow,                     
                      _gListInfo[j].dayflowu,//上流量                      
                      _gListInfo[j].strdayflowu,
                      _gListInfo[j].dayflowd,                      
                      _gListInfo[j].strdayflowd,//下流量
                      _gListInfo[j].lasttime                                            
                      );
         stralloc_cats(&strTemp,caTmpp);
      }else{
              sprintf(caTmpp,",{\"gname\":\"%s\",\"gflowu\":\"%llu\",\"flowbps\":\"%s\",\"flowp\":\"%0.3f\",\"daytime\":\"%lu\",\"strdaytime\":\"%s\",\"dayflow\":\"%llu\",\"strdayflow\":\"%s\",\"dayflowu\":\"%llu\",\"strdayflowu\":\"%s\",\"dayflowd\":\"%llu\",\"strdayflowd\":\"%s\",\"lasttime\":\"%s\"}",
                      _gListInfo[j].strgname, 
                      _gListInfo[j].flowbps,
                      _gListInfo[j].strflowbps,
                      (((float)_gListInfo[j].flowbps)*100)/allflow,
                      _gListInfo[j].daytime,
                      _gListInfo[j].strdaytime,
                      _gListInfo[j].dayflow,
                      _gListInfo[j].strdayflow,
                      _gListInfo[j].dayflowu,//上流量                      
                      _gListInfo[j].strdayflowu,
                      _gListInfo[j].dayflowd,                      
                      _gListInfo[j].strdayflowd,//下流量
                      _gListInfo[j].lasttime 
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

  return 0;
}
