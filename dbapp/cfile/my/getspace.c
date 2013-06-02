// getspace.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LV  "/dev/mapper/dnfs-osd"


/* used space used K   
 * available space used K
 */

typedef struct space
{
 uint64_t used;
 uint64_t avail;
}space;


int spaceinfo(space *sinfo)
{

 FILE *fp;
 if((fp = popen("df /mnt/osd", "r")) == NULL)
 {
  perror("popen failed");
  return -1;
 }

 char buf[256];
 int i;
 for(i = 0; i < 2; i++)
 {
  fgets(buf, sizeof(buf)-1, fp);
 }
 
       
 char *p = strstr(buf, LV);
        if(p == NULL)
 {
  perror("LV not found");
  return -3;
 }
 
 p += strlen(LV);
 while(*p == ' ') p++;
 sscanf(p, "%*u %u %u", &sinfo->used, &sinfo->avail);

 if(pclose(fp) == -1)
 {
  perror("pclose error");
  return -1;
 }

 return 0;
}

int main()
{
 space s;
 spaceinfo(&s);
 printf("%u %u\n", s.used, s.avail);
 return 0;
}