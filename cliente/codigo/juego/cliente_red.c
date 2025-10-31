#include "cliente_red.h"

// Devuelve EXITO o ERROR según haya o no conexión con el servidor
char hayConexionServidor()
{
    return VERDADERO;
}

// Devuelve EXITO o ERROR según se haya o no podido obtener los rankings del servidor
// La función debe insertar en la lista los rankings obtenidos
// El formato corresponde a tRanking (en el .h)
char solicitarRankingsServidor(tLista* listaRankings)
{
    // El servidor debería devolver el archivo rankings.dat entero y acá se tiene que procesar e insertar en la lista
    return EXITO;
}

// Devuelve EXITO o ERROR según se pudo o no mandar datos de la partida al servidor
char mandarDatosPartidaServidor(char* nombreJugador, size_t puntajeTotal, size_t cantMovimientos)
{
    // Hay que mandar nombreJugador, puntajeTotal y cantMovimientos

    return EXITO;
}
