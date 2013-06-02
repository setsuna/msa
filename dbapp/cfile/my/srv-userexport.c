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
#include <dirent.h>
#include "msa.h"


int loopgrouppid(stralloc *strTemp,int level,int pid,char *keyword,int *pbcount);
int isChekKeywordOk(int level,int gid,char *keyword);
int loopgroupKeyword(int level,int pid,char *keyword);
int loopgrougexport(stralloc *strTemp,int level,int pid);
int loopgroupallcount(int level,int pid,int *allcount);


/*账号导出动态显示页面*/
int msaAccoutShowPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");  
   //部门
   if(ltMsgGetVar_s(ltMsgPk,"gid")){
    lt_dbput_rootvars(dbPtr,1,"gid",ltMsgGetVar_s(ltMsgPk,"gid"));
   }
   //策略
   if(ltMsgGetVar_s(ltMsgPk,"pid")){
   	lt_dbput_rootvars(dbPtr,1,"policy",ltMsgGetVar_s(ltMsgPk,"pid"));
   }

   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
   
}




/*账户导出*/
int msaAccountExport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char *gid=NULL;
	 char pid[128];
	 char *glevel;
	 int  level;
	 char *keyword=NULL;
	 char *userflag=NULL;
	 int lRead;
	 
	 struct in_addr s;//IP
	 char strip[32];
   int k,g,iii,jjj,kkk;
   
	 nasDbUserInfo *tmpdbuserinfo;
	 
	 char caTmpp[1024];
   stralloc strTemp;
	 
   long lRowNum,lStartRec,allcount;  
	 lRowNum=100; //每页的行数
	 allcount=0; 
	 lStartRec=0;
	 
	 if(ltMsgGetVar_s(ltMsgPk,"limit")){
	   lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	   if(lRowNum<=0 || lRowNum>500){
	   	lRowNum=100;
	   }
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start")){
	   lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	   if(lStartRec<0){
	     lStartRec=0;
	   }
	 }
	 
   
   int int_gid;
   if(ltMsgGetVar_s(ltMsgPk,"gid")){
   	 gid=ltMsgGetVar_s(ltMsgPk,"gid");
   }
	 int_gid=atoi(gid);
   
   if(ltMsgGetVar_s(ltMsgPk,"pid")){
    	sprintf(pid,"%s",ltMsgGetVar_s(ltMsgPk,"pid"));
   }

   
   level=0;
   if(ltMsgGetVar_s(ltMsgPk,"level")){
    	glevel=ltMsgGetVar_s(ltMsgPk,"level");
    	level=atol(glevel);
    	if(level<0 || level>=_MAX_GROUPLEVEL){
	 	    level=0;
	    }
   }
   printf("level:%d\n",level);
   
	 	 
	 if(ltMsgGetVar_s(ltMsgPk,"keyword")){
	   keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
	 }else{
	 	 keyword="";
	 }
	 
	 
	 int user_flag=0;
	 if(ltMsgGetVar_s(ltMsgPk,"userflag")){
	   userflag=ltMsgGetVar_s(ltMsgPk,"userflag");
	   user_flag=atoi(userflag);
	 }
	 
	 printf("userflag:%s\nuser_flag:%d\n",userflag,user_flag);
   
	 tmpdbuserinfo=malloc(lRowNum*sizeof(nasDbUserInfo));//定义数组先是当页面的内容
	 
	 k=0;
	 g=0;
   allcount=0;
	 for(jjj=0;jjj<_ltPubInfo->_dbUserInfoHead.bufnum;jjj++){
	 	 if(strlen(_ltDbUserInfoList[jjj].username)>0){
	 	   //部门筛选
	 		
	 		if(int_gid!=-1){								
				    	if(_ltDbUserInfoList[jjj].groupid[level]!=int_gid){
									continue;
							}
							if(int_gid>0){
					    	for(g=1;g<_MAX_GROUPLEVEL;g++){
					    		if(_ltDbUserInfoList[jjj].groupid[g]!=int_gid){
										continue;
								  }
					    	}	
				      }																													
			}
	 		
	 		//策略筛选
 			if(strlen(pid)>0){
	 			for(iii=0;iii<_MAX_ALLPOLICY;iii++){	
	 				 if(_ltDbUserInfoList[jjj].policyid[iii]=='1'){
	 				    	   if(pid[iii]!='1'){
	 				    	   	  continue;
	 				    	   }
	 				 } 						
		 		}
 			}
 			
 			//关键字筛选
 			if(strlen(keyword)>0){ 				
				s.s_addr = htonl(_ltDbUserInfoList[jjj].bindip);
				sprintf(strip,"%s",inet_ntoa(s));			   		  
   		  if(strcmp(_ltDbUserInfoList[jjj].username,keyword)!=0){
 			     if(strcmp(_ltDbUserInfoList[jjj].dispname,keyword)!=0){
 			     	 if(strcmp(strip,keyword)!=0){
 			     	 	  if(strcmp(_ltDbUserInfoList[jjj].bindemail,keyword)!=0){
 			     	 	  	continue;
 			     	 	  }
 			     	 }
 			     }		     
	 			}
   		 
 		  }
 		  
 			//用户状态筛选
 			if(user_flag!=-1){
	 			if((int)_ltDbUserInfoList[jjj].useflags!=user_flag){
	 				continue;
	 			}
 			}
      
 			
      allcount++;//记录累加

      if(allcount>=lStartRec){
         //拷贝
        memcpy(&tmpdbuserinfo[k],&_ltDbUserInfoList[jjj],sizeof(nasDbUserInfo));
        k++;
      }  	
     
	    
	    
	    if(k>=lRowNum){
	       break;	
	    }
	    
	    
    }
	    
  }
   
   
   
     
   /*将符合条件的记录存入指针*/	
    strTemp.s=0;    
   // printf("k:%d\n",k);
   	for(jjj=0;jjj<k;jjj++){			
				 //写入用户登录名
			   sprintf(caTmpp,"%s,",tmpdbuserinfo[jjj].username);
				 stralloc_cats(&strTemp,caTmpp);	
				 				 				  				 				 			               	  				     				  	
				 //写入用户真实名
				 sprintf(caTmpp,"%s,",tmpdbuserinfo[jjj].dispname);
				 stralloc_cats(&strTemp,caTmpp);
				 	
				 
				  //mail				 
				 sprintf(caTmpp,"%s,",tmpdbuserinfo[jjj].bindemail);
				 stralloc_cats(&strTemp,caTmpp);	
				 
				 
				 	//ip         
			   struct in_addr bindip_add;
			   bindip_add.s_addr=htonl(tmpdbuserinfo[jjj].bindip);				 
				 sprintf(caTmpp,"%s,",inet_ntoa(bindip_add));
				 stralloc_cats(&strTemp,caTmpp);
		   
		     //mac
			   char _bindmac[32]; 
			   memset(_bindmac,0,sizeof(_bindmac));
         nasCvtMac(tmpdbuserinfo[jjj].bindmac,_bindmac);       				 
				 sprintf(caTmpp,"%s,",_bindmac);
				 stralloc_cats(&strTemp,caTmpp);
				 	
				 				 
				 //部门名称处理			
				 char groupname[1024];
				 memset(groupname,0,sizeof(groupname));
				 sprintf(groupname,"%s",_ltPubInfo->_GroupList[(int)tmpdbuserinfo[jjj].groupid[0]].name);
				 sprintf(caTmpp,"%s,",groupname);
				 stralloc_cats(&strTemp,caTmpp);    				 
				 for(g=1;g<_MAX_GROUPLEVEL;g++){				 		 	   		 	   	
			 	   	 if((int)_ltPubInfo->_GroupList[(int)tmpdbuserinfo[jjj].groupid[g]].groupid>0){
			            sprintf(groupname,"%s",_ltPubInfo->_GroupList[(int)tmpdbuserinfo[jjj].groupid[g]].name);				           				         
			            sprintf(caTmpp,"%s,",groupname);   	
			            stralloc_cats(&strTemp,caTmpp); 
				     }else{
				     	    sprintf(caTmpp,"%s",",");   	
			            stralloc_cats(&strTemp,caTmpp); 
				     } 	  	
				 }   
				 
				 
				 
				 //策略组名称处理
				 for(kkk=0;kkk<_MAX_ALLPOLICY;kkk++){ 	 
				 	 if(strcmp(_ltPubInfo->_PolicyList[kkk].pname,"")){
				 	 	 if(tmpdbuserinfo[jjj].policyid[kkk]==49){
				 	       sprintf(caTmpp,"%s,",_ltPubInfo->_PolicyList[kkk].pname);
				 	       stralloc_cats(&strTemp,caTmpp);
				 	   }				 	    
				 	 }else{
				 	 	 break;
				 	 }				 	   				 	 				   						    				
			   }
	       

			 	
				 
				 
				
				 
				 
//				 //添加时间
//				 char _addtime[32];  
//				 memset(_addtime,0,sizeof(_addtime));        
//         nasTimeFormat(_addtime,tmpdbuserinfo[jjj].addtime);
//				 sprintf(caTmpp,"%s,",_addtime);
//				 stralloc_cats(&strTemp,caTmpp);				 
//				 
//				 
//				 //最后上线时间
//				 char _lasttime[32];  
//				 memset(_lasttime,0,sizeof(_lasttime));
//         nasTimeFormat(_lasttime,tmpdbuserinfo[jjj].lasttime);			 
//				 sprintf(caTmpp,"%s,",_lasttime);
//				 stralloc_cats(&strTemp,caTmpp);	
//				 				
//				 //状态
//         sprintf(caTmpp,"%d",tmpdbuserinfo[jjj].useflags);
//				 stralloc_cats(&strTemp,caTmpp);	

			   
			   sprintf(caTmpp,"%s","\r\n");
				 stralloc_cats(&strTemp,caTmpp);		
				 	
			
					 
    }	
  
	 /*将记录写入响应文件*/
	  stralloc_0(&strTemp);
	  
	  //printf("%s\n",strTemp.s);
	  
	  ltMsgPk->msgpktype=2;
	  lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"accountexport.csv","contenttype","Application/MS-DOWNLOAD");  		
		ltMsgFree(ltMsgPk);	
		lRead=strlen(strTemp.s);
		printf("lRead:%d\n",lRead);
		write(confd,strTemp.s,lRead);
	    
		return 0;
	
}





