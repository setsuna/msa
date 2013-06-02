create database msa;

drop table msaAdminArea;
create table msaAdminArea (
    id           int       primary key,      
    ipstart      varchar(16),                  
    ipend        varchar(16)
);

drop table msaadmuser;
create table msaadmuser (
    name       varchar(32)   primary key,  
    password   varchar(32),    
    lright     varchar(128), 
    email      varchar(64),  
    lasttime   int,     
    bindip    varchar(32) default '',
    grouplist1 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist2 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist3 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist4 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    grouplist5 char(201) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'
);

insert into msaadmuser (name,password,lright,email,lasttime,bindip) values('admin','qoQhDpRa4xOAwkBtuS0mq0','1111111111111111111111111111','',0,'');

drop table msaadmlog;
create table msaadmlog (
    name       varchar(32),      
    sdate      varchar(24),
    subject    varchar(254),       /*  */
    loglevel   int                 /*1,2,3,4,5*/
);

drop table msaGroup;
create table msaGroup (
    id             int   primary key, 
    pid            int   default 0, 
    name           varchar(64) default 'DefaultGroup',
    mydesc         varchar(128) default '',
    level          int   default 0
);
create index msagroup_index1 on msaGroup(id);
create index msagroup_index2 on msaGroup(pid);
create index msagroup_index3 on msaGroup(name);

insert into msaGroup values('0','0','DefaultGroup','DefaultGroup','0');   


drop table msaPolicy;
create table msaPolicy (
    pid     int  primary key ,
    pname   varchar(48) not null,
    pdesc   varchar(64) default ''
);
insert into msaPolicy (pid,pname,pdesc) values (0,'defaultPolicy','default policy');


drop table msaUser;
create table msaUser (
    userid      int   primary key,
    username    varchar(32),       
    dispname    varchar(32), 
    Certificate_type  smallint default 0,
		Certificate_code  varchar(24)  default '',
		sex               smallint default 0,
    password    varchar(32),
    groupid0    int default 0,      /*gid */
    groupid1    int default 0,      /*gid */
    groupid2    int default 0,      /*gid */
    groupid3    int default 0,      /*gid */
    groupid4    int default 0,      /*gid */
    bindemail   varchar(64), 
    bindip      varchar(16),
    bindmac     varchar(32), 
    useflags    smallint default 0,              /*10禁止访问 9免监控 0正常*/      
    ratelimit   int  default 0,                  /*带宽*/
    ratetype    int  default 0,                  /*0共享  1独享*/
    topSrvNameList varchar(36) default '11111111111111111111111111111111111',
    addtime     bigint default 0,       
    locktime    bigint default 0,                /*该用户有效期限*/
    lasttime    bigint default 0,                /*用户最后一次上线时间*/
    lLimitBytes    bigint default 0,             /*每天最大的上网流量*/
    lLimitTimes    int default 0,                /*每天最大的上网时间*/
    lAllUpFlow     bigint default 0, 	           /*总的上行流量*/
    lAllDownFlow   bigint default 0,             /*总的下行流量*/
    lAllTime       int default 0,                /*总的上网时间*/
    lLimitBytesw   bigint default 0,             /*每星期最大的上网流量*/
    lLimitTimesw   int default 0,                /*每星期最大的上网时间*/
    lAllUpFloww    bigint default 0, 	           /*星期的上行流量*/
    lAllDownFloww  bigint default 0,             /*星期总的下行流量*/
    lAllTimew    int default 0,                  /*星期总的上网时间*
    lLimitBytesm   bigint default 0,             /*每月最大的上网流量*/
    lLimitTimesm   int default 0,                /*每月最大的上网时间*/
    lAllUpFlowm    bigint default 0, 	           /*月总的上行流量*/
    lAllDownFlowm  bigint default 0,             /*月总的下行流量*/
    lAllTimem    int default 0,                  /*月总的上网时间*/
    moreflag     varchar(32) default '00000000000000000000000000000000',
    policyid     varchar(128) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    jftype      int    default 0,  
    accountnum  float  default 0, 
    maxpackage  int    default 0, 
    maxBytes    bigint    default 0,
		maxudp     int default 0,      /* 最多udp连接 */   
    maxtcp     int default 0,      /*  最多tcp连接*/
    maxudpp    int default 0,      /*  每分最多udp包速率*/ 
    maxtcpp    int default 0,      /*  每分最多tcp包速率*/ 
    maxupp     int default 0,      /*  每分up包速率*/ 
    maxdownp   int default 0,      /*  每分down包速率*/  
    maxsp      int default 0,      /*  每分128字节小包速率*/ 
    maxnc      int default 0       /*  每分最多新建连接数目*/ 
); 
 
