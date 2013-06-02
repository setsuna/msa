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
#define formdir   "/formsearch"
#define JAVACMD "/app/ns/java/jdk1.6.0_01/bin/java -cp /app/ns/java/poi/poi-3.7-20101029.jar:./ "
#define BINCMD	"/app/msa/bin"

/*
MSA:数据中心
功能:表单提交内容动态链接页面
*/
int ltmsaform(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;

  char strBuf[512];
  char *gid;
  char *uid;
  char *url;
  char *dip;
  char *date;   
  char *btype; 
  char *bbyte;
  char *fbtype; 
  char *fbbyte;
  char *osort;  
  char *orderby;
  char *matype;
  int Num=0;

  char *dbUser;
  char *dbPass;
  char *dbName;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon!=NULL){
		printf("db connect ok\n");
	}else{
		fprintf(stderr,"db connect error\n");
	}
  
  gid=ltMsgGetVar_s(ltMsgPk,"gid");
  uid=ltMsgGetVar_s(ltMsgPk,"uid");
  url=ltMsgGetVar_s(ltMsgPk,"url");
  dip=ltMsgGetVar_s(ltMsgPk,"dip");
  date=ltMsgGetVar_s(ltMsgPk,"date");
  btype=ltMsgGetVar_s(ltMsgPk,"btype");
  bbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
  fbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
  fbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
	orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
  matype=ltMsgGetVar_s(ltMsgPk,"matype");

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  //分类
  memset(strBuf,0,sizeof(strBuf));
  sprintf(strBuf,"select rid,rname from msaformfilter");
	tablePtr=lt_dbput_table(dbPtr,"mtypelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
			Num++;		
			if(Num==1){
       	lt_dbput_recordvars(tablePtr,3,"rid",LT_TYPE_STRING,tempRow[0],"rname",LT_TYPE_STRING,tempRow[1]);
			}else{
        lt_dbput_recordvars(tablePtr,3,"rid",LT_TYPE_STRING,tempRow[0],"rname",LT_TYPE_STRING,tempRow[1]);
			}
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);




  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=6");
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
			Num++;
			char tempDate[32];
			char ToDate[32];
			memset(tempDate,0,sizeof(tempDate));
			memset(ToDate,0,sizeof(ToDate));
			sprintf(tempDate,"%s",tempRow[0]);
			ToDate[0]=tempDate[0];
			ToDate[1]=tempDate[1];
			ToDate[2]=tempDate[2];
			ToDate[3]=tempDate[3];
			ToDate[4]='-';
			ToDate[5]=tempDate[4];
			ToDate[6]=tempDate[5];
			ToDate[7]='-';
			ToDate[8]=tempDate[6];
			ToDate[9]=tempDate[7];
			ToDate[10]='\0';
			if(Num==1){
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],
       "strdate",LT_TYPE_STRING,ToDate);
			}else{
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],
       "strdate",LT_TYPE_STRING,ToDate);
			}
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);

	lt_dbput_rootvars(dbPtr,1,"gid",gid);
	lt_dbput_rootvars(dbPtr,1,"uid",uid);
	lt_dbput_rootvars(dbPtr,1,"mymtype",matype);
	lt_dbput_rootvars(dbPtr,1,"myurl",url);
	lt_dbput_rootvars(dbPtr,1,"mydip",dip);
	lt_dbput_rootvars(dbPtr,1,"mytabname",date);
	lt_dbput_rootvars(dbPtr,1,"mybtype",btype);
	lt_dbput_rootvars(dbPtr,1,"mybbyte",bbyte);
	lt_dbput_rootvars(dbPtr,1,"myfbtype",fbtype);
	lt_dbput_rootvars(dbPtr,1,"myfbtype",fbbyte);
	lt_dbput_rootvars(dbPtr,1,"myosort",osort);
	lt_dbput_rootvars(dbPtr,1,"myorderby",orderby);

  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/formsearch.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
  return 0;

}


