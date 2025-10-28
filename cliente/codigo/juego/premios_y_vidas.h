#ifndef PREMIOS_Y_VIDAS_H_INCLUDED
#define PREMIOS_Y_VIDAS_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include "laberinto.h"
#include "jugador.h"

#define PUNTAJE_BAJO 150
#define PUNTAJE_MEDIO 300
#define PUNTAJE_ALTO 700

#define OPCION_LIMITE 7
#define MIN_BAJO 1
#define MAX_BAJO 3
#define MIN_MEDIO 4
#define MAX_MEDIO 5

#define TOPE_VIDAS 99

void mostrarVidasYPuntos(tJugador* jugador);

// Funciones para premios
char chequeoPremio(tJugador* jugador, tLaberinto* laberinto);
void sumarPuntaje(tJugador* jugador);

// Funciones para vidas extra
char chequeoVida(tJugador* jugador, tLaberinto* laberinto);
void sumarVida(tJugador* jugador);

#endif // PREMIOS_Y_VIDAS_H_INCLUDED
