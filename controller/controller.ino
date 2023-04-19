#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

const int hallPin = 0;  // the number of the Hall sensor pin
const int ledPin = LED_BUILTIN;

int count = 0;
int hallState = 0;
bool magnet = false;

/*
Controller HID descriptor

-1 byte per axis X,Y (-127 to 127)

USAGE_PAGE (Generic Desktop)
USAGE (Game Pad)
COLLECTION (Application)
    COLLECTION (Physical)
    USAGE_PAGE (Generic Desktop) // ¯\_(ツ)_/¯
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

uint8_t const controllerHidDescriptor[] = {
  0x05, 0x01,  // Usage Page (Generic Desktop)
  0x09, 0x05,  // Usage (Game Pad)
  0xA1, 0x01,  // Collection (Application)
  0xA1, 0x00,  // Collection (Physical)
  0x05, 0x01,  // Usage Page (Generic Desktop)
  // Left Thumbstick  
  0x09, 0x30,                    // Usage (X)
  0x09, 0x31,                    // Usage (Y)
  0x15, 0x81,                    // Logical Minimum (-127)
  0x25, 0x7F,                    // Logical Maximum (127)
  0x75, 0x08,                    // Report Size (8)
  0x95, 0x02,                    // Report Count (2)
  0x81, 0x02,                    // Input (Data,Var,Abs)
  // Right Thumbstick
  0x09, 0x33,                    // Usage (Rx)
  0x09, 0x34,                    // Usage (Ry)
  0x15, 0x81,                    // Logical Minimum (-127)
  0x25, 0x7F,                    // Logical Maximum (127)
  0x75, 0x08,                    // Report Size (8)
  0x95, 0x02,                    // Report Count (2)
  0x81, 0x02,                    // Input (Data,Var,Abs)
  0xC0,        // End Collection
  0xC0         // End Collection
};

Adafruit_USBD_HID usb_controller(controllerHidDescriptor, sizeof(controllerHidDescriptor), HID_ITF_PROTOCOL_NONE, 4, false);

typedef struct TU_ATTR_PACKED {
  int8_t x;
  int8_t y;
  int8_t rx;
  int8_t ry;
} controllerInputs;

controllerInputs inputs;

void setup() {
  TinyUSB_Device_Init(0);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(hallPin, INPUT);

  SerialTinyUSB.begin(112000);

  usb_controller.begin();

  while (!TinyUSBDevice.mounted()) delay(1);

  SerialTinyUSB.println("Setup Complete");
}

void loop() {
  hallState = digitalRead(hallPin);

  if (hallState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    if (!magnet) {
      count++;
      if (usb_controller.ready()) {
        //SerialTinyUSB .println("Controller ready");
        uint8_t const report_id = 0;

        inputs.x = 0; //unused left x axis
        inputs.rx = 0; //unused left rx axis
        inputs.y = random(-127, 127);
        inputs.ry = random(-127, 127);

        usb_controller.sendReport(report_id, &inputs, sizeof(inputs));
      }
      //SerialTinyUSB .println(count);
    }
    magnet = true;
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    magnet = false;
  }
}
