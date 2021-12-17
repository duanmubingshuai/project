#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "descriptor.h"
#include "hid.h"


ALIGN(4)
#if 0
const static char* hid_ustring[] = 
{
	"Language",
	"SLI Micro System",
	"USB Receiver",
	"20210800",
	"HID Configuration",
	"HID Interface",
};
#endif
const static char* hid_ustring[] = 
{
	"Language",
	"USB Receiver",
	"USB Receiver",
	"20210800",
	"HID Configuration",
	"HID Interface",
};


ALIGN(4)
struct udevice_descriptor dev_desc =
{
    USB_DESC_LENGTH_DEVICE,     //bLength;
    USB_DESC_TYPE_DEVICE,       //type;
    USB_BCD_VERSION,            //bcdUSB;
    USB_CLASS_DEVICE,           //bDeviceClass;
    0x00,                       //bDeviceSubClass;
    0x00,                       //bDeviceProtocol;
    HID_MAX_PACKET_SIZE,        //bMaxPacketSize0;
    _VENDOR_ID,                 //idVendor;
    _PRODUCT_ID,                //idProduct;
    USB_BCD_DEVICE,             //bcdDevice;
    USB_STRING_MANU_INDEX,      //iManufacturer;
    USB_STRING_PRODUCT_INDEX,   //iProduct;
    USB_STRING_SERIAL_INDEX,    //iSerialNumber;
    1,                          //bNumConfigurations;
};

ALIGN(4)
struct usb_qualifier_descriptor dev_qualifier_desc = {
	USB_DESC_LENGTH_QUALIFIER,
	USB_DESC_TYPE_DEVICEQUALIFIER,
	USB_BCD_VERSION,
	USB_CLASS_DEVICE,
	0x0,
	0x0,
	HID_MAX_PACKET_SIZE,
	0x1,
	0x0,
};

/* communcation interface descriptor */
ALIGN(4)
static struct uhid_config_descriptor config_desc =
{
	/*Configuration Descriptor */
	{
		USB_DESC_LENGTH_CONFIG,
		USB_DESC_TYPE_CONFIGURATION,
		USB_DESC_LENGTH_CONFIG + USB_DESC_LENGTH_INTERFACE + USB_DESC_LENGTH_HID \
		+ USB_DESC_LENGTH_ENDPOINT + USB_DESC_LENGTH_INTERFACE + USB_DESC_LENGTH_HID \
		+ USB_DESC_LENGTH_ENDPOINT, 
		0x02,
		0x01,
		0x04,
		0xA0,
		0x31
	},
    /* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        0x00,
        0x00,   
        0x01,
        USB_CLASS_HID,
        USB_HID_SUBCLASS_BOOT,
        USB_HID_PROTOCOL_KEYBOARD,
        0x00,
    },
    /* HID Descriptor */   
    {
        USB_DESC_LENGTH_HID,                              
        USB_DESC_TYPE_HID,
        0x0111,
		0x00,
		0x01,
		USB_DESC_TYPE_REPORT,
        USB_DESC_LENGTH_KEYBOARD_REPORT,
    },
    /* Endpoint Descriptor */    
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        1 | USB_DIR_IN,
        USB_EP_ATTR_INT,
        0x40,
        0x8,
    },
	/* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        0x01,
        0x00,   
        0x01,
        USB_CLASS_HID,
        USB_HID_SUBCLASS_BOOT,
        USB_HID_PROTOCOL_MOUSE,
        0x00,
    },
    /* HID Descriptor */   
    {
        USB_DESC_LENGTH_HID,                              
        USB_DESC_TYPE_HID,
        0x0111,
		0x0,
		0x01,
		USB_DESC_TYPE_REPORT,
        USB_DESC_LENGTH_MOUSE_REPORT,
    },
    /* Endpoint Descriptor */    
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        2 | USB_DIR_IN,
        USB_EP_ATTR_INT,
        0x40,
        0x2,
    }
};

