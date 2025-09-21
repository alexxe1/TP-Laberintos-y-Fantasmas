#include "fantasma.h"

void crearFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    fantasma->filaInicial = fila;
    fantasma->columnaInicial = columna;

    fantasma->filaActual = fantasma->filaInicial;
    fantasma->columnaActual = fantasma->columnaInicial;
    crearCola(&fantasma->cola);

}

void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    printf("%c", FANTASMA);
}

// Si se necesita, agregar más parametros
char calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador)
{
    //matriz que dice en que posicion se moveria
    int i, pos = -1;
    int distMin = MAX_DIST;
    int movimiento[4][2] =
    {
        {-1,0},
        {1,0},
        {0,-1},
        {0,1}
    };
    //con esto voy a ver si tiene pared o no
    int nuevaPos[4][2] =
    {
        {-1,-1},
        {-1,-1},
        {-1,-1},
        {-1,-1}
    };

    int nuevaFila = fantasma->filaActual;
    int nuevaColumna = fantasma->columnaActual;
    int dist;
    //reviso cada posicion de la matriz para ver a donde se mueve y veo si tiene pared o se pasa del limite del mapa
    //si no tiene nada malo me fijo en la posicion en la que estuvo y despues calculo si era derecha, izq etc
    for(i = 0; i < 4; i++)
    {
        nuevaFila = fantasma->filaActual + movimiento[i][0];
        nuevaColumna = fantasma->columnaActual + movimiento[i][1];

        if(nuevaFila > 0 && nuevaFila < laberinto->filas && nuevaColumna > 0 &&
                nuevaColumna < laberinto->columnas && laberinto->casillas[nuevaFila][nuevaColumna] != '#')
        {
            nuevaPos[i][0] = nuevaFila;
            nuevaPos[i][1] = nuevaColumna;
        }
    }

    for(i = 0; i < 4; i++)
    {
        if(nuevaPos[i][0] != -1)
        {
            dist = abs(nuevaPos[i][0] - jugador->filaActual) + abs(nuevaPos[i][1] - jugador->columnaActual);

            if(dist < distMin)
            {
                distMin = dist;
                pos = i;
            }

        }
    }
    if (pos == -1) {

             return FALSO; }


    if (nuevaPos[pos][0] > fantasma->filaActual) return ABAJO;
    if (nuevaPos[pos][0] < fantasma->filaActual) return ARRIBA;
    if (nuevaPos[pos][1] > fantasma->columnaActual) return DERECHA;
    if (nuevaPos[pos][1] < fantasma->columnaActual) return IZQUIERDA;

    return FALSO;
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
