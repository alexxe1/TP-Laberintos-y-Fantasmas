#include "servidor.h"
#include "arbol.h"
#include "lista.h"
#include "cola.h"

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

int procesarEntrada(const char *peticion, char* respuesta, tArbol *a, tCmp cmp, SOCKET cliente)
{
    char operacion[16], nombre[16];
    int puntaje, movs, cantRank;
    tJugador jugador;
    tRanking rank;
    tLista rankL;

    operacion[0] = nombre[0] = '\0';
    puntaje = movs = 0;

    sscanf(peticion, "%15s %15s %d %d", operacion, nombre, &puntaje, &movs);

    strcpy(jugador.nombre, nombre);

    if(strcmpi(operacion, "RANKING") == 0)
    {
        //busco en el achivo usuario el nombre, con ese nombre busco el id, despues de que tengo el id voy al archivo de partidas
        // y inserto ordenado mientras acumulo los puntos de partida cuando se repita el id
        rankL = generarRanking(a,NOMBRE_ARCH_PARTIDAS);

        cantRank = contarNodosLista(&rankL);

        send(cliente, (char*)&cantRank, sizeof(int),0);

        while(!listaVacia(&rankL))
        {
            sacarListaPrimero(&rankL, &rank, sizeof(tRanking));
            send(cliente, (char*)&rank, sizeof(tRanking),0);
        }
        return RANK;

    }
    else if(strcmpi(operacion, "REGISTRAR") == 0)
    {
        if(nombre[0] != '\0')
        {
            if(!buscarNodoNoClave(a,&jugador,sizeof(tJugador), cmp))
            {
                //agregamos con "a+b" al final del archivo usuario el nombre con su id generado
                agregarAArchivo(NOMBRE_ARCH_USUARIOS, nombre);
                //creamos indice
                //balancear arbol
                crearArchIdx(a, NOMBRE_ARCH_USUARIOS, NOMBRE_ARCH_INDICE, sizeof(tJugador),sizeof(tIdxJugador),crearIdx, cmpIdx);
                cargarDesdeArchOrdenadoArbol(a,sizeof(tIdxJugador), NOMBRE_ARCH_INDICE, cmp);
                *respuesta = 1;
                //strcpy(respuesta, "USUARIO REGISTRADO CON EXITO");
            }
            /*else
            {
                strcpy(respuesta, "USUARIO YA EXISTENTE");

            }*/
        }
        else
        {
            //strcpy(respuesta, "ERROR: falta el nombre del jugador");
            *respuesta = 0;
        }

    }
    else if(strcmp(operacion, "GUARDAR") == 0)
    {
        if(nombre[0] != '\0')
        {
            //abrimos el archivo de partida con "a+b" nos fijamos el ultimo id de partida, le sumamos uno y lo guardamos al final del archivo
            if(guardarPartida(NOMBRE_ARCH_PARTIDAS, nombre, puntaje, movs,a) != NO_ENCONTRADO)
            {
                //strcpy(respuesta, "PARTIDA GUARDADA...");
                *respuesta = 1;

            }
            else
            {
                //strcpy(respuesta, "USUARIO NO REGISTRADO");
                *respuesta = 0;
            }
        }
        else

        {
            //strcpy(respuesta, "ERROR: falta el nombre del jugador");
            *respuesta = 0;
        }
    }
    else
    {
        //strcpy(respuesta, "ERROR: operacion invalida");
        *respuesta = 0;
    }

    return TODO_OK;
}

void run_server()
{
    char buffer[BUFFER_SIZE];
    char response = '\0';
    int bytes_received;

    struct sockaddr_in client_addr;
    int client_addr_size;
    int err;
    char peticion[BUFFER_SIZE];

    tCola cola;
    tArbol arbol;

    int result;

    crearArbol(&arbol);
    crearCola(&cola);
    //creamos indice y despues generamos el arbol balanceado, si no se pudo generar el indice porque todavia no jugo nadie y no se creo
    //el archivo usuario entonces no generamos el arbol
    result = crearArchIdx(&arbol, NOMBRE_ARCH_USUARIOS, NOMBRE_ARCH_INDICE, sizeof(tJugador),sizeof(tIdxJugador), crearIdx, cmpIdx);
    if(result)
    {
        cargarDesdeArchOrdenadoArbol(&arbol,sizeof(tIdxJugador), NOMBRE_ARCH_INDICE, cmpIdx);
    }

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

    client_addr_size = sizeof(client_addr);

    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    while (1)
    {
        // Intentar recibir un mensaje
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("[RX] Recibido: %s\n", buffer);

            // Encolamos el mensaje recibido
            ponerEnCola(&cola, buffer, BUFFER_SIZE);
        }
        else if (bytes_received == 0)
        {
            // El cliente ha cerrado la conexi�n de forma ordenada
            printf("Cliente desconectado (recv == 0). Saliendo del bucle.\n");
            break;
        }
        else /* bytes_received == SOCKET_ERROR */
        {
            err = WSAGetLastError();        // Ver que es esto
            if (err == WSAEWOULDBLOCK)
            {
                // No hay datos en modo no-bloqueante.
                // Simplemente seguimos el bucle y procesamos la cola.
            }
            else
            {
                // Error serio, salimos
                printf("Error en recv(): %d. Saliendo.\n", err);
                break;
            }
        }

        // Procesar mensajes pendientes en la cola
        printf("hola");
        while (!colaVacia(&cola))
        {
            printf("hola");
            // Desencolar la peticion
            sacarDeCola(&cola, peticion, BUFFER_SIZE);

            // Procesar la peticion
            printf("[PROC] Procesando: %s\n", peticion);
            if(procesarEntrada(peticion, &response, &arbol, cmpId, client_socket) != RANK)
            {
                send(client_socket, &response, strlen(&response), 0);
                printf("[TX] Enviado: %d\n", response);
            }

            // Enviar la respuesta al cliente (si es necesario)

        }

        // Evita usar de manera intenciva la CPU
        Sleep(100);
    }

    vaciarCola(&cola);
    vaciarArbol(&arbol);

    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}

