#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED
#define MAX_DIST 100

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
char calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador);
int moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto);

#endif // FANTASMA_H_INCLUDED
