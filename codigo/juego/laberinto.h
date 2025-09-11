#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#include <stdlib.h>
#include "../globales.h"

#define MAX_FILAS 50
#define MAX_COLUMNAS 50

#define PARED '#'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define JUGADOR 'J'
#define FANTASMA 'F'
#define PREMIO 'P'
#define VIDA_EXTRA 'V'

typedef struct
{
    char casillas[MAX_FILAS][MAX_COLUMNAS];
    size_t filas;
    size_t columnas;
} tLaberinto;

void crearLaberinto(tLaberinto* laberinto, size_t filas, size_t columnas);
void destruirLaberinto(tLaberinto* laberinto);

size_t obtenerFilasLaberinto(tLaberinto* laberinto);
size_t obtenerColumnasLaberinto(tLaberinto* laberinto);
char* obtenerCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna);

void generarLaberintoAleatorio(tLaberinto* laberinto);

#endif // LABERINTO_H_INCLUDED
