#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdbool.h>
#include "serial.h"

// Globals
struct	termios saveterm;
int		serial_fd = -1;

// Defines
//#define	SERIAL_DEVICE	"/dev/ttyACM0"
//#define	SERIAL_SPEED	9600

// Serial port initialisation
bool init_serial(char* device, int speed)
{
	struct termios new;

	int fd=open(device,O_RDWR|O_NOCTTY);
	
	if(fd<0)
		//perror(SERIAL_DEVICE); exit(-1);
		return false;

	tcgetattr(fd,&saveterm); /* save current port settings */
	bzero(&new,sizeof(new));
	new.c_cflag=CLOCAL|CREAD|speed|CS8;
	new.c_iflag=0;
	new.c_oflag=0;
	new.c_lflag=0;  /* set input mode (non-canonical, no echo,...) */
	new.c_cc[VTIME]=0; /* inter-character timer unused */
	new.c_cc[VMIN]=1; /* blocking read until 1 char received */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&new);
	serial_fd = fd;
	sleep(5);
	return true;
}

// Close serial port
void close_serial()
{
	tcsetattr(serial_fd,TCSANOW,&saveterm);
	close(serial_fd);
}

// Retrieve data
unsigned char getData()
{
	write(serial_fd, "?", 1);
	char buf;
	if(read(serial_fd, &buf, 1) == 1)
		return buf;
	else
		return 255;
}

// Process data through the UI
void processSerial(turret* myT, char* device, int speed)
{
	writeToLog("Please wait while the Arduino is rebooting", false);
	printf("\x1b[2J\x1b[H");
	displayHead(myT);
	displayArduinoConsole(0);
	displayLog();
	displayConsole();

	if(init_serial(device, speed))
	{
		bool stop = false;

		while(!stop)
		{
			unsigned char	data = getData();
			fd_set			events;
			bool			target[MAX_TURRET] = {false, false, false, false};
			unsigned char	tmp;

			struct timeval mTimeout = {0, 50000};

			target[0] = (data & 0x01) == 0x01;
			target[1] = (data & 0x02) == 0x02;
			target[2] = (data & 0x04) == 0x04;
			target[3] = (data & 0x08) == 0x08;

			for(int i=0; i<MAX_TURRET; i++)
				if(target[i])
				{
					if((data & 0x10) == 0x10)
						execute(myT, T_FIRE, i);
					else if((data & 0x60) == 0x20)
						execute(myT, T_LEFT, i);
					else if((data & 0x60) == 0x40)
						execute(myT, T_RIGHT, i);
					else if((data & 0x60) == 0x60)
						execute(myT, T_BOTTOM, i);
					else if((data & 0x80) == 0x80)
						execute(myT, T_TOP, i);
					else
						execute(myT, T_STOP, i);
				}

			FD_ZERO(&events);
			FD_SET(STDIN_FILENO, &events);

			if(select(STDIN_FILENO + 1, &events, NULL, NULL, &mTimeout) == -1)
				writeToLog("Select not responding properly", false);
			if(FD_ISSET(STDIN_FILENO, &events))
			{
				do
				{
					tmp = getc(stdin);
				}while(tmp != '\n');
				stop = true;
				writeToLog("Done", false);
			}

			printf("\x1b[2J\x1b[H");
			displayHead(myT);
			displayArduinoConsole(data);
			displayLog();
			displayConsole();
		}
	}
	else
		writeToLog("Seleted port was not opened successfully", false);
	close_serial();
}
