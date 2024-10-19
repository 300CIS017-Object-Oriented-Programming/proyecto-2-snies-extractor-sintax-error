#include "GestorCsv.h"

vector<int> GestorCsv::leerProgramas(string &ruta)
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

// FIXME: falta la nueva implementacion de leerArchivo para que todo se pueda relizar en una sola funcion y no se necesiten 3
vector<vector<string>> GestorCsv::leerArchivo(string &ruta, vector<string> &etiquetasColumnas, vector<int> &codigosSnies)
{
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
    while (getline(streamFila, dato, ';'))
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
        while (getline(streamFila, dato, ';'))
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

bool GestorCsv::crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<string> etiquetasProgramaString, vector<string> etiquetasProgramaInt, vector<string> etiquetasConsolidadoString, vector<string> etiquetasConsolidadoInt)
{
    // Este bool nos ayudará a saber si se creo exitosamente el archivo
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "resultados.csv";
    ofstream archivoResultados(rutaCompleta);
    if (archivoResultados.is_open())
    {
        // Imprimimos en el archivo las etiquetas (Primera fila)
        for (int i = 0; i < etiquetasColumnas.size(); i++)
        {
            archivoResultados << etiquetasColumnas[i] << ";";
        }
        archivoResultados << "GRADUADOS;INSCRITOS;MATRICULADOS;NEOS" << endl;

        map<int, ProgramaAcademico *>::iterator it;
        // Leemos todos los programas del mapa para imprimirlos en el archivo
        for (it = mapadeProgramasAcademicos.begin(); it != mapadeProgramasAcademicos.end(); it++)
        {
            // Imprimimos cada uno de los 8 consolidados por programa
            for (int i = 0; i < 8; i++)
            {
                // Imprimimos toda la información base del programa academico
                archivoResultados << (it->second)->getCodigoDeLaInstitucion() << ";";
                archivoResultados << (it->second)->getIesPadre() << ";";
                archivoResultados << (it->second)->getInstitucionDeEducacionSuperiorIes() << ";";
                archivoResultados << (it->second)->getPrincipalOSeccional() << ";";
                archivoResultados << (it->second)->getIdSectorIes() << ";";
                archivoResultados << (it->second)->getSectorIes() << ";";
                archivoResultados << (it->second)->getIdCaracter() << ";";
                archivoResultados << (it->second)->getCaracterIes() << ";";
                archivoResultados << (it->second)->getCodigoDelDepartamentoIes() << ";";
                archivoResultados << (it->second)->getDepartamentoDeDomicilioDeLaIes() << ";";
                archivoResultados << (it->second)->getCodigoDelMunicipioIes() << ";";
                archivoResultados << (it->second)->getMunicipioDeDomicilioDeLaIes() << ";";
                archivoResultados << (it->second)->getCodigoSniesDelPrograma() << ";";
                archivoResultados << (it->second)->getProgramaAcademico() << ";";
                archivoResultados << (it->second)->getIdNivelAcademico() << ";";
                archivoResultados << (it->second)->getNivelAcademico() << ";";
                archivoResultados << (it->second)->getIdNivelDeFormacion() << ";";
                archivoResultados << (it->second)->getNivelDeFormacion() << ";";
                archivoResultados << (it->second)->getIdMetodologia() << ";";
                archivoResultados << (it->second)->getMetodologia() << ";";
                archivoResultados << (it->second)->getIdArea() << ";";
                archivoResultados << (it->second)->getAreaDeConocimiento() << ";";
                archivoResultados << (it->second)->getIdNucleo() << ";";
                archivoResultados << (it->second)->getNucleoBasicoDelConocimientoNbc() << ";";
                archivoResultados << (it->second)->getIdCineCampoAmplio() << ";";
                archivoResultados << (it->second)->getDescCineCampoAmplio() << ";";
                archivoResultados << (it->second)->getIdCineCampoEspecifico() << ";";
                archivoResultados << (it->second)->getDescCineCampoEspecifico() << ";";
                archivoResultados << (it->second)->getIdCineCodigoDetallado() << ";";
                archivoResultados << (it->second)->getDescCineCodigoDetallado() << ";";
                archivoResultados << (it->second)->getCodigoDelDepartamentoPrograma() << ";";
                archivoResultados << (it->second)->getDepartamentoDeOfertaDelPrograma() << ";";
                archivoResultados << (it->second)->getCodigoDelMunicipioPrograma() << ";";
                archivoResultados << (it->second)->getMunicipioDeOfertaDelPrograma() << ";";

                // Imprimimos la información del consolidado: (ID SEXO;SEXO;AÑO;SEMESTRE;ADMITIDOS;GRADUADOS;INSCRITOS;MATRICULADOS;NEOS)
                Consolidado *consolidadoActual = (it->second)->getConsolidado(i);
                archivoResultados << consolidadoActual->getIdSexo() << ";";
                archivoResultados << consolidadoActual->getSexo() << ";";
                archivoResultados << consolidadoActual->getAno() << ";";
                archivoResultados << consolidadoActual->getSemestre() << ";";
                archivoResultados << consolidadoActual->getAdmitidos() << ";";
                archivoResultados << consolidadoActual->getGraduados() << ";";
                archivoResultados << consolidadoActual->getInscritos() << ";";
                archivoResultados << consolidadoActual->getMatriculados() << ";";
                archivoResultados << consolidadoActual->getMatriculadosPrimerSemestre();
                // Saltamos de linea para la siguiente fila
                archivoResultados << endl;
            }
        }

        // Cambiamos el valor del booleano si logramos llegar hasta este punto
        estadoCreacion = true;
        // Imprimimos ruta donde quedo el archivo
        cout << "Archivo Creado en: " << rutaCompleta << endl;
    }

    archivoResultados.close();
    return estadoCreacion;
}

