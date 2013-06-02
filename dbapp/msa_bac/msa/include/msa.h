#ifndef __MSADEF__
#define __MSADEF__


#include <netinet/ether.h>
#include "pubfun.h"


/*���Ų�μ���*/
#define _MAX_GROUPLEVEL   5
/*ȫ����������*/
#define _MAX_ALLGRPNUM     1000
#define _MAX_ALLPOLICY     128
#define _MAXSNMPNUM        7
#define _MAX_BIGAREANUM    100
#define _MAXSNMPNUM        7
#define _MAXNETAREANUM     1000
#define _MAXVPATHNUM       4
#define _MAX_QOSSTATIC_CHANEL    1000
#define NAS_MAXURLLIST_NUM       200000
#define NAS_MAXURLSORT_NUM       100
#define _MAX_SRVRULE_NUM    64

/*ϵͳ��������*/
#define NAS_MAX_COREFUNNUM    200
#define NAS_MAX_FUNNUM        200
#define NAS_DEFAULT_FUNTIME   200

/*����Ա����*/
#define MAX_ADMIN_SESSION            300
#define MAX_ADMIN_TIMEOUT            7200
#define MAX_ADMIN_TRYTIMEOUT         120
#define MAX_ADMIN_MAXRIGHT           100


/*�Զ����ַ���*/
#define MAX_SYSTEM_MSG               1000
/*�256*/
#define MAX_LANG_LIST_CONSIZE        256

#define MAX_SRVNAME_SIZE    32
#define MAX_TOPSRVNAME_SIZE    64
#define MAX_TOP_SRVNUM      35
#define MAX_SYS_SRVNUM      500
#define MAX_PORTNUM         65536
#define NAS_MAX_HOURMIN     5  

#define _pubconfdir   "/etc/msa/msa"
#define _datacenterdir   "/datacenter/msa"
#define _proccessdir   "/datacenter/proccess"
#define _TMPDIR   "/datacenter/tmp"

#define NAS_PROTOCOL_TCP 6    /*tcpЭ��*/
#define NAS_PROTOCOL_UDP 17   /*udpЭ��*/ 

#define MSA_SERVICE_UNKNOWN    0    /* ��������             */
#define MSA_SERVICE_HTTP       5    /* Web����              */
#define MSA_SERVICE_SMTP       22    /* SMTP����            */
#define MSA_SERVICE_POP3       19    /* POP����             */
#define MSA_SERVICE_HTTPSRV    34    /* ����Web����      */
#define MSA_SERVICE_SMTPSRV    37    /* ����SMTP����     */
#define MSA_SERVICE_POP3SRV    36    /* ����POP����      */

#define MSA_SERVICE_MSN        16    /* msn����          */
#define MSA_SERVICE_YAHOO      29    /* msn����          */
#define MSA_SERVICE_ICQ        1    /* msn����           */
#define MSA_SERVICE_FTP        3     /* ftp����          */
#define MSA_SERVICE_BT         33    /* BT����           */
#define MSA_SERVICE_QQ         28    /* QQ����           */
#define MSA_SERVICE_SMB        7    /* QQ����            */
#define MSA_SERVICE_EMULE      38    /* EMule����        */
#define MSA_SERVICE_PPLIVE     39    /* pplive����       */
#define MSA_SERVICE_QQLIVE     40    /* qqlive����       */

#define MSA_SERVICE_KaZaA      43    /* EMule����        */
#define MSA_SERVICE_Gnutella   44    /* Gnutella����       */
#define MSA_SERVICE_WinMX      45    /* WinMX����       */
#define MSA_SERVICE_XunLei     48    /* Ѹ������         */
#define MSA_SERVICE_HaoFang    49    /* �Ʒ���սƽ̨     */
#define MSA_SERVICE_Socks5     50    /* Socks5����       */
#define MSA_SERVICE_HTTPPROXY  51    /* http����         */
#define MSA_SERVICE_BF4G       32    /* �߷�4����ս      */
#define MSA_SERVICE_BF5Z       52    /* �߷�5�Ӵ�ս      */
#define MSA_SERVICE_BFZGXQ     53    /* �߷��й�����     */
#define MSA_SERVICE_BFXZ       54    /* �߷���Ϸ����     */
#define MSA_SERVICE_BAIDU_XB   55    /* �°�             */
#define MSA_SERVICE_Ares       56    /* Ares             */
#define MSA_SERVICE_SoulSeek   57    /* SoulSeek         */
#define MSA_SERVICE_appleJuice 58    /* appleJuice       */
#define MSA_SERVICE_DirectConnect 59 /* DirectConnect    */
#define MSA_SERVICE_xdcc       60    /* xdcc        */
#define MSA_SERVICE_waste      61    /* waste       */
#define MSA_SERVICE_qqcyclone  62    /* qqcyclone   */
#define MSA_SERVICE_PPStream   63    /* PPStream    */
#define MSA_SERVICE_UUSee      64    /* UUSee       */
#define MSA_SERVICE_feidian    65    /* feidian     */
#define MSA_SERVICE_POCO       66    /* POCO        */
#define MSA_SERVICE_QVOD       67    /* QVOD        */

