/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
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

static int chars_rxed = 0;

// RX interrupt handler
void on_uart_rx() {
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 1);
  printf("Received %d \n", uart_is_readable(UART_ID));

  // uint8_t ch = uart_getc(UART_ID);
  // // Can we send it back?
  // printf(" %c\n", ch);

  while (uart_is_readable(UART_ID)) {
    printf("Got:");
    uint8_t ch = uart_getc(UART_ID);
    // Can we send it back?
    printf(" %c\n", ch);

    chars_rxed++;
  }

  sleep_ms(10 * 1000);
  gpio_put(LED_PIN, 0);
}

static int addr = 0x68;

void sendMessage() {
  char endingChar[] = " \n";
  endingChar[0] = (char)26;
  sleep_ms(10000);
  printf("SendingAT\n");
  // uart_puts(UART_ID, "AT\n");
  printf("anju");
  uart_puts(UART_ID, "AT+CMGF=1\n");
  sleep_ms(1000);

  printf("chhetri");
  uart_puts(UART_ID, "AT+CMGS=\"+9779848655422\"\r\n");
  sleep_ms(1000);

  printf("yunika");
  uart_puts(
      UART_ID,
      "Fall detected. \nLocation: \nLatitude: 27.61821. \nLongitude: 85.5545");
  sleep_ms(100);

  printf("Bajracharya");
  uart_puts(UART_ID, endingChar);
  sleep_ms(100);
  printf("Done");
}

void sendAT() {
  sleep_ms(2000);
  printf("Sending AT\n");
  sleep_ms(2000);
  uart_puts(UART_ID, "AT\n");
  printf("Sent");
}

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

void readAndDisplay() {
  int16_t acceleration[3], gyro[3], temp;
  mpu6050_read_raw(acceleration, gyro, &temp);
  printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1],
         acceleration[2]);
  printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
  // Temperature is simple so use the datasheet calculation to get deg C.
  // Note this is chip temperature.
  printf("Temp. = %f\n", (temp / 340.0) + 36.53);

  sleep_ms(100);
}

void initUART() {
  // Set up our UART with a basic baud rate.
  uart_init(UART_ID, 2400);

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

  // Set up a RX interrupt
  // We need to set up the handler first
  // Select correct interrupt for the UART we are using
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  // And set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  irq_set_enabled(UART_IRQ, true);

  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_ID, true, false);
}

void initI2C() {
  i2c_init(i2c_default, 9600);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                             GPIO_FUNC_I2C));

  mpu6050_reset();
}
int main() {
  stdio_init_all();

  initUART();
  sendAT();
  // initI2C();

  while (1) {
    // tight_loop_contents();
    readAndDisplay();
  }
}

/// \end:uart_advanced[]