/*
MSA:数据中心
        表单提交内容
功能:当日表单记录查询
*/
int msaformList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{     
	  ltDbHeadPtr dbPtr;
	  ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char caWhere[512],delWhere[512];
    char *caUser=NULL; 
    char *gid=NULL;
    char *caMtype=NULL;  
    char *caUrl=NULL;
    char *caDip=NULL;
    char *tabname=NULL; 
    char *caBtype=NULL;
    char *caBbyte=NULL;
    char *caFbtype=NULL;
    char *caFbbyte=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char caTmpp[2048];
    char sqltablename[64];
    stralloc strTemp; 
    int  glevel,gpid;
	  char *dbUser;
	  char *dbPass;
	  char *dbName;
	  
	  strTemp.s=0;   
	  memset(strBuf,0,sizeof(strBuf));
	  memset(caWhere,0,sizeof(caWhere));
	  memset(delWhere,0,sizeof(delWhere));
	  memset(sqltablename,0,sizeof(sqltablename));
	  glevel=0;gpid=0;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
		} 
		   
    dbPtr=lt_dbinit();   	 
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
      iAction=0;
    }else{
      iAction=atol(m_action);
    }
    
    gid=ltMsgGetVar_s(ltMsgPk,"groupid"); 
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); 
    caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");  
    caUrl=ltMsgGetVar_s(ltMsgPk,"url");
    caDip=ltMsgGetVar_s(ltMsgPk,"dip");   
    tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
    caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
    caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
    caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
    caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");                
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");      // 排序方式
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby"); //升 降
       
		  
		lRowNum=100; /*每页的行数*/
		lSumRec=0;   /*总行数*/
		lCount=0;
		lStartRec=0;
		lNum=0;
	
		
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}

		sprintf(strBuf,"select level,name,pid from msagroup where id=%s",gid); /*获取部门管理级别*/
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				gpid=atol(tempRow[2]);
			}
		}
		ltDbCloseCursor(tempCursor);

		if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			   sprintf(caWhere,"%s","2=2");//查找整个部门用户
			   sprintf(delWhere,"%s","2=2");//查找整个部门用户
			}
		}

		if(strcmp(gid,"-1")!=0){//不等于-1
			if(strcmp(caUser,"-1")==0){
			    sprintf(caWhere,"a.groupid%d=%s",glevel,gid);//查找整个部门用户
			   	sprintf(delWhere,"2=2");
			}else{
					sprintf(caWhere,"b.userid=%s and a.groupid%d=%s",caUser,glevel,gid);
					sprintf(delWhere,"userid = %s",caUser);
		  }
		}     
			

   /*邮件分类*/
	 if(strcmp(caMtype,"-1")!=0){
	 	 sprintf(strBuf,"select rname from msaformfilter where rid=%s",caMtype);
		 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		 if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   sprintf(caWhere, "%s and (b.mtype='%s') ",caWhere,tempRow[0]);
			   sprintf(delWhere,"%s and (mtype='%s')",delWhere,tempRow[0]);
			  }
		 }
		 ltDbCloseCursor(tempCursor);	 
	 }
  
   if(caUrl!=NULL){
	    if(strlen(caUrl)>0) {
	        sprintf(caWhere,"%s and (b.formurl like '%c%s%c')",caWhere,'%',caUrl,'%');
	        sprintf(delWhere,"%s and (formurl like '%c%s%c')",delWhere,'%',caUrl,'%');
	    }
   }
    
   if(caDip!=NULL){
	    if(strlen(caDip)>0) {
	        sprintf(caWhere,"%s and (b.dip = '%s')",caWhere,caDip);
	        sprintf(delWhere,"%s and (dip = '%s')",delWhere,caDip);
	    }
   }


   if(strcmp(caBtype,"unit2") == 0) { /* 邮件大小*/
    	   sprintf(caWhere,"%s and (b.formsize >%s)",caWhere,caBbyte);
    	   sprintf(delWhere,"%s and (formsize >%s)",delWhere,caBbyte);
   }
    
   if(strcmp(caFbtype,"unit2") == 0) { /* 附件大小*/
    	  sprintf(caWhere,"%s and (b.formasize >%s)",caWhere,caFbbyte);
    	  sprintf(delWhere,"%s and (formasize >%s)",delWhere,caFbbyte);
   }
   
   
   if(strlen(tabname)>0){
   	    sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=6 and sdate='%s'",tabname);     
				tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
				if(tempCursor!=NULL){
					  tempRow=ltDbFetchRow(tempCursor);
					  if(tempRow!=NULL){
					    sprintf(sqltablename,"%s",tempRow[1]);	          
					  }
				}			
				ltDbCloseCursor(tempCursor);	
   }
   
	 sprintf(strBuf,"select count(*) from %s b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s",sqltablename,glevel,caWhere);    
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	 if(tempCursor!=NULL){
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL){
		   lCount=atol(tempRow[0]);
		  }
	 }
	 ltDbCloseCursor(tempCursor);	 

	 if(caOrder!=NULL){
		    if( strlen(caOrder)>0 ){
				    	if(strcmp(caOrder,"nosort")!=0){
						        if(strcmp(caOrder,"date")==0){
						            sprintf(caWhere,"%s order by b.sdate ",caWhere);
						        }else if(strcmp(caOrder,"from") == 0){
						            sprintf(caWhere,"%s order by b.mailfrom ",caWhere);
						        }else if(strcmp(caOrder,"to") == 0){
						            sprintf(caWhere,"%s order by b.mailto ",caWhere);
						        }else if(strcmp(caOrder,"dip") == 0){
												sprintf(caWhere,"%s order by b.dip ",caWhere);
							    	}else if(strcmp(caOrder,"size") == 0){
						            sprintf(caWhere,"%s order by b.mailsize ",caWhere);
						        }else if(strcmp(caOrder,"user") == 0){
						            sprintf(caWhere,"%s order by b.userid ",caWhere);
						        }
						        
						        if(strlen(caOrderby)){
							          if(caOrderby[0] == '1'){   		            	  
							              sprintf(caWhere,"%s desc",caWhere);
							          }else{
							          	  sprintf(caWhere,"%s asc",caWhere);
							          }
									  }
				    }
		   }   
   }	
   
   
     /*记录删除*/
		if(iAction==1){
			if(strlen(delWhere)>0){
				sprintf(sqlBuf,"delete from %s where %s ",sqltablename,delWhere);
			  ltDbExecSql(G_DbCon,sqlBuf);  
		  }
	
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
			ltMsgFree(ltMsgPk);
			
			return 0;		
		}

   		/*记录导出HTML*/
		if(iAction==2){
			      char xHead[1024];
			      char *lt_page_content=NULL;
			      char *ppp;
				    int  iii;
				    dbPtr=lt_dbinit();   		    					    
			      memset(xHead,0,sizeof(xHead));
				    sprintf(xHead,"%s","Content-disposition: attachment; filename=\"export.htm\"\nContent-type: application/octet-stream\nContent-transfer-encoding:BINARY\n\n");		      
			      lt_dbput_rootvars(dbPtr,1,"ipadd",_ltPubInfo->_msaOptionHead.strAuthIP);        
			      lt_dbput_rootvars(dbPtr,1,"gid",gid);
						lt_dbput_rootvars(dbPtr,1,"uid",caUser);
						lt_dbput_rootvars(dbPtr,1,"mymtype",caMtype);
						lt_dbput_rootvars(dbPtr,1,"myurl",caUrl);
						lt_dbput_rootvars(dbPtr,1,"mydip",caDip);
						lt_dbput_rootvars(dbPtr,1,"mytabname",tabname);
						lt_dbput_rootvars(dbPtr,1,"mybtype",caBtype);
						lt_dbput_rootvars(dbPtr,1,"mybtype",caBbyte);
						lt_dbput_rootvars(dbPtr,1,"myfbtype",caFbtype);
						lt_dbput_rootvars(dbPtr,1,"myfbtype",caFbbyte);
						lt_dbput_rootvars(dbPtr,1,"myosort",caOrder);
						lt_dbput_rootvars(dbPtr,1,"myorderby",caOrderby);
			      
				    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/datacenter/formrecordexport.htm",dbPtr->doc,0);
				    
				    if(lt_page_content==NULL){
				     	return -1;
				    }
				    iii=0;
				    ppp=lt_page_content;
				    for(iii=0;iii<30;iii++){
				    	if(*ppp!='<'){
				    		*ppp=' ';
				    	}else{
				    		break;
				      }
				    }
			
				    ppp=strrchr(lt_page_content,'>');
			      if(ppp){
			   		  *(ppp+1)='\0';
			      }

				  ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,xHead,"lthtml",LT_TYPE_STRING,lt_page_content);
				  free(lt_page_content);		    
			    ltMsgFree(ltMsgPk);
			    lt_dbfree(dbPtr);
							
				  return 0; 
				
			      
		}
		
		if(iAction==3){
		    char caFile[256];
		    char tmpdir[128];
		    char caTempDir[256];
		    char caLine[2048];
		    int fd=0;
    	  int flag=0;
		    memset(caFile,0,sizeof(caFile));
		   	memset(tmpdir,0,sizeof(tmpdir)); 
		    sprintf(tmpdir,"%lu",ltStrGetId());
		    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
				if(ltFileIsExist(caTempDir)!=1){
					if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
						ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					  ltMsgFree(ltMsgPk);
					  return 0;
					}
				}
				
	 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
			  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
			  if(fd == (-1)) {
					 ltMsgPk->msgpktype=1;
					 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					 ltMsgFree(ltMsgPk);
					 close(fd);				 
					 return 0;
				} 	
				
				sprintf(sqlBuf,"select b.userid,a.dispname,a.username,b.sdate,b.stime,b.formfile,b.formhost,b.formurl,b.formasize,b.formsize,b.dip,b.mtype,b.rback,b.fstart,b.flen,c.name from %s b left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %ld offset %ld",sqltablename,glevel,caWhere,lRowNum,lStartRec);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						 tempRow=ltDbFetchRow(tempCursor);
					   while(tempRow!=NULL){	
					   	    flag++;		   					   	    
					   		  char formasize[32];
					   		  char formsize[32];		   		  
					   		  char gname[512];
					   		  int  pid;
					       	int  i;
					       	pid=0;
					       	memset(gname,0,sizeof(gname));
					   		  memset(formasize,0,sizeof(formasize));
					   		  memset(formsize,0,sizeof(formsize));
					   		  if(atol(tempRow[8])>1024000){
			               sprintf(formasize,"%0.3fM",atol(tempRow[8])/1024000.00);
			            }else{
			               sprintf(formasize,"%0.3fK",atol(tempRow[8])/1024.00);
			            }
			
					   		  if(atol(tempRow[9])>1024000){
			               sprintf(formsize,"%0.3fM",atol(tempRow[9])/1024000.00);
			            }else{
			               sprintf(formsize,"%0.3fK",atol(tempRow[9])/1024.00);
			            }
					   	    if(glevel==0){
			             	 sprintf(gname,"%s",tempRow[15]);
			             }else if(glevel==1){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
			             	  	  	break;
			             	  	  }
			             	  }   
		   	  
			             }else if(glevel==2){
			               	for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	           	  
			             }else if(glevel==3){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	
			             }else if(glevel==4){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	             	  
			             }	
					   	   
								
							  sprintf(caLine,"%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n",
							    flag,
							    gname,//GroupName
									tempRow[1],//dispname
									tempRow[5],//表单文件名
									tempRow[3],//日期
									tempRow[4],//时间
									tempRow[6],//主机
									tempRow[7],//URL
									formasize,//附件大小
									formsize,//表单大小
									tempRow[10], //目的IP
									tempRow[11] //表单分类			
									);
							 write(fd,caLine,strlen(caLine));
							 tempRow= ltDbFetchRow(tempCursor);
						 }
					   ltDbCloseCursor(tempCursor);
				}
			
		   memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"%sFormrecordToexcel %s search",JAVACMD,caTempDir);
		   close(fd);
			 chdir(BINCMD);
			 system(caLine);
			 
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
			 ltMsgFree(ltMsgPk);
			 
			 return 0;
			      
		}
   
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select b.userid,a.dispname,a.username,b.sdate,b.stime,b.formfile,b.formhost,b.formurl,b.formasize,b.formsize,b.dip,b.mtype,b.rback,b.fstart,b.flen,c.name from %s b left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %ld offset %ld",sqltablename,glevel,caWhere,lRowNum,lStartRec);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
		       		  lNum++;
				   		  char formasize[32];
				   		  char formsize[32];		   		  
				   		  char gname[512];
				   		  char cDatetime[128];
				 
				   		  int  pid;
				       	int  i;
				       	pid=0;
				       	memset(gname,0,sizeof(gname));
				   		  memset(formasize,0,sizeof(formasize));
				   		  memset(formsize,0,sizeof(formsize));
				   		  memset(cDatetime,0,sizeof(cDatetime));
				   		  if(atol(tempRow[8])>1024000){
		               sprintf(formasize,"%0.3fM",atol(tempRow[8])/1024000.00);
		            }else{
		               sprintf(formasize,"%0.3fK",atol(tempRow[8])/1024.00);
		            }
		
				   		  if(atol(tempRow[9])>1024000){
		               sprintf(formsize,"%0.3fM",atol(tempRow[9])/1024000.00);
		            }else{
		               sprintf(formsize,"%0.3fK",atol(tempRow[9])/1024.00);
		            }
          
          	   if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[15]);
		           }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		           }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		           }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		           }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[15]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  
		           }		          
	   		   
	   		   sprintf(cDatetime,"%s %s",tempRow[3],tempRow[4]);
            
       		if(lNum==1){
	     	  	 sprintf(caTmpp,"{\"id\":\"%ld\",\"uid\":\"%s\",\"uname\":\"%s\",\"groupname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myformfile\":\"%s\",\"myformhost\":\"%s\",\"myformurl\":\"%s\",\"myformasize\":\"%s\",\"myformsize\":\"%s\",\"mydip\":\"%s\",\"mymtype\":\"%s\",\"myrback\":\"%s\",\"myfstart\":\"%s\",\"myflen\":\"%s\",\"datetime\":\"%s\"}",
	     	  	 	lNum,tempRow[0],tempRow[1],gname,tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],cDatetime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"id\":\"%ld\",\"uid\":\"%s\",\"uname\":\"%s\",\"groupname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myformfile\":\"%s\",\"myformhost\":\"%s\",\"myformurl\":\"%s\",\"myformasize\":\"%s\",\"myformsize\":\"%s\",\"mydip\":\"%s\",\"mymtype\":\"%s\",\"myrback\":\"%s\",\"myfstart\":\"%s\",\"myflen\":\"%s\",\"datetime\":\"%s\"}",
	     	  	 	lNum,tempRow[0],tempRow[1],gname,tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],formasize,formsize,tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],cDatetime);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
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


