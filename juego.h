#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"
#include "jugador.h"

#include <stdio.h> // Para imprimir por consola (temporal)
#include <windows.h> // Para limpiar la consola (temporal)
#include <conio.h> // Para detectar input (temporal)

void empezarJuego();
void actualizarLaberinto(tLaberinto* laberinto);
void dibujarLaberinto(tLaberinto* laberinto, tJugador* jugador);

#endif // JUEGO_H_INCLUDED
