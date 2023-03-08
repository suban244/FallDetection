from machine import UART
import time

endingchar = bytes()
endingchar += chr(26)
txData = b'AT\r\n'


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
    uart.write(b'hello from micropython\nBye')
    time.sleep(0.1)
    uart.write(endingchar)
    time.sleep(1)

    rxData = bytes()
    while uart.any() > 0:
        rxData += uart.read(1)

           
    try:
        print(rxData.decode('utf-8'))
    except:
        print(len(rxData))
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
        print('fuck')