#define MSA_SERVICE_POPO       71    /* popo        */
#define MSA_SERVICE_UC         72    /* UC          */
#define MSA_SERVICE_PPG        68    /* ƨƨ��      */
#define MSA_SERVICE_totolook   69    /* totolook    */
#define MSA_SERVICE_PPT        70    /* PP���ͨ    */

#define MSA_SERVICE_SINAGAME   79    /* sina��Ϸ    */
#define MSA_SERVICE_SOHUGAME   80    /* sohu��Ϸ    */

#define MSA_SERVICE_HTTPS      6     /* https    */
#define MSA_SERVICE_QQGAME     81    /* qq��Ϸ   */
#define MSA_SERVICE_UUCALL     97    /* uucall   */
#define MSA_SERVICE_SIP        98    /* SIPЭ��  */
#define MSA_SERVICE_RTP        99    /* RTP/RTCP */
#define MSA_SERVICE_IRC        11    /* IRC */
#define MSA_SERVICE_Telnet     25    /* telnet */
#define MSA_SERVICE_FeiXin     96    /* ����2008 */
#define MSA_SERVICE_sdo        93    /* ʢ�� */
#define MSA_SERVICE_BF         15    /* �߷� */
#define MSA_SERVICE_LZ         27    /* ���� */
#define MSA_SERVICE_Jabber     95    /* JABBER */


#define MSA_SERVICE_warcraft   90    /* warcraft */
#define MSA_SERVICE_cf         82    /* ��Խ���� */
#define MSA_SERVICE_dnf        83    /* ���³�����ʿ */
#define MSA_SERVICE_tx2        84    /* ����2 */
#define MSA_SERVICE_mir3       47    /* ����3 */
#define MSA_SERVICE_mirs       85    /* �����⴫ */
#define MSA_SERVICE_jx3        86    /* ������Ե3 */

#define MSA_SERVICE_popkart    89    /* ���ܿ����� */
#define MSA_SERVICE_bnb        91    /* ������ */
#define MSA_SERVICE_zt         87    /* ��; */
#define MSA_SERVICE_rxjh       92    /* ��Ѫ���� */

#define MSA_SERVICE_MSNPROXY   107   /* MSNPROXY */

/*ע�ắ����*/
typedef struct corefunList_s{
	char strFunName[64]; /*��������*/
  int (*op)();
} corefunList;



typedef struct funList_s{
	unsigned int lFunCode;  /*����*/
	unsigned int funFlag;   /*0 ֱ�ӵ��� 1 dll��̬���� 2java�� 3Ӧ�ó������*/
	char strNode[32]; /*����˵��*/
	char strFunName[64]; /*��������*/
	char strFunUrl[128]; /*dll���õ�url*/
	unsigned int  maxruntime;    /*�ú��������ʱ��*/
	unsigned int  activeflag;    /*��������� 0�뼤���޹� 1�뼤����� */
	unsigned int  rightflag;    /*Ȩ�������  0��Ȩ���޹� 1��Ȩ�����  */
  int (*op)();
} funList;

corefunList  _ltcorefunList[NAS_MAX_COREFUNNUM];
funList      _ltfunList[NAS_MAX_FUNNUM];
int          _maxfuntonnum;

typedef struct nasAdmSession_s {
	unsigned long  lSip;       /* ԴIP��ַ     */
	unsigned long  lSid;       /* session id  4λ����  */
	char           lright[MAX_ADMIN_MAXRIGHT+1];     /* Ȩ��       */
	char           caname[64];     /* Ȩ��       */
  char           allgroup[_MAX_ALLGRPNUM];   /* ��������   */
  unsigned int   trytimes;
	unsigned long  lasttime;   /*���һ�ε�½ʱ�� */
} nasAdmSession;

/*
0:chinese
1:en
*/
typedef struct conListType_s {
    char            conStr[MAX_LANG_LIST_CONSIZE];
} conListType;


