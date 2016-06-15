# Introduction

This repository contains the Hardware schematic, Firmware and Software code for the ButtonGizmo device. The ButtonGizmo device is a simple, USB 1.1 Human Interface Device (HID) which functions as a custom input device. It has 8 buttons, and sends USB reports to the host (PC) whenever a button is pressed or released. The USB stack used is V-USB by Objective Development GmBH (http://www.obdev.at/vusb/)

# Hardware
The hardware files are contained in the "Hardware" folder above. The device is built around an ATMega328P microcontroller. It needs a 12MHz crystal and few other external components, mostly resistors. Due to certain considerations, it runs at 3.3V (via an MCP1700 regulator) and is bus-powered. For more details about the hardware considerations, see here: http://vusb.wikidot.com/hardware

# Firmware
The software was written in and compiled with Atmel Studio 7. Asides from the V-USB distribution files, the files of interest here are:<br />

main.c - contains the main software code (the main() superloop and the primary application logic)

ADC.c - routines for initializing and reading from the microcontroller's ADC peripheral

ADC.h - header file containing function prototypes for ADC.c

usbconfig.h - This is part of the V-USB distribution, but it contains the configuration options for the device. Things like the VID/PID, Vendor and Product Name (needed for host-side discrimination), USB Device Class and Subclass, hardware configuration. In short, this is the file you want to pay thorough attention to.

# Software
Since this enumerates as a HID device, there are a number of options available for accessing it. The host side software is written in C#.NET, but there are USB libraries for basically every programming language out there. That said, the demo software here uses HIDSharp (because I wanted the demo to run across platforms). 
