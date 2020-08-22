#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

void IniGPIO(uint8_t LedArray[],uint8_t LedArrayLen,uint8_t SwitchArray[],uint8_t SwitchArrayLen){

    gpio_config_t gpioConfig;
    gpioConfig.pin_bit_mask=0;
    for(uint8_t i=0;i<LedArrayLen;i++)
    {
        gpioConfig.pin_bit_mask =gpioConfig.pin_bit_mask |(1 << LedArray[i]);
    }
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpioConfig);   

    gpio_config_t gpioConfig2;
    gpioConfig2.pin_bit_mask=0;
    for(uint8_t i=0;i<SwitchArrayLen;i++)
    {
        gpioConfig2.pin_bit_mask =gpioConfig2.pin_bit_mask |(1ULL << SwitchArray[i]);
    }
    //gpioConfig2.pin_bit_mask =gpioConfig2.pin_bit_mask |(1ULL<< GPIO_Switch3);
    gpioConfig2.mode = GPIO_MODE_INPUT;
    gpioConfig2.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig2.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig2.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpioConfig2);   

}
void GPIOTask(void *parms) {

    //Guardamos el dato recibido ya que es la Cola de Mensajes compartida con el Thread Principal
    struct GPIOTask_Queues_t* GPIOTask_Queues=(struct GPIOTask_Queues_t*)parms;
    QueueHandle_t GPIOTask_to_Main_Queue=GPIOTask_Queues->GPIOTask_to_Main_Queue;
    QueueHandle_t Main_to_GPIOTask_Queue=GPIOTask_Queues->Main_to_GPIOTask_Queue;

    uint8_t LedArray[]={GPIO_Led1,GPIO_Led2,GPIO_Led3,GPIO_Led4};
    uint8_t LedArrayLen=sizeof(LedArray) / sizeof(LedArray[0]);
    uint8_t SwitchArray[]={GPIO_Switch1,GPIO_Switch2,GPIO_Switch3,GPIO_Switch4};
    uint8_t SwitchArrayLen=sizeof(SwitchArray) / sizeof(SwitchArray[0]);

    IniGPIO(LedArray,LedArrayLen,SwitchArray,SwitchArrayLen);
    //Inicializamos la estructura de datos que sirve para enviar el estado de los pines a la rutina 
    //principal
    struct GPIOTask_to_Main_struct_t GPIOTask_to_Main_struct={0};
    struct Main_to_GPIOTask_struct_t Main_to_GPIOTask_struct={0};
    //El tiempo de espera entre lecturas/ewscrituras de los pines 
    uint32_t timeToWait=10*portTICK_PERIOD_MS;
    
 for( ;; )
        {
            //recive el valor de los switches, si la cola está vacía, vuelve inmediatamente
             BaseType_t returnValue=xQueueReceive(
                                   Main_to_GPIOTask_Queue,
                                   &Main_to_GPIOTask_struct,
                                   0  
                               );
            GPIOTask_to_Main_struct.PushPin=0;
            printf("GPIO:|");
            for(int i=0;i<LedArrayLen;i++) {
                if(LedArray[i]<10)printf("0");
                printf("%d|",LedArray[i]);
            }            
            for(int i=0;i<SwitchArrayLen;i++) {
                if(SwitchArray[i]<10)printf("0");
                printf("%d|",SwitchArray[i]);
            }            
            printf("\n");
            printf("DATA:|");
            for(uint8_t i=0;i<LedArrayLen;i++) {
                //Uso el tipo unsigned long long porque es de 64 bits.
                //puesto que algunas placas de ESP32 tienen más de 32 pines GPIO,
                //el tipo de 32 bits no sería suficiente para abarcarlos a todos.
                unsigned long long bit=(Main_to_GPIOTask_struct.LedPin&(1ULL<<i))>>i;
                printf(" %d|",(int)bit);
                gpio_set_level(LedArray[i],(int)bit);
            }
            for(int i=0;i<SwitchArrayLen;i++) {
                int value=gpio_get_level(SwitchArray[i]);
                printf(" %d|",value);
                if(value)GPIOTask_to_Main_struct.PushPin|=(1 << i);
            }
            printf("\n");
            //Envía el valor de los switches, si el buffer está lleno, vuelve inmediatamente
             returnValue=xQueueSendToBack(
                                   GPIOTask_to_Main_Queue,
                                   &GPIOTask_to_Main_struct,
                                   0  
                               );
            if(returnValue!=pdTRUE)printf("QueueSendToBack ha devuelto error\n");
            vTaskDelay(timeToWait);
        }

        /* Tasks must not attempt to return from their implementing
        function or otherwise exit.  In newer FreeRTOS port
        attempting to do so will result in an configASSERT() being
        called if it is defined.  If it is necessary for a task to
        exit then have the task call vTaskDelete( NULL ) to ensure
        its exit is clean. */
        vTaskDelete( NULL );
}