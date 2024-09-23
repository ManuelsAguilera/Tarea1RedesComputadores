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

void getSockAddr(const char* ip_addr, int port, sockaddr_in* addr_in) {
    addr_in->sin_family = AF_INET; // IPv4
    addr_in->sin_port = htons(port); // Puerto
    
    if (inet_pton(AF_INET, ip_addr, &addr_in->sin_addr) <= 0) { // Presentar dirección en formato binario
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



int main(int argc, const char* argv[])
{
	
	int sockFd = getSock();
    sockaddr_in addr;
    

    getSockAddr("127.0.0.1", 1024, &addr);
	socklen_t len_addr = sizeof(addr);



    int clientFd;

    bind(sockFd,(struct sockaddr * ) &addr, len_addr);

    
    listen(sockFd,10);
    
    printf("Escuchando dispositivos\n");

    clientFd = accept(sockFd,(struct sockaddr * ) &addr,&len_addr);
    printf("Se acaba de encontrar un dispositivo\n");

    if (clientFd == -1)
        error("Se recibio un mensaje error");

    
    char buff[256];
    while (strcmp(buff,"stop\n") != 0)
    {
        memset(buff,0,sizeof(buff));

        ssize_t bytes_r = recv(clientFd,buff,sizeof(buff),0);
        if (bytes_r == -1)
            error("Error al recibir mensaje");
        else if (bytes_r == 0)
        {
            printf("Se ha cerrado la conexion\n");
            break;
        }
            
        
        printf("%s",buff);
    }
    
    
    close(sockFd);
    close(clientFd);

    return 0;
}
