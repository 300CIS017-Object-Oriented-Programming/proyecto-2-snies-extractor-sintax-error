#include "GestorTxt.h"

//FIXME: IMPLEMENTAR :,,,)
bool GestorTxt::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "resultados.txt";
    ofstream archivoResultados(rutaCompleta);
    string delimitador = Settings::DELIMITADOR;
    if (!(archivoResultados.is_open()))
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        throw out_of_range(errorMsg);
    }

    //Imprimir las etiquetas de las columnas en la primera fila
    string fila = string("Codigo SNIES del programa") + delimitador + string("Programa Academico");
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;
    int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
    int FILA_ETIQUETAS_INT_PROGRAMAS = 1;
    int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
    int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;
    for (int filaEtiquetas = MIN_POS_ETIQUETAS; filaEtiquetas < MAX_POS_ETIQUETAS; filaEtiquetas++)
    {
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            fila += matrizEtiquetas[filaEtiquetas][columnaEtiquetas];
            if (columnaEtiquetas != matrizEtiquetas[filaEtiquetas].size() - 1)
            {
                fila += delimitador;
            }
        }
    }

    archivoResultados << fila << endl;


    return estadoCreacion;
}

bool GestorTxt::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> matrizEtiquetas)
{
}

bool GestorTxt::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}