// Protector
#ifndef __TURRET_H__
#define	__TURRET_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>

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
#define REQUEST         0x09
#define REQUEST_TYPE    0x21
#define TIMEOUT         500
#define	T_NONE			0x00
#define T_STOP			0x01
#define T_TOP			0x02
#define T_BOTTOM		0x04
#define T_LEFT			0x08
#define T_RIGHT			0x10
#define T_FIRE			0x20
#define	STATUS_ERROR	-1
#define	STATUS_OFFLINE	0
#define	STATUS_ONLINE	1


// Typedef
typedef struct
{
	libusb_device_handle*	handle;
	int						type;
	int						status;
	int						cmd;
}turret;

// Prototypes
bool	init			(turret*);
bool	execute			(turret*,	int,	int);
void	closeTurrets	(turret*);

#endif
