#ifndef SOCKETTask_h
#define SOCKETTask_h
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <sys/socket.h>
#include <sys/types.h> 
#include "string.h"

#include "globales_GPIO_WIFI.h"
#include "SharedStruct.h"

#define MY_WIFI_NAME "RODRIGUEZ_4"
#define MY_PASSSWORD "PITUFILLA"
// #define MY_WIFI_NAME "HUAWEI Y5 2019\n",
// #define MY_PASSSWORD "mayusculas\n",
#define MY_PORT_NUMBER 27015

int GOT_IP=pdFALSE;
static const char *TAG_SOCKETTask = "|WIFI_E|SOCKETTask|";
/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      "ESP32"
#define EXAMPLE_ESP_WIFI_PASS      "PASSWORD"
#define EXAMPLE_MAX_STA_CONN       5
//**************************************************************************
//***************************** Globales autoRecovery **********************
//**************************************************************************

// probando el sistema autoRecovery;
bool autoRecovery_Infinite_Loop=false;
// La wifi del ESP32 está configurada como Access Point (como si fuera un router)
// y permite la conexion de clientes
bool AP_funcionando=false;
//**************************************************************************
void case_SYSTEM_EVENT_SCAN_DONE(system_event_t *event){
    printf("Number of access points found: %d\n",
        event->event_info.scan_done.number);
        uint16_t apCount = event->event_info.scan_done.number;
        if (apCount == 0) {
            return ;
        }
         wifi_ap_record_t *list =
        (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, list));
        int i;
        for (i=0; i<apCount; i++) {
            char *authmode;
            switch(list[i].authmode) {
                case WIFI_AUTH_OPEN:
                    authmode = "WIFI_AUTH_OPEN";
                    break;
                case WIFI_AUTH_WEP:
                    authmode = "WIFI_AUTH_WEP";
                    break;
                case WIFI_AUTH_WPA_PSK:
                    authmode = "WIFI_AUTH_WPA_PSK";
                    break;
                case WIFI_AUTH_WPA2_PSK:
                    authmode = "WIFI_AUTH_WPA2_PSK";
                    break;
                case WIFI_AUTH_WPA_WPA2_PSK:
                    authmode = "WIFI_AUTH_WPA_WPA2_PSK";
                    break;
                default:
                    authmode = "Unknown";
                    break;
            }
            printf("ssid=%s, rssi=%d, authmode=%s\n",
            list[i].ssid, list[i].rssi, authmode);
        }
        free(list); 
}

void reconnect(){
    
        printf("ORP_WIFI|reconnect.\r\n");
        esp_err_t esp_err=esp_wifi_connect();
        switch (esp_err)
        {
        case ESP_OK:
            printf("ORP_WIFI|esp_wifi_connect returns ESP_OK\r\n");
            break;
        
        default:
            printf("ORP_WIFI|esp_wifi_connect returns UNKNOW\r\n");
            break;
        }
        ESP_ERROR_CHECK(esp_err);
}

