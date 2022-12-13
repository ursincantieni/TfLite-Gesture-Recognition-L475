# Gesture recognition using LSM6DSL acc. on STM32

In this project, a Tensorflow Lite model is used to recognize a shaking of the device and send a left-click signal if connected as USB-HID mouse. Additionally, by tilting the device the cursor can be controlled, which is also implemented using the accelerometer.
Note: "device" refers to the B-L475E-IOT01A discovery kit, for which this project is built.
Note: The build system generation using cmake allows for convenient use in CLion.

## Debug Project with GDB Server
In order to debug this project conveniently in CLion, a GDB-Server with OpenOCD can be used. (`brew install openocd`.)
The debug configuration in CLion using the GUI can be set up with the help of the debug config wizard. Go to Run -> New Embedded Configuration. Specify the correct path for the `st-util`, which is part of the OpenOCD package, possibly something like `/opt/homebrew/Cellar/stlink/1.7.0/bin/st-util`.
    Also, for `target remote args` add `tcp::26962` and for `GDB server args` add `-p 26962 --freq=1800K`.

## Run Project
For a simple build & run, only OpenOCD is used, no need for the GDB debugging server. For that, add a `OpenOCD Download and Run`configuration in CLion. It is necessary to specify the `.cfg` config file for the MCU.
