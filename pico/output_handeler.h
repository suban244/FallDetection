#pragma once

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

// Called by the main loop to produce some output based on the x and y values
void HandleOutput(tflite::ErrorReporter* error_reporter, float x_value,
                  float y_value);