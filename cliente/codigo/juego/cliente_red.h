#ifndef CLIENTE_RED_H_INCLUDED
#define CLIENTE_RED_H_INCLUDED

#include <stdlib.h>
#include <conio.h>
#include "controles.h"
#include "../retornos.h"
#include "entidades.h" // Para el MAX_NOM
#include "../estructuras/lista.h"
#include <winsock2.h> // Para los sockets

#define IP_SERVER "127.0.0.1"
#define PUERTO 12345

typedef struct
{
    size_t posicion;
    char nombre[MAX_NOM];
    size_t puntos;
} tRanking;

// General
int iniciarConexion();
SOCKET conectarseAlServidor(const char* ip, int puerto);
void cerrarConexion(SOCKET socket);

// Juego
char solicitarRankingsServidor(SOCKET* socket, tLista* listaRankings);
char intentarConectarServidor(SOCKET* socket, const char* ip, int puerto);
char mandarDatosPartidaServidor(char* nombreJugador, size_t puntajeTotal, size_t cantMovimientos);

#endif // CLIENTE_RED_H_INCLUDED