/* moreflag  1 noftp 2 nopost 3 nomsn 4 noqq*/  
create UNIQUE index msaUser_index on msaUser (username);
create  index msaUser_index2 on msaUser (bindip);


drop table msaqos;
create table msaqos(
    sid            int      primary key,
    level          int   default 0,
    qosname        char(128) default '',
    qosrated       int default 0,  /*下载带宽数值峰值*/
    qosrateu       int default 0,  /*上传带宽数值峰值*/
    qosmixd        int default 0, /*下载带宽保证*/
    qosmixu        int default 0, /*上传带宽保证*/
    prio          int default  0
);
create index msaqos_ind0 on msaqos (sid);

drop table nassrv;
create table nassrv(
		srvid           int       primary key,  --协议Id
		srvname         varchar(16),            --协议名称
		matchDirect     int   default 0,        --匹配方向
		matchAgree      int   default 0,        --匹配协议
		matchportl      varchar(16),             --匹配端口下
		matchportu      varchar(16),             --匹配端口上
		matchload       int   default 0,        --匹配负载
		srvip           varchar(16),            //服务器IP————类型1
		srvarea         varchar(16),            //域名解析————类型2
		srvdescmatch1   int   default 0,        //顺序起始字节下标————类型3
		srvdescmatch2   int   default 0,        //顺序截取字节数目————类型3
		srvdescmatch3   varchar(16),            //顺序截取字符16进制————类型3 判断位必须为2倍，16进制
		srvascmatchtype int   default 0,        //0 and; 1or  
		srvascmatch1    int   default 0,        //倒序起始字节下标————类型4  
		srvascmatch2    int   default 0,        //倒序起始字节数目————类型4
		srvascmatch3    varchar(16)             //倒序截取字符————类型4判断位必须为2倍，16进制
);



drop table nasurlsort;
CREATE TABLE nasurlsort (
  sortid int NOT NULL default 0,
  sorname varchar(40) NOT NULL default '',
  sortdesc varchar(100) default ''
) ;

drop table nasurls;
create table nasurls (
	urlsort int not null,
	url     varchar(36) 
);

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


drop table nasschedule;
create table nasschedule (
    schid      int   primary key,
    schname    varchar(32) not null, 
    schdesc    varchar(64) default '',
    schtime    varchar(200) default '0',
    schtime1    varchar(200) default '0'
);
insert into nasschedule values (1,'WorkDay','WorkDay','000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000','000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000'); 

drop table nasjjr;
create table nasjjr (
    jjrid      int   primary key,
    jjrname    varchar(32) not null, 
    jjrdesc    varchar(64) default '',
    jjrtime    varchar(200) default '0',
    jjrtime1   varchar(200) default '0'
);
insert into nasjjr values (1,'2011工作日','2011工作日','000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000','000000000000000000000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000111111111000000000000000000000000000000'); 



drop table nasNetArea;
create table nasNetArea (
    id            int   primary key,       /* 记录标识     */
    ipstart       varchar(16),             /* 起始IP地址   */
    ipend         varchar(16),             /* 终止IP地址   */
    netflag       smallint,      	         /* 网络标识  1--基于ip帮定   2mac帮定 3主机帮定 5NT域验证   8本地严整  99--免监控 100禁止访问 */
    ipdesp        varchar(64),
    snmpnum       int default 0,
    nohashcheck   int default 0,
    homeurl       varchar(250),
    groupid       int default 0, 
    policyid      int default 0 
);

drop table nasPortInfo;
create table nasPortInfo (
  id         int   primary key,       /* 记录标识     */	
	lPortStart int not null,
	lPortEnd   int not null,
	lFlag      int default 0, /* 该tcp端口是否被记录  3：仅仅监控 2:监控并记录  9：免监控 10 禁止访问*/
	lProp      int default 1, /* 1 tcp  2 udp */
	isLocal     int default 0  /*1 local 0 remote 2 both*/  
);


