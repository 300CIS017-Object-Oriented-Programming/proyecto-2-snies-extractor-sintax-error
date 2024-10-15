#ifndef PROGRAMA_ACADEMICO_H
#define PROGRAMA_ACADEMICO_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Consolidado.h"
#include "Utilidad.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

class ProgramaAcademico
{
    map<string&, string&> mapaDatosString;
    map<string&, int> mapaDatosEnteros;
    map<string,Consolidado*> consolidados;
    Utilidad ut;
    
    void minusculasSinEspacios(string&);
public:
    ProgramaAcademico();
   
    void agregarElementoTipoString(string&, string&);
    void agregarElementoTipoInt(string&, int);
    void setConsolidado(string&, int , int, Consolidado *);
    string consultarDatoString(string&);
    int consultarDatoInt(string&);
    Consolidado* buscarConsolidadoPorDato(string&, int , int);
    Consolidado* getConsolidado(string&);
    ~ProgramaAcademico();
};

#endif