// Libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include "serial.h"

//Defines
#define	REQUEST			0x09
#define	REQUEST_TYPE	0x21
#define	TIMEOUT			500
#define	RESET_SIR		0x00
#define	TOP_SIR			0x02
#define	BOTTOM_SIR		0x01
#define	LEFT_SIR		0x04
#define	RIGHT_SIR		0x08
#define	SHOOTHIMDOWN	0x10
/*
//Typedef
typedef struct
{
	libusb_device_handle*	handle;
	int						type;
}tourette;
*/
//Globals
//tourette	tourettes[MAX_TOURETTE];
//int			actual_fcking_tourette = 0;
bool		stop = false;

void stopProgram()
{
	stop = true;
}

void ui(turret* myTurrets)
{
	displayHead(myTurrets);
	displayArduinoConsole(getData());
	displayLog();
	displayConsole();
}

int initializeTurrets(turret* myTurrets)
{
	for(int i=0; i<MAX_TURRET; i++)
	{
		myTurrets[i].status = STATUS_OFFLINE;
		myTurrets[i].type = TYPE_NONE;
		myTurrets[i].cmd = T_NONE;
	}

	if(!init(myTurrets))
	{
		printf("Initialization failed\n");
		return -1;
	}

	return 0;
}

char getch()
{
    char buf=0;
	/*
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
	*/
    if(read(0,&buf,1)<0)
        perror("read()");
	/*
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
	*/
    return buf;
}

int main(void)
{
	char	command;
	fd_set	events;
	struct	timeval mTimeout = {0, 50000};
	turret	myTurrets[MAX_TURRET];
	int		ret;
    struct	termios old={0};
	struct timeval tv = {0, 0};

	initLog();

    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");

	if(initializeTurrets(myTurrets) != 0)
		return -1;

	do
	{
		ui(myTurrets);

		FD_ZERO(&events);
		FD_SET(STDIN_FILENO, &events);

		mTimeout.tv_usec = 50000;

		if(select(STDIN_FILENO + 1, &events, NULL, NULL, &mTimeout) == -1)
			writeToLog("Select not responding properly", false);
		if(FD_ISSET(STDIN_FILENO, &events))
		{
			command = getch();
			if(command == 127)
				// Delete previous char
				removeLastChar();
			else if(command >= 65 && command <= 90)
				// Add char
				writeConsole(command);
			else if((command >= 97 && command <= 122) || command == 32 || command == 47)
				// Add char	or space or slash
				writeConsole(command);
			else if((command >= 48 && command <= 57))
				// Add number
				writeConsole(command);
			else if(command == 10)
				// Send command
				sendCommand(myTurrets);
			else if(command == 27)
			{
				// Check for escape sequence
				command = getch();
				if(command == 91)
				{
					// Escape sequence confirmed
					// Getting control
					command = getch();
					if(command == 65)
					{
						// History top
					}
					else if(command == 66)
					{
						// History bottom
					}
					else
						printf("\a");
				}
				else
					printf("\a");
			}
			else
				printf("\a");
		}
		ret = libusb_handle_events_timeout_completed(NULL, &tv, NULL);
		if(ret != LIBUSB_SUCCESS)
			writeToLog("Handle events failed", false);
		printf("\x1b[2J\x1b[H");
		fflush(stdout);
	}while(!stop);

    old.c_lflag|=ICANON;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");

	closeTurrets(myTurrets);

	for(int i=0; i<MAX_TURRET; i++)
		if(myTurrets[i].status == STATUS_ONLINE)
			libusb_close(myTurrets[i].handle);

	libusb_exit(NULL);

	return 0;
}
