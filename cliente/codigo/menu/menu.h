#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "../juego/juego.h"
#include "../juego/cliente_red.h"
#include "../juego/controles.h"
#include "../estructuras/lista.h"

int mostrarMenuPrincipal();

void submenuDerrota (tJugador * jugador, unsigned nivel);
char submenuTransicion (tJugador * jugador, unsigned nivel);

void verReglas();
char verRankings(SOCKET* socket);

#endif // MENU_H_INCLUDED
