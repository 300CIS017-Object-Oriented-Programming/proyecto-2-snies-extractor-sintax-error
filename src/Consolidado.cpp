#include "Consolidado.h"

Consolidado::Consolidado() = default;

void Consolidado::agregarDatoInt(string& clave, int dato)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    datosIntConsolidados[llave] = dato;
}

void Consolidado::agregarDatoString(string& clave, string& dato)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    datosStringConsolidados[llave] = dato;
}

int Consolidado::obtenerDatoInt(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    if (datosIntConsolidados.find(llave) == datosIntConsolidados.end())
    {
        string msg = string("Llave No Encontrada") + llave + string("'");
        throw invalid_argument(msg);
    }
    return datosIntConsolidados.at(llave);
}

string Consolidado::obtenerDatoString(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    if (datosStringConsolidados.find(llave) == datosStringConsolidados.end())
    {
        string msg = string("Llave No Encontrada: '") + llave + string("'");
        throw invalid_argument(msg);
    }

    return datosStringConsolidados.at(llave);
}