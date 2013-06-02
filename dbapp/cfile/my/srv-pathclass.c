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
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <dirent.h>
#include <stdarg.h>
#include <iconv.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "msa.h"

int ltqosdownlink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char 				sqlBuf[1024];
	char        BANDDOWNLINK[24];
	char        myRate[24];
	//char        mySqf[24];
	int         mybrate;
	char        tmpDef[64];
	int         qosid;

	//int         myprio;
	char        strprio[64];


	
	if(_bcKernelHead->banddownlink<1 ||_bcKernelHead->banduplink<1 ){
		  char    BANDUPLINK[24];
			char    BANDDOWNLINK[24];
		  memset(BANDDOWNLINK,0,sizeof(BANDDOWNLINK));
		 	bcCnfGetValue_s("/etc/gnm/qos/settings","BANDDOWNLINK",BANDDOWNLINK);
	    memset(BANDUPLINK,0,sizeof(BANDUPLINK));
			bcCnfGetValue_s("/etc/gnm/qos/settings","BANDUPLINK",BANDUPLINK);	
			if(strlen(BANDUPLINK)>0 && strlen(BANDDOWNLINK)>0 ){
				_bcKernelHead->banddownlink=atol(BANDDOWNLINK);
				_bcKernelHead->banduplink=atol(BANDUPLINK);
			}
		  if(_bcKernelHead->banddownlink<1 ||_bcKernelHead->banduplink<1 ){
					ltbandlink(confd,ltMsgPk,lt_MMHead);
					return 0;
		  }
	}
	qosid=0;
	if(ltMsgGetVar_s(ltMsgPk,"qosid")!=NULL){
		qosid=atol(ltMsgGetVar_s(ltMsgPk,"qosid"));
	}
	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  if(qosid!=0){
	  	/*sid          int   primary key,
	    level          int   default 0,
	    qosname        char(128) default '',
	    qosrated       int default 0,  --下载带宽数值峰值
	    qosrateu       int default 0,  --上传带宽数值峰值
	    qosmixd        int default 0,  --下载带宽保证
	    qosmixu        int default 0,  --上传带宽保证
	    prio           int default  0*/
		  sprintf(sqlBuf,"select sid,qosname,qosrated,qosrateu,qosmixd,qosmixu,prio,level from msaqos where sid=%d",qosid);
			
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lt_dbput_rootvars(dbPtr,1,"sid",tempRow[0]);
					lt_dbput_rootvars(dbPtr,1,"qosname",tempRow[1]);	
				  lt_dbput_rootvars(dbPtr,1,"qosrate",tempRow[2]);	//下载带宽峰值
				  lt_dbput_rootvars(dbPtr,1,"qosceil",tempRow[3]);	//上传带宽峰值
				  
				  lt_dbput_rootvars(dbPtr,1,"qostype",tempRow[4]); //下载带宽保证
				  lt_dbput_rootvars(dbPtr,1,"sqf",tempRow[5]);	//上传带宽保证
				  
				  
				  
//				  myprio=atol(tempRow[6]);
//				  if(myprio<0 || myprio>8){
//				  	myprio=0;
//				  }
//				  sprintf(strprio,"selp%d",myprio);
//					lt_dbput_rootvars(dbPtr,1,strprio," selected " );	
//					myprio=atol(tempRow[6]);
//				  if(myprio<1 || myprio>2){
//				  	myprio=2;
//				  }
//				  sprintf(strprio,"check%d",myprio);
//				  lt_dbput_rootvars(dbPtr,1,strprio," checked " );	
//				  myprio=atol(tempRow[6]);
//				  if(myprio<1 || myprio>2){
//				  	myprio=1;
//				  }
//				  sprintf(strprio,"sel%d",myprio);
				  
				  if(atol(tempRow[7])==0){
				  	lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
				  }else if(atol(tempRow[7])==1){
				  	lt_dbput_rootvars(dbPtr,1,"linksel1"," selected " );	
				  }else if(atol(tempRow[7])==2){
				  	lt_dbput_rootvars(dbPtr,1,"linksel2"," selected " );	
				  }else if(atol(tempRow[7])==3){
				  	lt_dbput_rootvars(dbPtr,1,"linksel3"," selected " );	
				  }else{
				    lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
				  }
				  
				  //lt_dbput_rootvars(dbPtr,1,strprio," selected " );	
				  
					tempRow= ltDbFetchRow(tempCursor);
				}else{

				  lt_dbput_rootvars(dbPtr,1,"qosname","NewQos" );	
				  lt_dbput_rootvars(dbPtr,1,"qosrate","0" );	
				  lt_dbput_rootvars(dbPtr,1,"qosceil","0" );
				  lt_dbput_rootvars(dbPtr,1,"sid","0" );	
				  lt_dbput_rootvars(dbPtr,1,"qostype","0" );	
	        lt_dbput_rootvars(dbPtr,1,"sqf","0" );	
				  lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
			  }
				ltDbCloseCursor(tempCursor);
			}else{
	
				  lt_dbput_rootvars(dbPtr,1,"qosname","NewQos" );	
				  lt_dbput_rootvars(dbPtr,1,"qosrate","0" );	
				  lt_dbput_rootvars(dbPtr,1,"qosceil","0" );
				  lt_dbput_rootvars(dbPtr,1,"sid","0" );	
				  lt_dbput_rootvars(dbPtr,1,"qostype","0" );	
	        lt_dbput_rootvars(dbPtr,1,"sqf","0" );	
				  lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );			  
		 }
		 
  }else{
	  lt_dbput_rootvars(dbPtr,1,"qosname","NewQos" );	
	  lt_dbput_rootvars(dbPtr,1,"qosrate","0" );	
	  lt_dbput_rootvars(dbPtr,1,"qosceil","0" );	
	  lt_dbput_rootvars(dbPtr,1,"sid","0" );	
	  lt_dbput_rootvars(dbPtr,1,"qostype","0" );	
	  lt_dbput_rootvars(dbPtr,1,"sqf","0" );	
	  lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
	}

  sprintf(BANDDOWNLINK,"%lu",_bcKernelHead->banddownlink);
  lt_dbput_rootvars(dbPtr,1,"DOWNLINK",BANDDOWNLINK);
  
  sprintf(BANDDOWNLINK,"%lu",_bcKernelHead->banduplink);
  lt_dbput_rootvars(dbPtr,1,"UPLINK",BANDDOWNLINK);
  
  
  tablePtr=lt_dbput_table(dbPtr,"dqoslist");
  sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
