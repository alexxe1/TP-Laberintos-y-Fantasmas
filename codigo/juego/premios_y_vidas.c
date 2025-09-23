#include "premios_y_vidas.h"

void vidasYPuntos(tJugador *jugador)
{
    puts("+------------------------------+");
    printf("| VIDAS RESTANTES: %-10d  |\n", (int)jugador->vidas);
    printf("| PUNTAJE TOTAL: %-10d    |\n", (int)jugador->puntajeTotal);
    puts("+------------------------------+");
}

unsigned short chequeoPremio (tJugador * jugador, tLaberinto * laberinto)
{
    return (laberinto->casillas[jugador->filaActual][jugador->columnaActual] == PREMIO ? VERDADERO : FALSO);
}

void sumarPuntaje (tJugador * jugador, tLaberinto * laberinto)
{
    unsigned short opcion;

    opcion = 1 + (rand() % OPCION_LIMITE);

    if (opcion >= MIN_LOW && opcion <= MAX_LOW)
        jugador->puntajeTotal += PUNTAJE_LOW;
    else if (opcion >= MIN_MID && opcion <= MAX_MID)
        jugador->puntajeTotal += PUNTAJE_MID;
    else
        jugador->puntajeTotal += PUNTAJE_HIGH;

}

void ponerCamino (tJugador * jugador, tLaberinto * laberinto)
{
    laberinto->casillas[jugador->filaActual][jugador->columnaActual] = CAMINO;
}

unsigned short chequeoVida (tJugador * jugador, tLaberinto * laberinto)
{
    return (laberinto->casillas[jugador->filaActual][jugador->columnaActual] == VIDA_EXTRA ? VERDADERO : FALSO);
}

void sumarVida (tJugador * jugador)
{
 if (jugador->vidas < TOPE_VIDAS)
   jugador->vidas++;
}


