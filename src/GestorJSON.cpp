#include "GestorJSON.h"
#include <algorithm>
#include "json.hpp"
#include "Settings.h"

using namespace std;

void GestorJSON::escribirEtiquetasJson(json &etiquetasJson, string &strCodigoSNIES, string &strNombrePrograma, vector<vector<string>> &matrizEtiquetas, int MIN_POS_ETIQUETAS, int MAX_POS_ETIQUETAS)
{
    // Variable para almacenar el nombre de la etiqueta actual
    string nombreAtributo;
    // Clave para almacenar las etiquetas en el objeto JSON
    string ETIQUETAS = "Etiquetas";
    // Variable para verificar la validez de la etiqueta
    bool etiquetaValida;

    if (!etiquetasJson.contains(ETIQUETAS)) {
        etiquetasJson[ETIQUETAS] = nlohmann::json::array();
    }
    
    // Iterar sobre las filas de etiquetas en el rango especificado
    for (int filaEtiquetas = MIN_POS_ETIQUETAS; filaEtiquetas < MAX_POS_ETIQUETAS; filaEtiquetas++)
    {
        // Iterar sobre las columnas de etiquetas en la fila actual
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            // Obtener el nombre de la etiqueta de la matriz
            nombreAtributo = matrizEtiquetas[filaEtiquetas][columnaEtiquetas];

            // Verificar que la etiqueta no sea el código SNIES o el nombre del programa
            etiquetaValida = utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES);
            etiquetaValida = etiquetaValida && (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma));

            // Si la etiqueta es válida, agregarla al objeto JSON
            if (etiquetaValida)
            {
                etiquetasJson[ETIQUETAS].push_back(nombreAtributo);
            }
        }
    }
}

void GestorJSON::escribirProgramaJson(json &jsonData, string &nombrePrograma, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual, string &strCodigoSNIES, vector<string> &vectorAtributosPrograma)
{
    try
    {
        jsonData[nombrePrograma] = nlohmann::json::array();
        // Asignar el código SNIES del programa al objeto JSON bajo el nombre del programa
        jsonData[nombrePrograma].push_back(to_string(programaActual->consultarDatoInt(strCodigoSNIES)));
        // Añadir el nombre del programa al objeto JSON
        jsonData[nombrePrograma].push_back((nombrePrograma));

        // Definición de índices para las filas de etiquetas
        int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
        int FILA_ETIQUETAS_INT_PROGRAMAS = 1;

        string nombreAtributo;                                       // Variable para almacenar el nombre de la etiqueta
        string strNombrePrograma = string("Programa Academico");     // Nombre del atributo que se debe omitir
        string strCodigoSNIES = string("Codigo SNIES del programa"); // Código SNIES que se debe omitir

        // Iterar sobre las etiquetas de tipo string
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS][columnaEtiquetas];
            // Verificar que la etiqueta no sea el nombre del programa
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma))
            {
                jsonData[nombrePrograma] = nlohmann::json::array();
                // Agregar el dato correspondiente al objeto JSON
                jsonData[nombrePrograma].push_back((programaActual->consultarDatoString(nombreAtributo)));
                // Añadir el dato al vector de atributos del programa
                vectorAtributosPrograma.push_back(programaActual->consultarDatoString(nombreAtributo));
            }
        }

        // Iterar sobre las etiquetas de tipo int
        for (int columnaEtiqueta = 0; columnaEtiqueta < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiqueta++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiqueta];
            // Verificar que la etiqueta no sea el código SNIES
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES))
            {
                // Agregar el dato correspondiente al objeto JSON
                jsonData[nombrePrograma].push_back((programaActual->consultarDatoInt(nombreAtributo)));
                // Añadir el dato al vector de atributos del programa
                vectorAtributosPrograma.push_back(programaActual->consultarDatoString(nombreAtributo));
            }
        }
    }
    catch (invalid_argument &e)
    {
        // Re-lanzar la excepción si se produce un argumento no válido
        throw invalid_argument(e.what());
    }
}

