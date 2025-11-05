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
#define SIN_MOVIMIENTO 0
#define CONTINUA 2
#define DERROTA -1

// Inicialización y manejo de estados
int empezarJuego(SOCKET* socket);
int inicializarConfiguracion(tConfiguracion* configuracion);
int inicializarJuegoBase(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, size_t iteracion);
void pedirNombreJugador(char* nombre);
int conectarJugadorServidor(SOCKET* socket, tEntidades* entidades);
void manejarVictoria(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, size_t* iteracion, unsigned char* juegoTerminado);
void manejarFinDeJuego(SOCKET* socket, tLaberinto* laberinto, tEntidades* entidades, char dioAltaJugador);

// Bucles principales
char actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado);
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades);

// Procesamiento de datos
int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, const char* nombJug, unsigned iteracion);
char procesarMovimientos(tEntidades* entidades, tLaberinto* laberinto, unsigned char* juegoTerminado);
size_t mostrarMovimientos(tJugador* jugador);

// Comprobaciones
char chequeoSalida(tJugador* jugador, tLaberinto* laberinto);
char continuarJugando(tLaberinto* laberinto, tConfiguracion* configuracion, tEntidades* entidades, unsigned nivel);

void destruirColasYVectores(tEntidades* entidades);

#endif // JUEGO_H_INCLUDED
