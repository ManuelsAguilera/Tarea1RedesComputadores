# Nombre de los archivos fuente
SOURCES = client.c broker.c server.c

# Nombres de los ejecutables
EXECUTABLES = client broker server

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Regla por defecto
all: $(EXECUTABLES)

# Regla para compilar el cliente
client: client.c
	$(CC) $(CFLAGS) -o client client.c

# Regla para compilar el broker
broker: broker.c
	$(CC) $(CFLAGS) -o broker broker.c

# Regla para compilar el servidor
server: server.c
	$(CC) $(CFLAGS) -o server server.c

# Regla para limpiar los archivos generados
clean:
	rm -f $(EXECUTABLES)

.PHONY: all clean
