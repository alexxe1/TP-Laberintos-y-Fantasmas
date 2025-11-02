#include "cliente_red.h"

int iniciarConexion()
{
    WSADATA wsa;

    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET conectarseAlServidor(const char* ip, int puerto)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
        return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(puerto);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

char intentarConectarServidor(SOCKET* socket, const char* ip, int puerto)
{
    unsigned short opcion = FALSO; // 0 = reintentar, 1 = continuar sin conexión
    int tecla;

    while (1)
    {
        *socket = conectarseAlServidor(ip, puerto);

        if (*socket != INVALID_SOCKET)
        {
            puts("Conectado con exito al servidor!");
            Sleep(1000);
            return EXITO; // conexión exitosa
        }

        while (1)
        {
            system("cls");
            puts("+--------------------------------------------------------+");
            puts("| ERROR AL CONECTARSE AL SERVIDOR                        |");
            puts("|                                                        |");
            puts("| No se pudo establecer conexion.                        |");
            puts("| Usa las flechas para moverte y Enter para elegir:      |");
            puts("|                                                        |");
            printf("| %s REINTENTAR CONEXION                                  |\n", opcion == FALSO ? ">" : " ");
            printf("| %s CONTINUAR SIN CONEXION                               |\n", opcion == VERDADERO ? ">" : " ");
            puts("+--------------------------------------------------------+");

            tecla = _getch();

            if (tecla == 0 || tecla == 224) // Flechas
            {
                tecla = _getch();
                if (tecla == ARR && opcion > FALSO)
                    opcion--;
                else if (tecla == ABA && opcion < VERDADERO)
                    opcion++;
            }
            else if (tecla == TECLA_ENTER)
            {
                if (opcion == FALSO)
                {
                    system("cls");
                    puts("Reintentando conexion...");
                    break;
                }
                else
                    return ERROR; // continuar sin conexión
            }
        }
    }
}

void cerrarConexion(SOCKET* socket)
{
    closesocket(*socket);
    WSACleanup();
}

// Devuelve EXITO o ERROR según se haya o no podido obtener los rankings del servidor
char solicitarRankingsServidor(SOCKET* socket, tLista* listaRankings)
{
    int cantidadRankings;
    int bytesRecibidos;
    int i;
    int recibidos;
    const char* peticion = "RANKING";
    char* ptr;
    tRanking ranking;

    // Acá le decimos al servidor que vamos a querer los rankings
    if (send(*socket, peticion, strlen(peticion), 0) == SOCKET_ERROR)
        return ERROR;

    // Nos dice cuantos bytes recibimos y la cantidad de rankings
    bytesRecibidos = recv(*socket, (char*)&cantidadRankings, sizeof(int), 0);

    if (bytesRecibidos <= 0)
        return ERROR;

    // Procesamos cada ranking porque 'recv' no siempre da toda la información de una
    for (i = 0; i < cantidadRankings; i++)
    {
        recibidos = 0;
        ptr = (char*)&ranking;

        while (recibidos < sizeof(tRanking))
        {
            int r = recv(*socket, ptr + recibidos, sizeof(tRanking) - recibidos, 0);

            if (r <= 0)
                return ERROR;

            recibidos += r;
        }

        ponerAlFinal(listaRankings, &ranking, sizeof(tRanking));
    }

    return EXITO;
}

// Devuelve EXITO o ERROR según se pudo o no dar de alta a un jugador en el servidor
char darAltaJugadorServidor(SOCKET* socket, char* nombreJugador)
{
    char peticion[128];
    char rta;
    int bytesRecibidos;

    snprintf(peticion, sizeof(peticion), "REGISTRAR %s", nombreJugador);

    if(send(*socket, peticion, strlen(peticion), 0) == SOCKET_ERROR)
        return ERROR;

    bytesRecibidos = recv(*socket, &rta, sizeof(char), 0);

    if(bytesRecibidos <= 0)
        return ERROR;

    return rta == 1 ? EXITO : ERROR;
}

// Devuelve EXITO o ERROR según se pudo o no mandar datos de la partida al servidor
char mandarDatosPartidaServidor(SOCKET* socket, char* nombreJugador, size_t puntajeTotal, size_t cantMovimientos)
{
    char peticion[128];
    char rta;
    int bytesRecibidos;

    sprintf(peticion, "GUARDAR %s %lu %lu", nombreJugador, (unsigned long)puntajeTotal, (unsigned long)cantMovimientos);

    if(send(*socket, peticion, strlen(peticion), 0) == SOCKET_ERROR)
        return ERROR;

    bytesRecibidos = recv(*socket, &rta, sizeof(char), 0);

    if(bytesRecibidos <= 0)
        return ERROR;

    return rta == 1 ? EXITO : ERROR;
}
