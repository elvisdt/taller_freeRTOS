#include <stdio.h>
#include <string.h>


#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include ""

#include "esp_system.h"


/***************************************************************
 * DEFINITIONS
 ***************************************************************/
#define BLINK_GPIO GPIO_NUM_2

#define DEBUG_TASK_MONITOR
/***************************************************************
 * VARIABLES
 ***************************************************************/
TaskHandle_t hello_task_handle = NULL;
TaskHandle_t blink_task_handle = NULL;


int delay = 100;

/***************************************************************
 * TASK AND FUNCTIONS
 ***************************************************************/



// Tarea que imprime "Hola mundo!" periódicamente.
void hello_task(void *pvParameter){
    int count = 0;
    while(1){
        printf("Hola mundo! %d\n", count);
        vTaskDelay(pdMS_TO_TICKS(1000));
        count ++;
        if(count==5){
            delay = 500;
        }else if (count ==10){
            delay = 1000;
            count = 0;
        }
        
    }
    
    // vTaskDelete(hello_task_handle);
}



// Tarea que parpadea un LED
void blink_task(void *pvParameter){

    // Definir el TAG para los mensajes de log de esta tarea
    static const char *BLINK_TAG = "BLINK_TASK";
    esp_log_level_set(BLINK_TAG, ESP_LOG_INFO);

    // Restablecer GPIO y configurar modo salida.
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


    // variable para identificar estado del led
    static uint8_t s_led_state = 0;
  
    while(1) {       
        ESP_LOGI(BLINK_TAG, "LED Status: %s!", s_led_state == true ? "ON" : "OFF");
 
        // Apagar/Encender el LED por 1 segundo
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(pdMS_TO_TICKS(delay));
  
    }
}





void app_main(){

    /*
        @pvTaskCode: Función a llamar.
        @pcName: Nombre de la tarea.
        @usStackDepth: Tamaño de la pila de la tarea.
        @pvParameters: Parámetro a pasar a la función.
        @uxPriority: Prioridad de la tarea.(0 to configMAX_PRIORITIES - 1) .
        @pxCreatedTask: Task handle. (identificador)
    */
  
    // Crea la tarea hello_task 
    xTaskCreate(hello_task, "hello_task", 1024*5, NULL, configMAX_PRIORITIES -1, &hello_task_handle);
    // xTaskCreatePinnedToCore()

    // Crea la tarea blink_task
    xTaskCreate(blink_task, "blink_task", 1024*2, NULL, configMAX_PRIORITIES -2, &blink_task_handle);


}
