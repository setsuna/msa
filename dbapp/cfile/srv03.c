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
#include <stdarg.h>

#include "nas.h"
#include "lber.h"
#include "ldap.h"

/*1111*/


/*
netarea 控制
*/

int nasTimeSetF(char *pFormat,long lTime)
{
    struct tm * sTim;
    long lTemp;
    sTim = localtime(&lTime);

    lTemp = sTim->tm_year;
    if(lTemp > 100) lTemp = lTemp - 100;
    if(lTemp<10){
    	sprintf(pFormat,"200%ld",lTemp);
    }else{
			sprintf(pFormat,"20%ld",lTemp);
    }
    lTemp = sTim->tm_mon + 1;
     if(lTemp<10){
    	sprintf(pFormat,"%s.0%ld",pFormat,lTemp);
    }else{
			sprintf(pFormat,"%s.%ld",pFormat,lTemp);
    }

    lTemp = sTim->tm_mday;
     if(lTemp<10){
    	sprintf(pFormat,"%s.0%ld",pFormat,lTemp);
    }else{
			sprintf(pFormat,"%s.%ld",pFormat,lTemp);
    }

    lTemp = sTim->tm_hour;
    if(lTemp<10){
			sprintf(pFormat,"%s-0%ld",pFormat,lTemp);  
    }else{
			sprintf(pFormat,"%s-%ld",pFormat,lTemp);  
    }

    lTemp = sTim->tm_min;
    if(lTemp<10){
	sprintf(pFormat,"%s:0%ld",pFormat,lTemp);  
    }else{
	sprintf(pFormat,"%s:%ld",pFormat,lTemp);  
    }


    lTemp = sTim->tm_sec;
    if(lTemp<10){
	sprintf(pFormat,"%s:0%ld",pFormat,lTemp);  
    }else{
	sprintf(pFormat,"%s:%ld",pFormat,lTemp);  
    }      
    
    return 0;
}


int confReplace(char *strConf,char *strPar,char *strVar){
	
	
	char tmpStr[128];
	sprintf(tmpStr,"/bin/echo '%s=%s'>%s/%s ",strPar,strVar,strConf,strPar);
	system(tmpStr);
	return 0;
	
	
}

int confReplaceI(char *strConf,char *strPar,int strVar){
	
	char tmpStr[128];
	sprintf(tmpStr,"/bin/echo '%s=%d'>%s/%s ",strPar,strVar,strConf,strPar);
	system(tmpStr);
	return 0;
	
	
}


int ltconfhead(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char strTmp[8012];
	

	sprintf(strTmp,"%s",
	"['系统参数设定', '',\r\n		['线上注册', '/main/main.xp?Fun=ltsysregist'],\r\n		['注册码验证', '/main/main.xp?Fun=nasonlinechkcli'],\r\n		['系统服务设定', '/main/main.xp?Fun=ltsyssrvconf'],\r\n		['Network参数设定', '/main/netconnectconf.htm'],\r\n		['Network路由设定', '/main/main.xp?Fun=ltsysrouteconf'],\r\n		['监视参数设定', '/main/main.xp?Fun=ltappconfig'],\r\n		['系统功能设定', '/main/main.xp?Fun=ltsysvarconfig'],\r\n	 	['web代理设置', '/main/main.xp?Fun=ltwebproxyconf'],\r\n		['邮件发送设定', '/main/main.xp?Fun=ltsyssmtpconf'],\r\n		['邮件监控设定', '/main/nasmailfilter.htm'],\r\n		['表单監控设定', '/main/nasupfilter.htm'],\r\n		['系统URL资料库', '/main/main.xp?Fun=lturlsortlist'],\r\n		['时间策略设定', '/main/main.xp?Fun=ltschinfolist'],\r\n		['带宽策略设定', '/main/bandsetting.htm'],\r\n	['入侵检测', '/main/idssetting.htm'],\r\n	['计费费率设定', '/main/main.xp?Fun=ltjfrate'],\r\n		['服务类型设定', '/main/nassrvlist.htm'],\r\n		['系统访问限制设定', '/main/main.xp?Fun=ltpkgfilterlist']\r\n    ],\r\n");
	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
  if(_ltPubInfo->defaultlan==2){
	        	ltCvtGbToBig5(strTmp,strlen(strTmp));
	}
  
	lt_dbput_rootvars(dbPtr,1,"conf",strTmp);	
	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/adduser/confhead.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;


}



int ltsysvarconfig(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	
	
	char strTmp[128];
	int  iTmp;
	char strqqnoweb[24];
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	

	if(_ltPubInfo->lMacBindIp==0){
			lt_dbput_rootvars(dbPtr,1,"macbindip","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"macbindip"," checked ");	
	}

  iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","option","logonfixip"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"logonfixip","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"logonfixip"," checked ");	
	}
  
  iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","option","procmailpackage"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"procmailpackage","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"procmailpackage"," checked ");	
	}
	
	 iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","option","procwebpackage"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"procwebpackage","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"procwebpackage"," checked ");	
	}
	
	iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","system","controlSocketFd"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"controlSocketFd","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"controlSocketFd"," checked ");	
	}
	
	 iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","system","udpcontrolSocketFd"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"udpcontrolSocketFd","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"udpcontrolSocketFd"," checked ");	
	}

	
	if(_ltPubInfo->arpservice==0){
			lt_dbput_rootvars(dbPtr,1,"arpservice","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"arpservice"," checked ");	
	}
	
	if(_ltPubInfo->ipmaccheck==0){
			lt_dbput_rootvars(dbPtr,1,"ipmaccheck","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ipmaccheck"," checked ");	
	}
	
	sprintf(strTmp,"%lu",_ltPubInfo->alertarpnum);
	lt_dbput_rootvars(dbPtr,1,"alertarpnum",strTmp);	

  if(_ltPubInfo->checkcontype==0){
			lt_dbput_rootvars(dbPtr,1,"checkcontype","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"checkcontype"," checked ");	
	}

//  if(_ltPubInfo->flowlimit==0){
//			lt_dbput_rootvars(dbPtr,1,"flowlimit","");	
//	}else{
//		lt_dbput_rootvars(dbPtr,1,"flowlimit"," checked ");	
//	}
//  
//  if(_ltPubInfo->timelimit==0){
//			lt_dbput_rootvars(dbPtr,1,"timelimit","");	
//	}else{
//		lt_dbput_rootvars(dbPtr,1,"timelimit"," checked ");	
//	}



	if(_ltPubInfo->checkcon==0){
			lt_dbput_rootvars(dbPtr,1,"checkcon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"checkcon"," checked ");	
	}

	if(_ltPubInfo->lSrvRuleChk==0){
			lt_dbput_rootvars(dbPtr,1,"enablerule","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"enablerule"," checked ");	
	}
	
	if(_ltPubInfo->accountchk==0){
			lt_dbput_rootvars(dbPtr,1,"enableaccount","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"enableaccount"," checked ");	
	}

	if(_ltPubInfo->lWebRuleChk==0){
			lt_dbput_rootvars(dbPtr,1,"enablewebrule","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"enablewebrule"," checked ");	
	}

	if(_ltPubInfo->lWebPostRuleChk==0){
			lt_dbput_rootvars(dbPtr,1,"enablewebpostrule","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"enablewebpostrule"," checked ");	
	}

  if(_ltPubInfo->controltype!=2){
			lt_dbput_rootvars(dbPtr,1,"controltype","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"controltype"," checked ");	
	}

	if(_ltPubInfo->qqnoweb==0){
			lt_dbput_rootvars(dbPtr,1,"qqnoweb","0");	
	}else{
	  sprintf(strqqnoweb,"%d",_ltPubInfo->qqnoweb);
		lt_dbput_rootvars(dbPtr,1,"qqnoweb",strqqnoweb);	
	}

  if(_ltPubInfo->defaultsort==0){
			lt_dbput_rootvars(dbPtr,1,"defaultsort","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"defaultsort"," checked ");	
	}
	
	if(_ltPubInfo->authtype==1){
			lt_dbput_rootvars(dbPtr,1,"authtypechk1"," selected ");
	}else if(_ltPubInfo->authtype==2){
			lt_dbput_rootvars(dbPtr,1,"authtypechk2"," selected ");	
	}else if(_ltPubInfo->authtype==3){
			lt_dbput_rootvars(dbPtr,1,"authtypechk3"," selected ");	
	}else {
		lt_dbput_rootvars(dbPtr,1,"authtypechk1"," selected ");	
	}
	


	if(_ltPubInfo->daytimetype==0){
			lt_dbput_rootvars(dbPtr,1,"daytypesel0"," selected　");	
	}else if(_ltPubInfo->daytimetype==1) {
		lt_dbput_rootvars(dbPtr,1,"daytypesel1"," selected ");	
	}else if(_ltPubInfo->daytimetype==2) {
		lt_dbput_rootvars(dbPtr,1,"daytypesel2"," selected ");	
	}else{
			lt_dbput_rootvars(dbPtr,1,"daytypesel0"," selected　");	
	}
	

	
	if(_ltPubInfo->msnnofujian==0){
			lt_dbput_rootvars(dbPtr,1,"msnnofujian","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"msnnofujian"," checked ");	
	}
	
	if(_ltPubInfo->userdispip==0){
			lt_dbput_rootvars(dbPtr,1,"userdispip","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"userdispip"," checked ");	
	}
	
	
	if(_ltPubInfo->attpreview==0){
			lt_dbput_rootvars(dbPtr,1,"attpreview","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"attpreview"," checked ");	
	}


  if(_ltPubInfo->defaultadduser==0){
			lt_dbput_rootvars(dbPtr,1,"defaultadduser","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"defaultadduser"," checked ");	
	}

   iTmp =atol(bcCnfGetValueS("/etc/gnm/conf","option", "keeponlineuser"));
 	if(iTmp==0){
			lt_dbput_rootvars(dbPtr,1,"keeponlineuser","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"keeponlineuser"," checked ");	
	}
  
	
	sprintf(strTmp,"%lu",_ltPubInfo->userTimeOut);
	lt_dbput_rootvars(dbPtr,1,"usertimeout",strTmp);	
	
	sprintf(strTmp,"%lu",_ltPubInfo->usermaxtime);
	lt_dbput_rootvars(dbPtr,1,"usermaxtime",strTmp);	
	

 	if(_ltPubInfo->lmon==0){
			lt_dbput_rootvars(dbPtr,1,"localmon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"localmon"," checked ");	
	}
  
  if(_ltPubInfo->nopicblock==0){
			lt_dbput_rootvars(dbPtr,1,"nopicblock","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"nopicblock"," checked ");	
	}
  
   if(_ltPubInfo->mailbind!=2){
			lt_dbput_rootvars(dbPtr,1,"mailbind","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"mailbind"," checked ");	
	}
  
	lt_dbput_rootvars(dbPtr,1,"localdomain1",_ltPubInfo->ldomain1);	
	lt_dbput_rootvars(dbPtr,1,"localdomain2",_ltPubInfo->ldomain2);	
	
	lt_dbput_rootvars(dbPtr,1,"localdomain3",_ltPubInfo->ldomain3);	
	lt_dbput_rootvars(dbPtr,1,"localdomain4",_ltPubInfo->ldomain4);	
	
	if(_ltPubInfo->ltPPOEAction==0){
			lt_dbput_rootvars(dbPtr,1,"ppoechk0"," selected ");
	}else if(_ltPubInfo->ltPPOEAction==1){
			lt_dbput_rootvars(dbPtr,1,"ppoechk1"," selected ");
	}else if(_ltPubInfo->ltPPOEAction==2){
			lt_dbput_rootvars(dbPtr,1,"ppoechk2"," selected ");	
	}else {
			lt_dbput_rootvars(dbPtr,1,"ppoechk3"," selected ");
	}
	
	
	if(_ltPubInfo->autobind==1){
			lt_dbput_rootvars(dbPtr,1,"autobind"," checked ");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"autobind"," ");	
	}
	
	
	if(_ltPubInfo->nomutidown==1){
			lt_dbput_rootvars(dbPtr,1,"nomutidownchk"," checked ");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"nomutidownchk"," ");	
	}
  
  sprintf(strTmp,"bind%d",_ltPubInfo->defaultbind);
	lt_dbput_rootvars(dbPtr,1,strTmp," selected ");	
	
	
	
	{
		
		char    *pFile="/etc/conf.ini";
		char    strdbcheck[12];
	  char    denyurl[128];
	  sprintf(denyurl,"%f",_ltPubInfo->flowrate*1000000);
	  lt_dbput_rootvars(dbPtr,1,"flowrate",denyurl);	
	  sprintf(denyurl,"%f",_ltPubInfo->timerate*3600);
	  lt_dbput_rootvars(dbPtr,1,"timerate",denyurl);
	  
		memset(denyurl,0,sizeof(denyurl));
	  bcCnfGetValue_s(pFile,"denyurl",denyurl);

	  lt_dbput_rootvars(dbPtr,1,"denyurl",denyurl);
	  
	  memset(strdbcheck,0,sizeof(strdbcheck));
		bcCnfGetValue_s(pFile,"DBCHECK",strdbcheck);
		if( strcmp(strdbcheck,"y")==0){	
			lt_dbput_rootvars(dbPtr,1,"dbchecksel","checked");
		}
	
	}
	
	lt_dbput_rootvars(dbPtr,1,"homeurl",_ltPubInfo->strAuthOKUrl);
	
	if(_ltPubInfo->onlycheckauthurl==0){
			lt_dbput_rootvars(dbPtr,1,"onlycheckauthurl","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"onlycheckauthurl"," checked ");	
	}
	
	{
		char snmpcommunity[32];
		char snmpsip[32];
		char snmpoid[128];
		char snmpv[12];
		int  snmpebable;
		char *p;
		memset(snmpcommunity,0,sizeof(snmpcommunity));
		memset(snmpsip,0,sizeof(snmpsip));
		memset(snmpoid,0,sizeof(snmpoid));
		memset(snmpv,0,sizeof(snmpv));
		snmpebable=0;
		snmpebable=bcCnfGetValueL("/etc/gnm/conf","option","snmpebable");
	  p=bcCnfGetValueS("/etc/gnm/conf","option", "snmpcommunity");
		if(p!=NULL){
							sprintf(snmpcommunity,"%s",p);
		} 
		p=bcCnfGetValueS("/etc/gnm/conf","option", "snmpv");
		if(p!=NULL){
							sprintf(snmpv,"%s",p);
		} 
		p=bcCnfGetValueS("/etc/gnm/conf","option", "snmpsip");
		if(p!=NULL){
							sprintf(snmpsip,"%s",p);
		} 
		p=bcCnfGetValueS("/etc/gnm/conf","option", "snmpoid");
		if(p!=NULL){
							sprintf(snmpoid,"%s",p);
		} 
    if(snmpebable==1){
    	lt_dbput_rootvars(dbPtr,1,"snmpebable"," checked ");	

		}else{
			lt_dbput_rootvars(dbPtr,1,"snmpebable","");	
		}
		lt_dbput_rootvars(dbPtr,1,"snmpcommunity",snmpcommunity);	
		lt_dbput_rootvars(dbPtr,1,"snmpsip",snmpsip);	
		lt_dbput_rootvars(dbPtr,1,"snmpv",snmpv);	
		lt_dbput_rootvars(dbPtr,1,"snmpoid",snmpoid);	
		
		int myiii=1;
		for(myiii=1;myiii<NAS_MAXSNMPNUM;myiii++){
			  char cammm[32];
			  sprintf(cammm,"snmpcommunity%d",myiii+1);
				p=bcCnfGetValueS("/etc/gnm/conf","option", cammm);
				if(p!=NULL){
									sprintf(snmpcommunity,"%s",p);
									lt_dbput_rootvars(dbPtr,1,cammm,snmpcommunity);	
				} 
				sprintf(cammm,"snmpv%d",myiii+1);
				p=bcCnfGetValueS("/etc/gnm/conf","option", cammm);
				if(p!=NULL){
									sprintf(snmpv,"%s",p);
									lt_dbput_rootvars(dbPtr,1,cammm,snmpv);	
				} 
				sprintf(cammm,"snmpsip%d",myiii+1);
				p=bcCnfGetValueS("/etc/gnm/conf","option", cammm);
				if(p!=NULL){
									sprintf(snmpsip,"%s",p);
									lt_dbput_rootvars(dbPtr,1,cammm,snmpsip);	
				} 
				sprintf(cammm,"snmpoid%d",myiii+1);
				p=bcCnfGetValueS("/etc/gnm/conf","option", cammm);
				if(p!=NULL){
									sprintf(snmpoid,"%s",p);
									lt_dbput_rootvars(dbPtr,1,cammm,snmpoid);	
				} 
   }
		
		
		snmpebable=0;
		snmpebable=bcCnfGetValueL("/etc/gnm/conf","option","checklocalip");
		if(snmpebable==1){
    	lt_dbput_rootvars(dbPtr,1,"checklocalip"," checked ");	

		}else{
			lt_dbput_rootvars(dbPtr,1,"checklocalip","");	
		}
		
	}
	
	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysvarconfig.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltsysvarconfigok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	
	       char caMsg[256];
			

	      if(ltMsgGetVar_s(ltMsgPk,"procmailpackage")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","procmailpackage",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","procmailpackage",1);
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"procwebpackage")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","procwebpackage",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","procwebpackage",1);
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"controlSocketFd")==NULL){
			  	confReplaceI("/etc/gnm/conf/system","controlSocketFd",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/system","controlSocketFd",1);
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"udpcontrolSocketFd")==NULL){
			  	confReplaceI("/etc/gnm/conf/system","udpcontrolSocketFd",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/system","udpcontrolSocketFd",1);
			  }


			 if(ltMsgGetVar_s(ltMsgPk,"ipmaccheck")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","ipmaccheck",0);
			  	_ltPubInfo->ipmaccheck=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","ipmaccheck",1);
			  		_ltPubInfo->ipmaccheck=1;
			  }
			  
			   if(ltMsgGetVar_s(ltMsgPk,"arpservice")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","arpservice",0);
			  	_ltPubInfo->arpservice=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","arpservice",1);
			  		_ltPubInfo->arpservice=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"alertarpnum")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","alertarpnum",0);
			  	_ltPubInfo->alertarpnum=1000;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","alertarpnum",atol(ltMsgGetVar_s(ltMsgPk,"alertarpnum")));
			  		_ltPubInfo->alertarpnum=atol(ltMsgGetVar_s(ltMsgPk,"alertarpnum"));
			  }
			
			  if(ltMsgGetVar_s(ltMsgPk,"macbindip")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","macbindip",0);
			  	_ltPubInfo->lMacBindIp=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","macbindip",1);
			  		_ltPubInfo->lMacBindIp=1;
			  }
			  if(ltMsgGetVar_s(ltMsgPk,"logonfixip")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","logonfixip",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","logonfixip",1);
			  }
  			
  			if(ltMsgGetVar_s(ltMsgPk,"checkcontype")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","checkcontype",0);
			  	_ltPubInfo->checkcontype=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","checkcontype",1);
			  		_ltPubInfo->checkcontype=1;
			  }
			  
//			  if(ltMsgGetVar_s(ltMsgPk,"flowlimit")==NULL){
//			  	confReplaceI("/etc/gnm/conf/option","flowlimit",0);
//			  	_ltPubInfo->flowlimit=0;
//			  }else{
//			  		confReplaceI("/etc/gnm/conf/option","flowlimit",1);
//			  		_ltPubInfo->flowlimit=1;
//			  }
			  
			   if(ltMsgGetVar_s(ltMsgPk,"daytimetype")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","daytimetype",0);
			  	_ltPubInfo->daytimetype=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","daytimetype",atol(ltMsgGetVar_s(ltMsgPk,"daytimetype")));
			  		_ltPubInfo->daytimetype=atol(ltMsgGetVar_s(ltMsgPk,"daytimetype"));
			  }
  			

  			
//  			if(ltMsgGetVar_s(ltMsgPk,"timelimit")==NULL){
//			  	confReplaceI("/etc/gnm/conf/option","timelimit",0);
//			  	_ltPubInfo->timelimit=0;
//			  }else{
//			  		confReplaceI("/etc/gnm/conf/option","timelimit",1);
//			  		_ltPubInfo->timelimit=1;
//			  }
			  if(ltMsgGetVar_s(ltMsgPk,"checkcon")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","checkcon",0);
			  	_ltPubInfo->checkcon=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","checkcon",1);
			  		_ltPubInfo->checkcon=1;
			  }
			  
			  
  			if(ltMsgGetVar_s(ltMsgPk,"enablerule")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","enablerule",0);
			  	_ltPubInfo->lSrvRuleChk=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","enablerule",1);
			  		_ltPubInfo->lSrvRuleChk=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"enableaccount")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","enableaccount",0);
			  	_ltPubInfo->accountchk=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","enableaccount",1);
			  		_ltPubInfo->accountchk=1;
			  }
			  
			  
			  if(ltMsgGetVar_s(ltMsgPk,"enablewebrule")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","enablewebrule",0);
			  	_ltPubInfo->lWebRuleChk=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","enablewebrule",1);
			  		_ltPubInfo->lWebRuleChk=1;
			  }
			  
			  
			  if(ltMsgGetVar_s(ltMsgPk,"enablewebpostrule")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","enablewebpostrule",0);
			  	_ltPubInfo->lWebPostRuleChk=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","enablewebpostrule",1);
			  		_ltPubInfo->lWebPostRuleChk=1;
			  }
			  if(ltMsgGetVar_s(ltMsgPk,"controltype")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","controltype",1);
			  	_ltPubInfo->controltype=1;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","controltype",2);
			  		_ltPubInfo->controltype=2;
			  }
			  
			   if(ltMsgGetVar_s(ltMsgPk,"qqnoweb")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","qqnoweb",0);
			  	_ltPubInfo->qqnoweb=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","qqnoweb",atol(ltMsgGetVar_s(ltMsgPk,"qqnoweb")));
			  		_ltPubInfo->qqnoweb=atol(ltMsgGetVar_s(ltMsgPk,"qqnoweb"));
			  }
			  if(ltMsgGetVar_s(ltMsgPk,"defaultsort")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","defaultsort",0);
			  	_ltPubInfo->defaultsort=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","defaultsort",1);
			  		_ltPubInfo->defaultsort=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"msnnofujian")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","msnnofujian",0);
			  	_ltPubInfo->msnnofujian=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","msnnofujian",1);
			  		_ltPubInfo->msnnofujian=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"userdispip")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","userdispip",0);
			  	_ltPubInfo->userdispip=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","userdispip",1);
			  		_ltPubInfo->userdispip=1;
			  }
			  
			  
			  
			  if(ltMsgGetVar_s(ltMsgPk,"attpreview")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","attpreview",0);
			  	_ltPubInfo->attpreview=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","attpreview",1);
			  		_ltPubInfo->attpreview=1;
			  }
			  confReplace("/etc/gnm/conf/option","authtype",ltMsgGetVar_s(ltMsgPk,"authtype"));
			  _ltPubInfo->authtype=atol(ltMsgGetVar_s(ltMsgPk,"authtype"));
			  
			  confReplace("/etc/gnm/conf/option","pppoeaction",ltMsgGetVar_s(ltMsgPk,"pppoeaction"));
			  _ltPubInfo->ltPPOEAction=atol(ltMsgGetVar_s(ltMsgPk,"pppoeaction"));
			  
			  if(ltMsgGetVar_s(ltMsgPk,"defaultadduser")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","defaultadduser",0);
			  	_ltPubInfo->defaultadduser=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","defaultadduser",1);
			  		_ltPubInfo->defaultadduser=1;
			  }
			  if(ltMsgGetVar_s(ltMsgPk,"keeponlineuser")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","keeponlineuser",0);
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","keeponlineuser",1);
			  }
			  
			  confReplace("/etc/gnm/conf/option","usertimeout",ltMsgGetVar_s(ltMsgPk,"usertimeout"));
			  _ltPubInfo->userTimeOut=atol(ltMsgGetVar_s(ltMsgPk,"usertimeout"));
			  
			  confReplace("/etc/gnm/conf/option","usermaxtime",ltMsgGetVar_s(ltMsgPk,"usermaxtime"));
			  _ltPubInfo->usermaxtime=atol(ltMsgGetVar_s(ltMsgPk,"usermaxtime"));
			  
			
			  if(ltMsgGetVar_s(ltMsgPk,"autobind")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","autobind",0);
			  	_ltPubInfo->autobind=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","autobind",1);
			  		_ltPubInfo->autobind=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"onlycheckauthurl")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","onlycheckauthurl",0);
			  	_ltPubInfo->onlycheckauthurl=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","onlycheckauthurl",1);
			  		_ltPubInfo->onlycheckauthurl=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"defaultbind")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","defaultbind",1);
			  	_ltPubInfo->defaultbind=1;
			  }else{
			      int xxxi;
			      xxxi=atol(ltMsgGetVar_s(ltMsgPk,"defaultbind"));
			  		confReplaceI("/etc/gnm/conf/option","defaultbind",xxxi);
			  		_ltPubInfo->defaultbind=xxxi;			  		
			  }
			  
			  
			  {
			  	char caTmpCmd[1024];
			  	sprintf(caTmpCmd,"echo '127.0.0.1 localhost' >/etc/hosts");
			  	system(caTmpCmd);
			  	sprintf(caTmpCmd,"echo '61.152.241.173 www.baochuang.com.cn' >>/etc/hosts");
			  	system(caTmpCmd);
//			  	sprintf(caTmpCmd,"echo '%s %s' >>/etc/hosts",ltMsgGetVar_s(ltMsgPk,"pdcip"),ltMsgGetVar_s(ltMsgPk,"pdc"));
//			  	system(caTmpCmd);
//			  	sprintf(caTmpCmd,"echo '%s %s' >>/etc/hosts",ltMsgGetVar_s(ltMsgPk,"bdcip"),ltMsgGetVar_s(ltMsgPk,"bdc"));
//			  	system(caTmpCmd);


			}
			  
			  

  			if(ltMsgGetVar_s(ltMsgPk,"localmon")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","localmon",0);
			  	_ltPubInfo->lmon=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","localmon",1);
			  		_ltPubInfo->lmon=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"nopicblock")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","nopicblock",0);
			  	_ltPubInfo->nopicblock=0;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","nopicblock",1);
			  		_ltPubInfo->nopicblock=1;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"mailbind")==NULL){
			  	confReplaceI("/etc/gnm/conf/option","mailbind",1);
			  	_ltPubInfo->mailbind=1;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","mailbind",2);
			  		_ltPubInfo->mailbind=2;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"flowrate")!=NULL){
			  	//confReplaceI("/etc/gnm/conf/option","flowrate",atol(ltMsgGetVar_s(ltMsgPk,"flowrate")));
			  	confReplace("/etc/gnm/conf/option","flowrate",ltMsgGetVar_s(ltMsgPk,"flowrate"));
			  	_ltPubInfo->flowrate=atof(ltMsgGetVar_s(ltMsgPk,"flowrate"))/1000000;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","flowrate",0);
			  		_ltPubInfo->flowrate=0;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"timerate")!=NULL){
			  	confReplace("/etc/gnm/conf/option","flowrate",ltMsgGetVar_s(ltMsgPk,"timerate"));
			  	_ltPubInfo->timerate=atof(ltMsgGetVar_s(ltMsgPk,"timerate"))/3600;
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","timerate",0);
			  		_ltPubInfo->timerate=0;
			  }
			  
			  if(ltMsgGetVar_s(ltMsgPk,"nomutidown")!=NULL){
			  	confReplaceI("/etc/gnm/conf/option","nomutidown",atol(ltMsgGetVar_s(ltMsgPk,"nomutidown")));
			  	_ltPubInfo->nomutidown=atol(ltMsgGetVar_s(ltMsgPk,"nomutidown"));
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","nomutidown",0);
			  		_ltPubInfo->nomutidown=0;
			  }
			  
			  
			  confReplace("/etc/gnm/conf/option","localdomain1",ltMsgGetVar_s(ltMsgPk,"localdomain1"));
			  sprintf(_ltPubInfo->ldomain1,"%s",ltMsgGetVar_s(ltMsgPk,"localdomain1"));
			  confReplace("/etc/gnm/conf/option","localdomain2",ltMsgGetVar_s(ltMsgPk,"localdomain2"));
			  sprintf(_ltPubInfo->ldomain2,"%s",ltMsgGetVar_s(ltMsgPk,"localdomain2"));
			  confReplace("/etc/gnm/conf/option","localdomain3",ltMsgGetVar_s(ltMsgPk,"localdomain3"));
			  sprintf(_ltPubInfo->ldomain3,"%s",ltMsgGetVar_s(ltMsgPk,"localdomain3"));
			  confReplace("/etc/gnm/conf/option","localdomain4",ltMsgGetVar_s(ltMsgPk,"localdomain4"));
			  sprintf(_ltPubInfo->ldomain4,"%s",ltMsgGetVar_s(ltMsgPk,"localdomain4")); 
			   
  
      {
      	
      	char    *pFile="/etc/conf.ini";
      	char    dbcheck;
      	char    *denyurl;
      	char    caCmdCon[256];
      	
      	if( ltMsgGetVar_s(ltMsgPk,"dbcheck")){
					dbcheck='y';
				}else{
					dbcheck='n';
				}	
				denyurl=ltMsgGetVar_s(ltMsgPk,"denyurl");
				
				/*dbcheck*/
				sprintf(caCmdCon,"/bin/echo 'STARTSSL=n' > %s",pFile);
				system(caCmdCon);	
				if(dbcheck=='y'){
					sprintf(caCmdCon,"/bin/echo 'DBCHECK=y' >> %s",pFile);
					system(caCmdCon);	
				}else{
					sprintf(caCmdCon,"/bin/echo 'DBCHECK=n' >> %s",pFile);
					system(caCmdCon);
				}
				sprintf(caCmdCon,"/bin/echo 'denyurl=%s' >> %s",denyurl,pFile);
				system(caCmdCon);
				
				sprintf(_ltPubInfo->denyUrl,"%s",denyurl);
				if( strlen(_ltPubInfo->denyUrl)<5){
					if(_ltPubInfo->startssl ==1 ){
						sprintf(_ltPubInfo->denyUrl,"https://%s/main/denyaccessweb.htm",_ltPubInfo->authip);
					}else{
					  sprintf(_ltPubInfo->denyUrl,"http://%s/main/denyaccessweb.htm",_ltPubInfo->authip);
				  }
				}
      	
      	denyurl=ltMsgGetVar_s(ltMsgPk,"homeurl");
      	sprintf(caCmdCon,"/bin/echo 'authokurl=%s' > /etc/authokurl.ini ",denyurl);
				system(caCmdCon);
				snprintf(_ltPubInfo->strAuthOKUrl,255,"%s",denyurl);
      	
      }
      
      

			if(ltMsgGetVar_s(ltMsgPk,"checklocalip")!=NULL){
			  	confReplaceI("/etc/gnm/conf/option","checklocalip",atol(ltMsgGetVar_s(ltMsgPk,"checklocalip")));
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","checklocalip",0);
			  }
			

       if(ltMsgGetVar_s(ltMsgPk,"snmpebable")!=NULL){
			  	confReplaceI("/etc/gnm/conf/option","snmpebable",atol(ltMsgGetVar_s(ltMsgPk,"snmpebable")));
			  }else{
			  		confReplaceI("/etc/gnm/conf/option","snmpebable",0);
			  }
			  confReplace("/etc/gnm/conf/option","snmpcommunity",ltMsgGetVar_s(ltMsgPk,"snmpcommunity"));
        confReplace("/etc/gnm/conf/option","snmpsip",ltMsgGetVar_s(ltMsgPk,"snmpsip"));
        confReplace("/etc/gnm/conf/option","snmpv",ltMsgGetVar_s(ltMsgPk,"snmpv"));
        confReplace("/etc/gnm/conf/option","snmpoid",ltMsgGetVar_s(ltMsgPk,"snmpoid"));
        
      int myiii=1;
		  for(myiii=1;myiii<NAS_MAXSNMPNUM;myiii++){
			  char cammm[32];
			  sprintf(cammm,"snmpcommunity%d",myiii+1);
			  confReplace("/etc/gnm/conf/option",cammm,ltMsgGetVar_s(ltMsgPk,cammm));
			  sprintf(cammm,"snmpsip%d",myiii+1);
        confReplace("/etc/gnm/conf/option",cammm,ltMsgGetVar_s(ltMsgPk,cammm));
        sprintf(cammm,"snmpv%d",myiii+1);
        confReplace("/etc/gnm/conf/option",cammm,ltMsgGetVar_s(ltMsgPk,cammm));
        sprintf(cammm,"snmpoid%d",myiii+1);
        confReplace("/etc/gnm/conf/option",cammm,ltMsgGetVar_s(ltMsgPk,cammm));
     }
        
//	
//	    ltMsgPk->msgpktype=1;
//			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
//	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		
//
//	   ltMsgFree(ltMsgPk);
   
    ltsysvarconfig(confd,ltMsgPk,lt_MMHead);
    
    system("killall -9 gnmpkproc");
	   
	   snprintf(caMsg,255,"%s","update application setting success.");
	   logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);

	   return 0;

}



