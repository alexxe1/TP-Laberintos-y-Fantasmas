#include "codigo/menu/menu.h"

int main()
{
    unsigned finProceso = FALSO;
    int opcion;

    if(!generarArchivoReglas(PATH_REGLAS))
      finProceso = VERDADERO;


    while (!finProceso)
    {
        opcion = mostrarMenuPrincipal();

        switch (opcion)
        {
        case 0:
            empezarJuego();
            break;
        case 1:
            verRankings();
            break;
        case 2:
            puts("");
            verReglas(PATH_REGLAS);
            break;
        case 3: finProceso = VERDADERO;
            break;
        }
    }

    return 0;
}
