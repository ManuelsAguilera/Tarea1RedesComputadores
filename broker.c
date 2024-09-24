#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>



//typedefs
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;


//wrapper para errores
void error(char *msg){
    perror(msg);
    exit(1);
}

void getSockAddr(const char* ip_addr, int port, sockaddr_in* addr_in) {
    addr_in->sin_family = AF_INET; // IPv4
    addr_in->sin_port = htons(port); // Puerto
    
    if (inet_pton(AF_INET, ip_addr, &addr_in->sin_addr) <= 0) { // Presentar dirección en formato binario
        error("inet_pton error");
    }
}

void getSockAddrIPv6(const char* ip_addr, int port, struct sockaddr_in6* addr_in6) {
    addr_in6->sin6_family = AF_INET6; //IPv6
    
    
    addr_in6->sin6_port = htons(port); //Puerto
    addr_in6->sin6_addr = in6addr_any; //recieve ipv6
    
    if (inet_pton(AF_INET6, ip_addr, &addr_in6->sin6_addr) <= 0) { //Presentar direccion en formato binario
        error("inet_pton error");
    }
}

int getSock() //Obtiene un file descriptor para el socket
{
    int sockFd;
    
    if ((sockFd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        error("Hubo un error, socket FD no se pudo generar");
    return sockFd;
}

int getSock6() //Obtiene un file descriptor para el socket
{
    int sockFd;
    
    if ((sockFd = socket(AF_INET6,SOCK_DGRAM,0)) == -1)
        error("Hubo un error, socket FD no se pudo generar");
    return sockFd;
}


void recvFromClient(int clientFd, char* buff) {
    ssize_t bytes_r = recv(clientFd, buff, sizeof(buff), 0);
    if (bytes_r == -1)
        error("Error al recibir mensaje");
    else if (bytes_r == 0)
    {
        printf("Se ha cerrado la conexion\n");
        exit(EXIT_FAILURE);
    }
}


int isStringDigit(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int getClientPortFromArg(const char* argv[]) {
    //Corroborar que el string sea un numero
    
    if (!isStringDigit(argv[1])) {
        error("El puerto del cliente debe ser un numero");
    }
    return atoi(argv[1]);
}

int getServerPortFromArg(const char* argv[]) {
    //Corroborar que el string sea un numero
    
    if (!isStringDigit(argv[1])) {
        error("El puerto del cliente debe ser un numero");
    }

    return atoi(argv[2]);
}


void recibeMsgSubroutine(int clientPort,char* buff)
{
    int sockFd = getSock();
    
    sockaddr_in addr,clientAddr;

    getSockAddr("127.0.0.1", clientPort, &addr);
	socklen_t len_addr = sizeof(addr);
    socklen_t len_clientAddr = sizeof(clientAddr);


    //iniciar server
    int clientFd;

    bind(sockFd,(struct sockaddr * ) &addr, len_addr);

    
    listen(sockFd,10);
    
    printf("Escuchando dispositivos\n");

    clientFd = accept(sockFd,(struct sockaddr * ) &clientAddr, &len_clientAddr);
    printf("Se acaba de encontrar un dispositivo\n");

    if (clientFd == -1)
        error("Se recibio un mensaje error");

    
    //Recibir mensaje    
    recvFromClient(clientFd, buff);

    //Obtener ip y puerto para imprimir
    
    char* clientAddrText = inet_ntoa(clientAddr.sin_addr);
    
    printf("Client Ip %s , Client port %d\n",clientAddrText,clientPort);
    

    close(clientFd);
    close(sockFd);
    
}


int main(int argc, const char* argv[])
{
	
    if (argc != 3) {
        error("Uso: ./server <listen port> <send port>");
    }

    int clientPort = getClientPortFromArg(argv);
    int serverPort = getServerPortFromArg(argv);

    char buff[256]; //donde almacenar el mensaje

    recibeMsgSubroutine(clientPort,buff);

    
    // Enviar mensaje al servidor UDP en IPv6
    int sockFd = getSock6();
    sockaddr_in6 addr6;

    getSockAddrIPv6("::1", serverPort, &addr6);

    printf("sending: %s\n",buff);
    if (sendto(sockFd, buff, strlen(buff), 0, (sockaddr*)&addr6, sizeof(addr6)) == -1) {
        error("Error al enviar mensaje");
    }

    // Cerrar el socket
    close(sockFd);

    return 0;
}
