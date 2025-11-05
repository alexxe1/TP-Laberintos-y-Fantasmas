#include "juego.h"

int empezarJuego(SOCKET* socket)
{
    // Variables de estado
    unsigned char juegoTerminado = FALSO;
    char estado;
    size_t iteracion = 1;

    // Información del jugador
    char nombreJug[MAX_NOM];
    char dioAltaJugador = FALSO;

    // Estructuras y TDAs
    tLaberinto laberinto;
    tEntidades entidades;
    tConfiguracion configuracion;

    // --- 1. Configuración ---
    if (inicializarConfiguracion(&configuracion) == ERROR)
        return ERROR;

    // --- 2. Crear laberinto y entidades base ---
    if (inicializarJuegoBase(&laberinto, &entidades, &configuracion, iteracion) == ERROR)
        return ERROR;

    // --- 3. Nombre del jugador ---
    pedirNombreJugador(nombreJug);

    // Leemos el laberinto y lo interpretamos para generar todo
    if (!procesarEntidades(&laberinto, &entidades, &configuracion, nombreJug, FALSO))
    {
        destruirLaberinto(&laberinto);
        vaciarVector(&entidades.fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        _getch();
        return ERROR;
    }

    // --- 5. Alta jugador en servidor ---
    dioAltaJugador = conectarJugadorServidor(socket, &entidades);

    // --- 6. Dibujado inicial ---
    dibujarJuego(&laberinto, &entidades);

    // --- 7. Bucle principal ---
    while (!juegoTerminado)
    {
        estado = actualizarJuego(&laberinto, &entidades, &juegoTerminado);

        if (estado == CONTINUA)
            dibujarJuego(&laberinto, &entidades);
        else if (estado == VICTORIA)
            manejarVictoria(&laberinto, &entidades, &configuracion, &iteracion, &juegoTerminado);
    }

    // --- 8. Fin del juego ---
    manejarFinDeJuego(socket, &laberinto, &entidades, dioAltaJugador);

    destruirLaberinto(&laberinto);
    destruirColasYVectores(&entidades);

    return EXITO;
}

int inicializarConfiguracion(tConfiguracion* configuracion)
{
    int resultado = cargarArchivoConfiguracion(configuracion);

    switch (resultado)
    {
        case EXITO:
            return EXITO;
        case ERROR_ARCHIVO:
            puts("ERROR: No se pudo procesar config.txt");
            break;
        case ERROR_FILAS:
            printf("ERROR: Filas invalidas (MAX: %d - MIN: %d)\n", MAX_FILAS, MIN_FILAS);
            break;
        case ERROR_COLUMNAS:
            printf("ERROR: Columnas invalidas (MAX: %d - MIN: %d)\n", MAX_COLUMNAS, MIN_COLUMNAS);
            break;
        case ERROR_VIDAS_INICIALES:
            printf("ERROR: Vidas iniciales invalidas (MAX: %d - MIN: 1)\n", MAX_VIDAS_INICIALES);
            break;
        case ERROR_FANTASMAS:
            printf("ERROR: Fantasmas invalidos\n");
            break;
        default:
            puts("ERROR: Configuracion desconocida");
            break;
    }

    _getch();

    return ERROR;
}

int inicializarJuegoBase(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, size_t iteracion)
{
    srand((unsigned)time(NULL));

    if (!crearLaberintoAleatorio(laberinto, configuracion, iteracion))
    {
        puts("ERROR al crear laberinto aleatorio");
        _getch();
        return ERROR;
    }

    if (!crearVector(&entidades->fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(laberinto);
        puts("ERROR: Sin memoria para fantasmas");
        _getch();
        return ERROR;
    }

    return EXITO;
}

void pedirNombreJugador(char* nombre)
{
    char valido = FALSO;

    do
    {
        system("cls");
        printf("Ingrese el nombre del jugador: ");

        ingresarNombre(nombre, MAX_NOM);

        valido = esNombreValido(nombre);

        if (!valido)
        {
            puts("\nNombre invalido. Solo letras, numeros y guiones bajos.");
            puts("Debe empezar con letra, sin espacios, minimo 3 caracteres.\n");
            system("pause");
        }
    }
    while (!valido);
}

int conectarJugadorServidor(SOCKET* socket, tEntidades* entidades)
{
    if (darAltaJugadorServidor(socket, entidades->jugador.nombre) == ERROR)
    {
        puts("\nError al registrar el jugador. No se guardara el puntaje.\n");
        system("pause");
        return FALSO;
    }

    return VERDADERO;
}

void manejarVictoria(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, size_t* iteracion, unsigned char* juegoTerminado)
{
    char estado = submenuTransicion(&entidades->jugador, laberinto->nivel);

    // El usuario quiere irse
    if (estado == VERDADERO)
    {
        *juegoTerminado = VERDADERO;
        return;
    }

    // El usuario quiere continuar, reiniciamos el juego e iteramos
    destruirLaberinto(laberinto);
    vaciarVector(&entidades->fantasmas);
    (*iteracion)++;

    // Si el usuario quiere seguir pero hubo un error de memoria, borramos y salimos
    if (!continuarJugando(laberinto, configuracion, entidades, *iteracion))
    {
        destruirColasYVectores(entidades);
        *juegoTerminado = VERDADERO;
    }
    else
        dibujarJuego(laberinto, entidades);
}

void manejarFinDeJuego(SOCKET* socket, tLaberinto* laberinto, tEntidades* entidades, char dioAltaJugador)
{
    size_t cantMov = mostrarMovimientos(&entidades->jugador);

    puts("\n\nApreta cualquier tecla para continuar...");
    getch();

    // Le mostramos al jugador un resumen de su partida
    submenuDerrota(&entidades->jugador, laberinto->nivel);

    // Intentamos mandar datos de la partida si el jugador fue dado de alta previamente
    if (dioAltaJugador == VERDADERO)
    {
        system("cls");
        puts("Enviando puntaje al servidor...\n");

        if (mandarDatosPartidaServidor(socket, entidades->jugador.nombre, entidades->jugador.puntajeTotal, cantMov) == ERROR)
            puts("Error al enviar puntaje.");
        else
            puts("Puntaje enviado correctamente!");

        puts("\nApreta cualquier tecla para continuar...");
        getch();
    }
}

char actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado)
{
    char teclaApretada;
    char direccionJugador = NO_DIRECCION;
    tMov movimiento;

    if (!_kbhit()) // Si no se apretó nada, no perdemos tiempo en calcular nada
        return SIN_MOVIMIENTO;

    teclaApretada = _getch();

    if (teclaApretada == SALIR_DEL_JUEGO) // Si se va a cerrar el juego, tampoco deberíamos calcular nada más
    {
        *juegoTerminado = VERDADERO;
        return SIN_MOVIMIENTO;
    }

    // Determinamos la dirección del jugador
    if (TECLA_ABAJO(teclaApretada))
        direccionJugador = ABAJO;
    else if (TECLA_ARRIBA(teclaApretada))
        direccionJugador = ARRIBA;
    else if (TECLA_IZQUIERDA(teclaApretada))
        direccionJugador = IZQUIERDA;
    else if (TECLA_DERECHA(teclaApretada))
        direccionJugador = DERECHA;

    // Si apretó una tecla no contemplada, no se mueve
    if (direccionJugador == NO_DIRECCION)
        return SIN_MOVIMIENTO;

    // Empaquetamos el struct movimiento para encolarlo y después procesarlo
    movimiento.tipo = JUGADOR;
    movimiento.id = 0;
    movimiento.mov = direccionJugador;

    ponerEncola(&entidades->colaMov, &movimiento, sizeof(tMov));

    // Le mando la cola ya cargada con el movimiento del jugador ya que aca es donde se sabe para que lado se mueve
    procesarMovimientos(entidades, laberinto, juegoTerminado);

    if (jugadorEnSalida(&entidades->jugador, laberinto))
    {
        return VICTORIA;
    }

    if (chequeoPremio(&entidades->jugador, laberinto))
    {
        sumarPuntaje(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    if (chequeoVida(&entidades->jugador, laberinto))
    {
        sumarVida(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    // Segundo chequeo de fantasma, post movimiento de los mismos
    if(chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
    {
        volverYDescontar(&entidades->jugador);

        if (jugadorSinVidas(&entidades->jugador))
        {
            *juegoTerminado = VERDADERO;
            return DERROTA;
        }
    }

    return CONTINUA;
}

void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    tFantasma* fantasmaEncontrado;
    int indiceFantasmaEncontrado = -1;

    system("cls");

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            // Primero el jugador
            if (entidades->jugador.posActual.fila == i && entidades->jugador.posActual.columna == j)
            {
                dibujarJugador(&entidades->jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            indiceFantasmaEncontrado = buscarFantasmaEnPosicion(&entidades->fantasmas, i, j);

            if (indiceFantasmaEncontrado != -1)
            {
                fantasmaEncontrado = (tFantasma*)obtenerElementoVector(&entidades->fantasmas, indiceFantasmaEncontrado);
                dibujarFantasma(fantasmaEncontrado, i, j);
                continue;
            }

            // Dibujamos el resto del laberinto
            printf("%c", obtenerCasillaLaberinto(laberinto, i, j));
        }

        puts("");
    }

    puts("");
    mostrarVidasYPuntos(&entidades->jugador);
}

int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, const char* nombJug, unsigned iteracion)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    char casilla;
    int jugadorEncontrado = FALSO;
    tFantasma fantasmaAux;

    crearCola(&entidades->colaMov);

    // Procesamos todo el laberinto para asignarle a las entidades sus puntos de inicio y validar
    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            casilla = obtenerCasillaLaberinto(laberinto, i, j);

            switch (casilla)
            {
            case ENTRADA:
                if (jugadorEncontrado) // Por si hay más de un jugador en el laberinto
                    break;

                if (iteracion <= 1)
                {
                    crearJugador(&entidades->jugador, nombJug, configuracion, i, j);
                }
                else
                {
                    acomodarJugador(&entidades->jugador, i,j);
                }


                jugadorEncontrado = VERDADERO;
                break;

            case FANTASMA:
                crearFantasma(&fantasmaAux, i, j);
                insertarVector(&entidades->fantasmas, &fantasmaAux);
                modificarCasillaLaberinto(laberinto, i, j, CAMINO); // Sacamos la 'F' del laberinto
                break;
            }
        }
    }

    return jugadorEncontrado == VERDADERO;
}

char procesarMovimientos(tEntidades* entidades, tLaberinto* laberinto, unsigned char* juegoTerminado)
{
    int i;
    char mov;
    tMov evento;
    tFantasma *fantasma;

    size_t cantFantasmas = obtenerLongitudVector(&entidades->fantasmas);

    while(sacarDeCola(&entidades->colaMov, &evento, sizeof(tMov)))
    {
        //me fijo si es el movimiento del jugador, si es asi entonces lo muevo y calculo los movimientos que deben hacer los fantasmas
        //para alcanzarlo
        if(evento.tipo == JUGADOR)
        {
            if(moverJugador(&entidades->jugador, evento.mov, laberinto) == FALSO)
            {
                return CONTINUA;
            }

            ponerEncola(&entidades->jugador.colaMovimientos,&entidades->jugador.posActual,sizeof(tPosicion));

            if (chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
            {
                volverYDescontar(&entidades->jugador);

                if (jugadorSinVidas(&entidades->jugador))
                {
                    *juegoTerminado = VERDADERO;
                    return DERROTA;
                }
            }

            //calculo el movimiento de los fantasmas y le asigno su identificador y el movimiento que realiza a la cola
            for (i = 0; i < cantFantasmas; i++)
            {
                fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas,i);
                mov = calcularMovimientoFantasma(fantasma,laberinto,&entidades->jugador);

                evento.id = i;
                evento.tipo = FANTASMA;
                evento.mov = mov;

                if (fantasma->tocado == FALSO)
                {
                    ponerEncola(&entidades->colaMov, &evento, sizeof(tMov));
                }
            }

        }
        else if(evento.tipo == FANTASMA) // Si es un fantasma entonces el id nos da la posicion que tiene en el vector y movemos a ese fantasma
        {
            fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas, evento.id);
            moverFantasma(fantasma,evento.mov,laberinto);
        }
    }
    return CONTINUA;
}

