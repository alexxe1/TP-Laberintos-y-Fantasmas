#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED
#define MAX_DIST 100

#include "laberinto.h"
#include "juego.h"

#include "../estructuras/cola.h"
#include "../estructuras/vector.h"

#include <stdlib.h>


void crearFantasma(tFantasma* fantasma, size_t filaInicial, size_t columnaInicial);
int hayFantasma(tVector* vecFantasmas, size_t fila, size_t columna);
void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna);
char calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador);
int moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto);
char CalculaOpuesto(const char c);
char detectarMov(int mov[][2], int pos);
unsigned short chequeoFantasma (tVector* vecFantasmas, tJugador * jugador);
tPosicion obtenerPosFantasma(tFantasma *fantasma);


#endif // FANTASMA_H_INCLUDED
