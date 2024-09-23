#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//Boilerplate
int main(int argc, char *argv[]) {
    // Comprobar el número de argumentos
    if (argc > 1) {
        printf("Hola %s!\n", argv[1]); // Imprimir el primer argumento
    } else {
        printf("Hola mundo!\n"); // Mensaje por defecto
    }
    
    return 0;
}