size_t mostrarMovimientos(tJugador* jugador)
{
    tPosicion pos;
    int contador = 0;

    printf("\n-- HISTORIAL DE MOVIMIENTOS REALIZADOS --\n");

    while(!colaVacia(&jugador->colaMovimientos))
    {
        sacarDeCola(&jugador->colaMovimientos,&pos, sizeof(tPosicion));
        contador++;
        printf("(%lu,%lu) ", (unsigned long)pos.fila, (unsigned long)pos.columna);
    }

    return contador;
}

char continuarJugando(tLaberinto* laberinto, tConfiguracion* configuracion, tEntidades* entidades, unsigned nivel)
{
    if (!crearLaberintoAleatorio(laberinto, configuracion, nivel))
    {
        puts("ERROR: Hubo un error al crear el laberinto aleatorio");
        return ERROR;
    }

    if (!crearVector(&entidades->fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(laberinto);
        puts("ERROR: No hay memoria para crear los fantasmas");
        return ERROR;
    }

    if (!procesarEntidades(laberinto, entidades, configuracion, NULL, nivel))
    {
        destruirLaberinto(laberinto);
        vaciarVector(&entidades->fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        return ERROR;
    }

    return EXITO;
}

void destruirColasYVectores(tEntidades* entidades)
{
    destruirJugador(&entidades->jugador);

    for (size_t i = 0; i < obtenerLongitudVector(&entidades->fantasmas); i++)
    {
        destruirFantasma((tFantasma*)obtenerElementoVector(&entidades->fantasmas, i));
    }

    vaciarVector(&entidades->fantasmas);
    vaciarCola(&entidades->colaMov);
}
