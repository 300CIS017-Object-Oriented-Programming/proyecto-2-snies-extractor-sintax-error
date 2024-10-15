#include "Consolidado.h"

Consolidado::Consolidado() = default;

void Consolidado::agregarDatoInt(string& clave,int dato)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    datosIntConsolidados[llave] = dato;
}

void Consolidado::agregarDatoString(string& clave,string& dato)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    datosStringConsolidados[llave] = dato;
}

int Consolidado::obtenerDatoInt(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    return datosIntConsolidados.at(llave);
}

string Consolidado::obtenerDatoString(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    return datosStringConsolidados.at(llave);
}