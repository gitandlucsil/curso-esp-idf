#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED1 (1ULL<<21)    //GPIO1
#define LED2 (1ULL<<33)    //GPIO2


void app_main(void)
{

    REG_WRITE(GPIO_ENABLE_REG, LED1|LED2); //enable GPIO1 and GPIO2 
    
    while(true)
    {
        REG_WRITE(GPIO_OUT_W1TS_REG, LED1|LED2);   //set GPIO1 and GPIO2
        vTaskDelay(1000 / portTICK_PERIOD_MS);     //delay 1 secon
        REG_WRITE(GPIO_OUT_W1TC_REG, LED1|LED2);   //clear GPIO1 and GPIO2
        vTaskDelay(1000 / portTICK_PERIOD_MS);     //delay 1 secon
    }

}
