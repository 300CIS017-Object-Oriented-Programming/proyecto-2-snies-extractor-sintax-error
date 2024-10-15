#ifndef CONSOLIDADO_H
#define CONSOLIDADO_H

#include <string>
#include <vector>
#include <iostream>
#include <map>


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;


class Consolidado
{
private:
    map<string,int> datosIntConsolidados;
    map<string,string>datosStringConsolidados; 

public:
    Consolidado();
    void agregarDatoIntConsolidado(string&, int);
    void agregarDatoStringConsolidado(string&,string&);
    int obtenerDatoIntConsolidado(string&);
    string obtenerDatoStringConsolidado(string&);
};

#endif // CONSOLIDADO_H
