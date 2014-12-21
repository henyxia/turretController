// Protector
#ifndef __UI_H__
#define	__UI_H_

// Define
#define MAX_CHAR	75

// Includes
#include "frames.h"
#include "lang_enGB.h"
#include "turret.h"

// Prototypes
void	displayHead(turret*);
void	initLog();
void	displayLog();
void	displayConsole();
void	removeLastChar();
void	writeConsole(char);
void	sendCommand(turret*);
void	writeToLog(char*, bool);
void	resetScreen();

#endif