drop table msatimereport;
create table msatimereport (
    rtime       int,
		sdate       char(10),               --日期 
		hours	    	int,                    --hours
		mintue	    int,                    --mintue  
    ubytes      bigint,                 --上行总流量
		dbytes      bigint,                 --下行总流量  
		tcpconnect  int,                    --tcp 
    udpconnect  int,                    --udp  
		onlineuser  int                     --onlineuser
);

insert into  msatimereport values(12124578,'2011-08-03',8,0,25548,87456,54123,78945634,45643);
insert into  msatimereport values(12124578,'2011-08-02',9,44,352548,874565187,54623,7845634,54678);
insert into  msatimereport values(12124578,'2011-08-01',7,43,376958548,8745187,5423,7894634,79892);
insert into  msatimereport values(12124578,'2011-07-10',6,2,37958548,8745187,54123,7894564,79843);
insert into  msatimereport values(12124578,'2011-07-11',5,1,37428548,8765187,54123,7894564,57921);
insert into  msatimereport values(12124578,'2011-07-12',4,40,37258,87456587,5412,7945634,45125);
-------------------------------------------------------------------------------------
drop table msatimesrvreport;
create table msatimesrvreport (
		sdate       char(10),               -- 日期
    hours	      int,                  	-- 用户ID 
    service     int,                    -- 服务代码  
    ubytes      bigint,                 -- 上行流量  
    dbytes      bigint,                 -- 下行流量  
    onlinetime  int default 0           -- 连接时间 
);
create index msatimesrvreport_ind0 on msatimesrvreport(sdate);

drop table msaAccountReport;
create table msaAccountReport (
	userid	    int,                    /* 用户ID */
	sdate       char(10),               /* 日期      */
  ubytes       bigint,                /* 上行流量  */
  dbytes       bigint,                /* 下行流量  */
  ctime        	 int,                   /* 连接次数  */
	onlinetime       int default 0,        /* 连接时间  */
	Online_time      bigint   default  0,
	Offline_time     bigint   default  0,
	Net_ending_ip    bigint   default  0,
	Net_ending_mac  char(20)  default  '',
	account      float,
	nowaccount   float
);
create index msaAccountReport_index0 on msaAccountReport (userid);

drop table msasrvreport;
create table msasrvreport (
    userid	    int,                   /* 用户ID */
    sdate       char(10),              /* 日期      */
    service     int,                   /* 服务代码  */
		ubytes      bigint,                /* 上行流量  */
		dbytes      bigint,                /* 上行流量  */
		ctime       int default 0,
    onlinetime  int default 0          /* 连接时间  */
);
create index msasrvreport_index0 on msasrvreport (userid);
create index msasrvreport_index1 on msasrvreport (sdate);


drop table msahttpreport;
create table msahttpreport (
    sdate       char(10),           /* 日期      */
    host        varchar(64),        /* 网址      */
    bytes       bigint default 0,     /* 流量      */
    lcount      int default 0,        /* 点击次数  */
    ctime       int default 0,        /* 连接时间  */
    urlsort      int default 0
);
create index msahttpreport_index2 on msahttpreport(sdate);


drop table msauserhttpreport;
create table msauserhttpreport (
    userid	    int,               /* 用户ID */
    sdate       char(10),           /* 日期      */
    host        varchar(64),        /* 网址      */
    bytes       bigint default 0,     /* 流量      */
    lcount      int default 0,        /* 点击次数  */
    ctime       int default 0,        /* 连接时间  */
    urlsort      int default 0
);
create index msauserhttpreport_index2 on msauserhttpreport(sdate);


drop table msaDipReport;
create table msaDipReport (
		sdate       char(10),               /* 日期      */
    dip         bigint,                /* 上行流量  */
    bytes       bigint,                 /* 流量  */
    ctime        int,                  /* 连接次数  */
onlinetime    int default 0          /* 连接时间  */
);
create index msaClientList_index0 on msaDipReport (dip);



