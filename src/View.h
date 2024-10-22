#ifndef VIEW_H
#define VIEW_H
#include "SNIESController.h"
/*Todo el codigo va entre estas guardas*/
#include "SNIESController.h"
#include "Utilidad.h"

using namespace std;

class View
{
private:
    SNIESController controlador;
    Utilidad utilidadObj;
public:
    View() = default;
    ~View();
    bool mostrarPantallaBienvenido();
    void mostrarDatosExtra();
    void buscarPorPalabraClaveYFormacion();
    void salir();
};

#endif