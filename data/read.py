import serial
import time
import csv
import sys
# Open the serial port with PySerial
ser = serial.Serial('/dev/ttyACM0', 9600)  # IMU data
# ser1 = serial.Serial('/dev/ttyACM0', 115200)  # GSM and bluetooth

start = time.time()
queue = []
count = 0

fields = ['ax', 'ay', 'az', 'gx', 'gy', 'gz']
filename = sys.argv[1]
success = 0

with open(filename, 'w') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(fields)
    print("STarting")
    while True:
        if success >= 40:
            break
        try:
            line = ser.readline().decode()
            if line:
                # Split the line into a timestamp and data
                timestamp = line.strip().split(',')
                if (len(timestamp) == 6):
                    varlist = [timestamp[0], timestamp[1],
                               timestamp[2], timestamp[3], timestamp[4], timestamp[5]]

                    for i in range(6):
                        varlist[i] = float(varlist[i])
                    csvwriter.writerow(varlist)
                    success += 1
                    print(success, varlist)

        except:
            continue
