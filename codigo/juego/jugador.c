#include "jugador.h"

void crearJugador(tJugador* jugador, size_t fila, size_t columna)
{
    jugador->filaInicial = fila;
    jugador->columnaInicial = columna;

    jugador->filaActual = jugador->filaInicial;
    jugador->columnaActual = jugador->columnaInicial;
}

void dibujarJugador(tJugador* jugador, size_t fila, size_t columna)
{
    printf("%c", JUGADOR);
}

int moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto)
{
    size_t nuevaFila = jugador->filaActual;
    size_t nuevaColumna = jugador->columnaActual;

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
    jugador->filaActual = nuevaFila;
    jugador->columnaActual = nuevaColumna;

    return VERDADERO;
}
