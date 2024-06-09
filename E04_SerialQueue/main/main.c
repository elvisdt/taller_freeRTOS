/* Ejemplo asíncrono de UART, 
    para controlar el valor el delay del led.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include <stdlib.h>




/***************************************************
 * DEFINITIONS
 *************************************************/
// uart
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define RX_BUF_SIZE 1024

// gpio
#define BLINK_GPIO GPIO_NUM_2
/***************************************************
 * VARIABLES
 *************************************************/
QueueHandle_t buffer;//Objeto de la cola

void int_cfg(void){
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* logName, const char* data){
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
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

    uint32_t number=500;
    uint32_t delay_ms=500;
    while(1) {       
        
        if (xQueueReceive(buffer, &number, pdMS_TO_TICKS(1)) == true){
            ESP_LOGI(BLINK_TAG, "delay actualizado: %lu ms", number);
            delay_ms = number;
        }

        ESP_LOGI(BLINK_TAG, "LED Status: %s!", s_led_state == true ? "ON" : "OFF");

        // Apagar/Encender el LED por 1 segundo
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(pdMS_TO_TICKS(delay_ms));

  
    }
}




void rx_task(void *arg){
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);

    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);

    char str_snd[200]={0};
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, pdMS_TO_TICKS(1000));
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);

            // Verificar si los datos empiezan con 'L'
            if (data[0] == 'L') {
                // Intentar separar la letra 'L' y el número
                // L,5000
                char letter;
                uint32_t number;
                if (sscanf((char*)data, "%c,%lu", &letter, &number) == 2) {
                    // La conversión fue exitosa, ahora puedes usar 'number'
                    ESP_LOGI(RX_TASK_TAG, "Data Procesada delay: %lu ms", number);

                    //Envia la variable a la cola
                    xQueueSend(buffer, &number, pdMS_TO_TICKS(0));

                    sprintf(str_snd,"valor correcto, nuevo delay: %lu ms\r\n",number);
                    sendData(RX_TASK_TAG, str_snd);
                } else {
                    // La conversión falló, los datos después de 'L' no son un número
                    ESP_LOGW(RX_TASK_TAG, "Data despues de  'L' no es un numero");
                    sprintf(str_snd,"Error valor incorrecto \r\n");
                    sendData(RX_TASK_TAG, str_snd);
                }
            } else {
                // Los datos no empiezan con 'L'
                ESP_LOGE(RX_TASK_TAG, "Data no empieza por 'L'");
                sprintf(str_snd,"Error parametro desconocido\r\n");
            }

        }
    }
    free(data);
}


void app_main(void)
{
    // inicializar configuracion
    int_cfg();
    //crea la cola *buffer* con 10 slots de 4 Bytes
    buffer = xQueueCreate(10, sizeof(uint32_t)); 

    // inicalizar tareas
    xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(blink_task, "blink_task", 1024 * 2, NULL, configMAX_PRIORITIES - 2, NULL);
}

