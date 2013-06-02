int  FastMemmem2(unsigned char * a, int alen, unsigned char * b, int blen)
{
    int tag[256];
    int i, ja, jb, match;

    if(alen<blen){
       return -1;
    }

    for (i = 0; i < 256; ++ i)//初始化数组
    {
        tag[i] = 0;
    }

    for (i = 0; i < blen; ++ i)
    {
    	  /*b[i]<=256
    	    tag[b[0]]=1;
    	    tag[b[1]]=1;
    	    
    	  */
        tag[b[i]] = 1;
    }

    for (i = 0;i<=(alen-blen) ;)//无限循环
    {
         for(ja=i+blen-1,jb=blen-1,match=1;jb>=0;--ja,--jb)
         {
           if(!tag[a[ja]]){
                i = ja;
                match = 0;
                break;
           }

           if(match && a[ja] != b[jb]){
              match = 0;
              break;//可以不要break,性能可能会更好
           }
         }

         if(match){
            return i;
         }

          ++ i;
    }

    return -1;
}
