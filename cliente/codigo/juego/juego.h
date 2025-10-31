#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"
#include "entidades.h"
#include "fantasma.h"
#include "jugador.h"
#include "configuracion.h"
#include "premios_y_vidas.h"
#include "cliente_red.h"
#include "../estructuras/vector.h"
#include "../estructuras/cola.h"

#include "../menu/menu.h"

#include <stdio.h> // Para imprimir por consola (temporal)
#include <windows.h> // Para limpiar la consola (temporal)
#include <conio.h> // Para detectar input (temporal)

#define VICTORIA 1
#define CONTINUA 0
#define DERROTA -1

int empezarJuego();
char actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado);
size_t mostrarMovimientos(tJugador* jugador);
int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, const char* nombJug, unsigned iteracion);
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades);
char chequeoSalida(tJugador* jugador, tLaberinto* laberinto);
void imprimirPosicion(const void* p);
char continuarJugando (tLaberinto * laberinto, tConfiguracion * configuracion, tEntidades * entidades, unsigned nivel);
char procesarMovimientos(tEntidades* entidades, tLaberinto* laberinto, unsigned char* juegoTerminado);

#endif // JUEGO_H_INCLUDED