drop table msauserpolicy;
create table msauserpolicy(
    sid           int      primary key,
    policyname    char(64) default '',
    prio          float default 0,
    ratelimit   int  default 0,                  /*带宽*/
    ratetype    int  default 0,                  /*0共享  1独享*/
    topSrvNameList varchar(36) default '11111111111111111111111111111111111',
    lLimitBytes    bigint default 0,             /*每天最大的上网流量*/
    lLimitTimes    int default 0,                /*每天最大的上网时间*/
    lLimitBytesw   bigint default 0,             /*每星期最大的上网流量*/
    lLimitTimesw   int default 0,                /*每星期最大的上网时间*/
    lLimitBytesm   bigint default 0,             /*每月最大的上网流量*/
    lLimitTimesm   int default 0,                /*每月最大的上网时间*/
    moreflag     varchar(32) default '00000000000000000000000000000000',
    jftype      int    default 0,  
    accountnum  float  default 0, 
    maxpackage  int    default 0, 
    maxBytes    bigint    default 0,
		maxudp     int default 0,      /* 最多udp连接 */   
    maxtcp     int default 0,      /*  最多tcp连接*/
    maxudpp    int default 0,      /*  每分最多udp包速率*/ 
    maxtcpp    int default 0,      /*  每分最多tcp包速率*/ 
    maxupp     int default 0,      /*  每分up包速率*/ 
    maxdownp   int default 0,      /*  每分down包速率*/  
    maxsp      int default 0,      /*  每分128字节小包速率*/ 
    maxnc      int default 0,       /*  每分最多新建连接数目*/
    group0        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group1        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group2        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group3        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    policyid      varchar(129) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000' 
);
create index msauserpolicy_ind0 on msauserpolicy (sid);


