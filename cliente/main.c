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

    printf("Intentado conectar al servidor...");

    // Si devolvió ERROR es porque el usuario no se pudo conectar al servidor
    intentarConectarServidor(&socket, IP_SERVER, PUERTO);

    if(!generarArchivoReglas(PATH_REGLAS))
        finProceso = VERDADERO;

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
            puts("");
            verReglas(PATH_REGLAS);
            break;
        case 3:
            finProceso = VERDADERO;
            break;
        }
    }

    cerrarConexion(&socket);

    return 0;
}
