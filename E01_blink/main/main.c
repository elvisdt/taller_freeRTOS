/* 
    Ejemplo de blink de un led
    utlizando sleep
*/

/***************************************************************
 * INCLUDES
 ***************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"

/***************************************************************
 * DEFINITIONS
 ***************************************************************/

#define TAG  "MAIN"

#define BLINK_GPIO GPIO_NUM_2
/***************************************************************
 * VARIABLES
 ***************************************************************/
static uint8_t s_led_state = 0;


/***************************************************************
 * HEADER FUNCTIONS
 ***************************************************************/

void app_main(void){

    /*---setup---*/
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    
    // contador 
    int count = 0;
    /*---loop---*/
    while(1) {
        count ++; // incrementar cada 

        // blink  del led cada 1 segundos
        ESP_LOGI(TAG, "LED Status: %s!", s_led_state == true ? "ON" : "OFF");
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;

        if (count%2==0){
            printf("Hola mundo ! \r\n");
        }
        
        
        sleep(1);
    }
}