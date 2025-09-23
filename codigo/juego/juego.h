#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"
#include "jugador.h"
#include "fantasma.h"
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
tPosicion obtenerPosJugador(tJugador *jugador);
tPosicion obtenerPosFantasma(tFantasma *fantasma);
int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion * config);
short int actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado);
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades);
int hayFantasma(tVector* vecFantasmas, size_t fila, size_t columna);
void volverYDescontar(tJugador * jugador);
unsigned short esFinPartida (tJugador * jugador);
unsigned short chequeoSalida (tJugador * jugador, tLaberinto * laberinto);


#endif // JUEGO_H_INCLUDED
