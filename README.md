# Gesture recognition using LSM6DSL acc. on STM32

## What?

Projects which aims to make the B-L475E-IOT01A usable as a USB-mouse, based ont the LSM6DSL accelerometer. Cursor movement works by tilting the device, clicking by shaking it, which is recognized by a Tensorflow Lite model.

### Content

 - `firmware/`: firmware for the STM32L4 MCU on the B-L475E-IOT01A discovery kit.
 - `model/`: python scripts to collect and process data by listening to a serial port. Plus a python notebook to build the TfLite model itself.
