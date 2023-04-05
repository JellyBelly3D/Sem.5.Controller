/*
this is controller HID descriptor

-1 byte per axis X,Y (-128 to 128)

USAGE_PAGE (Generic Desktop)
USAGE (Game Pad)
COLLECTION (Application)
    COLLECTION (Physical)
    USAGE_PAGE (Generic Desktop) //not sure about Generic Desctop
    USAGE (X)                    //use X axis on left thumbstick //use Z for X axis on right thumbstick 
    USAGE (Rx)                   //use Y axis on left thumbstick //use Rx for Y axis on right thumbstick
    LOGICAL_MINIMUM (-128)       //min x,y
    LOGICAL_MAXIMUM (128)        //max x,y

    REPORT_SIZE (8)              //report size of 1 byte
    REPORT_COUNT (2)             //send both 1byte reports

    INPUT (Data,Var,Rel)        //send X,Y axis data to computer as variable coordinate
    END COLLECTION
END COLLECTION

*/

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

struct controllerInputs
{
    int8_t left_x;
	int8_t left_y;
	int8_t right_x;
	int8_t right_y;
};

const uint8_t controllerDescriptor[] =
{
    0x05, 0x01,                    // Usage Page (Generic Desktop)
    0x09, 0x04,                    // Usage (Game Pad)
    0xA1, 0x01,                    // Collection (Application)
    0xA1, 0x00,                    // Collection (Physical)
    0x05, 0x01,                    // Usage Page (Generic Desktop)
    0x09, 0x30,                    // Usage (X)
    0x09, 0x31,                    // Usage (Rx)
    0x15, 0x81,                    // Logical Minimum (-127)
    0x25, 0x7F,                    // Logical Maximum (127)
    0x75, 0x08,                    // Report Size (8)
    0x95, 0x02,                    // Report Count (2)
    0x81, 0x02,                    // Input (Data,Var,Abs)
    0xC0,                          // End Collection
    0xC0                           // End Collection
};
