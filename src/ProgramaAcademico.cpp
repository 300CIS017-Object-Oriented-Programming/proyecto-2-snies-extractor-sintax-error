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