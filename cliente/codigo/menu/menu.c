#include "menu.h"

int mostrarMenuPrincipal()
{
    int opcion = 0;  // índice actual (0 a 3)
    int tecla;

    while (VERDADERO)
    {
        system("cls"); // Limpia la pantalla

        puts("+--------------------------------------------------------+");
        printf("|               LABERINTOS Y FANTASMAS                   |\n");
        printf("|--------------------------------------------------------|\n");
        printf("| Usa las flechas para moverte y Enter para elegir!      |\n");
        puts("|                                                        |");
        printf("| %s NUEVA PARTIDA                                        |\n", opcion == 0 ? ">" : " ");
        printf("| %s VER RANKINGS                                         |\n", opcion == 1 ? ">" : " ");
        printf("| %s REGLAS DEL JUEGO                                     |\n", opcion == 2 ? ">" : " ");
        printf("| %s SALIR DEL JUEGO                                      |\n", opcion == 3 ? ">" : " ");
        puts("+--------------------------------------------------------+");

        tecla = _getch();

        if (tecla == 0 || tecla == 224) // Tecla especial (flechas)
        {
            tecla = _getch(); // Obtener código real

            if (tecla == ARR && opcion > 0)
                opcion--;
            else if (tecla == ABA && opcion < 3)
                opcion++;
        }
        else if (tecla == TECLA_ENTER)
        {
            return opcion; // Devuelve 0..3 según lo elegido
        }
    }
}

void submenuDerrota(tJugador* jugador, unsigned nivel)
{
    system("cls"); // Limpia la pantalla
    puts("+--------------------------------------------------------+");
    printf("| FIN DE LA PARTIDA!                                     |\n");
    printf("| JUGADOR: %-10s                                    |\n", jugador->nombre);
    printf("| NIVEL ALCANZADO: %-10u                            |\n", nivel);
    printf("| PUNTAJE FINAL: %-10d                              |\n", (int)jugador->puntajeTotal);
    puts("+--------------------------------------------------------+");
    puts("\nApreta cualquier tecla para continuar...");
    getch();
}

char submenuTransicion (tJugador * jugador, unsigned nivel)
{
    unsigned short opcion = FALSO; // 0 = primera opción, 1 = segunda opción
    int tecla;

    while (1)
    {
        system("cls"); // Limpia la pantalla

        puts("+--------------------------------------------------------+");
        printf("| NIVEL COMPLETO!                                        |\n");
        printf("| JUGADOR: %-10s                                    |\n", jugador->nombre);
        printf("| NIVEL ACTUAL: %-10u                               |\n", nivel);
        printf("| PUNTAJE ACTUAL: %-10d                             |\n", (int)jugador->puntajeTotal);
        printf("| VIDAS RESTANTES: %-10d                            |\n", (int)jugador->vidas);
        puts("|                                                        |");
        puts("| Continuar o Volver al menu principal?                  |");
        printf("| Usa las flechas para moverte y Enter para elegir       |\n");
        puts("|                                                        |");
        printf("| %s SIGUIENTE NIVEL                                      |\n", opcion == FALSO ? ">" : " ");
        printf("| %s VOLVER AL MENU PRINCIPAL                             |\n", opcion == VERDADERO ? ">" : " ");
        puts("+--------------------------------------------------------+");
        tecla = _getch();

        if (tecla == 0 || tecla == 224) // Tecla especial (flechas)
        {
            tecla = _getch(); // Obtener código real

            if (tecla == ARR && opcion > FALSO)
                opcion--;
            else if (tecla == ABA && opcion < VERDADERO)
                opcion++;
        }
        else if (tecla == TECLA_ENTER)
            return opcion; // Devuelve 0 o 1 según lo elegido
    }
}

char generarArchivoReglas(const char * pathArch)
{
    FILE *archivo = fopen(pathArch, "wt");

    if (!archivo)
        return ERROR;

    fprintf(archivo,
            "REGLAS DEL JUEGO - LABERINTOS Y FANTASMAS\n"
            "------------------------------------------\n\n"
            "Objetivo:\n"
            "Recorre el laberinto desde la entrada (E) hasta la salida (S) evitando a los fantasmas (F).\n\n"
            "Controles:\n"
            "- Usa las teclas de direccion (W, A, S, D) para moverte por el laberinto.\n\n"
            "Reglas basicas:\n"
            "1. Comienzas con un numero limitado de vidas.\n"
            "2. Si un fantasma te alcanza, pierdes una vida y regresas a la entrada. El fantasma desaparece del laberinto.\n"
            "3. Puedes recolectar premios (P) que otorgan puntos.\n"
            "4. Puedes recolectar vidas extra (V) para aumentar tus vidas disponibles.\n"
            "5. Si tus vidas llegan a 0, la partida termina.\n\n"
            "Condiciones de victoria:\n"
            "- Llega hasta la salida (S) sin quedarte sin vidas.\n\n"
            "Puntuacion:\n"
            "- Cada premio (P) recolectado suma un punto al marcador.\n"
            "- Se registran el nombre del jugador, la puntuacion final y la cantidad de movimientos.\n\n"
            "Buena suerte atravesando el laberinto!\n"
           );

    fclose(archivo);
    return EXITO;
}

char verReglas(const char * pathArch)
{
    FILE * pf;
    int car;

    pf = fopen (pathArch, "rt");
    if (!pf)
    {
        printf("Error");
        return ERROR;
    }


    while ( (car = fgetc(pf)) != EOF)
        putchar(car);

    fclose(pf);


    system("pause");
    system("cls");

    return EXITO;
}

char verRankings(SOCKET* socket)
{
    tLista listaRankings;
    tRanking rankingAux;
    size_t posicion = 1;

    system("cls");
    puts("Obteniendo rankings...");

    if (*socket == INVALID_SOCKET && intentarConectarServidor(socket, IP_SERVER, PUERTO) == ERROR)
        return ERROR;

    crearLista(&listaRankings);

    if (solicitarRankingsServidor(socket, &listaRankings) == ERROR)
    {
        system("cls");
        puts("Ocurrio un error al obtener los rankings. Presiona cualquier tecla para continuar...");
        vaciarLista(&listaRankings);
        _getch();
        return ERROR;
    }

    system("cls");

    printf("+----------+----------------------+------------+\n");
    printf("| %-8s | %-20s | %-10s |\n", "POSICION", "NOMBRE", "PUNTOS");
    printf("+----------+----------------------+------------+\n");

    while (sacarPrimeroLista(&listaRankings, &rankingAux, sizeof(tRanking)) == 1)
    {
        printf("| %-8lu | %-20s | %-10lu |\n",
               (unsigned long)posicion,
               rankingAux.nombre,
               (unsigned long)rankingAux.puntos);
        posicion++;
    }

    printf("+----------+----------------------+------------+\n");
    puts("\nPresiona cualquier tecla para volver...");
    _getch();


    vaciarLista(&listaRankings);

    return EXITO;
}



