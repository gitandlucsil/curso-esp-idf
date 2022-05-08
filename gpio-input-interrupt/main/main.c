#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define LED_PIN_1 21
#define BUTTON_PIN 6

xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
    uint32_t gpio_num = (uint32_t) args;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void buttonTask(void *pvParamaeters) 
{
    uint32_t gpio_num = 0;
    while(1) 
    {
        xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY);
        ESP_LOGI("buttonTask", "GPIO[%d] intr, val: %d\n", gpio_num, gpio_get_level(gpio_num));

        if(gpio_get_level(gpio_num) == 0) {
            gpio_set_level(LED_PIN_1, 1);
        } else {
            gpio_set_level(LED_PIN_1, 0);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_pad_select_gpio(LED_PIN_1); 
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT); 

    gpio_pad_select_gpio(BUTTON_PIN); 
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT); 
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    gpio_evt_queue = xQueueCreate(3, sizeof(uint32_t));
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 2, NULL); 
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void *) BUTTON_PIN);

}