//			if(atol(tempRow[2])>0){
//					sprintf(tmpDef,"<b>%s[def]</b>",tempRow[1]);
//			}else{
//			    sprintf(tmpDef,"%s",tempRow[1]);
//		  }
			sprintf(tmpDef,"%s",tempRow[1]);
			lt_dbput_recordvars(tablePtr,2,
				"dsid",LT_TYPE_STRING,tempRow[0],
				"dqosname",LT_TYPE_STRING,tmpDef);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	

  mybrate=0;
  tablePtr=lt_dbput_table(dbPtr,"bqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrated,qosmixd from msaqos where sid>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){		
			mybrate=mybrate+atol(tempRow[2]);
			sprintf(myRate,"%d",(int)( (atol(tempRow[2])*100)/_bcKernelHead->banddownlink) );									
			lt_dbput_recordvars(tablePtr,7,
				"bqosname",LT_TYPE_STRING,tempRow[1], //通道名称
				"bqosrate",LT_TYPE_STRING,tempRow[2],	//通道速率  配置
				"brate",LT_TYPE_STRING,myRate,				//通道速率  百分比
				"bqosceil",LT_TYPE_STRING,tempRow[3],	//通道峰值
				"bprio",LT_TYPE_STRING,"",						
				"btype",LT_TYPE_STRING,"保证",
				"bsqf",LT_TYPE_STRING,"");
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	if( _bcKernelHead->banddownlink>(mybrate+_bcKernelHead->banddowndef/1000)){
	   sprintf(myRate,"%lu", _bcKernelHead->banddownlink-mybrate );
	   lt_dbput_rootvars(dbPtr,1,"shareqos",myRate);	//kb
	   
	   sprintf(myRate,"%d", (int)(((_bcKernelHead->banddownlink-mybrate)*100)/_bcKernelHead->banddownlink)  );
	   lt_dbput_rootvars(dbPtr,1,"sharerate",myRate);	//%
  }else{
		 sprintf(myRate,"%d", _bcKernelHead->banddowndef/1000 );
		 lt_dbput_rootvars(dbPtr,1,"shareqos",myRate);	//kb
		 
	   sprintf(myRate,"%d", (int)(((_bcKernelHead->banddowndef/1000)*100)/_bcKernelHead->banddownlink)  );
	   lt_dbput_rootvars(dbPtr,1,"sharerate",myRate);	//%
	}
	
		  
 
  tablePtr=lt_dbput_table(dbPtr,"sqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrated,qosrated from msaqos where sid>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_bcKernelHead->banddownlink) );						
			lt_dbput_recordvars(tablePtr,7,
				"sqosname",LT_TYPE_STRING,tempRow[1],
				"sqosrate",LT_TYPE_STRING,tempRow[2],
				"srate",LT_TYPE_STRING,myRate,
				"sqosceil",LT_TYPE_STRING,tempRow[3],
				"sprio",LT_TYPE_STRING,"",
				"stype",LT_TYPE_STRING,"竞争",
				"ssqf",LT_TYPE_STRING,"");
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	
	/*shangchuang*/


  mybrate=0;
  tablePtr=lt_dbput_table(dbPtr,"upbqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrateu,qosmixu from msaqos where sid>0 and qosrateu>0");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			mybrate=mybrate+atol(tempRow[2]);
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_bcKernelHead->banduplink) );										
			lt_dbput_recordvars(tablePtr,7,
				"upbqosname",LT_TYPE_STRING,tempRow[1],
				"upbqosrate",LT_TYPE_STRING,tempRow[2],
				"upbrate",LT_TYPE_STRING,myRate,
				"upbqosceil",LT_TYPE_STRING,tempRow[3],
				"upbprio",LT_TYPE_STRING,"",
				"upbtype",LT_TYPE_STRING,"保证",
				"upbsqf",LT_TYPE_STRING,"");
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	if( _bcKernelHead->banduplink >(mybrate+_bcKernelHead->bandupdef/1000)){
	   	sprintf(myRate,"%lu", _bcKernelHead->banduplink-mybrate );
	   	lt_dbput_rootvars(dbPtr,1,"upshareqos",myRate);	
			sprintf(myRate,"%d", (int)(((_bcKernelHead->banduplink-mybrate)*100)/_bcKernelHead->banduplink)  );
			lt_dbput_rootvars(dbPtr,1,"upsharerate",myRate);
  }else{
		 sprintf(myRate,"%d", _bcKernelHead->bandupdef/1000 );
		 lt_dbput_rootvars(dbPtr,1,"upshareqos",myRate);	
		 sprintf(myRate,"%d", (int)(((_bcKernelHead->bandupdef/1000)*100)/_bcKernelHead->banduplink)  );
		 lt_dbput_rootvars(dbPtr,1,"upsharerate",myRate);
	}

  tablePtr=lt_dbput_table(dbPtr,"upsqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrateu,qosrateu from msaqos where sid>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_bcKernelHead->banduplink) );									
			lt_dbput_recordvars(tablePtr,7,
				"upsqosname",LT_TYPE_STRING,tempRow[1],
				"upsqosrate",LT_TYPE_STRING,tempRow[2],
				"upsrate",LT_TYPE_STRING,myRate,
				"upsqosceil",LT_TYPE_STRING,tempRow[3],
				"upsprio",LT_TYPE_STRING,"",
				"upstype",LT_TYPE_STRING,"竞争",
				"upssqf",LT_TYPE_STRING,"");
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	
  
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/bandconf.html",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
	
}



