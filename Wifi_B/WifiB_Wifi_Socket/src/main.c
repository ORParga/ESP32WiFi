#include <ORP_WIFI.h>

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
esp_err_t event_handler(void *ctx, system_event_t *event){
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
            printf("failed to convert address to string (errno=%d)",errno);
        }
        else{
            saveIPString(MY_buffer);
        }
        iniSocket();
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

void app_main() {
    //ORP_WIFI_ini_STATION(event_handler,"RODRIGUEZ_4\n","PITUFILLA\n");
    ORP_WIFI_ini_AP(event_handler);

}