#include "GestorTxt.h"

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
    //Metodo privado auxiliar para escribir las etiquetas de las columnas
    escribirEtiquetas(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    archivoResultados << fila << endl;

    //Iteramos sobre el mapa de los programasAcademicos para imprimir en cada fila 1 consolidado
    ProgramaAcademico * programaActual;
    bool consolidadoValido;
    for (map<int, ProgramaAcademico *>::iterator itProgramas = mapadeProgramasAcademicos.begin(); itProgramas != mapadeProgramasAcademicos.end(); ++itProgramas)
    {
        fila.clear();
        programaActual = itProgramas->second;
        consolidadoValido = true;
        //Tratamos de escribir en la fila la informacion del programa
        try
        {
            escribirPrograma(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, programaActual);
        } catch (invalid_argument& e)
        {
            //En caso que se produzca este error se intentará seguir imrpimiendo el resto de programa
            cout << "Programa SNIES '" << itProgramas->first << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false;
        }
        if (consolidadoValido)
        {
            imprimirConsolidados(fila, archivoResultados,delimitador, matrizEtiquetas, programaActual);
        }

    }

    //Cerramos el archivo una vez terminamos de iterar sobre los programas
    estadoCreacion = true;
    archivoResultados.close();
    return estadoCreacion;
}

void GestorTxt::escribirEtiquetas(string& strCodigoSNIES, string& strNombrePrograma, string& fila, string& delimitador, vector<vector<string>>& matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas)
{
    /*Orden de las Etiquetas
     CodigoSnies, Nombre Programa, (etiquetas Tipo String Programa), (etiquetas Tipo Int Programa)...
    ... (etiquetas Tipo String Consolidado), (etiquetas Tipo Int Consolidado)
     */
    fila = strCodigoSNIES + delimitador + strNombrePrograma;
    string nombreAtributo;
    bool etiquetaValida;
    for (int filaEtiquetas = minPosEtiquetas; filaEtiquetas < maxPosEtiquetas; filaEtiquetas++)
    {
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[filaEtiquetas].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[filaEtiquetas][columnaEtiquetas];
            //Verificamos que la etiqueta que vamos a escribir no sea el codigo snies o nombre que ya escribimos
            etiquetaValida = utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES);
            etiquetaValida = etiquetaValida && (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma));
            if (etiquetaValida)
            {
                fila += nombreAtributo;
            }
            if (columnaEtiquetas != matrizEtiquetas[filaEtiquetas].size() - 1)
            {
                fila += delimitador;
            }
        }
    }
}

void GestorTxt::escribirPrograma(string& strCodigoSNIES, string& strNombrePrograma, string& fila, string& delimitador, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual)
{
    try
    {
        //Empezamos por imprimir el codigo SNIES y el nombre del programa en la fila
        fila = to_string(programaActual->consultarDatoInt(strCodigoSNIES)) + delimitador;
        fila += programaActual->consultarDatoString(strNombrePrograma) + delimitador;

        //Seguimos por imprimir la informacion string e int del programa en la fila, respectivamente
        int FILA_ETIQUETAS_STRING_PROGRAMAS = 0;
        int FILA_ETIQUETAS_INT_PROGRAMAS = 1;
        string nombreAtributo;
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_PROGRAMAS][columnaEtiquetas];
            //Verificamos que el dato que vamos a escribir no sea el nombre del programa que ya escribimos
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strNombrePrograma))
            {
                fila += programaActual->consultarDatoString(nombreAtributo) + delimitador;
            }
        }
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS].size(); columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_PROGRAMAS][columnaEtiquetas];
            //Verificamos que el dato que vamos a escribir no sea el codigo SNIES que ya escribimos
            if (utilidadObj.minusculasSinEspacios(nombreAtributo) != utilidadObj.minusculasSinEspacios(strCodigoSNIES))
            {
                fila += to_string(programaActual->consultarDatoInt(nombreAtributo)) + delimitador;
            }
        }
    } catch (invalid_argument& e)
    {
        throw invalid_argument(e.what());
    }
}

