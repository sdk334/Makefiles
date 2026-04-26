# Configuracion del compilador
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = monitor_iot

# Listado de archivos fuente y objetos
SRCS = main.c gateway.c sensor_nodo.c
OBJS = $(SRCS:.c=.o)

# Regla principal
all: $(TARGET)

# Vinculacion del ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilacion de archivos objeto
%.o: %.c protocolo.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos temporales
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
