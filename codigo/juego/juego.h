#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "laberinto.h"
#include "jugador.h"
#include "fantasma.h"
#include "configuracion.h"
#include "../estructuras/vector.h"
#include "../estructuras/cola.h"

#include <stdio.h> // Para imprimir por consola (temporal)
#include <windows.h> // Para limpiar la consola (temporal)
#include <conio.h> // Para detectar input (temporal)

typedef struct
{
    tJugador jugador;
    tVector fantasmas;
} tEntidades;

int empezarJuego();
int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades);
int actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado);
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades);
int hayFantasma(tVector* vecFantasmas, size_t fila, size_t columna);

#endif // JUEGO_H_INCLUDED