int ltupdateqos(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char 				sqlBuf[1024];
	char        *qosname;
	char        *qosrate;
	char        *prio;
	char        *qostype;
	char        *qosceil;
	char        *sqf;
	char        *qoslinktype;
	long 				lCount;
	int         qosid;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	
  qosid=atol(ltMsgGetVar_s(ltMsgPk,"qosid"));
	
  qosname=ltMsgGetVar_s(ltMsgPk,"qosname");
	if(qosname==NULL){
		qosname="";
	}
	
	qosrate=ltMsgGetVar_s(ltMsgPk,"qosrate");//下载带宽峰值
	if(qosrate==NULL){
		qosrate="0";
	}
	
	prio=ltMsgGetVar_s(ltMsgPk,"prio");
	if(prio==NULL){
		prio="0";
	}
	
	qostype=ltMsgGetVar_s(ltMsgPk,"qostype");//下载带宽保证
	if(qostype==NULL){
		qostype="0";
	}
	
	qosceil=ltMsgGetVar_s(ltMsgPk,"qosceil");//上传带宽峰值qosrateu
	if(qosceil==NULL){
		qosceil="0";
	}
	
	sqf=ltMsgGetVar_s(ltMsgPk,"sqf");//上传带宽保证
	if(sqf==NULL){
		sqf="0";
	}
	
	qoslinktype=ltMsgGetVar_s(ltMsgPk,"qoslinktype");//有竞争时选择
	if(qoslinktype==NULL){
		qoslinktype="0";
	}
	
	
	if(ltStrIsDigital(qosrate)==0){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
					"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >带宽数值不对！ </body></html>");		
				return 0;
	}
	if(ltStrIsDigital(qosceil)==0){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
					"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >带宽数值不对！ </body></html>");		
				return 0;
	}
	if(atol(qosrate)<2){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
					"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >带宽数值太低，必须大于1K！ </body></html>");		
				return 0;
	}
	if(atol(qosceil)<2){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
					"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >带宽数值太低，必须大于1K！ </body></html>");		
				return 0;
	}
	
	if(qosid>0){	
			sprintf(sqlBuf,"update msaqos set qosname='%s',qosrated=%s, qosrateu=%s, qosmixd=%s,qosmixu=%s,prio=%s,level=%s where sid=%d",
			qosname,qosrate,qosceil,qostype,sqf,prio,qoslinktype,qosid);

	    ltDbExecSql(G_DbCon,sqlBuf); 
		  ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	    return 0;
	}
	qosid=1;
	while(1){
			sprintf(sqlBuf,"select count(*) from msaqos where sid=%d ",qosid);
			lCount=0;
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow!=NULL){
								lCount=atol(tempRow[0]);
							}
							ltDbCloseCursor(tempCursor);
			}
			if(lCount == 0) {
					break;
			}else{
		  		qosid++;
		  }
		  if(qosid>998){
		  	ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
					"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >通道超出系统限制！ </body></html>");		
				return 0;
		  }
	}

	
		/*sid            int   primary key,
	    level          int   default 0,
	    qosname        char(128) default '',
	    qosrated       int default 0,  --下载带宽数值峰值
	    qosrateu       int default 0,  --上传带宽数值峰值
	    qosmixd        int default 0,  --下载带宽保证
	    qosmixu        int default 0,  --上传带宽保证
	    prio           int default  0*/
	sprintf(sqlBuf,"insert into msaqos(sid,qosname,qosrated,qosrateu,qosmixd,qosmixu,prio,level) values (%ld,'%s',%s,%s,%s,%s,%s,%s)",
			qosid,qosname,qosrate,qosceil,qostype,sqf,prio,qoslinktype);
		
	printf("sql;%s\n",sqlBuf);
	ltDbExecSql(G_DbCon,sqlBuf); 
	
  ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	return 0;
	


}