typedef struct nasPortInfo_s {
    unsigned long  nPort;             /* �˿� */
    int		   udpLog;       /* ��udp�˿��Ƿ񱻼�¼  3��������� 2��ز���¼ 9������ 10 ��ֹ���� */
    int		   tcpLog;       /* ��tcp�˿��Ƿ񱻼�¼  3��������� 2��ز���¼ 9������ 10 ��ֹ����   */
    int      isTcplocal;      /* 1 local  0 remote*/
    int      isUdplocal;      /* 1 local  0 remote*/
    int		  srvTcpIn;     /* tcp inbound��service��*/
    int		  srvTcpOut;    /* tcp outbound��service��*/
    int		  srvUdpReceive;    /* udp Receive��service��*/
    int		  srvUdpSend;       /* udp Send��service��*/
} nasPortInfo;

typedef struct msaTopSrv_s {
	char          srvname[MAX_TOPSRVNAME_SIZE];
}msaTopSrv;

typedef struct msaService_s {
    char            srvName[MAX_SRVNAME_SIZE];     /*����*/
    unsigned char   srvFlag; 	       /*0 ϵͳ��1 �û��Խ�*/
    unsigned char   srvProt;         /* Э��6 tcp    17 udp */
    unsigned short  nPortL;          /* �˿ں�����    */
    unsigned short  nPortU;          /* �˿ں�����    */
    unsigned char   srvDirect;       /* 0:out  1:in   */
    unsigned char   topSrv;        
} msaService;

typedef struct nasFlowInfo_s {
    unsigned long long   srvflow[MAX_TOP_SRVNUM]; 
}nasFlowInfo;

typedef struct nasAllInfo_s {
	  unsigned long   udpconnect; 
    unsigned long   tcpconnect;
    nasFlowInfo  nowFlowInfo;
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
} nasAllInfo;

typedef struct nasVPathInfo_s { 
    nasFlowInfo  nowFlowInfo;  
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
} nasVPathInfo;


typedef struct nasQosPathInfo_s {   /*   max static qos    */ 
    uint16       sid;
    char         qosname[64];
    uint32       qosrated;
    uint32       qosrateu;
    uint32       qosmixd;
    uint32       qosmixu;
    nasFlowInfo  nowFlowInfo;  
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
} nasQosPathInfo;

typedef struct nasGroupInfo_s {  /*   maxgroup 1000    */   
	  unsigned short groupid;  
	  unsigned short pid; 
	  char         name[64];
	  char         mydesc[128]; 
	  unsigned char level;
    nasFlowInfo  nowFlowInfo;  
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
} nasGroupInfo;

typedef struct nasPolicyInfo_s {   /*   maxpolicy    */ 
	  unsigned int pid;
	  char         pname[48];
	  char         pdesc[64];
    nasFlowInfo  nowFlowInfo;  
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
} nasPolicyInfo;



typedef struct _snmpgroup{
	char snmpcommunity[32];
	char snmpsip[32];
	char snmpoid[128];
	char snmpv[12];
} SNMPGROUP;

typedef struct _NetArea_s {
	unsigned int ipstart;
	unsigned int ipend;
	int  netflag;
	int  snmpnum;
	char homeurl[255];
	unsigned int groupid;
	unsigned int policyid;
} _pNetArea;

typedef struct nasIpHostInfo_s { /* IP��ַ����Ϣ    */
    unsigned long   lIp;       /* IP��ַ    */
    unsigned long   netAreaIndex;       /* last time    */
} nasIpHostInfo;


typedef struct _IpHostInfoHead_s {
	  unsigned int       maxiphost;
	  unsigned int       lIpHostNum;
	  unsigned int       onlineIpHostptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       onlineIpHostsize;  /*��ͷ��ʼ��ƫ����*/
} IpHostInfoHead;

