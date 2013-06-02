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

#include "nas.h"

typedef struct nasTcpBufList_s {
    nasTcpBuffer  *ltTcpBuffer;
} nasTcpBufList;

static int nasSortTcpBufByLastTime(const void *p1,const void *p2)
{
    nasTcpBuffer *psTcp1,*psTcp2;
    psTcp1 = (nasTcpBuffer *)p1;
    psTcp2 = (nasTcpBuffer *)p2;    
    return (psTcp2->lLastTime - psTcp1->lLastTime);
}



int ltwebonlinenewip(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 
	int i,j;
  shm_hash_link_tcp *tmpHashLink= NULL;	
  shm_hash_link_tcp *_gnm_hash_tcp;	
  int  icount;
  char *p;
  nasTcpBuffer *myTcp;
  int  allcount;
  int  pagenum;
  int  curpage;
  char *sip;
  int  uid;
  int  gid;
  int  srvid;
  int  showtype;
  unsigned int my_ip;
  shm_hash_link_user *xHashLink;
  nasTcpBuffer  *ltTcpBuffer;
  char sSrv[48];
	char sByte1[30];
	char sByte0[30];
	char dTime[40];
	char     caGroup[64];          /* 部门           */
  char     caName[64];
	char     caProt[10];
	char     caCon[512];
	char     qosChannel[64];
	char     cMatchLevel[32];
	char     cUrlSort[64];
  char caHead[100000];
  char caTmpp[2048];
  char *pppp;
  nasUserInfo  *ltUserInfo;
  nasService   *ltService;
  struct in_addr s;
  struct in_addr d;
	char sDip[20];
	char sSip[20];
	char dPort[20];
	char sPort[20];
	char *sHost;
	int  lNum;
	char     strConn[100];
	char     sqlBuf[1024];
	nasTcpBufList *myBufList;
	ltDbCursor *tempCursor;
  LT_DBROW   tempRow;
  int        urlsort;
  
  ltService = (nasService *)(_ltPubInfo->ltService);
  
  allcount=200;
  pagenum=50;
  curpage=1;
  gid=-1;
  uid=-1;
  srvid=-1;
  showtype=0;
  
  if(ltMsgGetVar_s(ltMsgPk,"srvid")){
		srvid=atol(ltMsgGetVar_s(ltMsgPk,"srvid"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"user")){
		uid=atol(ltMsgGetVar_s(ltMsgPk,"user"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"group")){
		gid=atol(ltMsgGetVar_s(ltMsgPk,"group"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"curpage")){
		curpage=atol(ltMsgGetVar_s(ltMsgPk,"curpage"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"allcount")){
		allcount=atol(ltMsgGetVar_s(ltMsgPk,"allcount"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"pagenum")){
		pagenum=atol(ltMsgGetVar_s(ltMsgPk,"pagenum"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"showtype")){
		showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
	}
	sHost="";
	if(ltMsgGetVar_s(ltMsgPk,"sHost")){
		sHost=ltMsgGetVar_s(ltMsgPk,"sHost");
	}
	urlsort=-1;
	if(ltMsgGetVar_s(ltMsgPk,"urlsort")){
		urlsort=atol(ltMsgGetVar_s(ltMsgPk,"urlsort"));
	}
	
	my_ip=0;
	sip=ltMsgGetVar_s(ltMsgPk,"sip");
	if(sip){
		if(strlen(sip)>0){
      my_ip=ntohl(inet_addr(sip));
    }
  }
  
  if(allcount>500){
  	allcount=500;
  }
  
  myBufList=(nasTcpBufList *)malloc(allcount*sizeof(nasTcpBufList));
  memset(myBufList,0,allcount*sizeof(nasTcpBufList));
  
  icount=0;
  p=(char *)_ltPubInfo;
  _gnm_hash_tcp=(shm_hash_link_tcp *)(p+sizeof(nasPubInfo)+_ltPubInfo->onlinetcpptr);
  
	for (i = 0; i < _ltPubInfo->lMaxTcpBufferNum*(NAS_HASHSPARE+1); i++) {
	
		tmpHashLink=&_gnm_hash_tcp[i];
    if(tmpHashLink->flag==0){
    	continue;
    }
    if(tmpHashLink->flag==4){
    	continue;
    }
    myTcp=&tmpHashLink->item;
    if(myTcp->lId<0){
    	continue;
    }
    
    if(srvid!=-1){
    	if(srvid!=myTcp->cService){
    		continue;
      }
    }
    if(srvid==5 && strlen(sHost)>0){
    	
    	if(strstr(myTcp->keyword1,sHost)==NULL ){
    		continue;
      }
    }
    if(srvid==5 && urlsort!=-1){
    	if( (urlsort != myTcp->cUrlSort[0]) && (urlsort != myTcp->cUrlSort[1])  ){
    		continue;
      }
    }
    if(showtype!=0){
				if(showtype==1){/*nomar*/
					if(myTcp->cAction!=1 && myTcp->qosChannel==0 ){
						continue;
					}
				}else if(showtype==2){/*deny*/
					if(myTcp->cAction!=0){
						continue;
					}
				}else if(showtype==3){/*qos*/
					if( myTcp->cAction!=1 || myTcp->qosChannel==0 ){
						continue;
					}
				}
		}
		
    if(uid!=-1){
    	if(uid!=myTcp->lId){
    		continue;
      }
    }else{
    
	    if(gid!=-1){
		    xHashLink=NULL;    
				xHashLink=(shm_hash_link_user *)_gnm_hash_lookup_userbyid(_ltPubInfo, myTcp->lSip,myTcp->lId);
				if(xHashLink!=NULL){
				    	/*该用户已经在线*/
				    	ltUserInfo=&xHashLink->item;
	
							if(ltUserInfo->lGid!=gid){
									continue;
							}
							
				}
		  }
    
    }
    
    if(my_ip){
    	if(myTcp->lSip!=my_ip && myTcp->lDip!=my_ip ){
    		continue;
    	}
    }
    
    
    
    //printf("myTcp->lId:%d\n",myTcp->lId);
    myBufList[icount].ltTcpBuffer=myTcp;
    icount++;
    if(icount>allcount){
    	break;
    }
 }
 memset(caHead,0,sizeof(caHead)); 
 pppp=caHead;
 pppp=strcat(pppp,"[");
 lNum=0;
 
 i=(curpage-1)*pagenum;
 if(i<0 || i>=allcount){
 		i=0;	
 }
 j=i+pagenum;
 if(j>icount){
 	j=icount;
 }
 
 
 
 
 for(;i<j;i++){
 	  
 	  ltTcpBuffer=myBufList[i].ltTcpBuffer;
 	  if(ltTcpBuffer->lId<=0){
			continue;
		}
		sprintf(caGroup,"%s","");          /* 部门           */
    sprintf(caName,"%s","");
		

		sprintf(sSrv,"%s",ltService[ltTcpBuffer->cService].srvName);
		
		

		/*根据ip得到用户信息*/
		xHashLink=NULL;    
		xHashLink=(shm_hash_link_user *)_gnm_hash_lookup_userbyid(_ltPubInfo, ltTcpBuffer->lSip,ltTcpBuffer->lId);
		if(xHashLink!=NULL){
		    	/*该用户已经在线*/
		    	ltUserInfo=&xHashLink->item;
					sprintf(caGroup,"%s",ltUserInfo->caGroup);    
    			sprintf(caName,"%s",ltUserInfo->caName);
		}else{
					sprintf(caGroup,"%s","Unknown");    
    			sprintf(caName,"%s","Unknown");
		}
		
		memset(caCon,0,sizeof(caCon));
  
    if( (ltTcpBuffer->cService==NAS_SERVICE_HTTP || ltTcpBuffer->cService==NAS_SERVICE_HTTPSRV||  (ltTcpBuffer->cService<106 && ltTcpBuffer->cService>99) ) && ltTcpBuffer->keyword1[0] ){
    	sprintf(caCon,"http://%s%s", ltTcpBuffer->keyword1,ltTcpBuffer->keyword2);
    }else if(ltTcpBuffer->cService==NAS_SERVICE_QQ && ltTcpBuffer->keyword2[0] ){
    	if(ltTcpBuffer->keyword3[0]=='1'){
    		sprintf(caCon,"ID:%s LogOn", ltTcpBuffer->keyword2);
    	}else if(ltTcpBuffer->keyword3[0]=='2'){
    		sprintf(caCon,"ID:%s LogOff", ltTcpBuffer->keyword2);
    	}else if(ltTcpBuffer->keyword3[0]=='3'){
    		sprintf(caCon,"ID:%s SendMsg", ltTcpBuffer->keyword2);
    	}else if(ltTcpBuffer->keyword3[0]=='4'){
    		sprintf(caCon,"ID:%s SendMsg by proxy", ltTcpBuffer->keyword2);
    	}
    	
    }else if(ltTcpBuffer->cService==NAS_SERVICE_POP3){
    	sprintf(caCon,"USER:%s  ", ltTcpBuffer->keyword2);
    }else if(ltTcpBuffer->cService==NAS_SERVICE_SMTP){
    	sprintf(caCon,"FROM:%s  To:%s", ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    }
    else if(ltTcpBuffer->cService==NAS_SERVICE_FTP){
    	sprintf(caCon,"%s  %s-%s ", ltTcpBuffer->keyword3,ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    }else if(	ltTcpBuffer->cService==NAS_SERVICE_HaoFang || 
    					ltTcpBuffer->cService==NAS_SERVICE_BF4G || ltTcpBuffer->cService==NAS_SERVICE_BF5Z ||
    					ltTcpBuffer->cService==NAS_SERVICE_BFZGXQ   ){
    	if(ltTcpBuffer->keyword2[0]){
    		sprintf(caCon,"USER:%s ", ltTcpBuffer->keyword2);
      }
    }else if(	ltTcpBuffer->cService==NAS_SERVICE_MSN ){
    	
    	if(ltTcpBuffer->keyword4>0){
    		if(ltTcpBuffer->keyword4==1){
    			sprintf(caCon,"%s-%s %s","文字聊天",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    	  }else if(ltTcpBuffer->keyword4==2){
    			sprintf(caCon,"%s-%s %s %s","文件传输",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1,ltTcpBuffer->keyword3);
    	  }else if(ltTcpBuffer->keyword4==3){
    			sprintf(caCon,"%s-%s %s","应用共享",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    	  }else if(ltTcpBuffer->keyword4==4){
    			sprintf(caCon,"%s-%s %s","远程协助",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    	  }else if(ltTcpBuffer->keyword4==5){
    			sprintf(caCon,"%s-%s %s","音视频",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
    	  }
    	}
    	sprintf(caCon,"%s-%s ", ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
      
    }
    /*
    1>Content-Type: text/plain                    文本消息
conent-type:text/x-msmsgsinvite             音频连接
2>文件传输： 5D3E02AB-6190-11d3-BBBB-00C04F795683
3> 应用共享：F1B1920C-6A3C-4ce7-B18C-AFAB305FD03D 
4〉远程协助：56b994a7-380f-410b-9985-c809d78c1bdc
5〉音视频：  02D3C01F-BF30-4825-A83A-DE7AF41648AA*/

		s.s_addr = htonl(ltTcpBuffer->lSip);
		d.s_addr = htonl(ltTcpBuffer->lDip);
		sprintf(sSip,"%s",inet_ntoa(s));
		sprintf(sDip,"%s",inet_ntoa(d));
		nasTimeFormat(dTime,ltTcpBuffer->lLastTime);
		sprintf(dPort,"%d",ltTcpBuffer->nDport);
		sprintf(sPort,"%d",ltTcpBuffer->nSport);
 		
		sprintf(sByte0,"%llu",ltTcpBuffer->lBytes[0]/1024);
		sprintf(sByte1,"%llu",ltTcpBuffer->lBytes[1]/1024);
		if(ltTcpBuffer->cType==6){
			sprintf(caProt,"%s","TCP");
		}else if(ltTcpBuffer->cType==17){
			sprintf(caProt,"%s","UDP");
		}else{
			sprintf(caProt,"%s","???");
		}
		if(ltTcpBuffer->cDirect==1){
			sprintf(strConn," %s %s:%s<=%s:%s ",caProt,sSip,sPort,sDip,dPort);
			
		}else {
			sprintf(strConn," %s %s:%s=>%s:%s ",caProt,sSip,sPort,sDip,dPort);		
		}

		
		sprintf(qosChannel,"%s","No QOS");
		sprintf(cMatchLevel,"%s","系统级别");
		sprintf(cUrlSort,"%s","????");
		if(ltTcpBuffer->cMatchLevel==1){
			sprintf(cMatchLevel,"%s","部门级别");
		}else if(ltTcpBuffer->cMatchLevel==2){
			sprintf(cMatchLevel,"%s","策略级别");
		}else if(ltTcpBuffer->cMatchLevel==3){
			sprintf(cMatchLevel,"%s","用户级别");
		}else if(ltTcpBuffer->cMatchLevel==4){
			sprintf(cMatchLevel,"%s","IP级别");
		}

		ltTcpBuffer->cUrlSort[4]='\0';
		//printf("ltTcpBuffer->cUrlSort: %d-%d-%d-%d \n",ltTcpBuffer->cUrlSort[0],ltTcpBuffer->cUrlSort[1],ltTcpBuffer->cUrlSort[2],ltTcpBuffer->cUrlSort[3]);
		
		sprintf(cUrlSort,"%s",_ltPubInfo->urlSortName[ltTcpBuffer->cUrlSort[0]].sortname );

		if(ltTcpBuffer->qosChannel	< (NAS_MAX_QOSSTATIC_CHANEL+_ltPubInfo->maxonlineuser+1) ){	
				sprintf(sqlBuf,"select qosname from nasqosrun where cid=%d ",ltTcpBuffer->qosChannel);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow!=NULL){
						sprintf(qosChannel,"%s",tempRow[0]);
					}
					ltDbCloseCursor(tempCursor);
				}
		}
    
    lNum++;
		if(lNum==1){
   
   
					sprintf(caTmpp,"['%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s']",
								   ltTcpBuffer->lId,caName,caGroup, sSrv,_ltPubInfo->ltService[ltTcpBuffer->cService].srvName ,
								   caProt,sSip,sPort,sDip,dPort,sByte0,sByte1,caCon,cMatchLevel,cUrlSort,qosChannel);
								  
								  pppp=strcat(pppp,caTmpp);
								
		 }else{
					sprintf(caTmpp,",['%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s']",
								   ltTcpBuffer->lId,caName,caGroup, sSrv,_ltPubInfo->ltService[ltTcpBuffer->cService].srvName ,
								   caProt,sSip,sPort,sDip,dPort,sByte0,sByte1,caCon,cMatchLevel,cUrlSort,qosChannel);
								  pppp=strcat(pppp,caTmpp);
		
		}
 	
 }
 
 
   pppp=strcat(pppp,"]");
	
   ltMsgPk->msgpktype=3;  

    sprintf(caTmpp,"Content-Length: %d\nContent-type: text/html; charset=gb2312\n",strlen(caHead));
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,caTmpp,"lthtml",LT_TYPE_STRING,caHead);

	ltMsgFree(ltMsgPk);

	return 0;
 
   
}

//int ltwebonlinenewip(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//	
//	nasTcpBuffer  *ltTcpBuffer;
//  int i;
//	struct in_addr s;
//	struct in_addr d;
//	shm_hash_link_user *xHashLink;	
//	char sDip[20];
//	char sSip[20];
//	char dPort[20];
//	char sPort[20];
//	
//	char sSrv[48];
//	char sByte1[30];
//	char sByte0[30];
//	char dTime[40];
//	char     caGroup[64];          /* 部门           */
//  char     caName[64];
//	char     caProt[10];
//	char     caCon[512];
//	char     qosChannel[64];
//	char     cMatchLevel[32];
//	char     cUrlSort[64];
//	nasUserInfo  *ltUserInfo;
//	unsigned long lMaxTcpbuffer;
//	nasTcpBuffer *psTcp1;
//	int iIndex;
//	char *p;
//	char xPlt[100000];
//  nasService *ltService;
//	int iTemp;
//	char     strConn[100];
//	int      showtype;  /*0 all; 1:nomar; 2:block  3:qos*/
//	int      user;
//	int      group;
//	int      srvid;
//	char     sqlBuf[1024];
//	ltDbCursor *tempCursor;
//  LT_DBROW   tempRow;
//	
//	ltService = (nasService *)(_ltPubInfo->ltService);
//	showtype=0;
//	user=-1;
//	group=-1;
//	srvid=-1;
//	if(ltMsgGetVar_s(ltMsgPk,"srvid")){
//		srvid=atol(ltMsgGetVar_s(ltMsgPk,"srvid"));
//	}
//	if(ltMsgGetVar_s(ltMsgPk,"showtype")){
//		showtype=atol(ltMsgGetVar_s(ltMsgPk,"showtype"));
//	}
//	if(ltMsgGetVar_s(ltMsgPk,"user")){
//		user=atol(ltMsgGetVar_s(ltMsgPk,"user"));
//	}
//	if(ltMsgGetVar_s(ltMsgPk,"group")){
//		group=atol(ltMsgGetVar_s(ltMsgPk,"group"));
//	}
//	_ltPubInfo->nowviewtype=showtype;
//	_ltPubInfo->nowviewuid=user;
//	_ltPubInfo->nowviewgid=group;
//  _ltPubInfo->nowviewsrv=srvid;
//
//	_ltPubInfo->lasttime=time(0);
//	memset(xPlt,0,sizeof(xPlt));
//p=strcpy(xPlt,"<html><head><meta http-equiv=\"refresh\" content=\"10\"> \
//<style type=\"text/css\">\r\n \
//body {  font-size: 12pt}\r\n  \
//table {  font-size: 9pt}\r\n  \
//form {margin: 0px;}\r\n  \
//</style>\r\n  \
//</head> \r\n  \
//<body bgcolor=\"#ffffff\" text=\"#333333\" oncontextmenu=\"return false\"  leftmargin=\"1\" topmargin=\"1\"  style=\"SCROLLBAR-FACE-COLOR: #f0f0f0;  overflow-x:hidden;  OVERFLOW: scroll; SCROLLBAR-SHADOW-COLOR: #f0f0f0; SCROLLBAR-3DLIGHT-COLOR: #f0f0f0; SCROLLBAR-ARROW-COLOR: #f0f0f0; SCROLLBAR-DARKSHADOW-COLOR: #cccccc;\"> \
//<table  cellspacing=0  class=listDataTable id=DataTable cellpadding=\"0\"  border=\"0\" align=\"left\" width=\"140%\"><tbody>"); 
//
//
//if(srvid!=5 ){
//	if(showtype==1 || showtype==0){
//p=strcat(p,"<tr >\n");
//p=strcat(p,"    <td  width=\"9%\" height=20  valign=\"bottom\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">名称</div> \n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"25%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">连接状况</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"9%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">服务</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">进流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">出流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"11%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">访问时间</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"40%\" background=\"/images/barbg.gif\">\n"); 
//p=strcat(p,"      <div align=\"left\">内容</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"  </tr>\n");
//	}else if(showtype==2){/*block*/
//		p=strcat(p,"<tr >\n");
//p=strcat(p,"    <td  width=\"9%\" height=20  valign=\"bottom\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">名称</div> \n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"25%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">连接状况</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"9%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">服务</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"11%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">访问时间</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"40%\" background=\"/images/barbg.gif\">\n"); 
//p=strcat(p,"      <div align=\"left\">阻断级别及规则</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"  </tr>\n");
//	}else if(showtype==3){/*qos*/
//		
//		p=strcat(p,"<tr >\n");
//p=strcat(p,"    <td  width=\"9%\" height=20  valign=\"bottom\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">名称</div> \n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"30%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">连接状况及QOS通道</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"9%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">服务</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">进流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">出流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"11%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">访问时间</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"35%\" background=\"/images/barbg.gif\">\n"); 
//p=strcat(p,"      <div align=\"left\">内容</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"  </tr>\n");
//		
//	}
//}else{/*http*/
//	
//
//		p=strcat(p,"<tr >\n");
//p=strcat(p,"    <td  width=\"9%\" height=20  valign=\"bottom\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">名称</div> \n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td onyes=\"head\"   valign=\"bottom\" width=\"40%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">URL</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">进流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"   valign=\"bottom\" width=\"5%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">出流量</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"11%\" background=\"/images/barbg.gif\"> \n");
//p=strcat(p,"      <div align=\"left\">访问时间</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"    <td  onyes=\"head\"  valign=\"bottom\" width=\"20%\" background=\"/images/barbg.gif\">\n"); 
//p=strcat(p,"      <div align=\"left\">类别</div>\n");
//p=strcat(p,"    </td>\n");
//p=strcat(p,"  </tr>\n");
//	
//
//}
//
//	lMaxTcpbuffer=50;
//	psTcp1 = malloc(lMaxTcpbuffer*sizeof(nasTcpBuffer));
//  if(psTcp1 == NULL) {
//        	return 0;
//  }    	
//	memset(psTcp1,0,lMaxTcpbuffer*sizeof(nasTcpBuffer));
// 	
//	iIndex=0;
//	
//	for(iTemp=0;iTemp<50;iTemp++){
//		if(_lastBuffer[iTemp].lId>0){
//			if(srvid!=-1){
//				if(_lastBuffer[iTemp].cService!=srvid){
//					continue;
//				}
//			}
//			if(showtype!=0){
//				if(showtype==1){/*nomar*/
//					if(_lastBuffer[iTemp].cAction!=1){
//						continue;
//					}
//				}else if(showtype==2){/*deny*/
//					if(_lastBuffer[iTemp].cAction!=0){
//						continue;
//					}
//				}else if(showtype==3){/*qos*/
//					if( _lastBuffer[iTemp].cAction!=1 || _lastBuffer[iTemp].qosChannel==0 ){
//						continue;
//					}
//				}
//			}
//			if(user!=-1){
//				if(_lastBuffer[iTemp].lId!=user){
//					continue;
//				}
//			}
////			if(group!=-1){
////				if(_lastBuffer[iTemp].lGid!=group){
////					continue;
////				}
////			}
//			
//			memcpy(&psTcp1[iIndex],&_lastBuffer[iTemp],sizeof(nasTcpBuffer));
//			iIndex++;
//		}
//	}
//	/* 按最后访问时间排序  */
//	if(iIndex<1){
//		
//		p=strcat(p,"</tbody></body></html>");
//		
//		ltMsgPk->msgpktype=1;
//	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,xPlt);
//		free(psTcp1);
//		ltMsgFree(ltMsgPk);
//		return 0;
//	}
//	/* 按最后访问时间排序  */
//
//  qsort(psTcp1,iIndex,sizeof(nasTcpBuffer),nasSortTcpBufByLastTime);
//
//	for (i = 0; i<iIndex; i++ ) {
//		if(i>48){
//			break;
//		}
//		ltTcpBuffer=&psTcp1[i];
//		if(ltTcpBuffer->lId<=0){
//			continue;
//		}
//		sprintf(caGroup,"%s","");          /* 部门           */
//    sprintf(caName,"%s","");
//		
//
//		sprintf(sSrv,"%s",ltService[ltTcpBuffer->cService].srvName);
//
//		/*根据ip得到用户信息*/
//		xHashLink=NULL;    
//		xHashLink=(shm_hash_link_user *)_gnm_hash_lookup_user(_ltPubInfo, ltTcpBuffer->lSip);
//		if(xHashLink!=NULL){
//		    	/*该用户已经在线*/
//		    	ltUserInfo=&xHashLink->item;
//					sprintf(caGroup,"%s",ltUserInfo->caGroup);    
//    			sprintf(caName,"%s",ltUserInfo->caName);
//    			if(group!=-1){
//						if(ltUserInfo->lGid!=group){
//							continue;
//						}
//					}
//    			
//		}else{
//					sprintf(caGroup,"%s","Unknown");    
//    			sprintf(caName,"%s","Unknown");
//		
//		}
//		
//		memset(caCon,0,sizeof(caCon));
//  
//    if( (ltTcpBuffer->cService==NAS_SERVICE_HTTP || ltTcpBuffer->cService==NAS_SERVICE_HTTPSRV||  (ltTcpBuffer->cService<106 && ltTcpBuffer->cService>99) ) && ltTcpBuffer->keyword1[0] ){
//    	sprintf(caCon,"<a href=\"http://%s%s\">http://%s%s</a>", ltTcpBuffer->keyword1,ltTcpBuffer->keyword2,ltTcpBuffer->keyword1,ltTcpBuffer->keyword2);
//    }else if(ltTcpBuffer->cService==NAS_SERVICE_QQ && ltTcpBuffer->keyword2[0] ){
//    	if(ltTcpBuffer->keyword3[0]=='1'){
//    		sprintf(caCon,"ID:%s LogOn", ltTcpBuffer->keyword2);
//    	}else if(ltTcpBuffer->keyword3[0]=='2'){
//    		sprintf(caCon,"ID:%s LogOff", ltTcpBuffer->keyword2);
//    	}else if(ltTcpBuffer->keyword3[0]=='3'){
//    		sprintf(caCon,"ID:%s SendMsg", ltTcpBuffer->keyword2);
//    	}else if(ltTcpBuffer->keyword3[0]=='4'){
//    		sprintf(caCon,"ID:%s SendMsg by proxy", ltTcpBuffer->keyword2);
//    	}
//    	
//    }else if(ltTcpBuffer->cService==NAS_SERVICE_POP3){
//    	sprintf(caCon,"USER:%s  ", ltTcpBuffer->keyword2);
//    }else if(ltTcpBuffer->cService==NAS_SERVICE_SMTP){
//    	sprintf(caCon,"FROM:%s  To:%s", ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//    }
//    else if(ltTcpBuffer->cService==NAS_SERVICE_FTP){
//    	sprintf(caCon,"USER:%s  %s ", ltTcpBuffer->keyword1,ltTcpBuffer->keyword2);
//    }else if(	ltTcpBuffer->cService==NAS_SERVICE_HaoFang || 
//    					ltTcpBuffer->cService==NAS_SERVICE_BF4G || ltTcpBuffer->cService==NAS_SERVICE_BF5Z ||
//    					ltTcpBuffer->cService==NAS_SERVICE_BFZGXQ   ){
//    	if(ltTcpBuffer->keyword2[0]){
//    		sprintf(caCon,"USER:%s ", ltTcpBuffer->keyword2);
//      }
//    }else if(	ltTcpBuffer->cService==NAS_SERVICE_MSN ){
//    	
//    	if(ltTcpBuffer->keyword4>0){
//    		if(ltTcpBuffer->keyword4==1){
//    			sprintf(caCon,"%s-%s %s","文字聊天",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//    	  }else if(ltTcpBuffer->keyword4==2){
//    			sprintf(caCon,"%s-%s %s %s","文件传输",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1,ltTcpBuffer->keyword3);
//    	  }else if(ltTcpBuffer->keyword4==3){
//    			sprintf(caCon,"%s-%s %s","应用共享",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//    	  }else if(ltTcpBuffer->keyword4==4){
//    			sprintf(caCon,"%s-%s %s","远程协助",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//    	  }else if(ltTcpBuffer->keyword4==5){
//    			sprintf(caCon,"%s-%s %s","音视频",ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//    	  }
//    	}
//    	sprintf(caCon,"%s-%s ", ltTcpBuffer->keyword2,ltTcpBuffer->keyword1);
//      
//    }
//    /*
//    1>Content-Type: text/plain                    文本消息
//conent-type:text/x-msmsgsinvite             音频连接
//2>文件传输： 5D3E02AB-6190-11d3-BBBB-00C04F795683
//3> 应用共享：F1B1920C-6A3C-4ce7-B18C-AFAB305FD03D 
//4〉远程协助：56b994a7-380f-410b-9985-c809d78c1bdc
//5〉音视频：  02D3C01F-BF30-4825-A83A-DE7AF41648AA*/
//
//		s.s_addr = htonl(ltTcpBuffer->lSip);
//		d.s_addr = htonl(ltTcpBuffer->lDip);
//		sprintf(sSip,"%s",inet_ntoa(s));
//		sprintf(sDip,"%s",inet_ntoa(d));
//		nasTimeFormat(dTime,ltTcpBuffer->lLastTime);
//		sprintf(dPort,"%d",ltTcpBuffer->nDport);
//		sprintf(sPort,"%d",ltTcpBuffer->nSport);
// 		
//		sprintf(sByte0,"%llu",ltTcpBuffer->lBytes[0]);
//		sprintf(sByte1,"%llu",ltTcpBuffer->lBytes[1]);
//		if(ltTcpBuffer->cType==6){
//			sprintf(caProt,"%s","TCP");
//		}else if(ltTcpBuffer->cType==17){
//			sprintf(caProt,"%s","UDP");
//		}else{
//			sprintf(caProt,"%s","???");
//		}
//		if(ltTcpBuffer->cDirect==1){
//			sprintf(strConn," %s %s:%s<=%s:%s ",caProt,sSip,sPort,sDip,dPort);
//			
//		}else {
//			sprintf(strConn," %s %s:%s=>%s:%s ",caProt,sSip,sPort,sDip,dPort);		
//		}
//
//		
//		sprintf(qosChannel,"%s","No QOS");
//		sprintf(cMatchLevel,"%s","系统级别");
//		sprintf(cUrlSort,"%s","????");
//		if(ltTcpBuffer->cMatchLevel==1){
//			sprintf(cMatchLevel,"%s","部门级别");
//		}else if(ltTcpBuffer->cMatchLevel==2){
//			sprintf(cMatchLevel,"%s","策略级别");
//		}else if(ltTcpBuffer->cMatchLevel==3){
//			sprintf(cMatchLevel,"%s","用户级别");
//		}else if(ltTcpBuffer->cMatchLevel==4){
//			sprintf(cMatchLevel,"%s","IP级别");
//		}
//
//		ltTcpBuffer->cUrlSort[4]='\0';
//		//printf("ltTcpBuffer->cUrlSort: %d-%d-%d-%d \n",ltTcpBuffer->cUrlSort[0],ltTcpBuffer->cUrlSort[1],ltTcpBuffer->cUrlSort[2],ltTcpBuffer->cUrlSort[3]);
//		
//		sprintf(cUrlSort,"%s",_ltPubInfo->urlSortName[ltTcpBuffer->cUrlSort[0]].sortname );
//
//		if(ltTcpBuffer->qosChannel	< (NAS_MAX_QOSSTATIC_CHANEL+_ltPubInfo->maxonlineuser+1) ){	
//				sprintf(sqlBuf,"select qosname from nasqosrun where cid=%d ",ltTcpBuffer->qosChannel);
//				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
//				if(tempCursor!=NULL){
//					tempRow= ltDbFetchRow(tempCursor);
//					if(tempRow!=NULL){
//						sprintf(qosChannel,"%s",tempRow[0]);
//					}
//					ltDbCloseCursor(tempCursor);
//				}
//		}
//		
//
//		
//   p=strcat(p,"<tr>");
//		
//		p=strcat(p,"<td  nowrap width=\"9%\" valign=\"bottom\"><div align=\"left\">");
//		p=strcat(p,caName);
//		p=strcat(p,"</font></div></td>");
//		
//		if(srvid!=5){
//			if(showtype==1 || showtype==0){/*nomal*/
//					p=strcat(p,"<td nowrap width=\"30%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,strConn);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"12%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sSrv);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte1);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte0);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td  nowrap width=\"15%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,dTime);
//					p=strcat(p,"</div></td>");
//			    p=strcat(p,"<td nowrap width=\"40%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,caCon);
//					p=strcat(p,"</div></td></tr>");
//			}else if(showtype==2){/*block*/
//					p=strcat(p,"<td nowrap width=\"30%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,strConn);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"12%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sSrv);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td  nowrap width=\"15%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,dTime);
//					p=strcat(p,"</div></td>");
//			    p=strcat(p,"<td nowrap width=\"40%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,cMatchLevel);
//					p=strcat(p,"</div></td></tr>");
//		  }else if(showtype==3){/*qos*/
//					p=strcat(p,"<td nowrap width=\"30%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,strConn);
//					p=strcat(p,"[");
//					p=strcat(p,qosChannel);
//					p=strcat(p,"]");
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"12%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sSrv);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte1);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte0);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td  nowrap width=\"15%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,dTime);
//					p=strcat(p,"</div></td>");
//			    p=strcat(p,"<td nowrap width=\"40%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,caCon);
//					p=strcat(p,"</div></td></tr>");
//			}
//			
//		}else{
//			
//					p=strcat(p,"<td nowrap width=\"30%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,caCon);
//					p=strcat(p,"</div></td>");
//					
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte1);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td nowrap width=\"7%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,sByte0);
//					p=strcat(p,"</div></td>");
//					p=strcat(p,"<td  nowrap width=\"15%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,dTime);
//					p=strcat(p,"</div></td>");
//			    p=strcat(p,"<td nowrap width=\"40%\" valign=\"bottom\"><div align=\"left\">");
//					p=strcat(p,cUrlSort);
//					p=strcat(p,"</div></td></tr>");
//				
//		
//	  }
//	
//   
//  }
//        
//	p=strcat(p,"</tbody></body></html>");
//	ltMsgPk->msgpktype=1;
//	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,xPlt);
//	free(psTcp1);
//	ltMsgFree(ltMsgPk);
//	
//	return 0;
//}




static int nasSortUserBufBy1(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp1->lStartTime - psTcp2->lStartTime);
}
static int nasSortUserBufBy2(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lStartTime - psTcp1->lStartTime);
}
static int nasSortUserBufBy3(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp1->lLastTime - psTcp2->lLastTime);
}
static int nasSortUserBufBy4(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lLastTime - psTcp1->lLastTime);
}
static int nasSortUserBufBy5(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lBytesC[0] - psTcp1->lBytesC[0]);
}
static int nasSortUserBufBy6(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lBytesC[1] - psTcp1->lBytesC[1]);
}

static int nasSortUserBufBy7(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lContime - psTcp1->lContime);
}
static int nasSortUserBufBy8(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->nowmaxpackage - psTcp1->nowmaxpackage);
}
static int nasSortUserBufBy9(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->nowmaxbytes - psTcp1->nowmaxbytes);
}
static int nasSortUserBufBy10(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return (psTcp2->lIp - psTcp1->lIp);
}
static int nasSortUserBufBy11(const void *p1,const void *p2)
{
    nasUserInfo *psTcp1,*psTcp2;
    psTcp1 = (nasUserInfo *)p1;
    psTcp2 = (nasUserInfo *)p2;    
    return strcmp(psTcp2->caName ,psTcp1->caName);
}

