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

void calcularMovimientoFantasma(tFantasma* fantasma, const tJugador* jugador)
{
    // Acá se tendría que calcular el siguiente movimiento del fantasma según la posición del jugador
}

// Las direcciones están dadas por MACROS en fantasma.h
void moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto)
{

}
