#include "stdio.h"

#include "string.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#include <sys/types.h> 
#include <sys/socket.h>

#define MY_WIFI_NAME "RODRIGUEZ_4"

int espx_last_socket_errno(int socket) {
int ret = 0;
u32_t optlen = sizeof(ret);
getsockopt(socket, SOL_SOCKET, SO_ERROR, &ret, &optlen);
return ret;
}
char My_IPString[20];
void saveIPString (char* IPString){
        strcpy(My_IPString,IPString);
}
void iniSocket (){
        int ret;
        int portNumber=27015;
        //char buffer_char[]="hola mundo desde esp32\n";
        char buffer_char[]="HTTP/1.1 200 OK\nContent-Type: text/html\n\r\n<html>\n<body>\n<h1>!ESP32 esta en la red!</h1>\n</body></html>\n\n";

        printf("ORP_WIFI|Dentro de iniSocket()\n");
        
        printf("ORP_WIFI|socket()\n");
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock<0){printf("ORP_WIFI|socket() ha devuelto un numero negativo\n");}
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(portNumber);
        
        printf("ORP_WIFI|bind()\n");
        ret=bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (ret<0){printf("ORP_WIFI|bind() ha devuelto un numero negativo\n");}

        printf("ORP_WIFI|listen()\n");
        ret=listen(sock, 0);
        if (ret<0){printf("ORP_WIFI|listen() ha devuelto un numero negativo\n");}

        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        printf("**** Esperando conexion de cliente.  ****\n");
        printf("**** Conectate con un browser como el exploreer, crome, safari, etc  ****\n");
        printf("**** que tenga  acceso a la WIFI:  ");printf(MY_WIFI_NAME);printf("****\n");
        printf("**** y introduce la direccion: ");printf(My_IPString);;printf(":%d****\n",portNumber);
        printf("**** para ver una espectacular pagina web ****\n");
        
        printf("ORP_WIFI|accept()\n");

        int clientSock = accept(sock, (struct sockaddr *)&clientAddress,
        &clientAddressLength);
        if (clientSock<0){
                printf("ORP_WIFI|accept() ha devuelto un numero negativo\n");
                closesocket(sock);
                return ;
            }
        else{
                char MY_buffer[100];
                const char* result=inet_ntop(AF_INET,&clientAddress,MY_buffer,sizeof(MY_buffer));
                if (result==0) {
                    printf("ORP_WIFI|Conexion aceptada. IP desconocida.\n");
                }
                else{
                    printf("ORP_WIFI|Aceptada conexion de cliente:%s\n",MY_buffer);
                }
            }
        printf("ORP_WIFI|send()\n");
        ret=send(clientSock, (void *)buffer_char, sizeof(buffer_char), 0);
        if (clientSock<0){printf("ORP_WIFI|send() ha devuelto un numero negativo:%d\n",ret);}
        else{printf("ORP_WIFI|send() parece haber funcionado:%d bytes enviados\n",ret);}
        // ret= close(clientSock);
        // if (ret<0){printf("ORP_WIFI|close() ha devuelto un numero negativo\n");}
        // else{printf("ORP_WIFI|close() parece haber funcionado\n");}
}

void ORP_WIFI_ini_STATION(system_event_cb_t event_handler,char * SSID,char * PASSWORD)
{
        nvs_flash_init();
        printf("ORP_WIFI_1\r\n");
        tcpip_adapter_init();
        printf("ORP_WIFI_2\r\n");
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
        printf("ORP_WIFI_3\r\n");
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        printf("ORP_WIFI_4\r\n");
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        printf("ORP_WIFI_5\r\n");
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        printf("ORP_WIFI_6\r\n");
/*
        wifi_config_t sta_config1 = {0};
        
        sta_config1.sta.ssid="RODRIGUEZ_4\n";
        sta_config1.sta.password="PITUFILLA\n";
        sta_config1.sta.bssid_set=0;
*/

        
        //wifi_config_t sta_config={0};
        wifi_config_t sta_config = {
		.sta = {
			.ssid = MY_WIFI_NAME,
                        .password="PITUFILLA",
                        // .ssid="HUAWEI Y5 2019\n",
                        // .password="mayusculas\n",
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
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
        printf("ORP_WIFI_7\r\n");
        ESP_ERROR_CHECK(esp_wifi_start());
        printf("ORP_WIFI_8\r\n");
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
void ORP_WIFI_ini_AP(system_event_cb_t event_handler)
{
        nvs_flash_init();
        tcpip_adapter_init();
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

        //wifi_config_t ap_config={0};
        wifi_config_t  ap_config = {
		.ap = {
			.ssid = "ESP32",                  /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
			.password = "PITUFILLA",                /**< Password of ESP32 soft-AP. Null terminated string. */
			.ssid_len = 0,                          /**< Optional length of SSID field. */
			.channel = 1,                           /**< Channel of ESP32 soft-AP */
			.authmode = WIFI_AUTH_WPA2_PSK,         /**< Auth mode of ESP32 soft-AP. Do not support AUTH_WEP in soft-AP mode */
			.ssid_hidden = 0,                       /**< Broadcast SSID or not, default 0, broadcast the SSID */
			.max_connection = 4,                    /**< Max number of stations allowed to connect in, default 4, max 4 */
			.beacon_interval = 60000                /**< Beacon interval, 100 ~ 60000 ms, default 100 ms */
		}
	};
    	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

        ESP_ERROR_CHECK(esp_wifi_start());
}
