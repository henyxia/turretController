// Includes
#include "serial.h"
#include "stopProgram.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Define
#define	MAX_LINE	25

// Typedef
typedef char	line[MAX_CHAR+1];
typedef line	log[MAX_LINE];

// Global
log		mLog;
char	command[MAX_CHAR-4];

void dummy()
{
}

void displayHead(turret* myT)
{
	printf(HEAD_SEPARATOR);
	printf("|");
	//TODO use real constant
	for(int i=0; i<4; i++)
 		printf("%s|", myT[i].status == STATUS_ONLINE ? TURRET_OK : TURRET_OFFLINE);
	printf("\n|");
	for(int i=0; i<4; i++)
	{
		switch(myT[i].type)
		{
			case TYPE_WINBOND:
				printf(TURRET_WINBOND);
				break;
			case TYPE_CHESEN:
				printf(TURRET_CHESEN);
				break;
			case TYPE_TENX:
				printf(TURRET_TENX);
				break;
			default:
				printf(TURRET_NONE);
		}
		printf("|");
	}
	printf("\n|");

	for(int i=0; i<4; i++)
	{
		if(myT[i].cmd == T_LEFT)
			printf(" \x1b[7m\u2190\x1b[0m");
		else
			printf(" \u2190");
		if(myT[i].cmd == T_TOP)
			printf("\x1b[7m\u2191\x1b[0m");
		else
			printf("\u2191");
		if(myT[i].cmd == T_BOTTOM)
			printf("\x1b[7m\u2193\x1b[0m");
		else
			printf("\u2193");
		if(myT[i].cmd == T_RIGHT)
			printf("\x1b[7m\u2192\x1b[0m ");
		else
			printf("\u2192 ");
		if(myT[i].cmd == T_STOP)
			printf(" \x1b[7mStop\x1b[0m ");
		else
			printf(" Stop ");
		if(myT[i].cmd == T_FIRE)
			printf(" \x1b[7mFire\x1b[0m |");
		else
			printf(" Fire |");
	}

	printf("\n");

	printf(HEAD_SEPARATOR);
}

void displayArduinoConsole(unsigned char in)
{
	printf("| \u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
	printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510");
	printf("      Arduino Based Gamepad    RAW INPUT : %03d / 255  |\n", in);
	printf("| \u2502   \u2191          2   \u2502                       ");
	printf("        TURRET SELECTED : NONE |\n");
	printf("| \u2502 \u2190   \u2192      4   1 \u2502                                                      |\n");
	printf("| \u2502   \u2193          3   \u2502                                                      |\n");
	printf("| \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518                                                      |\n");
	printf("*-------------------------------------");
	printf("--------------------------------------*\n");
}

void initLog()
{
	// Clearing the log
	for(int i=0; i<MAX_LINE; i++)
		for(int j=0; j<=MAX_CHAR; j++)
			mLog[i][j] = '\0';

	// Endind the log the right way
	//for(int i=0; i<MAX_LINE; i++)
	//	mLog[i][MAX_CHAR] = '\0';

	// Initializing command
	/*
	// Old style
	for(int i=0; i<MAX_CHAR-2; i++)
		command[i] = ' ';
	command[MAX_CHAR-2] = '\0';
	*/
	command[0] = '\0';
}

void displayLog()
{
	for(int i=0; i<MAX_LINE; i++)
	{
		printf("|%s\x1b[%dG|\n", mLog[i], MAX_CHAR+2);
	}
}

void displayConsole()
{
	printf(CONSOLE_SEPARATOR);
	printf("|> %s\x1b[%dG|\n", command, MAX_CHAR+2);
	printf("%s\x1b[2A\x1b[%dC", CONSOLE_SEPARATOR, 3+strlen(command));
	fflush(stdout);
}

void removeLastChar()
{
	int i=0;
	while(i<(MAX_CHAR-4) && command[i]!='\0')
		i++;
	if(i == 0)
	{
		// Bell
	}
	else
		command[i-1]='\0';
}

void writeToLog(char* input, bool userInput)
{
	for(int i=1; i<MAX_LINE; i++)
		strcpy(mLog[i-1], mLog[i]);

	if(userInput)
	{
		char tmp[MAX_CHAR];
		sprintf(tmp, "> %s", input);
		strcpy(mLog[MAX_LINE-1], tmp);
	}
	else
		strcpy(mLog[MAX_LINE-1], input);
}

void usage()
{
	writeToLog("Available commands :", false);
	writeToLog("clear\t\tDelete log", false);
	writeToLog("help\t\tDisplay this screen", false);
	writeToLog("stop N\t\tMake the turret N stop", false);
	writeToLog("top N\t\tMake the turret N go to the top", false);
	writeToLog("bottom N\tMake the turret N go to the bottom", false);
	writeToLog("left N\t\tMake the turret N go to the left", false);
	writeToLog("right N\tMake the turret N go to the right", false);
	writeToLog("fire N\t\tMake the turret N fire", false);
	writeToLog("serial N M\tControl turrets from serial N at M bauds", false);
	writeToLog("serial\t\tControl turrets from /dev/ttyACM0 at 9600 bauds",
			false);
	writeToLog("exit\t\tExit this program", false);
}

void procedeCommand(turret* myT, char* cmd)
{
	if(strcmp(cmd, "clear") == 0)
		initLog();
	else if(strcmp(cmd, "help") == 0)
		usage();
	else if(strcmp(cmd, "exit") == 0)
		stopProgram();
	else
	{
		char	action[MAX_CHAR];
		char	serialTarget[MAX_CHAR];
		int		speed;
		int		turret;

		if(sscanf(cmd, "%s %d", action, &turret) == 2)
		{
			if(strcmp(action, "top") == 0)
				execute(myT, T_TOP, turret);
			else if(strcmp(action, "bottom") == 0)
				execute(myT, T_BOTTOM, turret);
			else if(strcmp(action, "left") == 0)
				execute(myT, T_LEFT, turret);
			else if(strcmp(action, "right") == 0)
				execute(myT, T_RIGHT, turret);
			else if(strcmp(action, "stop") == 0)
				execute(myT, T_STOP, turret);
			else if(strcmp(action, "fire") == 0)
				execute(myT, T_FIRE, turret);
			else
			{
				writeToLog("Command not recognized", false);
				usage();
			}
		}
		else if(sscanf(cmd, "%s", action) == 1)
		{
			if(strcmp(action, "serial") == 0)
			{
				strcpy(command, "Press enter to stop serial reading");
				processSerial(myT, "/dev/ttyACM0", 9600);
			}
			else
			{
					writeToLog("Command not recognized", false);
					usage();
			}
		}
		else if(sscanf(cmd, "%s %s %d", action, serialTarget, &speed) == 3)
		{
			strcpy(command, "Press enter to stop serial reading");
			processSerial(myT, serialTarget, speed);
		}
		else
		{
			writeToLog("Command not recognized", false);
			usage();
		}
	}
}

void sendCommand(turret* myT)
{
	writeToLog(command, true);
	procedeCommand(myT, command);
	command[0] = '\0';
}

void writeConsole(char in)
{
	int i = 0;
	while(i<(MAX_CHAR-4) && command[i]!='\0')
		i++;
	if(i == MAX_CHAR)
	{
		// Bell
		printf("FULL\n");
	}
	else
	{
		command[i] = in;
		command[i+1] = '\0';
		//printf("\x1b[1A|> %s\x1b[%dG|\n", command, MAX_CHAR+2);
	}
}