int lteditqos(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char 				sqlBuf[1024];
	int         qosid;
	char        *edittype;
	int         qoslinktype;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
		
	qosid=atol(ltMsgGetVar_s(ltMsgPk,"qosid"));
	edittype=ltMsgGetVar_s(ltMsgPk,"edittype");//Edit  Del  APP
	
	if(qosid==0){
		   ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: this qos rule can't be chanage!</body></html>");		
				ltMsgFree(ltMsgPk);
			
		    return 0;
	}
	
	if( strcmp(edittype,"setdef")==0){
//		qoslinktype=0;
//		sprintf(sqlBuf,"select qoslinktype from msaqos where sid=%d",qosid);
//		qoslinktype=0;
//			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
//			if(tempCursor!=NULL){
//						tempRow= ltDbFetchRow(tempCursor);
//						if(tempRow!=NULL){
//							qoslinktype=atol(tempRow[0]);
//						}
//						ltDbCloseCursor(tempCursor);
//			}
//		
//		sprintf(sqlBuf,"update  msaqos set qosdefault=0 where qoslinktype=%d ",qoslinktype);
//		printf("sqlbuf:%s\n",sqlBuf);
//	  ltDbExecSql(G_DbCon,sqlBuf); 
//		sprintf(sqlBuf,"update  msaqos set qosdefault=1 where sid=%d and  qoslinktype=%d ",qosid,qoslinktype);
//	  printf("sqlbuf:%s\n",sqlBuf);
//	  ltDbExecSql(G_DbCon,sqlBuf); 
//	  ltqosdownlink(confd,ltMsgPk,lt_MMHead);
//	  return 0;
	  
	}else if( strcmp(edittype,"Del")==0){
//		int lCount;
//		lCount=0;		
//		sprintf(sqlBuf,"select count(*) from nasqosmatch where qosid=%d ",qosid);
//		
//		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
//				if(tempCursor!=NULL){
//						tempRow= ltDbFetchRow(tempCursor);
//						if(tempRow!=NULL){
//							lCount=atol(tempRow[0]);
//						}
//						ltDbCloseCursor(tempCursor);
//				}
//		if(lCount == 0) {
//			sprintf(sqlBuf,"select count(*) from nasqosmatchup  where qosid=%d ",qosid);
//			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
//				if(tempCursor!=NULL){
//						tempRow= ltDbFetchRow(tempCursor);
//						if(tempRow!=NULL){
//							lCount=atol(tempRow[0]);
//						}
//						ltDbCloseCursor(tempCursor);
//				}
//		}
//		if(lCount>0){
//			 ltMsgPk->msgpktype=1;
//				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
//			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: this qos rule being used!</body></html>");		
//				ltMsgFree(ltMsgPk);
//			
//		    return 0;
//			
//		}
		sprintf(sqlBuf,"delete from msaqos where sid=%d ",qosid);
	  ltDbExecSql(G_DbCon,sqlBuf); 
		
		ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	  return 0;
	  
	}else if( strcmp(edittype,"APP")==0){
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><head><meta http-equiv=\"refresh\" content=\"20;URL=/msa/main.xp?Fun=ltnetarealist\" ></head><body bgcolor=\"#cccccc\" >Please try  20 seconds later.......</body></html>");		
	  system("killall -2 procasy");
	  ltMsgFree(ltMsgPk);
	  return 0;
	}
	
	
	if(_ltPubInfo->banddownlink<1){
		ltbandlink(confd,ltMsgPk,lt_MMHead);
		return 0;
	}
	
	ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	return 0;
	

}