/*Report Descriptor 
 * 
 * 
 * BIT[7:4] : bTag
 *  Main Item Tag:
 *     1000 : Input
 *     1001 : Output
 *     1010 : Collection
 *     1011 : Feature
 *     1100 : End Collection
 * 
 *  Global Item Tag:
 *     0000 : Usage Page
 *     0001 : Logical Minimum
 *     0010 : Logical Maximum
 *     0011 : Physical Minimum
 *     0100 : Physical Maximum
 *     0101 : Unit Exponent
 *     0110 : Unit
 *     0111 : Report Size
 *     1000 : Report ID
 *     1001 : Report Count
 *     1010 : Push
 *     1011 : POP
 *  
 *  Local Item Tag:  
 *     0000 : Usage
 *     0001 : Usage Minimum
 *     0010 : Usage Maximum   
 *     0011 : Designator Index
 *     0100 : Designator Minimum
 *     0101 : Designator Maximum
 *     0111 : String Index
 *     1000 : String Minimum
 *     1001 : String Maimum
 *     1010 : Delimiter
 *  
 * BIT[3:2] : bType
 *         0: Main
 *         1: Global
 *         2: Local
 *         3: Reserved
 * 
 * BIT[1:0] : bSize
 *         0: 0 Bytes
 *         1: 1 Bytes
 *         2: 2 Bytes
 *         3: 3 Bytes
 *
 * */
