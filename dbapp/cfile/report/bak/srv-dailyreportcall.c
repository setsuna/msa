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
#include "msa.h"
#define _dailyreport_ch	"/reportplt/dailyreport_ch"
#define _dailyreport_en	"/reportplt/dailyreport_en"
char *dbName;
char *dbUser;
char *dbPass;
char tempname[123];

/* 服务协议 相关报告 */
int srvreport(char date1[36],char date2[36],char gid[2048],int otype,char caTempDir[256],int lang,int work)
{
    ltDbCursor *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char        htmlpage[512];
    char        oldname[256];
    char        sqlBuf[1024];
    char        caSrv[64];
    char        caLabel[256];
    char        caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    char        *lt_page_content=NULL;
    long        lTime,lTime1,lTime2;
    unsigned    long long lMaxBytes;   
    int         srvindex,iFd; 
    iFd=-1;      
    lMaxBytes=1;    
    tempCursor=NULL;     
    memset(htmlpage,0,sizeof(htmlpage));
    ltDbConn *G_DbCon;
    dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (G_DbCon==NULL){
			_exit(0);
		}

    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    lTime2 = nasCvtLongTime(caEdate,"23:59:59"); 
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) { 
            return 0;
        }
    } 
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else {
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        lTime2 = 0;
    }
    if(lTime1 == 0){
        lTime1 = lTime;
    }
		if(lTime2 == 0){
        lTime2 = lTime;
    }
		if(lTime1 > lTime){
        lTime1 = lTime;
    }
    if(lTime2>lTime){
				lTime2=lTime;
		}
	 
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
		if(otype==1){													/*全部汇总*/		    
					sprintf(sqlBuf,"select c.name,service,sum(bytes)/1024+1 as abyte,sum(onlinetime) as atime,c.id from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by c.id,c.name,service,workflag order by c.id,abyte desc ",caSdate1,caSdate2,gid,work);		
		}else if(otype==2){										/*部门汇总*/
					sprintf(sqlBuf,"select c.name,service,sum(bytes)/1024+1 as abyte,sum(onlinetime) as atime,c.id from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by c.id,c.name,service,workflag order by c.id,abyte desc ",caSdate1,caSdate2,gid,work);
		}else if(otype==4){										/*按日期汇总*/
					sprintf(sqlBuf,"select sdate,service, sum(bytes)/1024+1 as abyte,sum(onlinetime) as atime from msasrvreport a,msauser b where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0 in (%s) and workflag=%d group by sdate,service,workflag order by sdate asc,abyte desc",caSdate1,caSdate2,gid,work);
		}else if(otype==3 || otype==7){				/*按用户流量 按用户连接*/			
					int       iTemp,usersrvrequest,*srvrequest;
					long long *srvflow,usersrvflow;
					char      caLine[30000],caggg[64];
					memset(oldname,0,sizeof(oldname));
					memset(caggg,0,sizeof(caggg));
					srvflow=(long long *)malloc(sizeof(long long)*(MAX_TOP_SRVNUM+1));
					srvrequest=(int *)malloc(sizeof(int)*(MAX_TOP_SRVNUM+1));	  
					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
						srvflow[iTemp]=0;
						srvrequest[iTemp]=0;
					}							
					usersrvflow=0;
					usersrvrequest=0;
				  sprintf(sqlBuf,"select b.username,service,sum(bytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d  group by b.username,service,c.name order by abyte desc",caSdate1,caSdate2,gid,work);												
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor == NULL) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
						system(caLabel);																	/*建立目录和正在处理标志*/
						return 0;
					}		

					dbPtr=lt_dbinit();
					lt_db_htmlpage(dbPtr,"utf-8");
					tablePtr=lt_dbput_table(dbPtr,"glist");
					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
						 if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){break;}
						 lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iTemp].srvname);
					}
					tablePtr=lt_dbput_table(dbPtr,"llist");
					tempRow= ltDbFetchRow(tempCursor);					
					while(tempRow!=NULL){
						if(strcmp(oldname,tempRow[0])!=0){								/*新的一行*/  	    
							if(oldname[0]!='\0'){								  	
								if(otype==3){
									sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",oldname,caggg,usersrvflow);
								}else if(otype==7){
									sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
								}
								
								for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){		
										if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){break;}															 
										if(otype==3){
										   sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKB</td>",caLine,srvflow[iTemp]);
										}else{
										   sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
										}										
								}									
								sprintf(caLine,"%s</tr>",caLine);
								lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
							}
							
							sprintf(oldname,"%s",tempRow[0]);
							sprintf(caggg,"%s",tempRow[4]);
							usersrvflow=0;
							usersrvrequest=0;
							for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
									srvflow[iTemp]=0;
									srvrequest[iTemp]=0;
							}						  
							srvflow[atol(tempRow[1])]=atol(tempRow[2]);
							srvrequest[atol(tempRow[1])]=atol(tempRow[3]);
							usersrvflow=usersrvflow+atol(tempRow[2]);
							usersrvrequest=usersrvrequest+atol(tempRow[3]);  	
						}else{
							srvflow[atol(tempRow[1])]=atol(tempRow[2]);			/*每一个服务*/
							srvrequest[atol(tempRow[1])]=atol(tempRow[3]);	/*每一个服务*/
							usersrvflow=usersrvflow+atol(tempRow[2]);				/*所有服务总流量*/
							usersrvrequest=usersrvrequest+atol(tempRow[3]);	/*所有服务总时间*/
						}
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
					
					if(oldname[0]!='\0'){																/*新的一行开始写*/					
						if(otype==3){
							sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%lldK</td>",oldname,caggg,usersrvflow);
						}else if(otype==7){
							sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'>%s</td>",oldname,caggg,time2str(usersrvrequest));
						}
						for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){	
							if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){break;}	
							if(otype==3){
								sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%lldKb</td>",caLine,srvflow[iTemp]);
							}else{
								sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(srvrequest[iTemp]));
							} 	
						}
						sprintf(caLine,"%s</tr>",caLine);
						lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
					}

					lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);		
					if(lang==0){				//Ch		
							sprintf(htmlpage,"%s%s/usersrvplt.htm",_msahtmldir,_dailyreport_ch);
							lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}else if(lang==1){	//Eng  
							sprintf(htmlpage,"%s%s/usersrvplt_en.htm",_msahtmldir,_dailyreport_en);
							lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}
					if(lt_page_content==NULL){ 
							sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
							system(caLabel);																		/*建立目录和正在处理标志*/
							lt_dbfree(dbPtr);
							return 0;
					}else{								 
							lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
							sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
							iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
							if(iFd >0) {
							  write(iFd,lt_page_content,strlen(lt_page_content));
							}
							close(iFd);
					} 
					lt_dbfree(dbPtr);	 
					return 0;
	  }else if(otype==5 || otype==6){																/*按日期流量 按日期连接*/
				 typedef struct gname_s {
					int             gid;
					char            gname[50];
					long long       gflow;
					int             gtime;
				 }nasGname;
					nasGname  gnamelist[1000];
					long long gflowlist[1000],gtimelist[1000];
					char      caLine[16396],oldDate[64];
					int       oldgroupid,oldsrvid,xgroupid,iTemp;
			    
					for(iTemp=0;iTemp<1000;iTemp++){
						gnamelist[iTemp].gid=-1;
						memset(gnamelist[iTemp].gname,0,50);
						gnamelist[iTemp].gflow=0;
						gnamelist[iTemp].gtime=0;
						gflowlist[iTemp]=0;
						gtimelist[iTemp]=0;
					}
			    				
				  sprintf(sqlBuf,"select id,name from msagroup where id in (%s) order by id asc ",gid);	      /*显示部门 */
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor == NULL){
						    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
						    system(caLabel);
					      return 0;
					}			
					iTemp=0;
					dbPtr=lt_dbinit();
			    lt_db_htmlpage(dbPtr,"utf-8");	    
			    tablePtr=lt_dbput_table(dbPtr,"glist");
					tempRow=ltDbFetchRow(tempCursor);
			    while(tempRow!=NULL){
				    	gnamelist[iTemp].gid=atol(tempRow[0]);										/*存储部门id*/	
				    	sprintf(gnamelist[iTemp].gname,"%s",tempRow[1]);					/*存储部门名字*/
				    	lt_dbput_recordvars(tablePtr,1,"groupname",LT_TYPE_STRING,tempRow[1]);	    	
				    	iTemp++;
				    	if(iTemp>=1000){
				    		break;
				    	}
				    	tempRow= ltDbFetchRow(tempCursor);
			    }
					ltDbCloseCursor(tempCursor);					
					sprintf(sqlBuf,"select a.sdate as xdate,service,c.id,sum(bytes)/1024+1 as abyte ,sum(onlinetime) as arequest from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d  group by a.sdate,service,c.id order by abyte desc ",caSdate1,caSdate2,gid,work);
			    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor == NULL) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
						system(caLabel);
						return 0;
					}
					oldgroupid=-1;oldsrvid=-1;memset(oldDate,0,sizeof(oldDate));
					tablePtr=lt_dbput_table(dbPtr,"llist");
					tempRow= ltDbFetchRow(tempCursor);
			    while(tempRow!=NULL){
			    	if(strcmp(oldDate,tempRow[0])!=0){																					/*日期变了*/
			    		if(oldDate[0]!='\0' ){	
			    		    for(iTemp=0;iTemp<1000;iTemp++){   		  	
										if(gnamelist[iTemp].gid==-1){
											break;
										}
										if(otype==5){
											sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[iTemp].gflow);
										}else{
											sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[iTemp].gtime));
										}
						      }
				    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
				    			if(otype==5){
								       sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>Totle Flow</td>");
									}else if(otype==6){
								       sprintf(caLine,"%s","<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>Totle Time</td>");
									}
			 			    
									for(iTemp=0;iTemp<1000;iTemp++){
										if(gnamelist[iTemp].gid==-1){
											break;
										}
										if(otype==5){
											sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldKb</td>",caLine,gflowlist[iTemp]);
										}else{
											sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gtimelist[iTemp]));
										}
									}
				    			lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
			    		}
			    		oldgroupid=-1;
				  		oldsrvid=-1;
				  		memset(caLine,0,sizeof(caLine));
			    		for(iTemp=0;iTemp<1000;iTemp++){
					    	gnamelist[iTemp].gflow=0;
					    	gnamelist[iTemp].gtime=0;
					    	gflowlist[iTemp]=0;
					    	gtimelist[iTemp]=0;
					    }
							sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);			 		  
			    		sprintf(oldDate,"%s",tempRow[0]);	    		
			    		sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'>%s</td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",tempRow[0],caSrv);
			    		oldsrvid=atol(tempRow[1]);
			    		xgroupid=atol(tempRow[2]);
			    		for(iTemp=0;iTemp<1000;iTemp++){
			    			if(gnamelist[iTemp].gid==xgroupid){
			    				 gnamelist[iTemp].gflow=atol(tempRow[3]);
					    		 gnamelist[iTemp].gtime=atol(tempRow[4]);
					    		 gflowlist[iTemp]=gflowlist[iTemp]+gnamelist[iTemp].gflow;
					    		 gtimelist[iTemp]=gtimelist[iTemp]+gnamelist[iTemp].gtime;
					    		 break;
			    			}
					    }
			    	}else{																	/*如果是同一天*/
							if(oldsrvid!=atol(tempRow[1])){				/*服务变了,新的一行开始*/
										if(oldsrvid!=-1 ){
												for(iTemp=0;iTemp<1000;iTemp++){
													if(gnamelist[iTemp].gid==-1){
														break;
													}
													if(otype==5){
														sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldK</td>",caLine,gnamelist[iTemp].gflow);
													}else{
														sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[iTemp].gtime));
													}
												}
											  lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
									  }
										oldgroupid=-1;
										oldsrvid=-1;
										for(iTemp=0;iTemp<1000;iTemp++){
											gnamelist[iTemp].gflow=0;
											gnamelist[iTemp].gtime=0;
										}
										memset(caLine,0,sizeof(caLine));
										sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
										sprintf(oldDate,"%s",tempRow[0]);
										sprintf(caLine,"<tr height=17 style='mso-height-source:userset;height:12.95pt'>\r\n <td class=xl5016118 style='border-top:none;border-left:none'></td>\r\n <td class=xl2816118 style='border-top:none;border-left:none'>%s</td>",caSrv);	
										oldsrvid=atol(tempRow[1]);
										xgroupid=atol(tempRow[2]);
										for(iTemp=0;iTemp<1000;iTemp++){
											if(gnamelist[iTemp].gid==xgroupid){
												gnamelist[iTemp].gflow=atoll(tempRow[3]);
												gnamelist[iTemp].gtime=atol(tempRow[4]);
												gflowlist[iTemp]=gflowlist[iTemp]+gnamelist[iTemp].gflow;
												gtimelist[iTemp]=gtimelist[iTemp]+gnamelist[iTemp].gtime;
												break;
											}
										}
			    	  }else{																/*服务没变，增加部门*/
			    	  	    xgroupid=atol(tempRow[2]);
			    		      for(iTemp=0;iTemp<1000;iTemp++){
							    			if(gnamelist[iTemp].gid==xgroupid){
							    				gnamelist[iTemp].gflow=atol(tempRow[3]);
									    		gnamelist[iTemp].gtime=atol(tempRow[4]);
									    		gflowlist[iTemp]=gflowlist[iTemp]+gnamelist[iTemp].gflow;
								    		  gtimelist[iTemp]=gtimelist[iTemp]+gnamelist[iTemp].gtime;
									    		break;
							    			}
						        }
						    
			    	  }																			/*end 服务没变，增加部门*/
			      }
			      tempRow= ltDbFetchRow(tempCursor);
			    }
			    ltDbCloseCursor(tempCursor);
			
					if(oldDate[0]!='\0' ){/*写入旧的一行*/	
							for(iTemp=0;iTemp<1000;iTemp++){					
								if(gnamelist[iTemp].gid==-1){
									break;
								}
								if(otype==5){
									sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%lldKb</td>",caLine,gnamelist[iTemp].gflow);
								}else{
									sprintf(caLine,"%s\r\n<td class=xl2916118 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[iTemp].gtime));
								}
							}
							lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
					}
					lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
					memset(htmlpage,0,sizeof(htmlpage));
					if(lang==0){//Ch
						 sprintf(htmlpage,"%s%s/srvgroupdateplt.htm",_msahtmldir,_dailyreport_ch);
						 lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}else if(lang==1){//Eng
						 sprintf(htmlpage,"%s%s/srvgroupdateplt_en.htm",_msahtmldir,_dailyreport_en);
						 lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					}
		 
		
					if(lt_page_content==NULL){
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
						system(caLabel); 
						lt_dbfree(dbPtr);
						return 0;
					}else{
						lt_page_content=strstr(lt_page_content,"<!DOCTYPE");  
						sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
						iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
						if(iFd >0) {
						 write(iFd,lt_page_content,strlen(lt_page_content));
						}
						close(iFd);
					}	  
				 
					lt_dbfree(dbPtr);
					return 0;
	  }   

		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		    system(caLabel);
		    return 0;
		}
		
    if(otype==1){
    	 	sprintf(caLabel,"%sdata.ini",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		    if(iFd == (-1)){
			      sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
		    	  system(caLabel);
		    	  ltDbCloseCursor(tempCursor);
	          return 0;
				}									
		}
		
		memset(oldname,0,sizeof(oldname));
    srvindex=1;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    tempRow=ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
        if(strcmp(oldname,tempRow[0])==0){	
							if(otype==1){ 
								sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
							}else{
								sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
							}
							  
							lt_dbput_recordvars(tablePtr,5,
								"gname",LT_TYPE_STRING,"",
								"sname",LT_TYPE_STRING,caSrv,
								"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[3])
								) 
							);
									
							if(otype==1){ 
								sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[2]));
								write(iFd, caLabel, strlen(caLabel));	
								if(lMaxBytes<atoll(tempRow[2])){
								  lMaxBytes=atoll(tempRow[2]);
								}
							}					
        }else{ 
							char caGname[512]; 
							if(otype==1){
								sprintf(caSrv,"No%d %s",srvindex,_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
							}else{
							  sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
							}
						  if(otype==2){
					      sprintf(caGname,"%s",tempRow[0]);
							}else{
							  sprintf(caGname,"%s",tempRow[0]);
							}
							sprintf(oldname,"%s",tempRow[0]);
							printf("oldname:%s\n",oldname);
							lt_dbput_recordvars(tablePtr,5,
								"gname",LT_TYPE_STRING,caGname,
								"sname",LT_TYPE_STRING,caSrv,
								"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[3]))
							);
							
							if(otype==1){  
							    sprintf(caLabel,"%d,%lld\r\n",srvindex,atoll(tempRow[2]));
									write(iFd, caLabel, strlen(caLabel));	
									if(lMaxBytes<atoll(tempRow[2])){
						         lMaxBytes=atoll(tempRow[2]);
									}						
							}   	    
				}
				
				if(otype==1){ 
					srvindex++;
				}
     
				tempRow= ltDbFetchRow(tempCursor);
		}
  	ltDbCloseCursor(tempCursor);
  
	  if(otype==1){
				close(iFd);							  
	      sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
		    system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"srvreporta.gif\">" );
		}
	
	  lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
	  memset(htmlpage,0,sizeof(htmlpage));
	  if(lang==0){						//Ch
	  		sprintf(htmlpage,"%s%s/srvreportplta.htm",_msahtmldir,_dailyreport_ch);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}else if(lang==1){			//Eng
				sprintf(htmlpage,"%s%s/srvreportplta_en.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}
 
	  if(lt_page_content==NULL){
		    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    	system(caLabel);
	    	lt_dbfree(dbPtr);
	      return 0;
		}else{  
		    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		    sprintf(caLabel,"%ssrv%d.htm",caTempDir,otype);
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
	      if(iFd >0) {
	         write(iFd,lt_page_content,strlen(lt_page_content));
	      }
	      close(iFd);
	  }
	  lt_dbfree(dbPtr); 
	  return 0;
}

 
/*部门网络  时间汇总 */
int usertimereport(char date1[36],char date2[36],char caTempDir[256],char gid[2048],int lang,int work)
{  
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char caSdate[32],caEdate[32],caSdate1[32],caSdate2[32];
    char caLabel[256];
    char htmlpage[512];
    char *lt_page_content=NULL;
    long lTime,lTime1,lTime2;
    int  iFd;

		ltDbConn *G_DbCon;
    dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if (G_DbCon==NULL){
			_exit(0);
		}
    iFd=-1;
 
    if(ltFileIsExist(caTempDir)!=1) {
    		mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    }
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        } 
    }
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    tempCursor=NULL;
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else {
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        lTime2 = 0;
    }
    if(lTime1 == 0) { 
          lTime1 = lTime;
    }
		if(lTime2 == 0) {
          lTime2 = lTime;
    }
		if(lTime1 > lTime) {
          lTime1 = lTime;
    }
    if(lTime2>lTime){
		 lTime2=lTime;
    }

    nasTimeGetDate(caSdate1,lTime);
    nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    int  groupnum;
    typedef struct gname_s {
				int             gid;
				char            gname[50];
				long long       gflow;
				int             gtime;
		}nasGname;

    nasGname gnamelist[1000];
    int      i;
    char     caLine[30000];
		char     tmpLine[30000];
    int      oldgroupid;
		int      oldsrvid;
		int      xgroupid;
		char     oldDate[64];
	
		memset(tmpLine,0,sizeof(tmpLine));
		memset(caLine,0,sizeof(caLine));
    for(i=0;i<1000;i++){
    	gnamelist[i].gid=-1;
    	memset(gnamelist[i].gname,0,50);
    	gnamelist[i].gflow=0;
    	gnamelist[i].gtime=0;
    }
		groupnum=0;	  
  	sprintf(sqlBuf,"select id,name from msagroup where id in(%s) order by id asc ",gid);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL) {
		  sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir); 
		  system(caLabel);/*建立目录和正在处理标志*/
		  return 0;
		}

		dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"glist");
		tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    	gnamelist[groupnum].gid=atol(tempRow[0]);
    	sprintf(gnamelist[groupnum].gname,"%s",tempRow[1]);
    	lt_dbput_recordvars(tablePtr,1,
		    "groupname",LT_TYPE_STRING,tempRow[1]
			);
    	tempRow= ltDbFetchRow(tempCursor);
    	groupnum++;
    	if(groupnum>99){
    		break;
    	}
    }
    ltDbCloseCursor(tempCursor);
	
		oldgroupid=-1;
		oldsrvid=-1;
		memset(oldDate,0,sizeof(oldDate));    
  	sprintf(sqlBuf,"select sum(onlinetime) as atime,sdate,a.userid,c.id from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and  b.groupid0 in(%s) and workflag=%d group by sdate,a.userid,c.id,b.groupid0 order by sdate,b.groupid0,atime desc",caSdate1,caSdate2,gid,work);
    printf("sq0:%s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/  
			return 0;
		}

		tablePtr=lt_dbput_table(dbPtr,"llist");
		tempRow= ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
    	if(strcmp(oldDate,tempRow[1])!=0){/*day change 全变了，新的一行加时间*/
    		if(oldDate[0]!='\0'){/*写入旧的一行*/	
						for(i=0;i<100;i++){
							if(gnamelist[i].gid==-1){
								break;
							}
							sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);
						}
						lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
						memset(caLine,0,sizeof(caLine));
		
						if(lang==0){//Ch
								sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
								strgb2utf8(caLine,tmpLine,sizeof(caLine));
								memset(caLine,0,sizeof(caLine));
								sprintf(caLine,"%s",tmpLine);
						}else if(lang==1){//Eng
								sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Time</td>");
						}
						for(i=0;i<100;i++){/*时间*/
								if(gnamelist[i].gid==-1){
									break;
								}
								sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
						}
		
						lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
						memset(caLine,0,sizeof(caLine));
						if(lang==0){//Ch
								sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
								strgb2utf8(caLine,tmpLine,sizeof(caLine));
								memset(caLine,0,sizeof(caLine));
								sprintf(caLine,"%s",tmpLine);
						}else if(lang==1){//Eng
								sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Average</td>");
						}
		
						for(i=0;i<100;i++){/*平均*/
							if(gnamelist[i].gid==-1){
								break;
							}
		
							if(gnamelist[i].gflow){
								sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime/gnamelist[i].gflow));
							}else{
								sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>0:0:0</td>",caLine);
							}
						}
						lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
    		}

    		oldgroupid=-1;
	  		oldsrvid=-1;
    		memset(caLine,0,sizeof(caLine));
    		for(i=0;i<100;i++){
		    	gnamelist[i].gflow=0;
		    	gnamelist[i].gtime=0;
		    }

    		sprintf(oldDate,"%s",tempRow[1]);
    		memset(caLine,0,sizeof(caLine));
			if(lang==0){//Ch
				sprintf(caLine,"<tr height=19 style='height:14.25pt'>\r\n \
  <td rowspan=3 height=57 class=xl488922 style='border-bottom:.5pt solid black;\r\n \
  height:42.75pt;border-top:none'>%s</td> \r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;\r\n \
 width:61pt'>上网人数</td>",oldDate);
        strgb2utf8(caLine,tmpLine,sizeof(caLine));
				memset(caLine,0,sizeof(caLine));
				sprintf(caLine,"%s",tmpLine);
			}else if(lang==1){//Eng
				sprintf(caLine,"<tr height=19 style='height:14.25pt'>\r\n \
  <td rowspan=3 height=57 class=xl488922 style='border-bottom:.5pt solid black;\r\n \
  height:42.75pt;border-top:none'>%s</td> \r\n \
  <td class=xl248922 width=81 style='border-top:none;border-left:none;\r\n \
 width:61pt'>Online number</td>",oldDate);
			}

    		
    		xgroupid=atol(tempRow[3]);
    		for(i=0;i<100;i++){
				if(gnamelist[i].gid==xgroupid){
					gnamelist[i].gflow++;
					gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
					break;
				}
			}



    	}else{/*keep day*/
    	  xgroupid=atol(tempRow[3]);
    		for(i=0;i<100;i++){
					if(gnamelist[i].gid==xgroupid){
						gnamelist[i].gflow++;
						gnamelist[i].gtime=gnamelist[i].gtime+atol(tempRow[0]);
						break;
					}
				}
 
        }
        tempRow= ltDbFetchRow(tempCursor);
    }
		ltDbCloseCursor(tempCursor);
	  
		if(oldDate[0]!='\0' ){/*写入旧的一行*/	
		for(i=0;i<100;i++){
			if(gnamelist[i].gid==-1){
				break;
			}
			sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%lld</td>",caLine,gnamelist[i].gflow);
		} 
		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);

		if(lang==0){//Ch
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>时间</td>");
			strgb2utf8(caLine,tmpLine,sizeof(caLine));
			memset(caLine,0,sizeof(caLine));
			sprintf(caLine,"%s",tmpLine);
		}else if(lang==1){//Eng
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Time</td>");
		}
		
		for(i=0;i<100;i++){/*时间*/
			if(gnamelist[i].gid==-1){
				break;
			}
			sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime));
		}

		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
		if(lang==0){//Ch
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
			  <td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>平均</td>");
			strgb2utf8(caLine,tmpLine,sizeof(caLine));
			memset(caLine,0,sizeof(caLine));
			sprintf(caLine,"%s",tmpLine);
		}else if(lang==1){//Eng
			sprintf(caLine,"%s","<tr height=19 style='height:14.25pt'>\r\n \
				<td class=xl248922 width=81 style='border-top:none;border-left:none;width:61pt'>Average</td>");
		}
									

		for(i=0;i<100;i++){/*平均*/
			if(gnamelist[i].gid==-1){
				break;
			}

			if(gnamelist[i].gflow){
				sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>%s</td>",caLine,time2str(gnamelist[i].gtime/gnamelist[i].gflow));
			}else{
			  sprintf(caLine,"%s\r\n<td class=xl258922 style='border-top:none;border-left:none'>0:0:0</td>",caLine);
			}
		}
		lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);

    }
 
    lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
 		memset(htmlpage,0,sizeof(htmlpage));
		if(lang==0){//Ch
				sprintf(htmlpage,"%s%s/usertimeplt.htm",_msahtmldir,_dailyreport_ch);
		    lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}else if(lang==1){//Eng
				sprintf(htmlpage,"%s%s/usertimeplt_en.htm",_msahtmldir,_dailyreport_en);
			  lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		}
		if(lt_page_content==NULL){ 
				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
		    system(caLabel);			/*建立目录和正在处理标志*/
	      lt_dbfree(dbPtr);
		    return 0;
		}else{
				lt_page_content=strstr(lt_page_content,"<html>");
				sprintf(caLabel,"%sdeparttime.htm",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
				if(iFd >0) {
		       write(iFd,lt_page_content,strlen(lt_page_content));
		    }
		    close(iFd);
		}
		lt_dbfree(dbPtr);
	  return 0;
} 
 

