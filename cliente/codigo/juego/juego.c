#include "juego.h"

int empezarJuego()
{
    unsigned char juegoTerminado = FALSO;
    char estado;
    tLaberinto laberinto;
    tEntidades entidades;
    tConfiguracion configuracion;

    // Cargamos el archivo de configuración
    if (!cargarArchivoConfiguracion(&configuracion))
        return ERROR;

    // Cargamos laberinto de un .txt
    // if (!crearLaberintoArchivo(&laberinto))
    //     return ERROR;

    // Inicializo semilla random
    srand((unsigned)time(NULL));

    // Generamos un laberinto aleatorio
    if (!crearLaberintoAleatorio(&laberinto, &configuracion))
    {
        puts("ERROR: Hubo un error al crear el laberinto aleatorio");
        return ERROR;
    }

    // Inicializar el vector de fantasmas
    if (!crearVector(&entidades.fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(&laberinto);
        puts("ERROR: No hay memoria para crear los fantasmas");
        return ERROR;
    }

    // Leemos el laberinto y lo interpretamos para generar todo
    if (!procesarEntidades(&laberinto, &entidades, &configuracion))
    {
        destruirLaberinto(&laberinto);
        vaciarVector(&entidades.fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        return ERROR;
    }

    // Bucle principal
    while (!juegoTerminado)
    {
        system("cls");

        estado = actualizarJuego(&laberinto, &entidades, &juegoTerminado);
        dibujarJuego(&laberinto, &entidades);

        /*
        if (estado == VICTORIA)
        {
            // Esperar entrada | Mostar menú transición
            llenarLaberinto();
            vaciarVector();
            procesarEntidades();
        }
        */

        Sleep(300);
    }

    mostrarMovimientos(&entidades.jugador, &entidades.fantasmas);

    if (estado == DERROTA)
        submenuDerrota(&entidades.jugador);

    destruirLaberinto(&laberinto);
    vaciarVector(&entidades.fantasmas);

    return EXITO;
}

char actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado)
{
    char teclaApretada;
    char direccionJugador = NO_DIRECCION;
    size_t cantFantasmas = obtenerLongitudVector(&entidades->fantasmas);
    char mov;
    int i;
    tPosicion pos;
    tFantasma* fantasma;

    if (!_kbhit()) // Si no se apretó nada, no perdemos tiempo en calcular nada
        return CONTINUA;

    teclaApretada = _getch();

    if (teclaApretada == SALIR_DEL_JUEGO) // Si se va a cerrar el juego, tampoco deberíamos calcular nada más
    {
        *juegoTerminado = VERDADERO;
        return CONTINUA;
    }

    // Determinamos la dirección del jugador
    if (TECLA_ABAJO(teclaApretada))
        direccionJugador = ABAJO;
    else if (TECLA_ARRIBA(teclaApretada))
        direccionJugador = ARRIBA;
    else if (TECLA_IZQUIERDA(teclaApretada))
        direccionJugador = IZQUIERDA;
    else if (TECLA_DERECHA(teclaApretada))
        direccionJugador = DERECHA;

    // Si no se mueve el jugador, no hay que calcular nada
    if (moverJugador(&entidades->jugador, direccionJugador, laberinto) == FALSO)
        return CONTINUA;

    pos = obtenerPosJugador(&entidades->jugador);
    ponerEncola(&entidades->jugador.colaMovimientos, &pos, sizeof(pos));

    // Primer chequeo de fantasmas, post movimiento jugador
    if (chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
    {
        volverYDescontar(&entidades->jugador);

        if (jugadorSinVidas(&entidades->jugador))
        {
            *juegoTerminado = VERDADERO;
            return DERROTA;
        }
    }

    for (i = 0; i < cantFantasmas; i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas,i);
        mov = calcularMovimientoFantasma(fantasma,laberinto,&entidades->jugador);
        pos = obtenerPosFantasma(fantasma);

        if (fantasma->tocado == FALSO)
        {
            ponerEncola(&fantasma->colaMovimientos, &pos, sizeof(pos));
            moverFantasma(fantasma, mov, laberinto);
        }
    }

    if (jugadorEnSalida(&entidades->jugador, laberinto))
    {
        *juegoTerminado = VERDADERO; // Esto es temporal, hay que sacarlo
        // Acá debería de generarse un laberinto aleatorio nuevo (o volver a cargar el que ya está si es un .txt)
        return VICTORIA;
    }

    if (chequeoPremio(&entidades->jugador, laberinto))
    {
        sumarPuntaje(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    if (chequeoVida(&entidades->jugador, laberinto))
    {
        sumarVida(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    // Segundo chequeo de fantasma, post movimiento de los mismos
    if(chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
    {
        volverYDescontar(&entidades->jugador);

        if (jugadorSinVidas(&entidades->jugador))
        {
            *juegoTerminado = VERDADERO;
            return DERROTA;
        }
    }

    return CONTINUA;
}

void mostrarMovimientos(tJugador* jugador, tVector* fantasmas)
{
    size_t cantFantasmas = obtenerLongitudVector(fantasmas);
    tFantasma* fantasma;

    puts("\nMovimientos del jugador:");
    mostrarCola(&jugador->colaMovimientos, imprimirPosicion);

    puts("\n");

    for (int i = 0; i < cantFantasmas; i++)
    {
        fantasma = obtenerElementoVector(fantasmas,i);
        printf("Movimientos del fantasma %d:\n", i + 1);
        mostrarCola(&fantasma->colaMovimientos, imprimirPosicion);
        puts("\n");
    }
}

int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    char casilla;
    int jugadorEncontrado = FALSO;
    tFantasma fantasmaAux;

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            casilla = obtenerCasillaLaberinto(laberinto, i, j);

            switch (casilla)
            {
            case ENTRADA:
                if (jugadorEncontrado) // Por si hay más de un jugador en el laberinto
                    break;

                crearJugador(&entidades->jugador, configuracion, i, j);
                jugadorEncontrado = VERDADERO;
                break;

            case FANTASMA:
                crearFantasma(&fantasmaAux, i, j);
                insertarVector(&entidades->fantasmas, &fantasmaAux);
                modificarCasillaLaberinto(laberinto, i, j, CAMINO); // Sacamos la 'F' para que no moleste
                break;
            }
        }
    }

    return jugadorEncontrado == VERDADERO;
}

void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    tFantasma* fantasmaEncontrado;
    int indiceFantasmaEncontrado = -1;

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            // Primero el jugador
            if (entidades->jugador.posActual.fila == i && entidades->jugador.posActual.columna == j)
            {
                dibujarJugador(&entidades->jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            indiceFantasmaEncontrado = buscarFantasmaEnPosicion(&entidades->fantasmas, i, j);

            if (indiceFantasmaEncontrado != -1)
            {
                fantasmaEncontrado = (tFantasma*)obtenerElementoVector(&entidades->fantasmas, indiceFantasmaEncontrado);
                dibujarFantasma(fantasmaEncontrado, i, j);
                continue;
            }

            // Dibujamos el resto del laberinto
            printf("%c", obtenerCasillaLaberinto(laberinto, i, j));
        }

        puts("");
    }

    puts("");
    mostrarVidasYPuntos(&entidades->jugador);
}

void imprimirPosicion(const void *p)
{
    tPosicion* pos = (tPosicion*)p;

    printf("(%zu, %zu)\t", pos->fila, pos->columna);
}
