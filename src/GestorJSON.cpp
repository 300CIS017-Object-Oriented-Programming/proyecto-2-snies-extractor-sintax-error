#include "GestorJSON.h"
#include <algorithm>
#include "json.hpp"
#include "Settings.h"

using namespace std;


// FIXME: falta toda la implementacion

void GestorJSON::escribirEtiquetasJson(json& etiquetasJson, string& strCodigoSNIES, string& strNombrePrograma, vector<vector<string>>& matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas)
{
    string nombreAtributo;
    bool etiquetaValida;
    for(int filaEtiquetas = minPosEtiquetas; filaEtiquetas < maxPosEtiquetas; filaEtiquetas++)
    {
        for(int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[filaEtiquetas][columnaEtiquetas];
            // Verificar que la etiqueta no sea el código SNIES o el nombre del programa
            etiquetaValida = utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES);
            etiquetaValida = etiquetaValida && (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma));
            if(etiquetaValida)
            {
                etiquetasJson["Etiquetas"].push_back(nombreAtributo);
            }
        }
    }
}

void GestorJSON:: escribirProgramaJson(json& jsonData, string& nombrePrograma, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual, string& strCodigoSNIES)
{
    try{
        jsonData[nombrePrograma] = to_string(programaActual->consultarDatoInt(strCodigoSNIES));
        jsonData[nombrePrograma].push_back(nombrePrograma);
        int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
        int FILA_ETIQUETAS_INT_PROGRAMAS = 1;
        string nombreAtributo;
        string strNombrePrograma =  string("Programa Academico");
        string strCodigoSNIES = string("Codigo SNIES del programa");

        for(int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS].size();columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS][columnaEtiquetas];
            if(utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma))
            {
                jsonData[nombrePrograma].push_back(programaActual->consultarDatoString(nombreAtributo));
            }
        }
        for(int columnaEtiqueta = 0; columnaEtiqueta < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiqueta++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiqueta];
            if(utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES))
            {
                jsonData[escribirEtiquetasJson].push_back(programaActual->consultarDatoInt(nombreAtributo));
            }
        }
    }catch (invalid_argument& e)
    {
        throw invalid_argument(e.what());
    }
    

}

bool GestorJSON::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "resultados.json";
    ofstream archivoResultados(rutaCompleta);

    if(!archivoResultados.is_open())
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoResultados.close();
        throw out_of_range(errorMsg);
    }

    json jsonData; //objeto json principal
    json etiquetasJson;//objeto json para las etiquetas

    //Guardar etiquetas en el json
    etiquetasJson["Etiquetas"] = "Codigo Snies del programa";
    etiquetasJson["Etiquetas"] = "Programa Academico";
    //variables de escribirEtiquetasJson
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strNombrePrograma =  string("Programa Academico");
    int minPosEtiquetas = 0;
    int maxPosEtiquetas = 3;

    //Crear una lista de etiquetas adicionales desde la matriz 
    escribirEtiquetasJson(etiquetasJson, strCodigoSNIES, strNombrePrograma,  matrizEtiquetas, minPosEtiquetas, maxPosEtiquetas);
    jsonData["Etiquetas"] = etiquetasJson;

    //añadir los consolidados
    ProgramaAcademico* programaAcademicoActual;

    //iterar sobre los programas academicos 
    for(map<int, ProgramaAcademico*>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        programaAcademicoActual = itProgramas->second;
        bool consolidadoValido = true;
        string nombrePrograma = programaAcademicoActual->consultarDatoString(strNombrePrograma);
        try
        {
            escribirProgramaJson(jsonData,nombrePrograma,matrizEtiquetas,programaAcademicoActual,strCodigoSNIES);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
}

bool GestorJSON::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas)
{
}

bool GestorJSON::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}