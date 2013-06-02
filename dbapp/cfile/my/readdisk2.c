/* 根据设备名取剩余磁盘空间  */
int ltFsGetDiskSpaceD(char *pDev,long *lTotal,long *lUsed,long *lFree)
{
    FILE *fp;
    char *p;
    char caBuffer[132];
    char caDev[32];
    char caRate[32];
    char caMount[32];
    fp = popen("df -k","r");
    if(fp) {
        p = fgets(caBuffer,131,fp);
        p = fgets(caBuffer,131,fp);
        while(p) {
            sscanf(caBuffer,"%s %d %d %d %s %s",
                caDev,lTotal,lUsed,lFree,caRate,caMount);
            if(p=strstr(caDev,pDev)) {
                pclose(fp);
                return 0;
            }
            p = fgets(caBuffer,132,fp);
        }
        pclose(fp);
    }
    return (-1);
}