void GestorJSON::imprimirConsolidadosJson(json &jsonData, ProgramaAcademico *programaActual, vector<vector<string>> &matrizEtiquetas, vector<string> &vectorAtributosPrograma)
{
    // Definición de índices para las filas de valores de sexo y año
    int FILA_VALORES_SEXO = 4;
    int FILA_VALORES_ANO = 5;
    // Llaves para los semestres
    int LLAVE_PRIMER_SEMESTRE = 1;
    int LLAVE_SEGUNDO_SEMESTRE = 2;

    // Variables para seleccionar el consolidado en base a año y sexo
    int anoActual;                  // Año actual a procesar
    string sexoActual;              // Sexo actual a procesar
    Consolidado *consolidadoActual; // Puntero al consolidado actual
    string llaveConsolidado;        // Llave para almacenar en el JSON

    // Iterar sobre los años disponibles
    for (int posVectoresAno = 0; posVectoresAno < matrizEtiquetas[FILA_VALORES_ANO].size(); posVectoresAno++)
    {
        anoActual = stoi(matrizEtiquetas[FILA_VALORES_ANO][posVectoresAno]); // Convertir el año a entero

        // Iterar sobre los sexos disponibles
        for (int posVectoresSexos = 0; posVectoresSexos < matrizEtiquetas[FILA_VALORES_SEXO].size(); posVectoresSexos++)
        {
            sexoActual = matrizEtiquetas[FILA_VALORES_SEXO][posVectoresSexos]; // Obtener el sexo actual

            try
            {
                // Buscar el consolidado para el primer semestre
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_PRIMER_SEMESTRE);
                llaveConsolidado = sexoActual + string("-") + to_string(anoActual) + string("-") + to_string(LLAVE_PRIMER_SEMESTRE); // Crear la llave para el JSON

                // Añadir los atributos del programa al JSON
                for (int elemento = 0; elemento < vectorAtributosPrograma.size(); elemento++)
                {
                    jsonData[llaveConsolidado] = nlohmann::json::array();
                    jsonData[llaveConsolidado].push_back(vectorAtributosPrograma[elemento]);
                }
                // Escribir el consolidado en el JSON
                escribirConsolidadoJson(jsonData, consolidadoActual, matrizEtiquetas, llaveConsolidado);
            }
            catch (invalid_argument &e)
            {
                // Manejar el caso donde no existe el consolidado
                cout << e.what() << endl;
            }
            catch (out_of_range &e)
            {
                // Manejar el caso donde falta algún atributo en el consolidado
                cout << e.what() << endl;
            }

            try
            {
                // Buscar el consolidado para el segundo semestre
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_SEGUNDO_SEMESTRE);
                llaveConsolidado = sexoActual + string("-") + to_string(anoActual) + string("-") + to_string(LLAVE_SEGUNDO_SEMESTRE); // Crear la llave para el JSON

                // Añadir los atributos del programa al JSON
                for (int elemento = 0; elemento < vectorAtributosPrograma.size(); elemento++)
                {
                    jsonData[llaveConsolidado].push_back(vectorAtributosPrograma[elemento]);
                }
                // Escribir el consolidado en el JSON
                escribirConsolidadoJson(jsonData, consolidadoActual, matrizEtiquetas, llaveConsolidado);
            }
            catch (invalid_argument &e)
            {
                // Manejar el caso donde no existe el consolidado
                cout << e.what() << endl;
            }
            catch (out_of_range &e)
            {
                // Manejar el caso donde falta algún atributo en el consolidado
                cout << e.what() << endl;
            }
        }
    }
}

void GestorJSON::escribirConsolidadoJson(json &jsonData, Consolidado *consolidadoActual, vector<vector<string>> &matrizEtiquetas, string &llaveConsolidado)
{
    // Definición de índices para las filas de etiquetas de atributos string e int del consolidado
    int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
    int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;

    try
    {
        string nombreAtributo; // Variable para almacenar el nombre del atributo

        // Iterar sobre las etiquetas de tipo string
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO][columnaEtiquetas]; // Obtener el nombre del atributo
            // Obtener el dato de tipo string del consolidado actual y agregarlo al JSON bajo la llave especificada
            jsonData[llaveConsolidado].push_back(consolidadoActual->obtenerDatoString(nombreAtributo));
        }

        // Iterar sobre las etiquetas de tipo int
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO][columnaEtiquetas]; // Obtener el nombre del atributo
            // Obtener el dato de tipo int del consolidado actual y agregarlo al JSON bajo la llave especificada
            jsonData[llaveConsolidado].push_back(consolidadoActual->obtenerDatoInt(nombreAtributo));
        }
    }
    catch (invalid_argument &e)
    {
        // Transformamos el invalid argument a out_of_range para la impresión de consolidados
        throw out_of_range(e.what());
    }
}

