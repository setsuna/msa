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


//加载清空页面
int sysclean(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char *date=NULL;
	if(ltMsgGetVar_s(ltMsgPk,"date")){
		date=ltMsgGetVar_s(ltMsgPk,"date"); 
	}
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"date",date);
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/sysclean.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}



//清空数据提交函数
int msaCleanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{     
	                                                            
			long cachr1=0,cachr2=0,cachr3=0,cachr4=0,cachr5=0;
			long cachr6=0,cachr7=0,cachr8=0,cachr9=0,cachr10=0; 
			long cachr11=0,cachr12=0,cachr13=0,cachr14=0,cachr15=0; 
			long cachr16=0,cachr17=0,cachr18=0,cachr19=0,cachr20=0; 
			long cachr21=0,cachr22=0,cachr23=0,cachr24=0,cachr25=0,cachr26=0; 
			char strSql[2048];		                                       
			char *sdate=NULL;
			char caTempDir[512];
			char proIniDir[512];
			char caLabel[1024];
			ltDbCursor *tempCursor;
      LT_DBROW tempRow;  
			
			if(ltMsgGetVar_s(ltMsgPk,"chr1")){
				 cachr1=atol(ltMsgGetVar_s(ltMsgPk,"chr1")); 
			}
			
		  if(ltMsgGetVar_s(ltMsgPk,"chr2")){
				 cachr2=atol(ltMsgGetVar_s(ltMsgPk,"chr2")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr3")){
				 cachr3=atol(ltMsgGetVar_s(ltMsgPk,"chr3")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr4")){
				 cachr4=atol(ltMsgGetVar_s(ltMsgPk,"chr4")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr5")){
				 cachr5=atol(ltMsgGetVar_s(ltMsgPk,"chr5")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr6")){
				 cachr6=atol(ltMsgGetVar_s(ltMsgPk,"chr6")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr7")){
				 cachr7=atol(ltMsgGetVar_s(ltMsgPk,"chr7")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr8")){
				 cachr8=atol(ltMsgGetVar_s(ltMsgPk,"chr8")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr9")){
				 cachr9=atol(ltMsgGetVar_s(ltMsgPk,"chr9")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr10")){
				 cachr10=atol(ltMsgGetVar_s(ltMsgPk,"chr10")); 
			}
			
		  if(ltMsgGetVar_s(ltMsgPk,"chr11")){
				 cachr11=atol(ltMsgGetVar_s(ltMsgPk,"chr11")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr12")){
				 cachr12=atol(ltMsgGetVar_s(ltMsgPk,"chr12")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr13")){
				 cachr13=atol(ltMsgGetVar_s(ltMsgPk,"chr13")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr14")){
				 cachr14=atol(ltMsgGetVar_s(ltMsgPk,"chr14")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr15")){
				 cachr15=atol(ltMsgGetVar_s(ltMsgPk,"chr15")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr16")){
				 cachr16=atol(ltMsgGetVar_s(ltMsgPk,"chr16")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr17")){
				 cachr17=atol(ltMsgGetVar_s(ltMsgPk,"chr17")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr18")){
				 cachr18=atol(ltMsgGetVar_s(ltMsgPk,"chr18")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr19")){
				 cachr19=atol(ltMsgGetVar_s(ltMsgPk,"chr19")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr20")){
				 cachr20=atol(ltMsgGetVar_s(ltMsgPk,"chr20")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr21")){
				 cachr21=atol(ltMsgGetVar_s(ltMsgPk,"chr21")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr22")){
				 cachr22=atol(ltMsgGetVar_s(ltMsgPk,"chr22")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr23")){
				 cachr23=atol(ltMsgGetVar_s(ltMsgPk,"chr23")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr24")){
				 cachr24=atol(ltMsgGetVar_s(ltMsgPk,"chr24")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr25")){
				 cachr25=atol(ltMsgGetVar_s(ltMsgPk,"chr25")); 
			}
			
			if(ltMsgGetVar_s(ltMsgPk,"chr26")){
				 cachr26=atol(ltMsgGetVar_s(ltMsgPk,"chr26")); 
			}
			
			
      if(ltMsgGetVar_s(ltMsgPk,"sdate")){
      	sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
      }
      
			
		  ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	    
	    memset(proIniDir,0,sizeof(proIniDir));
	    sprintf(proIniDir,"/datacenter/msa/process.ini");	 	

			if(cachr1==1){ //数据中心									
   					if(strlen(sdate)>0){   
   						  //详细访问记录  目录文件 						     
   						  sprintf(caTempDir,"/datacenter/msa/httpsearch"); 
   						  if(ltFileIsExist(caTempDir)==1) {    	
						    	  if(chdir(caTempDir)==0){
						    	 	  sprintf(caLabel,"/bin/rm -rf *");
						    	 	  system(caLabel);									    	 	  
						    	 	  sprintf(caLabel,"echo 'httpsearch'> %s",proIniDir);
							        system(caLabel);
							        sleep(1);
				       
						    	  }  						 																  	    	 
						    } 				
   						    						  		  
   						  sprintf(caTempDir,"/datacenter/msa/ipblocksearch");
   						  if(ltFileIsExist(caTempDir)==1) {    	
						    	  if(chdir(caTempDir)==0){
						    	 	  sprintf(caLabel,"/bin/rm -rf *");
						    	 	  system(caLabel);									    	 	  
						    	 	  sprintf(caLabel,"echo 'ipblocksearch'> %s",proIniDir);
							        system(caLabel);
							        sleep(1);
				       
						    	  }  						 																  	    	 
						    }
   						  
   						  
   						  sprintf(caTempDir,"/datacenter/msa/ipsearch");
						    if(ltFileIsExist(caTempDir)==1) {    	
						    	  if(chdir(caTempDir)==0){
						    	 	  sprintf(caLabel,"/bin/rm -rf *");
						    	 	  system(caLabel);									    	 	  
						    	 	  sprintf(caLabel,"echo 'ipsearch'> %s",proIniDir);
							        system(caLabel);
							        sleep(1);
				       
						    	  }  						 																  	    	 
						    }
						    
						    
                  
   						   
   						 
   						  //每天访问汇总记录
   						     /*服务访问汇总记录*/
   						    memset(strSql,0,sizeof(strSql));
   						    sprintf(strSql,"delete from msasrvreport where sdate<'%s'",sdate);
									ltDbExecSql(G_DbCon,strSql);
							   	sprintf(caLabel,"echo 'msasrvreport'>%s",proIniDir);
									system(caLabel);
									sleep(1);									
								    /*HTTP汇总记录*/  
								  memset(strSql,0,sizeof(strSql));  
								  sprintf(strSql,"delete from msahttpreport where sdate<'%s'",sdate);
									ltDbExecSql(G_DbCon,strSql);
							   	sprintf(caLabel,"echo 'msahttpreport'>%s",proIniDir);
									system(caLabel);
									sleep(1);
   						   
   						   
   						   
   						   
   						   //即时通信
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"delete from msaimlog where sdate<'%s'",sdate);
								  ltDbExecSql(G_DbCon,strSql);
   					   	  sprintf(caLabel,"echo 'msaimlog'>%s",proIniDir);
								  system(caLabel);
								  sleep(1);
   						      						     						   
   						   
   						   //文件传输
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"select tabname from msafileloglist where sdate<'%s'",sdate);
								  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
								  if(tempCursor!=NULL){
										   tempRow=ltDbFetchRow(tempCursor);
										   if(tempRow!=NULL) {
										       memset(strSql,0,sizeof(strSql));              
                           sprintf(strSql,"delete from %s",tempRow[0]);
                           ltDbExecSql(G_DbCon,strSql);
					   					   	 sprintf(caLabel,"echo '%s'> %s",tempRow[0],proIniDir);
													 system(caLabel);
													 sleep(1);
										   }
									}
								  ltDbCloseCursor(tempCursor);				  
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"delete from msafileloglist where sdate<'%s'",sdate);
								  ltDbExecSql(G_DbCon,strSql);
   					   	  sprintf(caLabel,"echo 'msafileloglist'> %s",proIniDir);
								  system(caLabel);
								  sleep(1);
   						   
   						   
   						   
   						   //POP3/SMTP邮件
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"select tabname from msamailloglist where sdate<'%s'",sdate);
								  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
								  if(tempCursor!=NULL){
										   tempRow=ltDbFetchRow(tempCursor);
										    if(tempRow!=NULL) {                
                           sprintf(strSql,"delete from %s",tempRow[0]);
                           ltDbExecSql(G_DbCon,strSql);
					   					   	 sprintf(caLabel,"echo '%s'> %s",tempRow[0],proIniDir);
													 system(caLabel);
													 sleep(1);
										   }
									}
								  ltDbCloseCursor(tempCursor);
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"delete from msamailloglist where sdate<'%s'",sdate);
								  ltDbExecSql(G_DbCon,strSql);
   					   	  sprintf(caLabel,"echo 'msamailloglist'> %s",proIniDir);
								  system(caLabel);
								  sleep(1);
   						   		   
   						   
   						   //WEB邮件
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"select tabname from msawebmailloglist where sdate<'%s'",sdate);
								  tempCursor=ltDbOpenCursor(G_DbCon,strSql);
								  if(tempCursor!=NULL){
										   tempRow=ltDbFetchRow(tempCursor);
										   if(tempRow!=NULL) { 
										   	   memset(strSql,0,sizeof(strSql));               
                           sprintf(strSql,"delete from %s ",tempRow[0]);
                           ltDbExecSql(G_DbCon,strSql);
					   					   	 sprintf(caLabel,"echo '%s'> %s",tempRow[0],proIniDir);
													 system(caLabel);
													 sleep(1);
										   }
									}
								  ltDbCloseCursor(tempCursor);
								  memset(strSql,0,sizeof(strSql));
								  sprintf(strSql,"delete from msawebmailloglist where sdate<'%s'",sdate);
								  printf("mysql:%s\n",strSql);
								  ltDbExecSql(G_DbCon,strSql);
   					   	  sprintf(caLabel,"echo 'msawebmailloglist' > %s",proIniDir);
								  system(caLabel);
								  sleep(1);

	      						   
   						   //搜索关键字记录
   						    memset(strSql,0,sizeof(strSql));
						      sprintf(strSql,"truncate msasearchkey");
						      printf("mysql:%s\n",strSql);
								  ltDbExecSql(G_DbCon,strSql);
						   	  sprintf(caLabel,"echo 'msasearchkey' > %s",proIniDir);
								  system(caLabel);
								  sleep(1);

									
								sprintf(caTempDir,"/datacenter/msa/mailmon");
   						  if(ltFileIsExist(caTempDir)==1) {    	
						    	  if(chdir(caTempDir)==0){
						    	 	  sprintf(caLabel,"/bin/rm -rf *");
						    	 	  system(caLabel);									    	 	  
						    	 	  sprintf(caLabel,"echo 'mailmon'> %s",proIniDir);
							        system(caLabel);
							        sleep(1);
				       
						    	  }  						 																  	    	 
						    }
								
								sprintf(caTempDir,"/datacenter/msa/packlist");
   						  if(ltFileIsExist(caTempDir)==1) {    	
						    	  if(chdir(caTempDir)==0){
						    	 	  sprintf(caLabel,"/bin/rm -rf *");
						    	 	  system(caLabel);									    	 	  
						    	 	  sprintf(caLabel,"echo 'packlist'> %s",proIniDir);
							        system(caLabel);
							        sleep(1);
				       
						    	  }  						 																  	    	 
						    }		  							 
								  
								  
								  
								


												  
   					}								        
			} 
			
						
			if(cachr2==1){//账户中心
				    /*清空管理员域表*/
				      memset(strSql,0,sizeof(strSql));
				      sprintf(strSql,"truncate msaadminarea");
						  ltDbExecSql(G_DbCon,strSql);
				   	  sprintf(caLabel,"echo 'msaadminarea' > %s",proIniDir);
						  system(caLabel);
									  
					    /*清空管理员信息表*/
					     memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"delete from msaadmuser where name<>'admin'");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msaadmuser' > %s",proIniDir);
						   system(caLabel);
						
						  /*清空部门信息表*/
						   memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"truncate msagroup");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msagroup' > %s",proIniDir);
						   system(caLabel);
						   
						 /*清空部门ip信息表*/
						   memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"truncate msagroupip");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msagroupip' > %s",proIniDir);
						   system(caLabel);
						 
						 /*清空策略信息表*/
						 	 memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"truncate msapolicy");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msapolicy' > %s",proIniDir);
						   system(caLabel);
	
						   
						 /*清空策略ip信息表*/
						   memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"truncate msapolicyip");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msapolicyip' > %s",proIniDir);
						   system(caLabel);
						 
						 /*清空用户信息表*/
						   memset(strSql,0,sizeof(strSql));
				       sprintf(strSql,"truncate msauser");
						   ltDbExecSql(G_DbCon,strSql);
				   	   sprintf(caLabel,"echo 'msauser' > %s",proIniDir);
						   system(caLabel);
						   	    
			}
			
			
			
			if(cachr3==1){//清空服务策略
				   
				   /*清空用户策略信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate msauserpolicy");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'msauserpolicy' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空小服务信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate msaservice");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'msaservice' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   
				   /*清空服务规则信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nassrvrule");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nassrvrule' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空ip段做了服务规则的信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nassrvruledip");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nassrvruledip' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空ip段做了规则的信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasruleip");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasruleip' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空对URL做了规则的信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasruleposturl");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasruleposturl' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空规则类型信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasruletype");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasruletype' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空规则URL信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasruleurl");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasruleurl' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				   
				   /*清空规则用户信息表*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasruleuser");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasruleuser' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
				      
				  		   	  		
			
			}
			

			/***************************清空报表中心**********************/
			if(cachr4==1){//清空报告模板						  
				  /*清空自动生成的报告dailyreport*/
				  sprintf(caTempDir,"/datacenter/msa/report/dailyreport");
			    if(ltFileIsExist(caTempDir)==1) {    	
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'dailyreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }    	    	     	 
			    }
			}
			 
			 
			if(cachr5==1){
			 	
			 	 /*清空手动生成的报告instantreport*/
			    sprintf(caTempDir,"/datacenter/msa/report/instantreport");
			    if(ltFileIsExist(caTempDir)==1) {    	  
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'instantreport' > %s",proIniDir);
				        system(caLabel);
				        sleep(1);	
						 } 	 	    	 
			    }			 
			}   
			    
			 
			if(cachr6==1){			 	  
			 	  /*清空每天汇总生成的报告dailygrpreport*/
			    sprintf(caTempDir,"/datacenter/msa/report/dailygrpreport");
			    if(ltFileIsExist(caTempDir)==1) {    	   	 
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'dailygrpreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 } 	    	 
			    }		 
			 
			}
			    
			 
			if(cachr7==1){
			 	  /*清空出口流量分析报告reportoutflow*/
				 	sprintf(caTempDir,"/datacenter/msa/report/reportoutflow");
			    if(ltFileIsExist(caTempDir)==1) {    		 
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'reportoutflow' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }   	     	 
			    } 
			    
			    memset(strSql,0,sizeof(strSql));
			    sprintf(strSql,"delete from msauserhttpreport where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"/bin/echo 'msauserhttpreport' > %s",caTempDir);
					system(caLabel);
					sleep(1);
			 	
			}	  
				
		 	if(cachr8==1){		 	 	
		 	 	  /*清空服务协议分析报告srvreport*/
				  sprintf(caTempDir,"/datacenter/msa/report/srvreport");
			    if(ltFileIsExist(caTempDir)==1) {    	
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'srvreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }	    	 	    	 
			    } 
			    
			    memset(strSql,0,sizeof(strSql));
			    sprintf(strSql,"delete from msatimesrvreport where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"/bin/echo 'msatimesrvreport' > %s",caTempDir);
					system(caLabel);	
					sleep(1);
		 	 	
		 	}
				 	
      if(cachr9==1){        	 
        	  /*清空用户时间分析报告timereport*/
				 	sprintf(caTempDir,"/datacenter/msa/report/timereport");
			    if(ltFileIsExist(caTempDir)==1) {
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'timereport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }    				    	 		    	 	    	 
			    }  
			    
			      /*msaaccountreport表记录清除*/
			    memset(strSql,0,sizeof(strSql));
				  sprintf(strSql,"delete from msaaccountreport where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"echo 'msaaccountreport' > %s",proIniDir);
					system(caLabel);
					sleep(1); 
          
          memset(strSql,0,sizeof(strSql));
          sprintf(strSql,"delete from msatimereport where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"/bin/echo 'msatimereport' > %s",caTempDir);
					system(caLabel);	
					sleep(1); 
        
      }	  	
			 	 
				 	 
			if(cachr10==1){
			  	 /*清空网络流量分析报告*/
			    sprintf(caTempDir,"/datacenter/msa/report/flowreport");
			    if(ltFileIsExist(caTempDir)==1) {
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'flowreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }       	
			    	 		    	 	    	 
			    }
			}  
			   
		    
		  if(cachr11==1){
		    	 /*清空网络次数分析报告*/
			    sprintf(caTempDir,"/datacenter/msa/report/connectreport");
			    if(ltFileIsExist(caTempDir)==1) {
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'connectreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }   	
			    	 		    	 	    	 
			    }
			    
			    memset(strSql,0,sizeof(strSql));
			    sprintf(strSql,"delete from msadipreport where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"/bin/echo 'msadipreport' > %s",caTempDir);
					system(caLabel);
					sleep(1);
			    
			    
			    
		  }

			if(cachr12==1){
			  	  /*清空网站分析报告*/
			    sprintf(caTempDir,"/datacenter/msa/report/webreport");
			    if(ltFileIsExist(caTempDir)==1) {
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'webreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }     	
			    	 		    	 	    	 
			    }
			} 
			  
			  
			if(cachr13==1){
			  	/*清空用户计费报告*/
			    sprintf(caTempDir,"/datacenter/msa/report/jfreport");
			    if(ltFileIsExist(caTempDir)==1) {
			    	 if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);							
								sprintf(caLabel,"echo 'jfreport' > %s",proIniDir);
				        system(caLabel);	
				        sleep(1);
						 }   			    	 		    	 	    	 
			    }
			}
			  
			       
			   
						
									
					



			/********************************************管理中心***************************************/
			if(cachr15==1){//管理员操作日志
				  memset(strSql,0,sizeof(strSql));
				  sprintf(strSql,"delete from msaadmlog where sdate<'%s'",sdate);
					ltDbExecSql(G_DbCon,strSql);
			   	sprintf(caLabel,"echo 'msaadmlog ' > %s",proIniDir);
					system(caLabel);				
			}
			
			if(cachr16==1){//网络对象
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasNetArea");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasNetArea ' > %s",proIniDir);
				   system(caLabel);			
			}
			
			if(cachr17==1){//端口属性
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasPortInfo");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasPortInfo ' > %s",proIniDir);
				   system(caLabel);		
			}
			
			
			if(cachr18==1){//系统参数		
				  sprintf(caTempDir,"/etc/msa/msa/option"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'option config '> %s",proIniDir);
				        system(caLabel);
				        sleep(2);      
			    	  }  						 																  	    	 
			    } 
				   			
			}
			
			
			if(cachr19==1){//邮件分类				
				 sprintf(caTempDir,"/etc/msa/msa/level/maillevel"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'maillevel '> %s",proIniDir);
				        system(caLabel);
				        sleep(2);      
			    	  }  						 																  	    	 
			    } 
			    
			      /*清空邮件监控表*/
				  memset(strSql,0,sizeof(strSql));
				  sprintf(strSql,"truncate msamailfilter");
				  ltDbExecSql(G_DbCon,strSql);
		   	  sprintf(caLabel,"echo 'msamailfilter ' > %s",proIniDir);
				  system(caLabel);
				  sleep(1);
				
							
			}
			
			if(cachr20==1){//表单分类						
					sprintf(caTempDir,"/etc/msa/msa/level/formlevel"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'formlevel '> %s",proIniDir);
				        system(caLabel);
				        sleep(2);      
			    	  }  						 																  	    	 
			    }
			    
			    sprintf(caTempDir,"/etc/msa/msa/url"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'url config'> %s",proIniDir);
				        system(caLabel);
				        sleep(2);      
			    	  }  						 																  	    	 
			    }
			    
			    //表单提交内容
			    memset(strSql,0,sizeof(strSql));
		      sprintf(strSql,"truncate msaformfilter");
		      printf("mysql:%s\n",strSql);
				  ltDbExecSql(G_DbCon,strSql);
		   	  sprintf(caLabel,"echo 'msaformfilter' > %s",proIniDir);
				  system(caLabel);
				  sleep(1);
			}
			
			if(cachr21==1){//URL分类
				  /*清空nasurls*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasurls");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasurls' > %s",proIniDir);//
				   system(caLabel);
				   sleep(1);
				 
				
				  /*清空nasurlsort*/
				   memset(strSql,0,sizeof(strSql));
				   char temp[64];
				   char mytype[64];
				   sprintf(temp,"%s","自定义分类");
	         strgb2utf8(temp,mytype,strlen(temp));
	         int sortid=76;
	         int i=1;
	         for(;sortid<91;sortid++){         	 
	         	 sprintf(strSql,"update nasurlsort set sorname='%s%d',sortdesc='' where sortid=%d",mytype,i,sortid);				   
				     ltDbExecSql(G_DbCon,strSql);
				     i++;
	         }
				   
		   	   sprintf(caLabel,"echo 'nasurlsort' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);	
							
			}
			
			if(cachr22==1){//时间策略  nasschedule				
				   /*清空nasschedule*/
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasschedule");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasschedule' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
							
			}
			
			
			
			if(cachr23==1){//法定假日 nasjjr							
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nasjjr");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nasjjr' > %s",proIniDir);
				   system(caLabel);			
				   sleep(1);		
			}
			
			
			if(cachr24==1){//服务类型nassrv				
				   memset(strSql,0,sizeof(strSql));
		       sprintf(strSql,"truncate nassrv");
				   ltDbExecSql(G_DbCon,strSql);
		   	   sprintf(caLabel,"echo 'nassrv' > %s",proIniDir);
				   system(caLabel);
				   sleep(1);
							
			}
			
			if(cachr25==1){//时间服务 配置文件
					sprintf(caTempDir,"/etc/msa/msa/time"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'timeservice'> %s",proIniDir);
				        system(caLabel);
				        sleep(1);      
			    	  }  						 																  	    	 
			    } 
			}
			
		 if(cachr26==1){//时间服务 配置文件
					sprintf(caTempDir,"/etc/msa/msa/dev"); 
				  if(ltFileIsExist(caTempDir)==1) {    	
			    	  if(chdir(caTempDir)==0){
			    	 	  sprintf(caLabel,"/bin/rm -rf *");
			    	 	  system(caLabel);									    	 	  
			    	 	  sprintf(caLabel,"echo 'dev config'> %s",proIniDir);
				        system(caLabel);
				        sleep(1);      
			    	  }  						 																  	    	 
			    } 
			}
			
			
			sprintf(caLabel,"echo '0' > %s",proIniDir);
		  system(caLabel);/*建立目录标志*/	  
			sleep(1);
			
			
			printf("system clean end.................\n");
			
			
  
		return 0; 
}


//清空状态栏显示信息
int msaStatusAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    FILE *fd=NULL;
		char str[1024];
		fd=fopen("/datacenter/msa/process.ini","r");
    if(fd){
    	fgets(str,256,fd);
    } 
    fclose(fd);	
 	 printf("str:%s\n",str);
 	 ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,str);
   ltMsgFree(ltMsgPk); 
   return 0;
   
}



