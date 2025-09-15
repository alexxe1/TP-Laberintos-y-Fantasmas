#include "laberinto.h"

int crearLaberintoAleatorio(tLaberinto* laberinto, tConfiguracion* configuracion)
{
    size_t i,j, fx, fy, px, py, vx, vy;
    size_t filas = configuracion->filas;
    size_t columnas = configuracion->columnas;

    size_t entradaFila = 0;                         //Primera Fila
    size_t entradaColumna = rand() % columnas;      //Cualquier Columna
    size_t salidaFila = filas - 1;                  //Ultima Fila
    size_t salidaColumna = rand() % columnas;       //Cualquier Columna

    ///Creo matriz laberinto
    laberinto->casillas = malloc(filas * sizeof(char*));
    if(!laberinto->casillas)
        return ERROR;

    for(i=0;i<filas;i++)
    {
        laberinto->casillas[i] = malloc(columnas * sizeof(char));
        if(!laberinto->casillas[i])
        {
            for(j=0;j<i;j++)
                free(laberinto->casillas[j]);
            free(laberinto->casillas);
            return ERROR;
        }
    }
    laberinto->filas = filas;
    laberinto->columnas = columnas;

    ///Inicializo todo con paredes
    for(i=0;i<filas;i++)
        for(j=0;j<columnas;j++)
            laberinto->casillas[i][j] = PARED;

    ///Camino aleatorio
    i = entradaFila;
    j = entradaColumna;

    while(i != salidaFila || j != salidaColumna)
    {
        laberinto->casillas[i][j] = CAMINO;

        if(i != salidaFila && j != salidaColumna)
        {
            if(rand() % 2)
                i += (salidaFila > i) ? 1 : -1;
            else
                j += (salidaColumna > j) ? 1 : -1;
        }
        else if(i != salidaFila)
            i += (salidaFila > i) ? 1 : -1;
        else if(j != salidaColumna)
        j += (salidaColumna > j) ? 1 : -1;
    }
    laberinto->casillas[i][j] = CAMINO;

    ///Generar fantasmas
    for(i=0;i < configuracion->maxFantasmas;i++)
    {
        do
        {
            fx = rand() % filas;
            fy = rand() % columnas;
        }while(laberinto->casillas[fx][fy] != CAMINO);
        laberinto->casillas[fx][fy] = FANTASMA;
    }

    ///Generar premios
    for(i=0;i < configuracion->maxPremios; i++)
    {
        do
        {
            px = rand() % filas;
            py = rand() % columnas;
        }while(laberinto->casillas[px][py] != CAMINO);
        laberinto->casillas[px][py] = PREMIO;
    }

    ///Generar vidas extra
    for(i=0;i < configuracion->maxVidasExtras;i++)
    {
        do
        {
            vx = rand() % filas;
            vy = rand() % columnas;
        }while(laberinto->casillas[vx][vy] != CAMINO);
        laberinto->casillas[vx][vy] = VIDA_EXTRA;
    }

    laberinto->casillas[entradaFila][entradaColumna] = ENTRADA;
    laberinto->casillas[entradaFila+1][entradaColumna] = CAMINO;
    laberinto->casillas[salidaFila][salidaColumna] = SALIDA;

    return EXITO;
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

    ///Cuento filas y columnas
    while(fgets(linea, sizeof(linea), pLab))
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

    ///Reinicio archivo
    pLab = fopen(ARCHIVO_LABERINTO, "rt");
    if(!pLab)
        return ERROR;

    ///Creo matriz laberinto
    laberinto->casillas = malloc(cFilas * sizeof(char*));
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

    ///Cargo laberinto
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
