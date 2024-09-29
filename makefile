# Nombre de los archivos fuente principales
SOURCES = client.c broker.c server.c

# Nombres de los ejecutables
EXECUTABLES = client broker server

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g -I./utils  # -I para incluir el directorio de utils.h

# Archivos objeto
OBJS_CLIENT = client.o utils.o
OBJS_BROKER = broker.o utils.o
OBJS_SERVER = server.o utils.o

# Regla por defecto
all: $(EXECUTABLES)

# Regla para compilar el cliente
client: $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o client $(OBJS_CLIENT)

# Regla para compilar el broker
broker: $(OBJS_BROKER)
	$(CC) $(CFLAGS) -o broker $(OBJS_BROKER)

# Regla para compilar el servidor
server: $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o server $(OBJS_SERVER)

# Compilación de los archivos fuente individuales
client.o: client.c
	$(CC) $(CFLAGS) -c client.c

broker.o: broker.c
	$(CC) $(CFLAGS) -c broker.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

# Compilación de utils
utils.o: utils/utils.c
	$(CC) $(CFLAGS) -c utils/utils.c

# Regla para limpiar los archivos generados
clean:
	rm -f *.o $(EXECUTABLES)

.PHONY: all clean