int crearIdx(tArbol *a, void *reg, int desp, tCmp cmp)
{
    tJugador *j = (tJugador*)reg;
    tIdxJugador jIdx;

    jIdx.id = j->id;
    jIdx.desp = desp;
    strcpy(jIdx.nombre, j->nombre);

    if(insertarEnArbolR(a, &jIdx, sizeof(tIdxJugador), cmp) == FALLO_MALLOC)
    {
        return FALLO_MALLOC;
    }

    return TODO_OK;
}

int cmpIdx(const void *a, const void *b)
{
    tJugador *x = (tJugador*)a;
    tJugador *y = (tJugador*)b;

    return strcmp(x->nombre, y->nombre);
}

int agregarAArchivo(const char *nombreArchUsuario, char *nombre)
{
    tJugador jugador;
    FILE *arch = fopen(nombreArchUsuario, "a+b");
    if(!arch)
        return ERR_ARCH;

    fseek(arch, 0, SEEK_END); // Ir al final
    long tam = ftell(arch);   // Obtener tamaño total del archivo

    if(tam == 0)
    {
        // El archivo está vacío → primer jugador
        jugador.id = 1;
    }
    else
    {
        // El archivo tiene datos → leer el último jugador
        fseek(arch, -(long)sizeof(tJugador), SEEK_END);
        fread(&jugador, sizeof(tJugador), 1, arch);
        jugador.id = jugador.id + 1;
    }

    strcpy(jugador.nombre, nombre);

    // Ir al final para agregar el nuevo registro
    fseek(arch, 0, SEEK_END);
    fwrite(&jugador, sizeof(tJugador), 1, arch);

    fclose(arch);
    return TODO_OK;
}


int guardarPartida(const char *nombreArchPartida, char *nombre, int puntaje, int movs, tArbol *a)
{
    tPartida partida;
    tIdxJugador indice;

    int tamArch;
    FILE *arch = fopen(nombreArchPartida, "a+b");
    if(!arch)
    {
        return ERR_ARCH;
    }

    strcpy(indice.nombre, nombre);

    if(!buscarNodoNoClave(a, &indice, sizeof(tIdxJugador), cmpIdx))
    {
        fclose(arch);

        return NO_ENCONTRADO;
    }

    fseek(arch,0,SEEK_END);

    tamArch = ftell(arch);

    if(tamArch == 0)
    {
        partida.idPartida = 1;
        partida.cantMovs = movs;
        partida.puntuacion = puntaje;
        partida.idJugador = indice.id;
    }
    else
    {
        fseek(arch,-(long)sizeof(tPartida),SEEK_END);
        fread(&partida, sizeof(tPartida),1, arch);

        partida.idPartida = partida.idPartida + 1;
        partida.cantMovs = movs;
        partida.idJugador = indice.id;
        partida.puntuacion = puntaje;
    }

    fseek(arch, 0, SEEK_END);
    fwrite(&partida, sizeof(tPartida),1, arch);

    fclose(arch);
    return TODO_OK;

}

tLista generarRanking(tArbol *a, const char *nombreArchPartidas)
{
    tPartida partida;
    tIdxJugador jugador;
    tRanking rank;

    tLista partidaL;
    tLista rankL;

    FILE *archPartida = fopen(nombreArchPartidas, "rb");
    if(!archPartida)
    {
        return NULL;
    }

    crearLista(&partidaL);
    crearLista(&rankL);

    while(fread(&partida, sizeof(tPartida),1, archPartida))
    {
        ponerEnOrden(&partidaL,&partida, sizeof(tPartida),cmpId, acumularPuntos);
    }

    sacarListaPrimero(&partidaL, &partida, sizeof(tPartida));
    jugador.nombre[0] = '\0';
    jugador.id = partida.idJugador;

    buscarNodoNoClave(a, &jugador, sizeof(tIdxJugador), cmpIdx);

    strcpy(rank.nombre, jugador.nombre);
    rank.puntos = partida.puntuacion;

    ponerEnListaUltimo(&rankL, &rank, sizeof(tRanking));

    //sacoprimero de lista y lo guardo en partida, con el id busco en el arbol el nombre, recordar que el arbol es tIdxJugador
    //basicamente tIdxJugador.id = partida.idJugador
    //como ya saque el primer de lista tengo cargada toda la partida, es decir id de jugador con su total de puntos y el nombre en el indice
    //asigno numero incremental como puesto en ranking y el nombre y puntuacion lo saco de las 2 anterior
    //buscar forma de copiar todo el ranking en la respuesta por ejemplo 1 juan 33 | 4 alberto 33
    //en la foto no esta el puesto, asi que pueden poner el id del jugador mejor
    fclose(archPartida);

    return rankL;
}


int acumularPuntos(void **dest, unsigned *tamDest, const void *origen, unsigned tamOrig)
{
    tPartida *p = (tPartida*)*dest;
    tPartida *dup = (tPartida*)origen;

    p->puntuacion += dup->puntuacion;

    return TODO_OK;
}

int cmpId(const void *a, const void *b)
{
    tPartida *x = (tPartida*)a;
    tPartida *y = (tPartida*)b;

    return x->idJugador - y->idJugador;
}
