/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/uart.h"

#include <stdio.h>

#include "hardware/irq.h"
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
  printf("Received");

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
  // Set up our UART with a basic baud rate.
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

  // Set up a RX interrupt
  // We need to set up the handler first
  // Select correct interrupt for the UART we are using
  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  // And set up and enable the interrupt handlers
  // irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  // irq_set_enabled(UART_IRQ, true);

  // Now enable the UART to send interrupts - RX only
  // uart_set_irq_enables(UART_ID, true, false);

  // OK, all set up.
  // Lets send a basic string out, and then run a loop and wait for RX
  // interrupts The handler will count them, but also reflect the incoming data
  // back with a slight change!

  sleep_ms(5000);
  printf("SendingTEXT \n");
  // uart_puts(UART_ID, "AT\n");
  // sleep_ms(5000);

  sendText();

  while (1) tight_loop_contents();
}

/// \end:uart_advanced[]
