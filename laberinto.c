#include "laberinto.h"

void crearLaberinto(tLaberinto* laberinto, size_t filas, size_t columnas)
{
    size_t i, j;

    laberinto->filas = filas;
    laberinto->columnas = columnas;

    for (i = 0; i < laberinto->filas; i++)
    {
        for (j = 0; j < laberinto->columnas; j++)
        {
            laberinto->casillas[i][j] = CAMINO;
        }
    }
}

void destruirLaberinto(tLaberinto* laberinto)
{
    // Todavía no hay nada que destruir porque no se reserva memoria
}

size_t obtenerFilasLaberinto(tLaberinto* laberinto)
{
    return laberinto->filas;
}

size_t obtenerColumnasLaberinto(tLaberinto* laberinto)
{
    return laberinto->columnas;
}

char* obtenerCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna)
{
    if (fila > laberinto->filas || columna > laberinto->columnas)
    {
        return NULL;
    }

    return &(laberinto->casillas[fila][columna]);
}
