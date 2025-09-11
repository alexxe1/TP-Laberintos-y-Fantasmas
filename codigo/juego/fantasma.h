#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED

#include "laberinto.h"
#include "jugador.h"
#include <stdlib.h>

#define DERECHA 0
#define IZQUIERDA 1
#define ARRIBA 2
#define ABAJO 3

typedef struct
{
    size_t filaInicial;
    size_t columnaInicial;
    size_t filaActual;
    size_t columnaActual;
} tFantasma;

void crearFantasma(tFantasma* fantasma, size_t filaInicial, size_t columnaInicial);
void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna);
void calcularMovimientoFantasma(tFantasma* fantasma, const tJugador* jugador);
void moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto);

#endif // FANTASMA_H_INCLUDED
