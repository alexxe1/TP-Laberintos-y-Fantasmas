#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED

#include "laberinto.h"
#include "jugador.h"
#include <stdlib.h>

typedef struct
{
    size_t filaInicial;
    size_t columnaInicial;
    size_t filaActual;
    size_t columnaActual;
} tFantasma;

void crearFantasma(tFantasma* fantasma, size_t filaInicial, size_t columnaInicial);
void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna);
void calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador);
void moverFantasma(tFantasma* fantasma, tJugador *jugador, const tLaberinto* laberinto);

#endif // FANTASMA_H_INCLUDED
