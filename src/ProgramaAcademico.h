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
    map<string,Consolidado*> consolidados;
    

public:
    ProgramaAcademico();
   
    void agregarElementoTipoString(string&, string&);
    void agregarElementoTipoInt(string&, int);
    void setConsolidado(string& , Consolidado *);
    string consultarDatoMapaStrings(string&);
    int consultarDatoMapaInt(string&);
    void buscarConsolidadoPorAno(int , vector<Consolidado*>&);
    void buscarConsolidadoPorSemestre(int , vector<Consolidado*>&);
    void buscarConsolidadoPorDato(int, vector<Consolidado*>&);
    void buscarConsolidadoPorDato(string& , vector<Consolidado*>&);
    void buscaroConsolidadoPorDato(string& , int, vector<Consolidado*>&);
    Consolidado* buscarConsolidadoPorDato(string&, int , int);
    Consolidado *getConsolidado(string&);
    ~ProgramaAcademico();
};

#endif