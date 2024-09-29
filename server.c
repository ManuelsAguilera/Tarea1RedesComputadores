#include "utils.h"

int getSockServer() //Obtiene un file descriptor para el socket
{
    return getSock(AF_INET6,SOCK_DGRAM);
}

int getPortFromArg(const char* argv[]) {

    return getIntFromArg(argv, 1);
}


int main(int argc,const char *argv[]) {
    // Comprobar el número de argumentos
    if (argc != 2) 
        error("Uso: ./server <listen port>");
    


    sockaddr_in6 addr;
    int port = getPortFromArg(argv);

    int sockFd = getSockServer();
    
    getSockAddrIPv6("::1", port, &addr);
    socklen_t addr_len = sizeof(addr);

    //Inicializar buffer vacio, para evitar basura
    char buff[BUFFER_SIZE] = {0};

    //Enlazar address, para actuar de server
    bind(sockFd,(sockaddr *)&addr,addr_len);
    
    printf("Waiting for broker...\n");

    if (recvfrom(sockFd, buff, BUFFER_SIZE, 0, (sockaddr*)&addr, &addr_len) == -1)
        error("Error al recibir mensaje");

    printf("Mensaje recibido: %s\n", buff);
    return 0;
}