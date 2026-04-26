# Uso y Documentación de Makefiles

**Alumno:** Jonathan Uriel Paredes Martinez
**Carrera:** Ingeniería en Sistemas Computacionales — ESCOM IPN

---

## Tabla de Contenidos

1. [¿Qué es un Makefile?](#1-qué-es-un-makefile)
2. [Tabla de análisis por componente](#2-tabla-de-análisis-por-componente)
3. [Proyecto Propio: Sistema de Monitoreo IoT](#3-proyecto-propio-sistema-de-monitoreo-iot)
   - [Arquitectura del Sistema](#arquitectura-del-sistema)
   - [Descripción de Archivos](#descripción-de-archivos)
   - [Explicación Detallada del Código Fuente](#explicación-detallada-del-código-fuente)
   - [El Makefile del Proyecto](#el-makefile-del-proyecto)
4. [Instrucciones de Compilación y Ejecución](#4-instrucciones-de-compilación-y-ejecución)
5. [Explicación de la Automatización](#5-explicación-de-la-automatización)
6. [Requisitos del Sistema](#6-requisitos-del-sistema)

---

## 1. ¿Qué es un Makefile?

Un **Makefile** es un archivo de texto plano que le indica a la herramienta `make` cómo compilar y enlazar un programa. En lugar de escribir manualmente cada comando de compilación en la terminal cada vez que se modifica el código, el Makefile automatiza todo ese proceso.

La utilidad `make` lee el archivo `Makefile` que se encuentre en el directorio actual y ejecuta las instrucciones definidas en él. Su principal ventaja es la **compilación incremental**: `make` compara las fechas de modificación de los archivos fuente (`.c`) con sus archivos objeto (`.o`) ya generados; si el fuente no ha cambiado, no lo recompila, ahorrando tiempo en proyectos grandes.


---



## 2. Tabla de análisis por componente

| Elemento | Código | Explicación técnica |
| :--- | :--- | :--- |
| **Variable `CC`** | `CC = gcc` | Define el compilador a usar. Al usar una variable en vez del valor literal, cambiar de compilador (p. ej. a `clang`) solo requiere modificar esta línea. Se referencia en el Makefile como `$(CC)`. |
| **Variable `CFLAGS`** | `CFLAGS = -Wall -Wextra -g` | Banderas de compilación. `-Wall` activa todas las advertencias comunes, `-Wextra` activa advertencias adicionales, y `-g` incluye información de depuración en el binario (útil con `gdb`). |
| **Target `hola`** | `hola: hola.o` | Define el objetivo final (el ejecutable). Su dependencia es `hola.o`; si ese archivo objeto no existe o está desactualizado, `make` lo reconstruye antes de ejecutar el comando de enlazado. |
| **Comando de enlazado** | `$(CC) $(CFLAGS) -o hola hola.o` | Invoca `gcc` para **enlazar** el archivo objeto `hola.o` y producir el ejecutable `hola`. La bandera `-o` especifica el nombre del archivo de salida. |
| **Target `hola.o`** | `hola.o: hola.c` | Define cómo producir el archivo objeto. Depende de `hola.c`; si el fuente cambia, este target se reconstruye. |
| **Comando de compilación** | `$(CC) $(CFLAGS) -c hola.c` | Invoca `gcc` para **compilar** (pero no enlazar) el fuente. La bandera `-c` le indica a `gcc` que solo genere el archivo objeto `.o`, no el ejecutable final. |
| **Directiva `.PHONY`** | `.PHONY: clean` | Le dice a `make` que `clean` no es un archivo real, sino una etiqueta lógica. Sin esto, si existiera un archivo llamado `clean` en el directorio, `make` pensaría que el objetivo ya está construido y no ejecutaría el comando. |
| **Target `clean`** | `rm -f hola hola.o` | Elimina el ejecutable y los archivos objeto. La bandera `-f` en `rm` previene errores si los archivos no existen. Se invoca manualmente con `make clean`. |

### Flujo de ejecución al correr `make`

```
make
  │
  ├─► ¿Existe el ejecutable 'hola'?
  │     ├─ No → construir
  │     └─ Sí → ¿Es más nuevo que 'hola.o'?
  │               └─ Sí → no hacer nada (ya está actualizado)
  │
  ├─► ¿Existe 'hola.o'?
  │     ├─ No → compilar hola.c → genera hola.o
  │     └─ Sí → ¿Es más nuevo que 'hola.c'?
  │               └─ Sí → no recompilar
  │
  └─► Enlazar hola.o → produce el ejecutable 'hola'
```

**Resultado del análisis:** Al ejecutar `make` por primera vez, el sistema compila y enlaza todo el proyecto. En ejecuciones subsecuentes, si ningún archivo `.c` fue modificado, `make` responde con `make: 'hola' is up to date` y no realiza ningún trabajo innecesario.

---

## 3. Proyecto Propio: Sistema de Monitoreo IoT

### Arquitectura del Sistema

Este proyecto simula un entorno de **Internet de las Cosas (IoT)** donde nodos sensores capturan datos ambientales (temperatura y humedad) y los transmiten a una pasarela central (Gateway) que los procesa y genera alertas cuando se detectan valores críticos.

```
┌─────────────────┐        protocolo.h        ┌──────────────────┐
│   sensor_nodo.c │  ──── Paquete binario ──►  │   gateway.c      │
│                 │                            │                  │
│ Simula lecturas │        (struct IoT)        │ Filtra y procesa │
│ de temperatura  │                            │ alertas críticas │
│ y humedad       │                            │                  │
└─────────────────┘                            └──────────────────┘
         ▲                                              │
         │                                              │
         └──────────────── main.c ─────────────────────┘
                    (Orquestador del flujo)
```

### Descripción de Archivos

```
Proyecto_IoT/
├── Makefile          ← Automatiza toda la compilación
├── protocolo.h       ← Cabecera: define la estructura de datos compartida
├── sensor_nodo.c     ← Módulo: simula la lectura de sensores
├── gateway.c         ← Módulo: procesa y filtra los datos recibidos
└── main.c            ← Punto de entrada: orquesta la simulación
```

---

### Explicación Detallada del Código Fuente

#### `protocolo.h` — La estructura de datos de comunicación

```c
#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#define MAX_NODOS 10
#define TEMP_CRITICA 38.0f

// Estructura del paquete de datos IoT
typedef struct {
    int   id_nodo;        // Identificador único del sensor
    float temperatura;    // Temperatura en grados Celsius
    float humedad;        // Humedad relativa en porcentaje (0–100)
} PaqueteIoT;

// Declaración de funciones (prototipos)
void simular_sensor(PaqueteIoT *paquete, int id);
void procesar_gateway(PaqueteIoT *paquete);

#endif // PROTOCOLO_H
```



---

#### `sensor_nodo.c` — El simulador de nodos sensores

```c
#include <stdio.h>
#include <stdlib.h>   // Para rand() y srand()
#include <time.h>     // Para time(), usada como semilla del aleatorio
#include "protocolo.h"

/*
 * simular_sensor: Llena la estructura PaqueteIoT con datos
 * simulados de temperatura y humedad para un nodo dado.
 *
 * Parámetros:
 *   paquete → puntero al struct que será rellenado
 *   id      → identificador del nodo sensor
 */
void simular_sensor(PaqueteIoT *paquete, int id) {
    // Asignar el ID del nodo
    paquete->id_nodo = id;

    // Generar temperatura aleatoria entre 20.0 y 45.0 °C
    // La fórmula: (rand() % 250) genera un entero 0–249
    // Dividir entre 10.0f da un float 0.0–24.9
    // Sumar 20.0f desplaza el rango a 20.0–44.9
    paquete->temperatura = 20.0f + (rand() % 250) / 10.0f;

    // Generar humedad aleatoria entre 30.0% y 90.0%
    paquete->humedad = 30.0f + (rand() % 600) / 10.0f;

    printf("[Nodo %d] Temperatura: %.1f°C | Humedad: %.1f%%\n",
           paquete->id_nodo,
           paquete->temperatura,
           paquete->humedad);
}
```

**Puntos clave:**

- Se usa el operador `->` para acceder a los miembros de la estructura a través del puntero `paquete`. Equivale a `(*paquete).temperatura`.
- Se pasa el paquete **por puntero** (`PaqueteIoT *paquete`) en lugar de por valor, para que los cambios hechos dentro de la función sean visibles en `main.c`. Pasar por valor crearía una copia local que se descarta al terminar la función.

---

#### `gateway.c` — El procesador central

```c
#include <stdio.h>
#include "protocolo.h"

/*
 * procesar_gateway: Recibe un paquete IoT y determina
 * si la temperatura supera el umbral crítico definido
 * en protocolo.h (TEMP_CRITICA = 38.0°C).
 */
void procesar_gateway(PaqueteIoT *paquete) {
    printf("[Gateway] Procesando nodo %d...\n", paquete->id_nodo);

    if (paquete->temperatura >= TEMP_CRITICA) {
        // Alerta: temperatura por encima del umbral
        printf("[ALERTA] Nodo %d: Temperatura CRÍTICA (%.1f°C). "
               "Se requiere intervención.\n",
               paquete->id_nodo,
               paquete->temperatura);
    } else {
        // Lectura normal
        printf("[OK]     Nodo %d: Temperatura normal (%.1f°C).\n",
               paquete->id_nodo,
               paquete->temperatura);
    }
}
```

**Puntos clave:**

- La constante `TEMP_CRITICA` se define en `protocolo.h` como `38.0f`. Usar una constante con nombre en lugar del número literal `38.0` directamente en el código mejora la mantenibilidad: si el umbral cambia, basta modificarlo en un solo lugar.
- La función no necesita retornar nada (`void`) porque su efecto es imprimir en pantalla. En un sistema real, podría retornar un código de error o enviar una señal a otro módulo.

---

#### `main.c` — El orquestador principal

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protocolo.h"

int main(void) {
    // Inicializar la semilla del generador de números aleatorios
    // con la hora actual, para obtener valores diferentes en cada ejecución
    srand((unsigned int)time(NULL));

    PaqueteIoT paquete;   // Estructura reutilizada para cada nodo
    int i;

    printf("=== Sistema de Monitoreo IoT ===\n\n");

    // Iterar sobre todos los nodos definidos en MAX_NODOS
    for (i = 0; i < MAX_NODOS; i++) {
        printf("--- Ciclo de lectura: Nodo %d ---\n", i + 1);

        // 1. El nodo sensor llena el paquete con datos
        simular_sensor(&paquete, i + 1);

        // 2. El gateway procesa el paquete y genera alertas
        procesar_gateway(&paquete);

        printf("\n");
    }

    printf("=== Simulación completada ===\n");
    return 0;   // Convención: retornar 0 indica éxito al sistema operativo
}
```

**Puntos clave:**

- `srand(time(NULL))` inicializa el generador pseudoaleatorio con la hora actual en segundos. Sin esta línea, `rand()` siempre generaría la misma secuencia de números.
- Se usa `&paquete` (dirección de memoria de la variable `paquete`) al llamar a `simular_sensor` y `procesar_gateway`, porque ambas funciones esperan un **puntero** (`PaqueteIoT *`).
- La constante `MAX_NODOS` (definida en `protocolo.h`) controla cuántos nodos simula el sistema. Cambiarla en el header afecta automáticamente a `main.c` sin necesidad de editar este archivo.

---

### El Makefile del Proyecto

El Makefile del proyecto IoT es el componente central de la automatización. A continuación se presenta y se explica cada línea:

```makefile
# ═══════════════════════════════════════════════════════════════
# Makefile — Sistema de Monitoreo IoT
# Ingeniería en Sistemas Computacionales, ESCOM IPN
# ═══════════════════════════════════════════════════════════════

# ──────────────────────────────────────────────
# SECCIÓN 1: Variables de configuración
# ──────────────────────────────────────────────

# Compilador a utilizar
CC = gcc

# Banderas de compilación:
#   -Wall    → activa advertencias comunes (Wall = "all warnings")
#   -Wextra  → activa advertencias adicionales
#   -g       → incluye símbolos de depuración (para usar con gdb)
CFLAGS = -Wall -Wextra -g

# Nombre del ejecutable final
TARGET = monitor_iot

# Lista de archivos objeto que componen el proyecto
# Cada uno se origina de un archivo .c con el mismo nombre base
OBJS = main.o sensor_nodo.o gateway.o


# ──────────────────────────────────────────────
# SECCIÓN 2: Target principal (all)
# ──────────────────────────────────────────────

# 'all' es el target por defecto; make lo ejecuta si no se
# especifica otro target en la línea de comandos.
# Su dependencia es el ejecutable final $(TARGET).
all: $(TARGET)

# Regla de enlazado: une todos los .o para formar el ejecutable
# $@ → nombre del target actual    = monitor_iot
# $^ → lista de todas las dependencias = main.o sensor_nodo.o gateway.o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


# ──────────────────────────────────────────────
# SECCIÓN 3: Regla de patrón (compilación modular)
# ──────────────────────────────────────────────

# Esta única regla reemplaza una regla individual para cada .c
# El símbolo % actúa como comodín (wildcard).
# Lee: "para producir CUALQUIER archivo .o, compila el .c del mismo nombre"
#
# $< → primera dependencia (el archivo .c correspondiente)
# $@ → el objetivo (el archivo .o a generar)
%.o: %.c protocolo.h
	$(CC) $(CFLAGS) -c $< -o $@


# ──────────────────────────────────────────────
# SECCIÓN 4: Target de limpieza
# ──────────────────────────────────────────────

# .PHONY indica que 'clean' no es un archivo real,
# sino una etiqueta de acción. Evita conflictos si
# existiera un archivo llamado 'clean' en el directorio.
.PHONY: all clean

clean:
	rm -f $(TARGET) $(OBJS)
```

#### Desglose de cada elemento del Makefile

**Variables automáticas de Make**

| Variable | Significado | Ejemplo en este proyecto |
| :---: | :--- | :--- |
| `$@` | El nombre del **target** (objetivo) actual | `monitor_iot` en la regla de enlazado |
| `$^` | **Todas** las dependencias del target | `main.o sensor_nodo.o gateway.o` |
| `$<` | La **primera** dependencia (o la única en reglas de patrón) | `main.c` cuando se compila `main.o` |

**La regla de patrón `%.o: %.c`**

Esta es la línea más importante del Makefile. En lugar de escribir:

```makefile
# Sin regla de patrón (repetitivo y propenso a errores):
main.o: main.c protocolo.h
	$(CC) $(CFLAGS) -c main.c -o main.o

sensor_nodo.o: sensor_nodo.c protocolo.h
	$(CC) $(CFLAGS) -c sensor_nodo.c -o sensor_nodo.o

gateway.o: gateway.c protocolo.h
	$(CC) $(CFLAGS) -c gateway.c -o gateway.o
```

Se escribe una sola vez:

```makefile
# Con regla de patrón (conciso y escalable):
%.o: %.c protocolo.h
	$(CC) $(CFLAGS) -c $< -o $@
```

`make` expande automáticamente el patrón para cada archivo objeto en `$(OBJS)`. Además, al incluir `protocolo.h` como dependencia, `make` sabe que si el header cambia, **todos** los módulos deben recompilarse.

---

## 4. Instrucciones de Compilación y Ejecución

### Requisitos previos

Asegúrate de tener instalado `gcc` y `make`. En sistemas Debian/Ubuntu:

```bash
sudo apt update && sudo apt install build-essential
```

### Clonar el repositorio

```bash
git clone https://github.com/sdk334/Makefiles.git
cd Makefiles/Proyecto_IoT
```

### Compilar el proyecto

```bash
make
```

`make` leerá el `Makefile` del directorio actual y ejecutará los siguientes pasos automáticamente:

1. Compila `main.c` → genera `main.o`
2. Compila `sensor_nodo.c` → genera `sensor_nodo.o`
3. Compila `gateway.c` → genera `gateway.o`
4. Enlaza los tres `.o` → genera el ejecutable `monitor_iot`

La salida esperada en la terminal es similar a:

```
gcc -Wall -Wextra -g -c main.c -o main.o
gcc -Wall -Wextra -g -c sensor_nodo.c -o sensor_nodo.o
gcc -Wall -Wextra -g -c gateway.c -o gateway.o
gcc -Wall -Wextra -g -o monitor_iot main.o sensor_nodo.o gateway.o
```

### Ejecutar la simulación

```bash
./monitor_iot
```

Salida:

```
=== Sistema de Monitoreo IoT ===

--- Ciclo de lectura: Nodo 1 ---
[Nodo 1] Temperatura: 24.3°C | Humedad: 65.0%
[Gateway] Procesando nodo 1...
[OK]     Nodo 1: Temperatura normal (24.3°C).

--- Ciclo de lectura: Nodo 2 ---
[Nodo 2] Temperatura: 41.7°C | Humedad: 78.2%
[Gateway] Procesando nodo 2...
[ALERTA] Nodo 2: Temperatura CRÍTICA (41.7°C). Se requiere intervención.

...
=== Simulación completada ===
```

### Limpiar el repositorio

```bash
make clean
```

Esto elimina todos los archivos generados durante la compilación (`monitor_iot`, `main.o`, `sensor_nodo.o`, `gateway.o`), dejando solo el código fuente.

---

## 5. Explicación de la Automatización

La automatización en este proyecto se logra mediante tres mecanismos combinados del Makefile:

### 5.1 Detección de cambios por estampas de tiempo

`make` compara automáticamente la fecha de modificación (`mtime`) de cada archivo. El flujo de decisión es:

```
¿monitor_iot existe?
    └─ No  → Construir (enlazar todos los .o)
    └─ Sí  → ¿Algún .o es más reciente que monitor_iot?
                └─ No  → "make: 'monitor_iot' is up to date" (no hace nada)
                └─ Sí  → Re-enlazar

¿Algún .o debe reconstruirse?
    └─ ¿Su .c correspondiente o protocolo.h son más recientes?
        └─ No  → No recompilar ese módulo
        └─ Sí  → Recompilar solo ese módulo
```

**Ejemplo práctico:** Si se modifica únicamente `gateway.c`, `make` recompilará solo `gateway.o` y volverá a enlazar el ejecutable. Los archivos `main.o` y `sensor_nodo.o`, que no cambiaron, no se tocan.

### 5.2 Reglas de patrón para escalabilidad

La regla `%.o: %.c` hace que el Makefile sea escalable. Si en el futuro se agrega un nuevo módulo, por ejemplo `logger.c`, solo es necesario añadir `logger.o` a la variable `OBJS`:

```makefile
OBJS = main.o sensor_nodo.o gateway.o logger.o
```

No hay que agregar ninguna regla de compilación adicional; la regla de patrón la cubre automáticamente.

### 5.3 Gestión de dependencias del header

Al incluir `protocolo.h` como dependencia en la regla de patrón:

```makefile
%.o: %.c protocolo.h
```

`make` garantiza que si el archivo de cabecera es modificado (por ejemplo, si se agrega un nuevo campo a `PaqueteIoT` o se cambia `TEMP_CRITICA`), **todos** los archivos objeto que dependen de él serán recompilados, evitando inconsistencias en el binario final.

---

## 6. Requisitos del Sistema

| Requisito | Versión mínima recomendada |
| :--- | :--- |
| Sistema Operativo | Linux (cualquier distribución moderna) |
| Compilador | GCC 9.0 o superior |
| Herramienta de build | GNU Make 4.2 o superior |
| Estándar C | C99 |

Para verificar las versiones instaladas:

```bash
gcc --version
make --version
```

---
