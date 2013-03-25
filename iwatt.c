    time_t timer;
    struct tm *t;

                time(&timer);
                t = localtime(&timer);
                IWatt[5]=t->tm_hour;	
                IWatt[6]=t->tm_min;	
                IWatt[7]=(t->tm_year+1900)&0xff;
                IWatt[8]=(t->tm_year+1900)>>8;
                IWatt[9]=t->tm_mon+1;
                IWatt[10]=t->tm_mday;
