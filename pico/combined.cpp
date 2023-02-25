/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "TFLModel.h"
#include "mpu.h"
#include "uart.h"

uint initializeLED() {
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  return LED_PIN;
}

int main() {
  bool sent = false;
  stdio_init_all();
  const uint LED_PIN = initializeLED();

  initUART();
  initMPU();
  TFLModel model = TFLModel();

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

    // if (acceleration[2] < 0) {
    //   if (!sent) {
    //     gpio_put(LED_PIN, 1);
    //     sendText();
    //     sent = true;
    //     gpio_put(LED_PIN, 0);
    //   }
    // } else {
    // }

    model.loop();

    sleep_ms(100);
  }

  return 0;
}