/* 用户当前状态   */
int ltshowwebonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

  ltDbHeadPtr dbPtr;

	char *caCurpg;
  char *caSear;
  char *caSrv;
  int  srvid;
  int  sortby;
	char caTotlePage[20],sCurPage[20];
	long lCurPage;
	long lRowNum,lStartRec,lSumRec,lCount,lNum;
	
	char caAllCount[24];
  int  i,indexWalker;
	int  orderby;
	int  tempmin,mymin;
	nasUserInfo  *psUserInfo;

	int          nowOnlineUser;

	
 
  shm_hash_link_user *_gnm_hash_user= NULL;
  shm_hash_link_user *tmpHashLink= NULL;	


	int  groupid;
  char caIp[20];
	char caMac[25];
	char caMacS[24];
	char caSortBy[24];
	char caorderby[24];
	char canewcon[1024];
  struct in_addr s;
  
  
  char  *p=(char *)_ltPubInfo;
	_gnm_hash_user=(shm_hash_link_user *)(p+sizeof(nasPubInfo));
	
	srvid=-1;
	orderby=0;
	groupid=-1;
	sortby=0;
  memset(caSortBy,0,sizeof(caSortBy));
	memset(canewcon,0,sizeof(canewcon));
	
  if(checkRight(confd,ltMsgPk,2,lt_MMHead)==-1){
		return -1;
	}
  

	caCurpg=ltMsgGetVar_s(ltMsgPk,"CurPg");
	caSear=ltMsgGetVar_s(ltMsgPk,"cond");
	if(!caSear){
		caSear="";
	}
	
	code_convert("utf-8","gb2312",caSear,strlen(caSear),canewcon,1024);
	caSear=canewcon;
	
	caSrv=ltMsgGetVar_s(ltMsgPk,"srvid");
	if(caSrv){
		srvid=atol(caSrv);
	}
	if(ltMsgGetVar_s(ltMsgPk,"group")!=NULL){
		groupid=atol(ltMsgGetVar_s(ltMsgPk,"group"));
	}
	

	
	if(ltMsgGetVar_s(ltMsgPk,"sortby")!=NULL){
		sortby=atol(ltMsgGetVar_s(ltMsgPk,"sortby"));
	}
	


	if(ltMsgGetVar_s(ltMsgPk,"orderby")!=NULL){

		orderby=atol(ltMsgGetVar_s(ltMsgPk,"orderby"));

	}

	
	if( ltStrIsSpaces(caCurpg) || !ltStrIsSpaces(caSear)) {
        	lCurPage = 1;
  }else {
       		lCurPage = atol(caCurpg);
  }
	lRowNum=_ltPubInfo->userofpage;
  lSumRec=0;/*总在线用户数*/
	lCount=0;
	lStartRec=(lCurPage-1)*lRowNum;
	lNum=0;

  mymin=_ltPubInfo->nasGeneralMin;
  tempmin=mymin%NAS_MAX_HOURMIN;
	lSumRec=0;


	nowOnlineUser=_ltPubInfo->maxonlineusernow;
	
	 for (i = 0; i < _ltPubInfo->maxonlineuser*NAS_HASHSPARE; i=i+NAS_HASHSPARE) {
      for (indexWalker=i,tmpHashLink = &_gnm_hash_user[i]; tmpHashLink!=NULL; indexWalker = tmpHashLink->next,tmpHashLink = &_gnm_hash_user[indexWalker]) {
		      
				 if(tmpHashLink->flag==0 || tmpHashLink->flag==4 ){
						if( tmpHashLink->next==-1){
							break;
						}
						continue;
					}
					if (indexWalker == tmpHashLink->next){
						break;
					}
					psUserInfo=&tmpHashLink->item;
					s.s_addr = htonl(psUserInfo->lIp);
			    sprintf(caIp,"%s",inet_ntoa(s));
			    memset(caMacS,0,sizeof(caMacS));
			    nasCvtMac(psUserInfo->mac,caMacS);
	        sprintf(caMac,"%s",caMacS);
	        
	        if(groupid!=-1){
			    	if(psUserInfo->lGid!=groupid){
			      	goto mycontinue;
			    	}	
			    }
	        
	        if(sortby==1){
	        	if(psUserInfo->natflag!=1){
								goto mycontinue;
			      }
	        }
	        if(sortby==2){
	        	if(psUserInfo->natflag!=2){
								goto mycontinue;
			      }
	        }
	        
	        if(sortby==3){/*p2p*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[14]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[15]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[16]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[17]==0 ){
								goto mycontinue;
			      }
	        }
	        if(sortby==4){/*网络电视人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[20]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[21]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[22]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[23]==0 ){
								goto mycontinue;
			      }
	        }
	        if(sortby==5){/*游戏*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[18]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[19]==0 ){
								goto mycontinue;
			      }
	        }
	        if(sortby==6){/*股票*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[10]==0){
								goto mycontinue;
			      }
	        }
	        if(sortby==7){/*邮件收发人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[7]==0){
								goto mycontinue;
			      }
	        }
	         if(sortby==8){/*即时通信人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[12]==0){
								goto mycontinue;
			      }
	        }
	        if(sortby==9){/*web*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[2]==0){
								goto mycontinue;
			      }
	        }
	        if(sortby==10){/*o*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[0]==0){
								goto mycontinue;
			      }
	        }
	        
	        if(srvid>=0 && srvid<NAS_MAX_SYSSRVNUM){
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[srvid]<1){
	        		goto mycontinue;
	        	}
	        }
	        
					if( ltStrIsSpaces(caSear) || strstr(psUserInfo->caName,caSear) ||
	             	 strstr(psUserInfo->caGroup,caSear)||
				         strstr(caIp,caSear) || strstr(caMac,caSear) ) {/*符合条件*/
										lSumRec++;
		      }
					
mycontinue:
				
				  if( tmpHashLink->next==-1){
							break;
					}
		 }
		 if(lSumRec>nowOnlineUser || lSumRec==nowOnlineUser){
				break;
		 }
	}
	

  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	
	/*全局变量*/
	sprintf(caTotlePage,"%ld",lSumRec/lRowNum+1);
	sprintf(sCurPage,"%ld",lCurPage);
	sprintf(caAllCount,"%ld",lSumRec);
	sprintf(caSortBy,"%d",sortby);
	sprintf(caorderby,"%d",orderby);
	lt_dbput_rootvars(dbPtr,6,"CurPg",sCurPage,"sortby",caSortBy,"orderby",caorderby,"TotPg",caTotlePage,"cond",caSear,"allline",caAllCount);

  sprintf(caorderby,"sel%d",orderby);
  lt_dbput_rootvars(dbPtr,1,caorderby,"selected");
  
   sprintf(caSortBy,"sort%d",sortby);
  lt_dbput_rootvars(dbPtr,1,caSortBy,"selected");
  
  sprintf(caSortBy,"%d",srvid);
  lt_dbput_rootvars(dbPtr,1,"srvid",caSortBy);
  
  sprintf(caSortBy,"%d",groupid);
  lt_dbput_rootvars(dbPtr,1,"groupid",caSortBy);

	ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/userlistinfo.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

   
}


