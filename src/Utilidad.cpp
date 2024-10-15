//
// Created by juanp on 15/10/2024.
//

#include "Utilidad.h"

void Utilidad::minusculasSinEspacios(string& input)
{
        std::string resultado;

        // Recorremos cada carácter del string de entrada
        for (char c : input) {
            if (!isspace(c)) {  // Verifica si no es un espacio
                resultado += tolower(c);  // Convierte a minúscula y añade al resultado
            }
        }

        input = resultado;
}
