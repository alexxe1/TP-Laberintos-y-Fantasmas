#include "laberinto.h"

int crearLaberintoAleatorio(tLaberinto* laberinto, tConfiguracion* configuracion)
{
    size_t i,j, fantasmaX, fantasmaY, premioX, premioY, VidaX, vidaY;
    size_t filas = configuracion->filas;
    size_t columnas = configuracion->columnas;
    size_t cant;

    size_t entradaFila = 0;                                     //Primera Fila
    size_t entradaColumna = 1 + rand() % (columnas - 2);        //Cualquier Columna menos primera o ultima

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

    ///Se coloca la entrada
    laberinto->casillas[entradaFila][entradaColumna] = ENTRADA;
    laberinto->casillas[entradaFila+1][entradaColumna] = CAMINO;

    ///Genero laberinto a partir de la Entrada con algoritmo Prim
    generarPrim(laberinto, entradaFila + 1, entradaColumna);

    ///Genero loops y vueltas
    agregarLoops(laberinto, PORCENTAJE_LOOPS);

    ///Coloco salida garantizada con algoritmo Breadth-First Search
    salidaBFS(laberinto, entradaFila + 1, entradaColumna);

    ///Generar fantasmas
    cant = 0;
    if(configuracion->maxFantasmas > 0)
        cant = 1 + rand() % configuracion->maxFantasmas;

    for(i=0;i < cant;i++)
    {
        do
        {
            fantasmaX = filas / 2 + rand() % (filas / 2);               //Evito spawn kill
            fantasmaY = rand() % columnas;
        }while(laberinto->casillas[fantasmaX][fantasmaY] != CAMINO);
        laberinto->casillas[fantasmaX][fantasmaY] = FANTASMA;
    }

    ///Generar premios
    cant = 0;
    if(configuracion->maxPremios > 0)
        cant = 1 + rand() % configuracion->maxPremios;
    for(i=0;i < cant; i++)
    {
        do
        {
            premioX = rand() % filas;
            premioY = rand() % columnas;
        }while(laberinto->casillas[premioX][premioY] != CAMINO);
        laberinto->casillas[premioX][premioY] = PREMIO;
    }

    ///Generar vidas extra
    cant = 0;
    if(configuracion->maxVidasExtras > 0)
        cant = 1 + rand() % configuracion->maxVidasExtras;
    for(i=0;i < cant;i++)
    {
        do
        {
            VidaX = rand() % filas;
            vidaY = rand() % columnas;
        }while(laberinto->casillas[VidaX][vidaY] != CAMINO);
        laberinto->casillas[VidaX][vidaY] = VIDA_EXTRA;
    }

    guardarLaberintoArchivo(laberinto);

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
    {
        fclose(pLab);
        return ERROR;
    }

    for(i=0;i<cFilas;i++)
    {
        laberinto->casillas[i] = malloc(cColumnas * sizeof(char));

        if(!laberinto->casillas[i])
        {
            for(j=0;j<i;j++)
                free(laberinto->casillas[j]);

            free(laberinto->casillas);

            fclose(pLab);
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

void generarPrim(tLaberinto* laberinto, int entradaFila, int entradaColumna)
{
    size_t filas = laberinto->filas;
    size_t columnas = laberinto->columnas;
    int i, nx, ny, idx, dx[4], dy[4], vecinos;
    int cantParedes, paredLista[2*filas*columnas];

    // Inicializo la celda de entrada
    laberinto->casillas[entradaFila][entradaColumna] = CAMINO;

    // Inicializo el array de desplazamiento
    dx[0]=1; dx[1]=-1; dx[2]=0; dx[3]=0;
    dy[0]=0; dy[1]=0; dy[2]=1; dy[3]= -1;

    // Lista de paredes (fila y columna alternadas)
    cantParedes = 0;

    // Agrego paredes adyacentes a la entrada
    for(i=0;i<4;i++)
    {
        nx = entradaFila + dx[i];
        ny = entradaColumna + dy[i];
        if(nx>=1 && nx<(int)filas-1 && ny>=1 && ny<(int)columnas-1 && laberinto->casillas[nx][ny]==PARED)
        {
            paredLista[cantParedes++] = nx;
            paredLista[cantParedes++] = ny;
        }
    }

    // Bucle principal
    while(cantParedes>0)
    {
        // Elegir pared al azar
        idx = (rand() % (cantParedes/2))*2;
        nx = paredLista[idx];
        ny = paredLista[idx+1];

        // Saco la pared de la lista
        paredLista[idx] = paredLista[cantParedes-2];
        paredLista[idx+1] = paredLista[cantParedes-1];
        cantParedes -= 2;

        // Contar vecinos que ya son camino
        vecinos = 0;
        for(i=0;i<4;i++)
        {
            int ex = nx + dx[i];
            int ey = ny + dy[i];
            if(laberinto->casillas[ex][ey]==CAMINO) vecinos++;
        }

        if(vecinos==1)
        {
            laberinto->casillas[nx][ny]=CAMINO;

            // Agregar paredes adyacentes
            for(i=0;i<4;i++)
            {
                int ex = nx + dx[i];
                int ey = ny + dy[i];
                if(ex>=1 && ex<(int)filas-1 && ey>=1 && ey<(int)columnas-1 && laberinto->casillas[ex][ey]==PARED)
                {
                    paredLista[cantParedes++] = ex;
                    paredLista[cantParedes++] = ey;
                }
            }
        }
    }
}

void agregarLoops(tLaberinto* laberinto, int probabilidad)
{
    size_t i, j, filas, columnas;
    int vecinos;

    filas = laberinto->filas;
    columnas = laberinto->columnas;

    for(i=1;i<filas - 1;i++)
    {
        for(j=1;j<columnas - 1;j++)
        {
            if(laberinto->casillas[i][j] == PARED)
            {
                vecinos = 0;
                if(laberinto->casillas[i+1][j] == CAMINO)
                    vecinos++;
                if(laberinto->casillas[i-1][j] == CAMINO)
                    vecinos++;
                if(laberinto->casillas[i][j+1] == CAMINO)
                    vecinos++;
                if(laberinto->casillas[i][j-1] == CAMINO)
                    vecinos++;

                if(vecinos >= 2 && (rand() % 100) < probabilidad)
                    laberinto->casillas[i][j] = CAMINO;
            }
        }
    }
}

void salidaBFS(tLaberinto* laberinto, size_t entradaFila, size_t entradaColumna)
{
    int *dist, *visitado, *cola;
    int dx[4], dy[4];
    int salidaX, salidaY, maxDist, i, frente, fin, indiceDx, fila, col, d, nuevaX, nuevaY, nuevoInDx;
    size_t filas, columnas;

    filas = laberinto->filas;
    columnas = laberinto->columnas;

    dist = (int*)malloc(filas * columnas * sizeof(int));
    visitado = (int*)malloc(filas * columnas * sizeof(int));
    cola = (int*)malloc(filas * columnas * sizeof(int));

    if(!dist || !visitado || !cola)
    {
        free(dist);
        free(visitado);
        free(cola);
        return;
    }

    for(i=0;i<(int)(filas * columnas);i++)
    {
        dist[i] = -1;
        visitado[i] = 0;
    }

    dx[0] = 1;
    dx[1] = -1;
    dx[2] = 0;
    dx[3] = 0;

    dy[0] = 0;
    dy[1] = 0;
    dy[2] = 1;
    dy[3] = -1;

    frente = 0;
    fin = 0;

    cola[fin++] = entradaFila * columnas + entradaColumna;
    dist[entradaFila * columnas + entradaColumna] = 0;
    visitado[entradaFila * columnas + entradaColumna] = 1;

    maxDist = 0;
    salidaX = entradaFila;
    salidaY = entradaColumna;

    while(frente < fin)
    {
        indiceDx = cola[frente++];
        fila = indiceDx / columnas;
        col = indiceDx % columnas;
        d = dist[indiceDx];

        if(d > maxDist)
        {
            maxDist = d;
            salidaX = fila;
            salidaY = col;
        }

        for(i=0;i<4;i++)
        {
            nuevaX = fila + dx[i];
            nuevaY = col + dy[i];
            nuevoInDx = nuevaX * columnas + nuevaY;

            if(nuevaX >= 0 && nuevaX < (int)filas && nuevaY >= 0 && nuevaY < (int)columnas && laberinto->casillas[nuevaX][nuevaY] == CAMINO && !visitado[nuevoInDx])
            {
                cola[fin++] = nuevoInDx;
                dist[nuevoInDx] = d + 1;
                visitado[nuevoInDx] = 1;
            }
        }
    }

    laberinto->casillas[salidaX][salidaY] = SALIDA;

    free(dist);
    free(visitado);
    free(cola);
}

int guardarLaberintoArchivo(tLaberinto* laberinto)
{
    FILE* pf = fopen("LaberintoGuardado.txt", "wt");
    int i,j;

    if(!pf)
        return ERROR;

    for(i=0;i<laberinto->filas;i++)
    {
        for(j=0;j<laberinto->columnas;j++)
            fprintf(pf, "%c", laberinto->casillas[i][j]);
        fprintf(pf, "\n");
    }

    fclose(pf);

    return EXITO;
}