typedef struct nasDbUserInfo_s {     /*�û���Ϣ*/
    int32    userid;
    char     username[32];         /*�ʺ���*/
    char     dispname[32];         /*����*/
    unsigned char  Certificate_type;
    char     Certificate_code[24]; 
    unsigned char  sex;
		char     password[32];        
    uint32   groupid[_MAX_GROUPLEVEL];
    char     bindemail[64];
    uint32   bindip; 
    unsigned char  bindmac[6];     /*MAC��ַ*/
    unsigned char  useflags;
    uint32   ratelimit;  
    unsigned char  ratetype;      /*0����  1����*/
    unsigned char  topSrvNameList[MAX_TOP_SRVNUM]; 
    uint32   addtime; 
    uint32   locktime; 
    uint32   lasttime; 
    uint64   lLimitBytes;                  
    uint32   lLimitTimes; 
    uint64   lAllUpFlow;           /*�ܵ���������*/
    uint64   lAllDownFlow;         /*�ܵ���������*/
    uint32   lAllTime;             /*�ܵ�����ʱ��*/
    uint64   lLimitBytesw;         /*ÿ����������������*/
    uint32   lLimitTimesw;         /*ÿ������������ʱ��*/
    uint64   lAllUpFloww;          /*���ڵ���������*/
    uint64   lAllDownFloww;        /*�����ܵ���������*/       
    uint32   lAllTimew;            /*�����ܵ�����ʱ��*/
    uint64   lLimitBytesm;         /*ÿ��������������*/
    uint32   lLimitTimesm;         /*ÿ����������ʱ��*/           
    uint64   lAllUpFlowm;          /*���ܵ���������*/
    uint64   lAllDownFlowm;        /*���ܵ���������*/   
    uint32   lAllTimem;    	       /*���ܵ�����ʱ��*/    
    char     moreflag[32];        
    char     policyid[128];               
    uint32   jftype;
    uint32   accountnum;
    uint32   maxpackage;
    uint32   maxBytes;
    uint32   maxudp;               /* ���udp���� */ 
    uint32   maxtcp;               /*  ���tcp����*/
    uint32   maxudpp;              /*  ÿ�����udp������*/
    uint32   maxtcpp;              /*  ÿ�����tcp������*/
    uint32   maxupp;               /*  ÿ��up������*/ 
    uint32   maxdownp;             /*  ÿ��down������*/  
    uint32   maxsp;                /*  ÿ��128�ֽ�С������*/ 
    uint32   maxnc;                /*  ÿ������½�������Ŀ*/  
}nasDbUserInfo;

/*
���ֵ�hash����
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_dbusernamehead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_dbusernamehead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_dbusernameindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_dbusernameindex;

typedef struct msaDbUserName_s { 
	  int32    userid;
    char     username[32];         /*�ʺ���*/
} msaDbUserName;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlineusername {
    int             next;
    msaDbUserName   item;
} shm_hash_link_onlineusername;



/*
IP��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_dbuseriphead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_dbuseriphead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_dbuseripindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_dbuseripindex;

typedef struct msaDbUserIp_s { 
	  int32    userid;
    int32    ipadd;         /*ipadd*/
} msaDbUserIp;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlineuserip {
    int             next;
    msaDbUserIp    item;
} shm_hash_link_onlineuserip;


/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_dbusermachead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_dbusermachead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_dbusermacindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_dbusermacindex;

typedef struct msaDbUserMac_s { 
	  int32    userid;
    unsigned char  caMac[6];
} msaDbUserMac;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlineusermac {
    int             next;
    msaDbUserMac    item;
} shm_hash_link_onlineusermac;


typedef struct _dbUserInfoHead {
    unsigned int    bufptr;
    unsigned int    bufsize;
    unsigned int    bufnum;
} dbUserInfoHead;

 
typedef struct nasOnlieUserInfo_s {     /*�û���Ϣ*/
    int32    userid;
    uint32   onlineip;
    unsigned char  onlinemac[6];
    uint32   nowmaxpackage;     /*��ǰÿ�����ݰ�*/
    uint64   nowmaxbytes;       /*��ǰÿ������*/
    uint32   nowmaxudpp;        /*��ǰÿ�����udp������*/ 
    uint32   nowmaxtcpp;        /*  ��ǰÿ�����tcp������*/ 
    uint32   nowmaxupp;         /*  ÿ��up������*/ 
    uint32   nowmaxdownp;       /*  ÿ��down������*/ 
    uint32   nowmaxsp;          /*  ÿ��128�ֽ�С������*/ 
    uint32   nowmaxnc;          /*  ÿ������½�������Ŀ*/
    uint32   udpconnect; 
    uint32   tcpconnect; 
    char     nowminute;
	  char     nowhour;
	  int      nowday;
	  nasFlowInfo  nowFlowInfo;  
    nasFlowInfo  hourFlowInfo;  
    nasFlowInfo  dayFlowInfo; 
    uint32   lStartTime;     	/* ��������ʱ�俪ʼ   */
    uint32   lLastTime;     	/*    */
    int      natflag;
    unsigned short    ident;
    int      natnums;
    int      natcheckm;
    char     firsthomeurl;
    uint32   lLastUpdateTime; /* �ϴ����ݸ���ʱ��*/
    unsigned char  nowFlag[6]; /**/
}nasOnlineUserInfo;

/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_onlineuserhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_onlineuserhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_onlineuserindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_onlineuserindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlineuser {
    int             next;
    nasOnlineUserInfo    item;
} shm_hash_link_onlineuser;



typedef struct nasUrlListTmp_s {
	char     url[128];
	unsigned char sort; 
} nasUrlListTmp;

