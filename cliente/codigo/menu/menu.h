#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "../juego/juego.h"
#include "../juego/controles.h"

#define PATH_REGLAS "../../reglas.txt"

int mostrarMenuPrincipal();
void submenuDerrota (tJugador * jugador, unsigned nivel);
char generarArchivoReglas(const char * pathArch);
char verReglas(const char * pathArch);
char submenuTransicion (tJugador * jugador, unsigned nivel);


#endif // MENU_H_INCLUDED
