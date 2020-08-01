
#include "esp_wifi.h"
#include "esp_system.h"

#include "nvs_flash.h"
//#include <sys/types.h> 
//#include <sys/socket.h>
#include <lwip/sockets.h>

#define MY_WIFI_NAME "RODRIGUEZ_4"
#define MY_PASSSWORD "PITUFILLA"
// #define MY_WIFI_NAME "HUAWEI Y5 2019\n",
// #define MY_PASSSWORD "mayusculas\n",
#define MY_PORT_NUMBER 27015

int GOT_IP=pdFALSE;

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
        ret=listen(ServerSocket, 0);
        if (ret<0){
            printf("ORP_WIFI|listen() ha devuelto un numero negativo\n");
            closesocket(ServerSocket);
            return ret;
            }
        return ServerSocket;
        
}

void app_main() {
    //********************************** WIFI *************************************************
    tcpip_adapter_if_t ta=TCPIP_ADAPTER_IF_STA ;
    tcpip_adapter_ip_info_t ip_info={0};
    char MY_buffer[25];
    int ha_fallado=1;
    esp_err_t returnValue;
   //********************************** SOCKET *************************************************
    int                         ServerSocket;
   
    //*****************************************************************************************
    //********************************** WIFI *************************************************
    //*****************************************************************************************
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
        tcpip_adapter_get_ip_info(ta,&ip_info);
        inet_ntop(AF_INET,&ip_info.ip,MY_buffer,sizeof(MY_buffer));
        printf("ORP WIFI|tcpip_adapter_get_ip_info ha devuelto IP:%s\n",MY_buffer);
        printf ("ORP WIFI| y además hemos vuelto\n");
        vTaskDelay(portTICK_PERIOD_MS*10);
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
        int ClientSocket = accept(ServerSocket, (struct sockaddr *)&clientAddress,
        &clientAddressLength);
        if (ClientSocket==-1){
            switch(errno)
            {
                case EBADF:
                printf("ORP_SOCK|accept() error-\"ServerSocket\" is not a valid descriptor.\n");
                closesocket(ServerSocket);
                ServerSocket=iniSocket(MY_buffer,MY_PORT_NUMBER);
                break;
                case EWOULDBLOCK :
                printf("ORP_SOCK|Waiting for Client on IP:%s PORT:%d\n",MY_buffer,MY_PORT_NUMBER);
                break;
                default:
                printf("ORP_SOCK|accept() ha devuelto un numero negativo\n");
                closesocket(ServerSocket);
            }
            }
        else{
                char MY_buffer[100];
                const char* result=inet_ntop(AF_INET,&clientAddress,MY_buffer,sizeof(MY_buffer));
                if (result==0) {
                    printf("ORP_SOCK|Conexion aceptada. IP desconocida.\n");
                }
                else{
                    printf("ORP_SOCK|Aceptada conexion de cliente:%s\n",MY_buffer);
                }
                ha_fallado=0;
            }
            vTaskDelay (xDelay);
    }while (ha_fallado);
    //*****************************************************************************************
    //********************************** RUTINA PRINCIPAL *************************************
    //*****************************************************************************************
    ha_fallado=0;
    char buffer[100];
    while(!ha_fallado){
        printf("ORP_MAIN|ServerSocket=%d\n",ServerSocket);
        recv(ServerSocket,buffer,100,0);
         vTaskDelay(1000/portTICK_PERIOD_MS);
    }

}