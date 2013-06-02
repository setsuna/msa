/*.网络次数分析*/
int connectreport(char date1[36],char date2[36],int userid,int gid,int lUnit,char caTempDir[256],int lang)
{
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024]; 
    struct nasFlowCount_s {
        char          caItem[16];
        unsigned long long lDbytes;
        unsigned long long lUbytes;
        unsigned long long lTbytes;
        unsigned long long lConTime;
    } sCount[35];

    unsigned long iMaxItem;
    //int lSize;
    unsigned long long lUbytes,lMaxBytes;
    int      i,iMax;
    long lStep,lTime,lTime1,lTime2,lTime0;
    char caSdate[32],caTime[32],caItem[16];
    char caSdate1[32],caSdate2[32],caTime1[32],caTime2[32];
    char caLabel[256];
    char caTemp[8192];
 
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    dbPtr=lt_dbinit();
    
    char caEdate[32];
    char reportname[123];
    char *lt_page_content=NULL;
    int  iFd;
    unsigned long long *lTop=NULL;
    int  iTemp;
    char  oldgname[128];
	char  egname[128];
    int   topuser=100;
    unsigned long long  lMax;
    iMaxItem=0;
    
    ltDbConn *G_DbCon;
    printf("connectreport start....................\n");
	printf("date1:%s\n",date1);
	printf("date2:%s\n",date2);
	printf("userid:%d\n",userid);
	printf("gid:%d\n",gid);
	printf("lUnit:%d\n",lUnit);
	printf("caTempDir:%s\n",caTempDir);
	printf("lang:%d\n",lang);
    dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
    if (G_DbCon==NULL){
		printf("report:can't open db\n");
		_exit(0);
	}
    
    sprintf(caSdate,"%s",date1);
    sprintf(caEdate,"%s",date2);
    
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
        lTime1 = nasCvtLongTime(caSdate,"00:00:01");
    }else {
        lTime1 = 0;
    }
    
    if(!ltStrIsSpaces(caEdate)) {
        lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
        lTime2 = lTime;
    }
  
    sprintf(reportname,"report%d",lUnit);
    if(reportname==NULL){
		sprintf(reportname,"******");
    }

    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {            
            return 0;
        }
    }
    
    nasCvtStime(lTime,caSdate1,caTime1);
    caTime1[5]=0;
    tempCursor=NULL;
    if(lUnit==1){/*天*/
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
            memcpy(sCount[i].caItem,caSdate+8,2);
            lTime0 = lTime0 + lStep;
        }
        nasTimeGetDate(caSdate1,lTime1);
        nasTimeGetDate(caSdate2,lTime2);
        nasTimeGetTime(caTime1,lTime1);
	    nasTimeGetTime(caTime2,lTime2);
        caTime1[2]=0;
	    caTime2[2]=0;
		
		if(gid==-1){
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport where ((substring(sdate ,3,8) > '%s') or(substring(sdate ,3,8) = '%s' and hours>='%s')) and ((substring(sdate,3,8) < '%s') or(substring(sdate ,3,8) = '%s' and hours<='%s')) group by sdate",caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
	    }else{
			if(userid!=-1){
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport where userid = %d and ((substring(a.sdate ,3,8) > '%s') or(substring(a.sdate ,3,8) = '%s' and hours>='%s')) and ((substring(a.sdate ,3,8) < '%s') or(substring(a.sdate ,3,8) = '%s' and hours<='%s')) group by sdate ",userid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
			}else{
				sprintf(sqlBuf,"select sdate,sum(ubytes),sum(dbytes) from msatimereport a,msauser b where a.userid = b.userid and b.groupid0=%d and ((substring(a.sdate ,3,8) > '%s') or(substring(a.sdate ,3,8) = '%s' and hours>='%s')) and ((substring(a.sdate ,3,8) < '%s') or(substring(a.sdate ,3,8) = '%s' and hours<='%s'))group by sdate ",gid,caSdate1,caSdate1,caTime1,caSdate2,caSdate2,caTime2);
			}       
		}
		printf("sql:%s\n",sqlBuf);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		//小时 已删除
    }else if(lUnit==3){/*用户*/       
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

		if(gid==-1){
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte, c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by a.userid,b.dispname,c.name order by abyte desc limit %d",caSdate1,caSdate2,topuser);
		}else{
			sprintf(sqlBuf,"select a.userid,b.dispname,sum(ctime) abyte,c.name from msasrvreport a,msauser b,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id and b.groupid0=%d group by a.userid,b.dispname,c.name order by abyte desc limit %d",caSdate1,caSdate2,gid,topuser);
		}
		printf("sql:%s\n",sqlBuf);
        tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);     
    }else if(lUnit==6) {
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));
		sprintf(sqlBuf,"select b.dispname,c.name,sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and  b.groupid0=c.id group by b.dispname,c.name order by mybytes desc",caSdate1,caSdate2);
		printf("sql:%s\n",sqlBuf);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }else if(lUnit==7) {
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));		  
		sprintf(sqlBuf,"select b.dispname,c.name,sum(a.ubytes+dbytes)/1000+1,sum(a.ctime)  as mybytes from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and b.groupid0=c.id group by b.dispname,b.groupid0,c.name order by b.groupid0,mybytes desc",caSdate1,caSdate2);
		printf("sql:%s\n",sqlBuf);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
	}else if(lUnit==8) {
		nasTimeGetDate(caSdate1,lTime1);
		nasTimeGetDate(caSdate2,lTime2);
		memset(oldgname,0,sizeof(oldgname));
		memset(egname,0,sizeof(egname));  
		sprintf(sqlBuf,"select c.name,a.sdate,sum(a.ubytes+dbytes)/1000+1,sum(a.ctime) as mybytes from msasrvreport a,msauser b ,msagroup c where substring(a.sdate ,3,8)>= '%s' and substring(a.sdate ,3,8)<= '%s' and a.userid = b.userid and  b.groupid0=c.id group by a.sdate,c.name order by a.sdate,mybytes desc",caSdate1,caSdate2);
		printf("sql:%s\n",sqlBuf);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
    }   
    if(tempCursor == NULL) {
		sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		system(caLabel);/*建立目录和正在处理标志*/
		return 0;
    }
    
    if(lUnit==1 || lUnit==2 ){ 
	    lMaxBytes = 1;
	    iMax = 0;
	    tempRow= ltDbFetchRow(tempCursor);
	    while(tempRow!=NULL){
	        sprintf(caItem,"%s",tempRow[0]);
			lUbytes=atoll(tempRow[1]);
	        if(lMaxBytes <= lUbytes){
	           lMaxBytes = lUbytes;
	        }
	        for(i=0;i<iMaxItem;i++){
	            if(strcmp(sCount[i].caItem,caSdate+8)==0){
	                if(iMax < i+1) iMax = i+1;
	                sCount[i].lUbytes = lUbytes ;
	                break;
	            }
	        }
	        tempRow= ltDbFetchRow(tempCursor);
	    }
    
	    ltDbCloseCursor(tempCursor);
	    iMaxItem = iMax;
        /* 结果输出  */
        /*  生成图表  */    
		sprintf(caLabel,"%sdata.ini",caTempDir);
		iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	    if(iFd == (-1)){
		    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
		    system(caLabel);/*建立目录和正在处理标志*/
		    ltDbCloseCursor(tempCursor);
		    return 0;
		}	

		if(lUnit==1){  
			  lt_dbput_rootvars(dbPtr,1,"xunit","Day");
		}else{
			  lt_dbput_rootvars(dbPtr,1,"xunit","Hour");
		}
    
		tablePtr=lt_dbput_table(dbPtr,"list2");
		for(i=0;i<iMaxItem;i++) {
			lt_dbput_recordvars(tablePtr,2,"mytime",LT_TYPE_STRING,sCount[i].caItem,"request",LT_TYPE_LONGLONG,sCount[i].lUbytes);
			sprintf(caLabel,"%s,%llu\r\n",sCount[i].caItem,sCount[i].lUbytes);
			write(iFd, caLabel, strlen(caLabel));	
		}

		close(iFd);	
		if(lUnit==1){   	 
		   sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreportn1_1.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		   system(caLabel);	 
		   lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_1.jpg");
		}else{
			 sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreportn1_2.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMaxBytes+1));
		   system(caLabel);	 
		   lt_dbput_rootvars(dbPtr,1,"file1","flowreportn1_2.jpg");
		}
   
		if(lang==0){//Ch
			 lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/requestreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			   lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/requestreportplt_en.htm",dbPtr->doc,0);
		}
			
    }else if(lUnit==3){ 
		iTemp=0;
		lMax=0;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");   
		tablePtr=lt_dbput_table(dbPtr,"flowlist");
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			lt_dbput_recordvars(tablePtr,4,
				"iIndex",LT_TYPE_LONG,iTemp,
				"uname",LT_TYPE_STRING,tempRow[1],
				"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"groupname",LT_TYPE_STRING,tempRow[3]
			);				
			lTop[iTemp]=atoll(tempRow[2]); 
			if(lTop[iTemp]>lMax){
				lMax=lTop[iTemp];
			}
			iTemp++;			
			tempRow= ltDbFetchRow(tempCursor);
		}
		
		ltDbCloseCursor(tempCursor);
		sprintf(caLabel,"%sdata.ini",caTempDir);
		iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
		if(iFd == (-1)){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
		  system(caLabel);/*建立目录和正在处理标志*/
		  ltDbCloseCursor(tempCursor);
		  return 0;
		}
			
		for(i=0;i<iTemp;i++){
			ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
			sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
			write(iFd, caLabel, strlen(caLabel));	    
		}
		close(iFd);		  
		sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv1.pl  %sdata.ini %sflowreportnum.gif %llu ",caTempDir,caTempDir,lMax);
		system(caLabel);	
		lt_dbput_rootvars(dbPtr,1,"piechart","<img src=\"flowreportnum.gif\">" );		
		    
		{ 
			int i;
			sprintf(caLabel,"%s--%s",caSdate,caSdate);
			lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
			lt_dbput_rootvars(dbPtr,1,"reportname",reportname);   
			sprintf(caLabel,"%sdata.ini",caTempDir);
			iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
			if(iFd == (-1)){
				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir); 
			system(caLabel);/*建立目录和正在处理标志*/
			ltDbCloseCursor(tempCursor);
				return 0;
			}	
			for(i=0;i<iTemp;i++) { 
				ltStrAddF(caTemp,"<value>%llu</value>\n",lTop[i]);
				sprintf(caLabel,"%d,%llu\r\n",i+1,lTop[i]);
				write(iFd, caLabel, strlen(caLabel));	
			}
			close(iFd);				 
			sprintf(caLabel,"/app/msa/msa/htmlplt/Scripts/onlinesrv-bar.pl  %sdata.ini %sflowreport1s.jpg %llu ",caTempDir,caTempDir,(unsigned long long)(lMax+1));
			system(caLabel);	
			lt_dbput_rootvars(dbPtr,1,"file1","flowreport1s.jpg"); 
		}
		
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userrequestreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/userrequestreportplt_en.htm",dbPtr->doc,0);
		}
		
	}else if(lUnit==6||lUnit==7||lUnit==8 ){ 
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"flowlist");    
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			if(lUnit==6 ){
				lt_dbput_recordvars(tablePtr,4,
				"uname",LT_TYPE_STRING," ",
				"groupname",LT_TYPE_STRING,tempRow[1],
				"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
				"ltime",LT_TYPE_STRING,tempRow[3]
				);
				sprintf(oldgname,"%s",tempRow[1]);		  
			}else{ 				
				if(strcmp(oldgname,tempRow[1])==0){
					memset(egname,0,sizeof(egname));
					memset(egname,' ',strlen(oldgname));
					lt_dbput_recordvars(tablePtr,4,
					"uname",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,egname,
					"lbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
					"ltime",LT_TYPE_STRING,tempRow[3]
					);
				}else{
					lt_dbput_recordvars(tablePtr,4,
					"*",LT_TYPE_STRING,tempRow[0],
					"groupname",LT_TYPE_STRING,tempRow[1],
					"lbyte",LT_TYPE_STRING," ",
					"ltime",LT_TYPE_STRING," "
					);
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
		sprintf(caLabel,"%s--%s",caSdate1,caSdate2);
		lt_dbput_rootvars(dbPtr,1,"lab",caLabel);
		lt_dbput_rootvars(dbPtr,1,"reportname",reportname);   
		if(lang==0){//Ch
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/grouprequestreportplt.htm",dbPtr->doc,0);
		}else if(lang==1){//Eng
			lt_page_content=ltPltFileParse("/app/msa/msa/htmlplt/reportcenter/grouprequestreportplt_en.htm",dbPtr->doc,0);
		}
	}
	
   	if(lt_page_content==NULL){
	    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
    	system(caLabel);
		lt_dbfree(dbPtr);
		return 0;
	}else{
	    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	    sprintf(caLabel,"%sflownum%d.htm",caTempDir,lUnit);
		iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
		if(iFd >0) {
        write(iFd,lt_page_content,strlen(lt_page_content));
		}
		close(iFd);
	  /*生成html格式报告*/
    }
    lt_dbfree(dbPtr);
	printf("connectreport end...................\n");
    return 0;
}
 
 