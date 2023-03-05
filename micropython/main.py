# # Example code for PiicoDev Motion Sensor MPU6050
# from PiicoDev_MPU6050 import PiicoDev_MPU6050
# # Cross-platform compatible sleep function
# from PiicoDev_Unified import sleep_ms

# from machine import UART

# from time import sleep

# motion = PiicoDev_MPU6050()
# sleep(0.5)

# Yellow_Offsets = (-5285, -2753, 1787, 57, 39, 33)
# Green_Offsets = (1153, 23, 2345, 8, 51, 57)
# motion._setOffset(Yellow_Offsets)

# while True:

#     # Accelerometer data
#     accel = motion.read_accel_data()  # read the accelerometer [ms^-2]

#     if accel is None:
#         continue

#     aX = accel["x"]
#     aY = accel["y"]
#     aZ = accel["z"]

#     # Gyroscope Data
#     gyro = motion.read_gyro_data()   # read the gyro [deg/s]
#     gX = gyro["x"]
#     gY = gyro["y"]
#     gZ = gyro["z"]
#     print(f"{str(aX)},{str(aY)},{str(aZ)},{str(gX)},{str(gY)},{str(gZ)}")

#     sleep_ms(100)

# from machine import UART, Pin
# import time

# uart0 = UART(0, baudrate=9600, tx=Pin(16), rx=Pin(17))

# endingchar = bytes()
# endingchar += chr(26)

# while uart0.any():
#     uart0.read(1)

# time.sleep(1)
# uart0.write(b"AT+CMGF=1\r\n")
# time.sleep(1)
# uart0.write(b'AT+CMGS=\"+9779848655422\"\r\n')
# time.sleep(1)
# uart0.write(b'hello from micropython\nBye')
# time.sleep(0.1)
# uart0.write(endingchar)
# time.sleep(4)


# rxData = bytes()
# while uart0.any() > 0:
#     rxData += uart0.read(1)

# print(rxData.decode('utf-8'))
# time.sleep(1)


# txData = b'AT\r\n'
# while True:
#     uart0.write(txData)
#     time.sleep(1)
#     rxData = bytes()

#     while uart0.any() > 0:
#         rxData += uart0.read(1)

#     # print("Received: ", len(rxData.decode('utf-8')), rxData.decode('utf-8'))
#     print(len(rxData.decode('utf-8')), rxData.decode('utf-8'))
#     time.sleep(1)

from machine import UART, Pin
import time
from GSM import sendSMS, checkStatus

uart0 = UART(0, baudrate=9600, tx=Pin(16), rx=Pin(17))
sendSMS(uart=uart0, number="9848655422")


while True:
    checkStatus(uart0)
    time.sleep(1)
