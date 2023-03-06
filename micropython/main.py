from GSM import sendSMS, checkStatus
import time
from machine import UART, Pin
from time import sleep
from machine import UART
from PiicoDev_Unified import sleep_ms
from PiicoDev_MPU6050 import PiicoDev_MPU6050
from machine import Pin

led = Pin("LED", Pin.OUT)
led.value(1)

uart0 = UART(0, baudrate=9600, tx=Pin(16), rx=Pin(17))
motion = PiicoDev_MPU6050()
sleep(0.5)


Yellow_Offsets = (-5285, -2753, 1787, 57, 39, 33)
Green_Offsets = (1153, 23, 2345, 8, 51, 57)
motion._setOffset(Yellow_Offsets)

checkStatus(uart=uart0)
time.sleep(1)

messageSent = False
while True:

    # Accelerometer data
    accel = motion.read_accel_data()  # read the accelerometer [ms^-2]

    if accel is None:
        continue

    aX = accel["x"]
    aY = accel["y"]
    aZ = accel["z"]

    # Gyroscope Data
    gyro = motion.read_gyro_data()   # read the gyro [deg/s]
    gX = gyro["x"]
    gY = gyro["y"]
    gZ = gyro["z"]
    print(f"{str(aX)},{str(aY)},{str(aZ)},{str(gX)},{str(gY)},{str(gZ)}")

    if aX > 3:
        if not messageSent:
            print("message time")
            sendSMS(uart=uart0, number="9848655422")
            messageSent = True

    sleep_ms(100)
    led.toggle()