ALIGN(4)
static uint8_t hid_mouse_report_desc[] ={
	0x05, 0x01,         /* Usage Page (Generic Desktop Controls)                                   */
	0x09, 0x02,         /* Usage (Mouse)                                                           */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x85, 0x02,         /*      Report ID (0x2)                                                    */
	0x09, 0x01,         /*      Usage (Pointer)                                                    */
	0xA1, 0x00,         /*      Collection (Physical)                                              */
	0x05, 0x09,         /*          Usage Page (Button)                                            */
	0x19, 0x01,         /*          Usage Minimum (0x01)                                           */
	0x29, 0x10,         /*          Usage Maximum (0x10)                                           */
	0x15, 0x00,         /*          Logical Minimum (0x00)                                         */
	0x25, 0x01,         /*          Logical Maxmum  (0x01)                                         */
	0x95, 0x10,         /*          Report Count (0x10)                                            */
	0x75, 0x01,         /*          Report Size (0x01)                                             */
	0x81, 0x02,         /*          Input (Data, Absolute, No Wrap, Linear, 
										   Preferred State, No Pull Position Bit Field)            */
    0x05, 0x01,         /*          Usage Page (Generic Desktop Controls)                          */
	0x16, 0x01, 0xf8,   /*          Logical Minimum(0xF801)                                        */
	0x26, 0xFF, 0x07,   /*          Logical Maximum(0x07FF)                                        */
	0x75, 0x0C,         /*          Report Size (0x0C)                                             */
	0x95, 0x02,         /*          Report Count (0x02)                                            */
	0x09, 0x30,         /*          Usage (X)                                                      */
	0x09, 0x31,         /*          Usage (Y)                                                      */
	0x81, 0x06,         /*          Input (Data, Variable, Relative, No Wrap, Linear, 
										   Preferred State, No Pull Position Bit Field)            */
	0x15, 0x81,         /*          Logical Mimimum (0x81)                                         */
	0x25, 0x7F,         /*          Logical Maximum (0x7F)                                         */
	0x75, 0x08,         /*          Report Size (0x08)                                             */
	0x95, 0x01,         /*          Report Count (0x01)                                            */
	0x09, 0x38,         /*          Usage (Wheel)                                                  */
	0x81, 0x06,         /*          Input (Data, Variable, Relative, No Wrap, Linear, 
										   Preferred State, No Pull Position Bit Field)            */
	0x05, 0x0C,         /*          Usage(Consumer)                                                */
	0x0A, 0x38, 0x02,    /*          Usage (AC PAN)                                                 */
	0x95, 0x01,         /*          Report Count(0x01)                                             */
	0x81, 0x06,         /*          Input (Data, Variable, Relative, No Wrap, Linear, 
						                   Preferred State, No Pull Position Bit Field)            */
	0xC0,               /*      End Collection                                                     */
	0xC0,               /* End Collection                                                          */
	0x05, 0x0C,         /* Usage Page (Consumer)                                                   */
	0x09, 0x01,         /* Usage (Consumer Control)                                                */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x85, 0x03,         /*     Report ID (0x03)                                                    */
	0x75, 0x10,         /*     Report Size (0x10)                                                  */
	0x95, 0x02,         /*     Report Count (0x02)                                                 */
	0x15, 0x01,         /*     Logical Minimum(0x01)                                               */
	0x26, 0x8C, 0x02,   /*     Logical Maximum(0x028C)                                             */
	0x19, 0x01,         /*     Usage Minimum (0x01)                                                */
	0x2A, 0x8C, 0x02,   /*     Usage Maximum (0x028C)                                              */
	0x81, 0x00,         /*     Input (Data, Array, Absolute, No Wrap, Linear, 
	                                  Preferred State, No Pull Position Bit Field)                 */
	0xC0,               /* End Collection                                                          */
	0x05, 0x01,         /* Usage Page (Generic Desktop Controls)                                   */
	0x09, 0x80,         /* Usage (System Control)                                                  */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x85, 0x04,         /*      Report ID (0x04)                                                   */
	0x75, 0x02,         /*      Report Size (0x02)                                                 */
	0x95, 0x01,         /*      Report Count (0x01)                                                */
	0x15, 0x01,         /*      Logical Minimum(0x01)                                              */
	0x25, 0x03,         /*      Logical Maximum(0x03)                                              */
	0x09, 0x82,         /*      Usage (System Sleep)                                               */
	0x09, 0x81,         /*      Usage (System Power Down)                                          */
	0x09, 0x83,         /*      Usage (System WakeUp)                                              */
	0x81, 0x60,         /*      Input (Data, Array, Absolute, No Wrap, Linear, 
									No Preferred, No Pull Position Bit Field)                      */
	0x75, 0x06,         /*      Report Size (0x06)                                                 */
    0x81, 0x03,         /*      Input (Constant, Variable, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Bit Field)                */
    0xC0,               /* End Collection                                                          */
	
	0x06, 0x00, 0xFF,   /* Usage Page (Undefined)                                                  */
	0x09, 0x01,         /* Usage(1)                                                                */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x85, 0x10,         /*      Report ID (0x10)                                                   */
	0x75, 0x08,         /*      Report Size (0x08)                                                 */
	0x95, 0x06,         /*      Report Count (0x06)                                                */
	0x15, 0x00,         /*      Logical Minimum(0x00)                                              */
	0x26, 0xff, 0x00,   /*      Logical Maximum(0xFF)                                              */
	0x09, 0x01,         /*      Usage(1)                                                           */
	0x81, 0x00,         /*      Input (Data, Aarry, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Bit Field)                */
	0x09, 0x01,         /*      Usage(1)                                                           */
	0x91, 0x00,         /*      Input (Data, Aarry, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Non Volatile Bit Field)   */
    0xC0,               /* End Collection                                                          */
	
    0x06, 0x00, 0xFF,   /* Usage Page (Undefined)                                                  */
	0x09, 0x02,         /* Usage(2)                                                                */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x85, 0x11,         /*      Report ID (0x11)                                                   */
	0x75, 0x08,         /*      Report Size (0x08)                                                 */
	0x95, 0x13,         /*      Report Count (0x13)                                                */
	0x15, 0x00,         /*      Logical Minimum(0x00)                                              */
	0x26, 0xff, 0x00,   /*      Logical Maximum(0xFF)                                              */
	0x09, 0x02,         /*      Usage(2)                                                           */
	0x81, 0x00,         /*      Input (Data, Aarry, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Bit Field)                */
	0x09, 0x02,         /*      Usage(2)                                                           */
	0x91, 0x00,         /*      Input (Data, Aarry, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Non Volatile Bit Field)   */
    0xC0                /* End Collection                                                          */
};

