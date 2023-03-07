#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

static int addr = 0x68;

static void mpu6050_reset() {
  printf("sending data");
  uint8_t buf[] = {0x6B, 0x00};
  i2c_write_blocking(i2c_default, addr, buf, 2, false);
  uint8_t buf2[] = {0x1C, 0x10};
  i2c_write_blocking(i2c_default, addr, buf2, 2, false);
  uint8_t buf3[] = {0x1B, 0x10};
  i2c_write_blocking(i2c_default, addr, buf3, 2, false);
  sleep_ms(100);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
  uint8_t buffer[6];

  uint8_t val = 0x3B;
  i2c_write_blocking(i2c_default, addr, &val, 1,
                     true);  // true to keep master control of bus
  i2c_read_blocking(i2c_default, addr, buffer, 6, false);

  for (int i = 0; i < 3; i++) {
    accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
  }

  val = 0x43;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 6,
                    false);  // False - finished with bus

  for (int i = 0; i < 3; i++) {
    gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    ;
  }

  val = 0x41;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 2,
                    false);  // False - finished with bus

  *temp = buffer[0] << 8 | buffer[1];
}
int main() {
  stdio_init_all();
  bool state = true;
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, state);

  const int SDA = 8;
  const int SCL = 9;

  i2c_init(i2c_default, 9600);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(SDA, SCL, GPIO_FUNC_I2C));

  // sleep_ms(5000);
  mpu6050_reset();

  int16_t acceleration[3], gyro[3], temp;
  float acc[3];

  while (1) {
    mpu6050_read_raw(acceleration, gyro, &temp);
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1],
           acceleration[2]);

    // printf("runing");
    sleep_ms(100);
    gpio_put(LED_PIN, state);
    state = !state;
  }
}
