#include <stdio.h>
#include "protocolo.h"

void procesar_datos(LecturaSensor lectura) {
    printf("[GATEWAY] Procesando datos del Nodo ID: %u\n", lectura.nodo_id);
    if (lectura.temperatura > 35.0) {
        printf("⚠️ ALERTA: Temperatura crítica detectada!\n");
    }
    imprimir_lectura(lectura);
}

void imprimir_lectura(LecturaSensor lectura) {
    printf(">> Temp: %.2f C | Hum: %.2f%% | TS: %lu\n", 
            lectura.temperatura, lectura.humedad, lectura.timestamp);
    printf("------------------------------------------\n");
}
