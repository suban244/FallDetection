# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}

  ${CMAKE_CURRENT_LIST_DIR}/combined.cpp
  ${CMAKE_CURRENT_LIST_DIR}/mpu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/model.h
  ${CMAKE_CURRENT_LIST_DIR}/model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/mpu.h
  ${CMAKE_CURRENT_LIST_DIR}/uart.cpp
  ${CMAKE_CURRENT_LIST_DIR}/uart.h
  ${CMAKE_CURRENT_LIST_DIR}/TFLModel.h
  ${CMAKE_CURRENT_LIST_DIR}/TFLModel.cpp
  ${CMAKE_CURRENT_LIST_DIR}/output_handeler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/output_handeler.h
)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}

  pico_stdlib

  hardware_pwm
  pico-tflmicro
  hardware_i2c
  hardware_uart
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)