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
#define MYBUFFER 1024


//关键字搜索
int msaSearchKeyWord(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  char sqlBuf[1024];
  char *searchkey=NULL;
  long lCount=0;
  
	memset(sqlBuf,0,sizeof(sqlBuf));
  if(ltMsgGetVar_s(ltMsgPk,"searchkey")){
  	searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
  }else{
  	searchkey="";
  }
  printf("searchkey::%s\n",searchkey);
  if(strlen(searchkey)>0){
   	  memset(sqlBuf,0,sizeof(sqlBuf));
	 	  sprintf(sqlBuf,"select count(*) from msacontentkeyword");
	    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	    if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
	    }
	    
	    if(lCount==0){
	      	memset(sqlBuf,0,sizeof(sqlBuf));
			 		sprintf(sqlBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
			 		printf("add:%s \n",sqlBuf);
			 		ltDbExecSql(G_DbCon,sqlBuf);
	    }else{
	    	  memset(sqlBuf,0,sizeof(sqlBuf));
	    	  //判断表中是否存在searchkey
	    	  sprintf(sqlBuf,"select kname from msacontentkeyword where kname like '%c%s%c'",'%',searchkey,'%');
	    	  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			    if(tempCursor!=NULL){
		          tempRow=ltDbFetchRow(tempCursor);
		          if(tempRow==NULL){
		             lCount=0;
		          }
		          ltDbCloseCursor(tempCursor);
			    }
	    	  
	    	  //插入数据
	    	  if(lCount==0){
	    	  	  memset(sqlBuf,0,sizeof(sqlBuf));
	    	  	  sprintf(sqlBuf,"select max(kid)+1 from msacontentkeyword");
					    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					    if(tempCursor!=NULL){
				          tempRow= ltDbFetchRow(tempCursor);
				          if(tempRow!=NULL){
				             lCount=atol(tempRow[0]);
				          }
				          ltDbCloseCursor(tempCursor);
					    }
					    
					    memset(sqlBuf,0,sizeof(sqlBuf));
					 		sprintf(sqlBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
					 		printf("add:%s \n",sqlBuf);
					 		ltDbExecSql(G_DbCon,sqlBuf);
	    	  }
	    	 
	    
	    }
   }
     
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8"); 
  lt_dbput_rootvars(dbPtr,1,"searchkey",searchkey);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/searchresult.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);

  printf("fund end.......\n");
  return 0;
	
}



//日期树构建————内容关键字检索
int msaSearchkeyDateTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char sqlBuf[512];
	char tmpstr[512];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	stralloc strTemp;

	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	sprintf(sqlBuf,"select distinct sdate from msaloglist where tabtype=7 order by sdate");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			char myday[8];
      char *tabDate=NULL;
			char dateDesc[10];
			while(tempRow!=NULL){
	      tabDate=tempRow[0];
	      myday[0]=tabDate[0];
	      myday[1]=tabDate[1];
	      myday[2]=tabDate[2];
	      myday[3]=tabDate[3];
	      myday[4]=tabDate[4];
	      myday[5]=tabDate[5];
	      myday[6]=tabDate[6];
	      myday[7]=tabDate[7];
	      myday[8]='\0';
			  sprintf(dateDesc,"%c%c%c%c-%c%c-%c%c",myday[0],myday[1],myday[2],myday[3],myday[4],myday[5],myday[6],myday[7]);
				/*判断有没有子节点*/
				if(iHead==1){
					sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],"Select a date,then choose Quick Search",dateDesc);
					iHead=0;
				}else{
				  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],"Select a date,then choose Quick Search",dateDesc);
			  }
			  stralloc_cats(&strTemp,tmpstr);
				stralloc_cats(&strTemp,",leaf: true}");
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




