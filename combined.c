/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "hardware/irq.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

/* Example code to talk to a MPU6050 MEMS accelerometer and gyroscope

   This is taking to simple approach of simply reading registers. It's perfectly
   possible to link up an interrupt line and set things up to read from the
   inbuilt FIFO to make it more useful.

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefor I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (pin 6)) -> SDA on MPU6050
   board GPIO PICO_DEFAULT_I2C_SCL_PIN (On Pico this is GP5 (pin 7)) -> SCL on
   MPU6050 board 3.3v (pin 36) -> VCC on MPU6050 board GND (pin 38)  -> GND on
   MPU6050 board
*/

// By default these devices  are on bus address 0x68
static int addr = 0x68;

#ifdef i2c_default
static void mpu6050_reset() {
  // Two byte reset. First byte register, second byte data
  // There are a load more options to set up the device in different ways that
  // could be added here
  // uint8_t buf[] = {0x6B, 0x80};
  uint8_t buf[] = {0x6B, 0x00};
  i2c_write_blocking(i2c_default, addr, buf, 2, false);
  uint8_t buf2[] = {0x1C, 0x10};
  i2c_write_blocking(i2c_default, addr, buf2, 2, false);
  uint8_t buf3[] = {0x1B, 0x10};
  i2c_write_blocking(i2c_default, addr, buf3, 2, false);
  sleep_ms(100);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
  // For this particular device, we send the device the register we want to read
  // first, then subsequently read from the device. The register is auto
  // incrementing so we don't need to keep sending the register we want, just
  // the first.

  uint8_t buffer[6];

  // Start reading acceleration registers from register 0x3B for 6 bytes
  uint8_t val = 0x3B;
  i2c_write_blocking(i2c_default, addr, &val, 1,
                     true);  // true to keep master control of bus
  i2c_read_blocking(i2c_default, addr, buffer, 6, false);

  for (int i = 0; i < 3; i++) {
    accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
  }

  // Now gyro data from reg 0x43 for 6 bytes
  // The register is auto incrementing on each read
  val = 0x43;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 6,
                    false);  // False - finished with bus

  for (int i = 0; i < 3; i++) {
    gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    ;
  }

  // Now temperature from reg 0x41 for 2 bytes
  // The register is auto incrementing on each read
  val = 0x41;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 2,
                    false);  // False - finished with bus

  *temp = buffer[0] << 8 | buffer[1];
}

void preprocessAcc(int16_t accleration[3], float acc[3]) {
  float someConst = 3000.0;
  for (int i = 0; i < 3; i++) {
    acc[i] = (float)accleration[i] / someConst;
  }
}

#endif
void sendText() {
  char endingChar[] = " \n";
  endingChar[0] = (char)26;

  sleep_ms(1000);
  printf("SendingAT\n");
  // uart_puts(UART_ID, "AT\n");
  printf("anju");
  uart_puts(UART_ID, "AT+CMGF=1\n");
  sleep_ms(1000);

  printf("chhetri");
  uart_puts(UART_ID, "AT+CMGS=\"+9779848655422\"\r\n");
  sleep_ms(1000);

  printf("yunika");
  uart_puts(UART_ID,
            "Fall detected. \nLocation: \nLatitude: 27.61821. \nLongitude: "
            "85.5545");
  sleep_ms(100);

  printf("Bajracharya");
  uart_puts(UART_ID, endingChar);
  sleep_ms(100);
  printf("Done");
}

int main() {
  bool sent = false;
  stdio_init_all();
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  uart_init(UART_ID, 2400);
  stdio_init_all();

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  // Actually, we want a different speed
  // The call will return the actual baud rate selected, which will be as close
  // as possible to that requested
  int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

  // Set UART flow control CTS/RTS, we don't want these, so turn them off
  uart_set_hw_flow(UART_ID, false, false);

  // Set our data format
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  // Turn off FIFO's - we want to do this character by character
  uart_set_fifo_enabled(UART_ID, false);

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || \
    !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/mpu6050_i2c example requires a board with I2C pins
  puts("Default I2C pins were not defined");
#else
  printf("Hello, MPU6050! Reading raw data from registers...\n");

  // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
  // i2c_init(i2c_default, 400 * 1000);
  i2c_init(i2c_default, 9600);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                             GPIO_FUNC_I2C));

  mpu6050_reset();

  int16_t acceleration[3], gyro[3], temp;
  float acc[3];

  sleep_ms(5000);

  // sendText();

  // gpio_put(LED_PIN, 1);
  while (1) {
    mpu6050_read_raw(acceleration, gyro, &temp);
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1],
           acceleration[2]);
    // printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
    // // Temperature is simple so use the datasheet calculation to get deg C.
    // // Note this is chip temperature.
    // printf("Temp. = %f\n", (temp / 340.0) + 36.53);

    if (acceleration[2] < 0) {
      if (!sent) {
        gpio_put(LED_PIN, 1);
        sendText();
        sent = true;
        gpio_put(LED_PIN, 0);
      }
    } else {
    }

    sleep_ms(100);
  }

#endif
  return 0;
}
