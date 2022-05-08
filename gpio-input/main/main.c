#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN_1 21
#define BUTTON_PIN 6

void app_main(void)
{
    gpio_pad_select_gpio(LED_PIN_1); 
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT); 

    gpio_pad_select_gpio(BUTTON_PIN); 
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT); 
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while(1) {

        if(gpio_get_level(BUTTON_PIN) == 0) {
            gpio_set_level(LED_PIN_1, 1);
            ESP_LOGI("Button", "Button pressed");
        } else {
            gpio_set_level(LED_PIN_1, 0);
            ESP_LOGI("Button", "Button released");
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
