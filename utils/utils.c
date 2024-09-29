#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//typedefs
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;



//wrapper para errores
void error(char *msg){
    perror(msg);
    exit(1);
}


int isStringDigit(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int getIntFromArg(const char* argv[],int index)
{
	//Corroborar que el string sea un numero
	if (!isStringDigit(argv[index])) {
		error("El puerto del cliente debe ser un numero");
	}
	return atoi(argv[index]);
}



int getSock(int domain,int type)
{
    
	int sockFd;
	
	if ((sockFd = socket(domain, type,0)) == -1)
		error("Hubo un error, socket FD no se pudo generar");
	return sockFd;
}



void getSockAddrIPv6(const char* ip_addr, int port, struct sockaddr_in6* addr_in6) {
    addr_in6->sin6_family = AF_INET6; //IPv6
    
    
    addr_in6->sin6_port = htons(port); //Puerto
    addr_in6->sin6_addr = in6addr_any; //recieve ipv6
    
    if (inet_pton(AF_INET6, ip_addr, &addr_in6->sin6_addr) <= 0) { //Presentar direccion en formato binario
        error("inet_pton error");
    }
}

void getSockAddrIPv4(const char* ip_addr, int port, sockaddr_in* addr_in) {
	addr_in->sin_family = AF_INET; // IPv4
	addr_in->sin_port = htons(port); // Puerto
	
	if (inet_pton(AF_INET, ip_addr, &addr_in->sin_addr) <= 0) { // Presentar dirección en formato binario
		error("inet_pton error");
	}
}

