rootdiskinfo=0;
appdiskinfo=0;
{
		FILE *fp;
		char *p;
		char caBuffer[132];
		char caDev[32];
		char caRate[32];
		char caMount[32];
		unsigned long long lTotal,lUsed,lFree;
		fp = popen("/sbin/df -k","r");
		if(fp){
			p = fgets(caBuffer,131,fp);
			p = fgets(caBuffer,131,fp);
			while(p) {
				//printf("p:%s\n",p);
				sscanf(caBuffer,"%s %lld %lld %lld %s %s",
					caDev,&lTotal,&lUsed,&lFree,caRate,caMount);
				if(strcmp(caMount,"/")==0) {
					rootdiskinfo=(lUsed*100)/lTotal;
				}
				if(strcmp(caMount,"/app")==0) {
					appdiskinfo=(lUsed*100)/lTotal;
					break;
				}
				p = fgets(caBuffer,132,fp);
			}
			pclose(fp);
	    }
	
}

{
	FILE *fp;
	char *ppppp;
	 
	int  iii;
	int  jjj;
	char *p;
	char caBuffer[256];
	freemem=0;
	allmem=0;
	unsigned long long xxxmem;
	 
	system("cat /proc/meminfo > /app/ns/bin/mymem");
	fp=fopen("/app/ns/bin/mymem","r");
	memset(caBuffer,0,256);
	p=fgets(caBuffer,255,fp);
	if(p){
		if(strncmp(caBuffer,"MemTotal:",9)==0 ){
			ppppp=caBuffer;
			iii=strlen(ppppp);
			for(jjj=0;jjj<iii;jjj++){
				if(caBuffer[jjj]!=' ' && caBuffer[jjj]!='k' && caBuffer[jjj]!='B' ){
				*ppppp=caBuffer[jjj];
				ppppp++;
			  }
			}
			*ppppp='\0';
			ppppp=caBuffer;
			ppppp=ppppp+9;
			allmem=atol(ppppp)/1000;
		}
	}
	memset(caBuffer,0,256);
	p=fgets(caBuffer,255,fp);
	if(p){
		if(strncmp(caBuffer,"MemFree:",8)==0 ){
			ppppp=caBuffer;
			iii=strlen(ppppp);
			for(jjj=0;jjj<iii;jjj++){
				if(caBuffer[jjj]!=' ' && caBuffer[jjj]!='k' && caBuffer[jjj]!='B' ){
				*ppppp=caBuffer[jjj];
				ppppp++;
			  }
			}
			*ppppp='\0';
			ppppp=caBuffer;
			ppppp=ppppp+8;
			freemem=atol(ppppp)/1000;
		}
	}
	xxxmem=(allmem-freemem)*100;
	mempp=xxxmem/allmem;
	if(mempp<0 ){
	   mempp=20;	
	}
	if(mempp>85 ){
	   mempp=85;	
	}
}