//el tamaño del buffer debe ser como mínimo de 40 bytes;
char* wifi_reason_to_string(uint8_t reasson,char* buffer){
        switch (reasson)
        {
        case WIFI_REASON_UNSPECIFIED:
        strcpy(buffer,"WIFI_REASON_UNSPECIFIED\n");
                return(buffer);        
        case WIFI_REASON_AUTH_EXPIRE:
        strcpy(buffer,"WIFI_REASON_AUTH_EXPIRE\n");
                return(buffer);        
        
        case WIFI_REASON_AUTH_LEAVE:
        strcpy(buffer,"WIFI_REASON_AUTH_LEAVE\n");
                return(buffer);        
        
        case WIFI_REASON_ASSOC_EXPIRE:
        strcpy(buffer,"WIFI_REASON_ASSOC_EXPIRE\n");
                return(buffer);        
        
        case WIFI_REASON_ASSOC_TOOMANY:
        strcpy(buffer,"WIFI_REASON_ASSOC_TOOMANY\n");
                return(buffer);        
        
        case WIFI_REASON_NOT_AUTHED:
        strcpy(buffer,"WIFI_REASON_NOT_AUTHED\n");
                return(buffer);        
        
        case WIFI_REASON_NOT_ASSOCED:
        strcpy(buffer,"WIFI_REASON_NOT_ASSOCED\n");
                return(buffer);        
        
        case WIFI_REASON_ASSOC_LEAVE:
        strcpy(buffer,"WIFI_REASON_ASSOC_LEAVE\n");
                return(buffer);        
        
        case WIFI_REASON_ASSOC_NOT_AUTHED:
        strcpy(buffer,"WIFI_REASON_ASSOC_NOT_AUTHED\n");
                return(buffer);        
        
        case WIFI_REASON_DISASSOC_PWRCAP_BAD:
        strcpy(buffer,"WIFI_REASON_DISASSOC_PWRCAP_BAD\n");
                return(buffer);        
        
        case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
        strcpy(buffer,"WIFI_REASON_DISASSOC_SUPCHAN_BAD\n");
                return(buffer);        
        
        case WIFI_REASON_IE_INVALID:
        strcpy(buffer,"WIFI_REASON_IE_INVALID\n");
                return(buffer);        
        
        case WIFI_REASON_MIC_FAILURE:
        strcpy(buffer,"WIFI_REASON_MIC_FAILURE\n");
                return(buffer);        
        
        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        strcpy(buffer,"WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT\n");
                return(buffer);        
        
        case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
        strcpy(buffer,"WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT\n");
                return(buffer);        
        
        case WIFI_REASON_IE_IN_4WAY_DIFFERS:
        strcpy(buffer,"WIFI_REASON_IE_IN_4WAY_DIFFERS\n");
                return(buffer);

        case WIFI_REASON_GROUP_CIPHER_INVALID:
        strcpy(buffer,"WIFI_REASON_GROUP_CIPHER_INVALID\n");
                return(buffer);

        case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
        strcpy(buffer,"WIFI_REASON_PAIRWISE_CIPHER_INVALID\n");
                return(buffer);

        case WIFI_REASON_AKMP_INVALID:
        strcpy(buffer,"WIFI_REASON_AKMP_INVALID\n");
                return(buffer);

        case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
        strcpy(buffer,"WIFI_REASON_UNSUPP_RSN_IE_VERSION\n");
                return(buffer);

        case WIFI_REASON_INVALID_RSN_IE_CAP:
        strcpy(buffer,"WIFI_REASON_INVALID_RSN_IE_CAP\n");
                return(buffer);        
        
        case WIFI_REASON_802_1X_AUTH_FAILED:
        strcpy(buffer,"WIFI_REASON_802_1X_AUTH_FAILED\n");
                return(buffer);        
        
        case WIFI_REASON_CIPHER_SUITE_REJECTED:
        strcpy(buffer,"WIFI_REASON_CIPHER_SUITE_REJECTED\n");
                return(buffer);        
        
        case WIFI_REASON_BEACON_TIMEOUT:
        strcpy(buffer,"WIFI_REASON_BEACON_TIMEOUT\n");
                return(buffer);        
        
        case WIFI_REASON_NO_AP_FOUND:
        strcpy(buffer,"WIFI_REASON_NO_AP_FOUND\n");
                return(buffer);        
        
        case WIFI_REASON_AUTH_FAIL:
        strcpy(buffer,"WIFI_REASON_AUTH_FAIL\n");
                return(buffer);        
        
        case WIFI_REASON_HANDSHAKE_TIMEOUT:
        strcpy(buffer,"WIFI_REASON_HANDSHAKE_TIMEOUT\n");
                return(buffer);        
        
        case WIFI_REASON_CONNECTION_FAIL:
        strcpy(buffer,"WIFI_REASON_CONNECTION_FAIL\n");
                return(buffer);        
        
        default:
        strcpy(buffer,"UNKNOW\n");
                return(buffer);  
        }
        
        return(buffer);  
}

//Inicializa un socket servidor con socket(),setsockopt() bind() y listen()
//si una de las funciones de la API de socket devuelve error,
//se cierra el socket y se devuelve el número devuelto
int iniSocket (char* My_IPString,int portNumber){
        int ret;
        
        printf("ORP_WIFI|Dentro de iniSocket()\n");
        
        printf("ORP_WIFI|socket()\n");
        int ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ServerSocket<0){printf(
            "ORP_WIFI|socket() ha devuelto un numero negativo\n");
            return ServerSocket;
            }
        printf("ORP_WIFI|socket() ha devuelto un SOCKET\n");
        // const int optlen=10;
        // const char optval[optlen];
        // ret=setsockopt(
        //     ServerSocket,
        //     level,         //The level at which the option is defined (for example, SOL_SOCKET).
        //     optname,       //The socket option for which the value is to be set (for example, SO_BROADCAST).
        //     &optval,       //A pointer to the buffer in which the value for the requested option is specified.
        //     optlen
        // );
        
        //Set ServerSocket as non-Blocking
        u_long iMode = 1;
        ret = ioctlsocket(ServerSocket, FIONBIO, &iMode);

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(portNumber);
        
        printf("ORP_WIFI|bind()\n");
        ret=bind(ServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (ret<0){
            printf("ORP_WIFI|bind() ha devuelto un numero negativo\n");
            closesocket(ServerSocket);
            return ret;
            }

        printf("ORP_WIFI|listen()\n");
        ret=listen(ServerSocket, 1);
        if (ret<0){
            printf("ORP_WIFI|listen() ha devuelto un numero negativo\n");
            closesocket(ServerSocket);
            return ret;
            }
        return ServerSocket;
       
}

