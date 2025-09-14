#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include "../retornos.h"
#include "configuracion.h"

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

#define ARCHIVO_LABERINTO "laberinto.txt"

typedef struct
{
    char** casillas;
    size_t filas;
    size_t columnas;
} tLaberinto;

int crearLaberintoAleatorio(tLaberinto* laberinto, tConfiguracion* configuracion);
int crearLaberintoArchivo(tLaberinto* laberinto);
void destruirLaberinto(tLaberinto* laberinto);

size_t obtenerFilasLaberinto(tLaberinto* laberinto);
size_t obtenerColumnasLaberinto(tLaberinto* laberinto);
char obtenerCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna);
void modificarCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna, char nuevaCasilla);

#endif // LABERINTO_H_INCLUDED
