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
    //matriz que dice en que posicion se moveria
    int movimiento[4][2] =
    {
        {-1,0},
        {1,0},
        {0,-1},
        {0,1}
    };
    //con esto voy a ver si tiene pared o no
    int validas[] = {0,0,0,0};

    int nuevaFila = fantasma->filaActual;
    int nuevaColumna = fantasma->columnaActual;

    //reviso cada posicion de la matriz para ver a donde se mueve y veo si tiene pared o se pasa del limite del mapa
    //si no tiene nada malo me fijo en la posicion en la que estuvo y despues calculo si era derecha, izq etc
    for(int i = 0; i < 4; i++)
    {
        nuevaFila = fantasma->filaActual + movimiento[i][0];
        nuevaColumna = fantasma->columnaActual + movimiento[i][1];

        if(nuevaFila >= 0 && nuevaFila <= laberinto->filas && nuevaColumna >= 0 &&
           nuevaColumna <= laberinto->columnas && laberinto->casillas[nuevaFila][nuevaColumna] != '#')
        {
            validas[i] = 1;
        }
    }
    // No permitir moverse si hay una pared


    // Si después de verificar todo, no hay problema, nos movemos

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
