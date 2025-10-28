#include "premios_y_vidas.h"

void mostrarVidasYPuntos(tJugador *jugador)
{
    puts("+------------------------------+");
    printf("| VIDAS RESTANTES: %-10d  |\n", (int)jugador->vidas);
    printf("| PUNTAJE TOTAL: %-10d    |\n", (int)jugador->puntajeTotal);
    puts("+------------------------------+");
}

char chequeoPremio(tJugador* jugador, tLaberinto* laberinto)
{
    char casilla = obtenerCasillaLaberinto(laberinto, jugador->posActual.fila, jugador->posActual.columna);

    return casilla == PREMIO ? VERDADERO : FALSO;
}

void sumarPuntaje(tJugador* jugador)
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
    char casilla = obtenerCasillaLaberinto(laberinto, jugador->posActual.fila, jugador->posActual.columna);

    return casilla == VIDA_EXTRA ? VERDADERO : FALSO;
}

void sumarVida(tJugador* jugador)
{
    if (jugador->vidas < TOPE_VIDAS)
        jugador->vidas++;
}