int ltappconfig(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	
	
	char httpcountofpage[128];
	char mailcountofpage[128];
	char imcountofpage[128];
	char ipcountofpage[128];
	
	
	


	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	

	if(_ltPubInfo->iplog==0){
			lt_dbput_rootvars(dbPtr,1,"iplogchk","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"iplogchk"," checked ");	
	}


 	if(_ltPubInfo->httplog==0){
			lt_dbput_rootvars(dbPtr,1,"httplogchk","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"httplogchk"," checked ");	
	}

  if(_ltPubInfo->ipblocklog==0){
			lt_dbput_rootvars(dbPtr,1,"ipblocklogchk","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ipblocklogchk"," checked ");	
	}

  
  if(_ltPubInfo->httpblocklog==0){
			lt_dbput_rootvars(dbPtr,1,"httpblocklogchk","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"httpblocklogchk"," checked ");	
	}
   
  if(_ltPubInfo->connectlog==0){
			lt_dbput_rootvars(dbPtr,1,"connectlogchk","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"connectlogchk"," checked ");	
	}

	if(_ltPubInfo->mailmonchk==0){
			lt_dbput_rootvars(dbPtr,1,"mailmon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"mailmon"," checked ");	
	}
	
	if(_ltPubInfo->pop3mon==0){
			lt_dbput_rootvars(dbPtr,1,"pop3mon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"pop3mon"," checked ");	
	}

	if(_ltPubInfo->formmonchk==0){
			lt_dbput_rootvars(dbPtr,1,"formmon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"formmon"," checked ");	
	}

	if(_ltPubInfo->aimmon==0){
			lt_dbput_rootvars(dbPtr,1,"aimmon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"aimmon"," checked ");	
	}

	if(_ltPubInfo->ftpmon==0){
			lt_dbput_rootvars(dbPtr,1,"ftpmon","");	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ftpmon"," checked ");	
	}

	
	
	sprintf(ipcountofpage,"%d",_ltPubInfo->ipcountofpage);
	lt_dbput_rootvars(dbPtr,1,"ipcountofpage",ipcountofpage);	
	sprintf(httpcountofpage,"%d",_ltPubInfo->httpcountofpage);
	lt_dbput_rootvars(dbPtr,1,"httpcountofpage",httpcountofpage);	
	sprintf(mailcountofpage,"%d",_ltPubInfo->mailcountofpage);
	lt_dbput_rootvars(dbPtr,1,"mailcountofpage",mailcountofpage);	
	sprintf(imcountofpage,"%d",_ltPubInfo->imcountofpage);
	lt_dbput_rootvars(dbPtr,1,"imcountofpage",imcountofpage);	
	
	sprintf(imcountofpage,"%d",_ltPubInfo->userofpage);
	lt_dbput_rootvars(dbPtr,1,"userofpage",imcountofpage);	
	

	
	sprintf(imcountofpage,"%lu",_ltPubInfo->ipkeepday);
	lt_dbput_rootvars(dbPtr,1,"ipkeepday",imcountofpage);	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->httpkeepday);
	lt_dbput_rootvars(dbPtr,1,"httpkeepday",imcountofpage);	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->ipblockkeepday);
	lt_dbput_rootvars(dbPtr,1,"ipblockkeepday",imcountofpage);	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->httpblockkeepday);
	lt_dbput_rootvars(dbPtr,1,"httpblockkeepday",imcountofpage);	
	
	//printf("mailkeepday:%lu\n",_ltPubInfo->mailkeepday);
	sprintf(imcountofpage,"%lu",_ltPubInfo->mailkeepday);
	lt_dbput_rootvars(dbPtr,1,"mailkeepday",imcountofpage);	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->historykeepday);
	lt_dbput_rootvars(dbPtr,1,"historykeepday",imcountofpage);	

	
  sprintf(imcountofpage,"%lu",_ltPubInfo->formkeepday);
	lt_dbput_rootvars(dbPtr,1,"formkeepday",imcountofpage);	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->aimkeepday);
	lt_dbput_rootvars(dbPtr,1,"aimkeepday",imcountofpage);	
	
	
	sprintf(imcountofpage,"%lu",_ltPubInfo->backkeepday);
	lt_dbput_rootvars(dbPtr,1,"backkeepday",imcountofpage);	
	
	
	if(_ltPubInfo->loglevel==1){
			lt_dbput_rootvars(dbPtr,1,"loglevel","1");	
	}else if(_ltPubInfo->loglevel==2){
			lt_dbput_rootvars(dbPtr,1,"loglevel","2");	
	}else if(_ltPubInfo->loglevel==3){
			lt_dbput_rootvars(dbPtr,1,"loglevel","3");	
	}else if(_ltPubInfo->loglevel==4){
			lt_dbput_rootvars(dbPtr,1,"loglevel","4");	
	}else if(_ltPubInfo->loglevel==5){
			lt_dbput_rootvars(dbPtr,1,"loglevel","5");	
	}
	
	if(_ltPubInfo->dbloglevel==1){
			lt_dbput_rootvars(dbPtr,1,"dbloglevel","1");	
	}else if(_ltPubInfo->dbloglevel==2){
			lt_dbput_rootvars(dbPtr,1,"dbloglevel","2");	
	}else if(_ltPubInfo->dbloglevel==3){
			lt_dbput_rootvars(dbPtr,1,"dbloglevel","3");	
	}else if(_ltPubInfo->dbloglevel==4){
			lt_dbput_rootvars(dbPtr,1,"dbloglevel","4");	
	}else if(_ltPubInfo->dbloglevel==5){
			lt_dbput_rootvars(dbPtr,1,"dbloglevel","5");	
	}
	
	if(_ltPubInfo->dblog==1){
			lt_dbput_rootvars(dbPtr,1,"dblogchk"," checked ");	
	}else{
			lt_dbput_rootvars(dbPtr,1,"dblogchk"," ");	
	}
	
	sprintf(imcountofpage,"%d",_ltPubInfo->maxdiskp);
	lt_dbput_rootvars(dbPtr,1,"maxdiskp",imcountofpage);	
	

	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/appconfig.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}




int ltappconfigok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	
	
	char httpcountofpage[128];
	char mailcountofpage[128];
	char imcountofpage[128];
	char ipcountofpage[128];
	char caMsg[256];
	
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	

  if(ltMsgGetVar_s(ltMsgPk,"iplog")==NULL){
  	_ltPubInfo->iplog=0;
  	
  }else{
  		_ltPubInfo->iplog=1;
  }
  
   if(ltMsgGetVar_s(ltMsgPk,"httplog")==NULL){
  	_ltPubInfo->httplog=0;
  }else{
  		_ltPubInfo->httplog=1;
  }
  
   if(ltMsgGetVar_s(ltMsgPk,"ipblocklog")==NULL){
  	_ltPubInfo->ipblocklog=0;
  }else{
  		_ltPubInfo->ipblocklog=1;
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"httpblocklog")==NULL){
  	_ltPubInfo->httpblocklog=0;
  }else{
  		_ltPubInfo->httpblocklog=1;
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"connectlog")==NULL){
  	_ltPubInfo->connectlog=0;
  }else{
  		_ltPubInfo->connectlog=1;
  }
  
  
  if(ltMsgGetVar_s(ltMsgPk,"mailmon")==NULL){
  	_ltPubInfo->mailmonchk=0;
  }else{
  		_ltPubInfo->mailmonchk=1;
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"pop3mon")==NULL){
  	_ltPubInfo->pop3mon=0;
  }else{
  		_ltPubInfo->pop3mon=1;
  }
  
   if(ltMsgGetVar_s(ltMsgPk,"formmon")==NULL){
  	_ltPubInfo->formmonchk=0;
  }else{
  		_ltPubInfo->formmonchk=1;
  }
  
   if(ltMsgGetVar_s(ltMsgPk,"aimmon")==NULL){
  	_ltPubInfo->aimmon=0;
  }else{
  		_ltPubInfo->aimmon=1;
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"ftpmon")==NULL){
  	_ltPubInfo->ftpmon=0;
  }else{
  		_ltPubInfo->ftpmon=1;
  }
  
  if(ltMsgGetVar_s(ltMsgPk,"maxdiskp")==NULL){
  	_ltPubInfo->maxdiskp=80;
  }else{
  		_ltPubInfo->maxdiskp=atol(ltMsgGetVar_s(ltMsgPk,"maxdiskp"));
  }
  

	
	sprintf(ipcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"ipcountofpage"));
	_ltPubInfo->ipcountofpage=atol(ipcountofpage);
	
	sprintf(httpcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"httpcountofpage"));
	_ltPubInfo->httpcountofpage=atol(httpcountofpage);
	
	sprintf(httpcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"historykeepday"));
	_ltPubInfo->historykeepday=atol(httpcountofpage);
	
	
	
	sprintf(mailcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"mailcountofpage"));
	_ltPubInfo->mailcountofpage=atol(mailcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"imcountofpage"));
	_ltPubInfo->imcountofpage=atol(imcountofpage);
	
	
	sprintf(ipcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"ipcountofpage"));
	_ltPubInfo->ipcountofpage=atol(ipcountofpage);
	
	sprintf(httpcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"httpcountofpage"));
	_ltPubInfo->httpcountofpage=atol(httpcountofpage);
	
	
	sprintf(mailcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"mailcountofpage"));
	_ltPubInfo->mailcountofpage=atol(mailcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"imcountofpage"));
	_ltPubInfo->imcountofpage=atol(imcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"userofpage"));
	_ltPubInfo->userofpage=atol(imcountofpage);
		
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"ipkeepday"));
	_ltPubInfo->ipkeepday=atol(imcountofpage);
	
		sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"httpkeepday"));
	_ltPubInfo->httpkeepday=atol(imcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"ipblockkeepday"));
	_ltPubInfo->ipblockkeepday=atol(imcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"httpblockkeepday"));
	_ltPubInfo->httpblockkeepday=atol(imcountofpage);

	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"mailkeepday"));
	_ltPubInfo->mailkeepday=atol(imcountofpage);

	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"formkeepday"));
	_ltPubInfo->formkeepday=atol(imcountofpage);
	
	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"aimkeepday"));
	_ltPubInfo->aimkeepday=atol(imcountofpage);

	sprintf(imcountofpage,"%s",ltMsgGetVar_s(ltMsgPk,"backkeepday"));
	_ltPubInfo->backkeepday=atol(imcountofpage);

	_ltPubInfo->dbloglevel=atol(ltMsgGetVar_s(ltMsgPk,"dbloglevel"));
	if(ltMsgGetVar_s(ltMsgPk,"dblog")==NULL){
  	_ltPubInfo->dblog=0;
  }else{
  		_ltPubInfo->dblog=1;
  }
  
  _ltPubInfo->loglevel=atol(ltMsgGetVar_s(ltMsgPk,"loglevel"));

	
	/*更新lt.conf*/
	/*修改lt.conf*/	
		{
	
			
		  confReplaceI("/etc/gnm/conf/option","maxdiskp",_ltPubInfo->maxdiskp);
		  confReplaceI("/etc/gnm/conf/option","iplog",_ltPubInfo->iplog);
		  confReplaceI("/etc/gnm/conf/option","httplog",_ltPubInfo->httplog);
		  confReplaceI("/etc/gnm/conf/option","connectlog",_ltPubInfo->connectlog);
		  confReplaceI("/etc/gnm/conf/option","httpblocklog",_ltPubInfo->httpblocklog);
		  confReplaceI("/etc/gnm/conf/option","ipblocklog",_ltPubInfo->ipblocklog);
		  
		  confReplaceI("/etc/gnm/conf/option","mailmon",_ltPubInfo->mailmonchk);
		  confReplaceI("/etc/gnm/conf/option","pop3mon",_ltPubInfo->pop3mon);
		  confReplaceI("/etc/gnm/conf/option","formmon",_ltPubInfo->formmonchk);
		  confReplaceI("/etc/gnm/conf/option","aimmon",_ltPubInfo->aimmon);
		  confReplaceI("/etc/gnm/conf/option","ftpmon",_ltPubInfo->ftpmon);
		  
		  confReplaceI("/etc/gnm/conf/option","ipcountofpage",_ltPubInfo->ipcountofpage);
		  confReplaceI("/etc/gnm/conf/option","httpcountofpage",_ltPubInfo->httpcountofpage);
		  confReplaceI("/etc/gnm/conf/option","mailcountofpage",_ltPubInfo->mailcountofpage);
		  confReplaceI("/etc/gnm/conf/option","imcountofpage",_ltPubInfo->imcountofpage);
		  confReplaceI("/etc/gnm/conf/option","userofpage",_ltPubInfo->userofpage);
		  
		  confReplaceI("/etc/gnm/conf/option","ipkeepday",_ltPubInfo->ipkeepday);
		  confReplaceI("/etc/gnm/conf/option","ipkeepday",_ltPubInfo->ipkeepday);
		  confReplaceI("/etc/gnm/conf/option","httpkeepday",_ltPubInfo->httpkeepday);
		  confReplaceI("/etc/gnm/conf/option","ipblockkeepday",_ltPubInfo->ipblockkeepday);
		  confReplaceI("/etc/gnm/conf/option","httpblockkeepday",_ltPubInfo->httpblockkeepday);
		  confReplaceI("/etc/gnm/conf/option","mailkeepday",_ltPubInfo->mailkeepday);
		  confReplaceI("/etc/gnm/conf/option","historykeepday",_ltPubInfo->historykeepday);
		  
		  
		  confReplaceI("/etc/gnm/conf/option","formkeepday",_ltPubInfo->formkeepday);
		  confReplaceI("/etc/gnm/conf/option","aimkeepday",_ltPubInfo->aimkeepday);
		  confReplaceI("/etc/gnm/conf/option","backkeepday",_ltPubInfo->backkeepday);

		  confReplaceI("/etc/gnm/conf/option","dbloglevel",_ltPubInfo->dbloglevel);
		  confReplaceI("/etc/gnm/conf/option","dblog",_ltPubInfo->dblog);  
     
      confReplaceI("/etc/gnm/conf/option","loglevel",_ltPubInfo->loglevel);
   }
	
//	    ltMsgPk->msgpktype=1;
//			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
//	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		
//
//	   ltMsgFree(ltMsgPk);
	   
	   snprintf(caMsg,255,"%s","update monitoring setting success.");
	   logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	   
	   
	   ltappconfig(confd,ltMsgPk,lt_MMHead);

	   return 0;

}



int ltsysregist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	

  /*
  1>运行getupdatelist 
  2>显示出来  类似报告  下面是升级包列表
  3> 手动选择升级  开始一个一个升级  用ajax动态显示
  
  升级过程：
  1〉get updatelist 
  2〉wget   /
  3> 解包  /app/ns/update/目录  有加密以及子目录现实状态
  4〉运行目录下的install 
  
  
  */
  /*列表和升级程序分开 */

		  int pim[2];
		  buffer ss;
		  char  xPlt[4096];
		  int   pid;
		  int   fdm;
		  static char inbuf[1024];
		  static stralloc line = {0};
		  int match;
		  int  linenum;
		  char alias[1024];
		  
		  static char *binqqargs[2] = { "uinfolist", 0 } ;
		  
		  memset(xPlt,0,sizeof(xPlt));
		  
		  
		  if (pipe(pim) == -1) {
		  	  ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,xPlt);
			    ltMsgFree(ltMsgPk);
			    return -1;
      }
		  	
	
		 
		  switch(pid = vfork()) {
		    case -1:
		      close(pim[0]); close(pim[1]);
		      return -1;
		    case 0:
		      close(pim[0]);
		      if (fd_move(1,pim[1]) == -1) _exit(120);
		      if (chdir("/app/ns/bin") == -1) _exit(61);
		      execv(*binqqargs,binqqargs);
		      _exit(120);
		  }
		
		  fdm = pim[0]; 
		  close(pim[1]);
      linenum=0;
      sprintf(xPlt,"%s","[");
		  buffer_init(&ss,read,fdm,inbuf,sizeof(inbuf));
		  for (;;)
			{

								if (getln(&ss,&line,&match,'\n') == -1) break;
								if (!match && !line.len) { close(fdm); break; }
								striptrailingwhitespace(&line);
								if (!stralloc_0(&line)) break;
								memset(alias,0,sizeof(alias));
								sprintf(alias,"%s",line.s);
								//printf("alias:%s\n",alias);
								if(strstr(alias,"error") ){
									memset(xPlt,0,sizeof(xPlt));
									sprintf(xPlt,"%s","[['产品版本状态','未知系统','可能无法同验证服务器通信或者非法版本,请联系400-889-2728确认']]");
									ltMsgPk->msgpktype=1;
								  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,xPlt);
							    ltMsgFree(ltMsgPk);
							    return -1;
								}
								if(linenum==0){
									sprintf(xPlt,"%s['产品版本状态','%s','']",xPlt,alias[0]=='2'?"正式注册":"测试注册");
								}else if(linenum==1){
									sprintf(xPlt,"%s,['厂商认可的代理商','%s','厂商仅仅认可从该代理商的销售为合法的']",xPlt,alias);
								}else if(linenum==2){
									sprintf(xPlt,"%s,['厂商认可的最终用户','%s','厂商仅仅认可该用户为合法的']",xPlt,alias);
								}else if(linenum==3){
									sprintf(xPlt,"%s,['产品型号','%s','厂商原始产品型号']",xPlt,alias);
								}else if(linenum==4){
									sprintf(xPlt,"%s,['开始服务周期','%s','如果服务周期不对，尽快联系400-8892718']",xPlt,alias);
								}else if(linenum==5){
									sprintf(xPlt,"%s,['结束服务周期','%s','']",xPlt,alias);
								}else{
								  sprintf(xPlt,"%s,['新升级包列表','%s','']",xPlt,alias);
							  }
								
								
								linenum++;
			}
			sprintf(xPlt,"%s%s",xPlt,"]");
      ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,xPlt);
			ltMsgFree(ltMsgPk);
		  return 0;


}



int ltsysregistok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char *begin;
	int  iReturn;
	char *ppp;
	char caBuf[4012];
	int srcfd;
  char   caFile1[256];
	/*
	begin=1  运行updategnm
	      0  检查状态
	*/
	/*
		proccess 有 1+
		         没有 0+
	*/
	begin=ltMsgGetVar_s(ltMsgPk,"begin");

	if(!begin){
		begin='0';
	}
	
	
	memset(caBuf,0,sizeof(caBuf));	
	if(begin[0]=='1'){
		char gnmkey[24];
  
    FILE *fp;
		memset(gnmkey,0,sizeof(gnmkey));
    fp=fopen("/etc/nsmkey","r");
    if(fp==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"0读文件错误！");
				ltMsgFree(ltMsgPk);
				return 0;	
    }
		fgets(gnmkey,sizeof(gnmkey),fp);
    gnmkey[19]='\0';
    fclose(fp);

	 system("/bin/rm /app/ns/bin/update.proccess");
	 chdir("/app/ns/bin");
	 sprintf(caBuf,"/app/ns/bin/updategnm %s &",gnmkey);
	 system(caBuf);
	 sleep(1);
	}
	memset(caBuf,0,sizeof(caBuf));	
	iReturn=ltFileIsExist("/app/ns/bin/update.proccess");
  if(iReturn==1){
  	caBuf[0]='1';
  }else{
    caBuf[0]='0';
  }
  ppp=caBuf;
  ppp=ppp+1;
  srcfd = open("/app/ns/bin/update.info", O_RDONLY);
	if(srcfd >0 ) {
		    int iii;
		    int jjj;
		    read(srcfd, ppp, 4011);
		    close(srcfd);
		    jjj=strlen(caBuf);
		    for(iii=0;iii<jjj;iii++){
		    	if(caBuf[iii]=='\r' || caBuf[iii]=='\n'){
		    		caBuf[iii]='\0';
		    		break;
		    	}
		    }
		    if( strncmp(ppp,"正在下载文件:",strlen("正在下载文件:"))==0 ){
		    		char *myppp;
		    		struct stat buf;
		    		myppp=ppp+strlen("正在下载文件:");
		    		sprintf(caFile1,"/%s",myppp);
		    		jjj=strlen(caFile1);
				    for(iii=0;iii<jjj;iii++){
				    	if(caFile1[iii]=='\r' || caFile1[iii]=='\n'){
				    		caFile1[iii]='\0';
				    		break;
				    	}
				    }
		    		stat(caFile1,&buf); 
		    		sprintf(caFile1,"已下载%dKB",buf.st_size/1024);
		    		
		    		strcat(ppp,caFile1);

		    }
	}	
  
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,caBuf);
	ltMsgFree(ltMsgPk);

	return 0;

}





int ltsyssmtpconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char    strroute[8012];
	FILE *fp;
	char    *pFile;
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	
	
	pFile="/var/qmail/control/routetype";
	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
				lt_dbput_rootvars(dbPtr,1,"routetype","1");
	}else{
		fgets(strroute,sizeof(strroute),fp);
		fclose(fp);
		lt_dbput_rootvars(dbPtr,1,"routetype",strroute);
	}
	
	pFile="/var/qmail/control/smtproutes";
	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
				lt_dbput_rootvars(dbPtr,1,"smtproutes","");
	}else{
			fread(strroute,sizeof(strroute),1,fp);
			
			fclose(fp);
			lt_dbput_rootvars(dbPtr,1,"smtproutes",strroute);
	}
	
	pFile="/var/qmail/control/authuser";
	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
				lt_dbput_rootvars(dbPtr,1,"authuser","");
	}else{
		fgets(strroute,sizeof(strroute),fp);
		
		fclose(fp);
		lt_dbput_rootvars(dbPtr,1,"authuser",strroute);
	}
	
	pFile="/var/qmail/control/authpass";
	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
				lt_dbput_rootvars(dbPtr,1,"authpass","");
	}else{
		fgets(strroute,sizeof(strroute),fp);
		
		fclose(fp);
		lt_dbput_rootvars(dbPtr,1,"authpass",strroute);
	}

		
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/syssmtpconf.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltsyssmtpconfok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    strroute[8012];
	FILE *fp;
	char    *pFile;
	


	
	pFile="/var/qmail/control/routetype";
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"routetype"));
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	fclose(fp);
	
	
	pFile="/var/qmail/control/smtproutes";
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"smtproutes"));
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	fclose(fp);
	
	
	pFile="/var/qmail/control/authuser";
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"authuser"));
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	fclose(fp);
	
	pFile="/var/qmail/control/authpass";
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"authpass"));
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	fclose(fp);
	
	
	 ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		
		ltMsgFree(ltMsgPk);
		return 0;

}


int ltsysrouteconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char    strroute[8012];
	FILE    *fp;
	char    *pFile="/etc/route.ini";
	


	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	
		
	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fread(strroute,sizeof(strroute),1,fp);
	
	fclose(fp);
	lt_dbput_rootvars(dbPtr,1,"strroute",strroute);

		
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysrouteconf.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int lysystongzhi(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char    strroute[8012];
	FILE    *fp;
	char    *pFile="/etc/tongzhi.conf";

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");

	memset(strroute,0,sizeof(strroute));
	fp=fopen(pFile,"r");
	if(fp==NULL){
			lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/systzconf.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
	}
	fread(strroute,sizeof(strroute),1,fp);
	
	fclose(fp);
	lt_dbput_rootvars(dbPtr,1,"tongzhi",strroute);

		
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/systzconf.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int lysystongzhiok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char    strroute[8012];
	FILE *fp;
	char    *pFile="/etc/tongzhi.conf";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"tongzhi"));
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get tongzhi file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	
	fclose(fp);
	
	lt_dbput_rootvars(dbPtr,1,"tongzhi",strroute);

		
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/systzconf.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltsysrouteconfok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char    strroute[8012];
	FILE *fp;
	char    *pFile="/etc/route.ini";
	int   iii,ilen;
	char  *ppp;

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	memset(strroute,0,sizeof(strroute));
	sprintf(strroute,"%s",ltMsgGetVar_s(ltMsgPk,"strroute"));
	ilen=strlen(strroute);
	ppp=strroute;
	for(iii=0;iii<ilen;iii++){
		if(strroute[iii]=='\r'){
			continue;
		}
		*ppp=strroute[iii];
		ppp++;
	}
	*ppp='\0';
	fp=fopen(pFile,"w");
	if(fp==NULL){
			printf("can't get route file\n");
			return 0; 	
	}
	fputs(strroute,fp);
	
	fclose(fp);
	
	lt_dbput_rootvars(dbPtr,1,"strroute",strroute);

		
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysrouteconf.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltsysnetconfok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
		char    *strcontype;
		int     contype;

		char *myinip;
		char *myinmask;
		char *mygw;
		char *mynameserver;
		char *mybr1;
		char *mybr2;
		char *mybr3;
		char *mybr4;
		char *myeth0;
		char *myeth1;
		char *myeth2;
		char *myeth3;
		char *myeth4;
		char *myeth5;
		char *myeth6;
		char *myeth7;
		char *myeth8;
		char *myeth9;
		
		char    *pFile="/etc/gnm/ethernet/settings";
		
		strcontype=ltMsgGetVar_s(ltMsgPk,"contype");
		if(strcontype==NULL){
			strcontype="4";	
		}
		contype=atol(strcontype);
		if(contype<1 || contype>7){
				contype=4;	
		}
		
		myinip=ltMsgGetVar_s(ltMsgPk,"inip");
		myinmask=ltMsgGetVar_s(ltMsgPk,"inmask");
		mygw=ltMsgGetVar_s(ltMsgPk,"gw");
		mynameserver=ltMsgGetVar_s(ltMsgPk,"nameserver");	
		
		
		mybr1=ltMsgGetVar_s(ltMsgPk,"br1");	
		if(mybr1==NULL)
		{
			mybr1="";
		}
    mybr2=ltMsgGetVar_s(ltMsgPk,"br2");
		if(mybr2==NULL)
		{
			mybr2="";
		}
		mybr3=ltMsgGetVar_s(ltMsgPk,"br3");
		if(mybr3==NULL)
		{
			mybr3="";
		}
		mybr4=ltMsgGetVar_s(ltMsgPk,"br4");
		if(mybr4==NULL)
		{
			mybr4="";
		}
		
		myeth0=ltMsgGetVar_s(ltMsgPk,"eth0");
		if(myeth0==NULL)
		{
			myeth0="";
		}	
		myeth1=ltMsgGetVar_s(ltMsgPk,"eth1");	
		if(myeth1==NULL)
		{
			myeth1="";
		}	
		myeth2=ltMsgGetVar_s(ltMsgPk,"eth2");	
		if(myeth2==NULL)
		{
			myeth2="";
		}	
		myeth3=ltMsgGetVar_s(ltMsgPk,"eth3");	
		if(myeth3==NULL)
		{
			myeth3="";
		}	
		myeth4=ltMsgGetVar_s(ltMsgPk,"eth4");	
		if(myeth4==NULL)
		{
			myeth4="";
		}	
		myeth5=ltMsgGetVar_s(ltMsgPk,"eth5");	
		if(myeth5==NULL)
		{
			myeth5="";
		}	
		myeth6=ltMsgGetVar_s(ltMsgPk,"eth6");	
		if(myeth6==NULL)
		{
			myeth6="";
		}	
		myeth7=ltMsgGetVar_s(ltMsgPk,"eth7");	
		if(myeth7==NULL)
		{
			myeth7="";
		}	
		myeth8=ltMsgGetVar_s(ltMsgPk,"eth8");	
		if(myeth8==NULL)
		{
			myeth8="";
		}	
		myeth9=ltMsgGetVar_s(ltMsgPk,"eth9");	
		if(myeth9==NULL)
		{
			myeth9="";
		}	
		{
			char    caCmdCon[128];
			sprintf(caCmdCon,"echo 'CONNECT_TYPE=%d' > %s",contype,pFile);
		  system(caCmdCon);
		
		  sprintf(caCmdCon,"echo 'GREEN_ADDRESS=%s' >> %s",myinip,pFile);
			system(caCmdCon);
			sprintf(caCmdCon,"echo 'GREEN_NETMASK=%s' >> %s",myinmask,pFile);
			system(caCmdCon);
			
			sprintf(caCmdCon,"echo 'RED_ADDRESS=' >> %s",pFile);
			system(caCmdCon);
			sprintf(caCmdCon,"echo 'RED_NETMASK=' >> %s",pFile);
			system(caCmdCon);
			
			sprintf(caCmdCon,"echo 'ORANGE_ADDRESS=' >> %s",pFile);
			system(caCmdCon);
			sprintf(caCmdCon,"echo 'ORANGE_NETMASK=' >> %s",pFile);
			system(caCmdCon);
			
			sprintf(caCmdCon,"echo 'USERNAME=' >> %s",pFile);
			system(caCmdCon);
			sprintf(caCmdCon,"echo 'PASSWORD=' >> %s",pFile);
			system(caCmdCon);
			
			sprintf(caCmdCon,"echo 'DEFAULT_GATEWAY=%s' >> %s",mygw,pFile);
			system(caCmdCon);
			sprintf(caCmdCon,"echo 'NAME_SERVER=%s' >> %s",mynameserver,pFile);
			system(caCmdCon);
			
			if(contype==7){
					sprintf(caCmdCon,"echo 'br1=%s' >> %s",mybr1,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'br2=%s' >> %s",mybr2,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'br3=%s' >> %s",mybr3,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'br4=%s' >> %s",mybr4,pFile);
					system(caCmdCon);
					
					
					sprintf(caCmdCon,"echo 'eth0=%s' >> %s",myeth0,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth1=%s' >> %s",myeth1,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth2=%s' >> %s",myeth2,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth3=%s' >> %s",myeth3,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth4=%s' >> %s",myeth4,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth5=%s' >> %s",myeth5,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth6=%s' >> %s",myeth6,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth7=%s' >> %s",myeth7,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth8=%s' >> %s",myeth8,pFile);
					system(caCmdCon);
					sprintf(caCmdCon,"echo 'eth9=%s' >> %s",myeth9,pFile);
					system(caCmdCon);
		  }
		}
	
	  ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head><title>系统网络设置成功</title><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. <br>系统服务配置成功，请重新启动系统。</body></html>");		
		ltMsgFree(ltMsgPk);
		return 0;
		
}
int ltsysnetconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char    strcontype[24];
	char    inip[24];
	char    inmask[24];
	char    outip[24];
	char    outmask[24];
	char    oip[24];
	char    omask[24];
	char    uname[32];
	char    upass[32];
	char    gw[24];
	char    nameserver[24];
	char 		xkey[24];
	FILE *fp;
	int   lcontype;
	char    *pFile="/etc/gnm/ethernet/settings";
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	if(ltMsgGetVar_s(ltMsgPk,"contype")==NULL){
		memset(strcontype,0,sizeof(strcontype));
		bcCnfGetValue_s(pFile,"CONNECT_TYPE",strcontype);
		lcontype=atol(strcontype);
	}else{
		memset(strcontype,0,sizeof(strcontype));
		sprintf(strcontype,"%s",ltMsgGetVar_s(ltMsgPk,"contype"));
		lcontype=atol(strcontype);
  }
	memset(xkey,0,sizeof(xkey));
	fp=fopen("/etc/nsmkey","r");
	if(fp==NULL){
			printf("can't get serial code\n");
			return 0;
	}
	fgets(xkey,sizeof(xkey),fp);
	xkey[19]='\0';
	fclose(fp);
	lt_dbput_rootvars(dbPtr,1,"gnmkey",xkey);

  if(lcontype==1){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);

			memset(uname,0,sizeof(uname));
			bcCnfGetValue_s(pFile,"USERNAME",uname);
			lt_dbput_rootvars(dbPtr,1,"uname",uname);


			memset(upass,0,sizeof(upass));
			bcCnfGetValue_s(pFile,"PASSWORD",upass);
			lt_dbput_rootvars(dbPtr,1,"upass",upass);

	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf1.htm",dbPtr->head,0);

  }else if(lcontype==2){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);

			memset(outip,0,sizeof(outip));
			bcCnfGetValue_s(pFile,"RED_ADDRESS",outip);
			lt_dbput_rootvars(dbPtr,1,"outip",outip);


			memset(outmask,0,sizeof(outmask));
			bcCnfGetValue_s(pFile,"RED_NETMASK",outmask);
			lt_dbput_rootvars(dbPtr,1,"outmask",outmask);


	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf2.htm",dbPtr->head,0);

  }else if(lcontype==3){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);

			memset(outip,0,sizeof(outip));
			bcCnfGetValue_s(pFile,"RED_ADDRESS",outip);
			lt_dbput_rootvars(dbPtr,1,"outip",outip);

			memset(outmask,0,sizeof(outmask));
			bcCnfGetValue_s(pFile,"RED_NETMASK",outmask);
			lt_dbput_rootvars(dbPtr,1,"outmask",outmask);

			memset(oip,0,sizeof(oip));
			bcCnfGetValue_s(pFile,"ORANGE_ADDRESS",oip);
			lt_dbput_rootvars(dbPtr,1,"oip",oip);

			memset(omask,0,sizeof(omask));
			bcCnfGetValue_s(pFile,"ORANGE_NETMASK",omask);
			lt_dbput_rootvars(dbPtr,1,"omask",omask);

	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf3.htm",dbPtr->head,0);

  }else if(lcontype==4){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);



	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf4.htm",dbPtr->head,0);

  }else if(lcontype==7){
  		char br1[16],br2[16],br3[16],br4[16];
  		char eth0[16],eth1[16],eth2[16],eth3[16],eth4[16],eth5[16],eth6[16],eth7[16],eth8[16],eth9[16];
  		char str[16];
  		
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);
			
		
			memset(br1,0,sizeof(br1));
			memset(str,0,sizeof(str));
			bcCnfGetValue_s(pFile,"br1",br1);
			if(br1!=NULL){
				sprintf(str,"br1chk%s",br1);
				lt_dbput_rootvars(dbPtr,1,str,"checked");
			}
			
			memset(br2,0,sizeof(br2));
			memset(str,0,sizeof(str));
			bcCnfGetValue_s(pFile,"br2",br2);
			if(br2!=NULL){
				sprintf(str,"br2chk%s",br2);
				lt_dbput_rootvars(dbPtr,1,str,"checked");
			}			
		
			memset(br3,0,sizeof(br3));
			memset(str,0,sizeof(str));
			bcCnfGetValue_s(pFile,"br3",br3);
			if(br3!=NULL){
				sprintf(str,"br3chk%s",br3);
				lt_dbput_rootvars(dbPtr,1,str,"checked");
			}			
			
			memset(br4,0,sizeof(br4));
			memset(str,0,sizeof(str));
			bcCnfGetValue_s(pFile,"br4",br4);
			if(br4!=NULL){
				sprintf(str,"br4chk%s",br4);
				lt_dbput_rootvars(dbPtr,1,str,"checked");
			}	
			
			//
			memset(str,0,sizeof(str));
			memset(eth0,0,sizeof(eth0));
			bcCnfGetValue_s(pFile,"eth0",eth0);
     	sprintf(str,"eth0%ssel",eth0);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
    	
     	memset(str,0,sizeof(str));
     	memset(eth1,0,sizeof(eth1));
			bcCnfGetValue_s(pFile,"eth1",eth1);
     	sprintf(str,"eth1%ssel",eth1);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
     	
     	memset(str,0,sizeof(str));
     	memset(eth2,0,sizeof(eth2));
			bcCnfGetValue_s(pFile,"eth2",eth2);
     	sprintf(str,"eth2%ssel",eth2);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
	      	
     	memset(str,0,sizeof(str));
     	memset(eth3,0,sizeof(eth3));
			bcCnfGetValue_s(pFile,"eth3",eth3);
     	sprintf(str,"eth3%ssel",eth3);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
     	
     	memset(str,0,sizeof(str));
     	memset(eth4,0,sizeof(eth4));
			bcCnfGetValue_s(pFile,"eth4",eth4);
     	sprintf(str,"eth4%ssel",eth4);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
	      	
     	memset(str,0,sizeof(str));
     	memset(eth5,0,sizeof(eth5));
			bcCnfGetValue_s(pFile,"eth5",eth5);
     	sprintf(str,"eth5%ssel",eth5);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
     	
     	memset(str,0,sizeof(str));
     	memset(eth6,0,sizeof(eth6));
			bcCnfGetValue_s(pFile,"eth6",eth6);
     	sprintf(str,"eth6%ssel",eth6);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
 		    	
     	memset(str,0,sizeof(str));
     	memset(eth7,0,sizeof(eth7));
			bcCnfGetValue_s(pFile,"eth7",eth7);
     	sprintf(str,"eth7%ssel",eth7);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
     	
     	memset(str,0,sizeof(str));
     	memset(eth8,0,sizeof(eth8));
			bcCnfGetValue_s(pFile,"eth8",eth8);
     	sprintf(str,"eth8%ssel",eth8);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
 		        	
     	memset(str,0,sizeof(str));
     	memset(eth9,0,sizeof(eth9));
			bcCnfGetValue_s(pFile,"eth9",eth9);
     	sprintf(str,"eth9%ssel",eth9);
     	lt_dbput_rootvars(dbPtr,1,str,"selected");
    	
	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf7.htm",dbPtr->head,0);

  }else if(lcontype==5){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"RED_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"RED_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);

	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf5.htm",dbPtr->head,0);

  }else if(lcontype==6){
			memset(inip,0,sizeof(inip));
			bcCnfGetValue_s(pFile,"GREEN_ADDRESS",inip);
			lt_dbput_rootvars(dbPtr,1,"inip",inip);
			memset(inmask,0,sizeof(inmask));
			bcCnfGetValue_s(pFile,"GREEN_NETMASK",inmask);
			lt_dbput_rootvars(dbPtr,1,"inmask",inmask);

			memset(gw,0,sizeof(gw));
			bcCnfGetValue_s(pFile,"DEFAULT_GATEWAY",gw);
			lt_dbput_rootvars(dbPtr,1,"gw",gw);

			memset(nameserver,0,sizeof(nameserver));
			bcCnfGetValue_s(pFile,"NAME_SERVER",nameserver);
			lt_dbput_rootvars(dbPtr,1,"nameserver",nameserver);

	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/sysnetconf6.htm",dbPtr->head,0);

  }

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



/*
netarea 控制
*/

int ltadminarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char strDesc[20];
	long iIndex;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"ipbindlist");

  sprintf(sqlBuf,"select id,ipstart,ipend from nasAdminArea order by ipstart desc;");
	iIndex=0;
	tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){

			lt_dbput_recordvars(tablePtr,4,
				"inum",LT_TYPE_LONG,iIndex,
				"listindex",LT_TYPE_STRING,tempRow[0],
				"bip",LT_TYPE_STRING,tempRow[1],
				"eip",LT_TYPE_STRING,tempRow[2]);
			tempRow= ltDbFetchRow(tempCursor);
			iIndex++;
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(strDesc,"%ld",iIndex);
	lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/adminarea.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int ltadminareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int  actiontype;
	char caMsg[256];
	
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
		long lCount;
		sprintf(sqlBuf,"select count(*) from nasAdminArea ");
		lCount =0;
		tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
			ltDbCloseCursor(tempCursor);
		}
		
		if(lCount == 0) {
			lCount=1;
		}else{
			
			sprintf(sqlBuf,"select max(id)+1 from nasAdminArea ");
			lCount = 1;
			tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
				ltDbCloseCursor(tempCursor);
			}
		}	
		sprintf(sqlBuf,"insert into nasAdminArea values (%ld,'%s','%s')",lCount,
			ltMsgGetVar_s(ltMsgPk,"bip"),
			ltMsgGetVar_s(ltMsgPk,"eip"));
//printf("sql:%s\n",sqlBuf);
		ltDbExecSql(G_DbCon,sqlBuf); 
		
		snprintf(caMsg,255,"%s","update adminarea success.");
	  logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	
	}else if (actiontype==2){/*del*/
		long allnum,i;
		char tmpVal[20];
		allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
		for(i=0;i<allnum;i++){
			sprintf(tmpVal,"checkbox%ld",i);
			if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
				snprintf(sqlBuf,1024,"delete from nasAdminArea where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));		
				ltDbExecSql(G_DbCon,sqlBuf); 
			}
		}
		snprintf(caMsg,255,"%s","delete adminarea success.");
	  logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	}
	ltadminarealist(confd,ltMsgPk,lt_MMHead);
	return 0;

}




/*
netarea 控制
*/

int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char strDesc[20];
	char strSnmp[64];
	long iIndex;

  memset(strSnmp,0,sizeof(strSnmp));
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"ipbindlist");

  sprintf(sqlBuf,"select id,ipstart,ipend,netflag,ipdesp,snmpnum from nasNetArea order by  netflag desc,ipstart desc;");
	iIndex=0;
	tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			   
			   if( atol(tempRow[5])== 0 ){
			     sprintf(strSnmp,"%s","不启动snmp读取");
			   }else{
			     sprintf(strSnmp,"启动第[%d]组snmp读取",atol(tempRow[5]));
			   }
			   
		     if(_ltPubInfo->defaultlan==2){/*en*/
					     
						if(atol(tempRow[3])==NAS_NETWORK_NOACCESS){
							sprintf(strDesc,"%s","No Access");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOMON){
							sprintf(strDesc,"%s","No Monitor");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOAUTHCLIENT){
							sprintf(strDesc,"%s","NoAuth Client");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOAUTHSERVER){
							sprintf(strDesc,"%s","NoAuth Server");
						}else if(atol(tempRow[3])==NAS_NETWORK_LOCALAUTH){
							sprintf(strDesc,"%s","Local Auth");
						}else if(atol(tempRow[3])==NAS_NETWORK_NTAUTH){
							sprintf(strDesc,"%s","Domain Auth");
						}else if(atol(tempRow[3])==NAS_NETWORK_HOSTBASE){
							sprintf(strDesc,"%s","Host Bind");
						}else if(atol(tempRow[3])==NAS_NETWORK_MACBASE){
							sprintf(strDesc,"%s","MAC Bind");
						}else if(atol(tempRow[3])==NAS_NETWORK_IPBASE){
							sprintf(strDesc,"%s","IP Bind");
						}else if(atol(tempRow[3])==NAS_NETWORK_HTTPAUTHEBASE){
							sprintf(strDesc,"%s","HTTP Auth");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUT){
							sprintf(strDesc,"%s","Outside IP");
						}else if(atol(tempRow[3])==NAS_NETWORK_PPPOEBASE){
							sprintf(strDesc,"%s","PPPOE User");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUTAUTH){
							sprintf(strDesc,"%s","PPPOE User");
						}else if(atol(tempRow[3])==NAS_NETWORK_LDAPAUTHBASE){
							sprintf(strDesc,"%s","LDAP User");
						}else if(atol(tempRow[3])==NAS_NETWORK_RADUSAUTHBASE){
							sprintf(strDesc,"%s","RADIUS User");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUTWAIT){
							sprintf(strDesc,"%s","OUT AUTH");
						}else if(atol(tempRow[3])==21){
							sprintf(strDesc,"%s","宾馆身份验证");
						}
			
			
		    }else{
		    
		    
					  if(atol(tempRow[3])==NAS_NETWORK_NOACCESS){
							sprintf(strDesc,"%s","禁止访问");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOMON){
							sprintf(strDesc,"%s","不监控");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOAUTHCLIENT){
							sprintf(strDesc,"%s","免过滤内部IP");
						}else if(atol(tempRow[3])==NAS_NETWORK_NOAUTHSERVER){
							sprintf(strDesc,"%s","免过滤外部IP");
						}else if(atol(tempRow[3])==NAS_NETWORK_LDAPAUTHBASE){
							sprintf(strDesc,"%s","LDAP验证用户");
						}else if(atol(tempRow[3])==NAS_NETWORK_RADUSAUTHBASE){
							sprintf(strDesc,"%s","RADIUS验证用户");
						}else if(atol(tempRow[3])==NAS_NETWORK_LOCALAUTH){
							sprintf(strDesc,"%s","本地帐户验证");
						}else if(atol(tempRow[3])==NAS_NETWORK_NTAUTH){
							sprintf(strDesc,"%s","域帐户验证");
						}else if(atol(tempRow[3])==NAS_NETWORK_HOSTBASE){
							sprintf(strDesc,"%s","主机名扫描锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_MACBASE){
							sprintf(strDesc,"%s","MAC扫描锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_IPBASE){
							sprintf(strDesc,"%s","IP扫描锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUT){
							sprintf(strDesc,"%s","外部IP");
						}else if(atol(tempRow[3])==NAS_NETWORK_PPPOEBASE){
							sprintf(strDesc,"%s","PPPOE用户");
						}else if(atol(tempRow[3])==NAS_NETWORK_HTTPAUTHEBASE){
							sprintf(strDesc,"%s","HTTP验证同步");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUTAUTH){
							sprintf(strDesc,"%s","外部验证");
						}else if(atol(tempRow[3])==NAS_NETWORK_IPCLIENT){
							sprintf(strDesc,"%s","IP客户端锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_MACCLIENT){
							sprintf(strDesc,"%s","MAC客户端锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_HOSTCLIENT){
							sprintf(strDesc,"%s","主机名客户端锁定");
						}else if(atol(tempRow[3])==NAS_NETWORK_OUTWAIT){
							sprintf(strDesc,"%s","外部被动身份验证");
						}else if(atol(tempRow[3])==21){
							sprintf(strDesc,"%s","宾馆身份验证");
						}
		  }	
			lt_dbput_recordvars(tablePtr,6,
				"inum",LT_TYPE_LONG,iIndex,
				"listindex",LT_TYPE_STRING,tempRow[0],
				"bip",LT_TYPE_STRING,tempRow[1],
				"eip",LT_TYPE_STRING,tempRow[2],
				"listdesc",LT_TYPE_STRING,strDesc,
				"snmpnum",LT_TYPE_STRING,strSnmp);
			tempRow= ltDbFetchRow(tempCursor);
			iIndex++;
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(strDesc,"%ld",iIndex);
	lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/netarea.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int  actiontype;
	char caMsg[256];
	
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
		long lCount;
		sprintf(sqlBuf,"select count(*) from nasNetArea ");
		lCount =0;
		tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
			ltDbCloseCursor(tempCursor);
		}
		
		if(lCount == 0) {
			lCount=1;
		}else{
			
			sprintf(sqlBuf,"select max(id)+1 from nasNetArea ");
			lCount = 1;
			tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
				ltDbCloseCursor(tempCursor);
			}
		}	
		sprintf(sqlBuf,"insert into nasNetArea values (%ld,'%s','%s',%s,'%s',%s)",lCount,
			ltMsgGetVar_s(ltMsgPk,"bip"),
			ltMsgGetVar_s(ltMsgPk,"eip"),
			ltMsgGetVar_s(ltMsgPk,"netflag"),"",ltMsgGetVar_s(ltMsgPk,"snmpnum"));
//printf("sql:%s\n",sqlBuf);
		ltDbExecSql(G_DbCon,sqlBuf); 
		
		snprintf(caMsg,255,"%s","update netarea success.");
	  logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	
	}else if (actiontype==2){/*del*/
		long allnum,i;
		char tmpVal[20];
		allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
		for(i=0;i<allnum;i++){
			sprintf(tmpVal,"checkbox%ld",i);
			if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
				snprintf(sqlBuf,1024,"delete from nasNetArea where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));		
				ltDbExecSql(G_DbCon,sqlBuf); 
			}
		}
		snprintf(caMsg,255,"%s","delete netarea success.");
	  logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	}
	ltnetarealist(confd,ltMsgPk,lt_MMHead);
	return 0;

}

void catch_myalrm(int signum){
	_exit(0);
}

int ltapplynetarea(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	
//	char sqlBuf[1024];
//	ltDbCursor *tempCursor;
//	LT_DBROW tempRow;
//	ltDbConn *Tmp_DbCon; 
//	unsigned long long lIpB,lIpE,iTemp;
//	
//	char *dbUser;
//	char *dbPass;
//	char *dbName;
//	unsigned long lmaxiphost,lTemp;
//	char  backserverip[32];
//
//	
//	dbName=_ltPubInfo->_dbname;
//	dbUser=_ltPubInfo->_dbuser;
//	dbPass=_ltPubInfo->_dbpass;
//	lmaxiphost=_ltPubInfo->maxiphost;
//	lTemp=0;
//
//	Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//	if (Tmp_DbCon==NULL){
//		return -1;
//	}
//
//	sprintf(sqlBuf,"delete from nasIpHostInfo");
//	ltDbExecSql(G_DbCon,sqlBuf);
//
//  sprintf(sqlBuf,"select id,ipstart,ipend,netflag from nasNetArea order by  netflag desc,ipstart desc");
//	
//	tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
//	if(tempCursor!=NULL){
//		tempRow= ltDbFetchRow(tempCursor);
//		while(tempRow!=NULL){
//			lIpB=ntohl(inet_addr(tempRow[1]));
//			lIpE=ntohl(inet_addr(tempRow[2]));
//			for(iTemp=lIpB;iTemp<=lIpE;iTemp++){
//				struct in_addr s;
//				if(lTemp>lmaxiphost){
//					break;
//				}
//				s.s_addr = htonl(iTemp);
//				sprintf(sqlBuf,"insert into nasIpHostInfo values (%llu,'%s',%s)",iTemp,inet_ntoa(s),tempRow[3]);
//				ltDbExecSql(Tmp_DbCon,sqlBuf);
//				lTemp++;
//				
//			}
//			tempRow= ltDbFetchRow(tempCursor);
//		}
//		ltDbCloseCursor(tempCursor);
//	}
//	ltDbClose(Tmp_DbCon);
	
	
//	{
//		int iPid; 
//	  iPid = fork();
//		if(iPid == 0) {
//			  signal (SIGALRM, catch_myalrm);
//     		alarm (120);
//				memset(backserverip,0,sizeof(backserverip));
//				sprintf(backserverip,"%s",bcCnfGetValueS("/etc/gnm/conf","system","hotbackserver"));
//				if(strlen(backserverip)>4){
//					  printf("backserverip:%s\n",backserverip);
//						remotebacksetting(_ltPubInfo);
//				}
//				_exit(0);
//	  }
//	  wait(NULL);
//	}
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head><meta http-equiv=\"refresh\" content=\"20;URL=/main/main.xp?Fun=ltnetarealist\" ></head><body bgcolor=\"#cccccc\" >Please try  20 seconds later.......</body></html>");		
	system("killall -1 gnmmainproc");
	ltMsgFree(ltMsgPk);
	
	return 0;

	/*
	nasInitIpHostInfo(lt_MMHead);

	ltnetarealist(confd,ltMsgPk,lt_MMHead);
	
	return 0;
	*/

}






int ltwebproxyconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char    ENABLEPROXY[24];
	char    TRANSPARENTPROXY[24];
	char    PROXYPORT[24];
	char    PROXYADMIN[24];
	char    PROXYLOG[24];
	char    PROXYHOST[24];
	char    PROXYMEMCACHE[24];
	char    PROXYCACHE[24];
	char    PROXYMINCACHE[24];
	char    PROXYMAXCACHE[24];
	char    PROXYTIMEWEEK[24];
	char    PROXYTIMESTART[24];
	char    PROXYTIMEEND[24];
	char    PROXYADAUTH[24];



	char    *pFile="/etc/gnm/proxy/settings";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	
	 if( bcCnfGetValueL("/etc/gnm/conf","advance", "webproxy")==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
	 }else{
			memset(ENABLEPROXY,0,sizeof(ENABLEPROXY));
			bcCnfGetValue_s(pFile,"ENABLEPROXY",ENABLEPROXY);
			if( strcmp(ENABLEPROXY,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"greensel","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
		  }
		}
		  
		  memset(PROXYPORT,0,sizeof(PROXYPORT));
			bcCnfGetValue_s(pFile,"PROXYPORT",PROXYPORT);
	    lt_dbput_rootvars(dbPtr,1,"proxyport",PROXYPORT );	
			
			
			memset(TRANSPARENTPROXY,0,sizeof(TRANSPARENTPROXY));
			bcCnfGetValue_s(pFile,"TRANSPARENTPROXY",TRANSPARENTPROXY);
			if( strcmp(TRANSPARENTPROXY,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"transparent","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"transparent"," " );	
		  }
			
			
			 memset(PROXYHOST,0,sizeof(PROXYHOST));
			bcCnfGetValue_s(pFile,"PROXYHOST",PROXYHOST);
	    lt_dbput_rootvars(dbPtr,1,"proxyhost",PROXYHOST );	
			
			 memset(PROXYADMIN,0,sizeof(PROXYADMIN));
			bcCnfGetValue_s(pFile,"PROXYADMIN",PROXYADMIN);
	    lt_dbput_rootvars(dbPtr,1,"proxyadmin",PROXYADMIN );	
			
			memset(PROXYLOG,0,sizeof(PROXYLOG));
			bcCnfGetValue_s(pFile,"PROXYLOG",PROXYLOG);
			if( strcmp(PROXYLOG,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"logcheck","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"logcheck"," " );	
		  }
			
			
			memset(PROXYMEMCACHE,0,sizeof(PROXYMEMCACHE));
			bcCnfGetValue_s(pFile,"PROXYMEMCACHE",PROXYMEMCACHE);
	    lt_dbput_rootvars(dbPtr,1,"cachemem",PROXYMEMCACHE );	
			
			 memset(PROXYCACHE,0,sizeof(PROXYCACHE));
			bcCnfGetValue_s(pFile,"PROXYCACHE",PROXYCACHE);
	    lt_dbput_rootvars(dbPtr,1,"cachesize",PROXYCACHE );	
	    
	    memset(PROXYMINCACHE,0,sizeof(PROXYMINCACHE));
			bcCnfGetValue_s(pFile,"PROXYMINCACHE",PROXYMINCACHE);
	    lt_dbput_rootvars(dbPtr,1,"mincache",PROXYMINCACHE );	
			
			 memset(PROXYMAXCACHE,0,sizeof(PROXYMAXCACHE));
			bcCnfGetValue_s(pFile,"PROXYMAXCACHE",PROXYMAXCACHE);
	    lt_dbput_rootvars(dbPtr,1,"maxcache",PROXYMAXCACHE );	
	    
	    memset(PROXYTIMEWEEK,0,sizeof(PROXYTIMEWEEK));
			bcCnfGetValue_s(pFile,"PROXYTIMEWEEK",PROXYTIMEWEEK);
			if( strstr(PROXYTIMEWEEK,"M") ){
	    		lt_dbput_rootvars(dbPtr,1,"checkm"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"T") ){
	    		lt_dbput_rootvars(dbPtr,1,"checkt"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"W") ){
	    		lt_dbput_rootvars(dbPtr,1,"checkw"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"H") ){
	    		lt_dbput_rootvars(dbPtr,1,"checkh"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"F") ){
	    		lt_dbput_rootvars(dbPtr,1,"checkf"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"A") ){
	    		lt_dbput_rootvars(dbPtr,1,"checka"," checked " );	
	    }
	    if( strstr(PROXYTIMEWEEK,"S") ){
	    		lt_dbput_rootvars(dbPtr,1,"checks"," checked " );	
	    }
	    
	    
	    memset(PROXYTIMESTART,0,sizeof(PROXYTIMESTART));
			bcCnfGetValue_s(pFile,"PROXYTIMESTART",PROXYTIMESTART);
	    lt_dbput_rootvars(dbPtr,1,"timestart",PROXYTIMESTART );	
	    
	    memset(PROXYTIMEEND,0,sizeof(PROXYTIMEEND));
			bcCnfGetValue_s(pFile,"PROXYTIMEEND",PROXYTIMEEND);
	    lt_dbput_rootvars(dbPtr,1,"timeend",PROXYTIMEEND );	
	    
	    memset(PROXYADAUTH,0,sizeof(PROXYADAUTH));
			bcCnfGetValue_s(pFile,"PROXYADAUTH",PROXYADAUTH);
			if( strcmp(PROXYADAUTH,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"adauthcheck","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"adauthcheck"," " );	
		  }
	
	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/webproxy.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltwebproxyconfok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    *ENABLEPROXY;
	char    *TRANSPARENTPROXY;
	char    *PROXYPORT;
	char    *PROXYADMIN;
	char    *PROXYLOG;
	char    *PROXYHOST;
	char    *PROXYMEMCACHE;
	char    *PROXYCACHE;
	char    *PROXYMINCACHE;
	char    *PROXYMAXCACHE;
	char    PROXYTIMEWEEK[24];
	char    *PROXYTIMESTART;
	char    *PROXYTIMEEND;
	char    *PROXYADAUTH;
	char    caCmdCon[128];
	ltDbHeadPtr dbPtr;
	char *lt_page_content;
	int  iFd;
	char    *pFile="/etc/gnm/proxy/settings";
	
		
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
  
  if( bcCnfGetValueL("/etc/gnm/conf","advance", "webproxy")==0 ){
	 	 ENABLEPROXY="NO";
	}else{
		ENABLEPROXY=ltMsgGetVar_s(ltMsgPk,"ENABLEPROXY");
		if(ENABLEPROXY==NULL){
			ENABLEPROXY="NO";
		}
	}
	
	PROXYPORT=ltMsgGetVar_s(ltMsgPk,"PROXYPORT");
	lt_dbput_rootvars(dbPtr,1,"PROXYPORT",PROXYPORT );	
	
	TRANSPARENTPROXY=ltMsgGetVar_s(ltMsgPk,"TRANSPARENTPROXY");
	if(TRANSPARENTPROXY==NULL){
		TRANSPARENTPROXY="NO";
	}
	
	
	PROXYLOG=ltMsgGetVar_s(ltMsgPk,"PROXYLOG");
	if(PROXYLOG==NULL){
		PROXYLOG="NO";
		lt_dbput_rootvars(dbPtr,1,"PROXYLOG"," none " );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"PROXYLOG"," /app/webproxy/var/logs/access.log " );	
  }
  
  PROXYADAUTH=ltMsgGetVar_s(ltMsgPk,"PROXYADAUTH");
	if(PROXYADAUTH==NULL){
		PROXYADAUTH="NO";
	
	}
	
	
	PROXYHOST=ltMsgGetVar_s(ltMsgPk,"PROXYHOST");
	lt_dbput_rootvars(dbPtr,1,"PROXYHOST",PROXYHOST );	
		
	PROXYADMIN=ltMsgGetVar_s(ltMsgPk,"PROXYADMIN");	
	lt_dbput_rootvars(dbPtr,1,"PROXYADMIN",PROXYADMIN );
	
	PROXYMEMCACHE=ltMsgGetVar_s(ltMsgPk,"PROXYMEMCACHE");
	lt_dbput_rootvars(dbPtr,1,"PROXYMEMCACHE",PROXYMEMCACHE );
	PROXYCACHE=ltMsgGetVar_s(ltMsgPk,"PROXYCACHE");	
	lt_dbput_rootvars(dbPtr,1,"PROXYCACHE",PROXYCACHE );
	PROXYMINCACHE=ltMsgGetVar_s(ltMsgPk,"PROXYMINCACHE");
	lt_dbput_rootvars(dbPtr,1,"PROXYMINCACHE",PROXYMINCACHE );
	PROXYMAXCACHE=ltMsgGetVar_s(ltMsgPk,"PROXYMAXCACHE");
	lt_dbput_rootvars(dbPtr,1,"PROXYMAXCACHE",PROXYMAXCACHE );	
	
	memset(PROXYTIMEWEEK,0,sizeof(PROXYTIMEWEEK));
	if(ltMsgGetVar_s(ltMsgPk,"TIME_MON")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='M';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_TUE")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='T';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_WED")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='W';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_THU")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='H';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_FRI")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='F';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_SAT")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='A';
	}
	if(ltMsgGetVar_s(ltMsgPk,"TIME_SUN")){
		PROXYTIMEWEEK[strlen(PROXYTIMEWEEK)]='S';
	}
	
	PROXYTIMESTART=ltMsgGetVar_s(ltMsgPk,"PROXYTIMESTART");
	PROXYTIMEEND=ltMsgGetVar_s(ltMsgPk,"PROXYTIMEEND");	
	
	lt_dbput_rootvars(dbPtr,1,"PROXYTIMEWEEK",PROXYTIMEWEEK );	
	lt_dbput_rootvars(dbPtr,1,"PROXYTIMESTART",PROXYTIMESTART );	
	lt_dbput_rootvars(dbPtr,1,"PROXYTIMEEND",PROXYTIMEEND );
	
	if( strcmp(PROXYADAUTH,"YES")==0){
		lt_page_content=ltPltFileParse("/etc/gnm/proxy/squid.conf.p.ad",dbPtr->doc,0);
  }else{
    lt_page_content=ltPltFileParse("/etc/gnm/proxy/squid.conf.p",dbPtr->doc,0);
  }
	if(lt_page_content==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: can;t create proxy conf file!. </body></html>");		
			ltMsgFree(ltMsgPk);
	}else{
	  lt_page_content=strstr(lt_page_content,"http_port");
		iFd = open("/etc/gnm/proxy/squid.conf",O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
    if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
    }
    close(iFd);
	  free(lt_page_content);
	  system("/bin/cp -f /etc/gnm/proxy/squid.conf /app/webproxy/etc/squid.conf");
	  system("killall -9 squid");
  }
		
	system("/bin/cp /etc/gnm/proxy/settings /etc/gnm/proxy/settings.old");
		/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEPROXY=%s' > %s",ENABLEPROXY,pFile);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'TRANSPARENTPROXY=%s' >> %s",TRANSPARENTPROXY,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'PROXYPORT=%s' >> %s",PROXYPORT,pFile);
	system(caCmdCon);
	
		sprintf(caCmdCon,"echo 'PROXYADMIN=%s' >> %s",PROXYADMIN,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYLOG=%s' >> %s",PROXYLOG,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYHOST=%s' >> %s",PROXYHOST,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYMEMCACHE=%s' >> %s",PROXYMEMCACHE,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYCACHE=%s' >> %s",PROXYCACHE,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYMINCACHE=%s' >> %s",PROXYMINCACHE,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYMAXCACHE=%s' >> %s",PROXYMAXCACHE,pFile);
		system(caCmdCon);
		
		sprintf(caCmdCon,"echo 'PROXYTIMEWEEK=%s' >> %s",PROXYTIMEWEEK,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYTIMESTART=%s' >> %s",PROXYTIMESTART,pFile);
		system(caCmdCon);
		sprintf(caCmdCon,"echo 'PROXYTIMEEND=%s' >> %s",PROXYTIMEEND,pFile);
		system(caCmdCon);
		
		sprintf(caCmdCon,"echo 'PROXYADAUTH=%s' >> %s",PROXYADAUTH,pFile);
		system(caCmdCon);

		

	    
	 ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. <br>系统服务配置成功，请重新启动系统。</body></html>");		
		ltMsgFree(ltMsgPk);
	
    return 0;

}



int ltsmtpscansetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char ENABLESMTPSCAN[24];

	char databytes[24];
	char maxrcptcount[24];
	char tarpitcount[24];
	char tarpitdelay[24];

	

	char    *pFile="/etc/gnm/mailscan/smtpscan.setting";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if( bcCnfGetValueL("/etc/gnm/conf","advance", "mailctl")==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"ENABLESMTPSCANSEL"," " );	
	}else{
		memset(ENABLESMTPSCAN,0,sizeof(ENABLESMTPSCAN));
		bcCnfGetValue_s(pFile,"ENABLESMTPSCAN",ENABLESMTPSCAN);
		if( strcmp(ENABLESMTPSCAN,"YES")==0){
			lt_dbput_rootvars(dbPtr,1,"ENABLESMTPSCANSEL","checked" );	
		}else{
			lt_dbput_rootvars(dbPtr,1,"ENABLESMTPSCANSEL"," " );	
		}
	}


	memset(databytes,0,sizeof(databytes));
	bcCnfGetValue_s(pFile,"databytes",databytes);
	lt_dbput_rootvars(dbPtr,1,"databytes",databytes );	
	
	memset(maxrcptcount,0,sizeof(maxrcptcount));
	bcCnfGetValue_s(pFile,"maxrcptcount",maxrcptcount);
	lt_dbput_rootvars(dbPtr,1,"maxrcptcount",maxrcptcount );
	
	memset(tarpitcount,0,sizeof(tarpitcount));
	bcCnfGetValue_s(pFile,"tarpitcount",tarpitcount);
	lt_dbput_rootvars(dbPtr,1,"tarpitcount",tarpitcount);
	
	memset(tarpitdelay,0,sizeof(tarpitdelay));
	bcCnfGetValue_s(pFile,"tarpitdelay",tarpitdelay);
	lt_dbput_rootvars(dbPtr,1,"tarpitdelay",tarpitdelay );	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/smtpscanface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltsmtpscanok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char    *pFile="/etc/gnm/mailscan/smtpscan.setting";
	char    *ENABLESMTPSCAN;

	char    *databytes;
	char    *maxrcptcount;
	char    *tarpitcount;
	char    *tarpitdelay;

	char    caCmdCon[512];
  
  if(  bcCnfGetValueL("/etc/gnm/conf","advance", "mailctl")==0 ){
	 	 ENABLESMTPSCAN="NO";
	}else{
		ENABLESMTPSCAN=ltMsgGetVar_s(ltMsgPk,"ENABLESMTPSCAN");
		if(ENABLESMTPSCAN==NULL){
			ENABLESMTPSCAN="NO";
		}
	}
	

	databytes=ltMsgGetVar_s(ltMsgPk,"databytes");
	maxrcptcount=ltMsgGetVar_s(ltMsgPk,"maxrcptcount");
	tarpitcount=ltMsgGetVar_s(ltMsgPk,"tarpitcount");
	tarpitdelay=ltMsgGetVar_s(ltMsgPk,"tarpitdelay");
	

	
	system("/bin/cp /etc/gnm/mailscan/smtpscan.setting /etc/gnm/mailscan/smtpscan.setting.old");
	/*连接方式*/
	sprintf(caCmdCon,"echo 'ENABLESMTPSCAN=%s' > %s",ENABLESMTPSCAN,pFile);
	system(caCmdCon);
		

	
	sprintf(caCmdCon,"echo 'databytes=%s' >> %s",databytes,pFile);
	system(caCmdCon);
	sprintf(caCmdCon,"echo '%s' > /var/qmail/control/databytes",databytes);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'maxrcptcount=%s' >> %s",maxrcptcount,pFile);
	system(caCmdCon);
	sprintf(caCmdCon,"echo '%s' > /var/qmail/control/maxrcptcount",maxrcptcount);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'tarpitcount=%s' >> %s",tarpitcount,pFile);
	system(caCmdCon);
	sprintf(caCmdCon,"echo '%s' > /var/qmail/control/tarpitcount",tarpitcount);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'tarpitdelay=%s' >> %s",tarpitdelay,pFile);
	system(caCmdCon);
	sprintf(caCmdCon,"echo '%s' > /var/qmail/control/tarpitdelay",tarpitdelay);
	system(caCmdCon);
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. </body></html>");		
	ltMsgFree(ltMsgPk);

	return 0;

}




int ltp3scansetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char ENABLEP3SCAN[24];
	char MAXCHILDS[24];
	char SPAMSCAN[24];
	char VIRNUSSCAN[24];

	char    *pFile="/etc/gnm/mailscan/p3scan.setting";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if(  bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"ENABLEP3SCANSEL"," " );	
	}else{
		memset(ENABLEP3SCAN,0,sizeof(ENABLEP3SCAN));
		bcCnfGetValue_s(pFile,"ENABLEP3SCAN",ENABLEP3SCAN);
		if( strcmp(ENABLEP3SCAN,"YES")==0){
			lt_dbput_rootvars(dbPtr,1,"ENABLEP3SCANSEL","checked" );	
		}else{
			lt_dbput_rootvars(dbPtr,1,"ENABLEP3SCANSEL"," " );	
		}
	}
	
	memset(SPAMSCAN,0,sizeof(SPAMSCAN));
	bcCnfGetValue_s(pFile,"SPAMSCAN",SPAMSCAN);
	if( strcmp(SPAMSCAN,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"SPAMSCANSEL","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"SPAMSCANSEL"," " );	
	}
	
	memset(VIRNUSSCAN,0,sizeof(VIRNUSSCAN));
	bcCnfGetValue_s(pFile,"VIRNUSSCAN",VIRNUSSCAN);
	if( strcmp(VIRNUSSCAN,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"VIRNUSSCANSEL","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"VIRNUSSCANSEL"," " );	
	}
	
	memset(MAXCHILDS,0,sizeof(MAXCHILDS));
	bcCnfGetValue_s(pFile,"MAXCHILDS",MAXCHILDS);
	lt_dbput_rootvars(dbPtr,1,"MAXCHILDS",MAXCHILDS );

		
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/p3scanface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltp3scanok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char    *pFile="/etc/gnm/mailscan/p3scan.setting";
	char 		*ENABLEP3SCAN;
	char 		*MAXCHILDS;
	char 		*SPAMSCAN;
	char 		*VIRNUSSCAN;

	char    caCmdCon[512];
	ltDbHeadPtr dbPtr;
	char *lt_page_content;
	int  iFd;

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");


	MAXCHILDS=ltMsgGetVar_s(ltMsgPk,"MAXCHILDS");
	lt_dbput_rootvars(dbPtr,1,"maxchilds",MAXCHILDS );	


  if( bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 ENABLEP3SCAN="NO";
	}else{
		ENABLEP3SCAN=ltMsgGetVar_s(ltMsgPk,"ENABLEP3SCAN");
		if(ENABLEP3SCAN==NULL){
			ENABLEP3SCAN="NO";
		}
	}
	
	SPAMSCAN=ltMsgGetVar_s(ltMsgPk,"SPAMSCAN");
	if(SPAMSCAN==NULL){
		SPAMSCAN="NO";
		lt_dbput_rootvars(dbPtr,1,"checkspam","#checkspam" );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"checkspam","checkspam" );	
  }
	
	VIRNUSSCAN=ltMsgGetVar_s(ltMsgPk,"VIRNUSSCAN");
	if(VIRNUSSCAN==NULL){
		VIRNUSSCAN="NO";
		lt_dbput_rootvars(dbPtr,1,"scanner","#scanner = /app/verus/bin/clamdscan --no-summary" );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"scanner","scanner = /app/verus/bin/clamdscan --no-summary" );	
  }
	
	lt_page_content=ltPltFileParse("/etc/gnm/mailscan/p3scan.conf.tpl",dbPtr->doc,0);

	if(lt_page_content==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: can;t create pop3 scan conf file!. </body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
	}else{
	  lt_page_content=strstr(lt_page_content,"#");
		iFd = open("/etc/gnm/mailscan/p3scan.conf",O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
    if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
    }
    close(iFd);
	  
  }

	
	system("/bin/cp /etc/gnm/mailscan/p3scan.setting /etc/gnm/mailscan/p3scan.setting.old");
	/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEP3SCAN=%s' > %s",ENABLEP3SCAN,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'SPAMSCAN=%s' >> %s",SPAMSCAN,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'VIRNUSSCAN=%s' >> %s",VIRNUSSCAN,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'MAXCHILDS=%s' >> %s",MAXCHILDS,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. </body></html>");		
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	free(lt_page_content);
	return 0;

}




int ltspamsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char ENABLESPAMD[24];
	char required_score[24];

	char rewrite_header[64];
	

	char    *pFile="/etc/gnm/mailscan/spam.setting";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if( bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"ENABLESPAMDSEL"," " );	
	}else{
		memset(ENABLESPAMD,0,sizeof(ENABLESPAMD));
		bcCnfGetValue_s(pFile,"ENABLESPAMD",ENABLESPAMD);
		if( strcmp(ENABLESPAMD,"YES")==0){
			lt_dbput_rootvars(dbPtr,1,"ENABLESPAMDSEL","checked" );	
		}else{
			lt_dbput_rootvars(dbPtr,1,"ENABLESPAMDSEL"," " );	
		}
	}
	
	memset(required_score,0,sizeof(required_score));
	bcCnfGetValue_s(pFile,"required_score",required_score);
	lt_dbput_rootvars(dbPtr,1,"required_score",required_score );

	memset(rewrite_header,0,sizeof(rewrite_header));
	bcCnfGetValue_s(pFile,"rewrite_header",rewrite_header);
	lt_dbput_rootvars(dbPtr,1,"rewrite_header",rewrite_header );	
		
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/spamface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltspamok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char    *pFile="/etc/gnm/mailscan/spam.setting";
	char    *required_score;
	char    *rewrite_header;
	char    *ENABLESPAMD;

	char    caCmdCon[512];
	ltDbHeadPtr dbPtr;
	char *lt_page_content;
	int  iFd;

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");


	required_score=ltMsgGetVar_s(ltMsgPk,"required_score");
	lt_dbput_rootvars(dbPtr,1,"required_score",required_score );	
	
	rewrite_header=ltMsgGetVar_s(ltMsgPk,"rewrite_header");
	lt_dbput_rootvars(dbPtr,1,"rewrite_header",rewrite_header );	
  
  if( bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 ENABLESPAMD="NO";
	}else{
		ENABLESPAMD=ltMsgGetVar_s(ltMsgPk,"ENABLESPAMD");
		if(ENABLESPAMD==NULL){
			ENABLESPAMD="NO";
		}
	}
	
	lt_page_content=ltPltFileParse("/etc/gnm/mailscan/local.cf.tpl",dbPtr->doc,0);

	if(lt_page_content==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: can;t create spam conf file!. </body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
	}else{
	  lt_page_content=strstr(lt_page_content,"#");
		iFd = open("/etc/gnm/mailscan/local.cf",O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
    if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
    }
    close(iFd);
	  
  }

	
	system("/bin/cp /etc/gnm/mailscan/spam.setting /etc/gnm/mailscan/spam.setting.old");
	/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLESPAMD=%s' > %s",ENABLESPAMD,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'required_score=%s' >> %s",required_score,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'rewrite_header=%s' >> %s",rewrite_header,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. </body></html>");		
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	free(lt_page_content);
	return 0;

}




int ltvernussetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char ENABLEVERNUSD[24];
	char maxthreads[24];

	char ScanHTML[24];
	char ScanArchive[24];
	char ScanRAR[24];
	char ArchiveMaxFileSize[24];
	char ArchiveMaxRecursion[24];
	char ArchiveMaxFiles[24];
	

	char    *pFile="/etc/gnm/mailscan/virnus.setting";
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if( bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"ENABLEVERNUSDSEL"," " );	
	}else{
		memset(ENABLEVERNUSD,0,sizeof(ENABLEVERNUSD));
		bcCnfGetValue_s(pFile,"ENABLEVERNUSD",ENABLEVERNUSD);
		if( strcmp(ENABLEVERNUSD,"YES")==0){
			lt_dbput_rootvars(dbPtr,1,"ENABLEVERNUSDSEL","checked" );	
		}else{
			lt_dbput_rootvars(dbPtr,1,"ENABLEVERNUSDSEL"," " );	
		}
	}
	memset(maxthreads,0,sizeof(maxthreads));
	bcCnfGetValue_s(pFile,"maxthreads",maxthreads);
	lt_dbput_rootvars(dbPtr,1,"maxthreads",maxthreads );

	memset(ScanHTML,0,sizeof(ScanHTML));
	bcCnfGetValue_s(pFile,"ScanHTML",ScanHTML);
	if( strcmp(ScanHTML,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"ScanHTMLsel","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ScanHTMLsel"," " );	
	}
	
	memset(ScanArchive,0,sizeof(ScanArchive));
	bcCnfGetValue_s(pFile,"ScanArchive",ScanArchive);
	if( strcmp(ScanArchive,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"ScanArchivesel","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ScanArchivesel"," " );	
	}
	
	memset(ScanRAR,0,sizeof(ScanRAR));
	bcCnfGetValue_s(pFile,"ScanRAR",ScanRAR);
	if( strcmp(ScanRAR,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"ScanRARsel","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"ScanRARsel"," " );	
	}
	
	memset(ArchiveMaxFileSize,0,sizeof(ArchiveMaxFileSize));
	bcCnfGetValue_s(pFile,"ArchiveMaxFileSize",ArchiveMaxFileSize);
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxFileSize",ArchiveMaxFileSize );
	
	
	memset(ArchiveMaxRecursion,0,sizeof(ArchiveMaxRecursion));
	bcCnfGetValue_s(pFile,"ArchiveMaxRecursion",ArchiveMaxRecursion);
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxRecursion",ArchiveMaxRecursion );	
	
	memset(ArchiveMaxFiles,0,sizeof(ArchiveMaxFiles));
	bcCnfGetValue_s(pFile,"ArchiveMaxFiles",ArchiveMaxFiles);
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxFiles",ArchiveMaxFiles );	
		
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/vernusface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltvernusok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char    *pFile="/etc/gnm/mailscan/virnus.setting";
	char    *maxthreads;
	char    *ScanHTML;
	char    *ScanArchive;
	char    *ScanRAR;
	
	char    *ArchiveMaxFileSize;
	char    *ArchiveMaxRecursion;
	char    *ArchiveMaxFiles;
	char    *ENABLEVERNUSD;

	char    caCmdCon[512];
	ltDbHeadPtr dbPtr;
	char *lt_page_content;
	int  iFd;

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");


	maxthreads=ltMsgGetVar_s(ltMsgPk,"maxthreads");
	lt_dbput_rootvars(dbPtr,1,"maxthreads",maxthreads );	
	
	ScanHTML=ltMsgGetVar_s(ltMsgPk,"ScanHTML");
	if(ScanHTML==NULL){
		ScanHTML="NO";
		lt_dbput_rootvars(dbPtr,1,"ScanHTML","#ScanHTML" );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"ScanHTML","ScanHTML" );	
  }
	
	ScanArchive=ltMsgGetVar_s(ltMsgPk,"ScanArchive");
	if(ScanArchive==NULL){
		ScanArchive="NO";
		lt_dbput_rootvars(dbPtr,1,"ScanArchive","#ScanArchive" );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"ScanArchive","ScanArchive" );	
  }
	
	ScanRAR=ltMsgGetVar_s(ltMsgPk,"ScanRAR");
	if(ScanRAR==NULL){
		ScanRAR="NO";
		lt_dbput_rootvars(dbPtr,1,"ScanRAR","#ScanRAR" );	
	}else{
	  lt_dbput_rootvars(dbPtr,1,"ScanRAR","ScanRAR" );	
  }
	
	if( bcCnfGetValueL("/etc/gnm/conf","advance", "pop3ctl")==0 ){
	 	 ENABLEVERNUSD="NO";
	}else{
		ENABLEVERNUSD=ltMsgGetVar_s(ltMsgPk,"ENABLEVERNUSD");
		if(ENABLEVERNUSD==NULL){
			ENABLEVERNUSD="NO";
		}
	}
	
	ArchiveMaxFileSize=ltMsgGetVar_s(ltMsgPk,"ArchiveMaxFileSize");
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxFileSize",ArchiveMaxFileSize );	
	
	ArchiveMaxRecursion=ltMsgGetVar_s(ltMsgPk,"ArchiveMaxRecursion");
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxRecursion",ArchiveMaxRecursion );	
	
	ArchiveMaxFiles=ltMsgGetVar_s(ltMsgPk,"ArchiveMaxFiles");
	lt_dbput_rootvars(dbPtr,1,"ArchiveMaxFiles",ArchiveMaxFiles );	

	
	
	lt_page_content=ltPltFileParse("/etc/gnm/mailscan/clamd.conf.tpl",dbPtr->doc,0);

	if(lt_page_content==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: can;t create virnus conf file!. </body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
	}else{
	  lt_page_content=strstr(lt_page_content,"#");
		iFd = open("/etc/gnm/mailscan/clamd.conf",O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
    if(iFd >0) {
          write(iFd,lt_page_content,strlen(lt_page_content));
    }
    close(iFd);
	  
  }

	
	system("/bin/cp /etc/gnm/mailscan/virnus.setting /etc/gnm/mailscan/virnus.setting.old");
	/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEVERNUSD=%s' > %s",ENABLEVERNUSD,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'maxthreads=%s' >> %s",maxthreads,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'ScanHTML=%s' >> %s",ScanHTML,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'ScanArchive=%s' >> %s",ScanArchive,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'ScanRAR=%s' >> %s",ScanRAR,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'ArchiveMaxFileSize=%s' >> %s",ArchiveMaxFileSize,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'ArchiveMaxRecursion=%s' >> %s",ArchiveMaxRecursion,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'ArchiveMaxFiles=%s' >> %s",ArchiveMaxFiles,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. </body></html>");		
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	free(lt_page_content);
	return 0;

}

int ltidssetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char    ENABLEGREEN[24];

	char    IDSREPORTKEEPDAY[24];
	char    IDSLOGKEEPDAY[24];
	

	char    *pFile="/etc/gnm/ids/settings";
	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	memset(ENABLEGREEN,0,sizeof(ENABLEGREEN));
	bcCnfGetValue_s(pFile,"ENABLEIDS",ENABLEGREEN);
	if( strcmp(ENABLEGREEN,"YES")==0){
		lt_dbput_rootvars(dbPtr,1,"idssel","checked" );	
	}else{
		lt_dbput_rootvars(dbPtr,1,"idssel"," " );	
	 }
	 

	
	memset(IDSLOGKEEPDAY,0,sizeof(IDSLOGKEEPDAY));
	bcCnfGetValue_s(pFile,"IDSLOGKEEPDAY",IDSLOGKEEPDAY);
	lt_dbput_rootvars(dbPtr,1,"IDSLOGKEEPDAY",IDSLOGKEEPDAY );
	
	
	memset(IDSREPORTKEEPDAY,0,sizeof(IDSREPORTKEEPDAY));
	bcCnfGetValue_s(pFile,"IDSREPORTKEEPDAY",IDSREPORTKEEPDAY);
	lt_dbput_rootvars(dbPtr,1,"IDSREPORTKEEPDAY",IDSREPORTKEEPDAY );	
		
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/idsface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltidsok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    *ENABLEGREEN;

	char    *IDSREPORTKEEPDAY;
	char    *IDSLOGKEEPDAY;

	char    caCmdCon[512];
	char    *pFile="/etc/gnm/ids/settings";
		
	ENABLEGREEN=ltMsgGetVar_s(ltMsgPk,"ENABLEIDS");
	if(ENABLEGREEN==NULL){
		ENABLEGREEN="NO";
	}
	

	IDSREPORTKEEPDAY=ltMsgGetVar_s(ltMsgPk,"IDSREPORTKEEPDAY");
	IDSLOGKEEPDAY=ltMsgGetVar_s(ltMsgPk,"IDSLOGKEEPDAY");
	
	system("/bin/cp /etc/gnm/ids/settings /etc/gnm/ids/settings.old");
	/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEIDS=%s' > %s",ENABLEGREEN,pFile);
	system(caCmdCon);
		
	
	sprintf(caCmdCon,"echo 'IDSREPORTKEEPDAY=%s' >> %s",IDSREPORTKEEPDAY,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'IDSLOGKEEPDAY=%s' >> %s",IDSLOGKEEPDAY,pFile);
	system(caCmdCon);
	
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: please reboot your system to be applyed. <br>系统服务配置成功，请重新启动系统。</body></html>");		
	ltMsgFree(ltMsgPk);
	
  return 0;

}

static char inbuf[512];
static stralloc line = {0};
int ltidslogdetail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    *logid;
  buffer  ss;
  int     fd;
  int     match;
  char    caFile[256];
  stralloc myCon = {0};
     
  
  logid=ltMsgGetVar_s(ltMsgPk,"logid");
	if(logid==NULL){
		logid="";
		sprintf(caFile,"/var/log/snort/alert");
	}else{
		sprintf(caFile,"/app/ns/conlog/idslog/log/%s",logid);
  }
  
  fd = open_read(caFile);
  if (fd == -1)
  {
      ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: File isn't exist.</body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
  }

 buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
 
 stralloc_cats(&myCon,"<html>");
 stralloc_cats(&myCon,"<body bgcolor=\"#f0f0f0\" text=\"#000000\">");
 
 for (;;)
  {
	   if (getln(&ss,&line,&match,'\n') == -1) break;
	   if (!match && !line.len) { break; }
	   if(line.len<2){
	   	stralloc_cats(&line,"<hr>");
	   }else{
	   	stralloc_cats(&line,"<br>");
	   }
     if (!stralloc_0(&line)) break;
     //write(confd,line.s,line.len);
     stralloc_cats(&myCon,line.s);
  }
 stralloc_cats(&myCon,"</body>");
 stralloc_0(&myCon);
 ltMsgPk->msgpktype=1;
 //lt_TcpResponse(confd,ltMsgPk,1,"filename",LT_TYPE_STRING,"idslog");	
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
			"lthtml",LT_TYPE_STRING,myCon.s);			

 close(fd);
 ltMsgFree(ltMsgPk);
 return 0;

}