void GestorTxt::imprimirConsolidados(string& fila, ofstream& archivoResultados, string& delimitador, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual)
{
    int FILA_VALORES_SEXO = 4;
    int FILA_VALORES_ANO = 5;
    int LLAVE_PRIMER_SEMESTRE = 1;
    int LLAVE_SEGUNDO_SEMESTRE = 2;
    //Vamos a seleccionar el consolidado apropiado con base en el sexo y año
    int anoActual;
    string sexoActual;
    Consolidado* consolidadoActual;
    string infoPrograma = fila;
    for (int posVectorAnos = 0; posVectorAnos < matrizEtiquetas[FILA_VALORES_ANO].size(); posVectorAnos++)
    {
        anoActual = stoi(matrizEtiquetas[FILA_VALORES_ANO][posVectorAnos]);
        for (int posVectorSexos = 0; posVectorSexos < matrizEtiquetas[FILA_VALORES_SEXO].size(); posVectorSexos++)
        {
            sexoActual = matrizEtiquetas[FILA_VALORES_SEXO][posVectorSexos];
            //Escribimos el consolidado del primer semestre del año e imprimimos al archivo
            try
            {
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_PRIMER_SEMESTRE);
                fila = infoPrograma;
                escribirConsolidado(fila, delimitador, consolidadoActual,matrizEtiquetas);
                //Imprimimos al archivo una fila correspondiente a la informacion de un consolidado
                archivoResultados << fila << endl;
            } catch (invalid_argument& e)
            {
                //De no existir el consolidado, se sigue adelante
                cout << e.what() << endl;
            }
            catch (out_of_range& e)
            {
                //Caso donde no tiene todos los atributos el consolidado
                cout << e.what() << endl;
            }

            //Escribimos el consolidado del segundo semestre del año e imprimimos al archivo
            try
            {
                //Manda invalid argument si no existe el consolidado
                consolidadoActual = programaActual->buscarConsolidado(sexoActual, anoActual, LLAVE_SEGUNDO_SEMESTRE);
                fila = infoPrograma;
                //Manda out of range si no tiene todos los atributos
                escribirConsolidado(fila, delimitador, consolidadoActual,matrizEtiquetas);
                //Imprimimos al archivo una fila correspondiente a la informacion de un consolidado
                archivoResultados << fila << endl;
            } catch (invalid_argument& e)
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

void GestorTxt::escribirConsolidado(string& fila, string& delimitador, Consolidado* consolidadoActual, vector<vector<string>>& matrizEtiquetas)
{
    int FILA_ETIQUETAS_STRING_CONSOLIDADO = 2;
    int FILA_ETIQUETAS_INT_CONSOLIDADO = 3;
    //Procedemos a escribir la informacion string e int del consolidado, respectivamente
    try
    {
        //Cualquier atributo faltante arroja invalid argument
        string nombreAtributo;
        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO].size();columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_STRING_CONSOLIDADO][columnaEtiquetas];
            fila += consolidadoActual->obtenerDatoString(nombreAtributo) + delimitador;
        }

        for (int columnaEtiquetas = 0; columnaEtiquetas < matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size();columnaEtiquetas++)
        {
            nombreAtributo = matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO][columnaEtiquetas];
            fila += to_string(consolidadoActual->obtenerDatoInt(nombreAtributo));

            if (columnaEtiquetas != matrizEtiquetas[FILA_ETIQUETAS_INT_CONSOLIDADO].size() - 1)
            {
                fila += delimitador;
            }
        }

    } catch (invalid_argument& e)
    {
        //Transformamos el invalid argument a out_of_range para la impresion de consolidados
        throw out_of_range(e.what());
    }
}


bool GestorTxt::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas)
{
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "buscados.txt";
    ofstream archivoBuscados(rutaCompleta);
    string delimitador = Settings::DELIMITADOR;
    if (!(archivoBuscados.is_open()))
    {
        string errorMsg = string("Error al abrir el archivo: ") + rutaCompleta;
        archivoBuscados.close();
        throw out_of_range(errorMsg);
    }

    //Imprimir las etiquetas de las columnas en la primera fila
    string strCodigoSNIES = string("Codigo SNIES del programa");
    string strnombrePrograma =  string("Programa Academico");
    string fila;
    int MIN_POS_ETIQUETAS = 0;
    int MAX_POS_ETIQUETAS = 3;
    //Metodo privado auxiliar para escribir las etiquetas de las columnas
    escribirEtiquetas(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, MIN_POS_ETIQUETAS, MAX_POS_ETIQUETAS);
    archivoBuscados << fila << endl;

    //Iteramos sobre la lista de los programasAcademicos para imprimir en cada fila 1 consolidado
    ProgramaAcademico * programaActual;
    bool consolidadoValido;
    for (list<ProgramaAcademico *>::iterator itProgramas = programasBuscados.begin(); itProgramas != programasBuscados.end(); ++itProgramas)
    {
        fila.clear();
        programaActual = *itProgramas;
        consolidadoValido = true;
        //Tratamos de escribir en la fila la informacion del programa
        try
        {
            escribirPrograma(strCodigoSNIES, strnombrePrograma, fila, delimitador, matrizEtiquetas, programaActual);
        } catch (invalid_argument& e)
        {
            //En caso que se produzca este error se intentará seguir imrpimiendo el resto de programa
            cout << "Programa SNIES '" << programaActual->consultarDatoInt(strCodigoSNIES) << "' tiene atributos faltantes. " << e.what() << endl;
            consolidadoValido = false;
        }
        if (consolidadoValido)
        {
            imprimirConsolidados(fila, archivoBuscados,delimitador, matrizEtiquetas, programaActual);
        }
    }


    //Cerramos el archivo una vez terminamos de iterar sobre los programas
    estadoCreacion = true;
    archivoBuscados.close();
    return estadoCreacion;
}

bool GestorTxt::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
}