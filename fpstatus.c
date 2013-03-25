#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "fplug.h"

int fp_main( int, char );

int main(int argc, char *argv[])
{
    int fd;  
    struct termios oldtio, newtio;

    if ( argc == 1 ) {
        printf("Usage: fpstatus <option>\n");
        return(0);
    }
    if ( *argv[1] == '-' ) argv[1]++;
    
    fd = open(SERIAL_PORT, O_RDWR );
    
    tcgetattr( fd, &oldtio );
    newtio = oldtio;
    newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    tcsetattr(fd, TCSANOW, &newtio);

    fp_main(fd, *argv[1]);
    
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);    

    return 0;
}

int fp_main( int fd, char mode )
{
    int cmd;
    int cnt;
    int a=0;
    double f;
    int ret;
    int i=0;
    unsigned char buf[256];
    time_t timer;
    struct tm *t;

    switch(mode) {
        case 't':
            while (1) {
                write(fd, Temp, sizeof(Temp) );
                ret=read(fd, buf, 256);
                if ( buf[5]==0x11 & buf[10]==0x72 & buf[13]==2  ) break;
                if ( i++ >10 ) return(0);
            }
            f = (buf[14]+(buf[15]<<8))/10;
            printf( "%.1f\n", f );
            break;
        case 'h':
            while (1) {
                write(fd, Humid, sizeof(Humid) );
                ret = read(fd, buf, 256);
                if ( buf[5]==0x12 & buf[10]==0x72 & buf[13]==1 ) break;
                if ( i++ >10 ) return(0);
            }
            a = buf[14];
            printf( "%d\n", a );
            break;
        case 'i':
            while (1) {
                write(fd, Illum, sizeof(Illum) );
                ret = read(fd, buf, 256);
                if ( buf[5]==0x0d & buf[10]==0x72 & buf[13]==2 ) break;
                if ( i++ >10 ) return(0);
            }
            a = buf[14]+(buf[15]<<8);
            printf( "%d\n", a );
            break;
        case 'w':
            while (1) {
                write(fd, RWatt, sizeof(RWatt) );
                ret = read(fd, buf, 256);
                if ( buf[5]==0x22 & buf[10]==0x72 & buf[13]==2 ) break;
                if ( i++ >10 ) return(0);
            }
            f = ((double)buf[14]+(double)(buf[15]<<8))/10;
            printf( "%.1f\n", f );
            break;
        default:
            break;
    }
}
