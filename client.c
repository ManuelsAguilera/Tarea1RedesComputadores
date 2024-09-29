#include "utils/utils.h"



//Envia mensaje utilizando tcp
void sendMsg(int sockFd, char* buff) { 
    int len = strlen(buff);  // Obtener el tamaño real del mensaje
    if (send(sockFd, buff, len, 0) == -1)
        error("No se enviaron datos");
}


int getSockClient() //wrapper de socket ipv4
{
	return getSock(AF_INET, SOCK_STREAM);
}



int getPortFromArg(const char* argv[]) {
    return getIntFromArg(argv, 1);
}


//Lee de argv, el mensaje para guardar en buff
void getMessageFromArg(char* buff, const char* argv[]) {
    strcpy(buff, argv[2]);
    //Agregar final a buff
    int stringSize = strlen(buff);
    
    buff[stringSize] = '\0';
}


int main(int argc, const char* argv[]) {

    //Asegura cantidad de argumentos. Nombre, port, message
    if (argc != 3) {
        error("Uso: ./client <port> <message>");
    }

    int port = getPortFromArg(argv);

    //Inicializar con ceros, para no tener basura.
    char buff[BUFFER_SIZE] = {0};
    getMessageFromArg(buff, argv);

    int sockFd = getSockClient();
    sockaddr_in addr;
    getSockAddrIPv4("127.0.0.1", port, &addr);
	

    //Se conecta a broker
	if ((connect(sockFd,(sockaddr*) &addr,sizeof(addr))) == -1)
		error("Hubo un error, conexion cancelada");
	
    
	sendMsg(sockFd, buff);
	
	
	close(sockFd);
	return  EXIT_SUCCESS;
}
