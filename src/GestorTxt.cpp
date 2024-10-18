#include "GestorTxt.h"

vector<int> GestorTxt::leerProgramas(string &ruta)
{
    vector<int> codigosSniesRetorno;
    ifstream archivoProgramasCsv(ruta);
    if (!(archivoProgramasCsv.is_open()))
    {
        //FIXME: Cambiar por throw
        //cout << "Archivo " << ruta << " no se pudo abrir correctamente" << endl;
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
            getline(streamLinea, dato, Settings::DELIMITADOR[0]);
            codigosSniesRetorno.push_back(stoi(dato));
        }
    }
    archivoProgramasCsv.close();
    return codigosSniesRetorno;
}

vector<vector<string>> GestorTxt::leerArchivo(string &rutaBase, vector<string> &etiquetasColumnas, vector<int> &codigosSnies)
{
}

bool GestorTxt::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<string> etiquetasColumnas)
{
}

bool GestorTxt::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<string> etiquetasColumnas)
{
}

bool GestorTxt::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}