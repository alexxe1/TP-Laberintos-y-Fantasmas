#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "../estructuras/vector.h"
#include "../estructuras/cola.h"

#define MAX_NOM 17

// Estructura para controlar posiciones
typedef struct
{
    size_t fila;
    size_t columna;
} tPosicion;

// Estructura correspondiente al jugador
typedef struct
{
    char nombre[MAX_NOM];
    tPosicion posInicial;
    tPosicion posActual;
    tCola colaMovimientos;
    size_t puntajeTotal;
    size_t vidas;
} tJugador;

// Estructura correspondiente al fantasma
typedef struct
{
    tPosicion posInicial;
    tPosicion posActual;
    tCola colaMovimientos;
    char ultimoMovimiento;
    char tocado;
} tFantasma;

// Estructura para englobar todas las entidades
typedef struct
{
    tJugador jugador;
    tVector fantasmas;
    tCola colaMov;

} tEntidades;

//Estructura para las colas
typedef struct
{
    char tipo;
    unsigned id;
    char mov;
}tMov;
#endif // STRUCTS_H_INCLUDED
