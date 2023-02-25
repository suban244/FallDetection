#include "mpu.h"
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
void mpu6050_reset() {
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

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
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

void initMPU() {
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
}