/*
 * (C) Copyright 2003
 * Gerry Hamel, geh@ti.com, Texas Instruments
 *
 * (C) Copyright 2006
 * Bryan O'Donoghue, bodonoghue@codehermit.ie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <config.h>

#include <stdio_dev.h>
#include "usbtty.h"

#include <usb/lpc18xx_usb/cdcuser.h>
#include <usb/lpc18xx_usb/usbhw.h>
#include <usb/lpc18xx_usb/usb.h>
#include <usb/lpc18xx_usb/usbuser.h>
#include <usb/lpc18xx_usb/usbcfg.h>
#include <usb/lpc18xx_usb/usbcore.h>

#include "asm/arch/lpc43xx_scu.h"



#ifdef DEBUG
#define TTYDBG(fmt,args...)\
	serial_printf("[%s] %s %d: "fmt, __FILE__,__FUNCTION__,__LINE__,##args)
#else
#define TTYDBG(fmt,args...) do{}while(0)
#endif

#if 1
#define TTYERR(fmt,args...)\
	serial_printf("ERROR![%s] %s %d: "fmt, __FILE__,__FUNCTION__,\
	__LINE__,##args)
#else
#define TTYERR(fmt,args...) do{}while(0)
#endif

extern DQH_T ep_QH[EP_NUM_MAX];
extern DTD_T ep_TD[EP_NUM_MAX];


LPC_USBDRV_INIT_T usb_cb;
extern uint8_t USB_Configuration;


static struct stdio_dev usbttydev;


/*
 * Test whether a character is in the RX buffer
 */

int usbtty_tstc (void)
{
	/* struct usb_endpoint_instance *endpoint =
		&endpoint_instance[rx_endpoint]; */

    int  numAvailByte;

    CDC_OutBufAvailChar (&numAvailByte);
    if (numAvailByte > 0) {
    	return 1;
    }
    else
    	return 0;

}

/*
 * Read a single byte from the usb client port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */

int usbtty_getc (void)
{
	static char getcBuf [2];
    int  numBytesToRead, numBytesRead, numAvailByte;

    CDC_OutBufAvailChar (&numAvailByte);
    while(numAvailByte <= 0)
    	CDC_OutBufAvailChar (&numAvailByte);

    if (numAvailByte > 0) {
    	numAvailByte = 1; /* getc _> 1 byte to read */
    	numBytesToRead = numAvailByte > 32 ? 32 : numAvailByte;
    	numBytesRead = CDC_RdOutBuf (&getcBuf[0], &numBytesToRead);
    	if(numBytesRead==1){
    		return getcBuf[0];
    	}
    }
    return 0;

	/* char c;
	struct usb_endpoint_instance *endpoint =
		&endpoint_instance[rx_endpoint];

	while (usbtty_input.size <= 0) {
		udc_unset_nak(endpoint->endpoint_address&0x03);
		usbtty_poll ();
	}

	buf_pop (&usbtty_input, &c, 1);
	udc_set_nak(endpoint->endpoint_address&0x03);

	return c; */
}

/*
 * Output a single byte to the usb client port.
 */
void usbtty_putc (const char c)
{
    if (CDC_DepInEmpty) {
      CDC_DepInEmpty = 0;
	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)&c, 1);
      if (c == '\n')
    	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)"\r", 1);
    }

}

/* usbtty_puts() helper function for finding the next '\n' in a string */
static int next_nl_pos (const char *s)
{
	int i;

	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == '\n')
			return i;
	}
	return i;
}

/*
 * Output a string to the usb client port - implementing flow control
 */

static void __usbtty_puts (const char *str, int len)
{
	if (CDC_DepInEmpty) {
      CDC_DepInEmpty = 0;
	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)str, len);
    }
}

void usbtty_puts (const char *str)
{
	int n;
	int len;

	len = strlen (str);
	/* add '\r' for each '\n' */
	while (len > 0) {
		n = next_nl_pos (str);

		if (str[n] == '\n') {
			__usbtty_puts (str, n + 1);
			__usbtty_puts ("\r", 1);
			str += (n + 1);
			len -= (n + 1);
		} else {
			/* No \n found.	 All done. */
			__usbtty_puts (str, n);
			break;
		}
	}


}


/*----------------------------------------------------------------------------
 Initialises the VCOM port.
 Call this function before using VCOM_putchar or VCOM_getchar
 *---------------------------------------------------------------------------*/
void VCOM_Init(void) {
#if PORT_NUM
  CDC_Init (1);
#else
  CDC_Init (0);
#endif
}


/*
 * Initialize the usb client port.
 *
 */
int drv_usbtty_init (void)
{

	LPC_USBDRV_INIT_T usb_cb;

/*	CGU_Init();
	IOInit(); */

	/* initilize call back structures */
		memset((void*)&usb_cb, 0, sizeof(LPC_USBDRV_INIT_T));
		usb_cb.USB_Reset_Event = USB_Reset_Event;
		usb_cb.USB_P_EP[0] = USB_EndPoint0;
		usb_cb.USB_P_EP[1] = USB_EndPoint1;
		usb_cb.USB_P_EP[2] = USB_EndPoint2;
		usb_cb.ep0_maxp = USB_MAX_PACKET0;

		VCOM_Init();

		USB_Init(&usb_cb);  // USB Initialization
		USB_Connect(1);     // USB Connect

		while (!USB_Configuration) ;	 // wait until USB is configured

		printf("Console is about to move to USB.\n");
		debug("USB connected and configured.\n");
		/* Device initialization */
		memset (&usbttydev, 0, sizeof (usbttydev));

		strcpy (usbttydev.name, "usbtty");
		usbttydev.ext = 0;	/* No extensions */
		usbttydev.flags = DEV_FLAGS_INPUT | DEV_FLAGS_OUTPUT;
		usbttydev.tstc = usbtty_tstc;	/* 'tstc' function */
		usbttydev.getc = usbtty_getc;	/* 'getc' function */
		usbttydev.putc = usbtty_putc;	/* 'putc' function */
		usbttydev.puts = usbtty_puts;	/* 'puts' function */
		debug("Console is about to be registered in USB.\n");

		if(!stdio_register(&usbttydev))
			return 1;
		else
			return -1;
}