/*
MSA:数据中心
				POP3,SMTP邮件
功能:邮件全文检索
*/
int ltmsamail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;

  char strBuf[512];
  char fWhere[64];
  char *gid;
  char *uid;
  char *flags;  //0发送1接收
  char *rid;    //规则id
  char *subject;
  char *dip;
  char *from;   //发件人
  char *to;     //收件人
  char *date;   
  char *btype;  //邮件大小
  char *bbyte;
  char *fbtype; //附件
  char *fbbyte;
  char *osort;  //排序方式
  char *orderby;//升降
  char *mailtype;
  int Num=0;

  char *dbUser;
  char *dbPass;
  char *dbName;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
		fprintf(stderr,"db connect error\n");
	}
  
  gid=ltMsgGetVar_s(ltMsgPk,"gid");
  uid=ltMsgGetVar_s(ltMsgPk,"uid");
  flags=ltMsgGetVar_s(ltMsgPk,"flags");
  rid=ltMsgGetVar_s(ltMsgPk,"rid");
  subject=ltMsgGetVar_s(ltMsgPk,"subject");
  dip=ltMsgGetVar_s(ltMsgPk,"dip");
  from=ltMsgGetVar_s(ltMsgPk,"from");
  to=ltMsgGetVar_s(ltMsgPk,"to");
  date=ltMsgGetVar_s(ltMsgPk,"date");
  btype=ltMsgGetVar_s(ltMsgPk,"btype");
  bbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
  fbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
  fbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
	orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
  mailtype=ltMsgGetVar_s(ltMsgPk,"mailtype");//web or pop3

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  if(flags){//类型
		sprintf(fWhere,"tse%s",flags);
		lt_dbput_rootvars(dbPtr,1,fWhere," selected");
	}
  //分类
  memset(strBuf,0,sizeof(strBuf));
  sprintf(strBuf,"select rid,rname from msamailfilter");
	tablePtr=lt_dbput_table(dbPtr,"mtypelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
			Num++;		
			if(Num==1){
       	lt_dbput_recordvars(tablePtr,3,"rid",LT_TYPE_STRING,tempRow[0],"rname",LT_TYPE_STRING,tempRow[1]);
			}else{
        lt_dbput_recordvars(tablePtr,3,"rid",LT_TYPE_STRING,tempRow[0],"rname",LT_TYPE_STRING,tempRow[1]);
			}
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);




  memset(strBuf,0,sizeof(strBuf));
  Num=0;
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=%d",atoi(mailtype));
	tablePtr=lt_dbput_table(dbPtr,"datelist");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
			Num++;
			char tempDate[32];
			char ToDate[32];
			memset(tempDate,0,sizeof(tempDate));
			memset(ToDate,0,sizeof(ToDate));
			sprintf(tempDate,"%s",tempRow[0]);
			ToDate[0]=tempDate[0];
			ToDate[1]=tempDate[1];
			ToDate[2]=tempDate[2];
			ToDate[3]=tempDate[3];
			ToDate[4]='-';
			ToDate[5]=tempDate[4];
			ToDate[6]=tempDate[5];
			ToDate[7]='-';
			ToDate[8]=tempDate[6];
			ToDate[9]=tempDate[7];
			ToDate[10]='\0';
			if(Num==1){
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],
       "strdate",LT_TYPE_STRING,ToDate);
			}else{
       lt_dbput_recordvars(tablePtr,3,
       "dayname",LT_TYPE_STRING,tempRow[0],
       "strdate",LT_TYPE_STRING,ToDate);
			}
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);

	lt_dbput_rootvars(dbPtr,1,"gid",gid);
	lt_dbput_rootvars(dbPtr,1,"uid",uid);
	lt_dbput_rootvars(dbPtr,1,"myflags",flags);
	lt_dbput_rootvars(dbPtr,1,"mymtype",rid);
	lt_dbput_rootvars(dbPtr,1,"mysubject",subject);
	lt_dbput_rootvars(dbPtr,1,"mydip",dip);
	lt_dbput_rootvars(dbPtr,1,"myfrom",from);
	lt_dbput_rootvars(dbPtr,1,"myto",to);
	lt_dbput_rootvars(dbPtr,1,"mytabname",date);
	lt_dbput_rootvars(dbPtr,1,"mybtype",btype);
	lt_dbput_rootvars(dbPtr,1,"mybtype",bbyte);
	lt_dbput_rootvars(dbPtr,1,"myfbtype",fbtype);
	lt_dbput_rootvars(dbPtr,1,"myfbtype",fbbyte);
	lt_dbput_rootvars(dbPtr,1,"myosort",osort);
	lt_dbput_rootvars(dbPtr,1,"myorderby",orderby);
	lt_dbput_rootvars(dbPtr,1,"mytabtype",mailtype);

  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/allmailsearch.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
  return 0;

}