/*网络流量  相关报告*/
int connectflow(char date1[36],char date2[36],char gid[2048],int lUnit,char caTempDir[128],int lang,int work,int inttopusernum)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;  
    struct nasFlowCount_s {
        char               caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    }sCount[35]; 
    long lStep,lTime,lTime1,lTime2,lTime0;
    unsigned long long lUbytes,lDbytes,lMaxBytes;
    unsigned long long *lTop;
    unsigned long long  lMax;  
    unsigned long iMaxItem; 
    int  iTemp,iFd,i,iMax,topuser;
    char caSdate[32],caTime[32],caItem[16];
    char caSdate1[32],caSdate2[32],caTime1[32];
    char caLabel[512];
    char caEdate[32];
    char htmlpage[512];
    char sqlBuf[1024]; 
    char *lt_page_content;
    char oldgname[128];
		char egname[128];   
    ltDbConn *G_DbCon;
		topuser=100;
    dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (G_DbCon==NULL){
				_exit(0);
		}
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);  
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)){
        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else{
        lTime1 = 0;
    }
    if(!ltStrIsSpaces(caEdate)){
        lTime2 = nasCvtLongTime(caEdate,"24:00:00");
    }else{
        lTime2 = lTime;
    }
    if(ltFileIsExist(caTempDir)!=1) {
      if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {         
        return 0;
      }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
    iMaxItem = 32;
    tempCursor=NULL;
    dbPtr=NULL;
    lt_page_content=NULL;
    lTop=NULL;
    if(lUnit==1){					/*按天汇总 */
        memset((char *)&sCount,0,35*sizeof(struct nasFlowCount_s));
        lStep = 24 * 3600;
        if(lTime1 == 0) {
          lTime1 = lTime-31*lStep;          
        }
        if(lTime2>(lTime1+31*lStep)){
        	lTime2=lTime1+31*lStep;
        }
        if(lTime2>lTime){
					lTime2=lTime;
				}
        lTime0 = lTime1;
        iMaxItem = 32;
        for(i=0;i<iMaxItem;i++) {
	    			nasCvtStime(lTime0,caSdate,caTime);
            memset(sCount[i].caItem,0,16);
            lTime0 = lTime0 + lStep;
        }
        nasTimeGetDate(caSdate1,lTime1);
        nasTimeGetDate(caSdate2,lTime2);
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimesrvreport where sdate>='%s' and sdate<='%s' group by sdate order by sdate desc",caSdate1,caSdate2); 			   
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);   
    }else if(lUnit==3){/*用户*/	
        lStep = 24 * 3600;
        if(lTime1 == 0) {
					lTime1 = lTime-31*lStep;
        }
        if(lTime2>lTime){
					lTime2=lTime;
				}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
				for(iTemp=0;iTemp<topuser;iTemp++){
					lTop[iTemp]=0;
				}
	   		nasTimeGetDate(caSdate1,lTime1);
  	    nasTimeGetDate(caSdate2,lTime2);
        sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1 as abyte,sum(a.ubytes)/1024+1,sum(a.dbytes)/1024+1,c.name from msaAccountReport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,gid,work,inttopusernum);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==4){/*用户上流量排序*/
        lStep = 24 * 3600;
        if(lTime1 == 0) {
           	lTime1 = lTime-31*lStep;
        }   
        if(lTime2>lTime){
		   			lTime2=lTime;
				}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
				for(iTemp=0;iTemp<topuser;iTemp++){
					lTop[iTemp]=0;
				}
        nasTimeGetDate(caSdate1,lTime1);
  	    nasTimeGetDate(caSdate2,lTime2);
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 abyte,sum(a.dbytes)/1024+1,c.name from msaAccountReport a,msauser b,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,gid,work,inttopusernum);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==5){/*用户下流量排序*/
        lStep = 24 * 3600;
        if(lTime1 == 0){
					lTime1 = lTime-31*lStep;
        }
        if(lTime2>lTime){
					lTime2=lTime;
				}
        lTime0 = lTime1;
        lTop=(unsigned long long *)malloc(sizeof(unsigned long long)*topuser);
				for(iTemp=0;iTemp<topuser;iTemp++){
					lTop[iTemp]=0;
				}
				nasTimeGetDate(caSdate1,lTime1);
				nasTimeGetDate(caSdate2,lTime2);
				sprintf(sqlBuf,"select a.userid,b.dispname,sum(a.ubytes + a.dbytes)/1024+1,sum(a.ubytes)/1024+1 ,sum(a.dbytes)/1024+1 abyte,c.name from msaAccountReport a,msauser b,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,gid,work,inttopusernum);   
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==6){//部门汇总
				nasTimeGetDate(caSdate1,lTime1);
				nasTimeGetDate(caSdate2,lTime2);
				memset(oldgname,0,sizeof(oldgname));
				memset(egname,0,sizeof(egname));
				sprintf(sqlBuf,"select c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime) from msaAccountReport a,msauser b ,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid = b.userid and  b.groupid0=c.id and  b.groupid0 in(%s) and workflag=%d group by c.name order by mybytes desc",caSdate1,caSdate2,gid,work);
				tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		}else if(lUnit==7){//部门人员汇总
				nasTimeGetDate(caSdate1,lTime1);
				nasTimeGetDate(caSdate2,lTime2);
				memset(oldgname,0,sizeof(oldgname));
				memset(egname,0,sizeof(egname));  
				sprintf(sqlBuf,"select b.dispname,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime) from msaAccountReport a,msauser b ,msagroup c where a.sdate>= '%s' and a.sdate <= '%s' and a.userid = b.userid and b.groupid0=c.id  and b.groupid0 in(%s) and workflag=%d group by b.dispname,c.name,b.groupid0 order by b.groupid0,mybytes desc",caSdate1,caSdate2,gid,work);
				tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	  }else if(lUnit==8){//部门日期汇总
				nasTimeGetDate(caSdate1,lTime1);
				nasTimeGetDate(caSdate2,lTime2);
				memset(oldgname,0,sizeof(oldgname));
				memset(egname,0,sizeof(egname));		  
				sprintf(sqlBuf,"select a.sdate,c.name,sum(a.ubytes+dbytes)/1000+1 as mybytes,sum(a.ctime) from msaAccountReport a,msauser b ,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid = b.userid and  b.groupid0=c.id and b.groupid0 in(%s) and workflag=%d group by a.sdate,c.name order by a.sdate,mybytes desc",caSdate1,caSdate2,gid,work);
				tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		}  
    if(tempCursor == NULL) {
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
    		system(caLabel);
        return 0;
    }

    if(lUnit==1){
				lMaxBytes = 1;
				iMax = 0;
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
					sprintf(caItem,"%s",tempRow[0]);
					lUbytes=atoll(tempRow[1]);
					lDbytes=atoll(tempRow[2]);
					if(lMaxBytes <= (lUbytes + lDbytes) / 1024) {
					   lMaxBytes = (lUbytes + lDbytes) / 1024;
					}
					sprintf(sCount[iMax].caItem,"%s",caItem);
					sCount[iMax].lUbytes = lUbytes / 1024;
					sCount[iMax].lDbytes = lDbytes / 1024;
					sCount[iMax].lTbytes =sCount[iMax].lUbytes + sCount[iMax].lDbytes;		  
					iMax++;
					tempRow= ltDbFetchRow(tempCursor);
				}   
				ltDbCloseCursor(tempCursor);

				iMaxItem = iMax;
				dbPtr=lt_dbinit();  
				sprintf(caLabel,"%sdata.ini",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
				if(iFd==(-1)){
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
					system(caLabel);
					ltDbCloseCursor(tempCursor);
					return 0;
				}
			
				sprintf(caLabel,"%s--%s",caSdate,caEdate);
				lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
				lt_dbput_rootvars(dbPtr,1,"reportname","system"); 			   
			  lt_dbput_rootvars(dbPtr,1,"xunit","Day");		
				tablePtr=lt_dbput_table(dbPtr,"list2");
				for(i=0;i<iMaxItem;i++) {
					lt_dbput_recordvars(tablePtr,4,
						"mytime",LT_TYPE_STRING,sCount[i].caItem,
						"ubyte",LT_TYPE_LONGLONG,sCount[i].lUbytes,
						"dbyte",LT_TYPE_LONGLONG,sCount[i].lDbytes,
						"abyte",LT_TYPE_LONGLONG,sCount[i].lTbytes
					);
					sprintf(caLabel,"%s,%llu,%llu,%llu\r\n",sCount[i].caItem,sCount[i].lTbytes,sCount[i].lDbytes,sCount[i].lUbytes);
					write(iFd, caLabel, strlen(caLabel));	
				}
				close(iFd); 

				sprintf(caLabel,"%s/Scripts/outflowplt.pl  %sdata.ini %sflowreport11_1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
				system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"file1","flowreport11_1.jpg");
				sprintf(caLabel,"%s/Scripts/outflowpltline.pl  %sdata.ini %sflowreport22_1.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
				system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"file2","flowreport22_1.jpg");	    

				memset(htmlpage,0,sizeof(htmlpage));
				if(lang==0){//Ch
					sprintf(htmlpage,"%s%s/flowreportplt.htm",_msahtmldir,_dailyreport_ch);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}else if(lang==1){//Eng
					sprintf(htmlpage,"%s%s/flowreportplt_en.htm",_msahtmldir,_dailyreport_en);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}
 		}else if(lUnit==3 || lUnit==4  || lUnit==5 ){ //3用户总流量排序 4用户上流量排序 5用户下流量排序 
				printf("sql%d:%s\n",lUnit,sqlBuf);
				iTemp=0;
				lMax=0;
				dbPtr=lt_dbinit();
				lt_db_htmlpage(dbPtr,"utf-8");
				tablePtr=lt_dbput_table(dbPtr,"flowlist");
				tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
					lt_dbput_recordvars(tablePtr,6,
						"iIndex",LT_TYPE_LONG,iTemp,
						"uname",LT_TYPE_STRING,tempRow[1],
						"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
						"lubyte",LT_TYPE_LONGLONG,atoll(tempRow[3]),
						"ldbyte",LT_TYPE_LONGLONG,atoll(tempRow[4]),
						"groupname",LT_TYPE_STRING,tempRow[5]
					);
					if(lUnit==3){
						lTop[iTemp]=atoll(tempRow[2]);
					}else if(lUnit==4){
						lTop[iTemp]=atoll(tempRow[3]);
					}else if(lUnit==5){
						lTop[iTemp]=atoll(tempRow[4]);
					}
					if(lTop[iTemp]>lMax){
						lMax=lTop[iTemp];
					}
					iTemp++;			
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
				sprintf(caLabel,"%sdata.ini",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
				if(iFd == (-1)) {
					sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
					system(caLabel);/*建立目录和正在处理标志*/
					ltDbCloseCursor(tempCursor);
					return 0;
				}	
				for(i=0;i<iTemp;i++) {			
					sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
					write(iFd, caLabel, strlen(caLabel));			    
				}
				close(iFd);	
				sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %sflowreportt.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMax);
				system(caLabel);	
				lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreportt.gif\">" );
				
				{
					int i;
					sprintf(caLabel,"%sdata.ini",caTempDir);
					iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
					if(iFd == (-1)){
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
						system(caLabel);/*建立目录和正在处理标志*/
						ltDbCloseCursor(tempCursor);	
						return 0;
					}	
					for(i=0;i<iTemp;i++){ 
						sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
						write(iFd, caLabel, strlen(caLabel));	
					}
					close(iFd);	
					sprintf(caLabel,"%s/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport13.jpg %llu ",_msahtmldir,caTempDir,caTempDir,(unsigned long long)(lMax+1));
					system(caLabel);	
					lt_dbput_rootvars(dbPtr,1,"file1","flowreport13.jpg"); 
				}
				
				sprintf(caLabel,"%s--%s",caSdate,caEdate);
				lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
				lt_dbput_rootvars(dbPtr,1,"reportname","system"); 
				memset(htmlpage,0,sizeof(htmlpage));
				if(lang==0){				//Ch
					sprintf(htmlpage,"%s%s/userreportplt.htm",_msahtmldir,_dailyreport_ch);
					printf("htmlpage:%s\n",htmlpage);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}else if(lang==1){	//Eng
					sprintf(htmlpage,"%s%s/userreportplt_en.htm",_msahtmldir,_dailyreport_en);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}
    }else if(lUnit==6 || lUnit==7  || lUnit==8 ){ 
    		printf("sql%d:%s\n",lUnit,sqlBuf);	
				dbPtr=lt_dbinit();
				lt_db_htmlpage(dbPtr,"utf-8");
				tablePtr=lt_dbput_table(dbPtr,"flowlist");   
				tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
					if(lUnit==6 ){				
						lt_dbput_recordvars(tablePtr,3,
						"groupname",LT_TYPE_STRING,tempRow[0],
						"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[1]),
						"ltime",LT_TYPE_STRING,tempRow[2]);
						sprintf(oldgname,"%s",tempRow[0]);			  
					}else{			 
						if(strcmp(oldgname,tempRow[1])==0){
								memset(egname,0,sizeof(egname));
								memset(egname,' ',strlen(oldgname));
								lt_dbput_recordvars(tablePtr,4,
								"uname",LT_TYPE_STRING,tempRow[0],
								"groupname",LT_TYPE_STRING,egname,
								"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"ltime",LT_TYPE_STRING,tempRow[3]);
						}else{
						    lt_dbput_recordvars(tablePtr,4,
								"*",LT_TYPE_STRING,tempRow[0],
								"groupname",LT_TYPE_STRING,tempRow[1],
								"lbyte",LT_TYPE_STRING," ",
								"ltime",LT_TYPE_STRING," ");
								sprintf(oldgname,"%s",tempRow[1]);							
								memset(egname,0,sizeof(egname));
								memset(egname,' ',strlen(oldgname));
								lt_dbput_recordvars(tablePtr,4,
								"uname",LT_TYPE_STRING,tempRow[0],
								"groupname",LT_TYPE_STRING,egname,
								"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"ltime",LT_TYPE_STRING,tempRow[3]);
						}				
					}
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
				sprintf(caLabel,"%s--%s",caSdate,caEdate);
				lt_dbput_rootvars(dbPtr,1,"lab",caLabel);   
				lt_dbput_rootvars(dbPtr,1,"reportname","system"); 
				memset(htmlpage,0,sizeof(htmlpage));
				if(lang==0){				 //Ch
					sprintf(htmlpage,"%s%s/groupreportplt.htm",_msahtmldir,_dailyreport_ch);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}else if(lang==1){		//Eng
					sprintf(htmlpage,"%s%s/groupreportplt_en.htm",_msahtmldir,_dailyreport_en);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
				}   
	  }
 
    if(lt_page_content==NULL){
	    	sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
        system(caLabel);/*建立目录和正在处理标志*/
    		lt_dbfree(dbPtr);
        return 0;
		}else{
		    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		    printf("calabel:%s\n",caLabel);
		    sprintf(caLabel,"%sflow%d.htm",caTempDir,lUnit);
		    iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);
	  		/*生成html格式报告*/
    }
    lt_dbfree(dbPtr);

    return 0;
}


 
/*WEB 访问相关报告 */  
int webreport(char date1[36],char date2[36],char gid[2048],int oType,char caTempDir[128],int lang,int work)
{
	    printf("1111111111111111111\n");
	    ltDbCursor *tempCursor;
	    LT_DBROW tempRow;
	    ltDbHeadPtr dbPtr;
	    ltTablePtr tablePtr;
	    char xName[64];
	    char caSdate[32],caEdate[32];
	    char caSdate1[32],caSdate2[32];
	    char caTime1[32],caTime2[32];
	    char sqlBuf[1024];
	    char caLabel[128];
	    char   oldname[256];		
	    char   *lt_page_content=NULL;
	    char   htmlpage[512];
	    long 	 lTime,lTime1,lTime2;
	    int    iFd,iTemp; 
	            
	    struct xs_group{
	    	char strGroup[128];
				int  idGroup;
			};
			struct xs_group xmyGroup[1000];
			memset(oldname,0,sizeof(oldname));	  
			sprintf(sqlBuf,"select id,name from msagroup where id in (%s)",gid);
	    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	    printf("aaaaaaaaaaaaaaaaaaaaaaa\n");
	    if(tempCursor == NULL) {
	      return 0;
	    }
	    tempRow= ltDbFetchRow(tempCursor);
	    printf("222222222222222222222\n");
	    iTemp=0;
	    while(tempRow!=NULL){
	    	xmyGroup[iTemp].idGroup=atol(tempRow[0]);
	    	sprintf(xmyGroup[iTemp].strGroup,"%s",tempRow[1]);
	    	iTemp++;
	    	if(iTemp>1000){
	    		break;
	    	}
	    	tempRow= ltDbFetchRow(tempCursor);
	    }
	    ltDbCloseCursor(tempCursor); 
			sprintf(caSdate,"%s",date1);
	    sprintf(caEdate,"%s",date2);
	
	    tempCursor=NULL;
	    lTime = time(0);
	    if(!ltStrIsSpaces(caSdate)) {
	        lTime1 = nasCvtLongTime(caSdate,"00:00:00");
	    }else {
	        lTime1 = 0;
	    }
	    if(!ltStrIsSpaces(caEdate)) {
	        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
	    }else {
	        lTime2 = 0;
	    }
	    if(lTime1 == 0) {
	        lTime1 = lTime;
	    }
			if(lTime2 == 0) {
	        lTime2 = lTime;
	    }
			if(lTime1 > lTime) {
	        lTime1 = lTime;
	    }
	    if(lTime2>lTime){
					lTime2=lTime;
	    }
	    if(ltFileIsExist(caTempDir)!=1) {
	        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
	            return 0;
	        }
	    }    
	    nasCvtStime(lTime,caSdate1,caTime1);
	    caTime1[5]=0;
	    nasTimeGetDate(caSdate1,lTime1);
	    nasTimeGetDate(caSdate2,lTime2);
    printf("333333333333333333333\n");
			if(oType==1){	
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by c.name,a.host,a.urlsort order by acount desc limit 30",caSdate1,caSdate2,gid,work);
			}else if(oType==2){
					sprintf(sqlBuf,"select c.name,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in(%s) and workflag=%d group by c.name,a.host,a.urlsort order by acount desc",caSdate1,caSdate2,gid,work);
			}else if(oType==3){	
				  sprintf(sqlBuf,"select b.dispname,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b,msagroup c where a.sdate>= '%s' and a.sdate<= '%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%d group by a.userid, a.host,a.urlsort,b.dispname order by a.userid,acount desc",caSdate1,caSdate2,gid,work);
			}else if(oType==4){
				  sprintf(sqlBuf,"select a.sdate,a.host ,sum(a.bytes)/1024 as abyte,sum(a.ctime) as atime, sum(a.lcount) as acount,a.urlsort from msauserhttpreport a,msauser b where a.sdate>= '%s' and a.sdate<= '%s' and a.userid=b.userid  and b.groupid0 in (%s)  and workflag=%d group by a.sdate,a.host,a.urlsort order by a.sdate,acount desc",caSdate1,caSdate2,gid,work);      
		  }
		  printf("sq8:%s\n",sqlBuf);
   		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	    if(tempCursor == NULL) {
		    sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
	    	system(caLabel);/*建立目录和正在处理标志*/
	        return 0;
	    }
	    dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    tablePtr=lt_dbput_table(dbPtr,"list1");
	    tempRow= ltDbFetchRow(tempCursor);
	    iTemp=0;
	    while(tempRow!=NULL){     
	        if(strcmp(oldname,tempRow[0])==0){
	           if(iTemp<30){
							    lt_dbput_recordvars(tablePtr,5,
									"uname",LT_TYPE_STRING,tempRow[1],
									"lbyte",LT_TYPE_LONG,atol(tempRow[2]),
									"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
									"lrequest",LT_TYPE_STRING,tempRow[4],
									"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[5])].srvName);
				     }
					   iTemp++;				
				}else{
				    sprintf(oldname,"%s",tempRow[0]);
				    sprintf(xName,"<b>%s</b>",oldname);
				    lt_dbput_recordvars(tablePtr,4,
						"uname",LT_TYPE_STRING,xName,
						"lbyte",LT_TYPE_STRING,"",
						"ltime",LT_TYPE_STRING,"",
						"lrequest",LT_TYPE_STRING,"");
				   
				    lt_dbput_recordvars(tablePtr,5,
						"uname",LT_TYPE_STRING,tempRow[1],
						"lbyte",LT_TYPE_LONG,atol(tempRow[2]),
						"ltime",LT_TYPE_STRING,time2str(atol(tempRow[3])),
						"lrequest",LT_TYPE_STRING,tempRow[4],
						"usort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[5])].srvName);
				    iTemp=0;
				}
			tempRow= ltDbFetchRow(tempCursor);
	    }
	    ltDbCloseCursor(tempCursor);
			memset(caSdate1,0,sizeof(caSdate1));
	    memset(caSdate2,0,sizeof(caSdate2));
	    nasCvtStime(lTime1,caSdate1,caTime1);
	    nasCvtStime(lTime2,caSdate2,caTime2);
	    sprintf(caLabel,"%s--%s",caSdate1,caSdate2);
			lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
			lt_dbput_rootvars(dbPtr,1,"reportname","system");
			memset(htmlpage,0,sizeof(htmlpage));
			if(lang==0){						//Ch
				sprintf(htmlpage,"%s%s/webreportplt.htm",_msahtmldir,_dailyreport_ch);
			  lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}else if(lang==1){			//Eng
				sprintf(htmlpage,"%s%s/webreportplt_en.htm",_msahtmldir,_dailyreport_en);
				lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
			}
    
	    if(lt_page_content==NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
				system(caLabel);			/*建立目录和正在处理标志*/
				lt_dbfree(dbPtr);
				return 0;
			}else{
				lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
				sprintf(caLabel,"%sweb%d.htm",caTempDir,oType);  
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
				if(iFd >0) {
				  write(iFd,lt_page_content,strlen(lt_page_content));
				} 
		    close(iFd);
	    }
	   
	    lt_dbfree(dbPtr);
	    return 0;
}

