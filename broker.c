#include "utils.h"


int getSockStream4() //Obtiene socket para comunicacion con cliente
{
    return getSock(AF_INET, SOCK_STREAM);
}

int getSockDgram6() //Obtiene un socket para comunicacion con server
{
    return getSock(AF_INET6, SOCK_DGRAM);
}


int getClientPortFromArg(const char* argv[]) {
    //primer arg es cliente
    return getIntFromArg(argv, 1);
}

int getServerPortFromArg(const char* argv[]) {
    //segundo arg es server
    return getIntFromArg(argv, 2);
}

void recvFromClient(int clientFd, char* buff);

void recibeMsgSubroutine(int clientPort,char* buff);


int main(int argc, const char* argv[])
{
	
    if (argc != 3) {
        error("Uso: ./server <listen port> <send port>");
    }

    int clientPort = getClientPortFromArg(argv);
    int serverPort = getServerPortFromArg(argv);

    char buff[256] = {0}; //donde almacenar el mensaje

    //Recibir mensaje del cliente, y mostrar ip, puerto
    recibeMsgSubroutine(clientPort,buff);

    
    // Enviar mensaje al servidor UDP en IPv6
    int sockFd = getSockDgram6();
    sockaddr_in6 addr6;

    getSockAddrIPv6("::1", serverPort, &addr6);

    
    if (sendto(sockFd, buff, strlen(buff), 0, (sockaddr*)&addr6, sizeof(addr6)) == -1) {
        error("Error al enviar mensaje");
    }

    // Cerrar el socket
    close(sockFd);

    return 0;
}

void recvFromClient(int clientFd, char* buff)
{
    //Guardamos bytes recibdos
    ssize_t bytes_r = recv(clientFd, buff, BUFFER_SIZE, 0);
    if (bytes_r == -1)
        error("Error al recibir mensaje");
    else if (bytes_r == 0)
    {
        printf("Se ha cerrado la conexion\n");
        exit(EXIT_FAILURE);
    }
}

void recibeMsgSubroutine(int clientPort,char* buff)
{
    int sockFd = getSockStream4();
    
    // addr es nuestra direccion, clientAddr es para obtener la de el cliente
    sockaddr_in addr,clientAddr;
    int clientFd; //Aqui guardaremos el file descriptor del cliente

    getSockAddrIPv4("127.0.0.1", clientPort, &addr);
	socklen_t len_addr = sizeof(addr);
    socklen_t len_clientAddr = sizeof(clientAddr);


    //iniciar server
    

    bind(sockFd,(struct sockaddr * ) &addr, len_addr);

    
    listen(sockFd,10);
    
    printf("Escuchando dispositivos\n");

    //Aceptamos conexion, y obtenemos info del cliente que se conecto
    //Gracias a que enviamos el puntero de clientAddr
    clientFd = accept(sockFd,(struct sockaddr * ) &clientAddr, &len_clientAddr);
    printf("Se acaba de encontrar un dispositivo\n");

    if (clientFd == -1)
        error("Se recibio un mensaje error");

    
    //Recibir mensaje    
    recvFromClient(clientFd, buff);


    //Obtener ip y puerto para imprimir
    char* clientAddrText = inet_ntoa(clientAddr.sin_addr);
    
    //No tengo manera legitima de encontrar el puerto asi q hago trampa >:V
    printf("Client Ip %s , Client port %d\n",clientAddrText,clientPort);
    

    close(clientFd);
    close(sockFd);
    
}