/*
MSA:数据中心
				邮件检索（pop3 and web）
功能:邮件记录查询,记录删除,记录导出HTML
*/
int ltmsamailquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  ltDbHeadPtr dbPtr;
	  ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;
    char sqlBuf[1024],strBuf[1024];
    char caWhere[512],delWhere[512];
    char *caUser=NULL; 
    char *gid=NULL;
    char *caFlag=NULL;
    char *caMtype=NULL;  
    char *caSubject=NULL;
    char *caDip=NULL;
    char *caFrom=NULL;
    char *caTo=NULL;
    char *tabname=NULL; 
    char *caBtype=NULL;
    char *caBbyte=NULL;
    char *caFbtype=NULL;
    char *caFbbyte=NULL;
    char *caOrder=NULL;
    char *caOrderby=NULL;
    char *m_action=NULL;
    char *tabtype=NULL;
    char caTmpp[2048];
    char sqltablename[64];
    stralloc strTemp; 
    int  glevel,gpid;
	  char *dbUser;
	  char *dbPass;
	  char *dbName;
	  
	  memset(strBuf,0,sizeof(strBuf));
	  memset(caWhere,0,sizeof(caWhere));
	  memset(delWhere,0,sizeof(delWhere));
	  memset(sqltablename,0,sizeof(sqltablename));
	  glevel=0;
	  gpid=0;
	  strTemp.s=0;   
	  
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
       return 0;
		} 
		   
    dbPtr=lt_dbinit();   	 
    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    if(m_action==NULL){
     	iAction=0;
    }else{
      iAction=atol(m_action);
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"groupid")){
    	 gid=ltMsgGetVar_s(ltMsgPk,"groupid");
    }
        
    if(ltMsgGetVar_s(ltMsgPk,"user")){
    	 caUser=ltMsgGetVar_s(ltMsgPk,"user");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"flags")){
    	caFlag=ltMsgGetVar_s(ltMsgPk,"flags");
    }

    if(ltMsgGetVar_s(ltMsgPk,"mtype")){
    	 caMtype=ltMsgGetVar_s(ltMsgPk,"mtype");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"subject")){
    	 caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
    }

    if(ltMsgGetVar_s(ltMsgPk,"dip")){
    	 caDip=ltMsgGetVar_s(ltMsgPk,"dip");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"from")){
    	 caFrom=ltMsgGetVar_s(ltMsgPk,"from");
    }
   
    if(ltMsgGetVar_s(ltMsgPk,"to")){
    	 caTo=ltMsgGetVar_s(ltMsgPk,"to");
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabname")){					//日期
    	tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
    }
     
    if(ltMsgGetVar_s(ltMsgPk,"btype")){
    	caBtype=ltMsgGetVar_s(ltMsgPk,"btype");
    }

    if(ltMsgGetVar_s(ltMsgPk,"bbyte")){
    	caBbyte=ltMsgGetVar_s(ltMsgPk,"bbyte");
    }

   
    if(ltMsgGetVar_s(ltMsgPk,"fbtype")){
    	caFbtype=ltMsgGetVar_s(ltMsgPk,"fbtype");
    }
  
    if(ltMsgGetVar_s(ltMsgPk,"fbbyte")){
    	 caFbbyte=ltMsgGetVar_s(ltMsgPk,"fbbyte");
    }
 
              
    if(ltMsgGetVar_s(ltMsgPk,"osort")){
    	caOrder=ltMsgGetVar_s(ltMsgPk,"osort");      	//排序方式
    }          
    
    if(ltMsgGetVar_s(ltMsgPk,"orderby")){
    	caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby"); 	//升 降
    }
    
    if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
    	tabtype=ltMsgGetVar_s(ltMsgPk,"tabtype");   	//表类型
    }
        

		  
		lRowNum=100; /*每页的行数*/
		lSumRec=0;   /*总行数*/
		lCount=0;
		lStartRec=0;
		lNum=0;
	
		
		if(ltMsgGetVar_s(ltMsgPk,"limit")){ 
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){ 
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}

		sprintf(strBuf,"select level,name,pid from msagroup where id=%s",gid); /*获取部门管理级别*/
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				gpid=atol(tempRow[2]);
			}
		}
		ltDbCloseCursor(tempCursor);

		if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			   sprintf(caWhere,"%s","2=2");//查找整个部门用户
			   sprintf(delWhere,"%s","2=2");//查找整个部门用户
			}
		}

		if(strcmp(gid,"-1")!=0){//不等于-1
			if(strcmp(caUser,"-1")==0){
			    sprintf(caWhere,"a.groupid%d=%s",glevel,gid);//查找整个部门用户
			    sprintf(delWhere,"2=2");
			}else{
					sprintf(caWhere,"b.userid=%s and a.groupid%d=%s",caUser,glevel,gid);
					sprintf(delWhere,"userid = %s",caUser);
		  }
		}     
			

   if(strcmp(caFlag,"-1")==0){						/*全部邮件*/
   		sprintf(caWhere,"%s and (b.flags<2)",caWhere);
   	  sprintf(delWhere,"%s and (flags<2)",delWhere);
   }else{
   		sprintf(caWhere,"%s and (b.flags=%s)",caWhere,caFlag);
   		sprintf(delWhere,"%s and (flags=%s)",delWhere,caFlag);
   }
   

	 if(strcmp(caMtype,"-1")!=0){						/*邮件分类*/
	 	 sprintf(strBuf,"select rname from msamailfilter where rid=%s",caMtype);
		 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		 if(tempCursor!=NULL){
			  tempRow=ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
			   sprintf(caWhere, "%s and (b.mtype='%s') ",caWhere,tempRow[0]);
			   sprintf(delWhere,"%s and (mtype='%s')",delWhere,tempRow[0]);
			  }
		 }
		 ltDbCloseCursor(tempCursor);	 
	 }
  
   if(caSubject!=NULL){
	    if(strlen(caSubject)>0) {
	        sprintf(caWhere,"%s and (b.mailsub like '%c%s%c')",caWhere,'%',caSubject,'%');
	        sprintf(delWhere,"%s and (mailsub like '%c%s%c')",delWhere,'%',caSubject,'%');
	    }
   }
    
   if(caDip!=NULL){
	    if(strlen(caDip)>0) {
	        sprintf(caWhere,"%s and (b.dip = '%s')",caWhere,caDip);
	        sprintf(delWhere,"%s and (dip = '%s')",delWhere,caDip);
	    }
   }
    
   if(caFrom!=NULL){
	    if(strlen(caFrom)>0) {
	       sprintf(caWhere,"%s and (b.mailfrom='%s')",caWhere,caFrom);
	       sprintf(delWhere,"%s and (mailfrom='%s')",delWhere,caFrom);
	    }
   }
    
   if(caTo!=NULL){
	    if(strlen(caTo)>0) {
	        sprintf(caWhere,"%s and (b.mailto='%s')",caWhere,caTo);
	        sprintf(delWhere,"%s and (mailto='%s')",delWhere,caTo);
	    }
   }


   if(strcmp(caBtype,"unit2") == 0) { /* 邮件大小*/
    	   sprintf(caWhere,"%s and (b.mailsize >%s)",caWhere,caBbyte);
    	   sprintf(delWhere,"%s and (mailsize >%s)",delWhere,caBbyte);
   }
    
   if(strcmp(caFbtype,"unit2") == 0) { /* 附件大小*/
    	  sprintf(caWhere,"%s and (b.mailasize >%s)",caWhere,caFbbyte);
    	  sprintf(delWhere,"%s and (mailasize >%s)",delWhere,caFbbyte);
   }
   
   if(strlen(tabtype)>0){
   	    sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=%s and sdate='%s'",tabtype,tabname);      
				tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
				if(tempCursor!=NULL){
					  tempRow=ltDbFetchRow(tempCursor);
					  if(tempRow!=NULL){
					    sprintf(sqltablename,"%s",tempRow[1]);	          
					  }
				}			
				ltDbCloseCursor(tempCursor);	
   }
  
	 sprintf(strBuf,"select count(*) from %s b left join msauser a on a.userid=b.userid left join msagroup c on a.groupid%d=c.id where %s",sqltablename,glevel,caWhere);      
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	 if(tempCursor!=NULL){
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL){
		   lCount=atol(tempRow[0]);
		  }
	 }
	 ltDbCloseCursor(tempCursor);	 

	 if(caOrder!=NULL){
		    if( strlen(caOrder)>0 ){
				    	if(strcmp(caOrder,"nosort")!=0){
						        if(strcmp(caOrder,"date")==0){
						            sprintf(caWhere,"%s order by b.sdate ",caWhere);
						        }else if(strcmp(caOrder,"from") == 0){
						            sprintf(caWhere,"%s order by b.mailfrom ",caWhere);
						        }else if(strcmp(caOrder,"to") == 0){
						            sprintf(caWhere,"%s order by b.mailto ",caWhere);
						        }else if(strcmp(caOrder,"dip") == 0){
												sprintf(caWhere,"%s order by b.dip ",caWhere);
							    	}else if(strcmp(caOrder,"size") == 0){
						            sprintf(caWhere,"%s order by b.mailsize ",caWhere);
						        }else if(strcmp(caOrder,"user") == 0){
						            sprintf(caWhere,"%s order by b.userid ",caWhere);
						        }
						        
						        if(strlen(caOrderby)){
							          if(caOrderby[0] == '1'){   		            	  
							              sprintf(caWhere,"%s desc",caWhere);
							          }else{
							          	  sprintf(caWhere,"%s asc",caWhere);
							          }
									  }
				    }
		   }   
   }	
  
    
    /*记录删除*/
		if(iAction==1){
					if(strlen(delWhere)>0){
						sprintf(sqlBuf,"delete from %s where %s ",sqltablename,delWhere);
					  ltDbExecSql(G_DbCon,sqlBuf);  
				  }
			
				  ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
					ltMsgFree(ltMsgPk);
					
					return 0;		
		}

    
		/*记录导出HTML*/
		if(iAction==2){
			      char xHead[1024];
			      char *lt_page_content=NULL;
			      char *ppp;
				    int  iii;
				    memset(xHead,0,sizeof(xHead));
				    dbPtr=lt_dbinit();   		    					    			        
				    sprintf(xHead,"%s","Content-disposition: attachment; filename=\"mailexport.htm\"\nContent-type: application/octet-stream\nContent-transfer-encoding:BINARY\n\n");		    
			      lt_dbput_rootvars(dbPtr,1,"ipadd",_ltPubInfo->_msaOptionHead.strAuthIP  );//ip	   		      			       
		      	lt_dbput_rootvars(dbPtr,1,"gid",gid);
						lt_dbput_rootvars(dbPtr,1,"uid",caUser);
						lt_dbput_rootvars(dbPtr,1,"myflags",caFlag);
						lt_dbput_rootvars(dbPtr,1,"mymtype",caMtype);
						lt_dbput_rootvars(dbPtr,1,"mysubject",caSubject);
						lt_dbput_rootvars(dbPtr,1,"mydip",caDip);
						lt_dbput_rootvars(dbPtr,1,"myfrom",caFrom);
						lt_dbput_rootvars(dbPtr,1,"myto",caTo);
						lt_dbput_rootvars(dbPtr,1,"mytabname",tabname);
						lt_dbput_rootvars(dbPtr,1,"mybtype",caBtype);
						lt_dbput_rootvars(dbPtr,1,"mybtype",caBbyte);
						lt_dbput_rootvars(dbPtr,1,"myfbtype",caFbtype);
						lt_dbput_rootvars(dbPtr,1,"myfbtype",caFbbyte);
						lt_dbput_rootvars(dbPtr,1,"myosort",caOrder);
						lt_dbput_rootvars(dbPtr,1,"myorderby",caOrderby);
						lt_dbput_rootvars(dbPtr,1,"mytabtype",tabtype);  
						
				    lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/datacenter/mailrecordexport.htm",dbPtr->doc,0);
				    
				    if(lt_page_content==NULL){
				     	return -1;
				    }
				    iii=0;
				    ppp=lt_page_content;
				    for(iii=0;iii<30;iii++){
				    	if(*ppp!='<'){
				    		*ppp=' ';
				    	}else{
				    		break;
				      }
				    }
			
				    ppp=strrchr(lt_page_content,'>');
			      if(ppp){
			   		  *(ppp+1)='\0';
			      }

				  ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,xHead,"lthtml",LT_TYPE_STRING,lt_page_content);
				  free(lt_page_content);		    
			    ltMsgFree(ltMsgPk);
			    lt_dbfree(dbPtr);
							
				  return 0; 
				
			      
		}
		
		if(iAction==3){
			    char caFile[256];
			    char tmpdir[128];
			    char caTempDir[512];
			    char caLine[2048];
			    int fd=0;
	    	  int flag=0;
		   		memset(caFile,0,sizeof(caFile));

					memset(tmpdir,0,sizeof(tmpdir)); 
			    sprintf(tmpdir,"%lu",ltStrGetId());
			    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
					if(ltFileIsExist(caTempDir)!=1){
						if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
							ltMsgPk->msgpktype=1;
						  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
						  ltMsgFree(ltMsgPk);
						  return 0;
						}
					}
					
		 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
				  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
				  if(fd == (-1)) {
						 ltMsgPk->msgpktype=1;
						 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
						 ltMsgFree(ltMsgPk);
						 close(fd);				 
						 return 0;
					} 
				  
				  
				  sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.stime,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,c.name,b.rback,b.fstart,b.flen from %s b left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %ld offset %ld",sqltablename,glevel,caWhere,lRowNum,lStartRec);
				  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				  if(tempCursor!=NULL){
						 tempRow=ltDbFetchRow(tempCursor);
					   while(tempRow!=NULL){	
					   	    flag++;		   					   	    
						   	  char mailaSize[128];//附件
					   		  char mailSize[128];//邮件
					   		  char gname[512];
					   		  char myflags[32];
					   		  int  pid;
					       	int  i;
					       	pid=0;   
					       	memset(gname,0,sizeof(gname));
					       	memset(myflags,0,sizeof(myflags));
					   		  memset(mailaSize,0,sizeof(mailaSize));
					   		  memset(mailSize,0,sizeof(mailSize));  
					   		  if(atol(tempRow[7])>1024000){					   		  	    
			               sprintf(mailaSize,"%0.3fM",atol(tempRow[7])/1024000.00);		              
			            }else{		            	 
			               sprintf(mailaSize,"%0.3fK",atol(tempRow[7])/1024.00);
			            }
					   		  if(atol(tempRow[8])>1024000){
			               sprintf(mailSize,"%0.3fM",atol(tempRow[8])/1024000.00);
			            }else{
			               sprintf(mailSize,"%0.3fK",atol(tempRow[8])/1024.00);
			            } 
						   	   
					   	    if(glevel==0){
			             	 sprintf(gname,"%s",tempRow[13]);
			             }else if(glevel==1){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[13]);
			             	  	  	break;
			             	  	  }
			             	  }   
		   	  
			             }else if(glevel==2){
			               	for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[13]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	           	  
			             }else if(glevel==3){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[13]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	
			             }else if(glevel==4){
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[13]);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
			             	  	  	break;
			             	  	  }
			             	  }	
			             	  
			             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
			             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
			             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
			             	  	  	break;
			             	  	  }
			             	  }	             	  
			             }
			          
				        if(atol(tempRow[9])==0){
			              sprintf(myflags,"%s","POP3");
		   		  		}
		   		  		if(atol(tempRow[9])==1){
		   		  				sprintf(myflags,"%s","SMTP");
		   		  		}
					   	   
							  sprintf(caLine,"%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n",
							    flag,
							    gname,//GroupName
									tempRow[0],//dispname
									tempRow[1],//mailfile
									tempRow[4],//mailsub
									tempRow[2],//imfrom
									tempRow[3],//imto
									tempRow[11],//dip
									tempRow[5],//发送日期
									tempRow[6],//发送时间
									mailSize, //邮件大小
									mailaSize,//附件大小
									myflags,//邮件状态
									tempRow[10]//邮件分类				
									);
							 write(fd,caLine,strlen(caLine));
							 tempRow= ltDbFetchRow(tempCursor);
						 }
					   ltDbCloseCursor(tempCursor);
				}
			
		   memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"%sMailrecordToexcel %s search",JAVACMD,caTempDir);
			 chdir(BINCMD);
			 system(caLine);
			 
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
			 ltMsgFree(ltMsgPk);
			 
			 return 0;
			      
		}
	
	  //json构造
		sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
	  stralloc_cats(&strTemp,caTmpp);  
	  sprintf(sqlBuf,"select a.dispname,b.mailfile,b.mailfrom,b.mailto,b.mailsub,b.sdate,b.mailasize,b.mailsize,b.flags,b.mtype,b.dip,a.userid,c.name,b.stime,b.rback,b.fstart,b.flen from %s b left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %ld offset %ld",sqltablename,glevel,caWhere,lRowNum,lStartRec);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			   tempRow=ltDbFetchRow(tempCursor);
			   	while(tempRow!=NULL){
			   		  lNum++;		   		  
			   		  char mailaSize[32];//附件
			   		  char mailSize[32];//邮件
			   		  char gname[512];
			   		  char myflags[32];
			   		  char cDatetime[128];
			   		  int  pid;
			       	int  i;
			       	pid=0;
			       	memset(myflags,0,sizeof(myflags));
			       	memset(gname,0,sizeof(gname));
			   		  memset(mailaSize,0,sizeof(mailaSize));
			   		  memset(mailSize,0,sizeof(mailSize));
			   		  memset(cDatetime,0,sizeof(cDatetime));
			   		  if(atol(tempRow[6])>1024000){
	               sprintf(mailaSize,"%0.3fM",atol(tempRow[6])/1024000.00);
	            }else{
	               sprintf(mailaSize,"%0.3fK",atol(tempRow[6])/1024.00);
	            }
	            
			   		  if(atol(tempRow[7])>1024000){
	               sprintf(mailSize,"%0.3fM",atol(tempRow[7])/1024000.00);
	            }else{
	               sprintf(mailSize,"%0.3fK",atol(tempRow[7])/1024.00);
	            }
	            
	             if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[12]);
		           }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[12]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		           }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[12]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		           }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[12]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		           }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[12]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }		             	  
		           }		          
	   		  		  
	   		  		if(atol(tempRow[8])==0){
		              sprintf(myflags,"%s","POP3");
	   		  		}
	   		  		if(atol(tempRow[8])==1){
	   		  				sprintf(myflags,"%s","SMTP");
	   		  		}
	   		  		
	   		  		
						  sprintf(cDatetime,"%s %s",tempRow[5],tempRow[13]);
			   		  		
	   		  		  
						  if(lNum==1){
								sprintf(caTmpp,"{\"id\":\"%ld\",\"uname\":\"%s\",\"uid\":\"%s\",\"groupname\":\"%s\",\"mymailfile\":\"%s\",\"mymailfrom\":\"%s\",\"mymailto\":\"%s\",\"mymailsub\":\"%s\",\"mysdate\":\"%s\",\"mymailasize\":\"%s\",\"mymailsize\":\"%s\",\"myflags\":\"%s\",\"mymtype\":\"%s\",\"myip\":\"%s\",\"mystime\":\"%s\",\"myrback\":\"%s\",\"myfstart\":\"%s\",\"myflen\":\"%s\",\"datetime\":\"%s\"}",
								lNum,tempRow[0],tempRow[11],gname,tempRow[1],tempRow[2],tempRow[3],tempRow[4],
								tempRow[5],mailaSize,mailSize,myflags,tempRow[9],tempRow[10],tempRow[13],tempRow[14],tempRow[15],tempRow[16],cDatetime);
							  stralloc_cats(&strTemp,caTmpp);					
							}else{
								sprintf(caTmpp,",{\"id\":\"%ld\",\"uname\":\"%s\",\"uid\":\"%s\",\"groupname\":\"%s\",\"mymailfile\":\"%s\",\"mymailfrom\":\"%s\",\"mymailto\":\"%s\",\"mymailsub\":\"%s\",\"mysdate\":\"%s\",\"mymailasize\":\"%s\",\"mymailsize\":\"%s\",\"myflags\":\"%s\",\"mymtype\":\"%s\",\"myip\":\"%s\",\"mystime\":\"%s\",\"myrback\":\"%s\",\"myfstart\":\"%s\",\"myflen\":\"%s\",\"datetime\":\"%s\"}",
								lNum,tempRow[0],tempRow[11],gname,tempRow[1],tempRow[2],tempRow[3],tempRow[4],
								tempRow[5],mailaSize,mailSize,myflags,tempRow[9],tempRow[10],tempRow[13],tempRow[14],tempRow[15],tempRow[16],cDatetime);
								stralloc_cats(&strTemp,caTmpp);
							}						
							tempRow=ltDbFetchRow(tempCursor);						
				  }
			 ltDbCloseCursor(tempCursor);
		}
		stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
		
				
		ltMsgPk->msgpktype=1;		
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json;charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);	
		ltMsgFree(ltMsgPk);
			
		return 0; 
}