typedef struct nasUrlList_s {
	uint32   urlstart;
	unsigned char urllen;
	unsigned char sort; 
	unsigned short cService; 
} nasUrlList;
/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_urlhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    uint32             freecontenthead;
    uint32             freecontenttail;
    int                locksemid;
    uint32             nownum;
    uint32             urlcontentHead;
    uint32             urlcontentSize;
    uint32             urlcontentNowSize;
} shm_hash_link_urlhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_urlindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_urlindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_url {
    int             next;
    nasUrlList      item;
} shm_hash_link_url;


typedef struct nasTcpBuffer_s {
    uint32         lId;              /* �û�Id  0--û��         */
    unsigned long  lSip;             /* ԴIP��ַ     */
    unsigned long  lDip;             /* Ŀ��IP��ַ   */
    unsigned short nSport;           /* Դ�˿ں�     */
    unsigned short nDport;           /* Ŀ��˿ں�   */
    unsigned long  lStartTime;       /* ��ʼʱ��     */
    unsigned long  lLastTime;        /* ����޸�ʱ�� */
    uint64	       lBytes[2];        /* 0-�����ֽ���  1-�����ֽ��� */
    unsigned char  cType;            /* Э��         */
    unsigned short cService;         /* ����         */
    int            nUrl;             /* ����userhttpBuffer��ָ��   */
    unsigned char  caMac[6];         /* ԴMac��ַ    */
    unsigned char  cUseFlags;        /* ʹ�ñ�ʶ 0--û���� 1--������ 2--�Ѵ��� 3--�ѱ���   */
    unsigned char  cFlags;           /* ��ʶ 0--���� 1--����  */
    unsigned long  lCount;           /* ������ */
    unsigned char  cDirect;          /* ���䷽��*/
    char           keyword1[64];
    char           keyword2[128];
    char           keyword3[32];
    unsigned int   keyword4;
    unsigned long  fdCount;        
    unsigned long  seq;
    char           cMatchRule;       
    char           procdev;           /*dev 1-3  */
    unsigned int   ssnid[2];
} nasTcpBuffer;

/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_TcpBufferhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    uint32             freecontenthead;
    uint32             freecontenttail;
    int                locksemid;
    uint32             nownum;
} shm_hash_link_TcpBufferhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_TcpBufferindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_TcpBufferindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_TcpBuffer {
    int             next;
    nasTcpBuffer    item;
} shm_hash_link_TcpBuffer;


typedef struct nasHttpBuffer_s {
    unsigned long  lStartTime;       /* ��ʼʱ��     */
    unsigned long  lLastTime;        /* ������ʱ��     */
    uint64	       lBytes;           /* ����         */
    char           caHost[64];
    unsigned long  lCount;           /* ����         */
    unsigned long  lTime;            /*      */
    unsigned char  cUrlSort[5];         /*      */
} nasHttpBuffer;
/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_HttpBufferhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    uint32             freecontenthead;
    uint32             freecontenttail;
    int                locksemid;
    uint32             nownum;
} shm_hash_link_HttpBufferhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_HttpBufferindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_HttpBufferindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_HttpBuffer {
    int             next;
    nasHttpBuffer   item;
} shm_hash_link_HttpBuffer;


typedef struct nasDipBuffer_s {
    unsigned long  lStartTime;       /* ��ʼʱ��     */
    unsigned long  lLastTime;        /* ������ʱ��     */
    uint64	       lBytes;           /* ����         */
    uint32         ldip;
    unsigned long  lCount;           /* ����         */
    unsigned long  lTime;            /*      */
    unsigned short cService;         /* ����         */
} nasDipBuffer;
/*
mac��hash���� 
hash ���ڷ����������ͻ������
freecon ����������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_DipBufferhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    uint32             freecontenthead;
    uint32             freecontenttail;
    int                locksemid;
    uint32             nownum;
} shm_hash_link_DipBufferhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_DipBufferindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_DipBufferindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_DipBuffer {
    int             next;
    nasDipBuffer    item;
} shm_hash_link_DipBuffer;


typedef struct nas2SrvInfo_s {
    unsigned long  lDip;             /* Ŀ��IP��ַ   */
    unsigned short nDport;           /* Ŀ��˿ں�   */
    unsigned char  cType;            /* Э��         */
    unsigned long  lLastTime;        /* ����޸�ʱ�� */
    unsigned short cService;         /* ����         */
} nas2SrvInfo;

typedef struct _shm_hash_link_2SrvInfohead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    uint32             freecontenthead;
    uint32             freecontenttail;
    int                locksemid;
    uint32             nownum;
} shm_hash_link_2SrvInfohead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_2SrvInfoindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_2SrvInfoindex;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_2SrvInfo {
    int             next;
    nas2SrvInfo     item;
} shm_hash_link_2SrvInfo;

