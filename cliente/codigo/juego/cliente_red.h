#ifndef CLIENTE_RED_H_INCLUDED
#define CLIENTE_RED_H_INCLUDED

#include <stdlib.h>
#include "../retornos.h"
#include "entidades.h" // Para el MAX_NOM
#include "../estructuras/lista.h"

typedef struct
{
    size_t posicion;
    char nombre[MAX_NOM];
    size_t puntos;
} tRanking;

char hayConexionServidor();
char solicitarRankingsServidor(tLista* listaRankings);
char mandarDatosPartidaServidor(char* nombreJugador, size_t puntajeTotal, size_t cantMovimientos);

#endif // CLIENTE_RED_H_INCLUDED
