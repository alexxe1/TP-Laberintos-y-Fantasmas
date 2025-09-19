#include "fantasma.h"

void crearFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    fantasma->filaInicial = fila;
    fantasma->columnaInicial = columna;

    fantasma->filaActual = fantasma->filaInicial;
    fantasma->columnaActual = fantasma->columnaInicial;
    fantasma->tocado = FALSO;
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
void moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto)
{

}

unsigned short chequeoFantasma (tVector* vecFantasmas, tJugador * jugador)
{
  size_t i;
  tFantasma* fantasma;
  /// Podria haber problemas si dos fantasmas te tocan al mismo tiempo (Solucionable con break o bandera)

    for (i = 0; i < obtenerLongitudVector(vecFantasmas); i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if (fantasma->filaActual == jugador->filaActual && fantasma->columnaActual == jugador->columnaActual &&
            !fantasma->tocado)
        {
         fantasma->tocado = VERDADERO;
         return VERDADERO;
        }
    }
  return FALSO;
}
