#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED
#include "arbol.h"

#include <winsock2.h>

#define PORT 12345
#define BUFFER_SIZE 1024

#define NOMBRE_ARCH_USUARIOS "usuarios.dat"
#define NOMBRE_ARCH_PARTIDAS "partidas.dat"
#define NOMBRE_ARCH_INDICE "indice.dat"

#define ERR_ARCH 2
#define NO_ENCONTRADO 3

typedef struct
{
    int id;
    char nombre[16];
    int desp;
}tIdxJugador;

typedef struct
{
    int id;
    char nombre[16];
}tJugador;

typedef struct
{
    int idPartida, idJugador, puntuacion, cantMovs;
}tPartida;

// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Procesa el mensaje recibido y genera la respuesta
void procesarEntrada(const char *peticion, char *respuesta, tArbol *a, tCmp cmp);

// Ejecuta el bucle principal del servidor
void run_server();


int agregarAArchivo(const char *nombreArchUsuario, char *nombre);
int crearIdx(tArbol *a, void *reg, int desp, tCmp cmp);
int cmpIdx(const void *a, const void *b);
int agregarAArchivo(const char *nombreArchUsuario, char *nombre);
int guardarPartida(const char *nombreArchPartida, char *nombre, int puntaje, int movs, tArbol *a);

void imprimirIdxJugador(void *a);

#endif // SERVIDOR_H_INCLUDED