int ltidsloglist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
  struct dirent *d;

  DIR    *dir;	
  ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
  
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"infolist");
  
  if ( dir = opendir("/app/ns/conlog/idslog/log/") ) {
      while ( d = readdir(dir) ) {
            if (d->d_name[0] == '.') continue;
            lt_dbput_recordvars(tablePtr,2,
							"logname",LT_TYPE_STRING,d->d_name,
							"logid",LT_TYPE_STRING,d->d_name);
       }
  }
  
 lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/idsloglist.htm",dbPtr->head,0);

 ltMsgFree(ltMsgPk);
 lt_dbfree(dbPtr);
 return 0;

}


int ltidsreportlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
  struct dirent *d;
 // struct stat st;
  DIR    *dir;	
  ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
  
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"infolist");
  
  if ( dir = opendir("/app/ns/conlog/idslog/report/") ) {
      while ( d = readdir(dir) ) {
            if (d->d_name[0] == '.') continue;
            lt_dbput_recordvars(tablePtr,2,
							"logname",LT_TYPE_STRING,d->d_name,
							"logid",LT_TYPE_STRING,d->d_name);
       }
  }
  
 lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/idsreportlist.htm",dbPtr->head,0);

 ltMsgFree(ltMsgPk);
 lt_dbfree(dbPtr);
 return 0;

}


//***************************
//***************************
//***************************
//***************************
int ltbandlink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	char    ENABLEBAND[24];
	char    BANDUPLINK[24];
	char    BANDDOWNLINK[24];
	char    BANDUPDEF[24];
	char    BANDDOWNDEF[24];

	char    *pFile="/etc/gnm/qos/settings";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if( atol(bcCnfGetValueS("/etc/gnm/conf","advance", "bandlimit"))==0 ){
	 	 lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
	}else{
			memset(ENABLEBAND,0,sizeof(ENABLEBAND));
			bcCnfGetValue_s(pFile,"ENABLEBAND",ENABLEBAND);
			if( strcmp(ENABLEBAND,"YES")==0){
				lt_dbput_rootvars(dbPtr,1,"greensel","checked" );	
			}else{
				lt_dbput_rootvars(dbPtr,1,"greensel"," " );	
		  }
	}  
		  memset(BANDUPLINK,0,sizeof(BANDUPLINK));
			bcCnfGetValue_s(pFile,"BANDUPLINK",BANDUPLINK);
	    lt_dbput_rootvars(dbPtr,1,"BANDUPLINK",BANDUPLINK );	
			
			
		  memset(BANDDOWNLINK,0,sizeof(BANDDOWNLINK));
		 	bcCnfGetValue_s(pFile,"BANDDOWNLINK",BANDDOWNLINK);
	    lt_dbput_rootvars(dbPtr,1,"BANDDOWNLINK",BANDDOWNLINK );	
			
			if(ltStrIsDigital(BANDDOWNLINK)==0){
				_ltPubInfo->banddownlink=atol(BANDDOWNLINK);
			}
			if(ltStrIsDigital(BANDUPLINK)==0){
				_ltPubInfo->banduplink=atol(BANDUPLINK);
			}
			
			memset(BANDUPDEF,0,sizeof(BANDUPDEF));
			bcCnfGetValue_s(pFile,"BANDUPDEF",BANDUPDEF);
	    lt_dbput_rootvars(dbPtr,1,"BANDUPDEF",BANDUPDEF );	
			
			
		  memset(BANDDOWNDEF,0,sizeof(BANDDOWNDEF));
		 	bcCnfGetValue_s(pFile,"BANDDOWNDEF",BANDDOWNDEF);
	    lt_dbput_rootvars(dbPtr,1,"BANDDOWNDEF",BANDDOWNDEF );	
			
			if(ltStrIsDigital(BANDDOWNDEF)==0){
				_ltPubInfo->banddowndef=atol(BANDDOWNDEF)*1000;
			}
			if(ltStrIsDigital(BANDUPDEF)==0){
				_ltPubInfo->bandupdef=atol(BANDUPDEF)*1000;
			}
							
	
	    lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandlink.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}




int ltbandlinkok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	char    *ENABLEBAND;
	char    *BANDUPLINK;
	char    *BANDDOWNLINK;
	char    *BANDUPDEF;
	char    *BANDDOWNDEF;
	char    caCmdCon[512];
	char    *pFile="/etc/gnm/qos/settings";

  if( atol(bcCnfGetValueS("/etc/gnm/conf","advance", "bandlimit"))==0 ){
	 	 ENABLEBAND="NO";
	}else{
		ENABLEBAND=ltMsgGetVar_s(ltMsgPk,"ENABLEBAND");
		if(ENABLEBAND==NULL){
			ENABLEBAND="NO";
		}
  }
	
	BANDUPLINK=ltMsgGetVar_s(ltMsgPk,"BANDUPLINK");
	BANDDOWNLINK=ltMsgGetVar_s(ltMsgPk,"BANDDOWNLINK");

	if(ltStrIsDigital(BANDDOWNLINK)==0){
				_ltPubInfo->banddownlink=atol(BANDDOWNLINK);
	}
	if(ltStrIsDigital(BANDUPLINK)==0){
				_ltPubInfo->banduplink=atol(BANDUPLINK);
	}
	BANDUPDEF=ltMsgGetVar_s(ltMsgPk,"BANDUPDEF");
	BANDDOWNDEF=ltMsgGetVar_s(ltMsgPk,"BANDDOWNDEF");

	if(ltStrIsDigital(BANDDOWNDEF)==0){
				_ltPubInfo->banddowndef=atol(BANDDOWNDEF)*1000;
	}
	if(ltStrIsDigital(BANDUPDEF)==0){
				_ltPubInfo->bandupdef=atol(BANDUPDEF)*1000;
	}
	
	if(_ltPubInfo->banddownlink<10 || _ltPubInfo->banduplink<10 ){
		ENABLEBAND="NO";
	}
	
	system("/bin/cp /etc/gnm/qos/settings /etc/gnm/qos/settings.old");
		/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLEBAND=%s' > %s",ENABLEBAND,pFile);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'BANDUPLINK=%s' >> %s",BANDUPLINK,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDDOWNLINK=%s' >> %s",BANDDOWNLINK,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDDOWNDEF=%s' >> %s",BANDDOWNDEF,pFile);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'BANDUPDEF=%s' >> %s",BANDUPDEF,pFile);
	system(caCmdCon);
	
//	_ltPubInfo->banduplink=atol(BANDUPLINK);
//	_ltPubInfo->banddownlink=atol(BANDDOWNLINK);
			    

		
	ltbandlink(confd,ltMsgPk,lt_MMHead);
	
  return 0;

}




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


	
	if(_ltPubInfo->banddownlink<1 ||_ltPubInfo->banduplink<1 ){
		  char    BANDUPLINK[24];
			char    BANDDOWNLINK[24];
		  memset(BANDDOWNLINK,0,sizeof(BANDDOWNLINK));
		 	bcCnfGetValue_s("/etc/gnm/qos/settings","BANDDOWNLINK",BANDDOWNLINK);
	    memset(BANDUPLINK,0,sizeof(BANDUPLINK));
			bcCnfGetValue_s("/etc/gnm/qos/settings","BANDUPLINK",BANDUPLINK);	
			if(strlen(BANDUPLINK)>0 && strlen(BANDDOWNLINK)>0 ){
				_ltPubInfo->banddownlink=atol(BANDDOWNLINK);
				_ltPubInfo->banduplink=atol(BANDUPLINK);
			}
		  if(_ltPubInfo->banddownlink<1 ||_ltPubInfo->banduplink<1 ){
					ltbandlink(confd,ltMsgPk,lt_MMHead);
					return 0;
		  }
	}
	qosid=0;
	if(ltMsgGetVar_s(ltMsgPk,"qosid")!=NULL){
		qosid=atol(ltMsgGetVar_s(ltMsgPk,"qosid"));
	}
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
  if(qosid!=0){
  	
		  sprintf(sqlBuf,"select sid,qosname,qosrate,qosceil,qossfq,prio,qosdefault,qostype,qoslinktype from nasqos where sid=%d",qosid);
			
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lt_dbput_rootvars(dbPtr,1,"sid",tempRow[0]);	
					lt_dbput_rootvars(dbPtr,1,"qosname",tempRow[1]);	
				  lt_dbput_rootvars(dbPtr,1,"qosrate",tempRow[2]);		
				  lt_dbput_rootvars(dbPtr,1,"qosceil",tempRow[3]);	
				  
				  lt_dbput_rootvars(dbPtr,1,"qostype",tempRow[7]);		
				  lt_dbput_rootvars(dbPtr,1,"sqf",tempRow[4]);	
				  
				  
				  
//				  myprio=atol(tempRow[5]);
//				  if(myprio<0 || myprio>8){
//				  	myprio=0;
//				  }
//				  sprintf(strprio,"selp%d",myprio);
//					lt_dbput_rootvars(dbPtr,1,strprio," selected " );	
//					myprio=atol(tempRow[7]);
//				  if(myprio<1 || myprio>2){
//				  	myprio=2;
//				  }
//				  sprintf(strprio,"check%d",myprio);
//				  lt_dbput_rootvars(dbPtr,1,strprio," checked " );	
//				  myprio=atol(tempRow[4]);
//				  if(myprio<1 || myprio>2){
//				  	myprio=1;
//				  }
//				  sprintf(strprio,"sel%d",myprio);
				  
				  if(atol(tempRow[8])==0){
				  	lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
				  }else if(atol(tempRow[8])==1){
				  	lt_dbput_rootvars(dbPtr,1,"linksel1"," selected " );	
				  }else if(atol(tempRow[8])==2){
				  	lt_dbput_rootvars(dbPtr,1,"linksel2"," selected " );	
				  }else if(atol(tempRow[8])==3){
				  	lt_dbput_rootvars(dbPtr,1,"linksel3"," selected " );	
				  }else{
				    lt_dbput_rootvars(dbPtr,1,"linksel0"," selected " );	
				  }
				  
				  lt_dbput_rootvars(dbPtr,1,strprio," selected " );	
				  
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

  sprintf(BANDDOWNLINK,"%lu",_ltPubInfo->banddownlink);
  lt_dbput_rootvars(dbPtr,1,"DOWNLINK",BANDDOWNLINK);
  
  sprintf(BANDDOWNLINK,"%lu",_ltPubInfo->banduplink);
  lt_dbput_rootvars(dbPtr,1,"UPLINK",BANDDOWNLINK);
  
  
  tablePtr=lt_dbput_table(dbPtr,"dqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosdefault from nasqos where sid>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			if(atol(tempRow[2])>0){
					sprintf(tmpDef,"<b>%s[def]</b>",tempRow[1]);
			}else{
			    sprintf(tmpDef,"%s",tempRow[1]);
		  }
			lt_dbput_recordvars(tablePtr,2,
				"dsid",LT_TYPE_STRING,tempRow[0],
				"dqosname",LT_TYPE_STRING,tmpDef);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	

  mybrate=0;
  tablePtr=lt_dbput_table(dbPtr,"bqoslist");
  sprintf(sqlBuf,"select sid,qosname,qostype,qosrate from nasqos where sid>0 and qostype>0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			mybrate=mybrate+atol(tempRow[2]);
			
			sprintf(myRate,"%d",(int)( (atol(tempRow[2])*100)/_ltPubInfo->banddownlink) );
			

							
			lt_dbput_recordvars(tablePtr,7,
				"bqosname",LT_TYPE_STRING,tempRow[1],
				"bqosrate",LT_TYPE_STRING,tempRow[2],
				"brate",LT_TYPE_STRING,myRate,
				"bqosceil",LT_TYPE_STRING,tempRow[3],
				"bprio",LT_TYPE_STRING,"",
				"btype",LT_TYPE_STRING,"保证",
				"bsqf",LT_TYPE_STRING,"");
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	if( _ltPubInfo->banddownlink>(mybrate+_ltPubInfo->banddowndef/1000)){
	   sprintf(myRate,"%lu", _ltPubInfo->banddownlink-mybrate );
	   lt_dbput_rootvars(dbPtr,1,"shareqos",myRate);	
	   sprintf(myRate,"%d", (int)(((_ltPubInfo->banddownlink-mybrate)*100)/_ltPubInfo->banddownlink)  );
	   lt_dbput_rootvars(dbPtr,1,"sharerate",myRate);	
  }else{
		 sprintf(myRate,"%lu", _ltPubInfo->banddowndef/1000 );
		 lt_dbput_rootvars(dbPtr,1,"shareqos",myRate);	
	   sprintf(myRate,"%d", (int)(((_ltPubInfo->banddowndef/1000)*100)/_ltPubInfo->banddownlink)  );
	   lt_dbput_rootvars(dbPtr,1,"sharerate",myRate);	
	}
	
	
	
  
 
  tablePtr=lt_dbput_table(dbPtr,"sqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrate,qosrate from nasqos where sid>0 and qostype=0");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_ltPubInfo->banddownlink) );
			

							
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
  sprintf(sqlBuf,"select sid,qosname,qossfq,qosceil from nasqos where sid>0 and qossfq>0");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			mybrate=mybrate+atol(tempRow[2]);
			
			sprintf(myRate,"%d",(int)( (atol(tempRow[2])*100)/_ltPubInfo->banduplink) );
			

							
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
	
	if( _ltPubInfo->banduplink >(mybrate+_ltPubInfo->bandupdef/1000)){
	   	sprintf(myRate,"%lu", _ltPubInfo->banduplink-mybrate );
	   	lt_dbput_rootvars(dbPtr,1,"upshareqos",myRate);	
			sprintf(myRate,"%d", (int)(((_ltPubInfo->banduplink-mybrate)*100)/_ltPubInfo->banduplink)  );
			lt_dbput_rootvars(dbPtr,1,"upsharerate",myRate);
  }else{
		 sprintf(myRate,"%lu", _ltPubInfo->bandupdef/1000 );
		 lt_dbput_rootvars(dbPtr,1,"upshareqos",myRate);	
		 sprintf(myRate,"%d", (int)(((_ltPubInfo->bandupdef/1000)*100)/_ltPubInfo->banduplink)  );
		 lt_dbput_rootvars(dbPtr,1,"upsharerate",myRate);
	}

		
	
	
  
 
  tablePtr=lt_dbput_table(dbPtr,"upsqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosceil,qosceil from nasqos where sid>0 and qossfq=0 ");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_ltPubInfo->banduplink) );
			

							
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
	
	
  
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandconf.html",dbPtr->head,0);

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
	
	qosrate=ltMsgGetVar_s(ltMsgPk,"qosrate");
	if(qosrate==NULL){
		qosrate="0";
	}
	
	prio=ltMsgGetVar_s(ltMsgPk,"prio");
	if(prio==NULL){
		prio="0";
	}
	
	qostype=ltMsgGetVar_s(ltMsgPk,"qostype");
	if(qostype==NULL){
		qostype="0";
	}
	
	qosceil=ltMsgGetVar_s(ltMsgPk,"qosceil");
	if(qosceil==NULL){
		qosceil="0";
	}
	
	sqf=ltMsgGetVar_s(ltMsgPk,"sqf");
	if(sqf==NULL){
		sqf="0";
	}
	
	qoslinktype=ltMsgGetVar_s(ltMsgPk,"qoslinktype");
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

		
		sprintf(sqlBuf,"update nasqos set qosname='%s',qosrate=%s, qosceil=%s, qostype=%s,qossfq=%s,prio=%s,qoslinktype=%s where sid=%d",
			qosname,qosrate,qosceil,qostype,sqf,prio,qoslinktype,qosid);

	  ltDbExecSql(G_DbCon,sqlBuf); 
		ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	  return 0;
	}
	qosid=1;
	while(1){
			sprintf(sqlBuf,"select count(*) from nasqos where sid=%d ",qosid);
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

	
	sprintf(sqlBuf,"insert into nasqos values (%ld,'%s',%s,%s,%s,%s,%s,0,%s)",
			qosid,qosname,qosrate,qosceil,qostype,sqf,prio,qoslinktype);
		
	//printf("sql;%s\n",sqlBuf);
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
	edittype=ltMsgGetVar_s(ltMsgPk,"edittype");
	
	if(qosid==0){
		   ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: this qos rule can't be chanage!</body></html>");		
				ltMsgFree(ltMsgPk);
			
		    return 0;
	}
	
	if( strcmp(edittype,"setdef")==0){
		qoslinktype=0;
		sprintf(sqlBuf,"select qoslinktype from nasqos where sid=%d",qosid);

		qoslinktype=0;
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							qoslinktype=atol(tempRow[0]);
						}
						ltDbCloseCursor(tempCursor);
			}
		
		sprintf(sqlBuf,"update  nasqos set qosdefault=0 where qoslinktype=%d ",qoslinktype);
		printf("sqlbuf:%s\n",sqlBuf);
	  ltDbExecSql(G_DbCon,sqlBuf); 
		sprintf(sqlBuf,"update  nasqos set qosdefault=1 where sid=%d and  qoslinktype=%d ",qosid,qoslinktype);
	  printf("sqlbuf:%s\n",sqlBuf);
	  ltDbExecSql(G_DbCon,sqlBuf); 
	  ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	  return 0;
	}else if( strcmp(edittype,"Del")==0){
		int lCount;
		lCount=0;
	
		
		sprintf(sqlBuf,"select count(*) from nasqosmatch where qosid=%d ",qosid);
		
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							lCount=atol(tempRow[0]);
						}
						ltDbCloseCursor(tempCursor);
				}
		if(lCount == 0) {
			sprintf(sqlBuf,"select count(*) from nasqosmatchup  where qosid=%d ",qosid);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							lCount=atol(tempRow[0]);
						}
						ltDbCloseCursor(tempCursor);
				}
		}
		if(lCount>0){
			 ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
			"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error: this qos rule being used!</body></html>");		
				ltMsgFree(ltMsgPk);
			
		    return 0;
			
		}
		sprintf(sqlBuf,"delete from nasqos where sid=%d ",qosid);
	  ltDbExecSql(G_DbCon,sqlBuf); 
		
		ltqosdownlink(confd,ltMsgPk,lt_MMHead);
	  return 0;
	}else if( strcmp(edittype,"APP")==0){
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head><meta http-equiv=\"refresh\" content=\"20;URL=/main/main.xp?Fun=ltnetarealist\" ></head><body bgcolor=\"#cccccc\" >Please try  20 seconds later.......</body></html>");		
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






int ltqosmatchlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	ltDbCursor  *tempCursor;
	LT_DBROW    tempRow;
	char 			  sqlBuf[1024];
	int         matchtype;
	char        *sid;
  char caHead[100000];
  char caTmpp[2048];
  char *pppp;
  int  lNum;
  memset(  caHead,0,sizeof(caHead)); 
	
  
   
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
  if(sid){
		  sprintf(sqlBuf,"delete from nasqosmatch where sid=%s ",	sid);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
			ltDbExecSql(G_DbCon,sqlBuf); 
  }

  if(ltMsgGetVar_s(ltMsgPk,"matchtype")==NULL){
  	  matchtype=0;
  }else{
  		matchtype=atol(ltMsgGetVar_s(ltMsgPk,"matchtype"));
  }
  if(matchtype>4){
  	matchtype=0;
  }
  lNum=0;
  pppp=caHead;
	pppp=strcat(pppp,"[");
	lNum=0;
	
  if(matchtype==2 ||matchtype==0 ){
	
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype from nasqosmatch a,nasqos b where a.matchtype=2  and a.qosid=b.sid ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
			  
			  lNum++;
			  if(lNum==1){
					  sprintf(caTmpp,"['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应用户");
						pppp=strcat(pppp,caTmpp);
				}else{
				    sprintf(caTmpp,",['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应用户");
						pppp=strcat(pppp,caTmpp);
			  }
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
	}
  
  if(matchtype==3 ){
  	int policyid;
  	policyid=atol(ltMsgGetVar_s(ltMsgPk,"pid"));
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype from nasqosmatch a,nasqos b,nasqosmatchpolicy c where a.matchtype=3  and a.qosid=b.sid and a.sid=c.sid and c.policy=%d ",policyid);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				
				lNum++;
			  if(lNum==1){
					  sprintf(caTmpp,"['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应策略组");
						pppp=strcat(pppp,caTmpp);
				}else{
				    sprintf(caTmpp,",['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应策略组");
						pppp=strcat(pppp,caTmpp);
			  }

				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
 }
  
  
  if(matchtype==0 ){
  

	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype from nasqosmatch a,nasqos b where a.matchtype=3  and a.qosid=b.sid  ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lNum++;
			  if(lNum==1){
					  sprintf(caTmpp,"['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应策略组");
						pppp=strcat(pppp,caTmpp);
				}else{
				    sprintf(caTmpp,",['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应策略组");
						pppp=strcat(pppp,caTmpp);
			  }
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
  }
  
  if(matchtype==4 ||matchtype==0 ){
	
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype from nasqosmatch a,nasqos b where a.matchtype=4  and a.qosid=b.sid ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
								
				lNum++;
			  if(lNum==1){
					  sprintf(caTmpp,"['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应部门");
						pppp=strcat(pppp,caTmpp);
				}else{
				    sprintf(caTmpp,",['%s','%s','%s','%s','%s','%s']",
					  tempRow[0],tempRow[1], tempRow[3], tempRow[2],tempRow[4],"对应部门");
						pppp=strcat(pppp,caTmpp);
			  }

				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
  }
  
  pppp=strcat(pppp,"]");
	
  ltMsgPk->msgpktype=3;  
  sprintf(caTmpp,"Content-Length: %d\nContent-type: text/html; charset=gb2312\n",strlen(caHead));
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,caTmpp,"lthtml",LT_TYPE_STRING,caHead);

	ltMsgFree(ltMsgPk);

	return 0;
	
}
  
  
int ltqosmatchlistup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	ltDbCursor  *tempCursor;
	LT_DBROW    tempRow;
	char 			  sqlBuf[1024];
	int         matchtype;
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
  if(ltMsgGetVar_s(ltMsgPk,"matchtype")==NULL){
  	  matchtype=0;
  }else{
  		matchtype=atol(ltMsgGetVar_s(ltMsgPk,"matchtype"));
  }
  

  if(matchtype==1 ||matchtype==0 ){
  
		  tablePtr=lt_dbput_table(dbPtr,"qosmatchl1");
		  sprintf(sqlBuf,"select a.sid,a.qossrvname, a.prio,b.qosname,a.sharetype from nasqosmatchup a,nasqos b where a.matchtype=1  and a.qosid=b.sid ");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
									
					lt_dbput_recordvars(tablePtr,6,
					  "qosm1sid",LT_TYPE_STRING,tempRow[0],
						"qosm1name",LT_TYPE_STRING,tempRow[1],
						"qosm1qos",LT_TYPE_STRING,tempRow[3],
						"qosm1prio",LT_TYPE_STRING,tempRow[2],
						"sharetype",LT_TYPE_STRING,tempRow[4],
						"qosm1dst",LT_TYPE_STRING,".....");
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
			}
	
  }
  
  if(matchtype==2 ||matchtype==0 ){
	  tablePtr=lt_dbput_table(dbPtr,"qosmatchl2");
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype from nasqosmatchup a,nasqos b where a.matchtype=2  and a.qosid=b.sid ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
								
				lt_dbput_recordvars(tablePtr,6,
				  "qosm2sid",LT_TYPE_STRING,tempRow[0],
					"qosm2name",LT_TYPE_STRING,tempRow[1],
					"qosm2qos",LT_TYPE_STRING,tempRow[3],
					"qosm2prio",LT_TYPE_STRING,tempRow[2],
					"share2type",LT_TYPE_STRING,tempRow[4],
					"qosm2dst",LT_TYPE_STRING,".....");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
	}
  
  if(matchtype==3 ){
  	int policyid;
  	policyid=atol(ltMsgGetVar_s(ltMsgPk,"pid"));
	  tablePtr=lt_dbput_table(dbPtr,"qosmatchl3");
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype  from nasqosmatchup a,nasqos b,nasqosmatchpolicy c where a.matchtype=3  and a.qosid=b.sid and a.sid=c.sid and c.policy=%d ",policyid);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
								
				lt_dbput_recordvars(tablePtr,6,
				  "qosm3sid",LT_TYPE_STRING,tempRow[0],
					"qosm3name",LT_TYPE_STRING,tempRow[1],
					"qosm3qos",LT_TYPE_STRING,tempRow[3],
					"qosm3prio",LT_TYPE_STRING,tempRow[2],
					"share3type",LT_TYPE_STRING,tempRow[4],
					"qosm3dst",LT_TYPE_STRING,".....");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
 }
  
  
  if(matchtype==0 ){
  
	  tablePtr=lt_dbput_table(dbPtr,"qosmatchl3");
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype  from nasqosmatchup a,nasqos b where a.matchtype=3  and a.qosid=b.sid  ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,6,
				  "qosm3sid",LT_TYPE_STRING,tempRow[0],
					"qosm3name",LT_TYPE_STRING,tempRow[1],
					"qosm3qos",LT_TYPE_STRING,tempRow[3],
					"qosm3prio",LT_TYPE_STRING,tempRow[2],
					"share3type",LT_TYPE_STRING,tempRow[4],
					"qosm3dst",LT_TYPE_STRING,".....");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
  }
  
  if(matchtype==4 ||matchtype==0 ){
	  tablePtr=lt_dbput_table(dbPtr,"qosmatchl4");
	  sprintf(sqlBuf,"select a.sid,a.qossrvname,a.prio,b.qosname,a.sharetype  from nasqosmatchup a,nasqos b where a.matchtype=4  and a.qosid=b.sid ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
								
				lt_dbput_recordvars(tablePtr,6,
				  "qosm4sid",LT_TYPE_STRING,tempRow[0],
					"qosm4name",LT_TYPE_STRING,tempRow[1],
					"qosm4qos",LT_TYPE_STRING,tempRow[3],
					"qosm4prio",LT_TYPE_STRING,tempRow[2],
					"share4type",LT_TYPE_STRING,tempRow[4],
					"qosm4dst",LT_TYPE_STRING,".....");
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
  }
  
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandsrvup.html",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
	
}
  
  
  

int ltqosmatchhead(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"rulelist");
	sprintf(sqlBuf,"select pid,pname from naspolicy ");
	
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			lt_dbput_recordvars(tablePtr,2,
				"pid",LT_TYPE_STRING,tempRow[0],
				"pname",LT_TYPE_STRING,tempRow[1]);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/nasqosmatchhead.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


  
int ltqosmatchheadup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"rulelist");
	sprintf(sqlBuf,"select pid,pname from naspolicy ");
	
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			lt_dbput_recordvars(tablePtr,2,
				"pid",LT_TYPE_STRING,tempRow[0],
				"pname",LT_TYPE_STRING,tempRow[1]);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/nasqosmatchheadup.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


  