typedef struct nasUrlSort_s {
	int           sortid;
	char          sortname[48];
}nasUrlSort;

#define NAS_MAXSCH_NUM       20
typedef struct nasschtime_s {
	int           schid;
	char          ruleschtime[400];
}nasschtime;

#define NAS_MAXJJR_NUM       20
typedef struct nasjjrtime_s {
	int           jjrid;
	char          jjrtime[370];
}nasjjrtime;

/*�������ƹ���,���������Ǳ���*/
/*
���ȼ��ǣ�
������ʽ������ҵ�����ͽ���
*/

typedef struct msasrvrulehead_s {
	  unsigned int       srvruleptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       srvrulesize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       lMaxServiceRuleDIp;
    unsigned int       srvruledipptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       srvruledipsize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       lMaxRuleIp;
    unsigned int       srvruleipptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       srvruleipsize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       lMaxWebRule;
    unsigned int       webruleptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       webrulesize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       lMaxWebPostRule;
    unsigned int       webpostruleptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       webpostrulesize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       lMaxWebTypeRule;
    unsigned int       webtyperuleptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       webtyperulesize;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       webruleurlsize;  /*��ͷ��ʼ��ƫ����*/
}msasrvrulehead;


typedef struct nasSrvRule_s {
    unsigned long   ruleid;          /* ������ */
    char            rulename[32];
    char	    			ruleaction;      /*0�ܾ�    1������*/
    int    					qosid;
    unsigned long   ruleschedule;    /*�ù����schedule*/
    char            ruleurlsort[101];
    unsigned long   rulejrid;        /*�ù����schedule*/
    char            ruleurl[128];    /*url*/
    int    					logaction;
    int    					conaction;
    uint32   				srvlist[MAX_SYS_SRVNUM]; /*�����array ������ʼλ��*/
    uint32    		  pid[_MAX_ALLPOLICY];     /*ϵͳ�Ĳ���id*/
    uint32   				userlistptr;             /*�û�array��������ʼλ��*/
    long   					grouplist[_MAX_ALLGRPNUM];  /*�û���array��������ʼλ��*/
    long   					ipStart;         /*ip��ַ��array��������ʼλ��*/
    long   					ipEnd;
    unsigned long   urlStart;	       /*url�б� array��������ʼλ��*/
    unsigned long   urlEnd;
    unsigned long   postStart;	     /*post�б� array��������ʼλ��*/
    unsigned long   postEnd;    
    unsigned long   typeStart;	     /*type�б� array��������ʼλ��*/
    unsigned long   typeEnd; 
    long   					dipStart;	       /*Ŀ��ip��ַ��array��������ʼλ��*/
    long   					dipEnd;   
    unsigned int    urlcheck;
    nasFlowInfo  nowFlowInfo;  
    nasFlowInfo     hourFlowInfo;  
    nasFlowInfo     dayFlowInfo;           
} nasSrvRule;


/*������ƹ����Ŀ��ip��ַ�飬����������array ����*/
typedef struct nasSrvRuleDIp_s {
    long   	    ruleid;       /* ������ */
    unsigned long   ipstart;      /*ip�ο�ʼ��ַ*/
    unsigned long   ipend;        /*ip�ν�����ַ*/
} nasSrvRuleDIp;

/*web���ƹ����ip��ַ�飬����������array ����*/
typedef struct nasRuleUrl_s {
    long   	    ruleid;       /* ������ */
    char        caUrl[64];      /*url�б�*/
} nasRuleUrl;

/*web���ƹ����ip��ַ�飬����������array ����*/
typedef struct nasRulePost_s {
    long   	    ruleid;       /* ������ */
    char        caUrl[64];      /*url�б�*/
} nasRulePost;

/*web���ƹ����ip��ַ�飬����������array ����*/
typedef struct nasRuleType_s {
    long   	    ruleid;       /* ������ */
    char        caUrl[64];      /*url�б�*/
} nasRuleType;


/*���ƹ����ip��ַ�飬����������array ����*/
typedef struct nasRuleIp_s {
    long   	    ruleid;       /* ������ */
    unsigned long   ipstart;      /*ip�ο�ʼ��ַ*/
    unsigned long   ipend;        /*ip�ν�����ַ*/
} nasRuleIp;

