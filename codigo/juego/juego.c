#include "juego.h"

void empezarJuego()
{
    unsigned char juegoTerminado = 0;

    // Estos parámetros deberían venir por el .txt
    size_t filasLaberinto = 20;
    size_t columnasLaberinto = 20;

    // Creación de los TDA
    tLaberinto laberinto;
    tJugador jugador;
    tFantasma fantasmas[3];
    tVector vecFantasmas;

    crearLaberinto(&laberinto, filasLaberinto, columnasLaberinto);
    crearJugador(&jugador, 0, 0);
    crearVector(&vecFantasmas, sizeof(tFantasma));

    // 3 fantasmas para probar
    crearFantasma(&fantasmas[0], 5, 5);
    crearFantasma(&fantasmas[1], 10, 10);
    crearFantasma(&fantasmas[2], 15, 15);

    insertarVector(&vecFantasmas, &fantasmas[0]);
    insertarVector(&vecFantasmas, &fantasmas[1]);
    insertarVector(&vecFantasmas, &fantasmas[2]);

    while (!juegoTerminado)
    {
        system("cls");

        if (_kbhit()) // _kbhit() ya viene con 'conio.h' y se traduce en keyboard hit
        {
            char tecla = _getch(); // Lo defino acá porque es temporal, con SDL es distinto

            if (tecla == 'q')
                juegoTerminado = 1;
            else
                moverJugador(&jugador, tecla, &laberinto);
        }

        actualizarLaberinto(&laberinto);
        dibujarLaberinto(&laberinto, &jugador, &vecFantasmas);

        Sleep(100);
    }

    destruirLaberinto(&laberinto);
}

void actualizarLaberinto(tLaberinto* laberinto)
{

}

void dibujarLaberinto(tLaberinto* laberinto, tJugador* jugador, tVector* vecFantasmas)
{
    size_t i, j;

    for (i = 0; i < obtenerFilasLaberinto(laberinto); i++)
    {
        for (j = 0; j < obtenerColumnasLaberinto(laberinto); j++)
        {
            // Primero el jugador
            if (jugador->fila == i && jugador->columna == j)
            {
                dibujarJugador(jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            if (recorrerFantasmas(vecFantasmas, i, j))
                continue;

            // Dibujamos el resto del laberinto
            printf("%c", *(obtenerCasillaLaberinto(laberinto, i, j)));
        }

        puts("");
    }
}

int recorrerFantasmas(tVector* vecFantasmas, size_t fila, size_t columna)
{
    size_t i;
    tFantasma* fantasma;

    for (i = 0; i < obtenerLongitudVector(vecFantasmas); i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if (fantasma->filaActual == fila && fantasma->columnaActual == columna)
        {
            dibujarFantasma(fantasma, fila, columna);
            return 1; // Salimos antes porque ya se dibujó un fantasma
        }
    }

    return 0;
}
