#include "jugador.h"

void crearJugador(tJugador* jugador, size_t fila, size_t columna)
{
    jugador->fila = fila;
    jugador->columna = columna;
}

void moverJugador(tJugador* jugador, char direccion, tLaberinto* laberinto)
{
    size_t nuevaFila = jugador->fila;
    size_t nuevaColumna = jugador->columna;

    // Detectar dirección y moverse
    if (TECLA_ARRIBA(direccion) && nuevaFila > 0) // Arriba
        nuevaFila--;
    else if (TECLA_IZQUIERDA(direccion) && nuevaColumna > 0) // Izquierda
        nuevaColumna--;
    else if (TECLA_DERECHA(direccion) && nuevaColumna < laberinto->columnas - 1) // Derecha
        nuevaColumna++;
    else if (TECLA_ABAJO(direccion) && nuevaFila < laberinto->filas - 1) // Abajo
        nuevaFila++;

    // No permitir moverse si hay una pared
    if (laberinto->casillas[nuevaFila][nuevaColumna] == '#')
        return;

    // Si después de verificar todo, no hay problema, nos movemos
    jugador->fila = nuevaFila;
    jugador->columna = nuevaColumna;
}
