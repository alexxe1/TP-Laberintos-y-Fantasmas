#include "juego.h"

int empezarJuego()
{
    unsigned char juegoTerminado = FALSO;
    tLaberinto laberinto;
    tEntidades entidades;

    // Generamos un laberinto (o lo cargamos de un .txt)
    if (!crearLaberintoArchivo(&laberinto, ARCHIVO_LABERINTO))
        return ERROR;

    // Inicializar el vector de fantasmas
    if (!crearVector(&entidades.fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(&laberinto);
        return ERROR;
    }

    // Leemos el laberinto y lo interpretamos para generar todo
    if (!procesarEntidades(&laberinto, &entidades))
    {
        destruirLaberinto(&laberinto);
        destruirVector(&entidades.fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        return ERROR;
    }

    // Bucle principal
    while (!juegoTerminado)
    {
        system("cls");

        actualizarJuego(&laberinto, &entidades, &juegoTerminado);
        dibujarJuego(&laberinto, &entidades);

        Sleep(100);
    }

    destruirLaberinto(&laberinto);
    destruirVector(&entidades.fantasmas);

    return EXITO;
}

int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades)
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

                    crearJugador(&entidades->jugador, i, j);
                    jugadorEncontrado = VERDADERO;
                    break;
                case FANTASMA:
                    crearFantasma(&fantasmaAux, i, j);
                    insertarVector(&entidades->fantasmas, &fantasmaAux);
                    break;
            }
        }
    }

    return jugadorEncontrado == VERDADERO;
}

void actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado)
{
    char teclaApretada;
    char direccionJugador;

    // Acá habría que hacer que espere a que el jugador toque una tecla para mover al personaje

    if (_kbhit()) // _kbhit() ya viene con 'conio.h' y se traduce en keyboard hit
    {
        teclaApretada = _getch();

        if (teclaApretada == 'q')
            *juegoTerminado = VERDADERO;
        else
        {
            if (TECLA_ABAJO(teclaApretada))
                direccionJugador = ABAJO;
            else if (TECLA_ARRIBA(teclaApretada))
                direccionJugador = ARRIBA;
            else if (TECLA_IZQUIERDA(teclaApretada))
                direccionJugador = IZQUIERDA;
            else if (TECLA_DERECHA(teclaApretada))
                direccionJugador = DERECHA;

            // Solo si el jugador se mueve, los fantasmas también
            // Acá en realidad habría que apilar el movimiento del jugador y el de los fantasmas y luego mover
            if (moverJugador(&entidades->jugador, direccionJugador, laberinto) == VERDADERO)
            {
                // Acá hay que hacer que se muevan los fantasmas

                // Para hacer: Comprobar si el jugador tocó un fantasma, vida extra o premio.
            }
        }
    }
}

void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            // Primero el jugador
            if (entidades->jugador.filaActual == i && entidades->jugador.columnaActual == j)
            {
                dibujarJugador(&entidades->jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            if (hayFantasma(&entidades->fantasmas, i, j) == VERDADERO)
                continue;

            // Dibujamos el resto del laberinto
            printf("%c", obtenerCasillaLaberinto(laberinto, i, j));
        }

        puts("");
    }
}

int hayFantasma(tVector* vecFantasmas, size_t fila, size_t columna)
{
    size_t i;
    tFantasma* fantasma;

    for (i = 0; i < obtenerLongitudVector(vecFantasmas); i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if (fantasma->filaActual == fila && fantasma->columnaActual == columna)
        {
            dibujarFantasma(fantasma, fila, columna);
            return VERDADERO; // Salimos antes porque ya se dibujó un fantasma
        }
    }

    return FALSO;
}
