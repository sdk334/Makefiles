#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <stdint.h>

// Estructura que representa un paquete de datos IoT
typedef struct {
    uint32_t nodo_id;
    float temperatura;
    float humedad;
    uint64_t timestamp;
} LecturaSensor;

void imprimir_lectura(LecturaSensor lectura);

#endif
