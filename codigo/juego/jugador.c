#include "jugador.h"

void crearJugador(tJugador* jugador, tConfiguracion * config, size_t fila, size_t columna)
{
    jugador->posInicial.fila = fila;
    jugador->posInicial.columna = columna;

    jugador->posActual.fila = fila;
    jugador->posActual.columna = columna;

    crearCola(&jugador->cola);

    jugador->puntajeTotal = 0;
    jugador->vidas = config->vidasIniciales;
}

void dibujarJugador(tJugador* jugador, size_t fila, size_t columna)
{
    printf("%c", JUGADOR);
}

int moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto)
{
    size_t nuevaFila = jugador->posActual.fila;
    size_t nuevaColumna = jugador->posActual.columna;

    switch (direccion)
    {
    case IZQUIERDA:
        if (nuevaColumna > 0)
            nuevaColumna--;
        break;

    case DERECHA:
        if (nuevaColumna < laberinto->columnas - 1)
            nuevaColumna++;
        break;

    case ARRIBA:
        if (nuevaFila > 0)
            nuevaFila--;
        break;

    case ABAJO:
        if (nuevaFila < laberinto->filas - 1)
            nuevaFila++;
        break;

    default: // No nos movemos si no hay dirección
        return FALSO;
    }

    // No permitir moverse si hay una pared
    if (laberinto->casillas[nuevaFila][nuevaColumna] == '#')
        return FALSO;

    // Si después de verificar todo, no hay problema, nos movemos
    jugador->posActual.fila = nuevaFila;
    jugador->posActual.columna = nuevaColumna;

    return VERDADERO;
}

tPosicion obtenerPosJugador(tJugador *jugador)
{
    tPosicion pos;

    pos.fila = jugador->posActual.fila;
    pos.columna = jugador->posActual.columna;

    return pos;

}
