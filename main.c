// Libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include "turret.h"
#include "ui.h"

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
int			requestValue[3] = {0x0002, 0x0300, 0x0200};
int			requestIndex[3] = {0x0001, 0x0000, 0x0000};
bool		stop = false;

void stopProgram()
{
	stop = true;
}

/*
int sendControlData(int target, int in_data)
{
		unsigned char data[1];
		int ret;
		
		data[0]=in_data%256;
		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], data, 1, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
			return -1;
		}
		printf("Success ! Amount of data sent : %d, with data : %02x\n", ret, data[0]);
		return 0;
		
}

int recieveInterruptData(int target)
{
		unsigned char data[1];
		int ret;
		
		int bytes_in = 0;
		data[0]=0xFF;
		ret=libusb_interrupt_transfer(tourettes[target].handle, 0x81, data, 1, &bytes_in, TIMEOUT);
		if(ret!=0)
			printf("Try to recieve data with 106.6 ! Error code %d\n", ret);
		else
			printf("Success ! Data recevied %d\n", data[0]);

		return ret;

}

void commandTurret(int target, int cmd)
{
	if(tourettes[target].type == TYPE_CHESEN)
	{
		sendControlData(target, 0x40);
		recieveInterruptData(target);
		sendControlData(target, cmd);
		sendControlData(target, 0x40);
		recieveInterruptData(target);
	}
	else if(tourettes[target].type == TYPE_WINBOND)
	{
		unsigned char datamain[5] = {0x5f, cmd, 0xe0, 0xff, 0xfe};
		unsigned char datamain1[5] = {0x5e, 0x00, 0x00, 0x00, 0x00};
		unsigned char datamain2[5] = {0x5c, 0x00, 0x00, 0x00, 0x00};
		unsigned char data[1];
		int ret;

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain1, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain1, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);
	
		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain2, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain2, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		data[0] = 0x00;
			ret=libusb_control_transfer(tourettes[target].handle, 0xa1, 0x01,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], data, 1, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d, with data : %02x\n", ret, data[0]);
	
	}
	else if(tourettes[target].type == TYPE_TENX)
	{
		unsigned char datamain[8] = {'U', 'S', 'B', 'C', 0x00, 0x00, 0x08, 0x08};
		unsigned char datamain1[8] = {'U', 'S', 'B', 'C', 0x00, 0x00, 0x04, 0x00};
		unsigned char datamain2[8] = {'U', 'S', 'B', 'C', 0x00, 0x40, 0x02, 0x00};
		unsigned char data[1];
		int ret;

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain1, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain2, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

		ret=libusb_control_transfer(tourettes[target].handle, REQUEST_TYPE, REQUEST,
				requestValue[tourettes[target].type], requestIndex[tourettes[target].type], datamain, 5, TIMEOUT);
		if(ret<0)
		{
			printf("You should try sending data without error %d\n", ret);
		}
		printf("Success ! Amount of data sent : %d\n", ret);

	}
	else
		printf("Turret type not recognized\n");
}

void populateTurrets()
{
	libusb_device	**list;
	ssize_t			cnt = libusb_get_device_list(NULL, &list);
	int				ret;
	
	struct libusb_device_descriptor dDevice;
	
	if (cnt < 0)
		printf("Unable to get USB device list, error : %d\n", cnt);
	else
		printf("%d USB devices detected\n", cnt);

	for (int i = 0; i < cnt; i++)
	{
		libusb_device *device = list[i];
		printf("Bus %03d ", libusb_get_bus_number(device));
		printf("Device %03d: ", libusb_get_device_address(device));
		
		ret = libusb_get_device_descriptor(device, &dDevice);
		// ret is always equal 0 since 1.0.16

		printf("ID %04x:%04x\n", dDevice.idVendor, dDevice.idProduct);
		//TODO Add vendor and device from usb.ids
		
		if(VENDOR_ID_1 == dDevice.idVendor && PRODUCT_ID_1 == dDevice.idProduct)
		{
			printf("Spotted ");
			if(actual_fcking_tourette < MAX_TOURETTE)
			{
				ret = libusb_open(device, &(tourettes[actual_fcking_tourette].handle));
				if(!ret)
				{
					tourettes[actual_fcking_tourette].type = TYPE_CHESEN;
					printf("and BURSTED !!! You will be mine forever !\n");
					actual_fcking_tourette++;
				}
				else
				{
					printf("and, well it's not you, I promise, it's me...\n");
					printf("Error code %d\n", ret);
				}
			}
			else
				printf("and ... What ! Am I already full ? God dammit !\n");
		}
		else if(VENDOR_ID_2 == dDevice.idVendor && PRODUCT_ID_2 == dDevice.idProduct)
		{
			printf("Winbond turret detected");
			if(actual_fcking_tourette < MAX_TOURETTE)
			{
				ret = libusb_open(device, &(tourettes[actual_fcking_tourette].handle));
				if(!ret)
				{
					tourettes[actual_fcking_tourette].type = TYPE_WINBOND;
					printf("and BURSTED !!! You will be mine forever !\n");
					actual_fcking_tourette++;
				}
				else
				{
					printf("and, well it's not you, I promise, it's me...\n");
					printf("Error code %d\n", ret);
				}
			}
			else
				printf("and ... What ! Am I already full ? God dammit !\n");
		}
		else if(VENDOR_ID_3 == dDevice.idVendor && PRODUCT_ID_3 == dDevice.idProduct)
		{
			printf("Spotted ");
			if(actual_fcking_tourette < MAX_TOURETTE)
			{
				ret = libusb_open(device, &(tourettes[actual_fcking_tourette].handle));
				if(!ret)
				{
					tourettes[actual_fcking_tourette].type = TYPE_TENX;
					printf("and BURSTED !!! You will be mine forever !\n");
					actual_fcking_tourette++;
				}
				else
				{
					printf("and, well it's not you, I promise, it's me...\n");
					printf("Error code %d\n", ret);
				}
			}
			else
				printf("and ... What ! Am I already full ? God dammit !\n");
		}
		else
			printf("Not recognized !\n");

	}

	libusb_free_device_list(list, 1);
}
*/
void initializeTurrets(turret* myTurrets)
{
	for(int i=0; i<MAX_TURRET; i++)
	{
		myTurrets[i].online = false;
		myTurrets[i].type = TYPE_NONE;
	}
}

