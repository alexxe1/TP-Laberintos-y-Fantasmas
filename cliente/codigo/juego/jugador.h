#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include "controles.h"
#include "laberinto.h"
#include "../estructuras/cola.h"
#include "juego.h"

void crearJugador(tJugador* jugador, const char* nombre, tConfiguracion* configuracion, size_t fila, size_t columna);
void acomodarJugador (tJugador* jugador, size_t fila, size_t columna);
void destruirJugador(tJugador* jugador);

void dibujarJugador(tJugador* jugador, size_t fila, size_t columna);
char moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto);

tPosicion obtenerPosJugador(tJugador* jugador);

// Ingreso y validación de nombre
void ingresarNombre (char * cadena, unsigned longitud);
char esNombreValido (const char * nombre);
char esLetra (int car);
char esNumero(int car);
char esEspacio(int car);

void volverYDescontar(tJugador* jugador);
char jugadorSinVidas(tJugador* jugador);
char jugadorEnSalida(tJugador* jugador, tLaberinto* laberinto);

#endif // JUGADOR_H_INCLUDED