bool GestorCsv::crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<string> etiquetasColumnas)
{
    // Este bool nos ayudará a saber si se creo exitosamente el archivo
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "buscados.csv";
    ofstream archivoBuscados(rutaCompleta);
    if (archivoBuscados.is_open())
    {

        // Imprimimos en el archivo las etiquetas (Primera fila)
        for (int i = 0; i < etiquetasColumnas.size(); i++)
        {
            archivoBuscados << etiquetasColumnas[i] << ";";
        }
        archivoBuscados << "GRADUADOS;INSCRITOS;MATRICULADOS;NEOS" << endl;

        list<ProgramaAcademico *>::iterator it;
        // Leemos todos los programas de la lista de los programas buscados para imprimirlos
        for (it = programasBuscados.begin(); it != programasBuscados.end(); it++)
        {
            // Imprimimos los 8 consolidados del programa
            for (int i = 0; i < 8; i++)
            {
                // Imprimimos la informacion base del programa
                archivoBuscados << (*it)->getCodigoDeLaInstitucion() << ";";
                archivoBuscados << (*it)->getIesPadre() << ";";
                archivoBuscados << (*it)->getInstitucionDeEducacionSuperiorIes() << ";";
                archivoBuscados << (*it)->getPrincipalOSeccional() << ";";
                archivoBuscados << (*it)->getIdSectorIes() << ";";
                archivoBuscados << (*it)->getSectorIes() << ";";
                archivoBuscados << (*it)->getIdCaracter() << ";";
                archivoBuscados << (*it)->getCaracterIes() << ";";
                archivoBuscados << (*it)->getCodigoDelDepartamentoIes() << ";";
                archivoBuscados << (*it)->getDepartamentoDeDomicilioDeLaIes() << ";";
                archivoBuscados << (*it)->getCodigoDelMunicipioIes() << ";";
                archivoBuscados << (*it)->getMunicipioDeDomicilioDeLaIes() << ";";
                archivoBuscados << (*it)->getCodigoSniesDelPrograma() << ";";
                archivoBuscados << (*it)->getProgramaAcademico() << ";";
                archivoBuscados << (*it)->getIdNivelAcademico() << ";";
                archivoBuscados << (*it)->getNivelAcademico() << ";";
                archivoBuscados << (*it)->getIdNivelDeFormacion() << ";";
                archivoBuscados << (*it)->getNivelDeFormacion() << ";";
                archivoBuscados << (*it)->getIdMetodologia() << ";";
                archivoBuscados << (*it)->getMetodologia() << ";";
                archivoBuscados << (*it)->getIdArea() << ";";
                archivoBuscados << (*it)->getAreaDeConocimiento() << ";";
                archivoBuscados << (*it)->getIdNucleo() << ";";
                archivoBuscados << (*it)->getNucleoBasicoDelConocimientoNbc() << ";";
                archivoBuscados << (*it)->getIdCineCampoAmplio() << ";";
                archivoBuscados << (*it)->getDescCineCampoAmplio() << ";";
                archivoBuscados << (*it)->getIdCineCampoEspecifico() << ";";
                archivoBuscados << (*it)->getDescCineCampoEspecifico() << ";";
                archivoBuscados << (*it)->getIdCineCodigoDetallado() << ";";
                archivoBuscados << (*it)->getDescCineCodigoDetallado() << ";";
                archivoBuscados << (*it)->getCodigoDelDepartamentoPrograma() << ";";
                archivoBuscados << (*it)->getDepartamentoDeOfertaDelPrograma() << ";";
                archivoBuscados << (*it)->getCodigoDelMunicipioPrograma() << ";";
                archivoBuscados << (*it)->getMunicipioDeOfertaDelPrograma() << ";";

                // Imprimimos la información del consolidado: (ID SEXO;SEXO;AÑO;SEMESTRE;ADMITIDOS;GRADUADOS;INSCRITOS;MATRICULADOS;NEOS)
                Consolidado *consolidadoActual = (*it)->getConsolidado(i);
                archivoBuscados << consolidadoActual->getIdSexo() << ";";
                archivoBuscados << consolidadoActual->getSexo() << ";";
                archivoBuscados << consolidadoActual->getAno() << ";";
                archivoBuscados << consolidadoActual->getSemestre() << ";";
                archivoBuscados << consolidadoActual->getAdmitidos() << ";";
                archivoBuscados << consolidadoActual->getGraduados() << ";";
                archivoBuscados << consolidadoActual->getInscritos() << ";";
                archivoBuscados << consolidadoActual->getMatriculados() << ";";
                archivoBuscados << consolidadoActual->getMatriculadosPrimerSemestre();
                // Saltamos de linea para la siguiente fila
                archivoBuscados << endl;
            }
        }

        // Cambiamos el valor del booleano si logramos llegar hasta este punto
        estadoCreacion = true;
        // Imprimimos ruta donde quedo el archivo
        cout << "Archivo Creado en: " << rutaCompleta << endl;
    }

    archivoBuscados.close();
    return estadoCreacion;
}

bool GestorCsv::crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir)
{
    // Este bool nos ayudará a saber si se creo el archivo exitosamente
    bool estadoCreacion = false;
    string rutaCompleta = ruta + "extras.csv";
    ofstream archivoExtra(rutaCompleta);
    if (archivoExtra.is_open())
    {
        // Imprimimos la matriz de datos que queremos imprimir
        for (int i = 0; i < datosAImprimir.size(); i++)
        {
            // Imprimimos cada fila
            for (int j = 0; j < datosAImprimir[i].size(); j++)
            {
                // Imprimimos cada dato separado por ';'
                archivoExtra << datosAImprimir[i][j];
                if (j != (datosAImprimir[i].size() - 1))
                {
                    archivoExtra << ";";
                }
            }
            // Saltamos de linea al terminar una fila
            archivoExtra << endl;
        }

        // Cambiamos el valor del booleano si logramos llegar hasta este punto
        estadoCreacion = true;
        // Imprimimos ruta donde quedo el archivo
        cout << "Archivo Creado en: " << rutaCompleta << endl;
    }

    archivoExtra.close();
    return estadoCreacion;
}