// Include
#include "ui.h"

// Global
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
		if((VENDOR_ID_1 == dDevice.idVendor && PRODUCT_ID_1 == dDevice.idProduct) || (VENDOR_ID_2 == dDevice.idVendor && PRODUCT_ID_2 == dDevice.idProduct) || (VENDOR_ID_3 == dDevice.idVendor && PRODUCT_ID_3 == dDevice.idProduct))
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
					myT->online = true;
					turretTarget++;
					if(VENDOR_ID_1 == dDevice.idVendor)
						myT->type = TYPE_CHESEN;
					else if(VENDOR_ID_2 == dDevice.idVendor)
						myT->type = TYPE_WINBOND;
					else
						myT->type = TYPE_TENX;
				}
			}
		}
	}

	libusb_free_device_list(list, 1);

	//libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);

	for(int i=0; i<MAX_TURRET; i++)
	{
		if(myT[i].online)
		{
			ret = libusb_get_config_descriptor(libusb_get_device(
						myT[i].handle), 0, &dConfig);
			if(ret!=0)
			{
				writeToLog("Descriptor for this device unavailable", false);
				//TODO NOT WORKING DEVICE
				myT[i].online=false;
			}
			else
			{
				if(libusb_kernel_driver_active(myT->handle, 0) &&
					(libusb_detach_kernel_driver(myT->handle, 0) != 0))
				{
					writeToLog("Unable to detach this device", false);
					myT[i].online = false;
				}
				else
				{
					if(libusb_set_configuration(myT[i].handle,
								dConfig->bConfigurationValue) != 0)
					{
						writeToLog("Configuration unavailable", false);
						myT[i].online = false;
					}
					else
						if(libusb_claim_interface(myT[i].handle, 0) != 0)
						{
							writeToLog("Device not claimed", false);
							myT[i].online = false;
						}
						else
							writeToLog("Device ready", false);
				}
			}
		}
	}

	return true;
}

bool execute(turret* myT, int command, int turret)
{
	if(turret < 0 || turret > MAX_TURRET-1)
	{
		writeToLog("Such turret does not exist", false);
		return false;
	}

	if(!myT[turret].online)
	{
		writeToLog("This turret is not online", false);
		return false;
	}

	myT->cmd = command;

	return true;
}
