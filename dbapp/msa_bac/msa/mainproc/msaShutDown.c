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

void die(int e)
{ 
	_exit(e); 
}

void catch_hup(int signum)
{
	return ;
}

int main(int argc,char *argv[])
{
    
    char  caCmd[256];

	  char *p;
	int  intMaxShmSize;
	int  intShmKey;
  msaPubInfo *ltPubInfo;
  lt_shmHead   *lt_MMHead;
   
  /*系统信号识别*/
  signal(SIGHUP,catch_hup);
  signal(SIGPIPE,SIG_IGN);
  signal(SIGCHLD,SIG_IGN);
  signal(SIGTERM,SIG_IGN);
  
  /*读取最大共享内存数*/
	p=bcCnfGetValueS(_pubconfdir,"kernel", "maxshmsize");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
		die(51);
	}
	intMaxShmSize=atol(p);

	/*读取系统shmKey*/
	p=bcCnfGetValueS(_pubconfdir,"kernel", "shmkey");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
		die(52);
	} 
	intShmKey=atol(p);
	
	lt_MMHead=msaopenShmMem(intShmKey,intMaxShmSize);
	if(lt_MMHead==NULL){
		fprintf(stderr,"can't open share memory!\n");
		die(53);
	}
  
  ltPubInfo=(msaPubInfo  *)lt_MMHead->ShmMemPtr;
	if(ltPubInfo == NULL) {
	 	  fprintf(stderr,"can't open share memory pubinfo!\n");
	    die(54);
	}
	 
  msaInitPubVar(lt_MMHead);
	  /*读取最大共享内存数*/
  
    semctl(_ltPubInfo->_dbusernamehead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_dbuseriphead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_dbusermachead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_onlineuserhead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_urlhashhead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_tcpsessionhead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_httpbufferhead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_dipbufferhead.locksemid,0, IPC_RMID, 0);
    semctl(_ltPubInfo->_2SrvInfohead.locksemid,0, IPC_RMID, 0);

    system("killall -9 msaLocalServer");
    system("killall -9 msaMainProc");
     
    sprintf(caCmd,"ipcrm -M %s",bcCnfGetValueS(_pubconfdir,"kernel", "shmkey"));    
    system(caCmd);
	
    return 0;
}


