#include "Consolidado.h"

Consolidado::Consolidado() = default;

void Consolidado::agregarDatoIntConsolidado(string& clave,int dato)
{
    datosIntConsolidados[clave] = dato;
}

void Consolidado::agregarDatoStringConsolidado(string& clave,string& dato)
{
    datosStringConsolidados[clave] = dato;
}

int Consolidado::obtenerDatoIntConsolidado(string& clave)
{
    return datosIntConsolidados.at(clave);
}

string Consolidado::obtenerDatoStringConsolidado(string& clave)
{
    return datosStringConsolidados.at(clave);
}