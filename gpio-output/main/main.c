#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN_1 21
#define LED_PIN_2 33

void app_main(void)
{
    gpio_pad_select_gpio(LED_PIN_1); 
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT); 

    gpio_pad_select_gpio(LED_PIN_2); 
    gpio_set_direction(LED_PIN_2, GPIO_MODE_OUTPUT); 

    while(1) {

        gpio_set_level(LED_PIN_1, 1);
        printf("LED_PIN_1 ON\n");
        gpio_set_level(LED_PIN_2, 0);
        printf("LED_PIN_2 OFF\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);

        gpio_set_level(LED_PIN_1, 0);
        printf("LED_PIN_1 OFF\n");
        gpio_set_level(LED_PIN_2, 1);
        printf("LED_PIN_2 ON\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
