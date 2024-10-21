//
// Created by juanp on 15/10/2024.
//

#include "Utilidad.h"

string Utilidad::limpiarString(string &texto)
{
    std::string resultado;
    // Recorre cada carácter de la cadena de entrada
    for (char c : texto)
    {
        // Si el carácter es alfanumérico (letra o número)
        if (std::isalnum(static_cast<unsigned char>(c)))
        {
            // Agrega el carácter al resultado si es alfanumérico
            resultado += c;
        }
    }
    // Devuelve el nuevo string con solo caracteres alfanuméricos
    return resultado;
}

string Utilidad::minusculasSinEspacios(string &input)
{
    std::string resultado;

    // Primero, limpiamos la cadena de entrada de caracteres no alfanuméricos
    string input2 = limpiarString(input);

    // Recorremos cada carácter de la cadena limpia
    for (char c : input2)
    {
        // Si el carácter no es un espacio en blanco
        if (!isspace(c))
        {
            // Convertimos el carácter a minúscula y lo agregamos al resultado
            resultado += tolower(c);
        }
    }

    // Devolvemos la cadena resultante en minúsculas y sin espacios
    return resultado;
}
