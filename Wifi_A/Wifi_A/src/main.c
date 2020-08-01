//WIFI_A SOCKET KOLVAN
#include <lwip/sockets.h>
#include <esp_log.h>
#include <string.h>
#include <errno.h>
#include "sdkconfig.h"

#define PORT_NUMBER 8001

static char tag[] = "socket_server";

static void sendData(int socket){
    char text[80];
    int i;
    for (i=1; i<10;i++){
        sprintf(text, "Messase %d",i);
        send(socket, text, strlen(text),0);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    close (socket);
}


/**
 * Create a listening socket.  We then wait for a client to connect.
 * Once a client has connected, we then read until there is no more data
 * and log the data read.  We then close the client socket and start
 * waiting for a new connection.
 */
void socket_server_task(void *ignore) {
	struct sockaddr_in clientAddress;
	struct sockaddr_in serverAddress;

	// Create a socket that we will listen upon.
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		ESP_LOGE(tag, "socket: %d %s", sock, strerror(errno));
		goto END;
	}

	// Bind our server socket to a port.
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT_NUMBER);
	int rc  = bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (rc < 0) {
		ESP_LOGE(tag, "bind: %d %s", rc, strerror(errno));
		goto END;
	}

	// Flag the socket as listening for new connections.
	rc = listen(sock, 5);
	if (rc < 0) {
		ESP_LOGE(tag, "listen: %d %s", rc, strerror(errno));
		goto END;
	}

	while (1) {
		// Listen for a new client connection.
		socklen_t clientAddressLength = sizeof(clientAddress);
		int clientSock = accept(sock, (struct sockaddr *)&clientAddress, &clientAddressLength);
		if (clientSock < 0) {
			ESP_LOGE(tag, "accept: %d %s", clientSock, strerror(errno));
			goto END;
		}

		// We now have a new client ...
	}
	END:
	vTaskDelete(NULL);
}
void read_data(int clientSock){

		// We now have a new client ...
		int total =	10*1024;
		int sizeUsed = 0;
		char *data = malloc(total);

		// Loop reading data.
		while(1) {
			ssize_t sizeRead = recv(clientSock, data + sizeUsed, total-sizeUsed, 0);
			if (sizeRead < 0) {
				ESP_LOGE(tag, "recv: %d %s", sizeRead, strerror(errno));
				return;
			}
			if (sizeRead == 0) {
				break;
			}
			sizeUsed += sizeRead;
		}

		// Finished reading data.
		ESP_LOGD(tag, "Data read (size: %d) was: %.*s", sizeUsed, sizeUsed, data);
		free(data);
		close(clientSock);
}

void app_main() {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}