esp_err_t event_handler(void *ctx, system_event_t *event){
    
char My_IPString[50];

        //struct esp_event_loop_handler_struct_t* esp_event_loop_struct;
        //esp_event_loop_struct=ctx;
        //printf("RECIEVER esp_event_loop_struct=%d\n",esp_event_loop_struct->hola);
        // int *unNumero_p=ctx;
        // int unNumero=&unNumero;
        // printf("RECIEVER unNumero=%d\n", unNumero);
        // printf("RECIEVER unNumero_p=%d\n", *unNumero_p);
        // printf("RECIEVER pointer=%p\n", ctx);
        // printf("RECIEVER sizeof pointer=%d\n", sizeof(ctx));
    switch (event->event_id)
    {
    case SYSTEM_EVENT_SCAN_DONE:
        printf("ORP_WIFI|ESP32 finish scanning AP\n");
        case_SYSTEM_EVENT_SCAN_DONE(event);
        break;
    
    case SYSTEM_EVENT_WIFI_READY:
        printf("ORP_WIFI|ESP32 WiFi ready\n");
        break;
        
    case SYSTEM_EVENT_STA_START:
        printf("ORP_WIFI|ESP32 station start\n");
        reconnect();
        break;
        
    case SYSTEM_EVENT_STA_STOP:
        printf("ORP_WIFI|ESP32 station stop\n");
        break;
        
    case SYSTEM_EVENT_STA_CONNECTED:
        printf("ORP_WIFI|ESP32 station connected to AP. SSID: ");
        printf((char *)(&event->event_info.connected.ssid));
        printf("\n");
        break;
        
    case SYSTEM_EVENT_STA_DISCONNECTED:
        printf("ORP_WIFI|ESP32 station disconnected from AP. Reasson: ");
        char buffer[50];
        wifi_reason_to_string(event->event_info.disconnected.reason,buffer);
        printf(buffer);
        printf("\n");
        reconnect();
        break;
        
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        printf("ORP_WIFI|the auth mode of AP connected by ESP32 station changed\n");
        break;
        
    case SYSTEM_EVENT_STA_GOT_IP:
        printf("ORP_WIFI|ESP32 station got IP from recently connected AP\n");
        printf("Got an IP: "); 
        printf(IPSTR, IP2STR(&event->event_info.got_ip.ip_info.ip));
        printf("\n");
        char MY_buffer[100];
        const char* result=inet_ntop(AF_INET,&event->event_info.got_ip.ip_info.ip,MY_buffer,sizeof(MY_buffer));
        if (result==0) {
            printf("failed to convert address to string ");
        }
        else{
            strcpy(My_IPString,MY_buffer);
        }
        GOT_IP=pdTRUE;
        // printf("LLamando a INISOCKET\n");
        // iniSocket(My_IPString,MY_PORT_NUMBER);
        // printf("VOLVIENDO de INISOCKET\n");
        break;
        
    case SYSTEM_EVENT_STA_LOST_IP:
        printf("ORP_WIFI|ESP32 station lost IP and the IP is reset to 0\n");
        break;
        
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        printf("ORP_WIFI|ESP32 station wps succeeds in enrollee mode\n");
        break;
        
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        printf("ORP_WIFI|ESP32 station wps fails in enrollee mode\n");
        break;
        
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        printf("ORP_WIFI|ESP32 station wps timeout in enrollee mode\n");
        break;
        
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        printf("ORP_WIFI|ESP32 station wps pin code in enrollee mode\n");
        break;
        
    case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:
        printf("ORP_WIFI|ESP32 station wps overlap in enrollee mode\n");
        break;
        
    case SYSTEM_EVENT_AP_START:
        printf("ORP_WIFI|ESP32 soft-AP start\n");
        break;
        
    case SYSTEM_EVENT_AP_STOP:
        printf("ORP_WIFI|ESP32 soft-AP stop\n");
        break;
        
    case SYSTEM_EVENT_AP_STACONNECTED:
        printf("ORP_WIFI|a station connected to ESP32 soft-AP\n");
        break;
        
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        printf("ORP_WIFI|a station disconnected from ESP32 soft-AP\n");
        break;
        
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        printf("ORP_WIFI|ESP32 soft-AP assign an IP to a connected station\n");
        break;
        
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        printf("ORP_WIFI|Receive probe request packet in soft-AP interface\n");
        break;
        
    case SYSTEM_EVENT_GOT_IP6:
        printf("ORP_WIFI|ESP32 station or ap or ethernet interface v6IP addr is preferred\n");
        break;
        
    case SYSTEM_EVENT_ETH_START:
        printf("ORP_WIFI|ESP32 ethernet start\n");
        break;
        
    case SYSTEM_EVENT_ETH_STOP:
        printf("ORP_WIFI|ESP32 ethernet stop\n");
        break;
        
    case SYSTEM_EVENT_ETH_CONNECTED:
        printf("ORP_WIFI|ESP32 ethernet phy link up\n");
        break;
        
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        printf("ORP_WIFI|ESP32 ethernet phy link down\n");
        break;
        
    case SYSTEM_EVENT_ETH_GOT_IP:
        printf("ORP_WIFI|ESP32 ethernet got IP from connected AP\n");
        break;
        
    case SYSTEM_EVENT_MAX:
        printf("ORP_WIFI|Number of members in this enum\n");
        break;
        
    default:
    printf("Unknow_Event in ORP_WIFI::event_handler()");
        break;
    }
    return ESP_OK;
}


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG_SOCKETTask, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG_SOCKETTask, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}
void wifi_init_softap()
{
    tcpip_adapter_init();
    ESP_LOGI(TAG_SOCKETTask, "tcpip_adapter Inicializado");
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG_SOCKETTask, "wifi AP Inicializado. SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

esp_err_t Ini_Default_WiFi(){

        esp_err_t returnValue;
        int unNumero=100;

        printf("ORP WIFI|nvs_flash_init()\r\n");
        returnValue=nvs_flash_init();
        if(returnValue!=ESP_OK)return returnValue;

        printf("ORP WIFI|tcpip_adapter_init()\r\n");
        tcpip_adapter_init();

        printf("ORP WIFI|esp_event_loop_init()\r\n");
        //struct esp_event_loop_handler_struct_t esp_event_loop_struct;
        //esp_event_loop_struct.hola=218;
        //returnValue=(esp_event_loop_init(event_handler, &esp_event_loop_struct));
        // printf("CALLER esp_event_loop_struct=%d\n",esp_event_loop_struct.hola);
        // printf("CALLER pointer=%p\n", &esp_event_loop_struct);
        returnValue=(esp_event_loop_init(event_handler, &unNumero));
        printf("ORP WIFI|CALLER unNumero=%d\n",unNumero);
        printf("ORP WIFI|CALLER pointer=%p\n", &unNumero);
        if(returnValue!=ESP_OK)return returnValue;

        printf("ORP WIFI|esp_wifi_init()\r\n");
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        returnValue=(esp_wifi_init(&cfg));
        if(returnValue!=ESP_OK)return returnValue;

        printf("ORP WIFI|esp_wifi_set_storage()\r\n");
        returnValue=(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        if(returnValue!=ESP_OK)return returnValue;

        printf("ORP WIFI|esp_wifi_set_mode()\r\n");
        returnValue=(esp_wifi_set_mode(WIFI_MODE_STA));
        if(returnValue!=ESP_OK)return returnValue;
        
        printf("ORP WIFI|Ini_Default_WiFi() ends.\r\n");

        return ESP_OK;
}
// Debe ser inicializada con Ini_Default_Wifi()
esp_err_t Ini_Wifi_As_Station(){
        esp_err_t returnValue;
    wifi_config_t sta_config = {
		.sta = {
			    .ssid = MY_WIFI_NAME,
                .password=MY_PASSSWORD,
                .scan_method=WIFI_FAST_SCAN,    /**< do all channel scan or fast scan */
                
                                                //WIFI_FAST_SCAN = 0,   /**< Do fast scan, scan will end after find SSID match AP */
                                                //WIFI_ALL_CHANNEL_SCAN, /**< All channel scan, scan will end after scan all the channel */
                .bssid_set=0,  
                /**bssid_set < whether set MAC address of target AP or not. Generally, 
                 * station_config.bssid_set needs to be 0; 
                 * and it needs to be 1 only when users need to check the MAC address of the AP.*/
                .bssid={0},                             /**< MAC address of target AP*/
                .channel=0,                             /**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
                .listen_interval=0,                     /**< Listen interval for ESP32 station to receive beacon when WIFI_PS_MAX_MODEM is set. Units: AP beacon intervals. Defaults to 3 if set to 0. */
                .sort_method=WIFI_CONNECT_AP_BY_SIGNAL, /**< sort the connect AP in the list by rssi or security mode */
                                                        //WIFI_CONNECT_AP_BY_SIGNAL = 0,        /**< Sort match AP in scan list by RSSI */
                                                        //WIFI_CONNECT_AP_BY_SECURITY,          /**< Sort match AP in scan list by security mode */
                .threshold.rssi=0,                      /**< When sort_method is set, only APs which have an auth mode that is more secure than the selected auth mode and a signal stronger than the minimum RSSI will be used. */
                                                        //int8_t rssi;/**< The minimum rssi to accept in the fast scan mode */
                .threshold.authmode=WIFI_AUTH_OPEN      /**< The weakest authmode to accept in the fast scan mode */                                        

                                                        //WIFI_AUTH_OPEN = 0,         /**< authenticate mode : open */
                                                        //WIFI_AUTH_WEP,              /**< authenticate mode : WEP */
                                                        //WIFI_AUTH_WPA_PSK,          /**< authenticate mode : WPA_PSK */
                                                        //WIFI_AUTH_WPA2_PSK,         /**< authenticate mode : WPA2_PSK */
                                                        //WIFI_AUTH_WPA_WPA2_PSK,     /**< authenticate mode : WPA_WPA2_PSK */
                                                        //WIFI_AUTH_WPA2_ENTERPRISE,  /**< authenticate mode : WPA2_ENTERPRISE */
                                                        //WIFI_AUTH_MAX
            }
        };

        printf("ORP WIFI|esp_wifi_set_config()\r\n");
	    returnValue=esp_wifi_set_config(WIFI_IF_STA, &sta_config);
        if(returnValue!=ESP_OK)return returnValue;

        printf("ORP WIFI|llamando a esp_wifi_start()\r\n");
        returnValue=esp_wifi_start();
        printf("ORP WIFI|volviendo de esp_wifi_start()\r\n");
        if(returnValue!=ESP_OK)return returnValue;

        return ESP_OK;
}

         /// <summary>
         /// Extrae los datos recibidos de una transmision TCP, elimina la "basura" al principio 
         /// y al final de la transmision
         ///, verifica su fiabilidad y los clasifica en la clase SharedClass
         /// el tamaño máximo para el buffer sería de 65535, puesto que usamos un "unsigned short" como indice
         /// </summary>
         /// <param name="BufferArrayIndex">Indice del Socket usado</param>
         /// <returns>TRUE if succeed.There is new data avaliable in member variable "sharedData"
         ///</returns>
bool GetBinaryDataFromSocketBuffer(struct SharedStruct *receivedSharedObject,unsigned char* BufferReceived,long ReceivedBytes)
{
    bool returnValue = false;
    unsigned short mark_index = 0, buffer_index = 0, sharedDataStart = 0;
    unsigned short i=0,endLoop=0;
    unsigned long checksum = 0;
    struct SharedStruct sharedData={{0x8e,0xb3,0x98,0xd2,0x58,0x8b,0x47,0x2d},0,0,0,0,0};

    printf("ORP_WIFI GetBinaryDataFromSocketBuffer()\n");
    if (ReceivedBytes < sizeof(struct SharedStruct))
        return false;
    while (buffer_index < ReceivedBytes)
    {
        while (BufferReceived[buffer_index] == sharedData.Mark[mark_index])
        {
            mark_index++;
            buffer_index++;
            if (mark_index == (sizeof(sharedData.Mark)))
            {
                //Marca encontrada
                if ((ReceivedBytes - buffer_index) <= (sizeof(struct SharedStruct) - sizeof(sharedData.Mark)))
                {
                    //Hay datos suficientes en el buffer para albergar un objeto SharedData
                    printf("ORP_WIFI extrayendo\n");
                    i = buffer_index;
                    sharedData.checksum = BufferReceived[i++];
                    sharedData.checksum += BufferReceived[i++] << 8;
                    sharedData.checksum += BufferReceived[i++] << 16;
                    sharedData.checksum += BufferReceived[i++] << 24;

                    sharedData.data1 = BufferReceived[i++];
                    sharedData.data2 = BufferReceived[i++];

                    sharedData.data3 = BufferReceived[i++];
                    sharedData.data3 += BufferReceived[i++] << 8;
                    sharedData.data3 += BufferReceived[i++] << 16;
                    sharedData.data3 += BufferReceived[i++] << 24;

                    sharedData.data4 = BufferReceived[i++];
                    sharedData.data4 += BufferReceived[i++] << 8;
                    sharedData.data4 += BufferReceived[i++] << 16;
                    sharedData.data4 += BufferReceived[i++] << 24;
                }
                //Verificar checksum
                endLoop = i;
                for (i = buffer_index + sizeof(sharedData.checksum), checksum = 0; i < endLoop; i++)
                {
                    checksum += BufferReceived[i];
                }
                if (checksum == sharedData.checksum)
                {
                    //los datos son válidos
                    receivedSharedObject->checksum = sharedData.checksum;
                    receivedSharedObject->data1 = sharedData.data1;
                    receivedSharedObject->data2 = sharedData.data2;
                    receivedSharedObject->data3 = sharedData.data3;
                    receivedSharedObject->data4 = sharedData.data4;
                    returnValue = true;
                    //En este punto devería comprobar si hay espacio en la clase para alvergar otro paquete entrante
                    //que pueda estar en la cola del buffer
                    //if(hay espacio en la clase OBDC_Server para más datos entrantes)
                    //buffer_index = i;
                    //else
                    return true;
                }
                else
                {
                    //Si no se supera el checksum, existe la posibilidad de que quede un paquete integro 
                    // en la cola del buffer y que éste empiece justo donde acaba el sharedData.Mark anterior
                    //así que no es necesario actualizar buffer_index (ya está apuntando al final de dicho sharedData.Mark)
                    //, solo restarle 1 para poder continuar con el bucle
                    buffer_index--;
                    break;
                }
            }
            else if (buffer_index >= ReceivedBytes)
            {
                return returnValue;
            }
        }
        mark_index = 0;
        buffer_index++;

    }
    return returnValue;
}
int writeSocket (struct SharedStruct *sendingSharedData,int ClientSocket,unsigned char* BufferSending,int BufferSize)
{    
    //Prepare checksum**********************************
    unsigned long checksum = sendingSharedData->data1+ sendingSharedData->data2;

    unsigned long* pointer = &sendingSharedData->data3;
    unsigned char* myByte = (unsigned char*)pointer;

    checksum += (myByte[0]);
    checksum += (myByte[1]);
    checksum += (myByte[2]);
    checksum += (myByte[3]);

    pointer = &sendingSharedData->data4;
    myByte = (unsigned char*)pointer;

    checksum += (myByte[0]);
    checksum += (myByte[1]);
    checksum += (myByte[2]);
    checksum += (myByte[3]);

    sendingSharedData->checksum = checksum;
    int i = 0;
    //Prepare buffer**********************************
    BufferSending[i] = sendingSharedData->Mark[0];
    BufferSending[++i] = sendingSharedData->Mark[1];
    BufferSending[++i] = sendingSharedData->Mark[2];
    BufferSending[++i] = sendingSharedData->Mark[3];
    BufferSending[++i] = sendingSharedData->Mark[4];
    BufferSending[++i] = sendingSharedData->Mark[5];
    BufferSending[++i] = sendingSharedData->Mark[6];
    BufferSending[++i] = sendingSharedData->Mark[7];


    BufferSending[++i] = checksum & 0xFF;
    BufferSending[++i] = (checksum >> 8) & 0xFF;
    BufferSending[++i] = (checksum >> 16) & 0xFF;
    BufferSending[++i] = (checksum >> 24) & 0xFF;

    BufferSending[++i] = sendingSharedData->data1 ;
    printf("ORP_SOCK|BufferSending[%d]= %d\n",i, BufferSending[i]);
    BufferSending[++i] = sendingSharedData->data2 ;
    printf("ORP_SOCK|BufferSending[%d]= %d\n",i, BufferSending[i]);

    BufferSending[++i] = sendingSharedData->data3 & 0xFF;
    BufferSending[++i] = (sendingSharedData->data3 >> 8) & 0xFF;
    BufferSending[++i] = (sendingSharedData->data3 >> 16) & 0xFF;
    BufferSending[++i] = (sendingSharedData->data3 >> 24) & 0xFF;

    BufferSending[++i] = sendingSharedData->data4 & 0xFF;
    BufferSending[++i] = (sendingSharedData->data4 >> 8) & 0xFF;
    BufferSending[++i] = (sendingSharedData->data4 >> 16) & 0xFF;
    BufferSending[++i] = (sendingSharedData->data4 >> 24) & 0xFF;
 //Send buffer**********************************
    int bytesSend = 0;
        bytesSend = send(ClientSocket, (char*)BufferSending, SHAREDSTRUCT_SIZE, 0);
        if (bytesSend == -1) {
            printf("ORP_SOCK|Error sending Data. errno: %d", errno);
        }
        else{
            
            printf("ORP_SOCK|bytesSend: %d", bytesSend);
        }
    return bytesSend;
}

int readSocket (struct SharedStruct *receivedSharedData,int ClientSocket,unsigned char* BufferReceived,int BufferSize)
{
    bool ret=0;
    int ReceivedBytes=0;
    
    //ReceivedBytes = recv(ClientSocket, BufferReceived, BufferSize, MSG_WAITALL);
    ReceivedBytes = recv(ClientSocket, BufferReceived, BufferSize, 0);
    if (ReceivedBytes==-1) {
        //Error
        switch (errno)
        {
            case EWOULDBLOCK:
                printf("ORP_SOCKET|errno=%d : EWOULDBLOCK. Socket no bloqueable\n",errno);
            break;
            case ENOTCONN:
                printf("ORP_SOCKET|errno=%d : ENOTCONN. Socket is not connected\n",errno);
            break;
            default:
                printf("ORP_SOCKET|errno=%d\n",errno);
            break;
        }
        return ReceivedBytes;
    }
    //Data received
    printf("ORP_SOCKET|Read.ReceivedBytes=%d\n",ReceivedBytes);
    ret=GetBinaryDataFromSocketBuffer(receivedSharedData,BufferReceived,ReceivedBytes);
    if(!ret){
        //No valid data received
    return -1;
    }
    return 0; 
}

// SOCKET task loop
// Inicializa la Wifi
// Inicializa SOCKET non-blocking cuando tiene una IP y lo pone en escucha
// Entra en un bucle esperando un cliente con accept()
// Cuando tiene un cliente entra en un bucle enviando el estado de los switches
// y reciviendo el estado de los leds
void SOCKETTask(void *parms){

    
    struct SOCKETTask_Queues_t* SOCKETTask_Queues=parms;
    QueueHandle_t Main_to_SocketTask_Queue=SOCKETTask_Queues->Main_to_SOCKETTask_Queue;
    QueueHandle_t SocketTask_to_Main_Queue=SOCKETTask_Queues->SOCKETTask_to_Main_Queue;

    struct Main_to_SOCKETTask_struct_t Main_to_SOCKETTask_struct={0};
    struct SOCKETTask_to_Main_struct_t SOCKETTask_to_Main_struct={0};

    tcpip_adapter_if_t ta=TCPIP_ADAPTER_IF_STA ;
    tcpip_adapter_ip_info_t ip_info_sta={0};
    tcpip_adapter_ip_info_t ip_info_ap={0};
    char MY_buffer[25]={0};
    int ha_fallado=1;
    int unNumero=10;
    esp_err_t returnValue;

    //************************** SEND RECIEVE ********************************************

    static int const            DATA_BUFSIZE = 100;
    unsigned char               BufferInputOutput[101];
    unsigned short              ReceivedBytes = { 0 };
    struct SharedStruct         sharedDataRecv={{0x8e,0xb3,0x98,0xd2,0x58,0x8b,0x47,0x2d},0,0,0,0,0};
    struct SharedStruct         sharedDataSend={{0x8e,0xb3,0x98,0xd2,0x58,0x8b,0x47,0x2d},0,0,0,0,0};
    int                         ServerSocket;
    int                         ClientSocket;
    //*****************************************************************************************
    //********************************** WIFI *************************************************
    //*****************************************************************************************
    
    //********************************Unrecoverable Inizialization Part ********************
        printf("%s Punto de entrada de SOCKETTask\n",TAG_SOCKETTask);
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

        printf("%s nvs_flash Inicializada\n",TAG_SOCKETTask);
    
        printf("%s esp_event_loop_init()\n",TAG_SOCKETTask);
        returnValue=(esp_event_loop_init(event_handler, &unNumero));
        printf("%s  unNumero=%d\n",TAG_SOCKETTask,unNumero);
        printf("%s &unNumero=%p\n",TAG_SOCKETTask, &unNumero);
        if(returnValue!=ESP_OK)
        {
            printf("%s esp_event_loop_init() FAILED\n",TAG_SOCKETTask);
        }
        else
        {
            printf("%s esp_event_loop_init() SUCCEED\n",TAG_SOCKETTask);
        }
        

    wifi_init_softap();
    char miniBuffer[25]={0};
    iniSocket(miniBuffer,27015); 

    //STA
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA,&ip_info_sta);
        inet_ntop(AF_INET,&ip_info_sta.ip,MY_buffer,sizeof(MY_buffer));
        printf("%s tcpip_adapter_get_ip(STA)_info ha devuelto IP:%s\n",TAG_SOCKETTask,MY_buffer);
        printf ("%s  y además hemos vuelto\n",TAG_SOCKETTask);
    //STA
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP,&ip_info_ap);
        inet_ntop(AF_INET,&ip_info_ap.ip,MY_buffer,sizeof(MY_buffer));
        printf("%s tcpip_adapter_get_ip(AP)_info ha devuelto IP:%s\n",TAG_SOCKETTask,MY_buffer);
        printf ("%s  y además hemos vuelto\n",TAG_SOCKETTask);
    //***************************************************************************************
    //********************************Unrecoverable Inizialization Part ********************
     do{
        do{

        printf("%s AP_ Inicialization Loop Start\n",TAG_SOCKETTask);

        printf("%s AP_ Inicialization Loop End\n",TAG_SOCKETTask);
        }while(AP_funcionando);
     }while(autoRecovery_Infinite_Loop);
     do{

     }while(true);
    //***************************************************************************************
   do
    {
        returnValue=Ini_Default_WiFi();
        if(returnValue!=ESP_OK){
            char buffer[50];
            printf("ORP WIFI|Ini_Default_WiFi() ha devuelto esp_err_t=%d - ",returnValue);
            printf(esp_err_to_name_r (returnValue,buffer,50));
            printf("\n");
        }
        else {
            ha_fallado=0;
            vTaskDelay(2*portTICK_PERIOD_MS);
        }
    }while (ha_fallado);

    ha_fallado=1;
    do{
        returnValue=Ini_Wifi_As_Station();
        if(returnValue!=ESP_OK){
            char buffer[50];
            printf("ORP WIFI|Ini_Wifi_As_Station() ha devuelto esp_err_t=%d - ",returnValue);
            printf(esp_err_to_name_r (returnValue,buffer,50));
            printf("\n");
        }
        else {
            ha_fallado=0;
            vTaskDelay(2*portTICK_PERIOD_MS);
        }
    }while (ha_fallado);


    int loop=1;
    do{
    if(GOT_IP){
        printf ("ORP WIFI| Ahora si hay IP\n");
        tcpip_adapter_get_ip_info(ta,&ip_info_sta);
        inet_ntop(AF_INET,&ip_info_sta.ip,MY_buffer,sizeof(MY_buffer));
        printf("ORP WIFI|tcpip_adapter_get_ip_info ha devuelto IP:%s\n",MY_buffer);
        printf ("ORP WIFI| y además hemos vuelto\n");
        loop=0;
    }
    else{
        printf ("ORP WIFI| ups... no hay IP todavía\n");
        vTaskDelay(portTICK_PERIOD_MS*5);
    }
    }while (loop);
    
    //*****************************************************************************************
    //********************************** SOCKET ***********************************************
    //*****************************************************************************************
    ha_fallado=1;
    ServerSocket=iniSocket(MY_buffer,MY_PORT_NUMBER);
    /* Block for 1000ms. */
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    do{
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        ClientSocket = accept(ServerSocket, (struct sockaddr *)&clientAddress,
        &clientAddressLength);
        if (ClientSocket==-1){
            switch(errno)
            {
                case EBADF:
                printf("ORP_WIFI|accept() error-\"ServerSocket\" is not a valid descriptor.\n");
                closesocket(ServerSocket);
                ServerSocket=iniSocket(MY_buffer,MY_PORT_NUMBER);
                break;
                case EWOULDBLOCK :
                printf("ORP_WIFI|Waiting for Client on IP:%s PORT:%d\n",MY_buffer,MY_PORT_NUMBER);
                break;
                default:
                printf("ORP_WIFI|accept() ha devuelto un numero negativo\n");
                closesocket(ServerSocket);
                ServerSocket=iniSocket(MY_buffer,MY_PORT_NUMBER);
            }
            }
        else{
            
                //Set ServerSocket as non-Blocking
                u_long iMode = 1;
                ioctlsocket(ClientSocket, FIONBIO, &iMode);
                char MY_buffer[100];
                const char* result=inet_ntop(AF_INET,&clientAddress,MY_buffer,sizeof(MY_buffer));
                if (result==0) {
                    printf("ORP_WIFI|Conexion aceptada. IP desconocida.\n");
                }
                else{
                    printf("ORP_WIFI|Aceptada conexion de cliente:%s\n",MY_buffer);
                }
                ha_fallado=0;
            }
            vTaskDelay (xDelay);
    }while (ha_fallado);
    //*****************************************************************************************
    //********************************** RUTINA PRINCIPAL *************************************
    //*****************************************************************************************
    char buffer[100];
    while(1){

        //recive el valor de los switches, si la cola está vacía, vuelve inmediatamente
             BaseType_t returnValue=xQueueReceive(
                                   Main_to_SocketTask_Queue,
                                   &Main_to_SOCKETTask_struct,
                                   0  
                               );
        if(returnValue==pdTRUE){
            printf("ORP_WIFI| Valor recibido en Socket <- Main%d:\n",Main_to_SOCKETTask_struct.SwitchPin);
        }
        sharedDataSend.data1=Main_to_SOCKETTask_struct.SwitchPin&0xF;
        writeSocket(&sharedDataSend,ClientSocket,BufferInputOutput,DATA_BUFSIZE);
        readSocket(&sharedDataRecv,ClientSocket,BufferInputOutput,DATA_BUFSIZE);
        printf("ORP_WIFI|ServerSocket=%d\n",ServerSocket);
        printf("ORP_WIFI|readSocket()*********************************************\n");
        printf("ORP_WIFI|readSocket() sharedDataRecv.data1=%d\n",sharedDataRecv.data1);
        printf("ORP_WIFI|readSocket() sharedDataRecv.data2=%d\n",sharedDataRecv.data2);
        printf("ORP_WIFI|readSocket() sharedDataRecv.data3=%ld\n",sharedDataRecv.data3);
        printf("ORP_WIFI|readSocket() sharedDataRecv.data4=%ld\n",sharedDataRecv.data4);
        SOCKETTask_to_Main_struct.LedPin=sharedDataRecv.data1&0xF;
            //Envía el valor de los leds, si el buffer está lleno, vuelve inmediatamente
             returnValue=xQueueSend(
                                   SocketTask_to_Main_Queue,
                                   &SOCKETTask_to_Main_struct,
                                   0  
                               ); if(returnValue==pdTRUE){
            printf("ORP_WIFI| Valor enviado de Socket -> Main%d:\n",Main_to_SOCKETTask_struct.SwitchPin);
        }
        
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
#endif //SOCKETTask_h