/*服务控制规则*/
drop table nassrvrule;
create table nassrvrule (
    ruleid     int ,   /*规则id*/
    rulename   varchar(32) not null,
    ruleflag   smallint default 1,  /*0不启动  1：启动*/
    ruleaction smallint default 0,  /*1允许    0：拒绝 */
    ruleschedule  int   default 0,  /*0 全部时间  其他在schedule中查找*/
    rulejjr       int   default 0,  /*0 全部时间  其他在schedule中查找*/
    rulesrv0      varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    rulesrv1      varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group0        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group1        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group2        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    group3        varchar(255) default '0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    policyid      varchar(129) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    qosid         int default 0 ,   /*qosid 0或cid*/
    logaction     int default 1 ,   /*记录匹配的连接*/
    conaction     int default 1 ,   /*记录匹配的内容*/
    ruleurlsort   char(100)  default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', /*100个URL分类*/
    rulerate      float default 0,    /*rate优先级别*/
    ruleurl       char(128)
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


drop table msasearchkey;
create table msasearchkey(
    userid      int,                
    ltime       bigint,          
    host        varchar(64),        
    searchkey   varchar(128)
);

create index msasearchkey_index1 on msasearchkey(userid);
create index msasearchkey_index2 on msasearchkey(ltime);


drop table msaimlog; 
create table msaimlog (
    userid     int,
    sdate      varchar(12),  
    stime      varchar(8), 
    imfrom     varchar(48),
    imto       varchar(48),
    imcon      varchar(254),  
    flags      int      /* 状态   1--Msn  2Yahoo  4 QQ 5:淘宝旺旺 6:飞信  6:skype */
);

create index msaimlog_ind1 on msaimlog (userid);
create index msaimlog_ind2 on msaimlog (sdate);


drop table msafileloglist;
create table msafileloglist (
    sdate      varchar(12), 
    tabname    varchar(64)
);


drop table msafilelog20101022;
create table msafilelog20101022 (
    userid     int,
    tantype    int,     /*0:upload 1:http download 2:smtp file 3:pop3 file  4:msn file 5:ftp */
    stime      varchar(8), 
    filename   varchar(64),
    fileurl    varchar(128),
    contenttype   varchar(64),  
    flags      int     /* 状态   1--Msn  2Yahoo 3AOL 4 QQ 5:taobao 6:feixin  */
);

create index msafilelog20101022_ind1 on msaimlog (userid);


drop table msamailloglist;
create table msamailloglist (
    sdate      varchar(12), 
    tabname    varchar(64)
);

drop table msamaillog20101022;
create table msamaillog20101022 (
    userid     int,
    sdate      varchar(24),
    mailfile   varchar(128),        /* 邮件文件名      */
    mailfrom   varchar(128),        /* 发件人          */
    mailto     varchar(128),        /* 收件人          */
    mailsub    varchar(128),        /* 主题            */
    mailasize  bigint,           /* 附件            */
    mailsize   bigint,           /* 邮件大小 大于   */
    flags      int,              /* 邮件状态 0--pop3  1-smtp*/
    mtype      varchar(64),         /* 邮件分类        */
    rback      int  default 0,   /* key*/
    dip        varchar(24) default '0.0.0.0'
);

create index msamaillog20101022_ind0 on msamaillog20101022 (sid);
create index msamaillog20101022_ind1 on msamaillog20101022 (userid);
create index msamaillog20101022_ind2 on msamaillog20101022 (sdate);

drop table msaformloglist;
create table msaformloglist (
    sdate      varchar(12), 
    tabname    varchar(64)
);

drop table msaformlog20101022;
create table msaformlog20101022 (
    userid     int,
    sdate      varchar(24),
    formfile   varchar(128),        
    formhost   varchar(128),
    formurl    varchar(128),
    formasize  bigint,           /* 附件            */
    formsize   bigint,           /* 大小 大于   */
    mtype      varchar(64),         /* 分类        */
    rback      int  default 0,   /* key*/
    dip        varchar(24) default '0.0.0.0'
);
create index msaformlog20101022_ind0 on msaformlog20101022 (sid);
create index msaformlog20101022_ind1 on msaformlog20101022 (userid);

/* 邮件监控过滤条件  */
drop table msamailfilter;
create table msamailfilter (  
    rid        int,             /* 规则ID          */
    num        float,             /* 执行序号  或  布控ID    */
    rname      varchar(63) not null,       /* 规则名称        */
    mailfrom   varchar(63) default '',     /* 发件人          */
    mailto     varchar(63)  default '',    /* 收件人          */
    mailsub    varchar(63)  default '',    /* 主题            */
    mailcont   varchar(63)  default '',    /* 内容            */
    mailsize   int default 0,              /* 邮件大小 大于   */
    attname    varchar(63)  default '',    /* 附件名称        */
    attsize    int default 0,              /* 附件大小 大于   */
    flags      smallint default 0,         /* 类别 0:发送  1:接收 */
    sflags     smallint default 0,   	     /* 通知方式 0--不通知  1--通知  2--转发  */
    smail      varchar(63)   default '',   /* 转发邮箱        */
    rnum       int   default 0,            /*收件人数目*/
    maction    int   default 1             /*监控动作1：监控  0：不监控*/
);

/* 邮件监控过滤条件  */
drop table msaformfilter;
create table msaformfilter (  
    rid        int,             /* 规则ID          */
    num        float default 0,             /* 执行序号  或  布控ID    */
    rname      varchar(63) not null,       /* 规则名称        */
    formfrom   varchar(63) default '',     /* 用户名称包括          */
    formhost   varchar(63)  default '',    /* 主机名称包括          */
    formurl    varchar(63)  default '',    /* url包括            */
    formlcont  varchar(63)  default '',    /* 内容            */
    formsize   int default 0,              /* 大小 大于   */
    attname    varchar(63)  default '',    /* 附件名称包括        */
    attsize    int default 0,              /* 附件大小 大于   */
    flags      smallint default 0,         /* 类别 0:手工建立  1:中心自动派发 */
    sflags     smallint default 0,   	     /* 通知方式 0--不通知  1--通知  2--转发  */
    smail      varchar(63)   default '',   /* 转发邮箱        */
    maction    int   default 1             /*监控动作1：监控  0：不监控*/
);

drop table naskeyword;
create table naskeyword(
    kid    int   primary key,
    hostkeyword   varchar(50),
    keyword   varchar(50),
    utf8  smallint    default 0,  /*  0-非勾选   1- 勾选*/ 
    web   smallint    default 0,   /*0-非勾选   1- 勾选 */
    type   smallint  default 0        /*  0-表单 1-网页*/ 
);


/*虚拟身份控制规则*/
drop table nasvidrule;
create table nasvidrule (
    ruleid     int ,   --规则id
    rulevid    varchar(32) not null,
    ruletype   smallint default 1 --1qq 2 msn 3ftp 4pop3 5smtp收件人 
);

