// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//Para poder leer consistenemente el buffer sin pasarse, o perder info
#define BUFFER_SIZE 256



// Estructuras comunes
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;

// Es el wrapper de errores
void error(char *msg);

//Para evitar errores edge cases
int isStringDigit(const char* str);

//Para leer puertos
int getIntFromArg(const char* argv[],int index); 

//Obtiene un file descriptor para el socket
int getSock(int domain, int type); 

//Inicializa la sockaddr_in6 con la dirección y puerto usando udp e ipv6
void getSockAddrIPv6(const char* ip_addr, int port, struct sockaddr_in6* addr_in6);

//Inicializa la sockaddr_in con la dirección y puerto usando udp e ipv4
void getSockAddrIPv4(const char* ip_addr, int port, struct sockaddr_in* addr_in);


#endif // UTILS_H
