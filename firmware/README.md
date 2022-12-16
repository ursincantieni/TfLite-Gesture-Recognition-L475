## Debug Project
In order to debug this project conveniently in CLion, a GDB-Server with OpenOCD can be used. (`brew install openocd`.)
The debug configuration in CLion using the GUI can be set up with the help of the debug config wizard. Go to Run -> New Embedded Configuration. Specify the correct path for the `st-util`, which is part of the OpenOCD package, possibly something like `/opt/homebrew/Cellar/stlink/1.7.0/bin/st-util`.
    Also, for `target remote args` add `tcp::26962` and for `GDB server args` add `-p 26962 --freq=1800K`.

## Run Project
For a simple build & run, only OpenOCD is used, no need for the GDB debugging server. For that, add a `OpenOCD Download and Run`configuration in CLion. It is necessary to specify the `.cfg` config file for the MCU.