ALIGN(4)
static uint8_t hid_keyboard_report_desc[] ={
	0x05, 0x01,         /* Usage Page (Generic Desktop Controls)                                   */
	0x09, 0x06,         /* Usage (Keyboard)                                                        */
	0xA1, 0x01,         /* Collection (Application)                                                */
	0x05, 0x07,         /*      Usage Page (Key)                                                   */
	0x19, 0xE0,         /*      Usage Minimum (0xE0)                                               */
	0x29, 0xE7,         /*      Usage Maximum (0xE7)                                               */
	0x15, 0x00,         /*      Logical Minimum (0x00)                                             */
	0x25, 0x01,         /*      Logical Maxmum  (0x01)                                             */
	0x95, 0x08,         /*      Report Count (0x8)                                                 */
	0x75, 0x01,         /*      Report Size (0x01)                                                 */
	0x81, 0x02,         /*      Input (Data, Variable, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Bit Field)                */
    0x81, 0x03,         /*      Input (Constant, Variable, Absolute, No Wrap, Linear, 
									Preferred State, No Pull Position Bit Field)                   */
	0x95, 0x05,         /*      Report Count (0x05)                                                */
	0x05, 0x08,         /*      Usage Page (LEDs)                                                  */
	0x19, 0x01,         /*      Usage Minimum (0x01)                                               */
	0x29, 0x05,         /*      Usage Maximum (0x05)                                               */
	0x91, 0x02,         /*      Output (Data, Variable, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Non VolatileBit Field)    */
	0x95, 0x01,         /*      Report Count(0x01)                                                 */
	0x75, 0x03,         /*      Report Size(0x03)                                                  */
	0x91, 0x01,         /*      Output (Constant, Array Absolute, No Wrap, Liner, 
	                                   Preferred State, No Pull Position Non VolatileBit Field)    */
	0x95, 0x06,         /*      Report Count (0x6)                                                 */
	0x75, 0x08,         /*      Report Size (0x08)                                                 */
	0x15, 0x00,         /*      Logical Minimum(0x00)                                              */
	0x26, 0xA4, 0x00,   /*      Logical Maximum(0x00A4)                                            */
	0x05, 0x07,         /*      Usage Page (Key)                                                   */
	0x19, 0x00,         /*      Usage Minimum (0x00)                                               */
	0x2A, 0xA4, 0x00,   /*      Usage Maximum (0x00A4)                                             */
	0x81, 0x00,         /*      Input (Data, Array, Absolute, No Wrap, Linear, 
						               Preferred State, No Pull Position Bit Field)                */
	0xC0,               /* End Collection                                                          */
};


int32_t get_dev_desc(uint8_t desc_idx, uint8_t str_idx, uint8_t  **buf , uint16_t *len)
{
	//uint32_t i = 0;
	//uint8_t *ptr=(uint8_t *)(&dev_desc);
	switch(desc_idx)
	{
		case USB_DESC_TYPE_DEVICE:
			*buf = (uint8_t *)&dev_desc;
			*len = sizeof(struct udevice_descriptor);
			break;
		case USB_DESC_TYPE_CONFIGURATION:
			*buf = (uint8_t *)&config_desc;
			*len = sizeof(struct uhid_config_descriptor);
			break;
		case USB_DESC_TYPE_STRING:
			*buf = (uint8_t *)hid_ustring[str_idx];
			*len = strlen(hid_ustring[str_idx]);
			break;
		case USB_DESC_TYPE_INTERFACE:
			break;
		case USB_DESC_TYPE_ENDPOINT:
			break;
		case USB_DESC_TYPE_DEVICEQUALIFIER:
		    //*buf = (uint8_t *)&dev_qualifier_desc;
			//*len = sizeof(struct usb_qualifier_descriptor);
			break;
		case USB_DESC_TYPE_OTHERSPEED:
			break;
		case USB_DESC_TYPE_IAD:
			break;
		case USB_DESC_TYPE_REPORT:
		    if(str_idx == 0)
			{
		        *buf = (uint8_t *)hid_keyboard_report_desc;
			    *len = sizeof(hid_keyboard_report_desc);
			}else{
				*buf = (uint8_t *)hid_mouse_report_desc;
			    *len = sizeof(hid_mouse_report_desc);
			}
		    break;
		default:
			return -1;
	}
	return 0;
}


