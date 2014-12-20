// Protector
#ifndef __TURRET_H__
#define	__TURRET_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libusb/libusb/libusb.h"

// Defines
#define	VENDOR_ID_1		0x0a81
#define	PRODUCT_ID_1	0xff01
#define	VENDOR_ID_2		0x0416
#define	PRODUCT_ID_2	0x9391
#define VENDOR_ID_3		0x1130
#define	PRODUCT_ID_3	0x0202
#define	TYPE_NONE		0
#define	TYPE_TENX		1
#define	TYPE_WINBOND	2
#define	TYPE_CHESEN		3
#define	MAX_TURRET		4

// Typedef
typedef struct
{
	libusb_device_handle*	handle;
	int						type;
	bool					online;
}turret;

#endif
