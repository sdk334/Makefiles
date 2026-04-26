#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protocolo.h"

void generar_lectura(LecturaSensor *l, uint32_t id) {
    l->nodo_id = id;
    l->temperatura = (float)(rand() % 30) + 15.0; // 15 - 45 C
    l->humedad = (float)(rand() % 50) + 30.0;     // 30 - 80 %
    l->timestamp = (uint64_t)time(NULL);
}