int ltqosmatchsetup1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW   tempRow;
	char       *sid;
	
	sid=ltMsgGetVar_s(ltMsgPk,"sid");
	
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if(_ltPubInfo->daytimetype==0){
		lt_dbput_rootvars(dbPtr,1,"daytimetype"," 每天");
	}else if(_ltPubInfo->daytimetype==1){
		lt_dbput_rootvars(dbPtr,1,"daytimetype"," 每周");
	}else{
	  lt_dbput_rootvars(dbPtr,1,"daytimetype"," 每月");
  }
	
	if(sid==NULL){
		
		 	lt_dbput_rootvars(dbPtr,1,"sid","0" );	
		  lt_dbput_rootvars(dbPtr,1,"qossrvname","" );	
		  lt_dbput_rootvars(dbPtr,1,"selp0"," selected " );	
		  lt_dbput_rootvars(dbPtr,1,"mchech3"," checked " );	
		  lt_dbput_rootvars(dbPtr,1,"shsel1"," selected " );	
		  
		  lt_dbput_rootvars(dbPtr,1,"maxflow","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxtime","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxtcp","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxudp","0" );
				  
				  
			lt_dbput_rootvars(dbPtr,1,"maxpackage","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxBytes","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxudpp","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxtcpp","0" );
		  lt_dbput_rootvars(dbPtr,1,"maxupp","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxdownp","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxsp","0" );
				  lt_dbput_rootvars(dbPtr,1,"maxnc","0" );
				  
		  
		  tablePtr=lt_dbput_table(dbPtr,"qoslist");
		  sprintf(sqlBuf,"select sid,qosname from nasqos order by sid asc ");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
									
					lt_dbput_recordvars(tablePtr,3,
						"qosid",LT_TYPE_STRING,tempRow[0],
						"qosname",LT_TYPE_STRING,tempRow[1],
						"qossel",LT_TYPE_STRING,"");
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
			}
		  
		  
		  
	}else{
	    char strprio[24];
	    char caMoreFlag[32];
	    
			int qosid=0;
      memset(caMoreFlag,0,sizeof(caMoreFlag));
			sprintf(sqlBuf,"select sid,qossrvname,qosid,prio,matchtype,sharetype,maxflow,maxtime,maxtcp,maxudp,\
moreflag,maxpackage,maxBytes,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,onlybindip,macbindip,onlylogone from nasqosmatch where sid=%s ",sid);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					qosid=atol(tempRow[2]);	
					
					sprintf(caMoreFlag,"%s",tempRow[10]);
					
					sprintf(strprio,"selp%s",	tempRow[3]);	
					lt_dbput_rootvars(dbPtr,1,"sid",sid );	
				  lt_dbput_rootvars(dbPtr,1,"qossrvname",tempRow[1] );	
				  lt_dbput_rootvars(dbPtr,1,strprio," selected " );
				  sprintf(strprio,"mchech%s",	tempRow[4]);
				  lt_dbput_rootvars(dbPtr,1,strprio," checked " );	
				  
				  lt_dbput_rootvars(dbPtr,1,"maxflow",tempRow[6] );
				  lt_dbput_rootvars(dbPtr,1,"maxtime",tempRow[7] );	
				  lt_dbput_rootvars(dbPtr,1,"maxtcp",tempRow[8] );	
				  lt_dbput_rootvars(dbPtr,1,"maxudp",tempRow[9] );		
				  
				
				   lt_dbput_rootvars(dbPtr,1,"maxpackage",tempRow[11] );
				  lt_dbput_rootvars(dbPtr,1,"maxBytes",tempRow[12] );	
				  lt_dbput_rootvars(dbPtr,1,"maxudpp",tempRow[13] );	
				  lt_dbput_rootvars(dbPtr,1,"maxtcpp",tempRow[14] );		
				  
				   lt_dbput_rootvars(dbPtr,1,"maxupp",tempRow[15] );
				  lt_dbput_rootvars(dbPtr,1,"maxdownp",tempRow[16] );	
				  lt_dbput_rootvars(dbPtr,1,"maxsp",tempRow[17] );	
				  lt_dbput_rootvars(dbPtr,1,"maxnc",tempRow[18] );		
				  
				  lt_dbput_rootvars(dbPtr,1,"onlybindip",tempRow[19][0]=='1'?" checked ":"" );	
				  lt_dbput_rootvars(dbPtr,1,"macbindip",tempRow[20][0]=='1'?" checked ":"" );
				  lt_dbput_rootvars(dbPtr,1,"onlylogone",tempRow[21][0]=='1'?" checked ":"" );
				  
				  sprintf(strprio,"shsel%s",	tempRow[5]);
				  lt_dbput_rootvars(dbPtr,1,strprio," selected " );	
				  
				  
				  lt_dbput_rootvars(dbPtr,4,"noftpupchk",caMoreFlag[0]=='1'?" checked ":" ",
							"nopostupchk",caMoreFlag[1]=='1'?" checked ":" ",
							"nomsnuphk",caMoreFlag[2]=='1'?" checked ":" ",
							"noqqupchk",caMoreFlag[3]=='1'?" checked ":" ");
				  
				  
				}
				ltDbCloseCursor(tempCursor);
			}
	    
	    tablePtr=lt_dbput_table(dbPtr,"qoslist");
		  sprintf(sqlBuf,"select sid,qosname from nasqos  order by sid asc ");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
					if(	qosid==atol(	tempRow[0]) ){		
							lt_dbput_recordvars(tablePtr,3,
								"qosid",LT_TYPE_STRING,tempRow[0],
								"qosname",LT_TYPE_STRING,tempRow[1],
								"qossel",LT_TYPE_STRING," selected ");
					}else{
							lt_dbput_recordvars(tablePtr,3,
								"qosid",LT_TYPE_STRING,tempRow[0],
								"qosname",LT_TYPE_STRING,tempRow[1],
								"qossel",LT_TYPE_STRING,"");
				  }
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
			}
	
  }
	
  
	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup1.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltqosmatchsetup1up(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW   tempRow;
	char       *sid;
	
	sid=ltMsgGetVar_s(ltMsgPk,"sid");

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if(sid==NULL){
		
		 	lt_dbput_rootvars(dbPtr,1,"sid","0" );	
		  lt_dbput_rootvars(dbPtr,1,"qossrvname","" );	
		  lt_dbput_rootvars(dbPtr,1,"selp0"," selected " );	
		  lt_dbput_rootvars(dbPtr,1,"mchech3"," checked " );	
		  
		  
		  tablePtr=lt_dbput_table(dbPtr,"qoslist");
		  sprintf(sqlBuf,"select sid,qosname from nasqos where sid>0 and qoslinktype=2");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
									
					lt_dbput_recordvars(tablePtr,3,
						"qosid",LT_TYPE_STRING,tempRow[0],
						"qosname",LT_TYPE_STRING,tempRow[1],
						"qossel",LT_TYPE_STRING,"");
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
			}
		  
		  
		  
	}else{
	    char strprio[24];
			int qosid=0;

			
			sprintf(sqlBuf,"select sid,qossrvname,qosid,prio,matchtype from nasqosmatchup where sid=%s ",sid);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					qosid=atol(tempRow[2]);	
					
					sprintf(strprio,"selp%s",	tempRow[3]);	
					lt_dbput_rootvars(dbPtr,1,"sid",sid );	
				  lt_dbput_rootvars(dbPtr,1,"qossrvname",tempRow[1] );	
				  lt_dbput_rootvars(dbPtr,1,strprio," selected " );
				  sprintf(strprio,"mchech%s",	tempRow[4]);
				  lt_dbput_rootvars(dbPtr,1,strprio," checked " );	
				}
				ltDbCloseCursor(tempCursor);
			}
	    
	    tablePtr=lt_dbput_table(dbPtr,"qoslist");
		  sprintf(sqlBuf,"select sid,qosname from nasqos where sid>0 and qoslinktype=2");
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
					if(	qosid==atol(	tempRow[0]) ){		
							lt_dbput_recordvars(tablePtr,3,
								"qosid",LT_TYPE_STRING,tempRow[0],
								"qosname",LT_TYPE_STRING,tempRow[1],
								"qossel",LT_TYPE_STRING," selected ");
					}else{
							lt_dbput_recordvars(tablePtr,3,
								"qosid",LT_TYPE_STRING,tempRow[0],
								"qosname",LT_TYPE_STRING,tempRow[1],
								"qossel",LT_TYPE_STRING,"");
				  }
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
			}
	
  }
	
  
	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup1up.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltqosmatchsetup1ok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW   tempRow;
	int        sid,qosmatchtype,sharetype;
	char       *qossrvname;
	char       *prio;
	char       *qosid;
	char *dbUser;
	char *dbPass;
	char *dbName;
	
	char       *maxflow;
	char       *maxtime;
	char       *maxtcp;
	char       *maxudp;
	
	char       *maxpackage;
	char       *maxBytes;
	char       *maxudpp;
	char       *maxtcpp;
	char       *maxupp;
	char       *maxdownp;
	char       *maxsp;
	char       *maxnc;
	char       *onlybindip;
	char       *macbindip;
	char       *onlylogone;
	
	char caMoreFlag[32];
	
	ltDbConn   *tempDbCon;
	ltDbCursor *tempCursor1;
	LT_DBROW   tempRow1;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	
	memset(caMoreFlag,0,sizeof(caMoreFlag));
	sprintf(caMoreFlag,"%s","0000000000");
	
	if(ltMsgGetVar_s(ltMsgPk,"noftpup")){
		caMoreFlag[0]='1';
	}
	if(ltMsgGetVar_s(ltMsgPk,"nopostup")){
		caMoreFlag[1]='1';
	}
	if(ltMsgGetVar_s(ltMsgPk,"nomsnup")){
		caMoreFlag[2]='1';
	}
	if(ltMsgGetVar_s(ltMsgPk,"noqqup")){
		caMoreFlag[3]='1';
	}
  
  if( ltMsgGetVar_s(ltMsgPk,"sid")==NULL ){
  	sid=0;
  }else{
  	sid=atol(ltMsgGetVar_s(ltMsgPk,"sid"));
  }
	
  qossrvname=ltMsgGetVar_s(ltMsgPk,"qossrvname");
  prio=ltMsgGetVar_s(ltMsgPk,"prio");
  qosid=ltMsgGetVar_s(ltMsgPk,"qosid");
  qosmatchtype=atol(ltMsgGetVar_s(ltMsgPk,"qosmatchtype"));
  
  maxflow=ltMsgGetVar_s(ltMsgPk,"maxflow");
  maxtime=ltMsgGetVar_s(ltMsgPk,"maxtime");
  maxtcp=ltMsgGetVar_s(ltMsgPk,"maxtcp");
  maxudp=ltMsgGetVar_s(ltMsgPk,"maxudp");
  
  maxpackage=ltMsgGetVar_s(ltMsgPk,"maxpackage");
  maxBytes=ltMsgGetVar_s(ltMsgPk,"maxbytes");
  maxudpp=ltMsgGetVar_s(ltMsgPk,"maxudpp");
  maxtcpp=ltMsgGetVar_s(ltMsgPk,"maxtcpp");
  maxupp=ltMsgGetVar_s(ltMsgPk,"maxupp");   
  maxdownp=ltMsgGetVar_s(ltMsgPk,"maxdownp");  
  maxsp=ltMsgGetVar_s(ltMsgPk,"maxsp");   
  maxnc=ltMsgGetVar_s(ltMsgPk,"maxnc");    
  onlybindip=ltMsgGetVar_s(ltMsgPk,"onlybindip");
  macbindip=ltMsgGetVar_s(ltMsgPk,"macbindip"); 
  onlylogone=ltMsgGetVar_s(ltMsgPk,"onlylogonechk");

  if(!onlybindip){
  	onlybindip="0";
  }else{
    onlybindip="1";
  }
  if(!macbindip){
  	macbindip="0";
  }else{
    macbindip="1";
  }
  if(!onlylogone){
  	onlylogone="0";
  }else{
    onlylogone="1";
  }

  
  sharetype=atol(ltMsgGetVar_s(ltMsgPk,"sharetype"));
 
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if(sid==0){
		
		  /*add*/
		  long lCount;
		  char strid[12];
		  
		  lCount=0;
		  sprintf(sqlBuf,"select count(*) from nasqosmatch ");
		  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						lCount=atol(tempRow[0]);
					}
					ltDbCloseCursor(tempCursor);
			}
		  
			
			if(lCount == 0) {
				lCount=1;
			}else{
				sprintf(sqlBuf,"select max(sid)+1 from nasqosmatch ");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							lCount=atol(tempRow[0]);
						}
						ltDbCloseCursor(tempCursor);
				}
      }
		  sprintf(sqlBuf,"insert into  nasqosmatch (sid,qossrvname,qosid,prio,matchtype,sharetype,maxflow,\
maxtime,maxtcp,maxudp,moreflag, maxpackage,maxBytes,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,onlybindip,macbindip,onlylogone) values(%ld,'%s',%s,%s,%d,%d,%s,%s,%s,%s,'%s',%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",
		    lCount,qossrvname,qosid,prio,qosmatchtype,sharetype,maxflow,maxtime,maxtcp,maxudp ,caMoreFlag,
		    maxpackage,maxBytes,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,onlybindip,macbindip,onlylogone);
		  ltDbExecSql(G_DbCon,sqlBuf); 
		  
		  sprintf(strid,"%ld",lCount);
		 	lt_dbput_rootvars(dbPtr,1,"sid",strid );	
		  
		  if(qosmatchtype==1){/*ip*/
		  	sprintf(sqlBuf,"insert into nasqosmatchsrv values(%s,'','',0,0)",strid);
				ltDbExecSql(G_DbCon,sqlBuf); 
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup21.htm",dbPtr->head,0);
		  }else if(qosmatchtype==2){/*user*/
		    char strNum[12];
		  	int iNum=0;
		  	sprintf(sqlBuf,"select userid,dispname from nasuser order by dispname ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						lt_dbput_recordvars(tablePtr,4,
								"userid",LT_TYPE_STRING,tempRow[0],
								"username",LT_TYPE_STRING,tempRow[1],
								"num",LT_TYPE_LONG,iNum,
								"chk",LT_TYPE_STRING," ");
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				sprintf(strNum,"%d",iNum);
				lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup22.htm",dbPtr->head,0);
		  }else if(qosmatchtype==3){/*policy*/
		  	int iNum=0;
		  	sprintf(sqlBuf,"select pid,pname from naspolicy ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						  
						  iNum++;
							lt_dbput_recordvars(tablePtr,3,
							"pid",LT_TYPE_STRING,tempRow[0],
							"pname",LT_TYPE_STRING,tempRow[1],
							"chk",LT_TYPE_STRING," ");
	
						  tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup23.htm",dbPtr->head,0);
		  }else if(qosmatchtype==4){/*group*/
		  	char strNum[12];
		  	int iNum=0;
		  	sprintf(sqlBuf,"select groupid,groupname from nasgroup  order by groupid asc ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						lt_dbput_recordvars(tablePtr,4,
							"groupid",LT_TYPE_STRING,tempRow[0],
							"num",LT_TYPE_LONG,iNum,
							"groupname",LT_TYPE_STRING,tempRow[1],
							"chk",LT_TYPE_STRING," ");
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				sprintf(strNum,"%d",iNum);
		  	lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup24.htm",dbPtr->head,0);
		  }
     
	}else{
			/*update*/
			
		  char strid[12];
		  
		  
		  sprintf(sqlBuf,"update nasqosmatch set qossrvname='%s',qosid=%s,prio=%s,matchtype=%d,sharetype=%d,maxflow=%s,\
 maxtime=%s,maxtcp=%s,maxudp=%s,moreflag='%s',maxpackage=%s,maxBytes=%s,maxudpp=%s,maxtcpp=%s,maxupp=%s,maxdownp=%s,maxsp=%s,maxnc=%s,\
 onlybindip=%s,macbindip=%s,onlylogone=%s where sid=%d",
		  qossrvname,qosid,prio,qosmatchtype,sharetype,maxflow,maxtime,maxtcp,maxudp,caMoreFlag,
		 maxpackage,maxBytes,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,onlybindip,macbindip,onlylogone, 
		  sid	  );
		  //printf("sqlBuf:%s\n",sqlBuf);
		  ltDbExecSql(G_DbCon,sqlBuf); 
		  
		  sprintf(strid,"%d",sid);
		 	lt_dbput_rootvars(dbPtr,1,"sid",strid );	
		  
		  if(qosmatchtype==1){/*ip*/
		    sprintf(sqlBuf,"select dst,src,dport,sport from nasqosmatchsrv where sid=%d ",sid);
		    //printf("sqlBuf:%s\n",sqlBuf);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						lt_dbput_rootvars(dbPtr,1,"dst",tempRow[0] );	
					  lt_dbput_rootvars(dbPtr,1,"src",tempRow[1] );		
					  lt_dbput_rootvars(dbPtr,1,"dport",tempRow[2] );	
					  lt_dbput_rootvars(dbPtr,1,"sport",tempRow[3] );	
					  ltDbCloseCursor(tempCursor);
					}else{
						ltDbCloseCursor(tempCursor);
						sprintf(sqlBuf,"insert into nasqosmatchsrv values(%d,'','',0,0)",sid);
					  ltDbExecSql(G_DbCon,sqlBuf); 
				  }
					
				}
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup21.htm",dbPtr->head,0);
		  }else if(qosmatchtype==2){/*user*/
		    char strNum[12];
		  	int iNum=0;
		  	int lCount=0;
		  	
		  	tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
				if (tempDbCon==NULL){
					return -1;
				}
				
		  	sprintf(sqlBuf,"select userid,dispname from nasuser order by dispname ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						
						sprintf(sqlBuf,"select count(*) from nasqosmatchuser where sid=%d and userid=%s ",sid,tempRow[0]);
						lCount=0;
					  tempCursor1=ltDbOpenCursor(tempDbCon,sqlBuf);
					  if(tempCursor1!=NULL){
							tempRow1= ltDbFetchRow(tempCursor1);
							if(tempRow1!=NULL){
								lCount=atol(tempRow1[0]);
							}
							ltDbCloseCursor(tempCursor1);
						}
						if(lCount==0){
								lt_dbput_recordvars(tablePtr,4,
										"userid",LT_TYPE_STRING,tempRow[0],
										"username",LT_TYPE_STRING,tempRow[1],
										"num",LT_TYPE_LONG,iNum,
										"chk",LT_TYPE_STRING," ");
						}else{
								lt_dbput_recordvars(tablePtr,4,
										"userid",LT_TYPE_STRING,tempRow[0],
										"username",LT_TYPE_STRING,tempRow[1],
										"num",LT_TYPE_LONG,iNum,
										"chk",LT_TYPE_STRING," checked ");
					  }
					  
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				ltDbClose(tempDbCon);
				sprintf(strNum,"%d",iNum);
				lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup22.htm",dbPtr->head,0);
		  }else if(qosmatchtype==3){/*policy*/
		  	int iNum=0;
		  	int pid=0;
		  	
		  	sprintf(sqlBuf,"select policy from nasqosmatchpolicy where sid=%d ",sid);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						pid=atol(tempRow[0] );
					}
					ltDbCloseCursor(tempCursor);
				}
				
		  	sprintf(sqlBuf,"select pid,pname from naspolicy ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						  
						  iNum++;
						  if(pid==atol(tempRow[0])){
								lt_dbput_recordvars(tablePtr,3,
								"pid",LT_TYPE_STRING,tempRow[0],
								"pname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," checked ");
						  }else{
						  	lt_dbput_recordvars(tablePtr,3,
								"pid",LT_TYPE_STRING,tempRow[0],
								"pname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING,"  ");
						  }
						  tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup23.htm",dbPtr->head,0);
		  }else if(qosmatchtype==4){/*group*/
		  	char strNum[12];
		  	int iNum=0;
		  	int lCount=0;
		  	
		  	tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
				if (tempDbCon==NULL){
					return -1;
				}
				
		  	
		  	sprintf(sqlBuf,"select groupid,groupname from nasgroup  order by groupid asc ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						sprintf(sqlBuf,"select count(*) from nasqosmatchgroup where sid=%d and groupid=%s ",sid,tempRow[0]);
						lCount=0;
					  tempCursor1=ltDbOpenCursor(tempDbCon,sqlBuf);
					  if(tempCursor1!=NULL){
							tempRow1= ltDbFetchRow(tempCursor1);
							if(tempRow1!=NULL){
								lCount=atol(tempRow1[0]);
							}
							ltDbCloseCursor(tempCursor1);
						}
						if(lCount==0){
							lt_dbput_recordvars(tablePtr,4,
								"groupid",LT_TYPE_STRING,tempRow[0],
								"num",LT_TYPE_LONG,iNum,
								"groupname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," ");
						}else{
								lt_dbput_recordvars(tablePtr,4,
								"groupid",LT_TYPE_STRING,tempRow[0],
								"num",LT_TYPE_LONG,iNum,
								"groupname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," checked ");
					  }
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				ltDbClose(tempDbCon);
				sprintf(strNum,"%d",iNum);
		  	lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup24.htm",dbPtr->head,0);
		  }
	
  }
  	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ltqosmatchsetup1okup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW   tempRow;
	int        sid,qosmatchtype,sharetype;
	char       *qossrvname;
	char       *prio;
	char       *qosid;
	char *dbUser;
	char *dbPass;
	char *dbName;
	ltDbConn   *tempDbCon;
	ltDbCursor *tempCursor1;
	LT_DBROW   tempRow1;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
  
  if( strlen(ltMsgGetVar_s(ltMsgPk,"sid"))==0 ){
  	sid=100000;
  }else{
  	sid=atol(ltMsgGetVar_s(ltMsgPk,"sid"));
  }
	
  qossrvname=ltMsgGetVar_s(ltMsgPk,"qossrvname");
  prio=ltMsgGetVar_s(ltMsgPk,"prio");
  qosid=ltMsgGetVar_s(ltMsgPk,"qosid");
  qosmatchtype=atol(ltMsgGetVar_s(ltMsgPk,"qosmatchtype"));
  sharetype=atol(ltMsgGetVar_s(ltMsgPk,"sharetype"));
 
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	if(sid==0){
		
		  /*add*/
		  long lCount;
		  char strid[12];
		  sprintf(sqlBuf,"select count(*) from nasqosmatchup ");
		  
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
				lCount=10000;
			}else{
				sprintf(sqlBuf,"select max(sid)+1 from nasqosmatchup ");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow!=NULL){
								lCount=atol(tempRow[0]);
							}
							ltDbCloseCursor(tempCursor);
				}
			}	
		  sprintf(sqlBuf,"insert into  nasqosmatchup (sid,qossrvname,qosid,prio,matchtype,sharetype) values(%ld,'%s',%s,%s,%d,%d) ",lCount,qossrvname,qosid,prio,qosmatchtype,sharetype	  );
		  ltDbExecSql(G_DbCon,sqlBuf); 
		  
		  sprintf(strid,"%ld",lCount);
		 	lt_dbput_rootvars(dbPtr,1,"sid",strid );	
		  
		  if(qosmatchtype==1){/*ip*/
				sprintf(sqlBuf,"insert into nasqosmatchsrv values(%s,'','',0,0)",strid);
				ltDbExecSql(G_DbCon,sqlBuf); 
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup21up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==2){/*user*/
		    char strNum[12];
		  	int iNum=0;
		  	sprintf(sqlBuf,"select userid,dispname from nasuser order by dispname ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						lt_dbput_recordvars(tablePtr,4,
								"userid",LT_TYPE_STRING,tempRow[0],
								"username",LT_TYPE_STRING,tempRow[1],
								"num",LT_TYPE_LONG,iNum,
								"chk",LT_TYPE_STRING," ");
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				sprintf(strNum,"%d",iNum);
				lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup22up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==3){/*policy*/
		  	int iNum=0;
		  	sprintf(sqlBuf,"select pid,pname from naspolicy ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						  
						  iNum++;
							lt_dbput_recordvars(tablePtr,3,
							"pid",LT_TYPE_STRING,tempRow[0],
							"pname",LT_TYPE_STRING,tempRow[1],
							"chk",LT_TYPE_STRING," ");
	
						  tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup23up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==4){/*group*/
		  	char strNum[12];
		  	int iNum=0;
		  	sprintf(sqlBuf,"select groupid,groupname from nasgroup  order by groupid asc ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						lt_dbput_recordvars(tablePtr,4,
							"groupid",LT_TYPE_STRING,tempRow[0],
							"num",LT_TYPE_LONG,iNum,
							"groupname",LT_TYPE_STRING,tempRow[1],
							"chk",LT_TYPE_STRING," ");
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				sprintf(strNum,"%d",iNum);
		  	lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup24up.htm",dbPtr->head,0);
		  }

	}else{
			/*update*/
			
		  char strid[12];
		  sprintf(sqlBuf,"update nasqosmatchup set qossrvname='%s',qosid=%s,prio=%s,matchtype=%d,sharetype=%d where sid=%d",qossrvname,qosid,prio,qosmatchtype,sharetype,sid	  );
		  ltDbExecSql(G_DbCon,sqlBuf); 
		  
		  sprintf(strid,"%d",sid);
		 	lt_dbput_rootvars(dbPtr,1,"sid",strid );	
		  
		  if(qosmatchtype==1){/*ip*/
		    sprintf(sqlBuf,"select dst,src,dport,sport from nasqosmatchsrv where sid=%d ",sid);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						lt_dbput_rootvars(dbPtr,1,"dst",tempRow[0] );	
					  lt_dbput_rootvars(dbPtr,1,"src",tempRow[1] );		
					  lt_dbput_rootvars(dbPtr,1,"dport",tempRow[2] );	
					  lt_dbput_rootvars(dbPtr,1,"sport",tempRow[3] );	
					  ltDbCloseCursor(tempCursor);
					}else{
						ltDbCloseCursor(tempCursor);
						sprintf(sqlBuf,"insert into nasqosmatchsrv values(%d,'','',0,0)",sid);
					  ltDbExecSql(G_DbCon,sqlBuf); 
				  }
				}
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup21up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==2){/*user*/
		    char strNum[12];
		  	int iNum=0;
		  	int lCount=0;
		  	
		  	tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
				if (tempDbCon==NULL){
					return -1;
				}
				
		  	sprintf(sqlBuf,"select userid,dispname from nasuser order by dispname ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						
						sprintf(sqlBuf,"select count(*) from nasqosmatchuser where sid=%d and userid=%s ",sid,tempRow[0]);
						lCount=0;
					  tempCursor1=ltDbOpenCursor(tempDbCon,sqlBuf);
					  if(tempCursor1!=NULL){
							tempRow1= ltDbFetchRow(tempCursor1);
							if(tempRow1!=NULL){
								lCount=atol(tempRow1[0]);
							}
							ltDbCloseCursor(tempCursor1);
						}
						if(lCount==0){
								lt_dbput_recordvars(tablePtr,4,
										"userid",LT_TYPE_STRING,tempRow[0],
										"username",LT_TYPE_STRING,tempRow[1],
										"num",LT_TYPE_LONG,iNum,
										"chk",LT_TYPE_STRING," ");
						}else{
								lt_dbput_recordvars(tablePtr,4,
										"userid",LT_TYPE_STRING,tempRow[0],
										"username",LT_TYPE_STRING,tempRow[1],
										"num",LT_TYPE_LONG,iNum,
										"chk",LT_TYPE_STRING," checked ");
					  }
					  
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				ltDbClose(tempDbCon);
				sprintf(strNum,"%d",iNum);
				lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup22up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==3){/*policy*/
		  	int iNum=0;
		  	int pid=0;
		  	
		  	sprintf(sqlBuf,"select policy from nasqosmatchpolicy where sid=%d ",sid);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						pid=atol(tempRow[0] );
					}
					ltDbCloseCursor(tempCursor);
				}
				
		  	sprintf(sqlBuf,"select pid,pname from naspolicy ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						  
						  iNum++;
						  if(pid==atol(tempRow[0])){
								lt_dbput_recordvars(tablePtr,3,
								"pid",LT_TYPE_STRING,tempRow[0],
								"pname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," ");
						  }else{
						  	lt_dbput_recordvars(tablePtr,3,
								"pid",LT_TYPE_STRING,tempRow[0],
								"pname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," checked ");
						  }
						  tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
		  	
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup23up.htm",dbPtr->head,0);
		  }else if(qosmatchtype==4){/*group*/
		  	char strNum[12];
		  	int iNum=0;
		  	int lCount=0;
		  	
		  	tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
				if (tempDbCon==NULL){
					return -1;
				}
				
		  	
		  	sprintf(sqlBuf,"select groupid,groupname from nasgroup  order by groupid asc ");
				tablePtr=lt_dbput_table(dbPtr,"list");
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						iNum++;
						sprintf(sqlBuf,"select count(*) from nasqosmatchgroup where sid=%d and groupid=%s ",sid,tempRow[0]);
						lCount=0;
					  tempCursor1=ltDbOpenCursor(tempDbCon,sqlBuf);
					  if(tempCursor1!=NULL){
							tempRow1= ltDbFetchRow(tempCursor1);
							if(tempRow1!=NULL){
								lCount=atol(tempRow1[0]);
							}
							ltDbCloseCursor(tempCursor1);
						}
						if(lCount==0){
							lt_dbput_recordvars(tablePtr,4,
								"groupid",LT_TYPE_STRING,tempRow[0],
								"num",LT_TYPE_LONG,iNum,
								"groupname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," ");
						}else{
								lt_dbput_recordvars(tablePtr,4,
								"groupid",LT_TYPE_STRING,tempRow[0],
								"num",LT_TYPE_LONG,iNum,
								"groupname",LT_TYPE_STRING,tempRow[1],
								"chk",LT_TYPE_STRING," checked ");
					  }
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
				}
				ltDbClose(tempDbCon);
				sprintf(strNum,"%d",iNum);
		  	lt_dbput_rootvar(dbPtr,"allnum",strNum);
		  	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandmatchsetup24up.htm",dbPtr->head,0);
		  }
	
  }
  	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int ltqosmatchsetup21ok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];

	char       *dst;
	char       *src;
	char       *dstport;
	char       *srcport;
	

	char *sid;

  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	 
	 
  dst=ltMsgGetVar_s(ltMsgPk,"dst");
  src=ltMsgGetVar_s(ltMsgPk,"src");
  dstport=ltMsgGetVar_s(ltMsgPk,"dstport");
  srcport=ltMsgGetVar_s(ltMsgPk,"srcport");
	
	
	sprintf(sqlBuf,"insert into  nasqosmatchsrv (sid,dst,src,dport,sport) values(%s,'%s','%s','%s','%s') ",
		sid,dst,src,dstport,srcport	  );
	ltDbExecSql(G_DbCon,sqlBuf); 
		  
  ltqosmatchlist(confd,ltMsgPk,lt_MMHead);

	return 0;

}


int ltqosmatchsetup22ok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
  int  allNum,iNum;
  char strNum[32];

	char *sid;

  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	

	allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	for(iNum=0;iNum<allNum;iNum++){
				sprintf(strNum,"index%d",iNum+1);
				if(ltMsgGetVar_s(ltMsgPk,strNum)==NULL){
					continue;
				}
				sprintf(sqlBuf,"insert into nasqosmatchuser values(%s,%s)",
					sid,ltMsgGetVar_s(ltMsgPk,strNum));
				ltDbExecSql(G_DbCon,sqlBuf); 
	}	

				
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		

	   ltMsgFree(ltMsgPk);
	   return 0;

	return 0;

}



int ltqosmatchsetup23ok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];

 

  char *sid;
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 


	sprintf(sqlBuf,"insert into nasqosmatchpolicy values(%s,%s)",
					sid,	ltMsgGetVar_s(ltMsgPk,"policy"));
	ltDbExecSql(G_DbCon,sqlBuf); 
	  
  			
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		

	   ltMsgFree(ltMsgPk);
	   return 0;


}

