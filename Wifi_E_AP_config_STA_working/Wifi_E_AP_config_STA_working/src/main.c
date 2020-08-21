#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"

#include "driver/gpio.h"

#include "globales_GPIO_WIFI.h"
#include "GPIOTask.h"
#include "SOCKETTask.h"



void app_main() {

    //The maximum number of items the queue can hold at any one time.
    const UBaseType_t uxQueueLength=5;

    struct GPIOTask_to_Main_struct_t GPIOTask_to_Main_struct={0};
    struct Main_to_GPIOTask_struct_t Main_to_GPIOTask_struct={0};
    struct SOCKETTask_to_Main_struct_t SOCKETTask_to_Main_struct={0};
    struct Main_to_SOCKETTask_struct_t Main_to_SOCKETTask_struct={0};
    Main_to_GPIOTask_struct.LedPin=9;
    
    struct GPIOTask_Queues_t GPIOTask_Queues={0};
    struct SOCKETTask_Queues_t SOCKETTask_Queues={0};

    GPIOTask_Queues.GPIOTask_to_Main_Queue =xQueueCreate( 
        uxQueueLength, 
        sizeof(struct GPIOTask_to_Main_struct_t) );

    GPIOTask_Queues.Main_to_GPIOTask_Queue =xQueueCreate( 
        uxQueueLength, 
        sizeof(struct Main_to_GPIOTask_struct_t) );

    SOCKETTask_Queues.SOCKETTask_to_Main_Queue =xQueueCreate( 
        uxQueueLength, 
        sizeof(struct SOCKETTask_to_Main_struct_t) );

    SOCKETTask_Queues.Main_to_SOCKETTask_Queue =xQueueCreate( 
        uxQueueLength, 
        sizeof(struct Main_to_SOCKETTask_struct_t) );

    xTaskCreate(&GPIOTask, "GPIOTask", 2048, &GPIOTask_Queues, 5, NULL);
    xTaskCreate(&SOCKETTask, "SOCKETTask", 4096, &SOCKETTask_Queues, 5, NULL);

    BaseType_t returnValue;
 for( ;; )
        {
            //La señalizacion de los eventos exteriores no es tan importante 
            //como transmitir los cambios en los pulsadores, 
            //así que interesa que el Main thread esté escuchando los pulsadores 
            //el mayor tiempo posible.

             printf("*********************Inicio del Loop MAIN******************\n");
            //Revisa el estado de los pines y si el buffer está vacío, espera un momento
             returnValue=xQueueReceive(
                                   GPIOTask_Queues.GPIOTask_to_Main_Queue,
                                   &GPIOTask_to_Main_struct,
                                   10*portTICK_PERIOD_MS 
                               );
            if(returnValue==pdTRUE){
                printf("valor recibido en Main <- GPIOTask:%d\n",GPIOTask_to_Main_struct.PushPin);
                Main_to_SOCKETTask_struct.SwitchPin=GPIOTask_to_Main_struct.PushPin;
                xQueueSend(
                            SOCKETTask_Queues.Main_to_SOCKETTask_Queue,
                            &Main_to_SOCKETTask_struct,
                            0
                        );
            }
            //Revisa el estado de los leds, si el buffer está vacío vuelve inmediatamente
            
             returnValue=xQueueReceive(
                                   SOCKETTask_Queues.SOCKETTask_to_Main_Queue,
                                   &SOCKETTask_to_Main_struct,
                                   10*portTICK_PERIOD_MS 
                               );
            if(returnValue==pdTRUE){
                printf("valor recibido en Main <- SOCKETTask:%d\n",SOCKETTask_to_Main_struct.LedPin);
                Main_to_GPIOTask_struct.LedPin=SOCKETTask_to_Main_struct.LedPin;
                //Envía el estado de los leds, pero si el buffer está lleno vuelve inmediatamente
                returnValue=xQueueSend(
                                   GPIOTask_Queues.Main_to_GPIOTask_Queue,
                                   &Main_to_GPIOTask_struct,
                                   0
                               );
                if(returnValue==pdTRUE){
                    printf("valor enviado de Main -> GPIOTask:%d\n",Main_to_GPIOTask_struct.LedPin);
                }
            }
            //vTaskDelay(timeToWait);


        }

}