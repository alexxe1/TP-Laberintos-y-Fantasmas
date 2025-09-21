#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#include "controles.h"
#include "laberinto.h"
#include "../estructuras/cola.h"

typedef struct
{
    size_t filaActual;
    size_t columnaActual;
    size_t filaInicial;
    size_t columnaInicial;
    tCola cola;
} tJugador;

void crearJugador(tJugador* jugador, size_t fila, size_t columna);
void dibujarJugador(tJugador* jugador, size_t fila, size_t columna);
int moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto);

#endif // JUGADOR_H_INCLUDED
