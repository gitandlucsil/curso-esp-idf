#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define LED_PIN_1 21
#define LED_PIN_2 33
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
            gpio_set_level(LED_PIN_2, 0);
        } else {
            gpio_set_level(LED_PIN_1, 0);
            gpio_set_level(LED_PIN_2, 1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    gpio_config_t io_config;
    io_config.pin_bit_mask = (1ULL<<LED_PIN_1) | (1ULL<<LED_PIN_2);
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config);

    io_config.pin_bit_mask = (1ULL<<BUTTON_PIN);
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_config);


    gpio_evt_queue = xQueueCreate(3, sizeof(uint32_t));
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 2, NULL); 
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void *) BUTTON_PIN);

}
