// Include
#include "serial.h"

// Globals
int         requestValue[3] = {0x0002, 0x0300, 0x0200};
int         requestIndex[3] = {0x0001, 0x0000, 0x0000};

int eventHotplug(libusb_context *ctx, libusb_device *device,
		libusb_hotplug_event event, void *tmpT)
{
	turret*	myT;
	myT = tmpT;

	if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
	{
		writeToLog("An USB device has been removed", false);
		for(int i=0; i<MAX_TURRET; i++)
			if(myT[i].status != STATUS_OFFLINE)
				if(device == libusb_get_device(myT[i].handle))
				{
					libusb_close(myT[i].handle);
					myT[i].status = STATUS_OFFLINE;
					myT[i].type = TYPE_NONE;
					myT[i].handle = NULL;
				}
	}
	else
	{
		char	tmp[MAX_CHAR];
		writeToLog("An USB device has been plugged", false);
		int i=0;
		while(i<MAX_TURRET && myT[i].status == STATUS_ONLINE)
			i++;
		if(i == MAX_TURRET)
			writeToLog("Cannot handle more turrets", false);
		else
		{
			struct	libusb_config_descriptor* dConfig = NULL;
			struct	libusb_device_descriptor dDevice;
			int		ret;

			libusb_get_device_descriptor(device, &dDevice);
			if((VENDOR_ID_1 == dDevice.idVendor && PRODUCT_ID_1 == dDevice.idProduct) ||
				(VENDOR_ID_2 == dDevice.idVendor && PRODUCT_ID_2 == dDevice.idProduct) ||
				(VENDOR_ID_3 == dDevice.idVendor && PRODUCT_ID_3 == dDevice.idProduct))
			{
				sprintf(tmp, "Bus %03d Device %03d: ID %04x:%04x",
						libusb_get_bus_number(device),
						libusb_get_device_address(device), dDevice.idVendor,
						dDevice.idProduct);
				writeToLog(tmp, false);
				ret = libusb_open(device, &(myT[i].handle));
				if(ret != 0)
				{
					sprintf(tmp, "Unable to open this device, error %d", ret);
					writeToLog(tmp, false);
				}
				else
				{
					myT[i].status = STATUS_ONLINE;
					if(VENDOR_ID_1 == dDevice.idVendor)
						myT[i].type = TYPE_CHESEN;
					else if(VENDOR_ID_2 == dDevice.idVendor)
						myT[i].type = TYPE_WINBOND;
					else
						myT[i].type = TYPE_TENX;

					ret = libusb_get_config_descriptor(libusb_get_device(
						myT[i].handle), 0, &dConfig);
					if(ret!=0)
					{
						writeToLog("Descriptor for this device unavailable", false);
						myT[i].status =	STATUS_ERROR;
					}
					else
					{
						for(int j=0; j<dConfig->bNumInterfaces; j++)
						{
							if(libusb_kernel_driver_active(myT[i].handle, j) &&
								(libusb_detach_kernel_driver(myT[i].handle, j) != 0))
							{
								writeToLog("Unable to detach this device", false);
								myT[i].status = STATUS_ERROR;
							}
						}
						if(myT[i].status == STATUS_ONLINE)
						{
							ret = libusb_set_configuration(myT[i].handle,
										dConfig->bConfigurationValue);
							if(ret != 0)
							{
								sprintf(tmp, "Configuration unavailable, error %d",
									   ret);
								writeToLog(tmp, false);	
								myT[i].status = STATUS_ERROR;
							}
							for(int j=0; j<dConfig->bNumInterfaces; j++)
								if(libusb_claim_interface(myT[i].handle, j) != 0)
								{
									writeToLog("Device not claimed", false);
									myT[i].status = STATUS_ERROR;
								}
								else
								{
									sprintf(tmp, "Interface %d of turret %d ready", j, i);
									writeToLog(tmp, false);
								}
						}
					}
				}
			}
			libusb_free_config_descriptor(dConfig);
		}
	}


	return 0;
}

void initHotplug(turret* myT)
{
	struct timeval tv = {0, 0};
	int ret;
	ret = libusb_handle_events_timeout_completed(NULL, &tv, NULL);
	if(ret != LIBUSB_SUCCESS)
		writeToLog("Handle events failed", false);
	if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
	{
		ret = libusb_hotplug_register_callback(NULL,
				LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
				LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0,
				LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY,
				LIBUSB_HOTPLUG_MATCH_ANY, eventHotplug, myT, NULL);
		if(ret != LIBUSB_SUCCESS)
			writeToLog("New device callback register failed", false);
		else
			writeToLog("New device callback register succeed", false);
	}
	else
		writeToLog("This version of libusb on this OS doesn't support hotplug",
				false);
}

