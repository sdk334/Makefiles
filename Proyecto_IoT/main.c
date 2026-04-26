#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "protocolo.h"

// Prototipos de funciones en otros módulos
void generar_lectura(LecturaSensor *l, uint32_t id);
void procesar_datos(LecturaSensor lectura);

int main() {
    srand(time(NULL));
    LecturaSensor buffer;
    
    printf("=== Sistema de Monitoreo IoT Iniciado ===\n\n");

    for(int i = 0; i < 3; i++) {
        generar_lectura(&buffer, i + 100);
        procesar_datos(buffer);
        sleep(1); // Simular intervalo de lectura
    }

    return 0;
}
