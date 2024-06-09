/* Ejemplo de esp_timer (temporizador de alta resolución)

   Este código de ejemplo está en el Dominio Público (o con licencia CC0, a tu elección).

   A menos que lo requiera la ley aplicable o se acuerde por escrito,
   este software se distribuye "TAL CUAL", SIN GARANTÍAS NI
   CONDICIONES DE NINGÚN TIPO, ya sea expresas o implícitas.
*/

/***************************************************************
 * INCLUDES
 ***************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"


/***************************************************************
 * DEFINITIONS
 ***************************************************************/

#define TAG  "MAIN"

/***************************************************************
 * VARIABLES
 ***************************************************************/


/***************************************************************
 * HEADER FUNCTIONS
 ***************************************************************/

static void periodic_timer_callback(void* arg);

void app_main(void)
{
    /* Crear dos temporizadores:
    * 1. un temporizador periódico que se ejecutará cada 0.5s y mostrará un mensaje
    * 2. un temporizador de una sola ejecución que se activará después de 5s, y
    *    reiniciará el temporizador periódico con un período de 1s.
    */

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* El nombre es opcional, pero puede ayudar a identificar el temporizador al depurar */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* El temporizador ha sido creado pero aún no está en funcionamiento */


    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, (uint64_t)(2*1e6)));
    ESP_LOGI(TAG, "Started timers, time since boot: %lld us", esp_timer_get_time());
    
    usleep
    /*
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(oneshot_timer));
    ESP_LOGI(TAG, "Stopped and deleted timers");
    */
}

static void periodic_timer_callback(void* arg){
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
}

