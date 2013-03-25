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

int testmenu( int );
void dump_message( char *, char *, int );
void command_menu();
void iwatt_24hour( time_t, unsigned char * );

int main(int argc, char *argv[])
{
    int fd;  
    struct termios oldtio, newtio;
    
    fd = open(SERIAL_PORT, O_RDWR );
    
    tcgetattr( fd, &oldtio );
    newtio = oldtio;
    newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    tcsetattr(fd, TCSANOW, &newtio);

    testmenu(fd);
    
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);    

    return 0;
}

int testmenu( int fd )
{
    int cmd;
    int cnt;
    int a=0;
    double f;
    int ret;
    unsigned char buf[256];

    while(1) {
        command_menu();
        scanf("%d", &cmd);
        switch(cmd) {
            case 1:
                write(fd, Temp, sizeof(Temp) );
                dump_message( "Temp Command:", Temp, 14 );
                ret=read(fd, buf, 256);
                printf( "ret size=%d\n", ret );
                dump_message( "Temp Status:", buf, ret );
                f = (buf[14]+(buf[15]<<8))/10;
                printf( "Temperature=%.1f\n", f );
                break;
            case 2:
                write(fd, Humid, sizeof(Humid) );
                ret = read(fd, buf, 256);
                printf( "ret size=%d\n", ret );
                dump_message( "Humid Status:", buf, ret );
                a = buf[14];
                printf( "Humidity=%d%\n", a );
                break;
            case 3:
                write(fd, Illum, sizeof(Illum) );
                ret = read(fd, buf, 256);
                printf( "ret size=%d\n", ret );
                dump_message( "Illum Status:", buf, ret );
                a = buf[14]+(buf[15]<<8);
                printf( "Illumination=%d\n", a );
                break;
            case 4:
                write(fd, RWatt, sizeof(RWatt) );
                ret = read(fd, buf, 256);
                printf( "ret size=%d\n", ret );
                dump_message( "Realtime Wattmeter:", buf, ret );
                f = ((double)buf[14]+(double)(buf[15]<<8))/10;
                printf( "Watt(Realtime)=%.1fW\n", f );
                break;
            case 5:
                return(0);
            default:
                break;
        }
        printf("\n");
    }
}

void dump_message( char *mes, char *buf, int size  )
{
    int cnt;
    printf( mes );
    for ( cnt=0; cnt<size; cnt++ ) {
        printf( "%02x,", buf[cnt] );
    }
    printf( "\n" );
    return;
}

void command_menu()
{
    printf("1.Get Temperature\n");
    printf("2.Get Humidity\n");
    printf("3.Illumination\n");
    printf("4.Watt(Realtime)\n");
    printf("5.Exit\n");
    printf("Input command no ->");
    return;
}
