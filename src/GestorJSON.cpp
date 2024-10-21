#include "GestorJSON.h"
#include <algorithm>
#include "json.hpp"
#include "Settings.h"

using namespace std;


// FIXME: falta toda la implementacion

void GestorJSON::escribirEtiquetasJson(json& etiquetasJson, string& strCodigoSNIES, string& strNombrePrograma, vector<vector<string>>& matrizEtiquetas, int MIN_POS_ETIQUETAS, int MAX_POS_ETIQUETAS)
{
    string nombreAtributo;
    string ETIQUETAS = "Etiquetas";
    bool etiquetaValida;
    for(int filaEtiquetas = MIN_POS_ETIQUETAS; filaEtiquetas < MAX_POS_ETIQUETAS; filaEtiquetas++)
    {
        for(int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[filaEtiquetas][columnaEtiquetas];
            // Verificar que la etiqueta no sea el código SNIES o el nombre del programa
            etiquetaValida = utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES);
            etiquetaValida = etiquetaValida && (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma));
            if(etiquetaValida)
            {
                etiquetasJson[ETIQUETAS].push_back(nombreAtributo);
            }
        }
    }
}

void GestorJSON:: escribirProgramaJson(json& jsonData, string& nombrePrograma, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual, string& strCodigoSNIES,vector<string>& vectorAtributosPrograma)
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
                vectorAtributosPrograma.push_back(programaActual->consultarDatoString(nombreAtributo));
            }
        }
        for(int columnaEtiqueta = 0; columnaEtiqueta < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiqueta++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiqueta];
            if(utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES))
            {
                jsonData[escribirEtiquetasJson].push_back(programaActual->consultarDatoInt(nombreAtributo));
                vectorAtributosPrograma.push_back(programaActual->consultarDatoString(nombreAtributo));
            }
        }
    }catch (invalid_argument& e)
    {
        throw invalid_argument(e.what());
    }
    
}

void GestorJSON::imprimirConsolidadosJson(json& jsonData,ProgramaAcademico* programaActual, vector<vector<string>>& matrizEtiquetas,vector<string>& vectorAtributosPrograma)
{
    int FILA_VALORES_SEXO = 4;
    int FILA_VALORES_ANO = 5;
    int LLAVE_PRIMER_SEMESTRE = 1;
    int LLAVE_SEGUNDO_SEMESTRE = 2;
    //seleccionar consolidado en base a año y sexo
    int anoActual;
    string sexoActual;
    Consolidado* consolidadoActual;
    string llaveConsolidado;
    for(int posVectoresAno = 0; posVectoresAno < matrizEtiquetas[FILA_VALORES_ANO].size();posVectoresAno++)
    {
        anoActual = stoi(matrizEtiquetas[FILA_VALORES_ANO][posVectoresAno]);
        for(int posVectoresSexos = 0; posVectoresSexos < matrizEtiquetas[FILA_VALORES_SEXO].size(); posVectoresSexos++)
        {
            sexoActual = matrizEtiquetas[FILA_VALORES_SEXO][posVectoresSexos];
            try
            {
                consolidadoActual = programaActual->buscarConsolidado(sexoActual,anoActual, LLAVE_PRIMER_SEMESTRE);
                llaveConsolidado = sexoActual  + string("-") + to_string(anoActual) + string("-") + to_string(LLAVE_PRIMER_SEMESTRE);
                for(int elemento = 0; elemento < vectorAtributosPrograma.size(); elemento++)
                {
                    jsonData[llaveConsolidado].push_back(vectorAtributosPrograma[elemento]);
                }
                escribirConsolidadoJson(jsonData,consolidadoActual,matrizEtiquetas, llaveConsolidado);
            }
            catch (invalid_argument& e)
            {
                //De no existir el consolidado, se sigue adelante
                cout << e.what() << endl;
            }
            catch (out_of_range& e)
            {
                //Caso donde no tiene todos los atributos el consolidado
                cout << e.what() << endl;
            }
            
            try
            {
                consolidadoActual = programaActual->buscarConsolidado(sexoActual,anoActual, LLAVE_SEGUNDO_SEMESTRE);
                llaveConsolidado = sexoActual  + string("-") + to_string(anoActual) + string("-") + to_string(LLAVE_SEGUNDO_SEMESTRE);
                for(int elemento = 0; elemento < vectorAtributosPrograma.size(); elemento++)
                {
                    jsonData[llaveConsolidado].push_back(vectorAtributosPrograma[elemento]);
                }
                escribirConsolidadoJson(jsonData,consolidadoActual,matrizEtiquetas, llaveConsolidado);
            }
            catch (invalid_argument& e)
            {
                //De no existir el consolidado, se sigue adelante
                cout << e.what() << endl;
            }
            catch (out_of_range& e)
            {
                //Caso donde no tiene todos los atributos el consolidado
                cout << e.what() << endl;
            }
        }
    }
    
}

