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

    crearLaberinto(&laberinto, filasLaberinto, columnasLaberinto);
    crearJugador(&jugador, 0, 0);

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
        dibujarLaberinto(&laberinto, &jugador);

        Sleep(100);
    }

    destruirLaberinto(&laberinto);
}

void actualizarLaberinto(tLaberinto* laberinto)
{

}

void dibujarLaberinto(tLaberinto* laberinto, tJugador* jugador)
{
    size_t i, j;

    for (i = 0; i < obtenerFilasLaberinto(laberinto); i++)
    {
        for (j = 0; j < obtenerColumnasLaberinto(laberinto); j++)
        {
            if (i == jugador->fila && j == jugador->columna) // Dibujar al jugador (es un objeto separado del laberinto)
                printf("%c", JUGADOR);
            else
                printf("%c", *(obtenerCasillaLaberinto(laberinto, i, j))); // Dibujar el laberinto
        }
        puts("");
    }
}