int ltqosmatchsetup24ok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
  int  allNum,iNum;
  char strNum[32];
  char *sid;
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 


	allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	for(iNum=0;iNum<allNum;iNum++){
				sprintf(strNum,"index%d",iNum+1);
				if(ltMsgGetVar_s(ltMsgPk,strNum)==NULL){
					continue;
				}else if( strlen(ltMsgGetVar_s(ltMsgPk,strNum))==0){
					continue;
				}
				sprintf(sqlBuf,"insert into nasqosmatchgroup values(%s,%s)",
					sid,
					ltMsgGetVar_s(ltMsgPk,strNum));
				//printf("%s\n",sqlBuf);
				ltDbExecSql(G_DbCon,sqlBuf); 
	}	

		  
			
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		

	   ltMsgFree(ltMsgPk);
	   return 0;

}




int ltqosmatchsetup21okup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];

	char       *dst;
	char       *src;
	char       *dstport;
	char       *srcport;
	

	char *sid;

  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	 
	 
  dst=ltMsgGetVar_s(ltMsgPk,"dst");
  src=ltMsgGetVar_s(ltMsgPk,"src");
  dstport=ltMsgGetVar_s(ltMsgPk,"dstport");
  srcport=ltMsgGetVar_s(ltMsgPk,"srcport");
	
	
	sprintf(sqlBuf,"insert into  nasqosmatchsrv (sid,dst,src,dport,sport) values(%s,'%s','%s','%s','%s') ",
		sid,dst,src,dstport,srcport	  );
	ltDbExecSql(G_DbCon,sqlBuf); 
		  
  ltqosmatchlistup(confd,ltMsgPk,lt_MMHead);

	return 0;

}


int ltqosmatchsetup22okup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
  int  allNum,iNum;
  char strNum[32];

	char *sid;

  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	

	allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	for(iNum=0;iNum<allNum;iNum++){
				sprintf(strNum,"index%d",iNum+1);
				if(ltMsgGetVar_s(ltMsgPk,strNum)==NULL){
					continue;
				}else if( strlen(ltMsgGetVar_s(ltMsgPk,strNum))==0){
					continue;
				}
				sprintf(sqlBuf,"insert into nasqosmatchuser values(%s,%s)",
					sid,
					ltMsgGetVar_s(ltMsgPk,strNum));
				ltDbExecSql(G_DbCon,sqlBuf); 
	}	

		  
  ltqosmatchlistup(confd,ltMsgPk,lt_MMHead);

	return 0;

}



int ltqosmatchsetup23okup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];

 

  char *sid;
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 


	sprintf(sqlBuf,"insert into nasqosmatchpolicy values(%s,%s)",
					sid,	ltMsgGetVar_s(ltMsgPk,"policy"));
	ltDbExecSql(G_DbCon,sqlBuf); 
	  
  ltqosmatchlistup(confd,ltMsgPk,lt_MMHead);

	return 0;

}

int ltqosmatchsetup24okup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
  int  allNum,iNum;
  char strNum[32];
  char *sid;
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 


	allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	for(iNum=0;iNum<allNum;iNum++){
				sprintf(strNum,"index%d",iNum+1);
				if(ltMsgGetVar_s(ltMsgPk,strNum)==NULL){
					continue;
				}else if( strlen(ltMsgGetVar_s(ltMsgPk,strNum))==0){
					continue;
				}
				sprintf(sqlBuf,"insert into nasqosmatchuser values(%s,%s)",
					sid,
					ltMsgGetVar_s(ltMsgPk,strNum));
				//printf("%s\n",sqlBuf);
				ltDbExecSql(G_DbCon,sqlBuf); 
	}	

		  
  ltqosmatchlistup(confd,ltMsgPk,lt_MMHead);

	return 0;

}



int ltqosmatchdelup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
  char  sqlBuf[1024];
  char *sid;
  
  sid=ltMsgGetVar_s(ltMsgPk,"sid");
  
  sprintf(sqlBuf,"delete from nasqosmatchup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchuser where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchgroup where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchsrv where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 
	sprintf(sqlBuf,"delete from nasqosmatchpolicy where sid=%s ",	sid);
	ltDbExecSql(G_DbCon,sqlBuf); 

		  
  ltqosmatchlistup(confd,ltMsgPk,lt_MMHead);

	return 0;

}



int lttimesetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	
	char ENABLENTP[24];
	char ntpadd1[24];
	char ntpadd2[24];
	
	struct tm * sTim;
  char   pFormat[24];
  long   lTemp;
  long   lTime;
   
	
	char    *pFile="/etc/gnm/time/settings";
	

	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	lTime=time(0);
	memset(ENABLENTP,0,sizeof(ENABLENTP));
	bcCnfGetValue_s(pFile,"ENABLENTP",ENABLENTP);
	if( strcmp(ENABLENTP,"YES")==0){
			lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL","checked" );	
	}else{
			lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL"," " );	
	}
	
	
	memset(ntpadd1,0,sizeof(ntpadd1));
	bcCnfGetValue_s(pFile,"NTP_ADDR_1",ntpadd1);
	lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_1",ntpadd1 );

	memset(ntpadd2,0,sizeof(ntpadd2));
	bcCnfGetValue_s(pFile,"NTP_ADDR_2",ntpadd2);
	lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_2",ntpadd2);
	
	sTim = localtime(&lTime);
  lTemp = sTim->tm_year;
  if(lTemp > 100) lTemp = lTemp - 100;
  if(lTemp<10){
    	sprintf(pFormat,"200%ld",lTemp);
  }else{
		sprintf(pFormat,"20%ld",lTemp);
  }
  lt_dbput_rootvars(dbPtr,1,"year",pFormat);
  
  lTemp = sTim->tm_mon + 1;
  if(lTemp<10){
    	sprintf(pFormat,"0%ld",lTemp);
  }else{
	  sprintf(pFormat,"%ld",lTemp);
  }
	lt_dbput_rootvars(dbPtr,1,"month",pFormat);
	
	lTemp = sTim->tm_mday;
  if(lTemp<10){
    	sprintf(pFormat,"0%ld",lTemp);
  }else{
		sprintf(pFormat,"%ld",lTemp);
  }
	lt_dbput_rootvars(dbPtr,1,"day",pFormat);
	
	
	 lTemp = sTim->tm_hour;
   if(lTemp<10){
			sprintf(pFormat,"0%ld",lTemp);  
   }else{
			sprintf(pFormat,"%ld",lTemp);  
   }
	lt_dbput_rootvars(dbPtr,1,"day",pFormat);
	
	lTemp = sTim->tm_hour;
   if(lTemp<10){
			sprintf(pFormat,"0%ld",lTemp);  
   }else{
			sprintf(pFormat,"%ld",lTemp);  
   }
	lt_dbput_rootvars(dbPtr,1,"hour",pFormat);
	
	lTemp = sTim->tm_min;
   if(lTemp<10){
			sprintf(pFormat,"0%ld",lTemp);  
   }else{
			sprintf(pFormat,"%ld",lTemp);  
   }
	lt_dbput_rootvars(dbPtr,1,"minute",pFormat);
	
	lTemp = sTim->tm_sec;
   if(lTemp<10){
			sprintf(pFormat,"0%ld",lTemp);  
   }else{
			sprintf(pFormat,"%ld",lTemp);  
   }
	lt_dbput_rootvars(dbPtr,1,"second",pFormat);
	
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/timeface.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int lttimeok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	char    *pFile="/etc/gnm/time/settings";
	char    *ntpadd1;
	char    *ntpadd2;
	char    *ENABLENTP;
  char    caCmdCon[256];

	ntpadd1=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_1");
	ntpadd2=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_2");
	ENABLENTP=ltMsgGetVar_s(ltMsgPk,"ENABLENTP");
  if(ENABLENTP==NULL){
			ENABLENTP="NO";
	}
 	
	system("/bin/cp /etc/gnm/time/settings /etc/gnm/time/settings.old");
	/*连接方式*/
	
	sprintf(caCmdCon,"echo 'ENABLENTP=%s' > %s",ENABLENTP,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	sprintf(caCmdCon,"echo 'NTP_ADDR_1=%s' >> %s",ntpadd1,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
	
	sprintf(caCmdCon,"echo 'NTP_ADDR_2=%s' >> %s",ntpadd2,pFile);
	//printf("caCmdCon:%s\n",caCmdCon);
	system(caCmdCon);
		
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
		"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success: </body></html>");		
	ltMsgFree(ltMsgPk);

	return 0;

}

int lttimeset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	


	char    *year;
	char    *month;
	char    *day;
	char    *hour;
	char    *minute;
	char    *second;
	char    caCmdCon[256];



	year=ltMsgGetVar_s(ltMsgPk,"year");
	month=ltMsgGetVar_s(ltMsgPk,"month");
	day=ltMsgGetVar_s(ltMsgPk,"day");
  hour=ltMsgGetVar_s(ltMsgPk,"hour");
	minute=ltMsgGetVar_s(ltMsgPk,"minute");
	second=ltMsgGetVar_s(ltMsgPk,"second");
 	
	
	
	sprintf(caCmdCon,"/bin/date -s '%s-%s-%s'",year,month,day);
	system(caCmdCon);
		
	sprintf(caCmdCon,"/bin/date -s '%s:%s:%s'",hour,minute,second);
	system(caCmdCon);
	
  lttimesetting(confd,ltMsgPk,lt_MMHead);
  
	return 0;

}





int ltadvarconfig(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;

	char strTmp[128];

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
  sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldaphost"));
	lt_dbput_rootvars(dbPtr,1,"ldaphost",strTmp);	

	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldapadmin"));
	lt_dbput_rootvars(dbPtr,1,"ldapadmin",strTmp);	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldappass"));
	lt_dbput_rootvars(dbPtr,1,"ldappass",strTmp);	
	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "depfield"));
	lt_dbput_rootvars(dbPtr,1,"depfield",strTmp);	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "policyfield"));
	lt_dbput_rootvars(dbPtr,1,"policyfield",strTmp);	


	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ntauth", "domain"));
	lt_dbput_rootvars(dbPtr,1,"domain",strTmp);	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldapbasedn"));
	lt_dbput_rootvars(dbPtr,1,"ldapbasedn",strTmp);
	
	if(_ltPubInfo->updatead==1){
		lt_dbput_rootvars(dbPtr,1,"updatechk"," checked ");	
	}

	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/advarconfig.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int loopLdap(stralloc *strTemp,LDAP	*ld,char *baseDN){
	
	  int   rc = LDAP_SUCCESS;
    LDAPMessage *res = NULL, *e = NULL;
    char		*srchattrs[] = { "objectClass", NULL };
	  char		**attrs = srchattrs;
	  char    *a,*dn;
	  char    pOut[512];
	  char    *ppp;
	  char    *ppp1;
	  char    tmpstr[512]; 
	  int     iHead; 
	  char    dispname[256]; 
	  int     isPerson;
	  void    *ptr;
    char    **vals;
    int     i;
			
	  iHead=1;

	  rc = ldap_search_ext_s( ld,baseDN, LDAP_SCOPE_ONELEVEL ,"(|(objectclass=organizationalUnit)(objectclass=organizationalPerson))", attrs, 0, NULL, NULL, NULL, LDAP_NO_LIMIT, &res );
		if ( rc== LDAP_SUCCESS) {
			if ( ldap_count_entries( ld, res ) == 0 ) {
				sprintf(tmpstr,"%s",",leaf: true");
				stralloc_cats(strTemp,tmpstr);
			}else{
			  sprintf(tmpstr,"%s",",children: [");
			  stralloc_cats(strTemp,tmpstr);
				for ( e = ldap_first_entry( ld, res ); e != NULL;e = ldap_next_entry( ld, e ) ) {
	            dn = ldap_get_dn( ld, e );
		           memset(pOut,0,512);
		           memset(dispname,0,sizeof(dispname));
		           ppp1=pOut;
		           if(strutf82gb(dn,pOut,strlen(dn))==0){
								  		ppp=strstr(pOut,",");
								  		memcpy(dispname,ppp1+3,(ppp-ppp1)-3);
							 }else{
							 		continue;
							 }   
							isPerson=0; 
							for ( a = ldap_first_attribute( ld, e, &ptr );a != NULL;a = ldap_next_attribute( ld, e, ptr ) ) {
									/* print each value */
									if(strcmp(a,"objectClass")==0){
		                vals = ldap_get_values( ld, e, a );
		                for ( i = 0; vals[i] != NULL; i++ ) {
		                    if(strcmp(vals[i],"organizationalPerson")==0){
		                    	isPerson=1;
		                    	break;
		                    }
		                }
		                ldap_value_free( vals );
		              }
	           }
	           if(isPerson==0){     
							if(iHead==1){
								sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
								iHead=0;
							}else{
							  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
						  }
						 }else{
						 	if(iHead==1){
								sprintf(tmpstr,"{id: '%s',qtip: '%s',leaf: true,text: '%s',icon: '/images/user.gif'",ppp1,ppp1,dispname);
								iHead=0;
							}else{
							  sprintf(tmpstr,",{id: '%s',qtip: '%s',leaf: true,text: '%s',icon: '/images/user.gif'",ppp1,ppp1,dispname);
						  }
						 }
						  stralloc_cats(strTemp,tmpstr);
						  if(isPerson==0){
						  	loopLdap(strTemp,ld,dn);	
						  }	
						  free( dn ); 
						  stralloc_cats(strTemp,"}");	
	      }
	      stralloc_cats(strTemp,"]");
	    }
	    ldap_msgfree( res );
     /* close and free connection resources */
    }
//	  rc = ldap_search_ext_s( ld,baseDN, LDAP_SCOPE_ONELEVEL ,"(objectclass=organizationalPerson)", attrs, 0, NULL, NULL, NULL, LDAP_NO_LIMIT, &res );
//		if ( rc== LDAP_SUCCESS) {
//			if ( ldap_count_entries( ld, res ) == 0 ) {
//				sprintf(tmpstr,"%s",",leaf: true");
//				stralloc_cats(strTemp,tmpstr);
//			}else{
//			  sprintf(tmpstr,"%s",",children: [");
//			  stralloc_cats(strTemp,tmpstr);
//			  iHead=1;
//				for ( e = ldap_first_entry( ld, res ); e != NULL;e = ldap_next_entry( ld, e ) ) {
//	            dn = ldap_get_dn( ld, e );
//		           memset(pOut,0,512);
//		           memset(dispname,0,sizeof(dispname));
//		           ppp1=pOut;
//		           if(strutf82gb(dn,pOut,strlen(dn))==0){
//								  		ppp=strstr(pOut,",");
//								  		memcpy(dispname,ppp1+3,(ppp-ppp1)-3);
//							 }else{
//							 		continue;
//							 }         
//							if(iHead==1){
//								sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
//								iHead=0;
//							}else{
//							  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
//						  }
//						  stralloc_cats(strTemp,tmpstr);	
//						  free( dn ); 
//						  stralloc_cats(strTemp,"}");	
//	      }
//	      stralloc_cats(strTemp,"]");
//	    }	
//    /* free the search results */
//    ldap_msgfree( res );
//    /* close and free connection resources */
//  }
  return 0;
}


int ltadvarconfigok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
			 char  domain1[256];
			 char  domain[256];
			 char  depfield[64];
			 char  policyfield[64];
			 char *ppp;
			 char *ppp1;

			 char  worksta[64];
			 char  inituser[64];
			 char  ldapadmin[256];
	     //char  caPerl[8092];
	     //int   destfd;
	     char baseDN[256];

				     memset(policyfield,0,sizeof(policyfield));
						 memset(depfield,0,sizeof(depfield));
						 memset(domain1,0,sizeof(domain1));
						 memset(domain,0,sizeof(domain));
						 memset(worksta,0,sizeof(worksta));
						 memset(inituser,0,sizeof(inituser));
						 memset(ldapadmin,0,sizeof(ldapadmin));
			       
			       
			       //xaction=atol(ltMsgGetVar_s(ltMsgPk,"action"));
			       snprintf(depfield,63,"%s",ltMsgGetVar_s(ltMsgPk,"depfield"));
			       snprintf(policyfield,63,"%s",ltMsgGetVar_s(ltMsgPk,"policyfield"));
			       snprintf(domain1,250,"%s",ltMsgGetVar_s(ltMsgPk,"domain"));
			       snprintf(ldapadmin,250,"%s",ltMsgGetVar_s(ltMsgPk,"ldapadmin"));
			       sprintf(baseDN,"%s",ltMsgGetVar_s(ltMsgPk,"ldapbasedn"));
			       ppp=strstr(baseDN,"dc=");
			       while(ppp){
			       		memcpy(ppp,"DC=",3);	
			       		ppp=strstr(ppp+4,"dc=");
			       }
			       confReplace("/etc/gnm/conf/ldapauth","ldapbasedn",baseDN);/*kdc-admin*/
						 				
					   confReplace("/etc/gnm/conf/ldapauth","ldaphost",ltMsgGetVar_s(ltMsgPk,"ldaphost")); /*hostname*/
						 confReplace("/etc/gnm/conf/ldapauth","ldapport","389"); /*kdc-ip*/
						 confReplace("/etc/gnm/conf/ntauth","domain",domain1);
						 
						 sprintf(_ltPubInfo->domain,"%s",domain1);
						 
						 if(ltMsgGetVar_s(ltMsgPk,"updatead")){
							 confReplaceI("/etc/gnm/conf/option","updatead",atol(ltMsgGetVar_s(ltMsgPk,"updatead")));
							 _ltPubInfo->updatead=atol(ltMsgGetVar_s(ltMsgPk,"updatead")); 
						 }else{
						   confReplaceI("/etc/gnm/conf/option","updatead",0);
							 _ltPubInfo->updatead=0; 
						 } 
						
						confReplace("/etc/gnm/conf/ldapauth","ldapadmin",ldapadmin);/*kdc-password*/
						
						ppp1=ldapadmin;
						ppp=strchr(ldapadmin,',');
						if(ppp){
								*ppp='\0';
								sprintf(inituser,"%s",ppp1+3);
						}
							
							//ltStrToLower(domain1);
							ppp1=domain1;
							ppp=strchr(ppp1,'.');
						  if(ppp){
										  	*ppp='\0';
//										  	sprintf(domain,"DC=%s",ppp1);
										  	snprintf(worksta,61,"%s",ppp1);
//										  	ppp1=ppp+1;
//										  
//											  ppp=strchr(ppp1,'.');
//											  while(ppp){
//											  	*ppp='\0';
//											  	sprintf(domain,"%s,DC=%s",domain,ppp1);
//											  	ppp1=ppp+1;
//											  	ppp=strchr(ppp1,'.');
//											  }
						 }
//						 else{
//										    i=0;
//						 }
//						 if(i==1){
//										  	sprintf(domain,"%s,DC=%s",domain,ppp1);
//						  }else{
//										    sprintf(domain,"DC=%s",ppp1);
//						 }
						  
					 snprintf(domain1,250,"%s",ltMsgGetVar_s(ltMsgPk,"domain")); 	  
			     ltStrToUpper(domain1);
					
					confReplace("/etc/gnm/conf/ldapauth","ldappass",ltMsgGetVar_s(ltMsgPk,"ldappass")); 
					confReplace("/etc/gnm/conf/ldapauth","depfield",depfield);
					confReplace("/etc/gnm/conf/ldapauth","policyfield",policyfield);
				  confReplace("/etc/gnm/conf/ldapauth","accountfield","sAMAccountName");
			    confReplace("/etc/gnm/conf/ldapauth","cnfield","sAMAccountName"); 
						  
					/*修改/etc/krb5.conf*/			  
					{
						  	char caTmpCmd[2056];
						   
						    
								
			
								sprintf(caTmpCmd,"echo '[logging]\ndefault = FILE:/var/log/krb5libs.log\nkdc = FILE:/var/log/krb5kdc.log\nadmin_server = FILE:/var/log/kadmind.log\n\n' >/etc/krb5.conf");
								system(caTmpCmd);
								
								sprintf(caTmpCmd,"echo '[libdefaults]\ndefault_realm = %s\n dns_lookup_realm = false\n dns_lookup_kdc = false\n\n' >>/etc/krb5.conf",domain1);
								system(caTmpCmd); 
								 
								sprintf(caTmpCmd,"echo '[realms]\n %s = {\n  kdc = %s:88\n  admin_server = %s:749\n  default_domain = %s\n }\n\n' >>/etc/krb5.conf",domain1,ltMsgGetVar_s(ltMsgPk,"ldaphost"),ltMsgGetVar_s(ltMsgPk,"ldaphost"),domain1);
								system(caTmpCmd);  
								
								sprintf(caTmpCmd,"echo '[domain_realm]\n .example.com = %s\n example.com = %s\n\n' >>/etc/krb5.conf",domain1,domain1);
								system(caTmpCmd);  
								
								sprintf(caTmpCmd,"echo '[kdc]\n profile = /var/kerberos/krb5kdc/kdc.conf\n\n' >>/etc/krb5.conf");
								system(caTmpCmd); 
								
								sprintf(caTmpCmd,"echo '[appdefaults]\n pam = {\n   debug = false\n   ticket_lifetime = 36000\n   renew_lifetime = 36000\n   forwardable = true\n   krb4_convert = false }\n\n' >>/etc/krb5.conf");
								system(caTmpCmd); 
								
								/*修改/app/samba/lib/smb.conf*/						
								sprintf(caTmpCmd,"echo '[global]\nworkgroup = %s\nrealm = %s\nsecurity = ADS\n' >/app/samba/lib/smb.conf",worksta,domain1);
								system(caTmpCmd);
								sprintf(caTmpCmd,"echo 'password server = %s\nencrypt passwords = yes\nwins server = %s\n' >>/app/samba/lib/smb.conf",ltMsgGetVar_s(ltMsgPk,"ldaphost"),ltMsgGetVar_s(ltMsgPk,"ldaphost"));
								system(caTmpCmd);
								sprintf(caTmpCmd,"echo 'idmap uid = 10000-20000\nidmap gid = 10000-20000\nwinbind enum users = yes\n' >>/app/samba/lib/smb.conf");
								system(caTmpCmd);
								sprintf(caTmpCmd,"echo 'winbind enum groups = yes\ntemplate shell = /bin/bash\ntemplate homedir = /home/\%cD/\%cU\n' >>/app/samba/lib/smb.conf",'%','%');
								system(caTmpCmd);
								sprintf(caTmpCmd,"echo 'winbind use default domain = yes\n' >>/app/samba/lib/smb.conf");
								system(caTmpCmd);
			
								/*修改/etc/gnm/start/samba.up*/		
			
			          sprintf(caTmpCmd,"echo \"/bin/echo '%s'| /usr/kerberos/bin/kinit '%s@%s' 2>&1 \" > /etc/gnm/start/samba.up ",ltMsgGetVar_s(ltMsgPk,"ldappass"),inituser,domain1);
								system(caTmpCmd);	
								sprintf(caTmpCmd,"/bin/echo '%s'| /usr/kerberos/bin/kinit '%s@%s' 2>&1 ",ltMsgGetVar_s(ltMsgPk,"ldappass"),inituser,domain1);
								system(caTmpCmd);	
								
								sprintf(caTmpCmd,"%s","echo \"export LD_PRELOAD=/usr/lib/libncurses.so.5\" >> /etc/gnm/start/samba.up ");
								system(caTmpCmd);
								sprintf(caTmpCmd,"export LD_PRELOAD=/usr/lib/libncurses.so.5 ");
								system(caTmpCmd);
								
								sprintf(caTmpCmd,"echo \"/app/samba/bin/net ads join -U %s%c'%s'\">> /etc/gnm/start/samba.up  ",inituser,'%',ltMsgGetVar_s(ltMsgPk,"ldappass"));
							  system(caTmpCmd);
							  sprintf(caTmpCmd,"/app/samba/bin/net ads join -U %s%c'%s' ",inituser,'%',ltMsgGetVar_s(ltMsgPk,"ldappass"));
							  system(caTmpCmd);
							  
							  sprintf(caTmpCmd,"echo 'cd /app/samba/sbin/' >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          chdir("/app/samba/sbin/");
			          
			          sprintf(caTmpCmd,"echo 'killall -9 winbindd'  >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          system("killall -9 winbindd");
			          
			          sprintf(caTmpCmd,"echo 'chmod -R 750 /app/samba/var/locks/winbindd_privileged/'   >> /etc/gnm/start/samba.up  ");
			          system(caTmpCmd);
			          system("chmod -R 750 /app/samba/var/locks/winbindd_privileged");
			          
			          sprintf(caTmpCmd,"echo 'chown -R root.root /app/samba/var/locks/winbindd_privileged/'   >> /etc/gnm/start/samba.up  ");
			          system(caTmpCmd);
			          system("chown -R root.root /app/samba/var/locks/winbindd_privileged");
			          
			          sprintf(caTmpCmd,"echo 'sleep 1'  >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          sprintf(caTmpCmd,"echo './winbindd'  >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          system("./winbindd");
			          
			          sprintf(caTmpCmd,"echo 'sleep 1'  >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          sleep(1);
			          
			          sprintf(caTmpCmd,"echo \"#/app/samba/bin/wbinfo --set-auth-user=%s%c'%s'\"  >> /etc/gnm/start/samba.up ",inituser,'%',ltMsgGetVar_s(ltMsgPk,"ldappass"));
			          system(caTmpCmd);
			          
			          sprintf(caTmpCmd,"echo 'chown -R nas.nas /app/samba/var/locks/winbindd_privileged/'   >> /etc/gnm/start/samba.up ");
			          system(caTmpCmd);
			          system("chown -R nas.nas /app/samba/var/locks/winbindd_privileged/");
			
			         system("chmod 755 /etc/gnm/start/samba.up");
			
			         
			
			     	/*修改adinport.pl*/		  
//									  sprintf(caPerl,"#!/usr/bin/perl\nuse Net::LDAP;\nuse Net::LDAP::LDIF;\nuse File::Path qw(rmtree);\n\
//use File::Basename qw(basename);\n$HOST = '%s';\n$PORT = %s;\n$SSL_PORT = 636;\n\
//$PASSWD   = '%s';\n$BASEDN   = '%s';\n$ADMIN= '%s';\n$searchstring='(&(objectclass=user))';\n\
//$attnames=['dn','%s','%s' ,'%s'];\n\n",
//ltMsgGetVar_s(ltMsgPk,"ldaphost"),
//"389",
//ltMsgGetVar_s(ltMsgPk,"ldappass"),
//domain,
//ltMsgGetVar_s(ltMsgPk,"ldapadmin"),
//"sAMAccountName",
//						depfield,
//						policyfield);
//						
//									destfd = open("/etc/gnm/scripts/adinport.pl", O_WRONLY | O_CREAT | O_TRUNC);
//						      if(destfd>0){
//						      	write(destfd, caPerl, strlen(caPerl));
//						      	close(destfd);
//						      }
//						      
//						      sprintf(caTmpCmd,"%s","cat /etc/gnm/scripts/adinport-b.pl >> /etc/gnm/scripts/adinport.pl");
//						      system(caTmpCmd);
//									
//									 system("chmod 755 /etc/gnm/scripts/adinport.pl");
//						
//						/*修改adupdate.pl*/		  
//									  sprintf(caPerl,"#!/usr/bin/perl\nuse Net::LDAP;\nuse Net::LDAP::LDIF;\nuse File::Path qw(rmtree);\n\
//use File::Basename qw(basename);\n$HOST = '%s';\n$PORT = %s;\n$SSL_PORT = 636;\n\
//$PASSWD   = '%s';\n$BASEDN   = '%s';\n$ADMIN= '%s';\n$searchstring=\"(&(objectclass=user)(%s=$ARGV[0]))\";\n\
//$attnames=['dn','%s','%s' ,'%s'];\n\n",
//						ltMsgGetVar_s(ltMsgPk,"ldaphost"),
//						"389",
//						ltMsgGetVar_s(ltMsgPk,"ldappass"),
//						domain,
//						ltMsgGetVar_s(ltMsgPk,"ldapadmin"),
//						"sAMAccountName",
//						"sAMAccountName",
//						depfield,
//						policyfield);
//						
//									destfd = open("/etc/gnm/scripts/adupdate.pl", O_WRONLY | O_CREAT | O_TRUNC);
//						      if(destfd>0){
//						      	write(destfd, caPerl, strlen(caPerl));
//						      	close(destfd);
//						      }
//						      
//						      sprintf(caTmpCmd,"%s","cat /etc/gnm/scripts/adupdate-b.pl >> /etc/gnm/scripts/adupdate.pl");
//						      system(caTmpCmd);
//									
//									 system("chmod 755 /etc/gnm/scripts/adupdate.pl");
			
			 
						}
						
						
						/*运行samba.up*/
				    //system("/etc/gnm/start/samba.up"); 
			      //sleep(3);
	
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"0");  
    
	ltMsgFree(ltMsgPk);
	return 0;

}

int ltadtree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	


			 char *ppp;
			 char *ppp1;
			 char  ldapadmin[256];
	     char baseDN[256];
			/*显示树结构*/
      char tmpstr[512];
			//int level,pid;
			stralloc strTemp;
			LDAP	*ld = NULL;

			int   rc = LDAP_SUCCESS;
			int	  version = LDAP_VERSION3;
			
			LDAPMessage *res = NULL, *e = NULL;
			char  uri[256];
			char  caPass[256];
			struct berval	passwd = { 0, NULL };
      int    iHead;
      char   ldaphost[128];
      char   pOut[512];
      char   dispname[128];
      char		*srchattrs[] = { "cn","objectclass", NULL };
	    char		**attrs = srchattrs;
	    char    *dn;
	    //void    *ptr;
      //char    **vals;
      //int	  nvalues = 0;
			//char	**values = NULL;
      
      sprintf(baseDN,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth","ldapbasedn"));
      sprintf(caPass,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth","ldappass"));
      sprintf(ldapadmin,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth","ldapadmin"));
      sprintf(ldaphost,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth","ldaphost"));
            
			passwd.bv_val = caPass;
			passwd.bv_len = strlen(caPass);
			
			sprintf(uri,"ldap://%s:389",ldaphost);
			ldap_initialize( &ld, uri );
			if ( ld == NULL ) {
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=gb2312\n","lthtml",LT_TYPE_STRING,"[]");  
				ltMsgFree(ltMsgPk);
	      return 0;
			}
			(void) ldap_set_option( ld, LDAP_OPT_PROTOCOL_VERSION, &version ); 
			(void) ldap_set_option( ld, LDAP_OPT_REFERRALS,LDAP_OPT_OFF );
			
			rc = ldap_sasl_bind_s( ld, ldapadmin, LDAP_SASL_SIMPLE, &passwd, NULL, NULL, NULL );
		  if ( rc != LDAP_SUCCESS ) {
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=gb2312\n","lthtml",LT_TYPE_STRING,"[]");  
				ltMsgFree(ltMsgPk);
	      return 0;
			}
			memset(tmpstr,0,sizeof(tmpstr));
			strTemp.s=0;
			stralloc_cats(&strTemp,"[");
			iHead=1;
	    rc = ldap_search_ext_s( ld,baseDN, LDAP_SCOPE_ONELEVEL ,"(|(&(objectclass=container)(cn=Users))(objectclass=organizationalUnit))", attrs, 0, NULL, NULL, NULL, LDAP_NO_LIMIT, &res );
			if ( rc== LDAP_SUCCESS) {
				if ( ldap_count_entries( ld, res ) > 0 ) {
					for ( e = ldap_first_entry( ld, res ); e != NULL;e = ldap_next_entry( ld, e ) ) {
		           /* print its name */
		           dn = ldap_get_dn( ld, e );
		           memset(pOut,0,512);
		           memset(dispname,0,sizeof(dispname));
		           ppp1=pOut;
		           if(strutf82gb(dn,pOut,strlen(dn))==0){
								  		ppp=strstr(pOut,",");
								  		memcpy(dispname,ppp1+3,(ppp-ppp1)-3);
							 }else{
							 		continue;
							 }         
							if(iHead==1){
								sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
								iHead=0;
							}else{
							  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",ppp1,ppp1,dispname);
						  }
						  stralloc_cats(&strTemp,tmpstr);
						  loopLdap(&strTemp,ld,dn);		
						  free( dn ); 
						  stralloc_cats(&strTemp,"}");	
		    }
		  }
	    /* free the search results */
	    ldap_msgfree( res );
	    /* close and free connection resources */
	  }
  rc = ldap_search_ext_s( ld, baseDN, LDAP_SCOPE_ONELEVEL ,"(objectclass=organizationalPerson)", attrs, 0, NULL, NULL, NULL, LDAP_NO_LIMIT, &res );
	if ( rc== LDAP_SUCCESS) {
			if ( ldap_count_entries( ld, res )> 0 ) {
				for ( e = ldap_first_entry( ld, res ); e != NULL;e = ldap_next_entry( ld, e ) ) {
	           /* print its name */
	            dn = ldap_get_dn( ld, e );
		           memset(pOut,0,512);
		           memset(dispname,0,sizeof(dispname));
		           ppp1=pOut;
		           if(strutf82gb(dn,pOut,strlen(dn))==0){
								  		ppp=strstr(pOut,",");
								  		memcpy(dispname,ppp1+3,(ppp-ppp1)-3);
							 }else{
							 		continue;
							 }         
							if(iHead==1){
								sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true,icon: '/images/user.gif'}",ppp1,ppp1,dispname);
								iHead=0;
							}else{
							  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true,icon: '/images/user.gif'}",ppp1,ppp1,dispname);
						  }
						  stralloc_cats(&strTemp,tmpstr);	
						  free( dn ); 
	     }
	   }
    /* free the search results */
    ldap_msgfree( res );
    /* close and free connection resources */
  }

  stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
  if ( ld != NULL ) {
			ldap_unbind_ext( ld, NULL, NULL );
	}
	
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=gb2312\n","lthtml",LT_TYPE_STRING,strTemp.s);  
    
	ltMsgFree(ltMsgPk);
	return 0;

}


int ldapconfig(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	char strTmp[128];

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
		
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldaphost"));
	lt_dbput_rootvars(dbPtr,1,"ldaphost",strTmp);	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldapport"));
	lt_dbput_rootvars(dbPtr,1,"ldapport",strTmp);	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldapbasedn"));
	lt_dbput_rootvars(dbPtr,1,"ldapbasedn",strTmp);	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldapadmin"));
	lt_dbput_rootvars(dbPtr,1,"ldapadmin",strTmp);	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "ldappass"));
	lt_dbput_rootvars(dbPtr,1,"ldappass",strTmp);	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "cnfield"));
	lt_dbput_rootvars(dbPtr,1,"cnfield",strTmp);	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "depfield"));
	lt_dbput_rootvars(dbPtr,1,"depfield",strTmp);	
	
	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "policyfield"));
	lt_dbput_rootvars(dbPtr,1,"policyfield",strTmp);	

	sprintf(strTmp,"%s",bcCnfGetValueS("/etc/gnm/conf","ldapauth", "accountfield"));
	lt_dbput_rootvars(dbPtr,1,"accountfield",strTmp);	

	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/ldapconfig.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}



