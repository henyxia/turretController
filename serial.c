#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>

// Globals
struct	termios saveterm;
int		serial_fd = -1;

// Defines
#define	SERIAL_DEVICE	"/dev/ttyACM0"
#define	SERIAL_SPEED	9600

// Serial port initialisation
void init_serial()
{
struct termios new;
int fd=open(SERIAL_DEVICE,O_RDWR|O_NOCTTY);
if(fd<0){perror(SERIAL_DEVICE); exit(-1);}
tcgetattr(fd,&saveterm); /* save current port settings */
bzero(&new,sizeof(new));
new.c_cflag=CLOCAL|CREAD|SERIAL_SPEED|CS8;
new.c_iflag=0;
new.c_oflag=0;
new.c_lflag=0;  /* set input mode (non-canonical, no echo,...) */
new.c_cc[VTIME]=0; /* inter-character timer unused */
new.c_cc[VMIN]=1; /* blocking read until 1 char received */
tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&new);
serial_fd = fd;
}

// Close serial port
void close_serial(){
tcsetattr(serial_fd,TCSANOW,&saveterm);
close(serial_fd);
}

// Retrieve data
unsigned char getData()
{
	char buf;
	if(read(serial_fd, &buf, 1) == 1)
		return buf;
	else
		return 255;
}