/*
MSA:数据中心
功能:虚拟身份记录页面链接
*/
int ltmsavcloglink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltTablePtr  tablePtr;
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  char strBuf[512];
  char *gid=NULL;
  char *uid=NULL;
  char *vctype=NULL;
  char *vccon=NULL;
  char *sdate=NULL;   
  char *osort=NULL;  
  char *orderby=NULL;
  char tempDate[32];
	char ToDate[32];
	char caSel[32];
	int  Num=0;
	
	memset(tempDate,0,sizeof(tempDate));
	memset(ToDate,0,sizeof(ToDate));
	memset(strBuf,0,sizeof(strBuf));
  memset(caSel,0,sizeof(caSel));
  

  char *dbUser;
  char *dbPass;
  char *dbName;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon!=NULL){
		printf("db connect ok\n");
	}else{
		fprintf(stderr,"db connect error\n");
	}
  
  gid=ltMsgGetVar_s(ltMsgPk,"groupid");
  uid=ltMsgGetVar_s(ltMsgPk,"uid");
  vctype=ltMsgGetVar_s(ltMsgPk,"vctype");
  sdate=ltMsgGetVar_s(ltMsgPk,"date");
  vccon=ltMsgGetVar_s(ltMsgPk,"vccon");
  osort=ltMsgGetVar_s(ltMsgPk,"osort");
	orderby=ltMsgGetVar_s(ltMsgPk,"orderby");
	
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	if(vctype!=NULL){
  	sprintf(caSel,"tse%s",vctype);
  	lt_dbput_rootvars(dbPtr,1,caSel,"selected");
  }
	
  sprintf(strBuf,"select sdate,tabname from msaloglist where tabtype=2 order by sdate desc");
	tablePtr=lt_dbput_table(dbPtr,"datelist");	
	ToDate[0]=sdate[0];
	ToDate[1]=sdate[1];
	ToDate[2]=sdate[2];
	ToDate[3]=sdate[3];
	ToDate[4]='-';
	ToDate[5]=sdate[4];
	ToDate[6]=sdate[5];
	ToDate[7]='-';
	ToDate[8]=sdate[6];
	ToDate[9]=sdate[7];
	ToDate[10]='\0';
	lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,sdate,"strdate",LT_TYPE_STRING,ToDate);
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
			 Num++;
			  memset(ToDate,0,sizeof(ToDate));
				sprintf(tempDate,"%s",tempRow[0]);
			 	ToDate[0]=tempDate[0];
				ToDate[1]=tempDate[1];
				ToDate[2]=tempDate[2];
				ToDate[3]=tempDate[3];
				ToDate[4]='-';
				ToDate[5]=tempDate[4];
				ToDate[6]=tempDate[5];
				ToDate[7]='-';
				ToDate[8]=tempDate[6];
				ToDate[9]=tempDate[7];
				ToDate[10]='\0';
					
			  if(strcmp(tempRow[0],sdate)!=0){
           lt_dbput_recordvars(tablePtr,2,"dayname",LT_TYPE_STRING,tempRow[0],"strdate",LT_TYPE_STRING,ToDate);
			  }
    tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);

	lt_dbput_rootvars(dbPtr,1,"gid",gid);
	lt_dbput_rootvars(dbPtr,1,"uid",uid);
	lt_dbput_rootvars(dbPtr,1,"myvctype",vctype);
	lt_dbput_rootvars(dbPtr,1,"myvccon",vccon);
	lt_dbput_rootvars(dbPtr,1,"mydate",sdate);
	lt_dbput_rootvars(dbPtr,1,"myosort",osort);
	lt_dbput_rootvars(dbPtr,1,"myorderby",orderby);

  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/msavclog.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	
  return 0;

}



