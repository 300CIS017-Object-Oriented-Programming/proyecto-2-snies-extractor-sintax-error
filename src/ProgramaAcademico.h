#ifndef PROGRAMA_ACADEMICO_H
#define PROGRAMA_ACADEMICO_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Consolidado.h"

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
   
    vector<Consolidado *> consolidados;

public:
    ProgramaAcademico();
   
    void agregarElementoTipoString(string&, string&);
    void agregarElementoTipoInt(string&, int);
    void setConsolidado(Consolidado *, int);
    string consultarDatoMapaStrings(string&);
    int consultarDatoMapaInt(string&);
    Consolidado *getConsolidado(int);
    ~ProgramaAcademico();
};

#endif