/*
 * PXA27x register declarations and HCD data structures
 *
 * Copyright (C) 2007 Rodolfo Giometti <giometti@linux.it>
 * Copyright (C) 2007 Eurotech S.p.A. <info@eurotech.it>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#ifndef __PXA270X_UDC_H__
#define __PXA270X_UDC_H__

#include <asm/byteorder.h>

/* Endpoint 0 states */
#define EP0_IDLE		0
#define EP0_IN_DATA		1
#define EP0_OUT_DATA		2
#define EP0_XFER_COMPLETE	3


/* Endpoint parameters */
#define MAX_ENDPOINTS		4
#define EP_MAX_PACKET_SIZE	64

#define EP0_MAX_PACKET_SIZE     16
#define UDC_OUT_ENDPOINT        0x02
#define UDC_OUT_PACKET_SIZE     EP_MAX_PACKET_SIZE
#define UDC_IN_ENDPOINT         0x01
#define UDC_IN_PACKET_SIZE      EP_MAX_PACKET_SIZE
#define UDC_INT_ENDPOINT        0x01
#define UDC_INT_PACKET_SIZE     EP_MAX_PACKET_SIZE
#define UDC_BULK_PACKET_SIZE    EP_MAX_PACKET_SIZE

/* bit defines for USBCMD register */
#define USBCMD_RS	    (1<<0)
#define USBCMD_RST	    (1<<1)
#define USBCMD_ATDTW 	(1<<12)
#define USBCMD_SUTW	    (1<<13)

/* bit defines for USBSTS register */
#define USBSTS_UI	    (1<<0)
#define USBSTS_UEI	    (1<<1)
#define USBSTS_PCI	    (1<<2)
#define USBSTS_URI	    (1<<6)
#define USBSTS_SRI	    (1<<7)
#define USBSTS_SLI	    (1<<8)
#define USBSTS_NAKI	    (1<<16)


/* bit defines EPcontrol registers*/
#define EPCTRL_RXS	      (1<<0)
#define EPCTRL_RX_TYPE(n) (((n) & 0x3)<<2)
#define EPCTRL_RX_CTL	  (0<<2)
#define EPCTRL_RX_ISO	  (1<<2)
#define EPCTRL_RX_BLK	  (2<<2)
#define EPCTRL_RXI	      (1<<5)
#define EPCTRL_RXR	      (1<<6)
#define EPCTRL_RXE	      (1<<7)
#define EPCTRL_TXS	      (1<<16)
#define EPCTRL_TX_TYPE(n) (((n) & 0x3)<<18)
#define EPCTRL_TX_CTL	  (0<<18)
#define EPCTRL_TX_ISO	  (1<<18)
#define EPCTRL_TX_BLK	  (2<<18)
#define EPCTRL_TX_INT	  (3<<18)
#define EPCTRL_TXI	      (1<<21)
#define EPCTRL_TXR	      (1<<22)
#define EPCTRL_TXE	      (1<<23)

/* dTD Transfer Description */
typedef volatile struct
{
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes ;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
}  DTD_T;

/* dQH  Queue Head */
typedef volatile struct
{
  volatile uint32_t cap;
  volatile uint32_t curr_dTD;
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
  volatile uint32_t setup[2];
  volatile uint32_t gap[4];
}  DQH_T;



void udc_irq(void);
/* Flow control */
void udc_set_nak(int epid);
void udc_unset_nak(int epid);

/* Higher level functions for abstracting away from specific device */
int udc_endpoint_write(struct usb_endpoint_instance *endpoint);

int  udc_init(void);

void udc_enable(struct usb_device_instance *device);
void udc_disable(void);

void udc_connect(void);
void udc_disconnect(void);

void udc_startup_events(struct usb_device_instance *device);
void udc_setup_ep(struct usb_device_instance *device,
	 unsigned int ep, struct usb_endpoint_instance *endpoint);

#endif