/*
MSA:数据中心
			虚拟身份记录
功能:记录数据查询 导出 删除
*/
int ltmsavclogquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    long lRowNum,lStartRec,lCount,lSumRec,lNum;
    int  iAction=0;   
    char strBuf[1024];
    char delWhere[512]; //记录删除条件构造参数
    char caWhere[512];  //记录查询条件构造参数
    char *caSdate=NULL; //日期
    char *caVctype=NULL;  //虚拟身份类型参数
    char *caOrder=NULL;   //分组方式参数
    char *caOrderby=NULL; //排序方式参数
    char *caUser=NULL;    //用户ID参数
    char *caVccon=NULL;   //虚拟身份特征号参数
    char *gid=NULL;
    char *m_action=NULL;
    int  intGid,glevel,gpid;
    char caTmpp[2048];
    char tabname[256];     
    stralloc strTemp;
    
    /*导出记录java 传入参数*/
    char group_name[256];	//部门
    char user_name[256];	//用户
    char servic_name[256];//虚拟身份类型
    char key_name[256];		//虚拟身份特征号

    memset(group_name,0,sizeof(group_name));
	  memset(servic_name,0,sizeof(servic_name));
	  memset(key_name,0,sizeof(key_name));
	  memset(user_name,0,sizeof(user_name));

    strTemp.s=0;

		char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
		}

    m_action=ltMsgGetVar_s(ltMsgPk,"action");
    gid=ltMsgGetVar_s(ltMsgPk,"groupid");  
    caUser=ltMsgGetVar_s(ltMsgPk,"user"); 
    caVctype=ltMsgGetVar_s(ltMsgPk,"vctype");
    caVccon=ltMsgGetVar_s(ltMsgPk,"vccon");
    caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caOrder=ltMsgGetVar_s(ltMsgPk,"osort");
    caOrderby=ltMsgGetVar_s(ltMsgPk,"orderby");
    if(m_action==NULL){
      iAction=0;
    }else{
      iAction=atol(m_action);
    }
    sprintf(tabname,"msavclog%s",caSdate);
    sprintf(servic_name,"%s","-");
    if(gid==NULL){
    	intGid=0;
    }else{
     	intGid=atol(gid);
    }
  
    glevel=0;gpid=0;

		if(ltMsgGetVar_s(ltMsgPk,"limit")){
			lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
		}
		if(ltMsgGetVar_s(ltMsgPk,"start")){
			lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
		}

		lRowNum=100;
		lSumRec=0;
		lCount=0;
		lStartRec=0;
		lNum=0;

		memset(caWhere,0,sizeof(caWhere));
		memset(delWhere,0,sizeof(delWhere));
		
		/*获取部门管理级别*/
		sprintf(strBuf,"select level,pid,name from msagroup where id=%d",intGid);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow=ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				glevel=atol(tempRow[0]);
				gpid=atol(tempRow[1]);
				sprintf(group_name,"%s",tempRow[2]);
			}
		}
		ltDbCloseCursor(tempCursor);

    if(strcmp(gid,"-1")==0){
			if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"%s","2=2");//查询所有用户			      
			       sprintf(group_name,"%s","Allgroup");
				}
		}

		if(strcmp(gid,"-1")!=0){
			  if(strcmp(caUser,"-1")==0){
			       sprintf(caWhere,"a.groupid%d=%d",glevel,intGid);//查找整个部门用户
			       sprintf(delWhere,"%s","2=2");
			       sprintf(user_name,"%s","Allusers");
				}else{
					   sprintf(caWhere,"b.userid=%s and a.groupid%d=%d",caUser,glevel,intGid);//查询单个用户
						 sprintf(delWhere,"userid=%s",caUser);
				}
		}
		
		if(caVctype!=NULL){//通信类型
	    if(strcmp(caVctype,"0")!=0){//flag:0 表所有
	    		sprintf(caWhere,"%s and (b.vctype = '%s') ",caWhere,caVctype);
	        sprintf(delWhere,"%s and (vctype='%s') ",delWhere,caVctype);	
	    }
	  }
	  
    if(caVccon!=NULL){//关键字处理
	    if(strlen(caVccon)>0) {
	        sprintf(caWhere,"%s and (b.vccon like '%c%s%c') ",caWhere,'%',caVccon,'%');
	        sprintf(delWhere,"%s and (vccon like '%c%s%c') ",delWhere,'%',caVccon,'%');	 
	        sprintf(key_name,"%s",caVccon);      
	    }else{
	    	  sprintf(key_name,"%s","-");	//表示为空
	    }
    }
    
    
    

    if(iAction==2){						/*删除记录操作*/
					if(strlen(delWhere)>0){
					    sprintf(strBuf,"delete from %s where %s ",tabname,delWhere);
					    ltDbExecSql(G_DbCon,strBuf);
			    }
				  ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
					ltMsgFree(ltMsgPk);
					return 0;
    }else if(iAction==1){			/*导出记录参数*/
    	    char tmpdir[128];
    	    char caTempDir[256];
			    char vctype[200];
			    char sDate[36];			/*日期*/
			    char caFile[256];		/*文件路径参数*/
			    char caLine[2048];
			    char gname[512];
			    int  	fd=0;   				/*文件标记符*/
	    	  int  	flag=0;
		   		int  	pid=0;
		   		int  	i=0;
		   		long  lTime=0;
		   		
		   		memset(caFile,0,sizeof(caFile));
		   		memset(caFile,0,sizeof(caFile));
   	      memset(gname,0,sizeof(gname));
			    memset(sDate,0,sizeof(sDate));
			    
			    lTime = time(0);
				  nasTimeGetDate(sDate,lTime);
					if(caOrder!=NULL){
					    if(strlen(caOrder)>0){
							    	if(strcmp(caOrder,"nosort")!=0) {
									        if(strcmp(caOrder,"date")==0) {
									            sprintf(caWhere,"%s order by b.sdate ",caWhere);
									        }else if(strcmp(caOrder,"user") == 0) {
									            sprintf(caWhere,"%s order by b.userid ",caWhere);
									        }
									        if(strlen(caOrderby)) {
									            if(caOrderby[0] == '1') {
													       ltStrAddF(caWhere," desc");
									            }else{
									               ltStrAddF(caWhere," asc");
									            }
									        }
							     }
					    }
		 			}

 			
			memset(tmpdir,0,sizeof(tmpdir)); 
	    sprintf(tmpdir,"%lu",ltStrGetId());
	    sprintf(caTempDir,"%s/%s/",_TMPDIR,tmpdir);		    
			if(ltFileIsExist(caTempDir)!=1){
				if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
					ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				  ltMsgFree(ltMsgPk);
				  return 0;
				}
			}
			
 		 	sprintf(caFile,"%ssearch.csv",caTempDir);
		  fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		  if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 close(fd);				 
				 return 0;
			} 
			
			sprintf(strBuf,"select a.dispname,b.sdate,b.stime,b.vctype,b.vccon,b.ip,b.mac,c.name from %s b left join msauser a on a.userid=b.userid  left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
			tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			if(tempCursor!=NULL){
					 tempRow=ltDbFetchRow(tempCursor);
				   while(tempRow!=NULL){	
				   	    flag++;		   	   
				   	    if(glevel==0){
		             	 sprintf(gname,"%s",tempRow[7]);
		             }else if(glevel==1){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	break;
		             	  	  }
		             	  }   
	   	  
		             }else if(glevel==2){
		               	for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	           	  
		             }else if(glevel==3){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	
		             }else if(glevel==4){
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[7]);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	pid=_ltPubInfo->_GroupList[i].pid;
		             	  	  	break;
		             	  	  }
		             	  }	
		             	  
		             	  for(i=0;i<_MAX_ALLGRPNUM;i++){
		             	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
		             	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
		             	  	  	break;
		             	  	  }
		             	  }	             	  
		             }	
				   	   
				     /*虚拟身份类型 1smtp发件人 2smtp收件人 3pop3账号登陆 4qq账号登陆 5qq账号退出 6telnet登陆 7ftp账号登陆 7ftp账号退出 …*/
				      char cTmp[128];
						  memset(cTmp,0,sizeof(cTmp));
						  if(atol(tempRow[3])==1){						  	
						  	strgb2utf8("smtp发件人",cTmp,strlen("smtp发件人"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==2){
								strgb2utf8("smtp收件人",cTmp,strlen("smtp收件人"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==3){
								strgb2utf8("pop3账号登陆",cTmp,strlen("pop3账号登陆"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==4){
								strgb2utf8("qq账号登陆",cTmp,strlen("qq账号登陆"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==5){
								strgb2utf8("qq账号退出",cTmp,strlen("qq账号退出"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==6){
								strgb2utf8("telnet登陆",cTmp,strlen("telnet登陆"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==7){
								strgb2utf8("ftp账号登陆",cTmp,strlen("ftp账号登陆"));
								sprintf(vctype,"%s",cTmp);
							}else if(atol(tempRow[3])==8){
								strgb2utf8("ftp账号退出",cTmp,strlen("ftp账号退出"));
								sprintf(vctype,"%s",cTmp);
							}
							
							sprintf(servic_name,"%s",vctype);
						  sprintf(caLine,"%d|%s|%s|%s|%s|%s|%s|%s|%s\r\n",
						    flag,
						    gname,//GroupName
								tempRow[0],//dispname
								tempRow[1],//sdate
								tempRow[2],//stime
								vctype,
								tempRow[4],
								tempRow[5],
								tempRow[6]);
								
						 if(strcmp(user_name,"Allusers")!=0){//单个用户
								sprintf(user_name,"%s",tempRow[0]);
						 }
						 write(fd,caLine,strlen(caLine));
						 tempRow= ltDbFetchRow(tempCursor);
					 }
				   ltDbCloseCursor(tempCursor);
			}
			
		   memset(caLine,0,sizeof(caLine));
		   sprintf(caLine,"%sVclogToexcel %s %s %s %s %s %s search",JAVACMD,group_name,user_name,servic_name,sDate,key_name,caTempDir);
			 close(fd);
			 chdir(BINCMD);
			 system(caLine);
			 
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,tmpdir);
			 ltMsgFree(ltMsgPk);
			 
			 return 0;

    }
  	sprintf(strBuf,"select count(*) from %s b  left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s ",tabname,glevel,caWhere);
		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
		}
		ltDbCloseCursor(tempCursor);

    if(caOrder!=NULL){
			    if(strlen(caOrder)>0){
					    	if(strcmp(caOrder,"nosort")!=0) {
								        if(strcmp(caOrder,"date")==0){
								            sprintf(caWhere,"%s order by b.sdate ",caWhere);
								        }else if(strcmp(caOrder,"user") == 0) {
								            sprintf(caWhere,"%s order by b.userid ",caWhere);
								        }
								        if(strlen(caOrderby)) {
								            if(caOrderby[0] == '1'){
												       ltStrAddF(caWhere," desc");
								            }else{
								              ltStrAddF(caWhere," asc");
								            }
											  }
					        }
			   }
    }

	 sprintf(strBuf,"select a.dispname,b.sdate,b.stime,b.vctype,b.vccon,b.ip,b.mac,b.userid,c.name from %s b  left join msauser a on (a.userid =b.userid) left join msagroup c on a.groupid%d=c.id where %s limit %lu offset %lu",tabname,glevel,caWhere,lRowNum,lStartRec);
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
    	while(tempRow!=NULL){
	       char caUrl[32];
	       char gname[512];
	       char vctype[200];
		   	 int  pid;
		   	 int  i;
		   	 pid=0;
   	     memset(gname,0,sizeof(gname));
   	     memset(caUrl,0,sizeof(caUrl));
	       lNum++;       
				 if(glevel==0){
         	  sprintf(gname,"%s",tempRow[8]);
         }else if(glevel==1){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	break;
         	  	  }
         	  }   

         }else if(glevel==2){
           	for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	           	  
         }else if(glevel==3){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==2){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){//获得一级部门名字
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	
         }else if(glevel==4){
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==gpid&&_ltPubInfo->_GroupList[i].level==3){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,tempRow[8]);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==2){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==1){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	pid=_ltPubInfo->_GroupList[i].pid;
         	  	  	break;
         	  	  }
         	  }	
         	  
         	  for(i=0;i<_MAX_ALLGRPNUM;i++){
         	  	  if(_ltPubInfo->_GroupList[i].groupid==pid&&_ltPubInfo->_GroupList[i].level==0){
         	  	  	sprintf(gname,"%s/%s",_ltPubInfo->_GroupList[i].name,gname);
         	  	  	break;
         	  	  }
         	  }	             	  
         }	
        
				  /*虚拟身份类型 1smtp发件人 2smtp收件人 3pop3账号登陆 4qq账号登陆 5qq账号退出 6telnet登陆 7ftp账号登陆 7ftp账号退出 …*/
		      char cTmp[128];
				  memset(cTmp,0,sizeof(cTmp));

				  if(atol(tempRow[3])==1){						  	
				  	strgb2utf8("smtp发件人",cTmp,strlen("smtp发件人"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==2){
						strgb2utf8("smtp收件人",cTmp,strlen("smtp收件人"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==3){
						strgb2utf8("pop3账号登陆",cTmp,strlen("pop3账号登陆"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==4){
						strgb2utf8("qq账号登陆",cTmp,strlen("qq账号登陆"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==5){
						strgb2utf8("qq账号退出",cTmp,strlen("qq账号退出"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==6){
						strgb2utf8("telnet登陆",cTmp,strlen("telnet登陆"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==7){
						strgb2utf8("ftp账号登陆",cTmp,strlen("ftp账号登陆"));
						sprintf(vctype,"%s",cTmp);
					}else if(atol(tempRow[3])==8){
						strgb2utf8("ftp账号退出",cTmp,strlen("ftp账号退出"));
						sprintf(vctype,"%s",cTmp);
					}
					
					

        if(lNum==1){
					  sprintf(caTmpp,"{\"mygroup\":\"%s\",\"uid\":\"%s\",\"uname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myvctype\":\"%s\",\"myvccon\":\"%s\",\"myip\":\"%s\",\"mymac\":\"%s\"}",
					     gname,tempRow[7],tempRow[0],tempRow[1], tempRow[2],vctype,tempRow[4],tempRow[5],tempRow[6]);
						   stralloc_cats(&strTemp,caTmpp);
				}else{
					  sprintf(caTmpp,",{\"mygroup\":\"%s\",\"uid\":\"%s\",\"uname\":\"%s\",\"mysdate\":\"%s\",\"mystime\":\"%s\",\"myvctype\":\"%s\",\"myvccon\":\"%s\",\"myip\":\"%s\",\"mymac\":\"%s\"}",
					     gname,tempRow[7],tempRow[0],tempRow[1], tempRow[2],vctype,tempRow[4],tempRow[5],tempRow[6]);
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


/*
MSA:数据中心
邮件和表单内容显示函数
*/
int msaShowContent(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char  caTempFile[512];
  char  *msgcon=NULL;
  char  *sDate=NULL;
  char  *sTime=NULL;
  char  *sTartp=NULL;
  char  *caFile=NULL;
  char  *caType=NULL;
  char  newHeader[128];
	char  strSet[32];
	long  fstart=0;

	memset(strSet,0,sizeof(strSet));
	memset(newHeader,0,sizeof(newHeader));
	memset(caTempFile,0,sizeof(caTempFile));
	
	sDate=ltMsgGetVar_s(ltMsgPk,"sdate"); 
	sTime=ltMsgGetVar_s(ltMsgPk,"stime"); 
	sTartp=ltMsgGetVar_s(ltMsgPk,"fstart"); 
	caFile=ltMsgGetVar_s(ltMsgPk,"file"); 
	caType=ltMsgGetVar_s(ltMsgPk,"type"); 
	if(sTartp==NULL){
		fstart=0;
	}else{
	  fstart=atol(sTartp);	
	}
	
	sprintf(caTempFile,"%s/%s/maillog/maillog-%ld.dat",_datacenterdir,sDate,atol(sTime));
	if(strcmp(caType,"0")==0){
			msgcon=dbMsgPrintMail(caTempFile,fstart,strSet);
	}
  if(strcmp(caType,"1")==0){
			msgcon=dbMsgPrintForm(caTempFile,fstart,strSet);
	}
	
	 sprintf(newHeader,"Content-type: text/html; charset=%s\n",strSet);	
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,newHeader,"lthtml",LT_TYPE_STRING,msgcon);
	 ltMsgFree(ltMsgPk); 

  return 0;
}




