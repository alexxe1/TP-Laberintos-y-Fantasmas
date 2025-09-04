#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stddef.h>
#include "laberinto.h"

// Controles del jugador
#define TECLA_ARRIBA(c) ((c) == 72 || (c) == 'w') // 72 es la clave de arriba
#define TECLA_IZQUIERDA(c) ((c) == 75 || (c) == 'a') // 75 es la clave de izquierda
#define TECLA_ABAJO(c) ((c) == 80 || (c) == 's') // 80 es la clave de abajo
#define TECLA_DERECHA(c) ((c) == 77 || (c) == 'd') // 77 es la clave de derecha

typedef struct
{
    size_t fila;
    size_t columna;
} tJugador;

void crearJugador(tJugador* jugador, size_t filaInicial, size_t columnaInicial);
void moverJugador(tJugador* jugador, char direccion, tLaberinto* laberinto);

#endif // JUGADOR_H_INCLUDED
