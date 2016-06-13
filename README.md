# Introduction

This repository contains the Hardware schematic, Firmware and Software code for the ButtonGizmo device. The ButtonGizmo device is a simple, USB 1.1 Human Interface Device (HID) which functions as a custom input device. It has 8 buttons, and sends USB reports to the host (PC) whenever a button is pressed. 

# Hardware
The hardware files are contained in the "Hardware" folder above. The device is built around an ATMega328P microcontroller running the VUSB stack from obdev.at. It needs a 12MHz crystal and few other external components, mostly resistors. Due to certain considerations, it runs at 3.3V (via an MCP1700 regulator) and is bus-powered. For more details about the hardware considerations, see here:

# Firmware

# Software
TBD




