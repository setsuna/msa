/*update log*/
/*2003-11-09*/
/*nassrvrule  add   portstart   portend  prottype  */
/*nassrvruledip del portstart   portend */
/* nasmailfilter add rnum  maction */
/* nasmailrule add rnum */


create database nas;
grant all privileges on nas.*  to nas@localhost identified by 'naspass';

grant all privileges on nas.*  to uuu@172.16.0.92 identified by 'uuu';

drop table nasPortInfo;
create table nasPortInfo (
  id         int   primary key,       /* 记录标识     */	
	lPortStart int not null,
	lPortEnd   int not null,
	lFlag      int default 0, /* 该tcp端口是否被记录  3：仅仅监控 2:监控并记录  9：免监控 10 禁止访问*/
	lProp      int default 1, /* 1 tcp  2 udp */
	isLocal     int default 0  /*1 local 0 remote 2 both*/  
);

drop table nasNetArea;
create table nasNetArea (
    id           int       primary key,       /* 记录标识     */
    ipstart      char(16),                    /* 起始IP地址   */
    ipend        char(16),                    /* 终止IP地址   */
    netflag      smallint,      	     /* 网络标识  1--基于ip帮定   2mac帮定 3主机帮定 5NT域验证   8本地严整  9--免监控 10禁止访问 */
    ipdesp       char(64),
    snmpnum      int default 0
);

/*insert into nasNetArea values (1,'192.168.0.1','192.168.0.255',1);*/

drop table nasIpHostInfo;
create table nasIpHostInfo (
	lip       int primary key,
	ipadd     char(16),      /*ip地址*/
  netflag   smallint       /* 用户映射绑定：  1--基于ip帮定   2mac帮定 5NT域验证   8本地严整  9--免监控 10禁止访问 */
);
create UNIQUE index nasIpHostInfo_index on nasIpHostInfo (lip);


/*和内存对应关系*/
/*
1 默认是ip绑定
2 nasIpHostInfo关系绑定，按顺序绑定 1 2 3 5 8 9
*/

/* 组信息  */
drop table nasgroup;
create table nasgroup (
    groupid    int    primary key,
    groupname  varchar(48),
    grouphomeurl  varchar(128)
);
create UNIQUE index nasgroup_index on nasgroup (groupname);
insert into nasgroup (groupid,groupname) values (0,'Default-Dpt.');

drop table nasgroupip;
create table nasgroupip (
    groupid    int,
    lip       int(11) unsigned NOT NULL,
	  ipadd     char(16),      
	  lipend       int(11) unsigned NOT NULL,
	  ipaddend     char(16)     
);
//create UNIQUE index nasgroupip_index on nasgroupip (groupid);

drop table naspolicy;
create table naspolicy (
    pid     int  primary key ,   /*policy id*/
    pdesc   varchar(128) default "",  /*描述*/
    pname   varchar(48) not null
);

insert into naspolicy (pid,pdesc,pname) values (0,'default policy','default-Policy');


/* 用户信息  */
drop table nasuser;
create table nasuser (
    userid      int   primary key,
    username    varchar(32), /*用户名*/
    password    varchar(32),
    groupid     int default 0,
    dispname    varchar(32),  /*显示的名字*/
    bindemail   varchar(128), /*绑定的email地址*/
    bindip      char(16), /*绑定的ip地址*/
    bindmac     char(32), /*绑定的mac地址*/
    useflags    smallint default 0,     /*10禁止访问 9免监控 0正常*/      
    ratelimit   int  default 0, /*带宽限制*/
    ratelower   int  default 0, /*带宽限制*/    
    addtime     char(16),     /*该用户有效期限*/
    lasttime    char(16),     /*用户最后一次上线时间*/
    lAllUpFlow  bigint default 0, 	 /*总的上行流量*/
    lAllDownFlow  bigint default 0, /*总的下行流量*/
    lAllTime    int default 0,   /*总的上网时间*/
    lLimitBytes bigint default 0, /*每天最大的上网流量*/
    lLimitTimes int default 0, /*每天最大的上网时间*/
    macbindip   smallint default 0, /*主机ip地址和mac地址必须一致才能访问*/
    onlybindip  smallint default 0, /*适用于用户验证方式，用户只有在该ip地址才能登陆*/
    onlybindmac smallint default 0,/*适用于用户验证方式，用户只有在该mac地址才能登陆*/
    onlylogone  smallint default 0,  /*适用于用户验证方式，用户同时只能登陆一次*/
    lCUpFlow    bigint default 0, 	 /*本次连接上行流量*/
    lCDownFlow  bigint default 0, 	 /*本次连接下行流量*/
    lCTime      int    default 0,        /*本次连接的记费时间*/
    policyid    int    default 0, 	/*用户默认的policyid*/
    jftype      int    default 0,  
    accountnum  int    default 0, 
    maxpackage  int    default 0, 
    maxBytes    bigint    default 0,
    formmon     int    default 1, 
    mailmon     int    default 1, 
    aimmon      int    default 1, 
    blockmon    int    default 1,
    policyid2    int    default 0, 	/*用户默认的policyid*/
    policyid3    int    default 0, 	/*用户默认的policyid*/
    policyid4    int    default 0, 	/*用户默认的policyid*/
    policyid5    int    default 0, 	/*用户默认的policyid*/
    policyid6    int    default 0, 	/*用户默认的policyid*/
    policyid7    int    default 0, 	/*用户默认的policyid*/
    policyid8    int    default 0, 	/*用户默认的policyid*/
    policyid9    int    default 0, 	/*用户默认的policyid*/
    policyid10   int    default 0, 	/*用户默认的policyid*/
    policyid11   int    default 0, 	/*用户默认的policyid*/
    policyid12   int    default 0,
    accountflag  int    default 0,   /*用户计费时间,0全部时间*/
    policyid13   int    default 0 ,  
		policyid14   int    default 0 ,  
		policyid15   int    default 0 ,   
		policyid16   int    default 0 ,  
		policyid17   int    default 0 ,  
		policyid18   int    default 0 ,  
		policyid19   int    default 0 ,  
		policyid20   int    default 0 ,   
		policyid21   int    default 0 ,  
		policyid22   int    default 0 ,   
		policyid23   int    default 0 ,   
		policyid24   int    default 0 ,   
		policyid25   int    default 0 ,   
		policyid26   int    default 0 ,   
		policyid27   int    default 0 ,   
		policyid28   int    default 0 ,   
		policyid29   int    default 0 ,   
		policyid30   int    default 0 ,   
		policyid31   int    default 0 ,   
		policyid32   int    default 0 ,   
		policyid33   int    default 0 ,   
		policyid34   int    default 0 ,   
		policyid35   int    default 0 ,   
		policyid36   int    default 0 ,   
		policyid37   int    default 0 ,   
		policyid38   int    default 0 ,   
		policyid39   int    default 0 ,   
		policyid40   int    default 0 ,   
		policyid41   int    default 0 ,   
		policyid42   int    default 0 ,   
		policyid43   int    default 0 ,   
		policyid44   int    default 0 ,   
		policyid45   int    default 0 ,   
		policyid46   int    default 0 ,   
		policyid47   int    default 0 ,   
		policyid48   int    default 0 ,   
		policyid49   int    default 0 ,   
		policyid50   int    default 0 ,   
		policyid51   int    default 0 ,   
		policyid52   int    default 0 ,   
		policyid53   int    default 0 ,   
		policyid54   int    default 0 ,   
		policyid55   int    default 0 ,   
		policyid56   int    default 0 ,   
		policyid57   int    default 0 ,   
		policyid58   int    default 0 ,   
		policyid59   int    default 0 ,   
		policyid60   int    default 0 ,   
		User_name         char(32)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Org_name          char(60)  default '',
		Country           char(20)  default '',
		Sex               char(10)  default '',
		moreflag          char(32)  default '0000000000',
		maxudp     int default 0,      /* 最多udp连接 */   
    maxtcp     int default 0,      /*  最多tcp连接*/
    maxudpp    int default 0,      /*  每分最多udp包速率*/ 
    maxtcpp    int default 0,      /*  每分最多tcp包速率*/ 
    maxupp     int default 0,      /*  每分up包速率*/ 
    maxdownp   int default 0,      /*  每分down包速率*/  
    maxsp      int default 0,       /*  每分128字节小包速率*/ 
    maxnc      int default 0,       /*  每分最多新建连接数目*/ 
    userno     varchar(32) default '',        
    machientype varchar(32) default '',       
    scretlevel  varchar(32) default '',        
    measure     varchar(64) default '',       
    remark      varchar(64) default '', 
    post      varchar(32) default '',        
    place     varchar(64) default '',       
    purpose   varchar(32) default '' 
); 
 
/* moreflag  1 noftp 2 nopost 3 nomsn 4 noqq*/  
    
    
create UNIQUE index nasuser_index on nasuser (username);
    
create  index nasuser_index2 on nasuser (bindip);
create  index nasuser_index3 on nasuser (bindmac);
create  index nasuser_index4 on nasuser (groupid);
    
insert into nasuser (userid,username,password,groupid,dispname, 
bindemail,bindip,bindmac,macbindip,addtime,lasttime,useflags,ratelimit,lLimitBytes,lLimitTimes) 
values (1,'Unknown','','0','Unknown','','','',0,'','',10,0,0,0);
    
insert into nasuser (userid,username,password,groupid,dispname, 
bindemail,bindip,bindmac,macbindip,addtime,lasttime,useflags,ratelimit,lLimitBytes,lLimitTimes) 
values (2,'vpnadmin','qoQhDpRa4xOAwkBtuS0mq0','0','admin','','','',0,'','',1,0,0,0);

