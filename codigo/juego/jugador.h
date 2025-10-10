#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#include "controles.h"
#include "laberinto.h"
#include "../estructuras/cola.h"
#include "juego.h"

void crearJugador(tJugador* jugador, tConfiguracion* configuracion, size_t fila, size_t columna);
void dibujarJugador(tJugador* jugador, size_t fila, size_t columna);
char moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto);
tPosicion obtenerPosJugador(tJugador* jugador);

#endif // JUGADOR_H_INCLUDED