void GestorJSON::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas)
{
    string rutaCompleta = ruta + "resultados.json"; // Construcción de la ruta completa del archivo JSON
    ofstream archivoResultados(rutaCompleta);       // Apertura del archivo para escritura
    vector<string> vectorAtributosPrograma;         // Vector para almacenar atributos del programa académico

    // Verificar si el archivo se abrió correctamente
    if (!archivoResultados.is_open())
    {
        // Si no se pudo abrir el archivo, se lanza una excepción con el mensaje de error
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoResultados.close();
        throw out_of_range(errorMsg);
    }

    json jsonData;      // Objeto JSON principal que contendrá todos los datos
    json etiquetasJson; // Objeto JSON para almacenar las etiquetas

    // Guardar etiquetas en el JSON
    etiquetasJson["Etiquetas"] = "Codigo Snies del programa"; // Agrega la etiqueta para el código SNIES
    etiquetasJson["Etiquetas"] = "Programa Academico";        // Agrega la etiqueta para el programa académico

    // Variables necesarias para la función escribirEtiquetasJson
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strNombrePrograma = string("Programa Academico");
    int MIN_POS_ETIQUETAS = 0; // Posición mínima para las etiquetas
    int MAX_POS_ETIQUETAS = 3; // Posición máxima para las etiquetas

    // Crear una lista de etiquetas adicionales desde la matriz
    escribirEtiquetasJson(etiquetasJson, strCodigoSNIES, strNombrePrograma, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    jsonData["Etiquetas"] = etiquetasJson; // Almacena las etiquetas en el objeto JSON principal

    // Añadir los consolidados de cada programa académico
    ProgramaAcademico *programaAcademicoActual;

    // Iterar sobre los programas académicos en el mapa
    for (map<int, ProgramaAcademico *>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        vectorAtributosPrograma.clear();                                                         // Limpiar el vector de atributos para cada programa
        programaAcademicoActual = itProgramas->second;                                           // Obtener el programa académico actual
        bool consolidadoValido = true;                                                           // Variable para verificar la validez del consolidado
        string nombrePrograma = programaAcademicoActual->consultarDatoString(strNombrePrograma); // Obtener el nombre del programa académico

        try
        {
            // Intentar escribir los datos del programa académico en el objeto JSON
            escribirProgramaJson(jsonData, nombrePrograma, matrizEtiquetas, programaAcademicoActual, strCodigoSNIES, vectorAtributosPrograma);
        }
        catch (invalid_argument &e)
        {
            // Si se lanza una excepción por atributos faltantes, imprimir mensaje de error
            cout << "Programa Snies '" << itProgramas->first << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false; // Marcar el consolidado como no válido
        }

        // Si el consolidado es válido, proceder a imprimir los consolidados en JSON
        if (consolidadoValido)
        {
            imprimirConsolidadosJson(jsonData, programaAcademicoActual, matrizEtiquetas, vectorAtributosPrograma);
        }
    }

    // Cerrar el archivo una vez acabamos de escribir en él
    int INDENTACION_JSON = 4;                             // Definir la cantidad de espacios para la indentación del JSON
    archivoResultados << jsonData.dump(INDENTACION_JSON); // Escribir los datos en el archivo con indentación
    archivoResultados.close();                            // Cerrar el archivo
}

void GestorJSON::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas)
{
    string rutaCompleta = ruta + "buscados.json"; // Construcción de la ruta completa del archivo JSON
    ofstream archivoBuscados(rutaCompleta);       // Apertura del archivo para escritura

    // Verificar si el archivo se abrió correctamente
    if (!archivoBuscados.is_open())
    {
        // Si no se pudo abrir el archivo, se lanza una excepción con el mensaje de error
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoBuscados.close();
        throw out_of_range(errorMsg);
    }

    json jsonData;                                               // Objeto JSON principal que contendrá todos los datos
    json etiquetasJson;                                          // Objeto JSON para almacenar las etiquetas
    string ETIQUETAS = "Etiquetas";                              // Nombre clave para las etiquetas
    vector<string> vectorAtributosPrograma;                      // Vector para almacenar atributos del programa académico
    string strCodigoSNIES = string("Código SNIES del programa"); // Nombre de atributo para el código SNIES
    string strNombrePrograma = string("Programa Académico");     // Nombre de atributo para el programa académico
    int MIN_POS_ETIQUETAS = 0;                                   // Posición mínima para las etiquetas
    int MAX_POS_ETIQUETAS = 3;                                   // Posición máxima para las etiquetas

    // Llamar a la función para escribir etiquetas adicionales en el JSON
    escribirEtiquetasJson(etiquetasJson, strCodigoSNIES, strNombrePrograma, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    jsonData[ETIQUETAS] = etiquetasJson; // Almacena las etiquetas en el objeto JSON principal

    ProgramaAcademico *programaActual; // Puntero para el programa académico actual
    bool consolidadoValido;            // Variable para verificar la validez del consolidado

    // Iterar sobre la lista de programas académicos buscados
    for (list<ProgramaAcademico *>::iterator itProgramas = programasBuscados.begin(); itProgramas != programasBuscados.end(); ++itProgramas)
    {
        vectorAtributosPrograma.clear();                                                // Limpiar el vector de atributos para cada programa
        programaActual = *itProgramas;                                                  // Obtener el programa académico actual
        consolidadoValido = true;                                                       // Inicializar la validez del consolidado
        string nombrePrograma = programaActual->consultarDatoString(strNombrePrograma); // Obtener el nombre del programa académico

        try
        {
            // Intentar escribir los datos del programa académico en el objeto JSON
            escribirProgramaJson(jsonData, nombrePrograma, matrizEtiquetas, programaActual, strCodigoSNIES, vectorAtributosPrograma);
        }
        catch (invalid_argument &e)
        {
            // Si se lanza una excepción por atributos faltantes, imprimir mensaje de error
            cout << "Programa SNIES '" << programaActual->consultarDatoInt(strCodigoSNIES) << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false; // Marcar el consolidado como no válido
        }

        // Si el consolidado es válido, proceder a imprimir los consolidados en JSON
        if (consolidadoValido)
        {
            imprimirConsolidadosJson(jsonData, programaActual, matrizEtiquetas, vectorAtributosPrograma);
        }
    }

    // Cerrar el archivo una vez acabamos de escribir en él
    int INDENTACION_JSON = 4;                           // Definir la cantidad de espacios para la indentación del JSON
    archivoBuscados << jsonData.dump(INDENTACION_JSON); // Escribir los datos en el archivo con indentación
    archivoBuscados.close();                            // Cerrar el archivo
}

void GestorJSON::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
    string rutaCompleta = ruta + "extras.json"; // Construcción de la ruta completa del archivo JSON
    ofstream archivoExtras(rutaCompleta);       // Apertura del archivo para escritura

    // Verificar si el archivo se abrió correctamente
    if (!(archivoExtras.is_open()))
    {
        // Si no se pudo abrir el archivo, se lanza una excepción con el mensaje de error
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoExtras.close();        // Cerrar el archivo si está abierto
        throw out_of_range(errorMsg); // Lanzar excepción
    }

    json jsonData; // Objeto JSON que contendrá los datos a imprimir

    // Iterar sobre las filas y columnas del vector de datos a imprimir
    for (int fila = 0; fila < datosAImprimir.size(); fila++)
    {
        for (int columna = 0; columna < datosAImprimir[fila].size(); columna++)
        {
            // Agregar cada elemento a la estructura JSON correspondiente a su fila
            jsonData[fila] += (datosAImprimir[fila][columna]) + Settings::DELIMITADOR;

        }
    }

    int INDENTACION_JSON = 4;                         // Definir la cantidad de espacios para la indentación del JSON
    archivoExtras << jsonData.dump(INDENTACION_JSON); // Escribir los datos en el archivo con indentación
    archivoExtras.close();                            // Cerrar el archivo
}
