#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "../estructuras/vector.h"
#include "../estructuras/cola.h"

/// Estructura para controlar posiciones
typedef struct
{
    unsigned fila;
    unsigned columna;
} tPosicion;

/// Estructura correspondiente al jugador
typedef struct
{
    tPosicion posActual;
    tPosicion posInicial;
    tCola cola;
    size_t puntajeTotal;
    size_t vidas;
} tJugador;

/// Estructura correspondiente al fantasma
typedef struct
{
    tPosicion posActual;
    tPosicion posInicial;
    tCola cola;
    char ultMov;
    unsigned short tocado;
} tFantasma;

/// Estructura para englobar todas las entidades
typedef struct
{
    tJugador jugador;
    tVector fantasmas;
} tEntidades;

#endif // STRUCTS_H_INCLUDED
