#include "GestorTxt.h"

void GestorTxt::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas)
{
    string rutaCompleta = ruta + "resultados.txt";

    // Abrimos el archivo donde se van a escribir los resultados
    ofstream archivoResultados(rutaCompleta);
    string delimitador = Settings::DELIMITADOR;

    // Verificamos si el archivo se abrió correctamente
    if (!(archivoResultados.is_open()))
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoResultados.close();
        throw out_of_range(errorMsg); // Lanzamos excepción si no se pudo abrir
    }

    // Imprimimos las etiquetas de las columnas en la primera fila
    string strCodigoSNIES = string("Código SNIES del programa");
    string strnombrePrograma = string("Programa Académico");
    string fila;
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;

    // Llamada a método auxiliar para escribir las etiquetas de las columnas
    escribirEtiquetas(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    archivoResultados << fila << endl;

    // Iteramos sobre el mapa de programas académicos para imprimir cada consolidado
    ProgramaAcademico *programaActual;
    bool consolidadoValido;

    for (map<int, ProgramaAcademico *>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        fila.clear(); // Limpiamos la fila para el nuevo programa
        programaActual = itProgramas->second;
        consolidadoValido = true;

        // Intentamos escribir la información del programa en la fila
        try
        {
            escribirPrograma(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, programaActual);
        }
        catch (invalid_argument &e)
        {
            // Capturamos el error si hay atributos faltantes y seguimos con los demás programas
            cout << "Programa SNIES '" << itProgramas->first << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false;
        }

        // Si la información del programa es válida, imprimimos los consolidados asociados
        if (consolidadoValido)
        {
            imprimirConsolidados(fila, archivoResultados, delimitador, matrizEtiquetas, programaActual);
        }
    }

    // Cerramos el archivo una vez terminamos de escribir los resultados
    archivoResultados.close();
}

void GestorTxt::escribirEtiquetas(string &strCodigoSNIES, string &strNombrePrograma, string &fila, string &delimitador, vector<vector<string>> &matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas)
{
    // Estructura de las etiquetas: CodigoSnies, Nombre Programa, etiquetas del programa y consolidados
    fila = strCodigoSNIES + delimitador + strNombrePrograma + delimitador;
    string nombreAtributo;
    bool etiquetaValida;

    // Iteramos sobre las etiquetas y agregamos aquellas que no sean duplicadas
    for (int filaEtiquetas = minPosEtiquetas; filaEtiquetas <= maxPosEtiquetas; filaEtiquetas++)
    {
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[filaEtiquetas][columnaEtiquetas];
            // Evitamos duplicar el código SNIES o el nombre del programa
            etiquetaValida = utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES);
            etiquetaValida = etiquetaValida && (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma));

            // Si es válida, la agregamos a la fila
            if (etiquetaValida)
            {
                fila += nombreAtributo;
            }

            // Agregamos el delimitador si no es la última columna de la última fila
            if ((columnaEtiquetas != matrizEtiquetas[filaEtiquetas].size() - 1) || (filaEtiquetas != maxPosEtiquetas))
            {
                fila += delimitador;
            }
        }
    }
}

void GestorTxt::escribirPrograma(string &strCodigoSNIES, string &strNombrePrograma, string &fila, string &delimitador, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual)
{
    try
    {
        // Escribimos el código SNIES y el nombre del programa en la fila
        fila = to_string(programaActual->consultarDatoInt(strCodigoSNIES)) + delimitador;
        fila += programaActual->consultarDatoString(strNombrePrograma) + delimitador;

        // Escribimos la información string del programa
        int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
        int FILA_ETIQUETAS_INT_PROGRAMAS = 1;
        string nombreAtributo;

        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS][columnaEtiquetas];

            // Evitamos duplicar el nombre del programa
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma))
            {
                fila += programaActual->consultarDatoString(nombreAtributo) + delimitador;
            }
        }

        // Escribimos la información numérica del programa
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiquetas];

            // Evitamos duplicar el código SNIES
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES))
            {
                fila += to_string(programaActual->consultarDatoInt(nombreAtributo)) + delimitador;
            }
        }
    }
    catch (invalid_argument &e)
    {
        // Si ocurre un error, lo propagamos
        throw invalid_argument(e.what());
    }
}

void GestorTxt::imprimirConsolidados(string &fila, ofstream &archivoResultados, string &delimitador, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual)
{
    int FILA_VALORES_SEXO = 4;
    int FILA_VALORES_ANO = 5;
    int LLAVE_PRIMER_SEMESTRE = 1;
    int LLAVE_SEGUNDO_SEMESTRE = 2;
    // Vamos a seleccionar el consolidado apropiado con base en el sexo y año
    int anoActual;
    string sexoActual;
    Consolidado *consolidadoActual;
    string infoPrograma = fila;
    for (int posVectorAnos = 0; posVectorAnos < matrizEtiquetas[FILA_VALORES_ANO].size(); posVectorAnos++)
    {
        anoActual = stoi(matrizEtiquetas[FILA_VALORES_ANO][posVectorAnos]);
        for (int posVectorSexos = 0; posVectorSexos < matrizEtiquetas[FILA_VALORES_SEXO].size(); posVectorSexos++)
        {
            sexoActual = matrizEtiquetas[FILA_VALORES_SEXO][posVectorSexos];
            // Escribimos el consolidado del primer semestre del año e imprimimos al archivo
            try
            {
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_PRIMER_SEMESTRE);
                fila = infoPrograma;
                escribirConsolidado(fila, delimitador, consolidadoActual, matrizEtiquetas);
                // Imprimimos al archivo una fila correspondiente a la informacion de un consolidado
                archivoResultados << fila << endl;
            }
            catch (invalid_argument &e)
            {
                // De no existir el consolidado, se sigue adelante
                cout << e.what() << endl;
            }
            catch (out_of_range &e)
            {
                // Caso donde no tiene todos los atributos el consolidado
                cout << e.what() << endl;
            }

            // Escribimos el consolidado del segundo semestre del año e imprimimos al archivo
            try
            {
                // Manda invalid argument si no existe el consolidado
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_SEGUNDO_SEMESTRE);
                fila = infoPrograma;
                // Manda out of range si no tiene todos los atributos
                escribirConsolidado(fila, delimitador, consolidadoActual, matrizEtiquetas);
                // Imprimimos al archivo una fila correspondiente a la informacion de un consolidado
                archivoResultados << fila << endl;
            }
            catch (invalid_argument &e)
            {
                // De no existir el consolidado, se sigue adelante
                cout << e.what() << endl;
            }
            catch (out_of_range &e)
            {
                // Caso donde no tiene todos los atributos el consolidado
                cout << e.what() << endl;
            }
        }
    }
}

