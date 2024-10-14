#include "ProgramaAcademico.h"

using namespace std;

ProgramaAcademico::ProgramaAcademico()
{
    consolidados = vector<Consolidado *>(8);
}

void ProgramaAcademico:: agregarElementoTipoString(string& nombreDato, string& dato)
{
    mapaDatosString[nombreDato] = dato;
}

void ProgramaAcademico:: agregarElementoTipoInt(string& nombreDato, int dato)
{
    mapaDatosEnteros[nombreDato] = dato;
}

string ProgramaAcademico::consultarDatoMapaStrings(string& clave)
{
    return mapaDatosString.at(clave);
}

int ProgramaAcademico::consultarDatoMapaInt(string& clave)
{
    return mapaDatosEnteros.at(clave);
}

void ProgramaAcademico::setConsolidado(Consolidado *nuevoConsolidado, int pos)
{
    consolidados[pos] = nuevoConsolidado;
}

Consolidado *ProgramaAcademico::getConsolidado(int posicionConsolidado)
{
    return consolidados[posicionConsolidado];
}

ProgramaAcademico::~ProgramaAcademico()
{
    for (Consolidado *consolidado : consolidados)
    {
        delete consolidado;
    }
}