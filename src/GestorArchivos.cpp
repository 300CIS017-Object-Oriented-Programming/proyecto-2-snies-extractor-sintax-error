#include "GestorArchivos.h"

vector<int> GestorArchivos::leerProgramas(string &ruta)
{
    vector<int> codigosSniesRetorno;
    ifstream archivoProgramasCsv(ruta);
    if (!(archivoProgramasCsv.is_open()))
    {
        cout << "Archivo " << ruta << " no se pudo abrir correctamente" << endl;
    }
    else
    {
        string linea;
        string dato;
        // Saltarse la primera linea
        getline(archivoProgramasCsv, linea);
        // Leer los programas
        while (getline(archivoProgramasCsv, linea))
        {
            stringstream streamLinea(linea);
            getline(streamLinea, dato, ';');
            codigosSniesRetorno.push_back(stoi(dato));
        }
    }
    archivoProgramasCsv.close();
    return codigosSniesRetorno;
}

vector<vector<string>> GestorArchivos::leerArchivo(string &ruta, vector<string> &etiquetasColumnas, vector<int> &codigosSnies)
{
    char delimitador = Settings::DELIMITADOR[0];
    vector<vector<string>> matrizResultado;
    ifstream archivoPrimero(ruta + string(".csv"));

    // Verificar si el archivo se abre correctamente
    if (!archivoPrimero.is_open())
    {
        cout << "Archivo " << ruta << " no se pudo abrir correctamente" << endl;
        return matrizResultado; // Retornar matriz vacía
    }

    map<int, string> mapa; // Mapa para almacenar la posición y etiqueta
    string fila;
    string dato;
    int columna = 0;
    int indiceColumnaCodigo = -1; // Índice de la columna con los códigos SNIES
    string datoMinusculasSinEspacios;

    // Primera iteración del ciclo para guardar las etiquetas
    getline(archivoPrimero, fila);
    stringstream streamFila(fila);

    // Procesar etiquetas
    while (getline(streamFila, dato, delimitador))
    {
        datoMinusculasSinEspacios = utilidadObj.minusculasSinEspacios(dato);
        auto it = find(etiquetasColumnas.begin(), etiquetasColumnas.end(), datoMinusculasSinEspacios);
        if (it != etiquetasColumnas.end())
        {
            mapa[columna] = datoMinusculasSinEspacios; // Almacena la posición y la etiqueta en el mapa
        }
        // Verificar si esta es la columna de los códigos SNIES
        if (datoMinusculasSinEspacios == minusculasSinEspacios("CÓDIGO SNIES DEL PROGRAMA"))
        {
            indiceColumnaCodigo = columna;
        }
        ++columna;
    }

    // Si no se encontró la columna de códigos SNIES, retornar matriz vacía
    if (indiceColumnaCodigo == -1)
    {
        cout << "No se encontró la columna de Código SNIES en el archivo." << endl;
        return matrizResultado;
    }

    // Añadir etiquetas a la matriz de resultados
    vector<string> etiquetas;
    for (const auto &par : mapa)
    {
        etiquetas.push_back(par.second);
    }
    matrizResultado.push_back(etiquetas);

    // Leer el resto del archivo
    while (getline(archivoPrimero, fila))
    {
        stringstream streamFila(fila);
        vector<string> filaActual;
        bool codigoCoincide = false;
        columna = 0;

        // Procesar fila
        while (getline(streamFila, dato, delimitador))
        {

            // Verificar si esta es la columna del código SNIES
            if (columna == indiceColumnaCodigo)
            {
                int numero = std::stoi(dato); // Convertir a entero
                if (std::find(codigosSnies.begin(), codigosSnies.end(), numero) != codigosSnies.end())
                {
                    codigoCoincide = true; // Código SNIES coincide
                }
            }

            // Solo almacenamos las columnas de interés (las que están en el mapa)
            if (mapa.find(columna) != mapa.end())
            {
                filaActual.push_back(dato); // Guardar el dato de la columna de interés
            }
            ++columna;
        }

        // Solo añadir la fila si el código coincide
        if (codigoCoincide)
        {
            matrizResultado.push_back(filaActual);
        }
    }

    archivoPrimero.close();

    /*// Imprimir matriz resultado para verificaciones
    for (int h = 0; h < matrizResultado.size(); h++)
    {
        for (int k = 0; k < matrizResultado[h].size(); k++)
        {
            cout << matrizResultado[h][k];
            if (k != (matrizResultado[h].size() - 1))
            {
                cout << ";";
            }
        }
        cout << endl;
    }*/
    return matrizResultado;
}