int ldapconfigok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
				char caPerl[8012];
				char caCmd[256];
				int  destfd;
				int   xaction;
				
				memset(caPerl,0,sizeof(caPerl));
			  
			  xaction=atol(ltMsgGetVar_s(ltMsgPk,"action"));
			  confReplace("/etc/gnm/conf/ldapauth","ldaphost",ltMsgGetVar_s(ltMsgPk,"ldaphost")); /*hostname*/
			  confReplace("/etc/gnm/conf/ldapauth","ldapport",ltMsgGetVar_s(ltMsgPk,"ldapport")); /*kdc-ip*/
			  confReplace("/etc/gnm/conf/ldapauth","ldapbasedn",ltMsgGetVar_s(ltMsgPk,"ldapbasedn"));/*kdc-admin*/
			  confReplace("/etc/gnm/conf/ldapauth","ldapadmin",ltMsgGetVar_s(ltMsgPk,"ldapadmin"));/*kdc-password*/
			  confReplace("/etc/gnm/conf/ldapauth","ldappass",ltMsgGetVar_s(ltMsgPk,"ldappass")); 
			  confReplace("/etc/gnm/conf/ldapauth","depfield",ltMsgGetVar_s(ltMsgPk,"depfield"));
			  confReplace("/etc/gnm/conf/ldapauth","policyfield",ltMsgGetVar_s(ltMsgPk,"policyfield"));
			  confReplace("/etc/gnm/conf/ldapauth","cnfield",ltMsgGetVar_s(ltMsgPk,"cnfield"));

/*修改ldapinport.pl*/		  
			  sprintf(caPerl,"#!/usr/bin/perl\nuse Net::LDAP;\nuse Net::LDAP::LDIF;\nuse File::Path qw(rmtree);\n\
use File::Basename qw(basename);\n$HOST = '%s';\n$PORT = %s;\n$SSL_PORT = 636;\n\
$PASSWD   = '%s';\n$BASEDN   = '%s';\n$ADMIN= '%s';\n$searchstring='(&(objectclass=user))';\n\
$attnames=['dn','%s','%s' ,'%s'];\n\n",
ltMsgGetVar_s(ltMsgPk,"ldaphost"),
ltMsgGetVar_s(ltMsgPk,"ldapport"),
ltMsgGetVar_s(ltMsgPk,"ldappass"),
ltMsgGetVar_s(ltMsgPk,"ldapbasedn"),
ltMsgGetVar_s(ltMsgPk,"ldapadmin"),
ltMsgGetVar_s(ltMsgPk,"accountfield"),
ltMsgGetVar_s(ltMsgPk,"depfield"),
ltMsgGetVar_s(ltMsgPk,"policyfield"));

			destfd = open("/etc/gnm/scripts/ldapinport.pl", O_WRONLY | O_CREAT | O_TRUNC);
      if(destfd>0){
      	write(destfd, caPerl, strlen(caPerl));
      	close(destfd);
      }
      
      sprintf(caCmd,"%s","cat /etc/gnm/scripts/ldapinport-b.pl >> /etc/gnm/scripts/ldapinport.pl");
      system(caCmd);
			
			 system("chmod 755 /etc/gnm/scripts/ldapinport.pl");

/*修改ldapupdate.pl*/		  
			  sprintf(caPerl,"#!/usr/bin/perl\nuse Net::LDAP;\nuse Net::LDAP::LDIF;\nuse File::Path qw(rmtree);\n\
use File::Basename qw(basename);\n$HOST = '%s';\n$PORT = %s;\n$SSL_PORT = 636;\n\
$PASSWD   = '%s';\n$BASEDN   = '%s';\n$ADMIN= '%s';\n$searchstring=\"(&(objectclass=user)(%s=$ARGV[0]))\";\n\
$attnames=['dn','%s','%s' ,'%s'];\n\n",
ltMsgGetVar_s(ltMsgPk,"ldaphost"),
ltMsgGetVar_s(ltMsgPk,"ldapport"),
ltMsgGetVar_s(ltMsgPk,"ldappass"),
ltMsgGetVar_s(ltMsgPk,"ldapbasedn"),
ltMsgGetVar_s(ltMsgPk,"ldapadmin"),
ltMsgGetVar_s(ltMsgPk,"cnfield"),
ltMsgGetVar_s(ltMsgPk,"accountfield"),
ltMsgGetVar_s(ltMsgPk,"depfield"),
ltMsgGetVar_s(ltMsgPk,"policyfield"));

			destfd = open("/etc/gnm/scripts/ldapupdate.pl", O_WRONLY | O_CREAT | O_TRUNC);
      if(destfd>0){
      	write(destfd, caPerl, strlen(caPerl));
      	close(destfd);
      }
      
      sprintf(caCmd,"%s","cat /etc/gnm/scripts/ldapupdate-b.pl >> /etc/gnm/scripts/ldapupdate.pl");
      system(caCmd);
			
			 system("chmod 755 /etc/gnm/scripts/ldapupdate.pl");

/*修改ldapcheck.pl*/					 
sprintf(caPerl,"#!/usr/bin/perl\nuse Net::LDAP;\nuse Net::LDAP::LDIF;\nuse File::Path qw(rmtree);\n\
use File::Basename qw(basename);\n$HOST = '%s';\n$PORT = %s;\n$SSL_PORT = 636;\n\
$PASSWD   = \"$ARGV[1]\";\n$BASEDN   = '%s';\n$ADMIN= \"%s=$ARGV[0],%s\";\n$searchstring='(&(objectclass=user))';\n\
$attnames=['dn','%s','%s' ,'%s'];\n\n",
ltMsgGetVar_s(ltMsgPk,"ldaphost"),
ltMsgGetVar_s(ltMsgPk,"ldapport"),
ltMsgGetVar_s(ltMsgPk,"ldapbasedn"),
ltMsgGetVar_s(ltMsgPk,"cnfield"),
ltMsgGetVar_s(ltMsgPk,"ldapbasedn"),
ltMsgGetVar_s(ltMsgPk,"accountfield"),
ltMsgGetVar_s(ltMsgPk,"depfield"),
ltMsgGetVar_s(ltMsgPk,"policyfield"));

			destfd = open("/etc/gnm/scripts/ldapcheck.pl", O_WRONLY | O_CREAT | O_TRUNC);
      if(destfd>0){
      	write(destfd, caPerl, strlen(caPerl));
      	close(destfd);
      }
      
      sprintf(caCmd,"%s","cat /etc/gnm/scripts/ldapcheck-b.pl >> /etc/gnm/scripts/ldapcheck.pl");
      system(caCmd);
			
			system("chmod 755 /etc/gnm/scripts/ldapcheck.pl");
			 			 
			
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!已经配置成功,稍候请刷新用户列表.</body></html>");		

	   ltMsgFree(ltMsgPk);
	   
	   
	   if(xaction==1){
	   	system("/etc/gnm/scripts/inportldapuser");
     }

	   return 0;

}








int ltqosrunlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char 				sqlBuf[1024];
	char        BANDDOWNLINK[24];
	char        myRate[24];
	char        mySqf[24];
	int         mybrate;
	
	int         qosid;

	
	


	
	if(_ltPubInfo->banddownlink<1 ||_ltPubInfo->banduplink<1 ){
		  char    BANDUPLINK[24];
			char    BANDDOWNLINK[24];
		  memset(BANDDOWNLINK,0,sizeof(BANDDOWNLINK));
		 	bcCnfGetValue_s("/etc/gnm/qos/settings","BANDDOWNLINK",BANDDOWNLINK);
	    memset(BANDUPLINK,0,sizeof(BANDUPLINK));
			bcCnfGetValue_s("/etc/gnm/qos/settings","BANDUPLINK",BANDUPLINK);	
			if(strlen(BANDUPLINK)>0 && strlen(BANDDOWNLINK)>0 ){
				_ltPubInfo->banddownlink=atol(BANDDOWNLINK);
				_ltPubInfo->banduplink=atol(BANDUPLINK);
			}
		  if(_ltPubInfo->banddownlink<1 ||_ltPubInfo->banduplink<1 ){
					ltbandlink(confd,ltMsgPk,lt_MMHead);
					return 0;
		  }
	}
	qosid=0;
	if(ltMsgGetVar_s(ltMsgPk,"qosid")!=NULL){
		qosid=atol(ltMsgGetVar_s(ltMsgPk,"qosid"));
	}
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	

  sprintf(BANDDOWNLINK,"%lu",_ltPubInfo->banddownlink);
  lt_dbput_rootvars(dbPtr,1,"DOWNLINK",BANDDOWNLINK);
  
  sprintf(BANDDOWNLINK,"%lu",_ltPubInfo->banduplink);
  lt_dbput_rootvars(dbPtr,1,"UPLINK",BANDDOWNLINK);
  


  mybrate=0;
  tablePtr=lt_dbput_table(dbPtr,"bqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrate,qosceil,qossfq,prio,qosdefault from nasqosrun where sid>0 and qostype=1 and qoslinktype=1");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			mybrate=mybrate+atol(tempRow[2]);
			
			sprintf(myRate,"%d",(int)( (atol(tempRow[2])*100)/_ltPubInfo->banddownlink) );
			
			if(atol(tempRow[4])==1){
				sprintf(mySqf,"%s","随机");
			}else{
				sprintf(mySqf,"%s","顺序");
		  }
							
			lt_dbput_recordvars(tablePtr,7,
				"bqosname",LT_TYPE_STRING,tempRow[1],
				"bqosrate",LT_TYPE_STRING,tempRow[2],
				"brate",LT_TYPE_STRING,myRate,
				"bqosceil",LT_TYPE_STRING,tempRow[3],
				"bprio",LT_TYPE_STRING,tempRow[5],
				"btype",LT_TYPE_STRING,"保证",
				"bsqf",LT_TYPE_STRING,mySqf);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(myRate,"%lu", _ltPubInfo->banddownlink-mybrate );
	lt_dbput_rootvars(dbPtr,1,"shareqos",myRate);	
	sprintf(myRate,"%d", (int)(((_ltPubInfo->banddownlink-mybrate)*100)/_ltPubInfo->banddownlink)  );
	lt_dbput_rootvars(dbPtr,1,"sharerate",myRate);	
	
	
  
 
  tablePtr=lt_dbput_table(dbPtr,"sqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrate,qosceil,qossfq,prio,qosdefault from nasqosrun where sid>0 and qostype=2  and qoslinktype=1");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_ltPubInfo->banddownlink) );
			
			if(atol(tempRow[4])==1){
				sprintf(mySqf,"%s","随机");
			}else{
				sprintf(mySqf,"%s","顺序");
		  }
							
			lt_dbput_recordvars(tablePtr,7,
				"sqosname",LT_TYPE_STRING,tempRow[1],
				"sqosrate",LT_TYPE_STRING,tempRow[2],
				"srate",LT_TYPE_STRING,myRate,
				"sqosceil",LT_TYPE_STRING,tempRow[3],
				"sprio",LT_TYPE_STRING,tempRow[5],
				"stype",LT_TYPE_STRING,"竞争",
				"ssqf",LT_TYPE_STRING,mySqf);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	
	/*shangchuang*/


  mybrate=0;
  tablePtr=lt_dbput_table(dbPtr,"upbqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrate,qosceil,qossfq,prio,qosdefault from nasqosrun where sid>0 and qostype=1  and qoslinktype=2");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			mybrate=mybrate+atol(tempRow[2]);
			
			sprintf(myRate,"%d",(int)( (atol(tempRow[2])*100)/_ltPubInfo->banduplink) );
			
			if(atol(tempRow[4])==1){
				sprintf(mySqf,"%s","随机");
			}else{
				sprintf(mySqf,"%s","顺序");
		  }
							
			lt_dbput_recordvars(tablePtr,7,
				"upbqosname",LT_TYPE_STRING,tempRow[1],
				"upbqosrate",LT_TYPE_STRING,tempRow[2],
				"upbrate",LT_TYPE_STRING,myRate,
				"upbqosceil",LT_TYPE_STRING,tempRow[3],
				"upbprio",LT_TYPE_STRING,tempRow[5],
				"upbtype",LT_TYPE_STRING,"保证",
				"upbsqf",LT_TYPE_STRING,mySqf);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(myRate,"%lu", _ltPubInfo->banduplink-mybrate );
	lt_dbput_rootvars(dbPtr,1,"upshareqos",myRate);	
	sprintf(myRate,"%d", (int)(((_ltPubInfo->banduplink-mybrate)*100)/_ltPubInfo->banduplink)  );
	lt_dbput_rootvars(dbPtr,1,"upsharerate",myRate);	
	
	
  
 
  tablePtr=lt_dbput_table(dbPtr,"upsqoslist");
  sprintf(sqlBuf,"select sid,qosname,qosrate,qosceil,qossfq,prio,qosdefault from nasqosrun where sid>0 and qostype=2  and qoslinktype=2");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			
			
			sprintf(myRate,"%d",(int)((atol(tempRow[2])*100)/_ltPubInfo->banduplink) );
			
			if(atol(tempRow[4])==1){
				sprintf(mySqf,"%s","随机");
			}else{
				sprintf(mySqf,"%s","顺序");
		  }
							
			lt_dbput_recordvars(tablePtr,7,
				"upsqosname",LT_TYPE_STRING,tempRow[1],
				"upsqosrate",LT_TYPE_STRING,tempRow[2],
				"upsrate",LT_TYPE_STRING,myRate,
				"upsqosceil",LT_TYPE_STRING,tempRow[3],
				"upsprio",LT_TYPE_STRING,tempRow[5],
				"upstype",LT_TYPE_STRING,"竞争",
				"upssqf",LT_TYPE_STRING,mySqf);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	
  
  lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/bandrunlist.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
	


}







int ltresetqos(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	  char *dbUser;
    char *dbPass;
    char *dbName;
    char *pid;
    ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
    ltDbConn *tempDbCon;
    char   sqlBuf[2048];
    dbName=_ltPubInfo->_dbname;
    dbUser=_ltPubInfo->_dbuser;
    dbPass=_ltPubInfo->_dbpass;
    
   
    tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (tempDbCon==NULL){
					
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >error!临时错误，稍候再试.</body></html>");		

	   ltMsgFree(ltMsgPk);
	   return 0;
    }  
    
  sprintf(sqlBuf,"%s","select a.sid,a.groupid,b.sharetype,b.maxflow,b.maxtime,b.maxtcp,b.maxudp, \
b.qosid,b.moreflag, b.maxpackage,b.maxBytes,b.maxudpp,b.maxtcpp,b.maxupp,b.maxdownp,b.maxsp,b.maxnc,\
b.onlybindip,b.macbindip,b.onlylogone from nasqosmatchgroup a left join nasqosmatch b on a.sid=b.sid order by b.prio desc " );
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){		
			tempRow= ltDbFetchRow(tempCursor); 
			while(tempRow!=NULL){
				sprintf(sqlBuf,"update nasuser set ratelimit=%s,ratelower=%s,lLimitBytes=%s,lLimitTimes=%s,maxpackage=%s,maxBytes=%s,moreflag='%s',\
 maxpackage=%s,maxBytes=%s,maxudpp=%s,maxtcpp=%s,maxupp=%s,maxdownp=%s,maxsp=%s,maxnc=%s,onlybindip=%s,macbindip=%s,onlylogone=%s where groupid=%s",
				tempRow[7],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[8],
				tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],
				tempRow[1]);
printf("sqlBuf:%s\n",sqlBuf);
				ltDbExecSql(tempDbCon,sqlBuf); 
				tempRow= ltDbFetchRow(tempCursor); 
			}
			ltDbCloseCursor(tempCursor);
	}
	
	sprintf(sqlBuf,"select a.sid,a.policy,b.sharetype,b.maxflow,b.maxtime,b.maxtcp,b.maxudp, \
b.qosid,b.moreflag , b.maxpackage,b.maxBytes,b.maxudpp,b.maxtcpp,b.maxupp,b.maxdownp,b.maxsp,b.maxnc,\
b.onlybindip,b.macbindip,b.onlylogone from nasqosmatchpolicy a left join nasqosmatch b on a.sid=b.sid  order by b.prio desc " );
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){		
			tempRow= ltDbFetchRow(tempCursor); 
			while(tempRow!=NULL){
				pid=tempRow[1];
				sprintf(sqlBuf,"update nasuser set ratelimit=%s,ratelower=%s,lLimitBytes=%s,lLimitTimes=%s,\
 maxpackage=%s,maxBytes=%s,moreflag='%s',\
 maxpackage=%s,maxBytes=%s,maxudpp=%s,maxtcpp=%s,maxupp=%s,maxdownp=%s,maxsp=%s,maxnc=%s,onlybindip=%s,\
 macbindip=%s,onlylogone=%s  where policyid= %s \
 or policyid2=%s or policyid3=%s or policyid4=%s or policyid5=%s or policyid6=%s \
 or policyid7=%s or policyid8=%s or policyid9=%s or policyid10=%s or policyid11=%s \
 or policyid12=%s or policyid13=%s or policyid14=%s or policyid15=%s or policyid16=%s \
 or policyid17=%s or policyid18=%s or policyid19=%s or policyid20=%s or policyid21=%s \
 or policyid22=%s or policyid23=%s or policyid24=%s or policyid25=%s or policyid26=%s \
 or policyid27=%s or policyid28=%s or policyid29=%s or policyid30=%s or policyid31=%s \ 
 or policyid32=%s or policyid33=%s or policyid34=%s or policyid35=%s or policyid36=%s \
 or policyid37=%s or policyid38=%s or policyid39=%s or policyid40=%s or policyid41=%s \
 or policyid42=%s or policyid43=%s or policyid44=%s or policyid45=%s or policyid46=%s \
 or policyid47=%s or policyid48=%s or policyid49=%s or policyid50=%s or policyid51=%s \
 or policyid52=%s or policyid53=%s or policyid54=%s or policyid55=%s or policyid56=%s \
 or policyid57=%s or policyid58=%s or policyid59=%s or policyid60=%s ",
				tempRow[7],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[8],
				tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],
				pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,
 pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,
 pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid,pid);	
 printf("sqlBuf:%s\n",sqlBuf);
				ltDbExecSql(tempDbCon,sqlBuf); 
				tempRow= ltDbFetchRow(tempCursor); 
			}
			ltDbCloseCursor(tempCursor);
	}
	
	  sprintf(sqlBuf,"select a.sid,a.userid,b.sharetype,b.maxflow,b.maxtime,b.maxtcp,b.maxudp, \
b.qosid,b.moreflag from nasqosmatchuser a left join nasqosmatch b on a.sid=b.sid order by b.prio desc " );
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){		
			tempRow= ltDbFetchRow(tempCursor); 
			while(tempRow!=NULL){
				sprintf(sqlBuf,"update nasuser set ratelimit=%s,ratelower=%s,lLimitBytes=%s,lLimitTimes=%s,maxpackage=%s,maxBytes=%s,moreflag='%s',\
 maxpackage=%s,maxBytes=%s,maxudpp=%s,maxtcpp=%s,maxupp=%s,maxdownp=%s,maxsp=%s,maxnc=%s,onlybindip=%s,macbindip=%s,onlylogone=%s where userid=%s",
				tempRow[7],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[8],
				tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[1]);
printf("sqlBuf:%s\n",sqlBuf);
				ltDbExecSql(tempDbCon,sqlBuf); 
				tempRow= ltDbFetchRow(tempCursor); 
			}
			ltDbCloseCursor(tempCursor);
	}
	
	
 			
	    ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n",
	"lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Success!</body></html>");		

	   ltMsgFree(ltMsgPk);
	   return 0;
  
}





int ltlanarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char strDesc[20];
	long iIndex;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"ipbindlist");

  sprintf(sqlBuf,"select id,vlanid,ip,mask,dev from nasVLanArea order by  vlanid asc ");
	iIndex=0;
	tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){

			lt_dbput_recordvars(tablePtr,6,
				"inum",LT_TYPE_LONG,iIndex,
				"listindex",LT_TYPE_STRING,tempRow[0],
				"vid",LT_TYPE_STRING,tempRow[1],
				"ip",LT_TYPE_STRING,tempRow[2],
				"mask",LT_TYPE_STRING,tempRow[3],
				"dev",LT_TYPE_STRING,tempRow[4]);
			tempRow= ltDbFetchRow(tempCursor);
			iIndex++;
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(strDesc,"%ld",iIndex);
	lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/naslanlist.htm",dbPtr->head,0);
	
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}


int ltlanaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int  actiontype;
	//char caMsg[256];
	
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
		long lCount;
		sprintf(sqlBuf,"select count(*) from nasVLanArea ");
		lCount =0;
		tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
				lCount=atol(tempRow[0]);
			}
			ltDbCloseCursor(tempCursor);
		}
		
		if(lCount == 0) {
			lCount=1;
		}else{
			
			sprintf(sqlBuf,"select max(id)+1 from nasVLanArea ");
			lCount = 1;
			tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
				ltDbCloseCursor(tempCursor);
			}
		}	
		sprintf(sqlBuf,"insert into nasVLanArea values (%ld,'%s','%s','%s','%s')",lCount,
			ltMsgGetVar_s(ltMsgPk,"vid"),
			ltMsgGetVar_s(ltMsgPk,"ip"),
			ltMsgGetVar_s(ltMsgPk,"mask"),ltMsgGetVar_s(ltMsgPk,"dev"));
//printf("sql:%s\n",sqlBuf);
		ltDbExecSql(G_DbCon,sqlBuf); 
		
//		snprintf(caMsg,255,"%s","update netarea success.");
//	  logWrite(confd,ltMsgPk,caMsg,3,lt_MMHead);
	
	}else if (actiontype==2){/*del*/
		long allnum,i;
		char tmpVal[20];
		allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
		for(i=0;i<allnum;i++){
			sprintf(tmpVal,"checkbox%ld",i);
			if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
				snprintf(sqlBuf,1024,"delete from nasVLanArea where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));		
				ltDbExecSql(G_DbCon,sqlBuf); 
			}
		}

	}
	ltlanarealist(confd,ltMsgPk,lt_MMHead);
	return 0;

}



int ltapplylan(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	//char strDesc[20];
	long iIndex;
  int fd ;
  char caLine[512];
  int fdvlan ;
  system("echo >/etc/gnm/conf/dev/macdev");
  fd = open_write("/etc/gnm/conf/dev/macdev");
	if (fd <0)
	{
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head></head><body bgcolor=\"#cccccc\" >open macdev error!</body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
	}
	system("echo >/etc/vlan.ini");
	fdvlan = open_write("/etc/vlan.ini");
	if (fdvlan <0)
	{
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head></head><body bgcolor=\"#cccccc\" >open vlan.ini error!</body></html>");		
			ltMsgFree(ltMsgPk);
			return 0;
	}
	write(fdvlan,"rmmod 8021q\nmodprobe 8021q\n",strlen("rmmod 8021q\nmodprobe 8021q\n"));
	

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"ipbindlist");

  sprintf(sqlBuf,"select id,vlanid,ip,mask,dev from nasVLanArea order by  vlanid asc ");
	iIndex=0;
	tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
      sprintf(caLine,"%s %s %s %s\n",tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
      write(fd,caLine,strlen(caLine));
      
      if(atol(tempRow[1])>0){
		      sprintf(caLine,"vconfig add %s %s\n",tempRow[4],tempRow[1]);
		      write(fdvlan,caLine,strlen(caLine));
		      sprintf(caLine,"ifconfig %s.%s %s netmask %s\n",tempRow[4],tempRow[1],tempRow[2],tempRow[3]);
		      write(fdvlan,caLine,strlen(caLine));
      }
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	close(fd);
	close(fdvlan);
	system("chmod 755 /etc/vlan.ini");


	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><head><meta http-equiv=\"refresh\" content=\"20;URL=/main/main.xp?Fun=ltnetarealist\" ></head><body bgcolor=\"#cccccc\" >此操作需要系统启动才可以生效！</body></html>");		
	ltMsgFree(ltMsgPk);
	
	return 0;

	/*
	nasInitIpHostInfo(lt_MMHead);

	ltnetarealist(confd,ltMsgPk,lt_MMHead);
	
	return 0;
	*/

}


