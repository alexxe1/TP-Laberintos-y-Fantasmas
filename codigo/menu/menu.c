#include "menu.h"

void mostrarMenu()
{
    // Acá deberían estar las opciones (ahora no importa)

    empezarJuego();
}

unsigned short submenuDerrota (tJugador * jugador)
{
    unsigned short opcion = 0; // 0 = primera opción, 1 = segunda opción
    int tecla;

    while (1)
    {
        system("cls"); // Limpia la pantalla

        puts("+--------------------------------------------------------+");
        printf("| FIN DE LA PARTIDA!                                     |\n");
        printf("| JUGADOR: PH                                            |\n");
        printf("| NIVEL ALCANZADO: PH                                    |\n");
        printf("| PUNTAJE FINAL: %-10d                              |\n", (int)jugador->puntajeTotal);
        puts("|                                                        |");
        puts("| Desea comenzar nuevamente o volver al menu principal?  |");
        printf("| Usa las flechas para moverte y Enter para elegir       |\n");
        puts("|                                                        |");
        printf("| %s COMENZAR NUEVAMENTE                                  |\n", opcion == 0 ? ">" : " ");
        printf("| %s VOLVER AL MENU PRINCIPAL                             |\n", opcion == 1 ? ">" : " ");
        puts("+--------------------------------------------------------+");
        tecla = _getch();

        if (tecla == 0 || tecla == 224) // Tecla especial (flechas)
        {
            tecla = _getch(); // Obtener código real

            if (tecla == ARR && opcion > 0)
                opcion--;
            else if (tecla == ABA && opcion < 1)
                opcion++;
        }
        else if (tecla == TECLA_ENTER)
            return opcion; // Devuelve 0 o 1 según lo elegido

    }
}

