#include "fantasma.h"

void crearFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    fantasma->posInicial.fila = fila;
    fantasma->posInicial.columna = columna;

    fantasma->posActual.fila = fila;
    fantasma->posActual.columna = columna;

    fantasma->ultimoMovimiento = -1;
    fantasma->tocado = FALSO;

    crearCola(&fantasma->colaMovimientos);
}

void destruirFantasma(tFantasma* fantasma)
{
    vaciarCola(&fantasma->colaMovimientos);
}

int buscarFantasmaEnPosicion(tVector* vecFantasmas, size_t fila, size_t columna)
{
    size_t i;
    size_t cantFantasmas = obtenerLongitudVector(vecFantasmas);
    tFantasma* fantasma;

    for (i = 0; i < cantFantasmas; i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if(!fantasma->tocado && fantasma->posActual.fila == fila && fantasma->posActual.columna == columna)
        {
            return i;
        }
    }

    return -1;
}

void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    printf("%c", FANTASMA);
}

char calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador)
{
    int i, bloqueadas, pos = -1;
    char mov, opuesto = -1;
    int distMin = MAX_DIST;
    size_t nuevaFila = fantasma->posActual.fila;
    size_t nuevaColumna = fantasma->posActual.columna;
    int dist;

    // Matriz que dicta las posiciones disponibles
    int movimiento[4][2] =
    {
        {-1,0},
        {1,0},
        {0,-1},
        {0,1}
    };

    // Con esto voy a ver si tiene pared o no
    int nuevaPos[4][2] =
    {
        {-1,-1},
        {-1,-1},
        {-1,-1},
        {-1,-1}
    };

    // Reviso cada posicion de la matriz para ver a donde se mueve y veo si tiene pared o se pasa del limite del mapa
    // Si no tiene nada malo me fijo en la posicion en la que estuvo y despues calculo si era derecha, izquierda, etc

    opuesto = calculaOpuesto(fantasma->ultimoMovimiento);
    bloqueadas = 0;

    for(i = 0; i < 4; i++)
    {
        nuevaFila = fantasma->posActual.fila + movimiento[i][0];
        nuevaColumna = fantasma->posActual.columna + movimiento[i][1];

        if(nuevaFila > 0 &&
                nuevaFila < laberinto->filas &&
                nuevaColumna > 0 &&
                nuevaColumna < laberinto->columnas &&
                laberinto->casillas[nuevaFila][nuevaColumna] != '#')
        {
            nuevaPos[i][0] = nuevaFila;
            nuevaPos[i][1] = nuevaColumna;
        }
        else
        {
            bloqueadas++;
        }
    }

    // Busco la menor distancia entre los validos, en caso de que solo tenga una posicion libre
    // para moverse se va a permitir moverse a la posicion opuesta para evitar bug de fantasma

    for (i = 0; i < 4; i++)
    {
        if (nuevaPos[i][0] != -1  && (detectarMov(movimiento,i) != opuesto || bloqueadas == 3))
        {
            dist = abs(nuevaPos[i][0] - jugador->posActual.fila) + abs(nuevaPos[i][1] - jugador->posActual.columna);

            if (dist < distMin)
            {
                distMin = dist;
                pos = i;
            }

        }
    }

    if (pos == -1)
    {
        return FALSO;
    }

    if (nuevaPos[pos][0] > fantasma->posActual.fila)
    {
        mov = ABAJO;
    }
    else if (nuevaPos[pos][0] < fantasma->posActual.fila)
    {
        mov = ARRIBA;
    }
    else if (nuevaPos[pos][1] > fantasma->posActual.columna)
    {
        mov = DERECHA;
    }
    else if (nuevaPos[pos][1] < fantasma->posActual.columna)
    {
        mov = IZQUIERDA;
    }
    if (bloqueadas == 4)
    {
        mov = calculaOpuesto(mov);
    }

    fantasma->ultimoMovimiento = mov;

    return mov;
}

char detectarMov(int mov[][2], int pos)
{
    if (pos == 0)
        return ARRIBA;

    if (pos == 1)
        return ABAJO;

    if (pos == 2)
        return IZQUIERDA;

    if (pos == 4)
        return DERECHA;

    return FALSO;
}

char calculaOpuesto(const char c)
{
    if (c == ARRIBA)
        return ABAJO;

    if (c == ABAJO)
        return ARRIBA;

    if (c == DERECHA)
        return IZQUIERDA;

    if (c == IZQUIERDA)
        return DERECHA;

    return FALSO;
}

char moverFantasma(tFantasma* fantasma, char direccion, const tLaberinto* laberinto)
{
    size_t nuevaFila = fantasma->posActual.fila;
    size_t nuevaColumna = fantasma->posActual.columna;

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
    fantasma->posActual.fila = nuevaFila;
    fantasma->posActual.columna = nuevaColumna;

    return VERDADERO;
}

char chequeoFantasma(tVector* vecFantasmas, tJugador* jugador)
{
    size_t i;
    size_t cantFantasmas = obtenerLongitudVector(vecFantasmas);
    tFantasma* fantasma;

    for (i = 0; i < cantFantasmas; i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if (!fantasma->tocado && fantasma->posActual.fila == jugador->posActual.fila && fantasma->posActual.columna == jugador->posActual.columna)
        {
            fantasma->tocado = VERDADERO;
            return VERDADERO;
        }
    }

    return FALSO;
}

tPosicion obtenerPosFantasma(tFantasma *fantasma)
{
    tPosicion pos;

    pos.fila = fantasma->posActual.fila;
    pos.columna = fantasma->posActual.columna;

    return pos;
}