typedef struct msaPubInfo_s {
	  unsigned int  lRunning;
	  char          updateflag[32];     /*����λ��־ */
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  int           msaGeneralHour;
	  nasAdmSession      admSession[MAX_ADMIN_SESSION];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int                defaultLan;    /*Ĭ������*/ 
	  msaTopSrv          topSrvName[MAX_TOP_SRVNUM]; 
	  unsigned int       lServiceNum;
	  msaService         ltService[MAX_SYS_SRVNUM]; /*0:unknown 1-400:sys  401-499:user*/
	  nasUrlSort         _urlSortlist[NAS_MAXURLSORT_NUM]; 
	  nasschtime         _schtimelist[NAS_MAXSCH_NUM];
	  nasjjrtime         _jjrtimelist[NAS_MAXJJR_NUM];
	  nasPortInfo        ltPortInfo[MAX_PORTNUM];
	  _pNetArea          ltNetArea[_MAXNETAREANUM];  /*ȫ��������� */
	  _pNetArea          NetAreaCkeck[_MAXNETAREANUM];   /*���������  */ 
	  SNMPGROUP          ltSnmpGroup[_MAXSNMPNUM];
	  nasAllInfo         _AllInfo;
	  nasVPathInfo			 _VPathList[_MAXVPATHNUM];
	  nasQosPathInfo		 _QosList[_MAX_QOSSTATIC_CHANEL];
	  nasGroupInfo	     _GroupList[_MAX_ALLGRPNUM];
	  nasPolicyInfo	     _PolicyList[_MAX_ALLPOLICY];
	  nasSrvRule         _Srvrulelist[_MAX_SRVRULE_NUM];
    IpHostInfoHead     _iphostinfohaed;
    dbUserInfoHead     _dbUserInfoHead;
    shm_hash_link_dbusernamehead  _dbusernamehead; 
    shm_hash_link_dbuseriphead    _dbuseriphead; 
    shm_hash_link_dbusermachead   _dbusermachead; 
    shm_hash_link_onlineuserhead  _onlineuserhead;
    shm_hash_link_urlhead         _urlhashhead;
    shm_hash_link_TcpBufferhead   _tcpsessionhead;
    shm_hash_link_HttpBufferhead  _httpbufferhead;
    shm_hash_link_DipBufferhead   _dipbufferhead;
    shm_hash_link_2SrvInfohead    _2SrvInfohead; 
    msasrvrulehead                _srvrulehead;
}msaPubInfo;

/*
*/
msaPubInfo  *_ltPubInfo;
ltDbConn     *G_DbCon; /*�������������̵����ݿ�����*/

typedef struct msasDir_s {
    char   caDirName[128];
    char   caDirPath[256];
    int    dirTime;    
} msasDir;

typedef struct jiffy_counts_t {
	unsigned long long usr,nic,sys,idle,iowait,irq,softirq,steal;
	unsigned long long total;
	unsigned long long busy;
} jiffy_counts_t;


