#include "laberinto.h"

void crearLaberintoAleatorio(tLaberinto* laberinto, tConfiguracion* configuracion)
{
    // Para hacer
    // La configuración está en tConfiguracion
}

int crearLaberintoArchivo(tLaberinto* laberinto)
{
    FILE* pLab = fopen(ARCHIVO_LABERINTO, "rt");
    char linea[TAM_LINEA];
    char* plinea;
    size_t cFilas = 0;
    size_t cColumnas = 0;

    if(!pLab)
        return ERROR;

    while(fgets(linea, sizeof(linea), pLab))
    {
        plinea = linea;
        cColumnas = 0;

        while(*plinea != '\n' && *plinea != '\0')
        {
            laberinto->casillas[cFilas][cColumnas] = *plinea;
            cColumnas++;
            plinea++;
        }

        cFilas++;
    }

    laberinto->filas = cFilas;
    laberinto->columnas = cColumnas;

    fclose(pLab);
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