void GestorTxt::escribirConsolidado(string &fila, string &delimitador, Consolidado *consolidadoActual, vector<vector<string>> &matrizEtiquetas)
{
    // Filas de la matriz que contienen las etiquetas para datos string e int respectivamente
    int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
    int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;

    // Intentamos escribir los datos string e int del consolidado
    try
    {
        // Variable para almacenar temporalmente el nombre del atributo a buscar
        string nombreAtributo;

        // Iteramos sobre las etiquetas que corresponden a datos de tipo string en el consolidado
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO].size(); columnaEtiquetas++)
        {
            // Obtenemos el nombre del atributo a partir de las etiquetas
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO][columnaEtiquetas];
            // Agregamos el dato obtenido del consolidado a la fila, seguido del delimitador
            fila += consolidadoActual->obtenerDatoString(nombreAtributo) + delimitador;
        }

        // Iteramos sobre las etiquetas que corresponden a datos de tipo int en el consolidado
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size(); columnaEtiquetas++)
        {
            // Obtenemos el nombre del atributo a partir de las etiquetas
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO][columnaEtiquetas];
            // Agregamos el dato obtenido del consolidado a la fila como una cadena de texto
            fila += to_string(consolidadoActual->obtenerDatoInt(nombreAtributo));

            // Si no es la última columna, añadimos el delimitador después del dato
            if (columnaEtiquetas != matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size() - 1)
            {
                fila += delimitador;
            }
        }
    }
    catch (invalid_argument &e)
    {
        // Si se lanza una excepción de tipo invalid_argument, la transformamos en out_of_range para un mejor manejo
        throw out_of_range(e.what());
    }
}

void GestorTxt::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas)
{
    string rutaCompleta = ruta + "buscados.txt";
    ofstream archivoBuscados(rutaCompleta);
    string delimitador = Settings::DELIMITADOR;
    if (!(archivoBuscados.is_open()))
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoBuscados.close();
        throw out_of_range(errorMsg);
    }

    // Imprimir las etiquetas de las columnas en la primera fila
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strnombrePrograma = string("Programa Academico");
    string fila;
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;
    // Metodo privado auxiliar para escribir las etiquetas de las columnas
    escribirEtiquetas(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    archivoBuscados << fila << endl;

    // Iteramos sobre la lista de los programasAcademicos para imprimir en cada fila 1 consolidado
    ProgramaAcademico *programaActual;
    bool consolidadoValido;
    for (list<ProgramaAcademico *>::iterator itProgramas = programasBuscados.begin(); itProgramas != programasBuscados.end(); ++itProgramas)
    {
        fila.clear();
        programaActual = *itProgramas;
        consolidadoValido = true;
        // Tratamos de escribir en la fila la informacion del programa
        try
        {
            escribirPrograma(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, programaActual);
        }
        catch (invalid_argument &e)
        {
            // En caso que se produzca este error se intentará seguir imrpimiendo el resto de programa
            cout << "Programa SNIES '" << programaActual->consultarDatoInt(strCodigoSNIES) << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false;
        }
        if (consolidadoValido)
        {
            imprimirConsolidados(fila, archivoBuscados, delimitador, matrizEtiquetas, programaActual);
        }
    }

    // Cerramos el archivo una vez terminamos de iterar sobre los programas
    archivoBuscados.close();
}

void GestorTxt::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
    // Definimos la ruta completa del archivo que se va a crear
    string rutaCompleta = ruta + "extras.txt";
    // Creamos un archivo de salida en la ruta especificada
    ofstream archivoExtras(rutaCompleta);
    // Obtenemos el delimitador de los ajustes globales
    string delimitador = Settings::DELIMITADOR;

    // Verificamos si el archivo se abrió correctamente
    if (!(archivoExtras.is_open()))
    {
        // Si hay un error al abrir el archivo, cerramos y lanzamos una excepción
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoExtras.close();
        throw out_of_range(errorMsg);
    }

    // Iteramos sobre las filas de los datos a imprimir
    for (int fila = 0; fila < datosAImprimir.size(); fila++)
    {
        // Iteramos sobre las columnas de la fila actual
        for (int columna = 0; columna < datosAImprimir[fila].size(); columna++)
        {
            // Escribimos el valor actual en el archivo
            archivoExtras << datosAImprimir[fila][columna];

            // Si no es la última columna, añadimos el delimitador
            if (columna != datosAImprimir[fila].size() - 1)
            {
                archivoExtras << delimitador;
            }
        }
        // Saltamos a la siguiente línea después de cada fila
        archivoExtras << endl;
    }

    // Cerramos el archivo después de terminar de escribir
    archivoExtras.close();
}