/*内容关键字查找ComboBox的json*/
int msaConKeywordJson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  stralloc strTemp;
  strTemp.s=0;	
  char caTmpp[2048],sqlBuf[1024];
  char *searchkey=NULL;
  int  lNum=0;
  long lCount=0;
  
	memset(caTmpp,0,sizeof(caTmpp));
	memset(sqlBuf,0,sizeof(sqlBuf));

	 //用户选择的时间
	 if(ltMsgGetVar_s(ltMsgPk,"searchkey")){
  	searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
   }else{
   	searchkey="";
   }
   
   printf("searchkey:%s\n",searchkey);
   /*if(strlen(searchkey)>0){
   	  memset(sqlBuf,0,sizeof(sqlBuf));
	 	  sprintf(sqlBuf,"select count(*) from msacontentkeyword");
	    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	    if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
             lCount=atol(tempRow[0]);
          }
          ltDbCloseCursor(tempCursor);
	    }
	    
	    if(lCount==0){
	      	memset(sqlBuf,0,sizeof(sqlBuf));
			 		sprintf(sqlBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
			 		printf("add:%s \n",sqlBuf);
			 		ltDbExecSql(G_DbCon,sqlBuf);
	    }else{
	    	  memset(sqlBuf,0,sizeof(sqlBuf));
	    	  //判断表中是否存在searchkey
	    	  sprintf(sqlBuf,"select kname from msacontentkeyword where kname like '%c%s%c'",'%',searchkey,'%');
	    	  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			    if(tempCursor!=NULL){
		          tempRow=ltDbFetchRow(tempCursor);
		          if(tempRow==NULL){
		             lCount=0;
		          }
		          ltDbCloseCursor(tempCursor);
			    }
	    	  
	    	  //插入数据
	    	  if(lCount==0){
	    	  	  memset(sqlBuf,0,sizeof(sqlBuf));
	    	  	  sprintf(sqlBuf,"select max(kid)+1 from msacontentkeyword");
					    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					    if(tempCursor!=NULL){
				          tempRow= ltDbFetchRow(tempCursor);
				          if(tempRow!=NULL){
				             lCount=atol(tempRow[0]);
				          }
				          ltDbCloseCursor(tempCursor);
					    }
					    
					    memset(sqlBuf,0,sizeof(sqlBuf));
					 		sprintf(sqlBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
					 		printf("add:%s \n",sqlBuf);
					 		ltDbExecSql(G_DbCon,sqlBuf);
	    	  }
	    	 
	    
	    }
   }*/
	 

  
  /*计算记录数目*/
  
 if(strlen(searchkey)>0){
   memset(sqlBuf,0,sizeof(sqlBuf));
   sprintf(sqlBuf,"select count(*) from msacontentkeyword where kname like '%c%s%c'",'%',searchkey,'%'); 
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			lCount=atol(tempRow[0]);
		}
	}
	ltDbCloseCursor(tempCursor);
 

	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select kid,ltrim(kname) from msacontentkeyword where kname like '%c%s%c'",'%',searchkey,'%');
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
		    tempRow=ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){  
	        lNum++;						
	        if(lNum==1){              				      
						      sprintf(caTmpp,"{\"kid\":\"%ld\",\"kname\":\"%s\"}",atol(tempRow[0]),tempRow[1]);				       
							    stralloc_cats(&strTemp,caTmpp);
					}else{					    
						      sprintf(caTmpp,",{\"kid\":\"%ld\",\"kname\":\"%s\"}",atol(tempRow[0]),tempRow[1]);
							    stralloc_cats(&strTemp,caTmpp);
	
				  }       
			    tempRow= ltDbFetchRow(tempCursor);
		    }
	     ltDbCloseCursor(tempCursor);
   }
   
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
		
		
	  ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		
		
	}else{
		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");	
	
	}
		
    
   ltMsgFree(ltMsgPk);

	 return 0;		

}


