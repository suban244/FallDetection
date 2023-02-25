#pragma once
#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

void initMPU();
void mpu6050_reset();

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp);

void preprocessAcc(int16_t accleration[3], float acc[3]);
