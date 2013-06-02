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

insert into msaadmuser values('admin','qoQhDpRa4xOAwkBtuS0mq0','1111111111111111111111111111','',0,'');

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

insert into msaGroup values('0','0','默认部门','默认部门','0');   

drop table msaGroupip;
create table msaGroupip (
    groupid    int,
    lip        bigint default 0 ,   
	  lipend     bigint default 0 
);

drop table msaPolicy;
create table naspolicy (
    pid     int  primary key ,   /*policy id*/
    pname   varchar(48) not null,
    pdesc   varchar(64) default "",  /*desc*/
);
insert into msaPolicy (pid,pname,pdesc) values (0,'default-Policy','default policy');

drop table msaPolicyip;
create table msaPolicyip (
    pid    int,
    lip        bigint default 0 ,   
	  lipend     bigint default 0 
);


drop table msaUser;
create table msaUser (
    userid      int   primary key,
    username    varchar(32), /*用户名*/
    dispname    varchar(32),  /*显示的名字*/
    Certificate_type  smallint default 0,
		Certificate_code  varchar(24)  default '',
		sex               smallint default 0,
    password    varchar(32),
    groupid0    int default 0,      /*gid */
    groupid1    int default 0,      /*gid */
    groupid2    int default 0,      /*gid */
    groupid3    int default 0,      /*gid */
    groupid4    int default 0,      /*gid */
    bindemail   varchar(64), /*绑定的email地址*/
    bindip      varchar(16), /*绑定的ip地址*/
    bindmac     varchar(32), /*绑定的mac地址*/
    useflags    smallint default 0,     /*10禁止访问 9免监控 0正常*/      
    ratelimit   int  default 0, /*带宽限制*/
    ratelower   int  default 0, /*带宽限制*/ 
    addtime     bigint default 0,     /*该用户有效期限*/   
    locktime    bigint default 0,     /*该用户有效期限*/
    lasttime    bigint default 0,     /*用户最后一次上线时间*/
    lAllUpFlow  bigint default 0, 	 /*总的上行流量*/
    lAllDownFlow  bigint default 0, /*总的下行流量*/
    lAllTime    int default 0,   /*总的上网时间*/
    lLimitBytes  bigint default 0, /*每天最大的上网流量*/
    lLimitTimes  int default 0, /*每天最大的上网时间*/
    lLimitBytess bigint default 0, /*每天最大的上网流量*/
    lLimitTimess int default 0, /*每天最大的上网时间*
    lLimitBytesm bigint default 0, /*每天最大的上网流量*/
    lLimitTimesm int default 0, /*每天最大的上网时间*
    moreflag     varchar(32) default '00000000000000000000000000000000',
    policyid     varchar(128) default '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
    jftype      int    default 0,  
    accountnum  int    default 0, 
    maxpackage  int    default 0, 
    maxBytes    bigint    default 0,
		maxudp     int default 0,      /* 最多udp连接 */   
    maxtcp     int default 0,      /*  最多tcp连接*/
    maxudpp    int default 0,      /*  每分最多udp包速率*/ 
    maxtcpp    int default 0,      /*  每分最多tcp包速率*/ 
    maxupp     int default 0,      /*  每分up包速率*/ 
    maxdownp   int default 0,      /*  每分down包速率*/  
    maxsp      int default 0,       /*  每分128字节小包速率*/ 
    maxnc      int default 0       /*  每分最多新建连接数目*/ 
); 
 
/* moreflag  1 noftp 2 nopost 3 nomsn 4 noqq*/  
    
    
create UNIQUE index nasuser_index on nasuser (username);
    
create  index nasuser_index2 on nasuser (bindip);
create  index nasuser_index3 on nasuser (bindmac);
create  index nasuser_index4 on nasuser (groupid);


\d msaadminarea;
\d msaadmlog;
\d msaadmuser;
\d msagroup;
\d msagroupip;
\d msapolicy;
\d msapolicyip;
\d msauser
select *from msaadminarea;
select *from msaadmlog;
select *from msaadmuser;
select *from msagroup;
select *from msagroupip;
select *from msapolicy;
select *from msapolicyip;
select *from msauser;