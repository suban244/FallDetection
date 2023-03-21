from machine import UART
import time

endingchar = bytes()
endingchar += chr(26)
txData = b'AT\r\n'


def uartSend(uart: UART, data: bytes, delay: float = 0.1):
    rxData = bytes()
    uart.write(data)
    time.sleep(delay)
    while uart.any() > 0:
        rxData += uart.read(1)
    try:
        print(rxData.decode('utf-8'))
    except:
        print(rxData)
    time.sleep(0.1)


def getLocation(uart: UART):
    print("Getting location")
    while uart.any():
        uart.read()
    time.sleep(0.1)

    uartSend(uart=uart, data=b'AT+CGATT =1\r\n')
    uartSend(uart=uart, data=b'AT+SAPBR =3,1,"CONTYPE","GPRS"\r\n')
    uartSend(uart=uart, data=b'AT+SAPBR =3,1,"APN","RCMNET"\r\n')

    uartSend(uart=uart, data=b'AT+SAPBR=1,1\r\n', delay=1)

    uartSend(uart=uart, data=b'AT+SAPBR=2,1\r\n', delay=1)

    # uartSend(uart=uart, data=b'AT+CIPGSMLOC=1,1\r\n', delay=2)
    uartSend(uart=uart, data=b'AT+CLBS=1,1\r\n', delay=2)

    uartSend(uart=uart, data=b'AT+CIPGSMLOC=2,1\r\n', delay=2)

    uartSend(uart=uart, data=b'AT+SAPBR =0,1\r\n', delay=1)
    time.sleep(1)


def sendSMS(uart: UART, number: str):
    print("sendingSMS")
    while uart.any():
        uart.read(1)
    time.sleep(0.1)

    words = f'AT+CMGS=\"+977{number}\"\r\n'

    uart.write(b"AT+CMGF=1\r\n")
    time.sleep(1)
    uart.write(words.encode('utf-8'))
    time.sleep(1)
    uart.write(
        b'Fall detected. \nLocation: \nLatitude: 27.61821. \nLongitude: 85.5545')
    time.sleep(0.1)
    uart.write(endingchar)
    time.sleep(1)

    rxData = bytes()
    while uart.any() > 0:
        rxData += uart.read(1)

    try:
        print(rxData.decode('utf-8'))
    except:
        print(rxData)
    time.sleep(1)


def checkStatus(uart: UART):
    uart.write(txData)
    time.sleep(1)
    rxData = bytes()

    while uart.any() > 0:
        rxData += uart.read(1)

    try:
        print(len(rxData), rxData.decode('utf-8'))
    except:
        print(rxData)
