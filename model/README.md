# Really cool mouse

Python script `monitor.py` to collect data from both an actual USB mouse as well as from the dev board via serial port.

## Usage

In `monitor.py`, set the values `USB` and `SERIAL` to turn on/off the collection of data from the respective peripherals, and `SAVE` to enable data being stored in the file with its path specified in `FILE`.

Example, collecting all data and save it:
```
USB = True
SERIAL = True
SAVE = True
FILE = 'lol.csv'
```
Run using 
```
sudo python3 monitor.py -p /dev/tty.usbmodemXXXX
```
specifying the path to the serial peripheral.

## Additional Informaiton

data structure of USB device by index:

- 1: clicks
    - 1: left click
    - 2: right click
    - 4: mouse wheel click

- 2: left/right velocity
    - left velocity starts at 255 and decreases as mouse moves faster
    - right velocity starts at 1 and increases as mouse moves faster
    - 0 means the mouse is not moving

- 3: left/right direction
    - 255: left
    - 0: right

- 4: up/down velocity
    - Up velocity range starts at 255 and decreases as the mouse moves faster upward
    - Down velocity range starts at 1 and increases as the mouse moves faster downward
    - 0 means the mouse is moving neither up or down

- 5: up/down direction
    - 0: down
    - 255: up

source:
    https://www.orangecoat.com/how-to/read-and-decode-data-from-your-mouse-using-this-pyusb-hack