insert into nasuser (userid,username,password,groupid,dispname, 
bindemail,bindip,bindmac,macbindip,addtime,lasttime,useflags,ratelimit,lLimitBytes,lLimitTimes) 
values (3,'vpnadmin1','qoQhDpRa4xOAwkBtuS0mq0','0','vpnadmin1','','','',0,'','',1,0,0,0);

/* 系统schedule  */

drop table nasschedule;
create table nasschedule (
    schid      int   primary key,
    schname    varchar(24) not null, 
    schdesc    varchar(64) default '',
    schtime    char(200) default '0',
    schtime1    char(200) default '0'
);
insert into nasschedule values (1,"WorkDay","WorkDay","000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000","000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000"); 



/*
0系统分类：手工作做
0：未知分类
1：互联网协议  http https dns  httpproxy  sock5 ftp  LDAP目录服务  Microsoft SQL Server  Windows终端服务 SSH Telnet HTTP服务器
2: 邮件：      smtp smtps pop3 pop3s
3: 流媒体：    MMS流媒体  PNM流媒体
4: IM:         AOL|ICQ   ICQ即时通信 IRC聊天 MSN Messenger  QQ聊天 Yahoo Messenger 
5: P2P:        BT  emule  xulei  
6: 游戏：
7：炒股软件：
8：网络电视: 
9: 
*/
/*
          其他未知服务  0  
 常用网络协议            
         
         web协议        2 
         加密web协议    3  
         目录与认证     4  
         数据库服务     5       
         文件拷贝       
         			smb文件共享 6  
         			FTP协议     1     
         邮件服务       7
  
         远端连接       8   
   
         代理协议       9  
   
         股票软件       10
    
         流媒体         11
         
         即时通信       12        
     
         网络电话       13
         
         [常用P2p协议] 
	         BT协议         14
	         电驴协议       15
	         迅雷协议       16
         
         其他P2P        17  
         
         游戏平台       18
         大型游戏       19
   
         [常用网络电视]  
	         pplive         20
	         QQlive         21
	         PPStream       22
         
         其他网络电视   23
         
     
         自定义服务     24  
         
         http视频流  25
         			
         http音频流  26 --flv
         
         http二进制下载 27
         
         http多进程下载 28
         
         80端口的非标准http 29
         http应用流  30
*/
/*系统的服务*/
/* 系统服务信息  */
drop table nasservice;
create table nasservice (
    srvid      int   primary key,    /*0 100 200 300.....*/
    srvname    varchar(24) not null, /*服务名*/
    srvdes     varchar(64) default '',
    srvflag    int default 0,     /*0系统，不许删除  1：自建服务*/      
    srvprot    int default 6,     /*协议 0表示所有  6 tcp    17 udp*/
    nportL     int default 0,     /*端口下限 */
    nPortU     int default 0,     /*端口上限*/
    srvDirect  int default 0, 	  /*方向：0:out  1:in  2：双向*/
    srvSort    int default 0 	    /*一级分类，用作报告等 0-99 */
);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (1,'AOL|ICQ','AOL|ICQ',0,6,5190,5190,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (2,'DNS查询','域名查询',0,17,53,53,0,2);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (3,'FTP下载','FTP协议',0,6,21,21,0,1);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (4,'H.323视频通信','H.323协议 - 包括Q.931/H.245/RTP/RTCP/T.120',0,6,1720,1720,0,13);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (5,'HTTP上网','上网传输协议',0,6,80,80,0,2);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (6,'HTTPS加密上网','加密的上网传输协议',0,6,443,443,0,3);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (7,'SMB文件共享协议','SMB文件共享协议',0,17,445,445,2,6);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (8,'squid','squid',0,6,3128,3128,0,9);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (9,'IMAP4邮件传输','主动邮件传输协议',0,6,143,143,0,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (10,'VNC协议','VNC协议',0,6,5900,5900,0,8);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (11,'IRC聊天','网络聊天',0,6,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (12,'LDAP目录服务','Lightweight Directory Access Protocol',0,6,389,389,0,4);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (13,'Microsoft SQL Server','Microsoft SQL Server protocol',0,6,1433,1433,0,5);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (14,'MMS流媒体','微软流媒体协议',0,6,1755,1755,0,11);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (15,'边锋游戏平台','边锋游戏平台',0,6,5100,5100,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (16,'MSN Messenger','MSN messenger协议',0,6,1863,1863,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (17,'Net2Phone','Net2Phone协议',0,17,6801,6801,0,13);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (18,'PNM流媒体','RealNetworks流媒体',0,6,7070,7070,0,11);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (19,'POP3收邮件','Post Office Protocol v.3',0,6,110,110,0,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (20,'kerberos','kerberos',0,6,88,88,0,4);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (21,'Windows终端服务','windown终端服务',0,6,3389,3389,0,8);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (22,'SMTP发邮件','Simple Mail Transfer Protocol (SMTP)',0,6,25,25,0,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (23,'SMTPS发邮件','Simple Mail Transfer Protocol',0,6,465,465,0,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (24,'SSH','加密的Shell protocol',0,6,22,22,0,8);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (25,'Telnet','Telnet protocol',0,6,23,23,0,8);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (26,'同花顺股市行情','同花顺股市行情',0,6,8601,8601,0,10);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (27,'联众游戏','联众游戏',0,6,2000,2000,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (28,'QQ聊天','QQ聊天',0,17,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (29,'Yahoo Messenger','Yahoo Messenger',0,6,5050,5050,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (30,'中游中心聊天','中游中心聊天',0,17,8001,8001,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (31,'中游中心游戏','中游中心游戏',0,6,8011,8011,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (32,'边锋四国游戏','边锋四国游戏',0,6,4200,4200,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (33,'BT协议','BT协议',0,6,2,1,0,14);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (34,'HTTP服务器','上网传输协议',0,6,80,80,1,2);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (35,'HTTPS服务器','加密的上网传输协议',0,6,443,443,1,3);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (36,'POP3服务器','Post Office Protocol v.3',0,6,110,110,1,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (37,'SMTP服务器','Simple Mail Transfer Protocol (SMTP)',0,6,25,25,1,7);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (38,'eMule协议','eMule协议',0,6,2,1,0,15);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (39,'pplive','pplive',0,6,2,1,0,20);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (40,'QQlive','QQlive',0,6,2,1,0,21);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (41,'Taobao','Taobao',0,6,16000,16000,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (42,'Skype','Skype',0,6,2,1,0,12);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (43,'KaZaA','KaZaA',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (44,'Gnutella','Gnutella',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (45,'WinMX','WinMX',0,6,2,1,0,17);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (46,'大智慧股市行情','大智慧股市行情',0,6,22223,22223,0,10);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (47,'传奇3','传奇3',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (48,'迅雷下载','迅雷下载',0,6,2,1,0,16);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (49,'浩方对战平台','浩方对战平台',0,6,1201,1203,0,18);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (50,'Socks5代理','Socks5',0,6,2,1,0,9);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (51,'HTTP代理','HTTP代理',0,6,2,1,0,9);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (52,'边锋五子棋','边锋五子棋',0,6,4310,4310,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (53,'边锋中国象棋','边锋中国象棋',0,6,4250,4250,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (54,'边锋游戏下载','边锋游戏下载',0,6,5005,5005,0,18);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (55,'百度下吧','百度下吧',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (56,'Ares','Ares',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (57,'SoulSeek','SoulSeek',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (58,'appleJuice','appleJuice',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (59,'DirectConnect','DirectConnect',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (60,'xdcc','xdcc',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (61,'waste','waste',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (62,'QQ超级旋风','QQ超级旋风',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (63,'PPStream','PPStream',0,6,2,1,0,22);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (64,'UUSee','UUSee',0,6,2,1,0,23);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (65,'沸点网络电视','沸点网络电视',0,6,2,1,0,23);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (66,'POCO','POCO',0,6,2,1,0,23);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (67,'QVOD','QVOD',0,6,2,1,0,23);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (68,'屁屁狗','屁屁狗',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (69,'totolook','totolook',0,6,2,1,0,17);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (70,'PP点点通,'PP点点通',0,6,2,1,0,17);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (71,'网易泡泡,'网易泡泡',0,6,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (72,'新浪UC,'新浪UC',0,6,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (73,'指南针,'指南针',0,6,2,1,0,10);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (74,'证券之星,'证券之星',0,6,2,1,0,10);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (75,'钱龙,'钱龙',0,6,9000,9000,0,10);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (76,'pcanywhere','pcanywhere',0,6,5631,5632,0,8);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (77,'mysql','mysql',0,6,3306,3306,0,5);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (78,'ORACLE','ORACLE',0,6,1521,1521,0,5);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (79,'sina游戏','sina游戏',0,6,2,1,0,15);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (80,'sohu游戏','sohu游戏',0,6,2,1,0,15);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (81,'QQ游戏','QQ游戏',0,6,2,1,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (82,'穿越火线','穿越火线',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (83,'地下城与勇士','地下城与勇士',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (84,'天下2','天下2',0,17,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (85,'传奇外传','传奇外传',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (86,'剑侠情缘3','剑侠情缘3',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (87,'征途','征途',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (88,'rayfile','rayfile',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (89,'跑跑卡丁车','跑跑卡丁车',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (90,'魔兽世界','魔兽世界',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (91,'泡泡堂','泡泡堂',0,6,2,1,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (92,'热血江湖','热血江湖',0,6,13000,13000,0,19);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (93,'盛大游戏','盛大游戏',0,6,2,1,0,18);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (94,'VS竞技平台','VS竞技平台',0,6,2,1,0,18);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (95,'GTalk|Jabber','GTalk|Jabber',0,6,5222,5222,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (96,'飞信','飞信',0,6,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (97,'UUCall','UUCall',0,6,2,1,0,13);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (98,'SIP协议','SIP协议',0,6,2,1,0,13);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (99,'RTP/RTCP数据','RTP/RTCP',0,6,2,1,0,13);


insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (100,'http视频流','http视频流',0,6,2,1,0,25);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (101,'http音频流','http视频流',0,6,2,1,0,26);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (102,'http的FLash流','http的FLash流',0,6,2,1,0,26);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (103,'http二进制下载','http二进制下载',0,6,2,1,0,27);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (104,'http多进程下载','http多进程下载',0,6,2,1,0,28);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (105,'http应用或文件流','http应用或文件流',0,6,2,1,0,30);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (106,'非标准http流','非标准http流',0,6,2,1,0,29);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (107,'MSN-httpproxy','MSN-httpproxy',0,6,2,1,0,12);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (108,'FTP数据流','FTP数据流',0,6,2,1,0,1);

insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (109,'海通委托交易,'海通委托交易',0,6,2,1,0,10);
insert into nasservice (srvid,srvname,srvdes,srvflag,srvprot,nportL,nPortU,srvDirect,srvSort) values (110,'海通证券软件','海通证券软件',0,6,2,1,0,10);


/*服务控制规则*/
drop table nassrvrule;
create table nassrvrule (
    ruleid     int ,   /*规则id*/
    ruletype   smallint default 0,  /*3：应用于用户； 1:应用于用户组  2policy 4 应用于ip地址段；   0-4：应用于全系统*/
    ruledtype  smallint default 0,  /* 该规则的目标地址类型  0：全部地址  1：地址段*/  
    ruleflag   smallint default 1,  /*0不启动  1：启动*/
    ruleaction smallint default 0,  /*1允许    0：拒绝 */
    ruleschedule  int   default 0,  /*0 全部时间  其他在schedule中查找*/
    rulesrv       int   default 0,  /*0 所有服务 1在srvrulesrv中查找 2自定义*/
    rulename   varchar(32) not null,
    pid        int   default 0,   /*策略组*/
    portstart  int default 0,   /*开*/
    portend    int default 0,   /*结束*/
    prottype   int default 6 ,   /*6 tcp 17 udp*/
    qosid      int default 0 ,   /*qosid 0或cid*/
    logaction  int default 1 ,   /*记录匹配的连接*/
    conaction  int default 1 ,   /*记录匹配的内容*/
    ruleurlsort char(100)  default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', /*100个URL分类*/
    rulejr     varchar(365) default '0',
    rulerate   int default 0,    /*rate优先级别*/
    ruleurl    char(128)
);


/*服务控制规则的服务组*/
drop table nassrvrulesrv;
create table nassrvrulesrv (
    ruleid     int ,   /*规则id*/
    srvid     int     /*服务的id 0是其他服务*/
);

/*服务控制规则针对目标ip段的部分*/
drop table nassrvruledip;
create table nassrvruledip (
    ruleid     int ,   /*规则id*/
    ipstart    char(20),   /*ip段开始地址*/
    ipend      char(20)   /*ip段结束地址*/
);

/*服务控制规则针对用户的部分*/
drop table nasruleuser;
create table nasruleuser (
    ruleid     int ,   /*规则id*/
    userid     int     /*用户的id */
);

/*服务控制规则针对用户的部分*/
drop table nasrulegroup;
create table nasrulegroup (
    ruleid     int ,   /*规则id*/
    groupid     int     /*用户组的id */
);


/*服务控制规则针对ip段的部分*/
drop table nasruleip;
create table nasruleip (
    ruleid     int ,   /*规则id*/
    ipstart    char(20),   /*ip段开始地址*/
    ipend      char(20)    /*ip段结束地址*/
);




/*web控制规则的url*/
drop table nasruleurl;
create table nasruleurl (
    ruleid     int ,   /*规则id*/
    caUrl      varchar(64)   /*url列表*/
);

/*web控制规则的type*/
drop table nasruletype;
create table nasruletype (
    ruleid     int ,   /*规则id*/
    caUrl      varchar(64)   /*url列表*/
);

/*webpost控制规则的type*/
drop table nasruleposturl;
create table nasruleposturl (
    ruleid     int ,   /*规则id*/
    caUrl      varchar(64)   /*url列表*/
);



drop table nasurlsort;
CREATE TABLE nasurlsort (
  sortid int(11) NOT NULL default '0',
  sorname varchar(40) NOT NULL default '',
  sortdesc varchar(100) default ''
) ;

/*
成人色情
违法网站


上市公司  娱乐 文学  彩票  博客  BT  金融  宠物 
宠物  新闻  动漫  FLASH  购物  生活  健康 交通 
交友 manwoman 聊天  论坛 旅游 美容 明星  视频 
时尚 天气 图铃 搞笑 星座 影视 美食 艺术 游戏
病毒 IT  法律 房产 服装  教育 军事 体育 邮箱 
汽车 手机 图片 招聘人才 黑客 政府 收索引 下载 
酷站 
*/



INSERT INTO nasurlsort VALUES (1,'宣扬暴力','宣扬暴力');
INSERT INTO nasurlsort VALUES (2,'具有侵略性','具有侵略性');
INSERT INTO nasurlsort VALUES (3,'色情网站','色情网站');
INSERT INTO nasurlsort VALUES (4,'裸体或暴露','裸体或暴露');
INSERT INTO nasurlsort VALUES (5,'盗版和破解','盗版和破解');
INSERT INTO nasurlsort VALUES (6,'枪支武器','枪支武器');
INSERT INTO nasurlsort VALUES (7,'间谍、钓鱼网站','间谍、钓鱼网站');
INSERT INTO nasurlsort VALUES (8,'感染病毒','感染病毒');
INSERT INTO nasurlsort VALUES (9,'政治与理念','政治与理念');
INSERT INTO nasurlsort VALUES (10,'政府网站','政府网站');
INSERT INTO nasurlsort VALUES (11,'宗教','宗教');
INSERT INTO nasurlsort VALUES (12,'军事','军事');
INSERT INTO nasurlsort VALUES (13,'教育','教育');
INSERT INTO nasurlsort VALUES (14,'法律','法律');
INSERT INTO nasurlsort VALUES (15,'体育','体育');
INSERT INTO nasurlsort VALUES (16,'艺术','艺术');
INSERT INTO nasurlsort VALUES (17,'健康','健康');
INSERT INTO nasurlsort VALUES (18,'交通','交通');
INSERT INTO nasurlsort VALUES (19,'天气','天气');
INSERT INTO nasurlsort VALUES (20,'上市与知名公司','上市与知名公司');
INSERT INTO nasurlsort VALUES (21,'搜索','搜索');
INSERT INTO nasurlsort VALUES (22,'下载','下载');
INSERT INTO nasurlsort VALUES (23,'邮箱','邮箱');
INSERT INTO nasurlsort VALUES (24,'人才职位','人才职位');
INSERT INTO nasurlsort VALUES (25,'BT网站','BT网站');
INSERT INTO nasurlsort VALUES (26,'IT综合网站','IT综合网站');
INSERT INTO nasurlsort VALUES (27,'黑客与安全技术','黑客与安全技术');
INSERT INTO nasurlsort VALUES (28,'反病毒反间谍网站','反病毒反间谍网站');
INSERT INTO nasurlsort VALUES (29,'升级站点','升级站点');
INSERT INTO nasurlsort VALUES (30,'代理服务器','代理服务器');
INSERT INTO nasurlsort VALUES (31,'新闻','新闻');
INSERT INTO nasurlsort VALUES (32,'博客','博客');
INSERT INTO nasurlsort VALUES (33,'媒体网站','媒体网站');
INSERT INTO nasurlsort VALUES (34,'网络广播电视','网络广播电视');
INSERT INTO nasurlsort VALUES (35,'网上电话','网上电话');
INSERT INTO nasurlsort VALUES (36,'汽车','汽车');
INSERT INTO nasurlsort VALUES (37,'手机','手机');
INSERT INTO nasurlsort VALUES (38,'服装','服装');
INSERT INTO nasurlsort VALUES (39,'宠物','宠物');
INSERT INTO nasurlsort VALUES (40,'游戏','游戏');
INSERT INTO nasurlsort VALUES (41,'旅游','旅游');
INSERT INTO nasurlsort VALUES (42,'购物','购物');
INSERT INTO nasurlsort VALUES (43,'影视','影视');
INSERT INTO nasurlsort VALUES (44,'音乐','音乐');

INSERT INTO nasurlsort VALUES (45,'动漫','动漫');
INSERT INTO nasurlsort VALUES (46,'美食','美食');
INSERT INTO nasurlsort VALUES (47,'文学','文学');
INSERT INTO nasurlsort VALUES (48,'图铃','图铃');
INSERT INTO nasurlsort VALUES (49,'图片','图片');
INSERT INTO nasurlsort VALUES (50,'时尚','时尚');
INSERT INTO nasurlsort VALUES (51,'Flash','Flash');
INSERT INTO nasurlsort VALUES (52,'生活频道','生活频道');
INSERT INTO nasurlsort VALUES (53,'聊天','聊天');

INSERT INTO nasurlsort VALUES (54,'论坛','论坛');
INSERT INTO nasurlsort VALUES (55,'交友','交友');
INSERT INTO nasurlsort VALUES (56,'同学录','同学录');
INSERT INTO nasurlsort VALUES (57,'社区','社区');
INSERT INTO nasurlsort VALUES (58,'彩票','彩票');
INSERT INTO nasurlsort VALUES (59,'明星','明星');
INSERT INTO nasurlsort VALUES (60,'星座','星座');
INSERT INTO nasurlsort VALUES (61,'搞笑','搞笑');
INSERT INTO nasurlsort VALUES (62,'型男型女','型男型女');

INSERT INTO nasurlsort VALUES (63,'娱乐圈','娱乐圈');
INSERT INTO nasurlsort VALUES (64,'模特','模特');
INSERT INTO nasurlsort VALUES (65,'银行','银行');
INSERT INTO nasurlsort VALUES (66,'证券与金融网站','证券与金融网站');
INSERT INTO nasurlsort VALUES (67,'保险公司','保险公司');
INSERT INTO nasurlsort VALUES (68,'房地产','房地产');
INSERT INTO nasurlsort VALUES (69,'网络广告','网络广告');
INSERT INTO nasurlsort VALUES (70,'网络计数','网络计数');


INSERT INTO nasurlsort VALUES (71,'自定义分类1','自定义分类1');
INSERT INTO nasurlsort VALUES (72,'自定义分类2','自定义分类2');
INSERT INTO nasurlsort VALUES (73,'自定义分类3','自定义分类3');
INSERT INTO nasurlsort VALUES (74,'自定义分类4','自定义分类4');
INSERT INTO nasurlsort VALUES (75,'自定义分类5','自定义分类5');
INSERT INTO nasurlsort VALUES (76,'自定义分类6','自定义分类6');
INSERT INTO nasurlsort VALUES (77,'自定义分类7','自定义分类7');
INSERT INTO nasurlsort VALUES (78,'自定义分类8','自定义分类8');



/*分类网址*/
drop table nasurls;
create table nasurls (
	urlsort int not null,
	url     varchar(36) 
);



/*http访问记录*/

drop table nashttplog;
create table nashttplog (
    userid      int,    
    sdate       char(10),
    stime       char(8), 
    conntime    int(10), 
    sip         char(16),
    host        varchar(64),
    uri         varchar(128),
    bytes       bigint,         
    flags       smallint,
    dip         char(16) default '',    
    keyword     char(48) default ''   
);

create index nashttplog_index1 on nashttplog (userid);
create index nashttplog_index2 on nashttplog (sdate);
create index nashttplog_index5 on nashttplog (stime);
create index nashttplog_index3 on nashttplog (sip);
create index nashttplog_index4 on nashttplog (bytes);
create index nashttplog_index6 on nashttplog (dip);


/*http访问记录*/

drop table nashttpblocklog;
create table nashttpblocklog (
    userid      int,    
    sdate       char(10),
    stime       char(8), 
    sip         char(16),
    dip         char(16) default '',  
    host        varchar(64),
    uri         varchar(128),
    sortid      char(6) default '00000'     
);

create index nashttpblocklog_index1 on nashttpblocklog (userid);
create index nashttpblocklog_index2 on nashttpblocklog (sdate);
create index nashttpblocklog_index3 on nashttpblocklog (sip);
create index nashttpblocklog_index4 on nashttpblocklog (sortid);


/* IP访问记录  */
drop table nasiplog;
create table nasiplog (
    userid      int,          /* 用户名     */
    sdate       char(10),     /* 起始时间   */
    stime       char(8),      /* 起始时间   */
    conntime    int default 0,          /* 连接时间   */
    sip         char(16) default '',  /* 源IP       */
    dip         char(16) default '',  /* 目标IP     */
    trantype    smallint default 6,     /* 协议 6:tcp */
    port        int default 0,          /* 端口号     */
    service     smallint default 0,     /* 服务内容   */
    ubytes      bigint default 0,          /* 上行字节数 */
    dbytes      bigint default 0,          /* 下行字节数 */
    flags       smallint default 0,      /* 记录处理情况 0--新 1--统计  */
    localport   int default 0,          /* 本地端口号     */
    trandirect  smallint  default 0,     /*传输方向*/
    keyword1    varchar(64) default '',
    keyword2    varchar(128) default '',
    keyword3    varchar(48) default '',
    keyword4    int default 0, 
		logid       char(24)  default '',
);

create index nasiplog_index1 on nasiplog (userid);
create index nasiplog_index2 on nasiplog (sdate);
create index nasiplog_index3 on nasiplog (sip);
create index nasiplog_index4 on nasiplog (stime);
create index nasiplog_index5 on nasiplog (dip);
create index nasiplog_index6 on nasiplog (service);
create index nasiplog_index7 on nasiplog (ubytes);
create index nasiplog_index8 on nasiplog (dbytes);


/* IP访问记录  */
drop table nasipblocklog;
create table nasipblocklog (
    userid      int,          /* 用户名     */
    sdate       char(10),     /* 起始时间   */
    stime       char(8),      /* 起始时间   */
    sip         char(16) default '',  /* 源IP*/
    dip         char(16) default '',  /* 目标IP*/
    trantype    smallint default 6,     /* 协议 6:tcp */
    port        int default 0,          /* 端口号     */
    service     smallint default 0,     /* 服务内容   */
    localport   int default 0,          /* 本地端口号*/
    trandirect  smallint  default 0     /*传输方向*/
);

create index nasipblocklog_index1 on nasipblocklog (userid);
create index nasipblocklog_index2 on nasipblocklog (sdate);
create index nasipblocklog_index3 on nasipblocklog (sip);


/* 监控的邮件内容 */
drop table nasmaillog;
create table nasmaillog (
    sid        int      auto_increment  primary key,
    userid     int,
    sdate      char(24),
    mailfile   char(128),        /* 邮件文件名      */
    mailfrom   char(128),       /* 发件人          */
    mailto     char(128),       /* 收件人          */
    mailsub    char(128),       /* 主题            */
    mailasize  bigint,          /* 附件            */
    mailsize   bigint,          /* 邮件大小 大于   */
    flags      int,             /* 邮件状态 0--发送  1--接收  2表单*/
    mtype      char(64),         /* 邮件分类        */
    rback      int  default 0,
    dip        char(24) default '0.0.0.0'
);

create index nasmaillog_ind0 on nasmaillog (sid);
create index nasmaillog_ind1 on nasmaillog (userid);
create index nasmaillog_ind2 on nasmaillog (sdate);


drop table nasformlog;
create table nasformlog (
    sid        int      auto_increment  primary key,
    userid     int,
    sdate      varchar(24),
    mailfile   varchar(128),        /* 邮件文件名      */
    mailfrom   varchar(128),       /* 发件人          */
    mailto     varchar(128),       /* 收件人          */
    mailsub    varchar(128),       /* 主题            */
    mailasize  bigint,          /* 附件            */
    mailsize   bigint,          /* 邮件大小 大于   */
    flags      int,             /* 邮件状态 0--发送  1--接收  2表单*/
    mtype      varchar(64),         /* 邮件分类        */
    rback      int  default 0,
    dip        char(24) default '0.0.0.0',
    mac        char(13) default ''
);

create index nasformlog_ind0 on nasmaillog (sid);
create index nasformlog_ind1 on nasmaillog (userid);
create index nasformlog_ind2 on nasmaillog (sdate);


/* 监控的邮件内容 */
drop table nasformlog;
create table nasformlog (
    sid        int      auto_increment  primary key,
    userid     int,
    sdate      char(24),
    mailfile   char(64),        /* 邮件文件名      */
    mailfrom   char(128),       /* 发件人          */
    mailto     char(128),       /* 收件人          */
    mailsub    char(128),       /* 主题            */
    mailasize  bigint,          /* 附件            */
    mailsize   bigint,          /* 邮件大小 大于   */
    flags      int,             /* 邮件状态 0--发送  1--接收  2表单*/
    mtype      char(64),         /* 邮件分类        */
    rback      int  default 0,
    dip        char(24) default '0.0.0.0'
);

create index nasformlog_ind0 on nasformlog (sid);
create index nasformlog_ind1 on nasformlog (userid);
create index nasformlog_ind2 on nasformlog (sdate);
create index nasformlog_ind3 on nasformlog (flags);




/* 邮件监控过滤条件  */
drop table nasmailrule;
create table nasmailrule (  
    rid        int,             /* 规则ID          */
    num        int,             /* 执行序号        */
    rname      varchar(63) not null,     /* 规则名称        */
    mailfrom   varchar(63) default '',     /* 发件人          */
    mailto     varchar(63)  default '',     /* 收件人          */
    mailsub    varchar(63)  default '',     /* 主题            */
    mailcont   varchar(63)  default '',     /* 内容            */
    mailsize   int default 0,    /* 邮件大小 大于   */
    attname    varchar(63)  default '',     /* 附件名称        */
    attsize    int default 0,   /* 附件大小 大于   */
    flags      smallint default 0,   /* 类别 0:正常发送  1:拒绝发送 2等待审核 3转发*/
    mail      varchar(63)   default '',    /* 转发邮箱        */
    sflags     smallint default 0,   /* 通知方式 0--不通知  1--通知发件人  2--通知指定信箱  */
    smail      varchar(63)   default '',    /* 指定信箱       */
    rnum       int   default 0         /*收件人数目*/
);

/* 监控的邮件内容 */
drop table nasmailauth;
create table nasmailauth (
    sid        int      auto_increment  primary key,
    userid     int,
    sdate      char(24),
    mailfile   char(64),        /* 邮件文件名      */
    intdfile   char(64),        /* 邮件头文件名      */
    mailfrom   char(128),       /* 发件人          */
    mailto     char(128),       /* 收件人          */
    mailsub    char(128),       /* 主题            */
    mailasize  bigint,          /* 附件            */
    mailsize   bigint,          /* 邮件大小 大于   */
    mtype      char(64)         /* 邮件分类        */
);
create index nasmailauth_ind1 on nasmailauth (userid);
create index nasmailauth_ind2 on nasmailauth (sdate);


drop table nasdenyuser;
create table nasdenyuser (
    userid     int
);
create index nasdenyuser_ind1 on nasdenyuser (userid);

drop table nasauthuser;
create table nasauthuser (
    userid     int
);
create index nasauthuser_ind1 on nasauthuser (userid);

drop table nasnoauthuser;
create table nasnoauthuser (
    userid     int
);
create index nasnoauthuser_ind1 on nasnoauthuser (userid);

/* 管理员信息    */
drop table nasadmuser;
create table nasadmuser (
    name       varchar(32)   primary key,   /* 用户名   */
    password   varchar(32),                 /* 口令md5(name,password)  */
    lright     varchar(128),               /* 权限  right--> lright系统目前有5个权限 123456   */
    email      varchar(64),                 /* 转发E-Mail              */
    lasttime   int,                         /* 最后一次访问时间        */
    allgroup   int,                          /* 部分部门 -1 所有部门*/
    bindip  char(32) default '',
    grouplist1 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist2 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist3 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist4 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist5 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'
);

insert into nasadmuser values('admin','qoQhDpRa4xOAwkBtuS0mq0','1111111111111111111111111111','',0,999999);

/*update nasadmuser set password='qoQhDpRa4xOAwkBtuS0mq0';*/

/*insert into nasadmuser values('xxxxx','qoQhDpRa4xOAwkBtuS0mq0','1111111111111111111111111111','',0,999999);*/

alter table nasadmuser add verifyCode   char(64)  default '';
alter table nasadmuser add verifyCode   char(64)  default '';
alter table nasadmuser add verifyCode   char(64)  default '';
alter table nasadmuser add verifyCode   char(64)  default '';



/* 网络总体流量记录*/

drop table nasflowlog;
create table nasflowlog (
    rtime       int, 
    smin        int,             
    sdate       char(10),       
    stime       char(8),        
    ubytes      bigint,            
    dbytes      bigint,   
    dev         char(12) default '', 
    httpnum     int default 0, 
    tcpnum      int default 0,
    udpnum      int default 0,
    usernum     int default 0,        
    flags       smallint default 0
);

create index nasflowlog_in1 on nasflowlog (sdate);
create index nasflowlog_in2 on nasflowlog (rtime);


/* IP统计数据  */
drop table nasipreport;
create table nasipreport (
    userid	    int,                  /* 用户ID */
    sdate       char(10),              /* 日期   */
    stime       char(2),               /* 时间   */
    ip          char(16),              /* IP地址 */
    ubytes      bigint,                   /* 上行流量  */
    dbytes      bigint,                   /* 下行流量  */
    ctime       int                       /* 连接时间  */
);

create index nasipreport_index1 on nasipreport (userid);
create index nasipreport_index2 on nasipreport (sdate);
create index nasipreport_index3 on nasipreport (stime);

drop table nassrvreport;
create table nassrvreport (
    userid	    int,                   /* 用户ID */
    sdate       char(10),              /* 日期      */
    stime       char(2),               /* 时间      */
    service     int,                   /* 服务代码  */
    ubytes      bigint,                /* 上行流量  */
    dbytes      bigint,                /* 下行流量  */
    ctime       int,                   /* 连接次数  */
    onlinetime  int default 0          /* 连接时间  */
);
create index nassrvreport_index0 on nassrvreport (userid);
create index nassrvreport_index1 on nassrvreport (sdate);
create index nassrvreport_index2 on nassrvreport (stime);

drop table nassrvdreport;
create table nassrvdreport (
    sdate       char(10),              /* 日期      */
    stime       char(2),               /* 时间      */
    port        int,                   /* 服务代码  */
    trantype    int,                   /* 服务类型  */
    ubytes      bigint,                /* 上行流量  */
    dbytes      bigint,                /* 下行流量  */
    ctime       int                    /* 连接次数  */
);

create index nassrvdreport_index1 on nassrvdreport (sdate);
create index nassrvdreport_index2 on nassrvdreport (stime);



drop table nashttpreport;
create table nashttpreport (
    userid      int,                /* 用户ID  */
    sdate       char(10),           /* 日期      */
    stime       char(2),            /* 时间      */
    host        varchar(64),        /* 网址      */
    bytes       bigint default 0,             /* 流量      */
    lcount      int default 0,             /* 点击次数  */
    ctime       int default 0,                 /* 连接时间  */
    urlsort     char(6) default '0000'
);

create index nashttpreport_index1 on nashttpreport (userid);
create index nashttpreport_index2 on nashttpreport (sdate);



drop table nasfirewall;
create table nasfirewall (
    fid	        int,                /* ID    */
    fflag       int,		    /*1:启动 0：不启动*/	
    fnumber     int,        	    /* 序号  */
    fname       varchar(30),        /* 名字  */
    fdesc       varchar(64),        /* 描述  */
    faction     int default 0,      /*0:deny 1:allow*/
    fchains     int,          	    /* 0input 1output 2forward */
    ffragment     int  default 0,   /* 应用于 1:fragment碎片 0:all package -1:非碎片包  */
    fstate        char(10) default '1111', /* 应用于 1111:所有状态 1234new established related invlaid */
    finputdev     char(20)  default '+',   /*输入设备 + eth0 !eth0.....*/
    foutputdev    char(20) default '+',    /*输入设备 + eth0 !eth0.....*/
    fprotocol     int default 0,	   /* 其他:自定, 1:icmp  6:tcp 17:udp 0:all*/
    flport        char(30) default '0',      /*local port  0:all 1:fixwd -1:!fixed*/
    frport        char(30) default '0',      /*remote port  0:all 1:fixwd -1:!fixed*/
    ficmptype     int,			     /*-1:all */
    ficmpcode     int,			     /*-1:all */
    ftcpsyn       int   default 0,         /*0:all 1:tcp请求包 -1:!syn*/
    fladd         char(50) default '0/0',  /*0/0:any*/
    fradd         char(50) default '0/0'   /*0/0:any*/
);

/* 管理员信息    */
drop table nasdbuser;
create table nasdbuser (
    name      varchar(32)   primary key,   /* 用户名   */
    password  varchar(32),                 /* 口令md5(name,password)  */
    ip     varchar(24)              /* 登陆的IP */
);

drop table nasuseraccount;
create table nasuseraccount (
    userid    int   ,   	/*时间   */
    sdate     char(10), 
    flow      bigint,
    time      int,
    account   int,
    accountflag  int default 1  /*1 计费时间  0非计费时间*/
);



drop table nasclientinfo;
create table nasclientinfo (
    gnmkey         char(32)  primary key,   /*用户key */
    Service_code   char(32)  default '',   /*用户id */
    Service_name   char(128) default '', 
    Address        char(128) default '',
    Zip            char(32) default '',
    Principal      char(64) default '',     /*责任人*/    
    Principal_id   char(32) default '',		  /*责任人身份证*/       
    Principal_tel  char(32) default '',     /*责任人电话*/
    Principal_email char(32) default '',		  /*责任人email*/
    yingyearea     int  default 0,		      /*营业面积*/
    Practitioner_number      int  default 0,		      /*从业人数*/
    Ending_number  int  default 0,		      /*主机数目*/
    Server_number  int  default 0,		      /*主机数目*/
    Infor_man      char(64) default '', 	  /* 安全人*/
    Infor_man_tel  char(64) default '',  	  /*安全人电话*/
    Infor_man_id   char(64) default '',		  /*安全人身份证*/
    Infor_man_email   char(64) default '',		  /*安全人email*/   		
    Ip             char(64) default '',		
    Net_type       char(32) default '',	    /*上网方式*/   
    clienttype     char(32) default '',     /*终端类型 0未知 1网吧 2宾馆 3企业单位 4事业单位 5教育机构 */
    producer_code  char(32) default '',     /*接入运营商代码*/ 
    Status         char(24) default '1',     /*终端状态*/ 
    Net_monitor_department char(64) default '',
    Net_monitor_man        char(64) default '',     
    Net_monitor_man_tel    char(64) default '',
    usernum        int  default 0,
    startday       char(64) default '',
    endday         char(64) default '',
    lasttime       char(64) default '',
    lastappupdatev int default 0,        
    lasturlupdatev int default 0,         
    updateflag   int  default 0,            /*是否允许升级服务*/
    lastappeupdate char(64)  default '',     
    lasturleupdate char(64)  default '',
    intervalfig    int default 3600,         /*同服务器通信时间间隔，状态包*/
    cmdupdateflag  int  default 0,           /*是否有命令*/
    cmdurl         char(128) default '',     /*命令存放的文件*/
    alarmlog       char(8) default 'N',      /*上报报警日志*/
    tdulog         char(8) default 'N',      /*上报主机上下线记录*/
    onlinelog      char(8) default 'N',      /*上报上拦截记录*/
    realidentity   char(8) default 'N',      /*上报真实身份记录*/
    virtualidentity  char(8) default 'N',    /*上报虚拟身份记录*/
    daystat        char(8) default 'N',      /*上报日统计记录*/ 
    onlinehost     int default 0,
    registname     char(64) default '',
    registuser     char(64) default '',
    registadd      char(128) default '',
    registtel      char(64) default '',
    registmail     char(64) default '',
    Remark         char(254) default '',
    registtime     char(32) default '',
    sessionid      char(32) default '',
    newsystem      char(4) default '1',
    UnitNo         int default 0,
    uploadtype     int default 0,  /*0-1  0定时 1即时*/
    uploadtimeval  int default 3600  /*上传时间间隔*/
    uploadcmd      char(64) default '',  /*指令: 目前仅仅支持ip*/
    myService_code   char(32)  default '',
    verifyCode   char(64)  default ''
);
create index nasclientinfo_index1 on nasclientinfo(gnmkey);
create index nasclientinfo_index2 on nasclientinfo(Service_code);


/*insert into nasclientinfo (gnmkey,clientid,clientname,clientadd,zherenren,zherenrenid    
zherenrentel,zherenrenemail,yingyearea,congyenum,termnum,secname,sectel,secid,secemail,
iplist,connecttype,clienttype,producer_code,clientstatus,usernum,startday,endday,lasttime,
lastappupdatev,lasturlupdatev,updateflag,lastappeupdate,lasturleupdate,intervalfig,cmdupdateflag,
cmdurl,alarmlog,tdulog,onlinelog,realidentity,virtualidentity,daystat,onlinehost)
values ('1234-1234-1234-1234','41010110000001','TEST','','','','','','','',100,'','','','',
'','DDN','','','',100,'','','','','','0','','',120,0,'','N','N','N','N','N','N',0,'','','','','');

alter table nasclientinfo add   registname     char(64) default '';
alter table nasclientinfo add   registuser     char(64) default '';
alter table nasclientinfo add   registadd      char(128) default '';
alter table nasclientinfo add   registtel      char(64) default '';
alter table nasclientinfo add   registmail     char(64) default '';

alter table nasclientinfo add   registtime     char(32) default '';
alter table nasclientinfo add   sessionid      char(32) default '';
alter table nasclientinfo add   newsystem      char(4) default '1';
alter table nasclientinfo add   UnitNo         int default 0;
alter table nasclientinfo add   add uploadtype     int default 0;
alter table nasclientinfo add uploadtimeval  int default 3600;
alter table nasclientinfo add uploadcmd      char(64) default '';
    

drop table nasclientip;
create table nasclientip (
    Service_code   char(32)  not null ,  
    cip            char(20)  not null,
    starttime      char(20)  default '', 
    endtime        char(20)  default ''
)

drop table RealIDList;
create table RealIDList (
    User_name  char(32)  default '',
		Sex               char(12)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Org_name          char(128)  default '',
		Country           char(32)  default '',
		Description       char(64)  default '',
		updateflag       int default 0
);

drop table cliRealIDList;
create table cliRealIDList (
    User_name  char(32)  default '',
		Sex               char(12)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Org_name          char(128)  default '',
		Country           char(32)  default '',
		Description       char(64)  default '',
		updateflag       int default 0
);


drop table NetIDBasicList;
create table NetIDBasicList (
    User_name         char(32)  default '',
		Sex               char(12)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Org_name          char(128)  default '',
		Country           char(32)  default '',
		Card              char(32)  default '',
		Dummy_type        char(32)  default '',
		Dummy_code        char(128)  default '',
		updateflag       int default 0
);

drop table cliNetIDBasicList;
create table cliNetIDBasicList (
    User_name         char(32)  default '',
		Sex               char(12)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Org_name          char(128)  default '',
		Country           char(32)  default '',
		Card              char(32)  default '',
		Dummy_type        char(32)  default '',
		Dummy_code        char(128)  default '',
		updateflag       int default 0
);

drop table NetIDTrackList;
create table NetIDTrackList (
    logid             char(32)  default '',
    User_name         char(32)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Dummy_type        char(32)  default '',
		Dummy_code        char(128)  default '',
		Login_time        char(32)  default '',
		Exit_time         char(32)  default '',
		Service_code      char(32)  default '',
		Net_ending_code   char(32)  default '',
		updateflag        int default 0
);


drop table cliNetIDTrackList;
create table cliNetIDTrackList (
    logid             char(32)  default '',
    User_name         char(32)  default '',
		Certificate_type  char(20)  default '',
		Certificate_code  char(20)  default '',
		Dummy_type        char(32)  default '',
		Dummy_code        char(128)  default '',
		Login_time        char(32)  default '',
		Exit_time         char(32)  default '',
		Service_code      char(32)  default '',
		Net_ending_code   char(32)  default '',
		updateflag        int default 0
);


drop table ClientList;
create table ClientList (
    logid            char(32)  default '',
    gnmkey           char(32)  default '',   
    Service_code     char(32)  default '',
		User_name        char(32)  default '',
		Certificate_type char(20)  default '',
		Certificate_code char(20)  default '',
		Online_time      char(32)  default '',
		Offline_time     char(32)  default '',
		Net_ending_name  char(32)  default '',
		Net_ending_ip    char(32)  default '',
		Net_ending_mac   char(32)  default '',
		Org_name         char(128)  default '',
		Country          char(32)  default '',
		area_code        char(12)  default '',
		node             char(64)  default '',
		updateflag       int default 0
);

drop table cliClientList;
create table cliClientList (
    logid            char(32)  default '',
    gnmkey           char(32)  default '',   
    Service_code     char(32)  default '',
		User_name        char(32)  default '',
		Certificate_type char(20)  default '',
		Certificate_code char(20)  default '',
		Online_time      char(32)  default '',
		Offline_time     char(32)  default '',
		Net_ending_name  char(32)  default '',
		Net_ending_ip    char(32)  default '',
		Net_ending_mac   char(32)  default '',
		Org_name         char(128)  default '',
		Country          char(32)  default '',
		userid           int default 0,
		area_code        char(12)  default '',
		node             char(64)  default '',
		updateflag       int default 0
);


drop table ComputerList;
create table ComputerList (
    logid          char(32)  default '',
    gnmkey         char(32)  default '',   
    Service_code   char(32)  default '',
		User_name      char(32)  default '',
		Certificate_type char(20)  default '',
		Certificate_code char(20)  default '',
		Net_ending_ip    char(32)  default '',
		Net_ending_mac   char(32)  default ''
);

drop table AccessList;
create table AccessList(
    gnmkey            char(32) default   '',   
		Service_code      char(32) default   '' ,   
		User_name         char(32)  default '',
		Certificate_type  char(12)  default '',
		Certificate_code  char(24)  default '',
		Net_ending_ip     char(32) default   '' ,
		Net_ending_name   char(32) default   '' ,
		Time              char(32) default   '' ,
    Net_ending_mac    char(32) default   '' ,
		Destination_ip    char(32) default   '' ,
		Port              char(12) default   '' ,
		Service_type      char(32) default   '' ,
		KeyWord1          char(64)  default '',
		KeyWord2          char(32)  default '',
		KeyWord3          char(24)  default '',
		updateflag        int default 0,
		pinyin            char(32)  default ''
);
insert into AccessList values('1234-1234-1234-1234','23010100000001','test1','xue','0','411081197701236858','','','','','','','0005','','','',0);


drop table AlertList;
create table AlertList(
    Matching_id       char(32) default   '' ,   
		Service_code      char(32) default   '' ,   
		Rule_id           char(32) default   '' ,  
		Rule_name         char(101) default   '' ,
		Matching_level    char(32) default   '' ,
		Matching_time     char(32) default   '' ,
		Net_ending_ip     char(32) default   '' ,
		Net_ending_code   char(32) default   '' ,
		Net_ending_mac    char(32) default   '' ,
		Destination_ip    char(32) default   '' ,
		Service_type      char(32) default   '' ,
		KeyWord1          char(64)  default '',
		KeyWord2          char(32)  default '',
		KeyWord3          char(24)  default '',
		Customer_name     char(32)  default '',
		Certificate_type  char(12)  default '',
		Certificate_code  char(24)  default '',
		Org_name          char(128)  default '',
		Country           char(12)  default '',
    updateflag       int default 0
);

drop table cliAlertList;
create table cliAlertList(
    Matching_id       char(32) default   '' ,   
		Service_code      char(32) default   '' ,   
		Rule_id           char(32) default   '' ,  
		Rule_name         char(101) default   '' ,
		Matching_level    char(32) default   '' ,
		Matching_time     char(32) default   '' ,
		Net_ending_ip     char(32) default   '' ,
		Net_ending_code   char(32) default   '' ,
		Net_ending_mac    char(32) default   '' ,
		Destination_ip    char(32) default   '' ,
		Service_type      char(32) default   '' ,
		KeyWord1          char(64)  default '',
		KeyWord2          char(32)  default '',
		KeyWord3          char(24)  default '',
		Customer_name     char(32)  default '',
		Certificate_type  char(12)  default '',
		Certificate_code  char(24)  default '',
		Org_name          char(128)  default '',
		Country           char(12)  default '',
		updateflag       int default 0
);



/**/
/*drop table nasdaystat;*/
/*create table nasdaystat (*/
/*    statday        char(64) default '', */
/*    clientonline   char(32)  default '',   */
/*    clientonlinep  char(12) default ''*/
/*);*/
/**/
/*create index nasdaystat_index1 on nasdaystat(statday);*/
/**/

/*作平均值*/
drop table nasclientdaystat;
create table nasclientdaystat (
    statday        char(32) default '', 
    Service_code       char(32)  default '',   
    hostonline     char(12) default '' 
);
create index nasclientdaystat_index1 on nasclientdaystat(statday);


drop table ClientCmd;
create table ClientCmd (
    cmdno          char(32)  primary key, 
    Service_code   char(32)  default '',
		cmdname        char(32)  default '',
		cmdmethod      char(32)  default '',
		cmdrange       char(20)  default '',
		cmdfile        char(64)  default '',
		cmdtime        char(32)  default '',
		updateflag     int default 0             /*0 not't run  1 run  2run ok  3 err*/
);


drop table ClientCmdVar;
create table ClientCmdVar (
    cmdno          char(32)  default '', 
    cmdvarname     char(32)  default '',
		cmdvartype     char(32)  default '=',
		cmdvarval      char(32)  default ''
);

drop table nasclientpunish;
create table nasclientpunish (
    punishid            char(32) default '',
    Service_code        char(32)  default '',   
    punishdate          char(20) default '',
    punishcon           char(32) default '',
    updateflag          int default 0  
);

drop table nasclientxiaoxi;
create table nasclientxiaoxi (
    xiaoxino            char(32) default '',
    Service_code        char(32)  default '',   
    xiaoxi              char(255) default '',
    xiaoxishijian       char(32) default '', 
    qiaoshou            char(128) default '', 
    qiaoshoushijian     char(32) default '',
    xioaxidirect        int default 0,
    updateflag          int default 0
);

drop table clientxiaoxi;
create table clientxiaoxi (
		Service_code        char(32)  default '',   
    xiaoxi              char(255) default '',
    xiaoxishijian       char(32) default '', 
    qiaoshou            char(128) default '', 
    qiaoshoushijian     char(32) default '',
    xioaxidirect        int default 0,
    updateflag          int default 0
);

drop table nasbkrule;
create table nasbkrule(  
    bid              int,             
    Rule_id          char(21)  not NULL,            
    Rule_name        char(101) default   '' ,          
    matching_level   char(2)   default   '' ,         
    Rule_action      char(2)   default  '',        
    Service_type     char(12)   default   '' ,
    Keyword1         char(64)  default '',
    Keyword2         char(32)  default '',
    Keyword3         char(12)   default '',
    Matching_word    char(2)   default '',
    Set_time         char(32)  default '',
    Validation_time  char(32)  default ''
);

drop table clibkrule;
create table clibkrule(  
    bid              int,             
    Rule_id          char(21)  not NULL,            
    Rule_name        char(101) default   '' ,          
    matching_level   char(2)   default   '' ,         
    Rule_action      char(2)   default  '',        
    Service_type     char(12)   default   '' ,
    Keyword1         char(64)  default '',
    Keyword2         char(32)  default '',
    Keyword3         char(12)   default '',
    Matching_word    char(2)   default '',
    Set_time         char(32)  default '',
    Validation_time  char(32)  default ''
);



drop table nasbkuser;
create table nasbkuser(  
    User_name         char(32)  not NULL,            
    Certificate_type  char(24) default   '' ,          
    Certificate_code  char(24)   default   ''
);


drop table nasbukong;
create table nasbukong(  
    bid        int primary key,             /* 布控ID    */
    bnum       char(64) default   '00' ,            /* 布控编号 */
    bman       char(64) default   '00' ,            /*布控管理员编号*/
    bname      char(63)  default  '00',         /* 布控案件名称 */
    bfuzeren      varchar(63)   default '',
    bfuzerentel   varchar(63)   default '',
    bmanager      varchar(63)   default '',
    bmanagertel   varchar(63)   default '',
    endday        varchar(32)   default '04-01-01',
    bnote         varchar(254)  default '',
    battach       varchar(128)  default '',
    bflag         int           default 0  /* 0还没有应用 1 已经应用  9999正在建立*/
);





drop table nasupdatelist;
create table nasupdatelist(           
    gnmvar              char(12)  default   '0000000000' ,            
    updatedesc          varchar(254) 
);



/*终端管理级别 */
drop table nasclientlevel;
create table nasclientlevel (
    id             int   primary key,          /*用户key */
    idnum          char(12) default '00',       /*两位的编号*/
    pid            int   default 0, 
    name           char(64) default '未知',
    mydesc         char(128) default '',
    level          int   default 0
);
create index nasclientlevel_index1 on nasclientlevel(id);

insert into nasclientlevel values('1','31','0','ShangHai','310000000000','0');   



drop table nassrvhttpblocklog;
create table nassrvhttpblocklog(
    gnmkey      char(32),
    sdate       char(10),
    stime       char(8), 
    sip         char(16),
    dip         char(16) default '',  /* 目标IP     */
    host        varchar(64),
    uri         varchar(128),
    sortid      int default -1    
);

create index nassrvhttpblocklog_index1 on nassrvhttpblocklog(gnmkey);
create index nassrvhttpblocklog_index2 on nassrvhttpblocklog(sdate);
create index nassrvhttpblocklog_index4 on nassrvhttpblocklog(sortid);




drop table nasbukonglog;
create table nasbukonglog(  
    bid        int ,            /* 布控ID    */
    alertmsg   char(254) default   '' ,    /* 布控告警内容 */
    gnmkey     char(32),
    pcname     char(64),
    alerttype  int  default  0 ,        /* 布控类型 1:mail 2:form 3:ftp 4:msn 5:qq */
    lasttime   int  default  0         /* 布控类型 */
);



/* 邮件监控过滤条件  */
drop table nasmailfilter;
create table nasmailfilter (  
    rid        int,             /* 规则ID          */
    num        int,             /* 执行序号  或  布控ID    */
    rname      varchar(63) not null,     /* 规则名称        */
    mailfrom   varchar(63) default '',     /* 发件人          */
    mailto     varchar(63)  default '',     /* 收件人          */
    mailsub    varchar(63)  default '',     /* 主题            */
    mailcont   varchar(63)  default '',     /* 内容            */
    mailsize   int default 0,    /* 邮件大小 大于   */
    attname    varchar(63)  default '',     /* 附件名称        */
    attsize    int default 0,   /* 附件大小 大于   */
    flags      smallint default 0,   /* 类别 0:发送  1:接收 2web  3mailbk 4webbk */
    sflags     smallint default 0,   	/* 通知方式 0--不通知  1--通知  2--转发  */
    smail      varchar(63)   default '',    /* 转发邮箱        */
    rnum       int   default 0,         /*收件人数目*/
    maction    int   default 1          /*监控动作1：监控  0：不监控*/
);


/* 监控的邮件内容 */
drop table nasmaillogsrv;
create table nasmaillogsrv(
    bid        int ,            /* 布控ID    */
    gnmkey     char(32),
    pcname     char(64),
    sdate      char(24),
    mailfile   char(64),        /* 邮件文件名      */
    alertmsg   char(248),       /* 发件人          */
    mtype      char(64),         /* 邮件分类        */
    alerttype  int  default 1,
    sendto     char(64)
);

create index nasmaillogsrv_ind0 on nasmaillogsrv(gnmkey);



/* 监控的邮件内容 */
drop table nasformlogsrv;
create table nasformlogsrv(
    bid        int ,            /* 布控ID    */
    gnmkey     char(32),
    pcname     char(64),
    sdate      char(24),
    mailfile   char(64),        /* 邮件文件名      */
    alertmsg   char(248),       /* 发件人          */
    mtype      char(64),         /* 邮件分类        */
    alerttype  int  default 1,
    sendto     char(64)
);

create index nasformlogsrv_ind0 on nasformlogsrv(gnmkey);




drop table nasftprule;
create table nasftprule(  
    rid        int,             /* 规则ID          */
    bid        int,              /* 布控ID    */
    num        int,             /* 执行序号        */
    rname      varchar(63)  not null,
    xfrom       varchar(63)  default '',
    hostip     varchar(24)  default '',
    flags      smallint default 0,
    sflags     smallint default 0,
    smail      varchar(63)  default '' 
);


drop table nasftprulesrv;
create table nasftprulesrv(  
    bid        int ,            /* 布控ID    */
    gnmkey     char(32),
    pcname     char(64),
    sdate      char(24),
    alertmsg   char(248),       /* 发件人          */
    mtype      char(64),         /* 邮件分类        */
    alerttype  int  default 1,
    sendto     char(64)
);


drop table nasmsnrule;
create table nasmsnrule(  
    rid        int,             /* 规则ID          */
    bid        int,              /* 布控ID    */
    num        int,             /* 执行序号        */
    rname      varchar(63)  not null,
    xfrom       varchar(63)  default '',
    flags      smallint default 0,
    sflags     smallint default 0,
    smail      varchar(63)  default '' 
);


drop table nasmsnrulesrv;
create table nasmsnrulesrv( 
    bid        int ,            /* 布控ID    */ 
    gnmkey     char(32),
    pcname     char(64),
    sdate      char(24),
    alertmsg   char(248),       /* 发件人          */
    mtype      char(64),         /* 邮件分类        */
    alerttype  int  default 1,
    sendto     char(64)
);



drop table nasqqrule;
create table nasqqrule(  
    rid        int,             /* 规则ID          */
    bid        int,              /* 布控ID    */
    num        int,             /* 执行序号        */
    rname      varchar(63)  not null,
    xfrom       varchar(63)  default '',
    flags      smallint default 0,
    sflags     smallint default 0,
    smail      varchar(63)  default '' 
);


drop table nasetrule;
create table nasetrule(  
    rid        int, 
    bid        int, 
    num        int, 
    rname      varchar(63)  not null,
    xfrom       varchar(63)  default '',
    flags      smallint default 0,
    sflags     smallint default 0,
    smail      varchar(63)  default '' 
);


drop table nasqqrulesrv;
create table nasqqrulesrv(  
     bid        int ,            /* 布控ID    */
    gnmkey     char(32),
    pcname     char(64),
    sdate      char(24),
    alertmsg   char(248),       /* 发件人          */
    mtype      char(64),         /* 邮件分类        */
    alerttype  int  default 1,
    sendto     char(64)
);




drop table nasonlineclent;
create table nasonlineclent(
    gnmkey         char(32)   primary key,   	/*用户key */
    clientid       char(128) default '', 
    clientname     char(128), 
    clientuser     char(128),
    clientadd      char(128),
    clienttel      char(128),
    clientright    int default 0, /*0 不能更新*/
    clientflag     int default 0,  /*0 不在线 1*/
    startday       char(64),
    endday         char(64),
    lasttime       char(64),
    lastupdatev    int ,        /*最后一次版本升级编号*/
    lastupdateurl  int,         /*最后一次版本升级编号*/
    dbupdateflag   int  default 0,  /*是否需要省级*/
    lastruleupdate char(64)  default '',   /*最后一次规则更新时间*/
    sysmsg         char(254)  default '无',   /*最后一次规则更新时间*/
    usernum        int  default 0,
    myname         char(128)
);
create index nasonlineclent_index1 on nasonlineclent(gnmkey);


/*insert into nasonlineclent values ('1111-1111-1111-1111','610101010000','测试用户[仅供测试]','uuuu','aaaddd','111111',1,0,'2003.10.01','2004.10.30','',800,800,0,'','正式使用请联系13701941874',50);*/

/*13903749192 */
/*内容免监控，带宽受限*/


/* 监控的邮件内容 */
drop table nasimlog;
create table nasimlog (
    sid        int      auto_increment  primary key,
    userid     int,
    sdate      char(24),
    imsubject  varchar(254),       /* 主题            */
    imcon      varchar(254),          /* 附件            */
    imfile     char(64),
    imsize     int,          /* 邮件大小 大于   */
    flags      int            /* 状态   1--Msn  2Yahoo 3AOL 4 QQ */
);

create index nasimlog_ind0 on nasimlog (sid);
create index nasimlog_ind1 on nasimlog (userid);
create index nasimlog_ind2 on nasimlog (sdate);
create index nasimlog_ind3 on nasimlog (flags);

drop table nasadmlog;
create table nasadmlog (
    name       varchar(32),      
    sdate      char(24),
    subject    varchar(254),       /*  */
    loglevel   int                 /*1,2,3,4,5*/
);


/* 带宽元素 */
drop table nasband;
create table nasband(
    sid           int      primary key,
    sname         char(128),
    bandlimit     int,
    prio          int
);

create index nasband_ind0 on nasband (sid);

insert into nasband values (0,'No-Limit','0','0');


/*虚拟身分*/
drop table nasuserv;
create table nasuserv(  
    userid     int,             /* 用户ID          */
    vtype      int,             /*1 msn 2msn-glist 3:msn-ulist 4:email 5:qq 6:et 7:ftp 8:sina webmail 9:sohu webamil   */
    vname      varchar(64)   default '',
    vdesc      varchar(254)  default '',
    vpass      varchar(63)  default '',
    vid        varchar(63)  default ''
);


drop table nasqos;
create table nasqos(
    sid           int      primary key,
    qosname       char(128) default '',
    qosrate       int default 0,  /*下载带宽数值峰值*/
    qosceil       int default 0,  /*上传带宽数值峰值*/
    qostype       int default 0, /*下载带宽保证*/
    qossfq        int default 0, /*上传带宽保证*/
    prio          int default 0,
    qosdefault    int default 0,  /*系统默认*/ 
    qoslinktype   int default 0   /*0-3 uflag:  0: 自身通道 1：默认通道  2：最快通道  3：最慢通道*/
);
create index nasqos_ind0 on nasqos (sid);

drop table nasqosrun;
create table nasqosrun(
    cid           int   ,
    sid           int   ,
    qosname       char(128) default '',
    qosrate       int default 0,
    qosceil       int default 0,
    qostype       int default 2, 
    qossfq        int default 1, 
    prio          int default 0,
    qosdefault    int default 0,   
    qoslinktype   int default 1   
);

create index nasqosrun_ind0 on nasqosrun (cid);


drop table nasqosmatch;
create table nasqosmatch(
    sid           int      primary key,
    qossrvname    char(128) default '',
    qosid         int default 0,
    prio          int default 0,
    matchtype     int default 1,  /*1234*/
    sharetype     int default 1   /*0-1*/  
    maxflow       bingint   default 0,
    maxtime       int       default 0,
    maxtcp        int       default 0,
    maxudp        int       default 0,
    moreflag      char(32)  default '0000000000',
    maxpackage int default 0;       
    maxBytes   bigint default 0;     
    maxudpp    int default 0;     
    maxtcpp    int default 0;     
    maxupp     int default 0;      
    maxdownp   int default 0;      
    maxsp      int default 0;       
    maxnc      int default 0; 
    onlybindip int default 0;      
    macbindip  int default 0;       
    onlylogone int default 0; 
);
create index nasqosmatch_ind0 on nasqosmatch (sid);



drop table nasqosmatchup;
create table nasqosmatchup(
    sid           int      primary key,
    qossrvname    char(128) default '',
    qosid         int default 0,
    prio          int default 0,
    matchtype     int default 1,  /*1234*/
    sharetype     int default 1   /*0-1*/
);

create index nasqosmatchup_ind0 on nasqosmatchup (sid);



drop table nasqosmatchsrv;
create table nasqosmatchsrv(
	sid           int,
	dst           char(32)  default '', 
  src           char(32)  default '', 
  dport         int default 0, 
  sport         int default 0 
);


drop table nasqosmatchuser;
create table nasqosmatchuser(
	sid           int,
	userid        int
);

drop table nasqosmatchpolicy;
create table nasqosmatchpolicy(
	sid           int,
	policy        int
);

drop table nasqosmatchgroup;
create table nasqosmatchgroup(
	sid           int,
	groupid       int
);



drop table nasconnectlog;
create table nasconnectlog (
    userid      int,    
    sdate       char(10),
    stime       char(8), 
    sip         char(16),
    dip         char(16) default '', 
    con        varchar(254)  
);

create index nasconnectlog_index1 on nasconnectlog (userid);
create index nasconnectlog_index2 on nasconnectlog (sdate);
create index nasconnectlog_index3 on nasconnectlog (sip);
create index nasconnectlog_index4 on nasconnectlog (stime);


drop table nasuserlog;
create table nasuserlog (
    userid      int,    
    stime       int default 0, 
    sip         int default 0,
    smac        char(13) default '', 
    shost       char(32) default '' 
);

create index nasuserlog_index1 on nasuserlog (userid);


drop table nasAdminArea;
create table nasAdminArea (
    id           int       primary key,      
    ipstart      char(16),                  
    ipend        char(16)
);


drop table nasnowconnect;
create table nasnowconnect (
    nowmin      int,    
    httpnum     int,
    tcpnum      int,
    udpnum      int,
    usernum     int 
);
insert into nasnowconnect values (0,1,1,1,1);
insert into nasnowconnect values (1,1,1,1,1);
insert into nasnowconnect values (2,1,1,1,1);
insert into nasnowconnect values (3,1,1,1,1);
insert into nasnowconnect values (4,1,1,1,1);
insert into nasnowconnect values (5,1,1,1,1);
insert into nasnowconnect values (6,1,1,1,1);
insert into nasnowconnect values (7,1,1,1,1);
insert into nasnowconnect values (8,1,1,1,1);
insert into nasnowconnect values (9,1,1,1,1);
insert into nasnowconnect values (10,1,1,1,1);
insert into nasnowconnect values (11,1,1,1,1);
insert into nasnowconnect values (12,1,1,1,1);
insert into nasnowconnect values (13,1,1,1,1);
insert into nasnowconnect values (14,1,1,1,1);
insert into nasnowconnect values (15,1,1,1,1);
insert into nasnowconnect values (16,1,1,1,1);
insert into nasnowconnect values (17,1,1,1,1);
insert into nasnowconnect values (18,1,1,1,1);
insert into nasnowconnect values (19,1,1,1,1);
insert into nasnowconnect values (20,1,1,1,1);
insert into nasnowconnect values (21,1,1,1,1);
insert into nasnowconnect values (22,1,1,1,1);
insert into nasnowconnect values (23,1,1,1,1);
insert into nasnowconnect values (24,1,1,1,1);
insert into nasnowconnect values (25,1,1,1,1);
insert into nasnowconnect values (26,1,1,1,1);
insert into nasnowconnect values (27,1,1,1,1);
insert into nasnowconnect values (28,1,1,1,1);
insert into nasnowconnect values (29,1,1,1,1);
insert into nasnowconnect values (30,1,1,1,1);
insert into nasnowconnect values (31,1,1,1,1);
insert into nasnowconnect values (32,1,1,1,1);
insert into nasnowconnect values (33,1,1,1,1);
insert into nasnowconnect values (34,1,1,1,1);
insert into nasnowconnect values (35,1,1,1,1);
insert into nasnowconnect values (36,1,1,1,1);
insert into nasnowconnect values (37,1,1,1,1);
insert into nasnowconnect values (38,1,1,1,1);
insert into nasnowconnect values (39,1,1,1,1);
insert into nasnowconnect values (40,1,1,1,1);
insert into nasnowconnect values (41,1,1,1,1);
insert into nasnowconnect values (42,1,1,1,1);
insert into nasnowconnect values (43,1,1,1,1);
insert into nasnowconnect values (44,1,1,1,1);
insert into nasnowconnect values (45,1,1,1,1);
insert into nasnowconnect values (46,1,1,1,1);
insert into nasnowconnect values (47,1,1,1,1);
insert into nasnowconnect values (48,1,1,1,1);
insert into nasnowconnect values (49,1,1,1,1);
insert into nasnowconnect values (50,1,1,1,1);
insert into nasnowconnect values (51,1,1,1,1);
insert into nasnowconnect values (52,1,1,1,1);
insert into nasnowconnect values (53,1,1,1,1);
insert into nasnowconnect values (54,1,1,1,1);
insert into nasnowconnect values (55,1,1,1,1);
insert into nasnowconnect values (56,1,1,1,1);
insert into nasnowconnect values (57,1,1,1,1);
insert into nasnowconnect values (58,1,1,1,1);
insert into nasnowconnect values (59,1,1,1,1);

drop table nasVLanArea;
create table nasVLanArea (
    id           int       primary key,  
    vlanid       smallint default 0, 
    ip           char(16) default '',             
    mask         char(16) default '',   
    dev          char(12) default ''
);



drop table arpreport;
create table arpreport (
    arpip        char(24) default '',
    arpmac       char(24) default '',             
    requestnum   int  default 0,   
    requestlasttime  char(32) default '',
    requestednum   int  default 0,   
    requestedlasttime  char(32) default ''
);


drop table nassearchkey;
create table nassearchkey(
    userid      int,                
    sdate       char(10),          
    stime       char(2),           
    host        varchar(64),        
    searchkey   varchar(128)
);

create index nassearchkey_index1 on nassearchkey(userid);
create index nassearchkey_index2 on nassearchkey(sdate);