// Functions
bool init(turret* myT)
{
	char			tmp[MAX_CHAR];
	int				ret;

	ret = libusb_init(NULL);
	if(ret != 0)
	{
		sprintf(tmp, "Error while initializing libusb, return : %d\n", ret);
		writeToLog(tmp, false);
		return false;
	}

	struct			libusb_config_descriptor* dConfig = NULL;
	libusb_device**	list = NULL;
	ssize_t			cnt = libusb_get_device_list(NULL, &list);
	int				turretTarget = 0;

	struct libusb_device_descriptor dDevice;


	writeToLog("Starting lsusb things", false);

	if(cnt < 0)
	{
		writeToLog("Unable to get USB device list", false);
		return false;
	}

	sprintf(tmp, "%d devices detected", cnt);
	writeToLog(tmp, false);
	writeToLog("List of compatible devices detected", false);

	for (int i = 0; i < cnt; i++)
	{
		libusb_device *device = list[i];
		ret = libusb_get_device_descriptor(device, &dDevice);
		if((VENDOR_ID_1 == dDevice.idVendor && PRODUCT_ID_1 == dDevice.idProduct) ||
				(VENDOR_ID_2 == dDevice.idVendor && PRODUCT_ID_2 == dDevice.idProduct) ||
				(VENDOR_ID_3 == dDevice.idVendor && PRODUCT_ID_3 == dDevice.idProduct))
		{
			if(turretTarget>MAX_TURRET-1)
			{
				writeToLog("Cannot handle more turrets", false);
			}
			else
			{
				sprintf(tmp, "Bus %03d Device %03d: ID %04x:%04x",
						libusb_get_bus_number(device),
						libusb_get_device_address(device), dDevice.idVendor,
						dDevice.idProduct);
				writeToLog(tmp, false);
				ret = libusb_open(device, &(myT[turretTarget].handle));
				if(ret != 0)
				{
					sprintf(tmp, "Unable to open this device, error %d", ret);
					writeToLog(tmp, false);
				}
				else
				{
					myT[turretTarget].status = STATUS_ONLINE;
					if(VENDOR_ID_1 == dDevice.idVendor)
						myT[turretTarget].type = TYPE_CHESEN;
					else if(VENDOR_ID_2 == dDevice.idVendor)
						myT[turretTarget].type = TYPE_WINBOND;
					else
						myT[turretTarget].type = TYPE_TENX;
					turretTarget++;
				}
			}
		}
	}

	libusb_free_device_list(list, 1);

	for(int i=0; i<MAX_TURRET; i++)
	{
		if(myT[i].status == STATUS_ONLINE)
		{
			ret = libusb_get_config_descriptor(libusb_get_device(
						myT[i].handle), 0, &dConfig);
			if(ret!=0)
			{
				writeToLog("Descriptor for this device unavailable", false);
				myT[i].status =	STATUS_ERROR;
			}
			else
			{
				for(int j=0; j<dConfig->bNumInterfaces; j++)
				{
					if(libusb_kernel_driver_active(myT[i].handle, j) &&
						(libusb_detach_kernel_driver(myT[i].handle, j) != 0))
					{
						writeToLog("Unable to detach this device", false);
						myT[i].status = STATUS_ERROR;
					}
				}
				if(myT[i].status == STATUS_ONLINE)
				{
					ret = libusb_set_configuration(myT[i].handle,
								dConfig->bConfigurationValue);
					if(ret != 0)
					{
						sprintf(tmp, "Configuration unavailable, error %d",
							   ret);
						writeToLog(tmp, false);	
						myT[i].status = STATUS_ERROR;
					}
					for(int j=0; j<dConfig->bNumInterfaces; j++)
						if(libusb_claim_interface(myT[i].handle, j) != 0)
						{
							writeToLog("Device not claimed", false);
							myT[i].status = STATUS_ERROR;
						}
						else
						{
							sprintf(tmp, "Interface %d of turret %d ready", j, i);
							writeToLog(tmp, false);
						}
				}
			}
		}
	}

	libusb_free_config_descriptor(dConfig);

	initHotplug(myT);

	return true;
}

void recieveInterruptData(libusb_device_handle* handle)
{
	int				bytesIn = 0;
	unsigned char	data[1] = {0xFF};
	int				ret;
	char			tmp[MAX_CHAR];

	ret = libusb_interrupt_transfer(handle, 0x81, data, 0xFF, &bytesIn, TIMEOUT);
	if(ret!=0)
	{
		sprintf(tmp, "Interruption failed with error code %d", ret);
		writeToLog(tmp, false);
	}
	else
	{
		sprintf(tmp, "Interruption succeed ! Data recieved %d", data[0]);
		writeToLog(tmp, false);
	}
}