//
//int ltwebonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
//{
//
//
//	nasUserInfo  *psUserInfo;
//	nasUserInfo  *psUserInfoSort;
//	int          nowOnlineUser;
//	int      		 iIndex;
//	char *caSrv;
//  int  srvid;
//  int i;
//	struct in_addr s;
//  shm_hash_link_user *tmpHashLink= NULL;	
//	char sTime[40];
//  char dTime[40];
//  
//	char *caCurpg;
//  char *caSear;
//
//	long lCurPage;
//	long lRowNum,lStartRec,lSumRec,lCount,lNum;
//	char caIp[20];
//	char caMac[25];
//  char caFlag[20];
//	char caMacS[24];
//	char canewcon[1024];
//  int  indexWalker;
//	int  orderby;
//	shm_hash_link_user *_gnm_hash_user= NULL;
//	int  groupid;
//	int  mymin,tempmin;
//	int  tmpmon;
//  
//  char caHead[150000];
//  char caTmpp[2048];
//  memset(  caHead,0,sizeof(caHead)); 
//  char  *pppp;
//  int   netfx;
//  int   sortby;
//  sortby=0;
//  
//  memset(canewcon,0,sizeof(canewcon));
//  
//  char  *p=(char *)_ltPubInfo;
//	_gnm_hash_user=(shm_hash_link_user *)(p+sizeof(nasPubInfo));
//	
//	orderby=0;
//	groupid=-1;
//	
//	
//  if(checkRight(confd,ltMsgPk,2,lt_MMHead)==-1){
//		return -1;
//	}
//  
//  if(ltMsgGetVar_s(ltMsgPk,"sortby")!=NULL){
//		sortby=atol(ltMsgGetVar_s(ltMsgPk,"sortby"));
//	}
//	caCurpg=ltMsgGetVar_s(ltMsgPk,"CurPg");
//	
//	caSear=ltMsgGetVar_s(ltMsgPk,"cond");
//	
//	code_convert("utf-8","gb2312",caSear,strlen(caSear),canewcon,1024);
//	caSear=canewcon;
//	
//	if(ltMsgGetVar_s(ltMsgPk,"group")!=NULL){
//		groupid=atol(ltMsgGetVar_s(ltMsgPk,"group"));
//	}
//	
//	srvid=-1;
//	caSrv=ltMsgGetVar_s(ltMsgPk,"srvid");
//	if(caSrv){
//		srvid=atol(caSrv);
//	}
//	
//	if(ltMsgGetVar_s(ltMsgPk,"orderby")!=NULL){
//
//		orderby=atol(ltMsgGetVar_s(ltMsgPk,"orderby"));
//
//	}
//
//	
//	if( ltStrIsSpaces(caCurpg) || !ltStrIsSpaces(caSear)) {
//        	lCurPage = 1;
//  }else {
//       		lCurPage = atol(caCurpg);
//  }
//	lRowNum=_ltPubInfo->userofpage;
//  lSumRec=0;/*总在线用户数*/
//	lCount=0;
//	lStartRec=(lCurPage-1)*lRowNum;
//	lNum=0;
//
//	mymin=_ltPubInfo->nasGeneralMin;
//	tempmin=mymin%NAS_MAX_HOURMIN;
//	
//	
//	nowOnlineUser=_ltPubInfo->maxonlineusernow;
//
//	psUserInfoSort=(nasUserInfo  *)malloc( sizeof(nasUserInfo)*(nowOnlineUser+1));
//  if(psUserInfoSort){
//  	memset(psUserInfoSort,0,sizeof(nasUserInfo)*(nowOnlineUser+1));
//  }
//	iIndex=0;
//	
//	 for (i = 0; i < _ltPubInfo->maxonlineuser*NAS_HASHSPARE; i=i+NAS_HASHSPARE) {
//      for (indexWalker=i,tmpHashLink = &_gnm_hash_user[i]; tmpHashLink!=NULL; indexWalker = tmpHashLink->next,tmpHashLink = &_gnm_hash_user[indexWalker]) {
//		      
//				 if(tmpHashLink->flag==0 || tmpHashLink->flag==4 ){
//						if( tmpHashLink->next==-1){
//							break;
//						}
//						continue;
//					}
//					if (indexWalker == tmpHashLink->next){
//						break;
//					}
//					psUserInfo=&tmpHashLink->item;
//					s.s_addr = htonl(psUserInfo->lIp);
//			    sprintf(caIp,"%s",inet_ntoa(s));
//			    memset(caMacS,0,sizeof(caMacS));
//			    nasCvtMac(psUserInfo->mac,caMacS);
//	        sprintf(caMac,"%s",caMacS);
//	        if(sortby==1){
//	        	if(psUserInfo->natflag!=1){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==2){
//	        	if(psUserInfo->natflag!=2){
//								goto mycontinueuser;
//			      }
//	        }
//	        
//	        if(sortby==3){/*p2p*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[14]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[15]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[16]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[17]==0 ){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==4){/*网络电视人员*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[20]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[21]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[22]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[23]==0 ){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==5){/*游戏*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[18]==0 &&
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[19]==0 ){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==6){/*股票*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[10]==0){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==7){/*邮件收发人员*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[7]==0){
//								goto mycontinueuser;
//			      }
//	        }
//	         if(sortby==8){/*即时通信人员*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[12]==0){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==9){/*web*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[2]==0){
//								goto mycontinueuser;
//			      }
//	        }
//	        if(sortby==10){/*o*/
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[0]==0){
//								goto mycontinueuser;
//			      }
//	        }
//	        
//	        if(srvid>=0 && srvid<NAS_MAX_SYSSRVNUM){
//	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[srvid]<1){
//	        		goto mycontinueuser;
//	        	}
//	        }
//	        
//					if( ltStrIsSpaces(caSear) || strstr(psUserInfo->caName,caSear) ||
//	             		strstr(psUserInfo->caGroup,caSear)||
//				          strstr(caIp,caSear) || strstr(caMac,caSear) ) {/*符合条件*/
//				      psUserInfoSort[iIndex].nowmaxbytes=0;   
//							memcpy(&psUserInfoSort[iIndex],psUserInfo,sizeof(nasUserInfo));
//							psUserInfoSort[iIndex].nowmaxpackage=psUserInfoSort[iIndex].nowmaxpackage/(psUserInfoSort[iIndex].lLastTime%60+5);
//							psUserInfoSort[iIndex].nowmaxbytes=psUserInfoSort[iIndex].nowmaxbytes/(psUserInfoSort[iIndex].lLastTime%60+5);
//							iIndex++;
//							if(iIndex>nowOnlineUser || iIndex==nowOnlineUser){
//								break;
//						  }
//				  }
//mycontinueuser:
//				  if( tmpHashLink->next==-1){
//							break;
//					}
//		 }
//		 if(iIndex>nowOnlineUser || iIndex==nowOnlineUser){
//				break;
//		 }
//	}
//	nowOnlineUser=iIndex;
//
//	
//  if(orderby==1){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy1);
//	}else if(orderby==2){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy2);
//	}else if(orderby==3){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy3);
//	}else if(orderby==4){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy4);
//	}else if(orderby==5){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy5);
//	}else if(orderby==6){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy6);
//	}else if(orderby==7){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy7);
//	}else if(orderby==8){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy8);
//	}else if(orderby==9){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy9);
//	}else if(orderby==10){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy10);
//	}else if(orderby==11){
//  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy11);
//	}
//	   
//	//hid=(shm_hash_table_num *)lt_MMHead->mmHead->userHeadList[NAS_LNK_USERINFO];
//
//	
//	
//
//	
//	pppp=caHead;
//	sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",nowOnlineUser);
//  pppp=strcat(pppp,caTmpp);
//
//	for (i = 0; i < nowOnlineUser; i++) {
//		
//			psUserInfo=(nasUserInfo  *)&psUserInfoSort[i];
//			
//	        	
//		    if(groupid!=-1){
//		    	if(psUserInfo->lGid!=groupid){
//		      	continue;
//		    	}	
//		    }
//	      s.s_addr = htonl(psUserInfo->lIp);
//			  sprintf(caIp,"%s",inet_ntoa(s));
//			  memset(caMacS,0,sizeof(caMacS));
//			  nasCvtMac(psUserInfo->mac,caMacS);
//	      sprintf(caMac,"%s",caMacS);
//				lSumRec++;
//				lCount++;
//				if( lCount>=lStartRec && lCount<(lStartRec+lRowNum)){/*本页内容 */
//					lNum++;
//					netfx=0;
//
//					memset(caTmpp,0,sizeof(caTmpp));
//					memset(sTime,0,sizeof(sTime));
//					memset(dTime,0,sizeof(dTime));
//					
//					nasTimeFormat(dTime,psUserInfo->lStartTime);
//					
//					sprintf(sTime,"%d",psUserInfo->lContime/3600);
//					tmpmon=psUserInfo->lContime%3600;
//					sprintf(sTime,"%s:%d",sTime,tmpmon/60);
//					sprintf(sTime,"%s:%d",sTime,tmpmon%60);
//					
//					if(_ltPubInfo->defaultlan==2){/*en*/
//						if(psUserInfo->cUseflags==10){
//							sprintf(caFlag,"%s","No Access");
//						}else if(psUserInfo->cUseflags==9){
//							sprintf(caFlag,"%s","No Monitor");
//						}else{
//							sprintf(caFlag,"%s","Normal");
//						}
//					}else{
//						if(psUserInfo->cUseflags==10){
//							sprintf(caFlag,"%s","禁止");
//						}else if(psUserInfo->cUseflags==9){
//							sprintf(caFlag,"%s","免监控");
//						}else{
//							sprintf(caFlag,"%s","正常");
//						}
//					}
//					if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[14]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[15]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[16]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[17]>0 ){
//								netfx++;
//			    }
//			    if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[20]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[21]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[22]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[23]>0 ){
//								netfx++;
//			    }
//			    if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[18]>0 ||
//			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[19]>0 ){
//								netfx++;
//			    }
//			    if(psUserInfo->nowmaxbytes > (_ltPubInfo->lpFlowPerSec[0]+_ltPubInfo->lpFlowPerSec[1])*30 ){
//								netfx++;
//			    }
//			    if(psUserInfo->natflag==1){
//								netfx++;
//			    }
//			    
//
//					if(lNum==1){
//						sprintf(caTmpp,"{\"uid\":\"%lu\",\"uname\":\"%s\",\"dep\":\"%s\",\"ipadd\":\"%s\",\"macadd\":\"%s\",\"tcpnum\":\"%lu\",\"udpnum\":\"%lu\",\"ub\":\"%llu\",\"db\":\"%llu\",\"ontime\":\"%s\",\"status\":\"%s\",\"ps\":\"%lu\",\"bs\":\"%llu\",\"netfx\":\"%d\",\"sTime\":\"%s\"}",
//						  psUserInfo->lId,psUserInfo->caName,psUserInfo->caGroup,caIp,caMac,
//						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].tcpconnect,
//						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].udpconnect,
//						  (unsigned long long)(psUserInfo->lBytesC[0]/1024),
//						  (unsigned long long)(psUserInfo->lBytesC[1]/1024),
//						  sTime,caFlag,psUserInfo->nowmaxpackage,psUserInfo->nowmaxbytes/1024,netfx, dTime);
//						  
//						  pppp=strcat(pppp,caTmpp);
//						
//					}else{
//					    sprintf(caTmpp,",{\"uid\":\"%lu\",\"uname\":\"%s\",\"dep\":\"%s\",\"ipadd\":\"%s\",\"macadd\":\"%s\",\"tcpnum\":\"%lu\",\"udpnum\":\"%lu\",\"ub\":\"%llu\",\"db\":\"%llu\",\"ontime\":\"%s\",\"status\":\"%s\",\"ps\":\"%lu\",\"bs\":\"%llu\",\"netfx\":\"%d\",\"sTime\":\"%s\"}",
//						  psUserInfo->lId,psUserInfo->caName,psUserInfo->caGroup,caIp,caMac,
//						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].tcpconnect,
//						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].udpconnect,
//						  (unsigned long long)(psUserInfo->lBytesC[0]/1024),
//						  (unsigned long long)(psUserInfo->lBytesC[1]/1024),
//						  sTime,caFlag,psUserInfo->nowmaxpackage,psUserInfo->nowmaxbytes/1024,netfx,dTime);
//						  
//						  pppp=strcat(pppp,caTmpp);
//				  }
//					
//				}
//			  
//			
//		}
//	pppp=strcat(pppp,"]}");
//	
//  ltMsgPk->msgpktype=3;  
//  sprintf(caTmpp,"Content-Length: %d\nContent-type: text/html; charset=gb2312\n",strlen(caHead));
//	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,caTmpp,"lthtml",LT_TYPE_STRING,caHead);
//
//	ltMsgFree(ltMsgPk);
//
//	return 0;
//
//   
//}



int ltwebonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

	nasUserInfo  *psUserInfo;
	nasUserInfo  *psUserInfoSort;
	int          nowOnlineUser;
	int      		 iIndex;
	char *caSrv;
  int  srvid;
  int i;
	struct in_addr s;
  shm_hash_link_user *tmpHashLink= NULL;	
	char sTime[40];
  char dTime[40];
  
	char *caCurpg;
  char *caSear;

	long lCurPage;
	long lRowNum,lStartRec,lSumRec,lCount,lNum;
	char caIp[20];
	char caMac[25];
  char caFlag[20];
	char caMacS[24];
	char canewcon[1024];
  int  indexWalker;
	int  orderby;
	shm_hash_link_user *_gnm_hash_user= NULL;
	int  groupid;
	int  mymin,tempmin;
	int  tmpmon;
  
  char caHead[100000];
  char caTmpp[2048];
  memset(  caHead,0,sizeof(caHead)); 
  char  *pppp;
  int   netfx;
  int   sortby;
  sortby=0;
  
  memset(canewcon,0,sizeof(canewcon));
  
  char  *p=(char *)_ltPubInfo;
	_gnm_hash_user=(shm_hash_link_user *)(p+sizeof(nasPubInfo));
	
	orderby=0;
	groupid=-1;
	
	
  if(checkRight(confd,ltMsgPk,2,lt_MMHead)==-1){
		return -1;
	}
  
  if(ltMsgGetVar_s(ltMsgPk,"sortby")!=NULL){
		sortby=atol(ltMsgGetVar_s(ltMsgPk,"sortby"));
	}
	caCurpg=ltMsgGetVar_s(ltMsgPk,"CurPg");
	
	caSear=ltMsgGetVar_s(ltMsgPk,"cond");
	
	code_convert("utf-8","gb2312",caSear,strlen(caSear),canewcon,1024);
	caSear=canewcon;
	
	if(ltMsgGetVar_s(ltMsgPk,"group")!=NULL){
		groupid=atol(ltMsgGetVar_s(ltMsgPk,"group"));
	}
	
	srvid=-1;
	caSrv=ltMsgGetVar_s(ltMsgPk,"srvid");
	if(caSrv){
		srvid=atol(caSrv);
	}
	
	if(ltMsgGetVar_s(ltMsgPk,"orderby")!=NULL){

		orderby=atol(ltMsgGetVar_s(ltMsgPk,"orderby"));

	}

	
	if( ltStrIsSpaces(caCurpg) || !ltStrIsSpaces(caSear)) {
        	lCurPage = 1;
  }else {
       		lCurPage = atol(caCurpg);
  }
	lRowNum=_ltPubInfo->userofpage;
  lSumRec=0;/*总在线用户数*/
	lCount=0;
	lStartRec=(lCurPage-1)*lRowNum;
	lNum=0;

	mymin=_ltPubInfo->nasGeneralMin;
	tempmin=mymin%NAS_MAX_HOURMIN;
	
	
	nowOnlineUser=_ltPubInfo->maxonlineusernow;

	psUserInfoSort=(nasUserInfo  *)malloc( sizeof(nasUserInfo)*(nowOnlineUser+1));
  if(psUserInfoSort){
  	memset(psUserInfoSort,0,sizeof(nasUserInfo)*(nowOnlineUser+1));
  }
	iIndex=0;
	
	 for (i = 0; i < _ltPubInfo->maxonlineuser*NAS_HASHSPARE; i=i+NAS_HASHSPARE) {
      for (indexWalker=i,tmpHashLink = &_gnm_hash_user[i]; tmpHashLink!=NULL; indexWalker = tmpHashLink->next,tmpHashLink = &_gnm_hash_user[indexWalker]) {
		      
				 if(tmpHashLink->flag==0 || tmpHashLink->flag==4 ){
						if( tmpHashLink->next==-1){
							break;
						}
						continue;
					}
					if (indexWalker == tmpHashLink->next){
						break;
					}
					psUserInfo=&tmpHashLink->item;
					s.s_addr = htonl(psUserInfo->lIp);
			    sprintf(caIp,"%s",inet_ntoa(s));
			    memset(caMacS,0,sizeof(caMacS));
			    nasCvtMac(psUserInfo->mac,caMacS);
	        sprintf(caMac,"%s",caMacS);
	        if(sortby==1){
	        	if(psUserInfo->natflag!=1){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==2){
	        	if(psUserInfo->natflag!=2){
								goto mycontinueuser;
			      }
	        }
	        
	        if(sortby==3){/*p2p*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[14]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[15]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[16]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[17]==0 ){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==4){/*网络电视人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[20]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[21]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[22]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[23]==0 ){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==5){/*游戏*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[18]==0 &&
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[19]==0 ){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==6){/*股票*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[10]==0){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==7){/*邮件收发人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[7]==0){
								goto mycontinueuser;
			      }
	        }
	         if(sortby==8){/*即时通信人员*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[12]==0){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==9){/*web*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[2]==0){
								goto mycontinueuser;
			      }
	        }
	        if(sortby==10){/*o*/
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[0]==0){
								goto mycontinueuser;
			      }
	        }
	        
	        if(srvid>=0 && srvid<NAS_MAX_SYSSRVNUM){
	        	if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[srvid]<1){
	        		goto mycontinueuser;
	        	}
	        }
	        
					if( ltStrIsSpaces(caSear) || strstr(psUserInfo->caName,caSear) ||
	             		strstr(psUserInfo->caGroup,caSear)||
				          strstr(caIp,caSear) || strstr(caMac,caSear) ) {/*符合条件*/
				      psUserInfoSort[iIndex].nowmaxbytes=0;   
							memcpy(&psUserInfoSort[iIndex],psUserInfo,sizeof(nasUserInfo));
							psUserInfoSort[iIndex].nowmaxpackage=psUserInfoSort[iIndex].nowmaxpackage/(psUserInfoSort[iIndex].lLastTime%60+5);
							psUserInfoSort[iIndex].nowmaxbytes=psUserInfoSort[iIndex].nowmaxbytes/(psUserInfoSort[iIndex].lLastTime%60+5);
							iIndex++;
							if(iIndex>nowOnlineUser || iIndex==nowOnlineUser){
								break;
						  }
				  }
mycontinueuser:
				  if( tmpHashLink->next==-1){
							break;
					}
		 }
		 if(iIndex>nowOnlineUser || iIndex==nowOnlineUser){
				break;
		 }
	}
	nowOnlineUser=iIndex;

	
  if(orderby==1){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy1);
	}else if(orderby==2){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy2);
	}else if(orderby==3){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy3);
	}else if(orderby==4){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy4);
	}else if(orderby==5){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy5);
	}else if(orderby==6){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy6);
	}else if(orderby==7){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy7);
	}else if(orderby==8){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy8);
	}else if(orderby==9){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy9);
	}else if(orderby==10){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy10);
	}else if(orderby==11){
  	qsort(psUserInfoSort,nowOnlineUser,sizeof(nasUserInfo),nasSortUserBufBy11);
	}
	   
	//hid=(shm_hash_table_num *)lt_MMHead->mmHead->userHeadList[NAS_LNK_USERINFO];

	
	

	pppp=caHead;
	pppp=strcat(pppp,"[");
	for (i = 0; i < nowOnlineUser; i++) {
		
			psUserInfo=(nasUserInfo  *)&psUserInfoSort[i];
			
	        	
		    if(groupid!=-1){
		    	if(psUserInfo->lGid!=groupid){
		      	continue;
		    	}	
		    }
	      s.s_addr = htonl(psUserInfo->lIp);
			  sprintf(caIp,"%s",inet_ntoa(s));
			  memset(caMacS,0,sizeof(caMacS));
			  nasCvtMac(psUserInfo->mac,caMacS);
	      sprintf(caMac,"%s",caMacS);
				lSumRec++;
				lCount++;
				if( lCount>=lStartRec && lCount<(lStartRec+lRowNum)){/*本页内容 */
					lNum++;
					netfx=0;

					memset(caTmpp,0,sizeof(caTmpp));
					memset(sTime,0,sizeof(sTime));
					memset(dTime,0,sizeof(dTime));
					
					nasTimeFormat(dTime,psUserInfo->lStartTime);
					
					sprintf(sTime,"%d",psUserInfo->lContime/3600);
					tmpmon=psUserInfo->lContime%3600;
					sprintf(sTime,"%s:%d",sTime,tmpmon/60);
					sprintf(sTime,"%s:%d",sTime,tmpmon%60);
					
					if(_ltPubInfo->defaultlan==2){/*en*/
						if(psUserInfo->cUseflags==10){
							sprintf(caFlag,"%s","No Access");
						}else if(psUserInfo->cUseflags==9){
							sprintf(caFlag,"%s","No Monitor");
						}else{
							sprintf(caFlag,"%s","Normal");
						}
					}else{
						if(psUserInfo->cUseflags==10){
							sprintf(caFlag,"%s","禁止");
						}else if(psUserInfo->cUseflags==9){
							sprintf(caFlag,"%s","免监控");
						}else{
							sprintf(caFlag,"%s","正常");
						}
					}
					if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[14]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[15]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[16]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[17]>0 ){
								netfx++;
			    }
			    if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[20]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[21]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[22]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[23]>0 ){
								netfx++;
			    }
			    if(_ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[18]>0 ||
			        _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].srvflow[19]>0 ){
								netfx++;
			    }
			    if(psUserInfo->nowmaxbytes > (_ltPubInfo->lpFlowPerSec[0]+_ltPubInfo->lpFlowPerSec[1])*30 ){
								netfx++;
			    }
			    if(psUserInfo->natflag==1){
								netfx++;
			    }
			    

					if(lNum==1){
						sprintf(caTmpp,"['%lu','%s','%s','%s','%s','%lu','%lu','%llu','%llu','%s','%s','%lu','%llu',%d,'%s','%lu']",
						  psUserInfo->lId,psUserInfo->caName,psUserInfo->caGroup,caIp,caMac,
						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].tcpconnect,
						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].udpconnect,
						  (unsigned long long)(psUserInfo->lBytesC[0]/1024),
						  (unsigned long long)(psUserInfo->lBytesC[1]/1024),
						  sTime,caFlag,psUserInfo->nowmaxpackage,psUserInfo->nowmaxbytes/1024,netfx, dTime,psUserInfo->lLimitTimes);
						  pppp=strcat(pppp,caTmpp);
					}else{
					    sprintf(caTmpp,",['%lu','%s','%s','%s','%s','%lu','%lu','%llu','%llu','%s','%s','%lu','%llu',%d,'%s','%lu']",
						  psUserInfo->lId,psUserInfo->caName,psUserInfo->caGroup,caIp,caMac,
						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].tcpconnect,
						  _ltHourUserFlow[psUserInfo->onlineseq].hourUserFlow[tempmin].udpconnect,
						  (unsigned long long)(psUserInfo->lBytesC[0]/1024),
						  (unsigned long long)(psUserInfo->lBytesC[1]/1024),
						  sTime,caFlag,psUserInfo->nowmaxpackage,psUserInfo->nowmaxbytes/1024,netfx,dTime,psUserInfo->lLimitTimes);
						  pppp=strcat(pppp,caTmpp);
				  }
				}
		}
		 pppp=strcat(pppp,"]");
	
  ltMsgPk->msgpktype=3;  
  sprintf(caTmpp,"Content-Length: %d\nContent-type: text/html; charset=gb2312\n",strlen(caHead));
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,caTmpp,"lthtml",LT_TYPE_STRING,caHead);

	ltMsgFree(ltMsgPk);

	return 0;

   
}


