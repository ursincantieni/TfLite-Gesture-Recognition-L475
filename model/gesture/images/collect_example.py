import serial
import argparse
import re

import sys
import csv

SAVE = True
FILE = 'example.csv'

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
    fields = ['acc_x', 'acc_y', 'acc_z', 'prediction']
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
                    fields[0]:serialData[0],
                    fields[1]:serialData[1],
                    fields[2]:serialData[2],
                    fields[3]:serialData[3]
            }
            writer.writerow(saveData)
