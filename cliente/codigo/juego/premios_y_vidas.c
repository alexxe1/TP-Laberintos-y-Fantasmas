#include "premios_y_vidas.h"

void mostrarVidasYPuntos(tJugador *jugador)
{
    puts("+------------------------------+");
    printf("| JUGADOR: %-10s          |\n", jugador->nombre);
    printf("| VIDAS RESTANTES: %-10d  |\n", (int)jugador->vidas);
    printf("| PUNTAJE TOTAL: %-10d    |\n", (int)jugador->puntajeTotal);
    puts("+------------------------------+");
}

char chequeoPremio(tJugador* jugador, tLaberinto* laberinto)
{
    return (laberinto->casillas[jugador->posActual.fila][jugador->posActual.columna] == PREMIO ? VERDADERO : FALSO);
}

void sumarPuntaje(tJugador* jugador, tLaberinto* laberinto)
{
    unsigned short opcion;

    opcion = 1 + (rand() % OPCION_LIMITE);

    if (opcion >= MIN_BAJO && opcion <= MAX_BAJO)
        jugador->puntajeTotal += PUNTAJE_BAJO;
    else if (opcion >= MIN_MEDIO && opcion <= MAX_MEDIO)
        jugador->puntajeTotal += PUNTAJE_MEDIO;
    else
        jugador->puntajeTotal += PUNTAJE_ALTO;
}

char chequeoVida(tJugador* jugador, tLaberinto* laberinto)
{
    return (laberinto->casillas[jugador->posActual.fila][jugador->posActual.columna] == VIDA_EXTRA ? VERDADERO : FALSO);
}

void sumarVida(tJugador* jugador)
{
    if (jugador->vidas < TOPE_VIDAS)
        jugador->vidas++;
}