/*部门导出动态显示页面*/
int msaGroupExportShowPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");  
   //部门
   if(ltMsgGetVar_s(ltMsgPk,"gid")){
    lt_dbput_rootvars(dbPtr,1,"gid",ltMsgGetVar_s(ltMsgPk,"gid"));
   }
   
   
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/groupexport.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
   
}



/*部门查询*/
int msaGroupSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   
	 char *keyword=NULL;
	 int level,pid;
	 int j;
	 int k;
	 int allcount;
	 int pbcount;
	 
   char caTmpp[1024];
   stralloc strTemp;
   
   long lRowNum,lStartRec;  
	 lRowNum=100; //每页的行数
	 lStartRec=0;
	 
	 if(ltMsgGetVar_s(ltMsgPk,"limit")){
	   lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	   if(lRowNum<=0 || lRowNum>500){
	   	lRowNum=100;
	   }
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start")){
	   lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	   if(lStartRec<0){
	     lStartRec=0;
	   }
	 }
   
	 allcount=0;	
	 pbcount=0;
	 
	 pid=0;
	 level=0;
	 j=0;
	 	
	 	 
	 if(ltMsgGetVar_s(ltMsgPk,"keyword")){
	   keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
	 }else{
	 	 keyword="";
	 }
	 
	 printf("keyword:%s\n",keyword);
	  
	
	 for(j=0;j<_MAX_ALLGRPNUM;j++){	
	  	if(strcmp(keyword,"")==0){
	  		 if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){				
				  allcount++;
				  pid=_ltPubInfo->_GroupList[j].groupid;	
					if(level<5){
						loopgroupallcount(level,pid,&allcount);					
					}
				 }		
		  }else{
	  		   if(isChekKeywordOk(_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,keyword)){
					 		allcount++;				
	         }
	  	
	  	}
	  	
	  	if(allcount>lRowNum){
		   break;	
		  }
	  	
	  	
	  }
	
	
	  											
   pid=0;
	 level=0;
	 j=0;
   k=0;
	 strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   
 	 for(j=0;j<_MAX_ALLGRPNUM;j++){	
 	 	
		if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){	
			
			  printf("ok?=%d\n",isChekKeywordOk(_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,keyword));
				if(isChekKeywordOk(_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,keyword)){						
					if(k==0){
						sprintf(caTmpp,"{\"groupid\":\"%d\",\"groupmydesc\":\"%s\",\"groupname\":\"%s\"}",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
						stralloc_cats(&strTemp,caTmpp);
					}else{
					  sprintf(caTmpp,",{\"groupid\":\"%d\",\"groupmydesc\":\"%s\",\"groupname\":\"%s\"}",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				    stralloc_cats(&strTemp,caTmpp);
				  }
				  
				  printf("catempp:%s\n",caTmpp);
				  pbcount++;
					pid=_ltPubInfo->_GroupList[j].groupid;	
					if(level<5){
						loopgrouppid(&strTemp,level,pid,keyword,&pbcount);
						
					}
					
					k++;
									
			 }							
		}
		
		if(pbcount>lRowNum){//页面控制
		   break;	
		}
		
		
	 }
   
   
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
 
   printf("groupjson:%s\n",strTemp.s);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 
	 return 0;
 
  
}



