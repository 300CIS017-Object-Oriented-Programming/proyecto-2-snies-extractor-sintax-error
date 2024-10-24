//
// Created by juanp on 15/10/2024.
//

#ifndef UTILIDAD_H
#define UTILIDAD_H

#include <string>
#include <cctype>

using namespace std;

class Utilidad
{
public:
    Utilidad() = default;
    ~Utilidad() = default;

    string minusculasSinEspacios(string &);
    string limpiarString(string &);
    bool isConvertibleToInt(const string&);
};

#endif // UTILIDAD_H
