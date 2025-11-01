#include "servidor.h"
#include "arbol.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int init_winsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

void process_request(const char *request, char *response)
{
    char operation[16], text[BUFFER_SIZE];
    sscanf(request, "%15s %[^\n]", operation, text);

    if (strcmp(operation, "MAYUS") == 0)
    {
        to_upper(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "MINUS") == 0)
    {
        to_lower(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else if (strcmp(operation, "INV") == 0)
    {
        reverse(text);
        snprintf(response, BUFFER_SIZE, "%s", text);
    }
    else
    {
        snprintf(response, BUFFER_SIZE, "Operacion no valida");
    }
}

void procesarEntrada(const char *peticion, char *respuesta, tArbol *a, tCmp cmp)
{
    char operacion[16], nombre[21];
    int puntaje, movs;

    operacion[0] = nombre[0] = '\0';
    puntaje = movs = 0;

    sscanf(peticion, "%15s %20s %d %d", operacion, nombre, &puntaje, &movs);

    if(strcmpi(operacion, "RANKING") == 0)
    {
        generarRanking(NOMBRE_ARCH_USUARIOS, NOMBRE_ARCH_PARTIDAS, respuesta);

    }
    else if(strcmpi(operacion, "REGISTRAR") == 0)
    {
        if(nombre[0] != '\0')
        {
            if(!buscarNodoNoClave(a,nombre,sizeof(nombre), cmp))
            {
                agregarAArchivo(NOMBRE_ARCH_USUARIOS, nombre);
                cargarDesdeArchOrdenadoArbol(a, sizeof(tIdxJugador), NOMBRE_ARCH_USUARIOS, cmp);
                strcpy(respuesta, "USUARIO REGISTRADO CON EXITO");
            }
            else
            {
                strcpy(respuesta, "USUARIO YA EXISTENTE");

            }
        }
        else
        {
            strcpy(respuesta, "ERROR: falta el nombre del jugador");
            return;
        }

    }
    else if(strcmp(operacion, "GUARDAR"))
    {
        if(nombre[0] != '\0')
        {
            guardarPartida(NOMBRE_ARCH_PARTIDAS, nombre, puntaje, movs);
            strcpy(respuesta, "PARTIDA GUARDADA...");
        }
        else

        {
            strcpy(respuesta, "ERROR: falta el nombre del jugador");
        }
    }
    else
    {
        strcpy(respuesta, "ERROR: operacion invalida");
    }


}

void run_server()
{
    tArbol arbol;
    tJugador jugador;

    crearArbol(&arbol);

    crearArchIdx(&arbol, NOMBRE_ARCH_USUARIOS, NOMBRE_ARCH_INDICE, sizeof(tJugador), sizeof(jugador.nombre),crearIdx);

    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en puerto %d...\n", PORT);

    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        printf("Recibido: %s\n", buffer);
        procesarEntrada(buffer, response);
        send(client_socket, response, strlen(response), 0);
        printf("Enviado:  %s\n", response);
    }

    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}

void crearIdx(tArbol *a, )
