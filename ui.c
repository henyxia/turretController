// Includes
#include "turret.h"
#include "ui.h"
#include <string.h>

// Define
#define	MAX_CHAR	75
#define	MAX_LINE	25

// Typedef
typedef char	line[MAX_CHAR+1];
typedef line	log[MAX_LINE];

// Global
log		mLog;
char	command[MAX_CHAR-4];

void displayHead(turret* myTurrets)
{
	printf(HEAD_SEPARATOR);
	printf("|");
	//TODO use real constant
	for(int i=0; i<4; i++)
 		printf("%s|", myTurrets[i].online ? TURRET_OK : TURRET_OFFLINE);
	printf("\n|");
	for(int i=0; i<4; i++)
	{
		switch(myTurrets[i].type)
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
	printf("\n");

	printf(HEAD_SEPARATOR);
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

void writeToLog(char* input)
{
	for(int i=1; i<MAX_LINE; i++)
		strcpy(mLog[i-1], mLog[i]);

	strcpy(mLog[MAX_LINE-1], input);
}

void sendCommand()
{
	writeToLog(command);
	command[0] = '\0';
	//procedeCommand(command);
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
