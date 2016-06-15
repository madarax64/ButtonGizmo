
#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "usbdrv.h"
#include "ADC.h"

/* Defines */
#define BUTTON_ONE 818
#define BUTTON_TWO 614
#define BUTTON_THREE 409
#define BUTTON_FOUR 205
#define THRESHOLD 50
#define BANK_A_PIN 4
#define BANK_B_PIN 5
#define BUTTON_DOWN 0x34
#define BUTTON_UP 0x43
#define HID_REPORT_SIZE 8

/* Variables */
uint16_t iterations = 0;
uint16_t buttonValues[] = {BUTTON_ONE, BUTTON_TWO, BUTTON_THREE, BUTTON_FOUR};
uint8_t buttonPressedState = 0;
uint8_t lastButtonPressedState = BUTTON_UP;
uint8_t buttonVal = 0;
	
/* Function Prototypes */
void checkBankA(void);
void checkBankB(void);
void sendButtonDown(int);
void sendButtonUp(void);
uint8_t outputBuffer[HID_REPORT_SIZE];

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
	0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
	0x09, 0x01,                    // USAGE (Vendor Usage 1)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x09, 0x01,                    //   USAGE (Vendor Usage 1)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //   REPORT_SIZE (8)
	0x95, HID_REPORT_SIZE,         //   REPORT_COUNT (8)
	0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	0x09, 0x02,                    //   USAGE (Vendor Usage 2)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //   REPORT_SIZE (8)
	0x95, HID_REPORT_SIZE,         //   REPORT_COUNT (8)
	0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
	0xc0                           // END_COLLECTION
};


USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {
		 usbRequest_t* rq = (void*)data;
		 
		 if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    /* HID class request */
			 
			 if(rq->bRequest == USBRQ_HID_SET_REPORT){
				 return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host (we aren't actually using this here)*/
			 }
		 }
		 return 0;
}


int main(void)
{
	/* Initialization code for the ADC and the USB Stack */
	ADC_Init();
	usbInit();
	usbDeviceDisconnect();
	for (int i=0; i < 250;i++) {
		_delay_ms(2);
	}
	usbDeviceConnect();
	sei();
	
    while (1) 
    {
		usbPoll();
		iterations++;
		
		if (iterations > 25000) {
		
		buttonPressedState = BUTTON_UP; //reset the buttonPressed flag, and assume that all buttons are up initially
		checkBankA();
		
		if (buttonPressedState == BUTTON_UP) {
			//this means nothing in Bank A is pressed, so we check bank B
			checkBankB();
		}
		
		//if there has been some change, react accordingly by sending an appropriate report
		if (buttonPressedState != lastButtonPressedState) {
			switch (buttonPressedState) {
				case BUTTON_DOWN:
				sendButtonDown(buttonVal);
				break;
				case BUTTON_UP:
				sendButtonUp();
				break;
			}
			
			lastButtonPressedState = buttonPressedState; //update the state variable
		}
		
		//reset the button poll iterator count
		iterations = 0;
		}
    }
}

USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len) {
	return 1;
}

USB_PUBLIC void usbFunctionWriteOut(uchar *data, uchar len) {
	//do nothing
}

void checkBankA(void) {
	for (int i=0; i < 4;i++) {
		int bankAValue = ADC_Read(BANK_A_PIN);
		int rawValue = bankAValue - buttonValues[i];
		rawValue = (rawValue < 0) ? -rawValue : rawValue;
		if (rawValue <= THRESHOLD) {
			//then this must be button 1 + 1 pressed- 1,2,3,4
			buttonPressedState = BUTTON_DOWN;
			buttonVal = i + 1;
			break;
		}
	}
}

void checkBankB(void) {
	int bankBValue = ADC_Read(BANK_B_PIN);
	
	for (int i=0; i < 4;i++) {
		int rawValue = bankBValue - buttonValues[i];
		rawValue = (rawValue < 0) ? -rawValue : rawValue;
		if (rawValue <= THRESHOLD) {
			//then this must be button i + 5 pressed - 5,6,7,8
			buttonPressedState = BUTTON_DOWN;
			buttonVal = i + 5;
			break;
		}
	}
}

void sendButtonDown(int i) {
	outputBuffer[0] = 0x40; //button down flag
	outputBuffer[1] = i;
	usbSetInterrupt(outputBuffer, HID_REPORT_SIZE);
}

void sendButtonUp() {
	outputBuffer[0] = 0x41; //button up flag
	outputBuffer[1] = buttonVal;
	usbSetInterrupt(outputBuffer, HID_REPORT_SIZE);
}