void GestorJSON:: escribirConsolidadoJson(json& jsonData, Consolidado* consolidadoActual, vector<vector<string>>& matrizEtiquetas, string& llaveConsolidado)
{
        int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
        int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;

        try
        {
            string nombreAtributo;
            for(int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO].size();columnaEtiquetas)
            {
                nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO][columnaEtiquetas];
                jsonData[llaveConsolidado].push_back(consolidadoActual->obtenerDatoString(nombreAtributo));
            }

            for(int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size(); columnaEtiquetas++)
            {
                nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO][columnaEtiquetas];
                jsonData[llaveConsolidado].push_back(consolidadoActual->obtenerDatoInt(nombreAtributo));

            }
        }catch (invalid_argument& e)
        {
        //Transformamos el invalid argument a out_of_range para la impresion de consolidados
        throw out_of_range(e.what());
        }
        
}

bool GestorJSON::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "resultados.json";
    ofstream archivoResultados(rutaCompleta);
    vector<string> vectorAtributosPrograma;

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
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;

    //Crear una lista de etiquetas adicionales desde la matriz 
    escribirEtiquetasJson(etiquetasJson, strCodigoSNIES, strNombrePrograma,  matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    jsonData["Etiquetas"] = etiquetasJson;

    //añadir los consolidados
    ProgramaAcademico* programaAcademicoActual;

    //iterar sobre los programas academicos 
    for(map<int, ProgramaAcademico*>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        vectorAtributosPrograma.clear();
        programaAcademicoActual = itProgramas->second;
        bool consolidadoValido = true;
        string nombrePrograma = programaAcademicoActual->consultarDatoString(strNombrePrograma);
        try
        {
            escribirProgramaJson(jsonData,nombrePrograma,matrizEtiquetas,programaAcademicoActual,strCodigoSNIES,vectorAtributosPrograma);
        }
        catch(invalid_argument& e)
        {
           cout << "Programa Snies '" << itProgramas->first << "' tiene atributos faltantes. " << e.what() << endl;
           consolidadoValido = false;
        }
        if(consolidadoValido)
        {
            imprimirConsolidadosJson(jsonData,programaAcademicoActual,matrizEtiquetas,vectorAtributosPrograma);
        }
        
    }

    //Cerrar el archivo una vez acabamos de escribir en el
    int INDENTACION_JSON = 4;
    archivoResultados << jsonData.dump(INDENTACION_JSON);
    estadoCreacion = true;
    archivoResultados.close();
    return estadoCreacion;
}

bool GestorJSON::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "buscados.json";
    ofstream archivoBuscados(rutaCompleta);

    if(!archivoBuscados.is_open())
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoBuscados.close();
        throw out_of_range(errorMsg);
    }
    
    json jsonData; //objeto json principal
    json etiquetasJson;
    string ETIQUETAS = "Etiquetas";
    vector<string> vectorAtributosPrograma;
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strNombrePrograma =  string("Programa Academico");
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;

    escribirEtiquetasJson(etiquetasJson,strCodigoSNIES,strNombrePrograma,matrizEtiquetas,MIN_POS_ETIQUETAS,MAX_POS_ETIQUETAS);
    jsonData[ETIQUETAS] = etiquetasJson;

    ProgramaAcademico* programaActual;
    bool consolidadoValido;
    for(list<ProgramaAcademico *>::iterator itProgramas = programasBuscados.begin(); itProgramas != programasBuscados.end(); ++itProgramas)
    {
        vectorAtributosPrograma.clear();
        programaActual = *itProgramas;
        consolidadoValido = true;
        string nombrePrograma = programaActual->consultarDatoString(strNombrePrograma);

        try
        {
            escribirProgramaJson(jsonData,nombrePrograma,matrizEtiquetas,programaActual,strCodigoSNIES,vectorAtributosPrograma);
        }catch (invalid_argument& e)
        {
            //En caso que se produzca este error se intentará seguir imrpimiendo el resto de programa
            cout << "Programa SNIES '" << programaActual->consultarDatoInt(strCodigoSNIES) << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false;
        }
        if(consolidadoValido)
        {
            imprimirConsolidadosJson(jsonData,programaActual,matrizEtiquetas,vectorAtributosPrograma);
        }
        
    }

    int INDENTACION_JSON = 4;
    archivoBuscados << jsonData.dump(INDENTACION_JSON);
    estadoCreacion = true;
    archivoBuscados.close();
    return estadoCreacion;
}

bool GestorJSON::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}