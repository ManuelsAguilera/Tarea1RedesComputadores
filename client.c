#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//typedefs
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;




//wrapper para errores
void error(char *msg){
    perror(msg);
    exit(1);
}

void sendMsg(int sockFd, char* buff) {
	
    if (send(sockFd,buff,strlen(buff),0) == -1)
        error("No se enviaron datos");

}


int getSock() //Obtiene un file descriptor para el socket
{
	int sockFd;
	
	if ((sockFd = socket(AF_INET,SOCK_STREAM,0)) == -1)
		error("Hubo un error, socket FD no se pudo generar");
	return sockFd;
}

void getSockAddr(const char* ip_addr, int port, sockaddr_in* addr_in) {
    addr_in->sin_family = AF_INET; // IPv4
    addr_in->sin_port = htons(port); // Puerto
    
    if (inet_pton(AF_INET, ip_addr, &addr_in->sin_addr) <= 0) { // Presentar dirección en formato binario
        error("inet_pton error");
    }
}

int getPortFromArg(const char* argv[]) {
    return atoi(argv[1]);
}


void getMessageFromArg(char* buff, const char* argv[]) {
    strcpy(buff, argv[2]);
}


int main(int argc, const char* argv[]) {

    if (argc != 3) {
        error("Uso: ./client <port> <message>");
    }

    int port = getPortFromArg(argv);

    char buff[256];
    getMessageFromArg(buff, argv);

    int sockFd = getSock();
    sockaddr_in addr;
    getSockAddr("127.0.0.1", port, &addr);
	


	if ((connect(sockFd,(sockaddr*) &addr,sizeof(addr))) == -1)
		error("Hubo un error, conexion cancelada");
	

	sendMsg(sockFd, buff);
	
	

	close(sockFd);
	return  0;
}
