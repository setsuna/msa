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
#define  MAX_SHOW_TCP_NUM 500


/*实时连接页面返回函数*/
int ltconquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
  int iii;
  
  char *ipadd;
  char *groupid;
  char *userid;
  char *srvid;
  
  if(ltMsgGetVar_s(ltMsgPk,"sip")){ 	  
      ipadd=ltMsgGetVar_s(ltMsgPk,"sip");	
      if(strcmp(ipadd,"-1")==0){
      	ipadd="";	
      }
  }else{
      ipadd="";	
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"groupid")){
      groupid=ltMsgGetVar_s(ltMsgPk,"groupid");	
  }else{
      groupid="";	
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"userid")){
      userid=ltMsgGetVar_s(ltMsgPk,"userid");	
  }else{
      userid="";	
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"srvid")){
      srvid=ltMsgGetVar_s(ltMsgPk,"srvid");	
  }else{
      srvid="-1";	
  }
  
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  
  //groupid,userid，服务三样参数 

  if(strcmp(srvid,"-1")==0){
  	  lt_dbput_rootvars(dbPtr,1,"mysrvid",srvid);
  	  lt_dbput_rootvars(dbPtr,1,"myselected","selected");
     	
  }else{
  	  lt_dbput_rootvars(dbPtr,1,"mysrvid","-1");
  	  lt_dbput_rootvars(dbPtr,1,"myselected","");
  }
  
  lt_dbput_rootvars(dbPtr,1,"showtype",ltMsgGetVar_s(ltMsgPk,"showtype"));
  lt_dbput_rootvars(dbPtr,1,"ipadd",ipadd);  
  lt_dbput_rootvars(dbPtr,1,"mygid",groupid);  
  lt_dbput_rootvars(dbPtr,1,"myuid",userid);  


  
  
  tablePtr=lt_dbput_table(dbPtr,"srvlist");	
  for(iii=0;iii<MAX_TOP_SRVNUM;iii++){
	  if(_ltPubInfo->topSrvName[iii].srvname[0]){
		  	 if(iii==atoi(srvid)){
		  	 	printf("iii:  %d\n",iii);
		  		lt_dbput_recordvars(tablePtr,3,
				  "srvid",LT_TYPE_LONG,iii,
				  "sel",LT_TYPE_STRING,"selected",
				  "srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
		     }else{
		     lt_dbput_recordvars(tablePtr,3,
				  "srvid",LT_TYPE_LONG,iii,
				  "sel",LT_TYPE_STRING,"",
				  "srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
				 }
	  	}
						
	}                
  
  
  
  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/nowview/nowconect.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  	
  return 0;
  
}



static int msaSortTcpBufByTime(const void *p1,const void *p2)
{
    nasTcpBuffer *ps1,*ps2;
    ps1=(nasTcpBuffer *)p1;//类型转换
    ps2=(nasTcpBuffer *)p2;//类型转换    
    return (ps2->lLastTime-ps1->lLastTime);//流量差
}


int msaNowConectInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
	  char *cIp;
	  char sip[32];
	  char dip[32];
	  struct in_addr sip_add;
	  struct in_addr dip_add;
	  unsigned long  my_IP;  
	  
	  int  uid;
	  char userId[30];
	  int  gid;
	  int  srvid;
	  int  showtype;
    
    char srvName[48];
    char groupname[30];
    char uname[30];
		char sByte1[30];
		char sByte0[30];		
		char dPort[20];
		char sPort[20];
		
		char caCon[512];
		msaService   *ltService; 
    long lRowNum,lStartRec;
    
    //变量初始化
    lRowNum=100L;
    lStartRec=0L;     
    uid=-1;
    gid=-1;
    srvid=-1;
    showtype=0;
    my_IP=0;
    
    ltService = (msaService *)(_ltPubInfo->ltService);
    
    if(ltMsgGetVar_s(ltMsgPk,"start")){
     	lStartRec=atoi(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
     	lRowNum=atoi(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    
	  if(ltMsgGetVar_s(ltMsgPk,"srvid")){
			srvid=atol(ltMsgGetVar_s(ltMsgPk,"srvid"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"uid")){
			uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"gid")){
			gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"showtype")){
			showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
		}
		
    
	  cIp=ltMsgGetVar_s(ltMsgPk,"sip");
	  if(cIp){
		  if(strlen(cIp)>0){
       my_IP=ntohl(inet_addr(cIp));
      }
    }
    
    
	       
     struct sembuf lock={0,-1,SEM_UNDO};
     struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
     unsigned int MSAS_MAXHASHNUM;//最大哈希数
     shm_hash_link_TcpBufferindex *_gnm_hash_index;//哈希索引
     shm_hash_link_TcpBuffer      *_gnm_hash_content;//哈希内容
     shm_hash_link_TcpBuffer      *walker;//标记
     long indexWalker;
     int b;
     int k;
     int j;
     int jjj; 

     char caTmpp[2048];
	   stralloc strTemp;
     nasTcpBuffer  *nasHashItemInfo;        
	   nasTcpBuffer  *_TmpTcpBuffer;	  
     _TmpTcpBuffer=(nasTcpBuffer*)malloc(sizeof(nasTcpBuffer)*(MAX_SHOW_TCP_NUM));//分配空间
     memset((char *)_TmpTcpBuffer,0,sizeof(nasTcpBuffer)*(MAX_SHOW_TCP_NUM));//初始化
     
     jjj=0;
     char  *p=(char *)_ltPubInfo;
     p=(char *)_ltPubInfo;//公共指针位置
     _gnm_hash_index=(shm_hash_link_TcpBufferindex *)(p+sizeof(msaPubInfo)+_ltPubInfo->_tcpsessionhead.bufptr);
     _gnm_hash_content=(shm_hash_link_TcpBuffer *)(p+sizeof(msaPubInfo)+_ltPubInfo->_tcpsessionhead.conbufptr);
     MSAS_MAXHASHNUM=_ltPubInfo->_tcpsessionhead.bufnum;//MSAS_MAXHASHNUM 10000
     

	  for( b=0;b< MSAS_MAXHASHNUM;b++){   	         
	         semop(_Gsemid[TCPBUFSEMID],&lock,1);//上锁 TCPBUFSEMID=3
				   indexWalker=_gnm_hash_index[b].next;//			   
				   semop(_Gsemid[TCPBUFSEMID],&unlock,1);//解锁
				   if(indexWalker==-1){
				            continue;
				   }
				   walker=&_gnm_hash_content[indexWalker];
	         	         	         
				   for (; ; ) {
				             nasHashItemInfo=&walker->item;    				          			             
				                            				             
				             if( nasHashItemInfo->lBytes>0 ){   //本类型指针访问成员                                       
	                        //条件判断	                        
                          if(showtype!=0){
															if(showtype==1){/*正常*/	
																if(nasHashItemInfo->cMatchRule!=-1){ 
																	 if(_ltPubInfo->_Srvrulelist[(int)nasHashItemInfo->cMatchRule].ruleaction!=1){//不满足的条件
																	  continue;
																   }
																}										
															}else if(showtype==2){/*流控连接*/	
																if(nasHashItemInfo->cMatchRule==-1){
																	continue;
																}
																														
																if(_ltPubInfo->_Srvrulelist[(int)nasHashItemInfo->cMatchRule].ruleaction==0
																	|| _ltPubInfo->_Srvrulelist[(int)nasHashItemInfo->cMatchRule].qosid==0){//0 拒绝 1 允许
																		continue;
																}
															  
															}else if(showtype==3){/*规则阻断的连接*/
																if(nasHashItemInfo->cMatchRule==-1){
																	continue;
																}	
																													
																if( _ltPubInfo->_Srvrulelist[(int)nasHashItemInfo->cMatchRule].ruleaction==1 ){//没有满足条件则继续循环
																	 //qosid!=0 action=1 则通过
																		continue;
																}
															  
															}
																													
													}                                             

                               
                          if(srvid!=-1){
                          	
											    	if(srvid!=(int)_ltPubInfo->ltService[nasHashItemInfo->cService].topSrv){
											    		continue;
											      }
											    }
											    
											    
											    if(uid!=-1){
												    	if(uid!=nasHashItemInfo->lId){
												    		continue;
												      }
												  }else{	
												  												    
													    if(gid!=-1){								
													    	int g;
													    	int glevel;
													      glevel=_ltPubInfo->_GroupList[gid].level;
													    	if(_ltDbUserInfoList->groupid[glevel]!=gid){
																		continue;
																}
																if(gid>0){
														    	for(g=1;g<_MAX_GROUPLEVEL;g++){
														    		if(_ltDbUserInfoList->groupid[g]!=gid){
																			continue;
																	  }
														    	}	
													      }																													
														  }

												  }
                          
                          
                          if(my_IP){
											    	if(nasHashItemInfo[jjj].lSip!=my_IP && nasHashItemInfo->lDip!=my_IP ){
											    		continue;
											    	}
											    }
                          
	                        memcpy(&_TmpTcpBuffer[jjj],nasHashItemInfo,sizeof(nasTcpBuffer));//copy
	                        jjj++;//记录符合条件的记录数目
	                        if(jjj>=MAX_SHOW_TCP_NUM){
							               break;	
							            }	
	                        
	                        
		                 }       			
		                 
				             semop(_Gsemid[TCPBUFSEMID],&lock,1);			
				             if(walker->next == -1){	
				                       semop(_Gsemid[TCPBUFSEMID],&unlock,1);//解除			
				                       break;		
				             }
				
				             indexWalker = walker->next;
				             semop(_Gsemid[TCPBUFSEMID],&unlock,1);//TCPBUFSEMID=3
				             walker = &_gnm_hash_content[indexWalker];
				
				  }
	
	 
	
	  }

   //排序
    qsort(_TmpTcpBuffer,jjj,sizeof(nasTcpBuffer),msaSortTcpBufByTime);
   
   //构建json 
 
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

	 	     //用户id
	 	     memset(userId,0,sizeof(userId));
	 	     sprintf(userId,"%d",_TmpTcpBuffer[j].lId);	
	 	     //用户名
	 	     memset(uname,0,sizeof(uname));
				 sprintf(uname,"%s",_ltDbUserInfoList[_TmpTcpBuffer[j].lId].username);
			 	 memset(groupname,0,sizeof(groupname));
			 	 //部门名称处理
		     sprintf(groupname,"%s",_ltPubInfo->_GroupList[_ltDbUserInfoList[_TmpTcpBuffer[j].lId].groupid[0]].name);
				 int g;
				 for(g=1;g<_MAX_GROUPLEVEL;g++){
					         if(_ltDbUserInfoList[_TmpTcpBuffer[j].lId].groupid[g]==0){
					                   break;
					         }else{
					            sprintf(groupname,"%s_%s",groupname,_ltPubInfo->_GroupList[_ltDbUserInfoList[_TmpTcpBuffer[j].lId].groupid[g]].name);
					         }
				 }
				 
	 	     
			 //ip处理	
			 memset(sip,0,sizeof(sip)); 
	     memset(dip,0,sizeof(dip)); 
			 sip_add.s_addr=htonl(_TmpTcpBuffer[j].lSip);
		   dip_add.s_addr=htonl(_TmpTcpBuffer[j].lDip);
		   sprintf(sip,"%s",inet_ntoa(sip_add));
	     sprintf(dip,"%s",inet_ntoa(dip_add));	 
	     //端口处理
	     memset(sPort,0,sizeof(sPort)); 
	     memset(dPort,0,sizeof(dPort)); 
	     sprintf(sPort,"%d",_TmpTcpBuffer[j].nSport);
	     sprintf(dPort,"%d",_TmpTcpBuffer[j].nDport);		
	         
	     //上行、下行流量处理	    
	     memset(sByte1,0,sizeof(sByte1)); 
	     memset(sByte0,0,sizeof(sByte0)); 
	     if(_TmpTcpBuffer[j].lBytes[1]>1024000){
    			 sprintf(sByte1,"%0.3fM",_TmpTcpBuffer[j].lBytes[1]/1024000.00);
    	 }else{
    			 sprintf(sByte1,"%0.3fK",_TmpTcpBuffer[j].lBytes[1]/1024.00);
    	 }
       
       if(_TmpTcpBuffer[j].lBytes[0]>1024000){
    			 sprintf(sByte0,"%0.3fM",_TmpTcpBuffer[0].lBytes[1]/1024000.00);
    	 }else{
    			 sprintf(sByte0,"%0.3fK",_TmpTcpBuffer[0].lBytes[1]/1024.00);
    	 }
	     //内容处理			     
	     memset(caCon,0,sizeof(caCon));
	     sprintf(caCon,"%s",_TmpTcpBuffer[j].keyword);	
	     //一级服务 topsrv服务名称		
       memset(srvName,0,sizeof(srvName));
       sprintf(srvName,"%s-%s",_ltPubInfo->topSrvName[_ltPubInfo->ltService[_TmpTcpBuffer[j].cService].topSrv].srvname,ltService[_TmpTcpBuffer[j].cService].srvName);//得到服务名称
		  	   
			 if(k==0){
			  	 sprintf(caTmpp,"{\"uid\":\"%s\",\"uname\":\"%s\",\"gname\":\"%s\",\"lSip\":\"%s\",\"lDip\":\"%s\",\"nSport\":\"%s\",\"nDport\":\"%s\",\"shBytes\":\"%s\",\"xiaBytes\":\"%s\",\"cService\":\"%s\",\"keyword\":\"%s\"}",
			  	 	userId,
			  	 	uname,
			      groupname,
			  	 	sip, 
			  	 	dip, 
			  	 	sPort, 
			  	 	dPort,
				 	 	sByte1,
				 	 	sByte0,
			 	 	 	srvName,
			 	 	 	caCon);  	     
			     stralloc_cats(&strTemp,caTmpp);
			  }else{
			  	 sprintf(caTmpp,",{\"uid\":\"%s\",\"uname\":\"%s\",\"gname\":\"%s\",\"lSip\":\"%s\",\"lDip\":\"%s\",\"nSport\":\"%s\",\"nDport\":\"%s\",\"shBytes\":\"%s\",\"xiaBytes\":\"%s\",\"cService\":\"%s\",\"keyword\":\"%s\"}",
			  	 	userId,
			  	 	uname,
			      groupname,
			  	 	sip, 
			  	 	dip, 
			  	 	sPort, 
			  	 	dPort,
				 	 	sByte1,
				 	 	sByte0,
			 	 	 	srvName,
			 	 	 	caCon);
			     stralloc_cats(&strTemp,caTmpp);
			  }
			  
			  k++;//自增标记 
	 }    	
	 	
	 stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
	  
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 free(_TmpTcpBuffer);
	 
	 return 0;

}


