import serial
import argparse
import re

import sys
import csv

SAVE = True
FILE = 'data/holding.csv'

# Setup serial listener
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
    fields = ['acc_x', 'acc_y', 'acc_z']
    writer = csv.DictWriter(csvfile, fieldnames=fields)
    writer.writeheader()

    # Loop
    while True:
        lines = serial_port.readline()
        serialData = re.findall(
            "[+-]?\d+(?:\.\d+)?", lines.decode('utf-8'))

        #print("**************")
        print(serialData)

        if SAVE and type(serialData) == list and len(serialData) >= 3:
            saveData = {
                'acc_x': serialData[0],
                'acc_y': serialData[1],
                'acc_z': serialData[2],
            }
            writer.writerow(saveData)
