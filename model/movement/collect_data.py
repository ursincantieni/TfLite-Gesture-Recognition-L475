import serial
import argparse
import re

import sys
import usb.core
import usb.util
import csv


USB = False
SERIAL = True
SAVE = True
FILE = 'data/try2/dataset.csv'

# Setup USB mouse (reference)
# http://stackoverflow.com/questions/8218683/pyusb-cannot-set-configuration
if USB:
    dev = usb.core.find(idVendor=0x046d, idProduct=0xc332)
    interface = 0
    endpoint = dev[0][(0, 0)][0]
    if dev.is_kernel_driver_active(interface) is True:
        dev.detach_kernel_driver(interface)
        usb.util.claim_interface(dev, interface)

    mouseData = [None] * 6

# Setup serial listener
if SERIAL:
    ap = argparse.ArgumentParser()
    ap.add_argument("-p", "--port", required=True, help="Enter Port Name")
    args = vars(ap.parse_args())
    PORT = args['port']
    #try:
    serial_port = serial.Serial(PORT, 115200)
    print(f"The Port name is {serial_port.name}")
    #except:
    #    print("ERROR")
    #    print("Check serial connection.")
    #    exit()

    serialData = [None] * 3

with open(FILE, "w", newline='') as csvfile:
    # CSV
    fields = ['acc_x', 'acc_y', 'acc_z',
              'lr_vel', 'lr_dir', 'ud_vel', 'ud_dir']
    writer = csv.DictWriter(csvfile, fieldnames=fields)
    writer.writeheader()

    # Loop
    while True:
        if SERIAL:
            lines = serial_port.readline()
            serialData = re.findall(
                "[+-]?\d+(?:\.\d+)?", lines.decode('utf-8'))

            #print("**************")
            #print(serialData)

        if USB:
            try:
                mouseData = dev.read(endpoint.bEndpointAddress,
                                     endpoint.wMaxPacketSize,
                                     timeout=1)
                print(mouseData)
                print("**************")

            except usb.core.USBError as e:
                print(e)
                if e.args != (60, 'Operation timed out'):
                    break

        if SAVE and type(serialData) == list and len(serialData) >= 3:
            if USB:
                print(serialData)
                saveData = {
                    'acc_x': serialData[0],
                    'acc_y': serialData[1],
                    'acc_z': serialData[2],
                    'lr_vel': mouseData[2],
                    'lr_dir': mouseData[3],
                    'ud_vel': mouseData[4],
                    'ud_dir': mouseData[5],
                }
            else:
                saveData = {
                    'acc_x': serialData[0],
                    'acc_y': serialData[1],
                    'acc_z': serialData[2],
                }
            writer.writerow(saveData)


"""
data structure of USB device by index:

1: clicks
    1: left click
    2: right click
    4: mouse wheel click

2: left/right velocity
    - left velocity starts at 255 and decreases as mouse moves faster
    - right velocity starts at 1 and increases as mouse moves faster
    - 0 means the mouse is not moving

3: left/right direction
    255: left
    0: right

4: up/down velocity
    - Up velocity range starts at 255 and decreases as the mouse moves faster upward
    - Down velocity range starts at 1 and increases as the mouse moves faster downward
    - 0 means the mouse is moving neither up or down

5: up/down direction
    0: down
    255: up

source:
    https://www.orangecoat.com/how-to/
    read-and-decode-data-from-your-mouse-using-this-pyusb-hack
"""
