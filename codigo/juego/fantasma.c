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
void calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador)
{
    // Acá se tendría que calcular el siguiente movimiento del fantasma según la posición del jugador
}

// Las direcciones están dadas por MACROS en controles.h
void moverFantasma(tFantasma* fantasma, tJugador *jugador, const tLaberinto* laberinto)
{
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
        if(laberinto->casillas[nuevaFila][fantasma->columnaActual] = '#')
        {

        }
        fantasma->filaActual = nuevaFila;
    }

    else
        fantasma->columnaActual = nuevaColumna;

}