//记录统计loop
int loopgroupallcount(int level,int pid,int *allcount)
{
	int j;
	level++; 
	for(j=0;j<_MAX_ALLGRPNUM;j++){
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){                   	               
					(*allcount)++;
					pid=_ltPubInfo->_GroupList[j].groupid;			
					if(level<5){
						loopgroupallcount(level,pid,allcount);
					}
	  }
  }

	return 0;
	
}





char myName1[64],myName2[64],myName3[64],myName4[64]; 
//子部门json构造  
int loopgrouppid(stralloc *strTemp,int level,int pid,char *keyword,int *pbcount){
	char caTmpp[512]; 
	char strgname[512];
	int j;
	int i;
	level++;
  memset(strgname,0,sizeof(strgname));
  
	for(j=0;j<_MAX_ALLGRPNUM;j++){
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){                   	               
        if(isChekKeywordOk(_ltPubInfo->_GroupList[j].level,_ltPubInfo->_GroupList[j].groupid,keyword)){
          if(level==1){	       		
	       		sprintf(myName1,"%s",_ltPubInfo->_GroupList[pid].name);
	       	}else if(level==2){	       		
	       		sprintf(myName2,"%s",_ltPubInfo->_GroupList[pid].name);
	       	}else if(level==3){
	       		sprintf(myName3,"%s",_ltPubInfo->_GroupList[pid].name);
	       	}else{
	       		sprintf(myName4,"%s",_ltPubInfo->_GroupList[pid].name);
	       	}
	       	 
	       	sprintf(strgname,"%s","_");	
	       	int index=0;     
			    for(i=0;i<_ltPubInfo->_GroupList[j].level;i++){		    	  
			    	  if(index==0){
			    	  	sprintf(strgname,"%s_%s",strgname,myName1);
			    	  }
			    	  
			    	  if(index==1){
			    	  	sprintf(strgname,"%s_%s",strgname,myName2);
			    	  }
			    	  
			    	  if(index==2){
			    	  	sprintf(strgname,"%s_%s",strgname,myName3);
			    	  }
			    	  
			    	  if(index==3){
			    	  	sprintf(strgname,"%s_%s",strgname,myName4);
			    	  }
			    	  
			        index++;
			        printf("strgname:%s\n",strgname);
			    }  		
			    sprintf(strgname,"%s_%s",strgname,_ltPubInfo->_GroupList[j].name);
	
				  sprintf(caTmpp,",{\"groupid\":\"%d\",\"groupmydesc\":\"%s\",\"groupname\":\"%s\"}",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,strgname);
			    stralloc_cats(strTemp,caTmpp);
		
	        (*pbcount)++;
					pid=_ltPubInfo->_GroupList[j].groupid;			
					if(level<5){
						loopgrouppid(strTemp,level,pid,keyword,pbcount);
					}
					
				}
	
	  }
  }

	return 0;
	
}



