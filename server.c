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
typedef struct sockaddr_in6 sockaddr_in6;




//wrapper para errores
void error(char *msg){
    perror(msg);
    exit(1);
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
    
    if ((sockFd = socket(AF_INET6,SOCK_DGRAM,0)) == -1)
        error("Hubo un error, socket FD no se pudo generar");
    return sockFd;
}


int isStringDigit(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}


int getPortFromArg(const char* argv[]) {
    //Corroborar que el string sea un numero
    
    if (!isStringDigit(argv[1])) {
        error("El puerto del cliente debe ser un numero");
    }
    return atoi(argv[1]);
}


//Boilerplate
int main(int argc,const char *argv[]) {
    // Comprobar el número de argumentos
    if (argc != 2) 
        error("Uso: ./server <listen port>");
    


    sockaddr_in6 addr;
    int port = getPortFromArg(argv);

    int sockFd = getSock();
    
    getSockAddrIPv6("::1", port, &addr);
    socklen_t addr_len = sizeof(addr);

    //Making udp server
    char buff[256];

    bind(sockFd,(sockaddr *)&addr,addr_len);
    
    printf("Waiting for broker...\n");
    
    if (recvfrom(sockFd, buff, sizeof(buff), 0, (sockaddr*)&addr, &addr_len) == -1)
        error("Error al recibir mensaje");

    printf("Mensaje recibido: %s\n", buff);
    return 0;
}