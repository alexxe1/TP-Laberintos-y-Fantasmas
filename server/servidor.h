#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED
#include "arbol.h"
#include "lista.h"

#include <winsock2.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_NOM 16
#define RANK 5

#define NOMBRE_ARCH_USUARIOS "usuarios.dat"
#define NOMBRE_ARCH_PARTIDAS "partidas.dat"
#define NOMBRE_ARCH_INDICE "indice.dat"

#define ERR_ARCH 2
#define NO_ENCONTRADO 3

typedef struct
{
    int id;
    char nombre[MAX_NOM];
    int desp;
}tIdxJugador;

typedef struct
{
    int id;
    char nombre[MAX_NOM];
}tJugador;

typedef struct
{
    int id;
    char nombre[MAX_NOM];
    size_t puntos;
}tRanking;

typedef struct
{
    int idPartida, idJugador, puntuacion, cantMovs;
}tPartida;

// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Procesa el mensaje recibido y genera la respuesta
int procesarEntrada(const char *peticion, char respuesta, tArbol *a, tCmp cmp, SOCKET cliente);
// Ejecuta el bucle principal del servidor
void run_server();


int agregarAArchivo(const char *nombreArchUsuario, char *nombre);
int crearIdx(tArbol *a, void *reg, int desp, tCmp cmp);
int cmpIdx(const void *a, const void *b);
int agregarAArchivo(const char *nombreArchUsuario, char *nombre);
int guardarPartida(const char *nombreArchPartida, char *nombre, int puntaje, int movs, tArbol *a);
tLista generarRanking(tArbol *a, const char *nombreArchPartidas);

void imprimirIdxJugador(void *a);
int cmpId(const void *a, const void *b);
int acumularPuntos(void **dest, unsigned *tamDest, const void *origen, unsigned tamOrig);
void imprimirPartida(void *a);

#endif // SERVIDOR_H_INCLUDED
