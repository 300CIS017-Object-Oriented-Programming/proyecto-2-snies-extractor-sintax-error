#include "GestorTxt.h"

//FIXME: IMPLEMENTAR :,,,)
bool GestorTxt::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "resultados.txt";
    ofstream archivoResultados(rutaCompleta);
    string delimitador = Settings::DELIMITADOR;
    if (!(archivoResultados.is_open()))
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoResultados.close();
        throw out_of_range(errorMsg);
    }

    //Imprimir las etiquetas de las columnas en la primera fila
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strnombrePrograma =  string("Programa Academico");
    string fila;
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;
    int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
    int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;
    //Metodo privado auxiliar para escribir el resto de etiquetas
    escribirEtiquetas(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    archivoResultados << fila << endl;

    //Iteramos sobre el mapa de los programasAcademicos para imprimir en cada fila 1 consolidado
    ProgramaAcademico * programaActual;
    bool consolidadoValido = true;
    for (map<int, ProgramaAcademico *>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        fila.clear();
        programaActual = itProgramas->second;
        try
        {
            escribirPrograma(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, programaActual);
        } catch (invalid_argument& e)
        {
            cout << "Programa " << itProgramas->first << "tiene atributos faltantes. " << e.what() << endl;
        }

    }

    estadoCreacion = true;
    archivoResultados.close();
    return estadoCreacion;
}

void GestorTxt::escribirEtiquetas(string& strCodigoSNIES, string& strnombrePrograma, string& fila, string& delimitador, vector<vector<string>>& matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas)
{
    /*Orden de las Etiquetas
     CodigoSnies, Nombre Programa, (etiquetas Tipo String Programa), ...
    ... (etiquetas Tipo String Programa), (etiquetas Tipo String Consolidado)
     */
    fila = strCodigoSNIES + delimitador + strnombrePrograma;
    for (int filaEtiquetas = minPosEtiquetas; filaEtiquetas < maxPosEtiquetas; filaEtiquetas++)
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
}

void GestorTxt::escribirPrograma(string& strCodigoSNIES, string& strnombrePrograma, string& fila, string& delimitador, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual)
{
    try
    {
        //Empezamos por imprimir el codigo SNIES y el nombre del programa en la fila
        fila = to_string(programaActual->consultarDatoInt(strCodigoSNIES)) + delimitador;
        fila += to_string(programaActual->consultarDatoInt(strnombrePrograma)) + delimitador;

        //Seguimos por imprimir la informacion string e int del programa en la fila, respectivamente
        int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
        int FILA_ETIQUETAS_INT_PROGRAMAS = 1;
        string nombreAtributo;
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS][columnaEtiquetas];
            fila += programaActual->consultarDatoString(nombreAtributo) + delimitador;
        }
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiquetas];
            fila += to_string(programaActual->consultarDatoInt(nombreAtributo)) + delimitador;
        }
    } catch (invalid_argument& e)
    {
        throw invalid_argument(e.what());
    }
}



bool GestorTxt::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas)
{
}

bool GestorTxt::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}