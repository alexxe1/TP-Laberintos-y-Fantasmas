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
    size_t i, j;
    size_t contado = 0;


    if(!pLab)
        return ERROR;

    while(fgets(linea, sizeof(linea), pLab))    //Cuento filas y columnas
    {
        plinea = linea;

        while(*plinea != '\n' && *plinea != '\0' && !contado)
        {
            cColumnas++;
            plinea++;
        }

        cFilas++;
        contado = 1;
    }

    fclose(pLab);

    pLab = fopen(ARCHIVO_LABERINTO, "rt");      //Reinicio el archivo
    if(!pLab)
        return ERROR;

    laberinto->casillas = malloc(cFilas * sizeof(char*));   //Creo matriz Laberinto
    if(!laberinto->casillas)
        return ERROR;

    for(i=0;i<cFilas;i++)
    {
        laberinto->casillas[i] = malloc(cColumnas * sizeof(char));
        if(!laberinto->casillas[i])
        {
            for(j=0;j<i;j++)
                free(laberinto->casillas[j]);
            free(laberinto->casillas);

            return ERROR;
        }
    }
    laberinto->filas = cFilas;
    laberinto->columnas = cColumnas;

    cFilas = 0;

    while(fgets(linea, sizeof(linea), pLab))
    {
        plinea = linea;
        cColumnas = 0;

        while(*plinea != '\n' && *plinea != '\0')
        {
            modificarCasillaLaberinto(laberinto, cFilas, cColumnas, *plinea);
            cColumnas++;
            plinea++;
        }
        cFilas++;
    }

    fclose(pLab);
    return EXITO;
}

void destruirLaberinto(tLaberinto* laberinto)
{
    size_t i;

    for(i=0;i<laberinto->filas;i++)
        free(laberinto->casillas[i]);
    free(laberinto->casillas);

    laberinto->columnas = 0;
    laberinto->filas = 0;
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
        return CAMINO;

    return laberinto->casillas[fila][columna];
}

void modificarCasillaLaberinto(tLaberinto* laberinto, size_t fila, size_t columna, char nuevaCasilla)
{
    if (fila >= laberinto->filas || columna >= laberinto->columnas)
        return;

    laberinto->casillas[fila][columna] = nuevaCasilla;
}
