#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#include <stdlib.h>
#include "../retornos.h"

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

typedef struct
{
    size_t filas;
    size_t columnas;
    size_t vidasIniciales;
    size_t maxFantasmas;
    size_t maxPremios;
    size_t maxVidasExtras;
} tConfiguracion;

void crearLaberintoAleatorio(tLaberinto* laberinto);
int crearLaberintoArchivo(tLaberinto* laberinto, char* nombreArchivo);
void destruirLaberinto(tLaberinto* laberinto);

size_t obtenerFilasLaberinto(tLaberinto* laberinto);
size_t obtenerColumnasLaberinto(tLaberinto* laberinto);
char obtenerCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna);
void modificarCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna, char nuevaCasilla);

#endif // LABERINTO_H_INCLUDED
