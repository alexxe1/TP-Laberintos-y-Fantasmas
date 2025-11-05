#include "codigo/menu/menu.h"

int main()
{
    SOCKET socket;
    unsigned finProceso = FALSO;
    int opcion;

    if (iniciarConexion() != 0)
    {
        printf("Error al inicializar Winsock!\n");
        return ERROR;
    }

    puts("Intentado conectar al servidor...\n");

    // Si devolvió ERROR es porque el usuario no se pudo conectar al servidor
    intentarConectarServidor(&socket, IP_SERVER, PUERTO);

    while (!finProceso)
    {
        opcion = mostrarMenuPrincipal();

        switch (opcion)
        {
        case 0:
            empezarJuego(&socket);
            break;
        case 1:
            verRankings(&socket);
            break;
        case 2:
            verReglas();
            break;
        case 3:
            finProceso = VERDADERO;
            break;
        }
    }

    puts("\nGracias por jugar! - Hecho por God of C - 2025");

    cerrarConexion(&socket);

    return 0;
}
