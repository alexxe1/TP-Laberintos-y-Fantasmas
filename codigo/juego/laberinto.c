#include "laberinto.h"

void crearLaberintoAleatorio(tLaberinto* laberinto)
{
    // Para hacer
}

int crearLaberintoArchivo(tLaberinto* laberinto, char* nombreArchivo)
{
    // Para hacer. Ahora solo cargamos uno fijo para probar. Debe ser dinámico luego.
    size_t i, j;

    laberinto->filas = 20;
    laberinto->columnas = 20;

    for (i = 0; i < laberinto->filas; i++)
    {
        for (j = 0; j < laberinto->columnas; j++)
        {
            laberinto->casillas[i][j] = CAMINO;
        }
    }

    return EXITO;
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

char obtenerCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna)
{
    if (fila > laberinto->filas || columna > laberinto->columnas)
    {
        return CAMINO;
    }

    return laberinto->casillas[fila][columna];
}

void modificarCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna, char nuevaCasilla)
{
    if (fila > laberinto->filas || columna > laberinto->columnas)
    {
        return;
    }

    laberinto->casillas[fila][columna] = nuevaCasilla;
}
