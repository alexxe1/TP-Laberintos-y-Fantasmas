#include "fantasma.h"

void crearFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    fantasma->filaInicial = fila;
    fantasma->columnaInicial = columna;

    fantasma->filaActual = fantasma->filaInicial;
    fantasma->columnaActual = fantasma->columnaInicial;

    fantasma->ultMov = -1;

    crearCola(&fantasma->cola);

    fantasma->tocado = FALSO;

}

void dibujarFantasma(tFantasma* fantasma, size_t fila, size_t columna)
{
    printf("%c", FANTASMA);
}

// Si se necesita, agregar más parametros
char calcularMovimientoFantasma(tFantasma* fantasma, const tLaberinto* laberinto, const tJugador* jugador)
{
    //matriz que dice en que posicion se moveria
    int i,bloqueadas, pos = -1;
    char mov,opuesto = -1;
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

    opuesto = CalculaOpuesto(fantasma->ultMov);

    bloqueadas = 0;

    for(i = 0; i < 4; i++)
    {
        nuevaFila = fantasma->filaActual + movimiento[i][0];
        nuevaColumna = fantasma->columnaActual + movimiento[i][1];

        if(nuevaFila > 0 && nuevaFila < laberinto->filas && nuevaColumna > 0 &&
                nuevaColumna < laberinto->columnas && laberinto->casillas[nuevaFila][nuevaColumna] != '#')
        {
            nuevaPos[i][0] = nuevaFila;
            nuevaPos[i][1] = nuevaColumna;
        }else
        {
            bloqueadas++;
        }
    }

    //busco la menor distancia entre los validos, en caso de que solo tenga una posicion libre para moverse se va a permitir
    //moverse a la posicion opuesta para evitar bug de fantasma
    for(i = 0; i < 4; i++)
    {
        if(nuevaPos[i][0] != -1  && (detectarMov(movimiento,i) != opuesto || bloqueadas == 3))
        {
            dist = abs(nuevaPos[i][0] - jugador->filaActual) + abs(nuevaPos[i][1] - jugador->columnaActual);

            if(dist < distMin)
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


    if (nuevaPos[pos][0] > fantasma->filaActual)
    {
        mov =  ABAJO;
    }
    else if (nuevaPos[pos][0] < fantasma->filaActual)
    {
        mov = ARRIBA;
    }
    else if (nuevaPos[pos][1] > fantasma->columnaActual)
    {
        mov = DERECHA;
    }
    else if (nuevaPos[pos][1] < fantasma->columnaActual)
    {
        mov = IZQUIERDA;
    }
    if(bloqueadas == 4)
    {
        mov = CalculaOpuesto(mov);
    }
    fantasma->ultMov = mov;

    return mov;
}

char detectarMov(int mov[][2], int pos)
{
    switch(pos)
    {
    case 0:
        return ARRIBA;
        break;
    case 1:
        return ABAJO;
        break;
    case 2:
        return IZQUIERDA;
        break;
    case 4:
        return DERECHA;
        break;
    default:
        return FALSO;
    }
}

char CalculaOpuesto(const char c)
{
    char opuesto;

    switch(c) {
    case ARRIBA:
        opuesto = ABAJO;
        break;
    case ABAJO:
        opuesto = ARRIBA;
        break;
    case DERECHA:
        opuesto = IZQUIERDA;
        break;
    case IZQUIERDA:
        opuesto = DERECHA;
        break;
    default:
        opuesto = FALSO;
        break;
    }
    return opuesto;
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