lt_shmHead *msacreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *msaopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int msacloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
unsigned int get3wordsHash(unsigned int a, unsigned int b, unsigned int c);
unsigned int getMacHash(unsigned char *caMac);
unsigned int  msa_ssn_hash(unsigned int sip, unsigned int  dip,unsigned short  sport,unsigned short dport,int  protocol);
unsigned int getStringHash(unsigned char *caStr,unsigned int strLen);
int msaInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int msaInitPubVar(lt_shmHead *lt_MMHead);
int logWrite(ltMsgHead *ltMsgPk,char *msg,int msgtype);
int nasTimeFormat(char *pFormat,long lTime);
int checkRight(int confd,ltMsgHead *ltMsgPk,int right);
int strgb2utf8(const char *cpInput,char *cpOut,int iInputLen);
int pubGetName(char *caKey,int start,int end,char *caGetName);
int dirtimecmp(msasDir* a,msasDir* b);
msasDir* mydirlist(char* dir,int* len);
int nasTimeGetDate(char *cDate,long lTime);
unsigned long nasCvtLongTime(char *caSdate,char *caTime);
int nasTimeLongFormat(char *pFormat,long lTime);
int nasCvtStime(unsigned long lTime,char *caDate,char *caTime);
char *nasCvtMac(unsigned char *caMac,char *caMacStr);
int nasCvtMacI(unsigned char *caMacStr,unsigned char *caMac);
int msaReportList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaDataCenetrDownLoad(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int sysHasBeenActive();
int  ReadCpu(char *cpuName);
int readDisk(char * DiskInfo);
int ReadEth0Mac(char *strHadd);
void striptrailingwhitespace(stralloc *sa);

/*msaService.c*/
int msaInitPortInfo(lt_shmHead *lt_MMHead);
int nasInitTopService(lt_shmHead *lt_MMHead);
int msaInitServiceArray(lt_shmHead *lt_MMHead);

/*msaIpHost.c*/
int nasInitIpHostInfo(lt_shmHead *lt_MMHead);


/*dbUserLib.c*/
int msaInitDbUserList(msaPubInfo  *ltPubInfo);
int msaHashInsertDbUser(msaPubInfo  *ltPubInfo, nasDbUserInfo *item);
int msaHashDelDbUser(msaPubInfo  *ltPubInfo, nasDbUserInfo *item);
nasDbUserInfo *msaLookUpDbUserById(msaPubInfo  *ltPubInfo,unsigned int userid );
nasDbUserInfo *msaLookUpDbUserByIp(msaPubInfo  *ltPubInfo,unsigned int ipadd );
nasDbUserInfo *msaLookUpDbUserByName(msaPubInfo  *ltPubInfo,char *username );
nasDbUserInfo *msaLookUpDbUserByMac(msaPubInfo  *ltPubInfo,char *caMac );
unsigned int msaGetFreeUserid(msaPubInfo  *ltPubInfo);

/*OlineUserLib.c*/
int msaInitOnlineUserHash(msaPubInfo  *ltPubInfo);
nasOnlineUserInfo *msaHashInsertOnlineUser(msaPubInfo  *ltPubInfo, nasOnlineUserInfo *item);
nasOnlineUserInfo *msaHashLookUpOnlineUser(msaPubInfo  *ltPubInfo,unsigned int ipadd);
int msaDeleteOnlineUser(msaPubInfo  *ltPubInfo,unsigned int ipadd);

/*msaUrllist.c*/
nasUrlList *msaHashLookUpUrl(msaPubInfo  *ltPubInfo,char *strurl);
int nasInitUrlList(lt_shmHead *lt_MMHead);

/*msaTcpSession.c*/
int msaInitTcpBufferHash(msaPubInfo  *ltPubInfo);
nasTcpBuffer *msaHashInsertTcpBuffer(msaPubInfo  *ltPubInfo,unsigned int ssnhash, nasTcpBuffer *item);
nasTcpBuffer *msaHashLookUpTcpSession(msaPubInfo  *ltPubInfo,unsigned int ssnhash, nasTcpBuffer *item);
int msaDeleteTcpSession(msaPubInfo  *ltPubInfo,unsigned int ssnhash, nasTcpBuffer *item);


/*msaHttpBuffer.c*/
int msaInitHttpBufferHash(msaPubInfo  *ltPubInfo);
nasHttpBuffer *msaHashInsertHttpBuffer(msaPubInfo  *ltPubInfo,nasHttpBuffer *item);
nasHttpBuffer *msaHashLookUpHttpBuffer(msaPubInfo  *ltPubInfo,char *caHost);
int msaDeleteHttpBuffer(msaPubInfo  *ltPubInfo, char *caHost);

/*msaDipBuffer.c*/
int msaInitDipBufferHash(msaPubInfo  *ltPubInfo);
nasDipBuffer *msaHashInsertDipBuffer(msaPubInfo  *ltPubInfo,nasDipBuffer *item);
nasDipBuffer *msaHashLookUpDipBuffer(msaPubInfo  *ltPubInfo,unsigned int ldip);
int msaDeleteDipBuffer(msaPubInfo  *ltPubInfo, unsigned int ldip);

/*msa2SrvInfo.c*/
int msaInit2SrvInfoHash(msaPubInfo  *ltPubInfo);
nas2SrvInfo *msaHashInsert2SrvInfo(msaPubInfo  *ltPubInfo,nas2SrvInfo *item);
nas2SrvInfo *msaHashLookUp2SrvInfo(msaPubInfo  *ltPubInfo,unsigned int lDip,unsigned int nDport,unsigned int cType);
int msaDelete2SrvInfo(msaPubInfo  *ltPubInfo, unsigned int lDip,unsigned int nDport,unsigned int cType);



/*srv-admin.c*/
int msaShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);


/*srv-group.c*/
int msalooparea(stralloc *strTemp,int level,int pid);
int msaGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);


/*srv-manager.c*/
int msaAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowArea(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int ltportinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltportinfoaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltapplyportinfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
/*srv-users.c*/
int msaShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaPolicyTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaTerminfoUp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
/*srv-data.c*/
int msaShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaBackListDownLoadCSV(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltmsasearchkeyquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//�ؼ��ּ���
int msaipbacksearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//�����뵼������
int msaNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//gird���������ɾ��
int ltshowservicequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//������ʼ�¼���ܽڵ�����
int ltwebshowwebsitequery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//HTTP������ܽڵ�����
void striptrailingwhitespace(stralloc *sa);//ɾ���հ׷���
int ltimshowwebquery(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//QQ/msn
int ltmsasearchkeyhttpa(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);//�ؼ����б������ύ
unsigned long msaCvtLongTime(char *caSdate,char *caTime);//ʱ��ת��
int msaCvtStime(unsigned long lTime,char *caDate,char *caTime);

#endif

