from GSM import sendSMS, checkStatus, getLocation
import time
from machine import UART, Pin
from time import sleep
from machine import UART
from PiicoDev_Unified import sleep_ms
from PiicoDev_MPU6050 import PiicoDev_MPU6050
# from nic import converter

from machine import Pin
led = Pin("LED", Pin.OUT)
led.value(1)

interrupt_flag = 0
debounce_time = 0
pin = Pin(14, Pin.IN, Pin.PULL_UP)
led = Pin("LED", Pin.OUT)
count = 0
messageSent = False


def callback(pin):
    global interrupt_flag, debounce_time, messageSent

    if (time.ticks_ms() - debounce_time) > 50:
        if (time.ticks_ms() - debounce_time) < 500:
            interrupt_flag = 1
            if not messageSent:
                print("message time interrupt")
                sendSMS(uart=uart0, number="9848655422")
                checkStatus(uart=uart0)
                messageSent = True

        debounce_time = time.ticks_ms()


pin.irq(trigger=Pin.IRQ_FALLING, handler=callback)


# uart0 = UART(0, baudrate=9600, tx=Pin(16), rx=Pin(17))
# motion = PiicoDev_MPU6050(bus=0, freq=9600, sda=Pin(8), scl=Pin(9))
motion = PiicoDev_MPU6050(bus=0, freq=19200, sda=Pin(4), scl=Pin(5))
samples_per_second = 10
delay_time = 1000/samples_per_second

sleep(0.5)


Yellow_Offsets = (-5285, -2753, 1787, 57, 39, 33)
Green_Offsets = (1153, 23, 2345, 8, 51, 57)
motion._setOffset(Yellow_Offsets)


# checkStatus(uart=uart0)
# getLocation(uart=uart0)
time.sleep(1)

while True:
    start = time.ticks_ms()

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
#     if aX > 3:

#         if not messageSent:
#             print("message time")
    # sendSMS(uart=uart0, number="9848655422")
#             checkStatus(uart=uart0)
#             messageSent = True

#     converter(accel)

#     if interrupt_flag is 1:
#         interrupt_flag=0
#         print("Interrupt Detected")
#         led.toggle()

    end = time.ticks_ms()
    diff = time.ticks_diff(end, start)
    if diff > delay_time:
        diff = delay_time

    sleep_ms(int(delay_time - diff))
    led.toggle()