/*判断某节点属性是否符合关键字搜索*/
int isChekKeywordOk(int level,int gid,char *keyword)
{	
  int j; 
  if(strcmp(keyword,"")==0){
					return 1;
	}
	
	for(j=0;j<_MAX_ALLGRPNUM;j++){
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].groupid==gid && _ltPubInfo->_GroupList[j].name[0] ){          		   												
				 if(strcmp(_ltPubInfo->_GroupList[j].name,keyword)==0){
								return 1;
		 		 }else if(strcmp(_ltPubInfo->_GroupList[j].mydesc,keyword)==0){											
								return 1;											
				 }else{			 	
							if(level<5){
								if(loopgroupKeyword(level,_ltPubInfo->_GroupList[j].groupid,keyword)){
								  return 1;	
								}
							}
				 }				
							
	  }
 
  } 
  
  return 0;
}



//子部门关键字检查
int loopgroupKeyword(int level,int pid,char *keyword)
{
  int j;
  level++;  
	for(j=0;j<_MAX_ALLGRPNUM;j++){
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){							   
			   if(strcmp(_ltPubInfo->_GroupList[j].name,keyword)==0){
								return 1;
		 		 }else if(strcmp(_ltPubInfo->_GroupList[j].mydesc,keyword)==0){												
								return 1;											
				 }else{			 	
						if(level<5){				
							pid=_ltPubInfo->_GroupList[j].groupid;
							if(loopgroupKeyword(level,pid,keyword)){
							   return 1;	
							}
						}
				 } 			 		 									
	  }
 
  }   
 
 return 0;
 
}









