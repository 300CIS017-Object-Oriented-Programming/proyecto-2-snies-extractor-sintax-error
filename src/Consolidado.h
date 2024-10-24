#ifndef CONSOLIDADO_H
#define CONSOLIDADO_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Utilidad.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

class Consolidado
{
private:
    map<string, int> datosIntConsolidados;
    map<string, string> datosStringConsolidados;
    Utilidad utilidadObj;

public:
    Consolidado();
    ~Consolidado() = default;
    void agregarDatoInt(string &, int);
    void agregarDatoString(string &, string &);
    int obtenerDatoInt(string &);
    string obtenerDatoString(string &);
};

#endif // CONSOLIDADO_H
