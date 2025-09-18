#ifndef PREMIOS_Y_VIDAS_H_INCLUDED
#define PREMIOS_Y_VIDAS_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include "laberinto.h"
#include "jugador.h"

#define PUNTAJE_LOW 150
#define PUNTAJE_MID 300
#define PUNTAJE_HIGH 700

#define OPCION_LIMITE 7
#define MIN_LOW 1
#define MAX_LOW 3
#define MIN_MID 4
#define MAX_MID 5

void vidasYPuntos(tJugador * jugador);

unsigned short chequeoPremio (tJugador * jugador, tLaberinto * laberinto);
void sumarPuntaje (tJugador * jugador, tLaberinto * laberinto);
void reemplazarPremio (tJugador * jugador, tLaberinto * laberinto);

#endif // PREMIOS_Y_VIDAS_H_INCLUDED