void sendControlData(libusb_device_handle* handle, int type, int len, int in0,
		int in1, int in2, int in3, int in4, int in5, int in6, int in7)
{
	unsigned char	data[8];

	if(len > 0)
		data[0] = in0;
	if(len > 1)
		data[1] = in1;
	if(len > 2)
		data[2] = in2;
	if(len > 3)
		data[3] = in3;
	if(len > 4)
		data[4] = in4;
	if(len > 5)
		data[5] = in5;
	if(len > 6)
		data[6] = in6;
	if(len > 7)
		data[7] = in7;

	libusb_control_transfer(handle, REQUEST_TYPE, REQUEST, requestValue[type-1],
			requestIndex[type-1], data, len, TIMEOUT);
}

bool execute(turret* myT, int command, int turret)
{
	if(turret < 0 || turret > MAX_TURRET-1)
	{
		writeToLog("Such turret does not exist", false);
		return false;
	}

	if(myT[turret].status != STATUS_ONLINE)
	{
		writeToLog("This turret is not online", false);
		return false;
	}

	myT[turret].cmd = command;

	if(myT[turret].type == TYPE_CHESEN)
	{
		sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x40, 0, 0, 0, 0, 0, 0,
						0);
		recieveInterruptData(myT[turret].handle);
		switch(command)
		{
			case T_TOP:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x02, 0, 0, 0,
						0, 0, 0, 0);
				break;
			case T_BOTTOM:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x01, 0, 0, 0,
						0, 0, 0, 0);
				break;
			case T_LEFT:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x04, 0, 0, 0,
						0, 0, 0, 0);
				break;
			case T_RIGHT:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x08, 0, 0, 0,
						0, 0, 0, 0);
				break;
			case T_FIRE:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x10, 0, 0, 0,
						0, 0, 0, 0);
				break;
			default:
				sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x00, 0, 0, 0,
						0, 0, 0, 0);
		}
		sendControlData(myT[turret].handle, TYPE_CHESEN, 1, 0x40, 0, 0, 0, 0,
				0, 0, 0);
		recieveInterruptData(myT[turret].handle);
	}
	else if(myT[turret].type == TYPE_WINBOND)
	{
		sendControlData(myT[turret].handle, TYPE_WINBOND, 5, 0x5e, 0x00, 0x00,
				0x00, 0x00, 0, 0, 0);
		sendControlData(myT[turret].handle, TYPE_WINBOND, 5, 0x5e, 0x00, 0x00,
				0x00, 0x00, 0, 0, 0);
		sendControlData(myT[turret].handle, TYPE_WINBOND, 5, 0x5c, 0x00, 0x00,
				0x00, 0x00, 0, 0, 0);
		sendControlData(myT[turret].handle, TYPE_WINBOND, 5, 0x5c, 0x00, 0x00,
				0x00, 0x00, 0, 0, 0);
		sendControlData(myT[turret].handle, TYPE_WINBOND, 5, 0x5f,
				( command == T_TOP ? 0x02 :
				  ( command == T_BOTTOM ? 0x01 :
					( command == T_RIGHT ? 0x04 :
					  ( command == T_LEFT ? 0x08 :
						( command == T_FIRE ? 0x10 : 0x00
						)
					  )
					)
				  )
				), 0xe0, 0xff, 0xfe, 0, 0, 0);
	}
	else
	{
		for(int i=0; i<2; i++)
		{
		sendControlData(myT[turret].handle, TYPE_TENX, 8, 'U', 'S', 'B',
				'C', 0x00, 0x00, 0x04, 0x00);
		sendControlData(myT[turret].handle, TYPE_TENX, 8, 'U', 'S', 'B',
				'C', 0x00, 0x40, 0x02, 0x00);
		sendControlData(myT[turret].handle, TYPE_TENX, 8,  0x00,
				command == T_LEFT ? 0x01 : 0x00,
				command == T_RIGHT ? 0x01 : 0x00,
				command == T_TOP ? 0x01 : 0x00,
				command == T_BOTTOM ? 0x01 : 0x00,
				command == T_FIRE ? 0x01 : 0x00,
				0x08,
				0x08);
		}
	}

	return true;
}

void closeTurrets(turret* myT)
{
	printf("Cleaning ...\n");
	struct	libusb_config_descriptor* dConfig = NULL;
	int		ret;

	for(int i=0; i<MAX_TURRET; i++)
		if(myT[i].status == STATUS_ONLINE)
		{
			ret = libusb_get_config_descriptor(libusb_get_device(
						myT[i].handle), 0, &dConfig);
			if(ret!=0)
				printf("Unable to get configuration for the device %d\n", i);
			else
				if(myT[i].status == STATUS_ONLINE)
					for(int j=0; j<dConfig->bNumInterfaces; j++)
						if(libusb_release_interface(myT[i].handle, j) != 0)
							printf("Device not released\n");
						else
							printf("Interface %d of turret %d safely disconnected\n", j, i);
		}

	libusb_free_config_descriptor(dConfig);
}
