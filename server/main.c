#include "servidor.h"

typedef void(*tAccion)(void *a);

unsigned short recorrerArchBin (const char * pathArch, unsigned esize, tAccion accion);

void imprimirIdxJugador(void *a)
{
    tIdxJugador *idx = (tIdxJugador*)a;

    printf("--- Indice Jugador ---\n");
    printf("  ID:           %d\n", idx->id);
    printf("  Nombre:       %s\n", idx->nombre);
    printf("  Desplazamiento: %d bytes\n", idx->desp);
    printf("----------------------\n");
}

void imprimirJugador(void *a)
{
    tJugador *jug = (tJugador*)a;
    printf("------ Jugador ------\n");
    printf("  ID:     %d\n", jug->id);
    printf("  Nombre: %s\n", jug->nombre);
    printf("---------------------\n");
}

void imprimirPartida(void *a)
{
    tPartida *partida = (tPartida*)a;

    printf("------- Partida -------\n");
    printf("  ID Partida:     %d\n", partida->idPartida);
    printf("  ID Jugador:     %d\n", partida->idJugador);
    printf("  Puntuacion:     %d puntos\n", partida->puntuacion);
    printf("  Cant. Movs:     %d\n", partida->cantMovs);
    printf("-----------------------\n");
}

int main()
{
    tArbol arbol;
    char respuesta[128];

    int result;

    crearArbol(&arbol);
    //creamos indice y despues generamos el arbol balanceado, si no se pudo generar el indice porque todavia no jugo nadie y no se creo
    //el archivo usuario entonces no generamos el arbol
    result = crearArchIdx(&arbol, NOMBRE_ARCH_USUARIOS, NOMBRE_ARCH_INDICE, sizeof(tJugador),sizeof(tIdxJugador),crearIdx, cmpIdx);

    if(result == TODO_OK)
    {
        cargarDesdeArchOrdenadoArbol(&arbol,sizeof(tIdxJugador), NOMBRE_ARCH_INDICE, cmpIdx);
    }
    printf("\nARBOLLL");
    mostrarEnOrden(&arbol, (const void*)imprimirIdxJugador);

    const char *comandos[] = {
        "REGISTRAR juan",
        "REGISTRAR ana",
        "REGISTRAR juan",
        "GUARDAR juan 100 25",
        "GUARDAR ana 80 30",
        "GUARDAR pedro 120 40",
        "REGISTRAR alexei",
        NULL
    };

    for (int i = 0; comandos[i]; i++)
    {
        procesarEntrada(comandos[i], respuesta, &arbol, cmpIdx);
        printf("Entrada: %-25s → Respuesta: %s\n", comandos[i], respuesta);
    }

    recorrerArchBin(NOMBRE_ARCH_USUARIOS, sizeof(tJugador), imprimirJugador);
    recorrerArchBin(NOMBRE_ARCH_PARTIDAS, sizeof(tPartida), imprimirPartida);
    recorrerArchBin(NOMBRE_ARCH_INDICE, sizeof(tIdxJugador),imprimirIdxJugador);

    return 0;
}

unsigned short recorrerArchBin (const char * pathArch, unsigned esize, tAccion accion)
{
    FILE * pf;
    void * buffer;

    pf = fopen(pathArch, "rb");
    if (!pf)
        return ERR_ARCH;

    buffer = malloc(esize);
    if (!buffer)
    {
        fclose(pf);
        return ERR_ARCH;
    }

    fread(buffer, esize, 1, pf);
    while (!feof(pf))
    {
        accion(buffer);
        fread(buffer, esize, 1, pf);
    }

    free(buffer);
    fclose(pf);
    return EXITO;
}