/* 用户当前状态   */
int ltwebonlinebip(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

  ltDbHeadPtr dbPtr;
  ltTablePtr tablePtr;
	nasControl *ltControl;
	char         dTime[24];
	int      		 i;
  struct in_addr s;
  char         caIp[24];
  
  char         bsys[24];
  char         btcp[24];
  char         budp[24];
  char         btime[24];
  char         bflow[24];
  char         bjf[24];
  char         bout[24];
  
  shm_hash_link_controlbuffer *tmpHashLink= NULL;	
	shm_hash_link_controlbuffer *_gnm_hash_controlbuffer;
  char  *p=(char *)_ltPubInfo;
	_gnm_hash_controlbuffer=(shm_hash_link_controlbuffer *)(p+sizeof(nasPubInfo)+_ltPubInfo->nasControlBufferPtr);
	
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");
	tablePtr=lt_dbput_table(dbPtr,"onlineuser");

  for (i = 0; i < _ltPubInfo->maxcontrolbuffer*(NAS_HASHSPARE+1); i=i+1) {
	
		tmpHashLink=&_gnm_hash_controlbuffer[i];
		if(tmpHashLink->flag==1  ){ 
			ltControl=&tmpHashLink->item;
			memset(dTime,0,sizeof(dTime));
			nasTimeFormat(dTime,ltControl->lLastTime);
			s.s_addr = htonl(ltControl->lSip);
			sprintf(caIp,"%s",inet_ntoa(s));

			
			memset(bsys,0,sizeof(bsys));
			memset(btcp,0,sizeof(btcp));
			memset(budp,0,sizeof(budp));
			memset(btime,0,sizeof(btime));
			memset(bflow,0,sizeof(bflow));
			memset(bjf,0,sizeof(bjf));
			memset(bout,0,sizeof(bout));
			
			if(ltControl->cFlag ==0){
				sprintf(bsys,"%s","checked");
			}
			if(( 1 & ltControl->cFlag )==1){
				sprintf(btcp,"%s","checked");
			}
			if((2 & ltControl->cFlag )==2){
				sprintf(budp,"%s","checked");
			}
			if((16 & ltControl->cFlag)==16){
				sprintf(btime,"%s","checked");
			}
			if((32 & ltControl->cFlag )==32){
				sprintf(bflow,"%s","checked");
			}
			if((64 & ltControl->cFlag )==64){
				sprintf(bjf,"%s","checked");
			}
			if((128 & ltControl->cFlag )==128){
				sprintf(bout,"%s","checked");
			}

			lt_dbput_recordvars(tablePtr,9,
					  "ip",LT_TYPE_STRING,caIp,
						"lasttime",LT_TYPE_STRING,dTime,
						"bsys",LT_TYPE_STRING,bsys,
						"btcp",LT_TYPE_STRING,btcp,
						"budp",LT_TYPE_STRING,budp,
						"btime",LT_TYPE_STRING,btime,
						"bflow",LT_TYPE_STRING,bflow,
						"bjf",LT_TYPE_STRING,bjf,
						"bout",LT_TYPE_STRING,bout
			); 
			
		}
		
	}
	
	lt_dbput_html_lang(confd,ltMsgPk,dbPtr->doc,"/app/ns/html/main/blockipstatus.htm",dbPtr->head,0);

	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

   
}



int lttesttimeout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char flowInfo[1000];
	int i;
	i=0;
	while(1){
		printf("time:%d\n",i);
		sleep(3);
		i++;
		if(i>50){
			break;
		}
	}
	
	
	sprintf(flowInfo,"%s","<html><body bgcolor=\"#cccccc\" >ok 300秒!<br>Soory, Please contract 13701941874 to active your system. </body></html>");
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,flowInfo);
	ltMsgFree(ltMsgPk);
	return 0;

}

