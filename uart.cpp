#include "uart.h"

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

  printf(".....\n");
  uart_puts(UART_ID,
            "Fall detected. \nLocation: \nLatitude: 27.61821. \nLongitude: "
            "85.5545");
  sleep_ms(100);

  printf("\n..........");
  uart_puts(UART_ID, endingChar);
  sleep_ms(100);
  printf("Done");
}

void initUART() {
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
}