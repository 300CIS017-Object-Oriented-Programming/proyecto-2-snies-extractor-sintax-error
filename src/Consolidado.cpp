#include "Consolidado.h"

// Constructor por defecto de la clase Consolidado
Consolidado::Consolidado() = default;

// Método para agregar un dato entero a la colección consolidada
// clave: referencia a la clave asociada al dato
// dato: valor entero a ser almacenado en el mapa
void Consolidado::agregarDatoInt(string& clave, int dato)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Almacena el valor entero en el mapa datosIntConsolidados
    datosIntConsolidados[llave] = dato;
}

// Método para agregar un dato tipo string a la colección consolidada
// clave: referencia a la clave asociada al dato
// dato: referencia al valor string a ser almacenado en el mapa
void Consolidado::agregarDatoString(string& clave, string& dato)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Almacena el valor string en el mapa datosStringConsolidados
    datosStringConsolidados[llave] = dato;
}

// Método para obtener un dato entero asociado a una clave
// clave: referencia a la clave del dato a buscar
// return: el valor entero asociado a la clave
int Consolidado::obtenerDatoInt(string& clave)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Verifica si la clave existe en el mapa, de lo contrario lanza una excepción
    if (datosIntConsolidados.find(llave) == datosIntConsolidados.end())
    {
        // Genera un mensaje de error con la clave no encontrada
        string msg = string("Llave No Encontrada: '") + llave + string("'");
        throw invalid_argument(msg);  // Lanza excepción si la clave no existe
    }
    // Retorna el valor entero asociado a la clave
    return datosIntConsolidados.at(llave);
}

// Método para obtener un dato tipo string asociado a una clave
// clave: referencia a la clave del dato a buscar
// return: el valor string asociado a la clave
string Consolidado::obtenerDatoString(string& clave)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Verifica si la clave existe en el mapa, de lo contrario lanza una excepción
    if (datosStringConsolidados.find(llave) == datosStringConsolidados.end())
    {
        // Genera un mensaje de error con la clave no encontrada
        string msg = string("Llave No Encontrada: '") + llave + string("'");
        throw invalid_argument(msg);  // Lanza excepción si la clave no existe
    }
    // Retorna el valor string asociado a la clave
    return datosStringConsolidados.at(llave);
}