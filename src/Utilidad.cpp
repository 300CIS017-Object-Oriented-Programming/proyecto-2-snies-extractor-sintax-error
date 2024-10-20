//
// Created by juanp on 15/10/2024.
//

#include "Utilidad.h"

string Utilidad::limpiarString(string &texto)
{
    std::string resultado;
    for (char c : texto)
    {
        // Si el carácter es alfanumérico
        if (std::isalnum(static_cast<unsigned char>(c)))
        {
            resultado += c;
        }
    }
    return resultado;
}

string Utilidad::minusculasSinEspacios(string& input)
{
    std::string resultado;

    string input2 = limpiarString(input);

    // Recorremos cada carácter del string de entrada
    for (char c : input2) {
        if (!isspace(c)) {  // Verifica si no es un espacio
            resultado += tolower(c);  // Convierte a minúscula y añade al resultado
        }
    }

    return resultado;
}