char getch(){
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
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
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

void ui(turret* myTurrets)
{
	displayHead(myTurrets);
	displayLog();
	displayConsole();
	//printf("\x1b[2A\x1b[3C");
	//fflush(stdout);
}

/*
void ui()
{
	//Vars
	char userInput[64];
	int target = 0;

	// Setting terminal to raw input
	struct termios tio;
	tcgetattr( 0, &tio );
	tio.c_lflag &= ~ICANON;
	tcsetattr( 0, TCSANOW, &tio );

	// Reading user input
	do
	{
		system("clear");
		
		// Displaying UI
		system("clear");
		printf("*-----------*\n");
		printf("|     Z     |\n");
		printf("|     +     |\n");
		printf("|     |     |\n");
		printf("|Q +-- --+ D|\n");
		printf("|     |     |\n");
		printf("|     +     |\n");
		printf("|     S     |\n");
		printf("*-----------*\n");
		printf("\nFire : T\nExit : E\n");
		printf("R: Next turret\nF: Previous turret\n");
		printf("Tourette %d\n", target);

		scanf("%c", userInput);
		switch(userInput[0])
		{
			case 'z':
				commandTurret(target, TOP_SIR);
				break;
			case 'd':
				commandTurret(target, LEFT_SIR);
				break;
			case 'q':
				commandTurret(target, RIGHT_SIR);
				break;
			case 's':
				commandTurret(target, BOTTOM_SIR);
				break;
			case 't':
				commandTurret(target, SHOOTHIMDOWN);
				break;
			case 'r':
				target++;
				break;
			case 'f':
				target--;
				break;
			default:
				commandTurret(target, RESET_SIR);
		}
	}while(userInput[0]!='e');
}*/

void mode_raw(int activer) 
{ 
    static struct termios cooked; 
    static int raw_actif = 0; 
  
    if (raw_actif == activer) 
        return; 
  
    if (activer) 
    { 
        struct termios raw; 
  
        tcgetattr(STDIN_FILENO, &cooked); 
  
        raw = cooked; 
        cfmakeraw(&raw); 
        tcsetattr(STDIN_FILENO, TCSANOW, &raw); 
    } 
    else 
        tcsetattr(STDIN_FILENO, TCSANOW, &cooked); 
  
    raw_actif = activer; 
}

int main(void)
{
	char		command;
	int			target = 0;
	int			ret;
	turret		myTurrets[MAX_TURRET];

	initLog();
/*
	// Setting terminal to raw input
	struct termios tio;
	tcgetattr( 0, &tio );
	tio.c_lflag &= ~ICANON;
	tcsetattr( 0, TCSANOW, &tio );
*/

	//TODO
	//Reset terminal at exit

	initializeTurrets(myTurrets);
/*
	struct libusb_config_descriptor* dConfig = NULL;

	ret = libusb_init(NULL);
	if(ret != 0)
	{
		printf("Error while initializing libusb, return : %d\n", ret);
		return -1;
	}

	printf("Starting lsusb things ...\n");

	populateTurrets();

	libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);

	printf("\nNext, configuration stuff !\n\n");

	if(actual_fcking_tourette<1)
	{
		printf("WHERE IS THE F@CKING TOURETTE ?\n");
		return -1;
	}

	for(int i=0; i<actual_fcking_tourette; i++)
	{
		ret = libusb_get_config_descriptor(libusb_get_device(
					tourettes[i].handle), 0, &dConfig);
		if(ret!=0)
		{
			printf("I definitely should'nt have a look inside that ...\n");
			return -1;
		}
		
		printf("Configuration value for the element %d: %d\n", i,
				dConfig->bConfigurationValue);
		
		ret = libusb_detach_kernel_driver(tourettes[i].handle, 0);
		
		if(ret!=0)
		{
			printf("Device not detached, this will be complicated ...\n");
			printf("Error code %d\n", ret);
			printf("Maybe the device is already detached ?\n");
		}
		else
			printf("Device successfully detached\n");

		ret = libusb_set_configuration(tourettes[i].handle,
				(int) dConfig->bConfigurationValue);
		
		if(ret!=0)
		{
			printf("You don't understand what you're manipulating uh ?\n");
			printf("Error code %d\n", ret);
			return -1;
		}
		
		printf("Element %d now configured !\n", i);

		ret = libusb_claim_interface(tourettes[i].handle, 0);

		if(ret!=0)
		{
			printf("Claiming failed ... Maybe you're not so much powerfull\n");
			printf("Error code %d\n", ret);
			return -1;
		}

		while(!stop)
		{
			system("clear");
			printf("Que voulez-vous faire ?\n");
			fgets(command, 256, stdin);
			sscanf(command, "%s\n", command);
			printf("Commande recue : %s\n", command);
			if(!strcmp(command, "left"))
			{
				printf("Going left\n");
				commandTurret(target, LEFT_SIR);
			}
			else if(!strcmp(command, "right"))
			{
				printf("Going right\n");
				commandTurret(target, RIGHT_SIR);
			}
			else if(!strcmp(command, "top"))
			{
				printf("Going top\n");
				commandTurret(target, TOP_SIR);
			}
			else if(!strcmp(command, "bottom"))
			{
				printf("Going bottom\n");
				commandTurret(target, BOTTOM_SIR);
			}
			else if(!strcmp(command, "reset"))
			{
				printf("Going to RESET\n");
				commandTurret(target, RESET_SIR);
			}
			else if(!strcmp(command, "fire"))
			{
				printf("FIRE !!!!\n");
				commandTurret(target, SHOOTHIMDOWN);
			}
			else if(!strcmp(command, "exit"))
			{
				stop = true;
				printf("Have a nice day !\n");
			}
			else if(!strcmp(command, "ui"))
				ui();
			else
			{
				printf("Non !\nListe des commandes disponibles\n");
				printf("\tleft\n\tright\n\ttop\n\tbottom\n\treset\n\treset\n");
			}
			printf("Appuyez sur ENTRER pour continuer\n");
			fgets(command, 256, stdin);
		}

	}

	libusb_exit(NULL);
*/
	do
	{
		ui(myTurrets);
		// 27 91 65 Top
		// 27 91 66 Bottom
		// 126 DEL
		// 127 BKSP
		// 97 - 122 a-z
		// 65 - 90 A-Z
		command = getch();
		if(command == 127)
			// Delete previous char
			removeLastChar();
		else if(command >= 65 && command <= 90)
			// Add char with translation
			writeConsole(command + 32);
		else if((command >= 97 && command <= 122) || command == 32)
			// Add char	or space
			writeConsole(command);
		else if((command >= 48 && command <= 57))
			// Add number
			writeConsole(command);
		else if(command == 10)
			// Send command
			sendCommand();
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
				{
					// Bell
				}
			}
			else
			{
				// Bell
			}
		}
		else
		{
			// Bell
		}
		printf("\x1b[2J\x1b[H");
		fflush(stdout);
	}while(!stop);

	return 0;
}
