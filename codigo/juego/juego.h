#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"
#include "fantasma.h"
#include "jugador.h"
#include "configuracion.h"
#include "premios_y_vidas.h"
#include "../estructuras/vector.h"

#include "../estructuras/cola.h"

#include "../menu/menu.h"

#include <stdio.h> // Para imprimir por consola (temporal)
#include <windows.h> // Para limpiar la consola (temporal)
#include <conio.h> // Para detectar input (temporal)

#define VICTORIA 1
#define CONTINUA -1
#define DERROTA 0



typedef struct
{
    tJugador jugador;
    tVector fantasmas;
} tEntidades;

typedef struct
{
    unsigned fila;
    unsigned columna;
}tPosicion;

int empezarJuego();
void registroMov(tJugador *jugador, tVector *fantasmas);
int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion * config);
short int actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado);
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades);
void volverYDescontar(tJugador * jugador);
unsigned short esFinPartida (tJugador * jugador);
unsigned short chequeoSalida (tJugador * jugador, tLaberinto * laberinto);
tPosicion obtenerPosFantasma(tFantasma *fantasma);
tPosicion obtenerPosJugador(tJugador *jugador);

#endif // JUEGO_H_INCLUDED
