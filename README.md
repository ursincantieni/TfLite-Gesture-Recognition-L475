# Gesture recognition using LSM6DSL acc. on STM32

## What?

Firmware to recognize a physical shaking as well as tilting of the device. With this infomation, the board can be used as USB-HID mouse to control the cursor in terms of left-clicking and movement. The gesture recognition is implemented using a Tensorflow Lite model.

### Content

 - `firmware/`: firmware for the STM32L4 MCU on the B-L475E-IOT01A discovery kit.
 - `model/`: python scripts to collect and process data by listening to a serial port. Plus a python notebook to build the TfLite model itself.
