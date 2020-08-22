
#ifndef globales_GPIO_WIFI
#define globales_GPIO_WIFI

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"


#define GPIO_LedBlue GPIO_NUM_4
#define GPIO_LedYellow GPIO_NUM_0
#define GPIO_LedRed GPIO_NUM_2
#define GPIO_LedGreen GPIO_NUM_15

//Definiciones de salidas de leds.
//se pueden añadir todas las salidas como tu placa ESP32 permita
//pero debes actualizar el array LedArray[]
#define GPIO_Led1 GPIO_NUM_4
#define GPIO_Led2 GPIO_NUM_0
#define GPIO_Led3 GPIO_NUM_2
#define GPIO_Led4 GPIO_NUM_15

//Definiciones de entradas de switches (o pulsadores).
//se pueden añadir todas las entradas como tu placa ESP32 permita
//pero debes actualizar el array SwitchArray[]
#define GPIO_Switch1 GPIO_NUM_33
#define GPIO_Switch2 GPIO_NUM_35
#define GPIO_Switch3 GPIO_NUM_32
#define GPIO_Switch4 GPIO_NUM_34

struct esp_event_loop_handler_struct_t{
    int hola;
};


struct GPIOTask_to_Main_struct_t{
    uint8_t PushPin;
};
struct Main_to_GPIOTask_struct_t{
    uint8_t LedPin;
};
struct GPIOTask_Queues_t{
    QueueHandle_t GPIOTask_to_Main_Queue;
    QueueHandle_t Main_to_GPIOTask_Queue;
};


struct SOCKETTask_to_Main_struct_t{
    uint8_t LedPin;
};
struct Main_to_SOCKETTask_struct_t{
    uint8_t SwitchPin;
};
struct SOCKETTask_Queues_t{
    QueueHandle_t SOCKETTask_to_Main_Queue;
    QueueHandle_t Main_to_SOCKETTask_Queue;
};
#endif //globales_GPIO_WIFI