/*部门导出*/
int msaGroupDataExport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   

	 int  j,lRead=0;
	 char caTmpp[1024];
   stralloc strTemp; 	
   int pid=0;
   int level=0;
   strTemp.s=0;    
	 for(j=0;j<_MAX_ALLGRPNUM;j++){	
		    if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){	
					  sprintf(caTmpp,"%s,",_ltPubInfo->_GroupList[j].name);
				 		stralloc_cats(&strTemp,caTmpp);	 	 	
				 		
				 		sprintf(caTmpp,"%s","\n");				 		
				    stralloc_cats(&strTemp,caTmpp);
				    
						pid=_ltPubInfo->_GroupList[j].groupid;	
						if(level<5){						
							loopgrougexport(&strTemp,level,pid);
							
						}
						
										
				 }	
							
	  }
 
	  stralloc_0(&strTemp);	  
	  printf("%s\n",strTemp.s);	  
	  ltMsgPk->msgpktype=2;
	  lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"groupexport.csv","contenttype","Application/MS-DOWNLOAD");  		
		ltMsgFree(ltMsgPk);	
		lRead=strlen(strTemp.s);
		write(confd,strTemp.s,lRead);

	    
		return 0;
 
  
}


//子部门导出格式构造
int loopgrougexport(stralloc *strTemp,int level,int pid){
	char caTmpp[1024]; 
	int j;
	level++;
	for(j=0;j<_MAX_ALLGRPNUM;j++){
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){                   		     							
				
				if(level==1){					
					sprintf(caTmpp,"%s",",");				 		
					stralloc_cats(strTemp,caTmpp); 
					
					sprintf(caTmpp,"%s",_ltPubInfo->_GroupList[j].name);
			 		stralloc_cats(strTemp,caTmpp);	 	 	
		 		 	
				}else if(level==2){
								
					sprintf(caTmpp,"%s",",,");				 		
					stralloc_cats(strTemp,caTmpp); 
					
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 
										
					
					sprintf(caTmpp,"%s",_ltPubInfo->_GroupList[j].name);
			 		stralloc_cats(strTemp,caTmpp);	 	 	
					
				}else if(level==3){
					sprintf(caTmpp,"%s",",,,");				 		
					stralloc_cats(strTemp,caTmpp); 
					
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 
//										
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 					
					
					sprintf(caTmpp,"%s",_ltPubInfo->_GroupList[j].name);
			 		stralloc_cats(strTemp,caTmpp);
				}else{
					sprintf(caTmpp,"%s",",,,,");				 		
					stralloc_cats(strTemp,caTmpp); 
					
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 
//										
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 			
//					
//					sprintf(caTmpp,"%s",",");				 		
//					stralloc_cats(strTemp,caTmpp); 		
					
					sprintf(caTmpp,"%s",_ltPubInfo->_GroupList[j].name);
			 		stralloc_cats(strTemp,caTmpp);
				
				}
				
				
				sprintf(caTmpp,"%s","\n");				 		
				stralloc_cats(strTemp,caTmpp);
				
				 
				pid=_ltPubInfo->_GroupList[j].groupid;	
						
				if(level<5){ 
						loopgrougexport(strTemp,level,pid);
				}

	  }
  }

	return 0;
	
}




