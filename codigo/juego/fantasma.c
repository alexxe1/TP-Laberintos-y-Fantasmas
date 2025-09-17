#include "fantasma.h"

void crearFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    fantasma->filaInicial = fila;
    fantasma->columnaInicial = columna;

    fantasma->filaActual = fantasma->filaInicial;
    fantasma->columnaActual = fantasma->columnaInicial;
}

void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    printf("%c", FANTASMA);
}

// Si se necesita, agregar más parametros
int calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador)
{
    // Acá se tendría que calcular el siguiente movimiento del fantasma según la posición del jugador
    int nuevaFila = fantasma->filaActual;
    int nuevaColumna = fantasma->columnaActual;

    if((nuevaFila - 1) > 0 && fantasma->filaActual > jugador->filaActual)
    {
        nuevaFila--;
    }
    else if((nuevaFila + 1) < laberinto->filas)
    {
        nuevaFila++;
    }

    if((nuevaColumna - 1) > 0 && fantasma->columnaActual > jugador->columnaActual)
    {
        nuevaColumna--;
    }
    else if((nuevaFila + 1) < laberinto->columnas)
    {
        nuevaColumna++;
    }
    // No permitir moverse si hay una pared


    // Si después de verificar todo, no hay problema, nos movemos
    int distFila = abs(jugador->filaActual - nuevaFila);
    int distCol = abs(jugador->columnaActual - nuevaColumna);

    if(distFila > distCol)
    {
        if(nuevaFila > fantasma->filaActual)
        {
            return ARRIBA;
        }
        else
        {
            return ABAJO;
        }
    }
    else
    {
        if(nuevaColumna > fantasma->columnaActual)
        {
            return DERECHA;
        }else
        {
            return IZQUIERDA;
        }

    }
}

// Las direcciones están dadas por MACROS en controles.h
int moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto)
{

    size_t nuevaFila = fantasma->filaActual;
    size_t nuevaColumna = fantasma->columnaActual;

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
    fantasma->filaActual = nuevaFila;
    fantasma->columnaActual = nuevaColumna;

    return VERDADERO;
}
