// Libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include "serial.h"
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
	
	//ui(myTurrets);

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

int main(void)
{
	char		command;
	turret		myTurrets[MAX_TURRET];

	initLog();

	if(initializeTurrets(myTurrets) != 0)
		return -1;

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
		printf("\x1b[2J\x1b[H");
		fflush(stdout);
	}while(!stop);

	return 0;
}
