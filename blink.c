#include "pico/stdlib.h"
int main() {
  bool state = 1;
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, state);

  while (1) {
    sleep_ms(400);
    state = !state;
    gpio_put(LED_PIN, state);
  }
}