/*grid显示json构造*/
int msaShowContentJson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  stralloc strTemp;
  strTemp.s=0;	
  char caTmpp[MYBUFFER*2],sqlBuf[MYBUFFER];
  char *searchkey=NULL;
  char *searchdate=NULL;
  char *caMsgFile=NULL;
  char tableDate[8];
  char tableDate2[11];
  char sqltablename[64];
  char sDate[32];
  char caFile1[MYBUFFER];
  char readFile[512];
  char myContent[MYBUFFER];
  long lRowNum,lStartRec,lSumRec,lCount,lNum;
  
  int iReturn;
  int i;
  struct stat st;
  struct dirent *d;
  FILE *fp;
  DIR *dir;	
  
	memset(caTmpp,0,sizeof(caTmpp));
	memset(sqlBuf,0,sizeof(sqlBuf));
  lRowNum=100; 
  lSumRec=0;
  lCount=0; 
  lStartRec=0;
  lNum=0;
  
  //获取关键字
  if(ltMsgGetVar_s(ltMsgPk,"searchkey")){
  	searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
  }else{
  	searchkey="";
  }
  printf("searchkey:%s\n",searchkey);
  
  if(ltMsgGetVar_s(ltMsgPk,"date")){
  	searchdate=ltMsgGetVar_s(ltMsgPk,"date");
  }else{
  	searchdate="";
  }
  printf("searchdate:%s\n",searchdate);
  if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
	printf("lRowNum:%ld\n",lRowNum);
	if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	}
	printf("lStartRec:%ld\n",lStartRec);
	 memset(tableDate,0,sizeof(tableDate));
	 memset(sqltablename,0,sizeof(sqltablename));
	 memset(tableDate2,0,sizeof(tableDate2));
	 memset(readFile,0,sizeof(readFile));  
	 nasTimeFormat(sDate,time(0));
	 tableDate[0]='2';
	 tableDate[1]='0';
	 tableDate[2]=sDate[0];
	 tableDate[3]=sDate[1];
	 tableDate[4]=sDate[3];
	 tableDate[5]=sDate[4];
	 tableDate[6]=sDate[6];
	 tableDate[7]=sDate[7];
	 tableDate[8]='\0';
	 
	 tableDate2[0]=searchdate[0];
	 tableDate2[1]=searchdate[1];
	 tableDate2[2]=searchdate[2];
	 tableDate2[3]=searchdate[3];
	 tableDate2[4]='-';
	 tableDate2[5]=searchdate[4];
	 tableDate2[6]=searchdate[5];
	 tableDate2[7]='-';
	 tableDate2[8]=searchdate[6];
	 tableDate2[9]=searchdate[7];
	 tableDate2[10]='\0';
	 printf("tableDate2:%s\n",tableDate2);

	   
   if(strlen(searchdate)>0){
   	 sprintf(sqlBuf,"select sdate,tabname,tabtype from msaloglist where tabtype=7 and sdate='%s'",searchdate);
   }else{
   	 sprintf(sqlBuf,"select sdate,tabname,tabtype from msaloglist where tabtype=7 and sdate='%s'",tableDate);
   }
	
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL){	  	
		    sprintf(sqltablename,"%s%s",tempRow[1],tempRow[0]);     
		  }
   }	
	 ltDbCloseCursor(tempCursor);
	 printf("sqltablename:%s\n",sqltablename);
  
  memset(sqlBuf,0,sizeof(sqlBuf));
  sprintf(sqlBuf,"select count(*) from %s",sqltablename); 
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow=ltDbFetchRow(tempCursor);
		if(tempRow!=NULL){
			lCount=atol(tempRow[0]);
		}
	}
	ltDbCloseCursor(tempCursor);
 
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 if(strlen(searchkey)>0){
	 	 sprintf(sqlBuf,"select subject,sdate,file from %s where searchkey like '%c%s%c' limit %ld offset %ld",sqltablename,'%',searchkey,'%',lRowNum,lStartRec);
	 }else{
	 	 sprintf(sqlBuf,"select subject,sdate,file from %s limit %ld offset %ld",sqltablename,lRowNum,lStartRec);
	 }
	 printf("sqlBuf:%s\n",sqlBuf);
	 
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);  
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
		    tempRow=ltDbFetchRow(tempCursor);
	    	while(tempRow!=NULL){  
	        lNum++;		
	        caMsgFile=tempRow[2];	
				  for(i=0;i<strlen(caMsgFile);i++){
				 	    if(caMsgFile[i]==' '){
				 	    	caMsgFile[i]='\0';
				 	    }
				  }			
				  
				  iReturn=stat(caMsgFile ,&st);
			    if(iReturn!=0){
			        tempRow=ltDbFetchRow(tempCursor);
			        continue;
			    }

			    if(S_ISDIR(st.st_mode)==0){
			    	  tempRow=ltDbFetchRow(tempCursor);
					    continue;
			    }	
				  
				  dir=opendir(caMsgFile);
				  if(dir){		   					   		   		
				   	  while((d=readdir(dir))){					   	  	
				   	  	if(strcmp(d->d_name,".")==0) continue;
				    		if(strcmp(d->d_name,"..")==0) continue;			   	  	 
			   	  	  if(strcmp(d->d_name,"key.txt")==0){
			   	  	   	  sprintf(caFile1,"%s/%s", caMsgFile, d->d_name);			   	  	   	 
			   	  	   	  fp=fopen(caFile1,"r");		
			   	  	   	  printf("filepath:%s\n",caFile1);		  			   	  	   			   	  	   	  			   	  	   	  	  	   	  
			   	  	   	  while(fread(readFile, strlen(readFile)+1, 1,fp)){	 
				   	  	   	  	if(strstr(readFile,searchkey)){	
				   	  	   	  		char tmp1[1024],tmp2[1024];  	 	   	  		
				   	  	   	  		fseek(fp, -40L, SEEK_CUR); 	
				   	  	   	  		memset(myContent,0,sizeof(myContent));  	   	  	   	  		 	  	   	  		
				   	  	   	  		fread(myContent,30, 1,fp);					   	  	   	  				
				   	  	   	  		memset(tmp1,0,sizeof(tmp1)); 
				   	  	   	  		fread(tmp1,strlen(searchkey)+10, 1,fp);
				   	  	   	  		sprintf(myContent,"%s<font color=red >%s</font>",myContent,tmp1);	
				   	  	   	  		memset(tmp2,0,sizeof(tmp2)); 
				   	  	   	  		fread(tmp2,30, 1,fp);
				   	  	   	  		sprintf(myContent,"%s%s",myContent,tmp2);
				   	  	   	  		break;
				   	  	   	  	}else{
				   	  	   	  		 sprintf(myContent,"%s","<font color=red >Sorry,not find the keyword!</font>");
				   	  	   	  	}				   	  	   	  	
			   	  	   	  }
			   	  	   	 	
			   	  	   	 	
			   	  	   	 	 
			   	  	   	 	   	  			   	  	   	  										   	  	   	 
			   	  	   	  fclose(fp);				            
			   	  	  }				   	
				   	  	
				   	  }
			   	 }
			    closedir(dir);
				  
	        if(lNum==1){              				      
						      sprintf(caTmpp,"{\"id\":\"%ld\",\"subject\":\"%s\",\"sdate\":\"%s\",\"content\":\"%s\"}",lNum,tempRow[0],tempRow[1],myContent);				       
							    stralloc_cats(&strTemp,caTmpp);
					}else{					    
						      sprintf(caTmpp,",{\"id\":\"%ld\",\"subject\":\"%s\",\"sdate\":\"%s\",\"content\":\"%s\"}",lNum,tempRow[0],tempRow[1],myContent);
							    stralloc_cats(&strTemp,caTmpp);
	
				  }       
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



/*设置关键字*/
int msaConfigKeyword(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char strBuf[1024];
	 char *addflag=NULL;
	 char *delflag=NULL;
	 char *searchkey=NULL;
	 long lCount=0,kid=0;
	 
	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 /*获取页面参数*/
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"kid")){
	 	 kid=atol(ltMsgGetVar_s(ltMsgPk,"kid"));
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"searchkey")){
	 	 searchkey=ltMsgGetVar_s(ltMsgPk,"searchkey");
	 }

	 /*表时间处理*/
	 memset(strBuf,0,sizeof(strBuf)); 
	 /*******************添加关键字************/
	 if(addflag){		
		 	 if(strlen(searchkey)>0){
		   	   memset(strBuf,0,sizeof(strBuf));
			 	   sprintf(strBuf,"select count(*) from msacontentkeyword");
			     tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			     if(tempCursor!=NULL){
		          tempRow= ltDbFetchRow(tempCursor);
		          if(tempRow!=NULL){
		             lCount=atol(tempRow[0]);
		          }
		          ltDbCloseCursor(tempCursor);
			     }
		    
				    if(lCount==0){
				      	memset(strBuf,0,sizeof(strBuf));
						 		sprintf(strBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
						 		printf("add:%s \n",strBuf);
						 		ltDbExecSql(G_DbCon,strBuf);
				    }else{
				    	  memset(strBuf,0,sizeof(strBuf));
				    	  //判断表中是否存在searchkey
				    	  sprintf(strBuf,"select kname from msacontentkeyword where kname like '%c%s%c'",'%',searchkey,'%');
				    	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
						    if(tempCursor!=NULL){
					          tempRow=ltDbFetchRow(tempCursor);
					          if(tempRow==NULL){
					             lCount=0;
					          }
					          ltDbCloseCursor(tempCursor);
						    }
				    	  
				    	  //插入数据
				    	  if(lCount==0){
				    	  	  memset(strBuf,0,sizeof(strBuf));
				    	  	  sprintf(strBuf,"select max(kid)+1 from msacontentkeyword");
								    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
								    if(tempCursor!=NULL){
							          tempRow= ltDbFetchRow(tempCursor);
							          if(tempRow!=NULL){
							             lCount=atol(tempRow[0]);
							          }
							          ltDbCloseCursor(tempCursor);
								    }
								    
								    memset(strBuf,0,sizeof(strBuf));
								 		sprintf(strBuf,"insert into msacontentkeyword(kid,kname) values(%ld,'%s')",lCount,searchkey);
								 		printf("add:%s \n",strBuf);
								 		ltDbExecSql(G_DbCon,strBuf);
				    	  }
				    	 
				    
				    }
	     }		
	 }
	 
	 /*******************删除关键字************/
	 if(delflag){	      
	 		memset(strBuf,0,sizeof(strBuf));
	 		sprintf(strBuf,"delete from msacontentkeyword where kid=%ld",kid);
	 		printf("del:%s \n",strBuf);
	 		ltDbExecSql(G_DbCon,strBuf);
	 }


  //返回页面
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(strBuf,0,sizeof(strBuf));
	 sprintf(strBuf,"select kid,kname from msacontentkeyword");
	 printf("strBuf:%s \n",strBuf);
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,	
				"kid",LT_TYPE_STRING,tempRow[0],	
				"keyword",LT_TYPE_STRING,tempRow[1]		
				);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/keyconfig.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   
   return 0;
}







