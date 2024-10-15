#include "ProgramaAcademico.h"

using namespace std;

ProgramaAcademico::ProgramaAcademico()
{
    //consolidados = vector<Consolidado *>(8);
}

void ProgramaAcademico:: agregarElementoTipoString(string& nombreDato, string& dato)
{
    string llave = utilidadObj.minusculasSinEspacios(nombreDato);
    mapaDatosString[llave] = dato;
}

void ProgramaAcademico:: agregarElementoTipoInt(string& nombreDato, int dato)
{
    string llave = utilidadObj.minusculasSinEspacios(nombreDato);
    mapaDatosEnteros[llave] = dato;
}

string ProgramaAcademico::consultarDatoString(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    return mapaDatosString.at(llave);
}

int ProgramaAcademico::consultarDatoInt(string& clave)
{
    string llave = utilidadObj.minusculasSinEspacios(clave);
    return mapaDatosEnteros.at(llave);
}

void ProgramaAcademico::setConsolidado(string& infoSexo, int infoAno , int infoSemestre, Consolidado *nuevoConsolidado)
{
    string claveConsolidado = infoSexo + "-" + to_string(infoAno) + to_string(infoSemestre);
    claveConsolidado = utilidadObj.minusculasSinEspacios(claveConsolidado);
    consolidados[claveConsolidado] = nuevoConsolidado;
}

Consolidado *ProgramaAcademico::getConsolidado(string& claveConsolidado)
{
    string llave = utilidadObj.minusculasSinEspacios(claveConsolidado);
    return consolidados[llave];
}

ProgramaAcademico::~ProgramaAcademico()
{
    for (auto& pair : consolidados)
    {
        delete pair.second;
    }
    consolidados.clear();
}


Consolidado* ProgramaAcademico::buscarConsolidadoPorDato(string& infoSexo, int infoAno , int infoSemestre)
{
    map<string,Consolidado*>::iterator iteradorConsolidado = consolidados.begin();
    string claveConsolidado = infoSexo + "-" + to_string(infoAno) + to_string(infoSemestre);
    return consolidados[claveConsolidado];
}