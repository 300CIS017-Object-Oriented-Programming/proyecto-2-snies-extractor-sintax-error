#include "SNIESController.h"

using namespace std;

SNIESController::SNIESController()
{
    try
    {
        inicializarGestores();
        inicializarEtiquetas();
    } catch (out_of_range& e)
    {
        string errorMsg = string("No se pudo inicializar el Controlador. ") + e.what();
        throw out_of_range(errorMsg);
    }
}

void SNIESController::inicializarGestores()
{
    //Agregar los gestores al vector
    gestoresArchivos.push_back(new GestorCsv());
    gestoresArchivos.push_back(new GestorTxt());
    gestoresArchivos.push_back(new GestorJSON());
}

void SNIESController::inicializarEtiquetas()
{
    matrizEtiquetas = vector<vector<string>>(6);
    //Leer el archivo de configuracion para obtener la información necesaria
    leerArchivoConfiguracion(matrizEtiquetas);
}

void SNIESController::leerArchivoConfiguracion(vector<vector<string>>& matriz)
{
    string rutaConfig = Settings::ETIQUETAS_CONFIG_PATH;
    ifstream archivoConfig(rutaConfig);
    if (!(archivoConfig.is_open()))
    {
        throw out_of_range("Error al abrir el archivo de configuracion de atributos");
    }

    string fila;
    //Se inicia en -1 para que cuando lea la primera serie de etiquetas empiece por la fila 0 de la matriz
    int posFilaMatriz = -1;
    while (getline(archivoConfig,fila))
    {
        /*Las distintas categorias de etiquetas empizan por una fila con el nombre de la categoria
         * El primer caracter del nombre de la categoria siempre empieza por '-'
         * Esto se aprovecha para saber cuando cambiamos de categoria y por tanto de fila en la matriz de etiquetas
         */
        if (fila[0] == '-')
        {
            posFilaMatriz++;
        }
        else
        {
            matriz[posFilaMatriz].push_back(fila);
        }
    }

    //Cerramos el archivo una vez hemos terminado
    archivoConfig.close();
}

SNIESController::~SNIESController()
{
    for (auto &pair : programasAcademicos)
    {
        ((pair).second)->~ProgramaAcademico();
        delete pair.second;
    }
}

//FIXME: Adaptar al nuevo diseño teniendo en cuenta el diseño viejo
void SNIESController::procesarDatos(vector<string> anos)
{
    int FILA_ATRIBUTOS_STRING_PROGRAMA = 0;
    int FILA_ATRIBUTOS_INT_PROGRAMA = 1;
    int FILA_ATRIBUTOS_STRING_CONSOLIDADO = 2;
    int FILA_ATRIBUTOS_INT_CONSOLIDADO = 3;
    int FILA_SEXOS_DISPONIBLES = 4;
    int FILA_ANOS_DISPONIBLES = 5;
    //Llenamos la ultima fila de la matriz de etiquetas con los años a considerar
    matrizEtiquetas[FILA_ANOS_DISPONIBLES] = anos;

    vector<int> codigosSNIES;
    vector<string> etiquetasParaLeer;
    vector<vector<string>> matrizArchivo;
    string rutaActual;
    //Leemos los codigos SNIES que nos piden buscar
    rutaActual = Settings::PROGRAMAS_FILTRAR_FILE_PATH;
    //Se utiliza el gestor 0 porque para los propósitos de leer sirve cualquier gestor
    codigosSNIES = gestoresArchivos[0]->leerProgramas(rutaActual);

    /*
     * Ahora leeremos el primer archivo
     * Donde mandaremos a buscar la informacion base de los programas
     * Y los consolidados de este archivo en particular
     */

    //Llenamos el vector de etiquetas con las que nos iteresan para esta lectura
    string strAdmitidos = "ADMITIDOS";
    seleccionarEtiquetas(FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_STRING_PROGRAMA, etiquetasParaLeer);
    etiquetasParaLeer.push_back(strAdmitidos);

    //Leemos el archivo de admitidos del primer año de todos
    rutaActual = Settings::ADMITIDOS_FILE_PATH + matrizEtiquetas[FILA_ANOS_DISPONIBLES][0];
    matrizArchivo = gestoresArchivos[0]->leerArchivo(rutaActual, etiquetasParaLeer, codigosSNIES);

    //Ahora procesamos los datos que nos llegaron de la lectura del archivo para crear los programas
    crearProgramas(matrizArchivo, FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, FILA_ATRIBUTOS_STRING_CONSOLIDADO, FILA_ATRIBUTOS_INT_CONSOLIDADO);

    /*
     * A continuación leeremos el resto de los archivos, donde solo pediremos los datos de los consolidados
     * y los asignaremos al programaAcademico apropiado utlizando el mapa y el codigo SNIES
     */
    vector<vector<string>> atributosClave = vector<vector<string>>(2);
    atributosClave[0].push_back(string("ADMITIDOS"));
    atributosClave[1].push_back(Settings::ADMITIDOS_FILE_PATH);
    atributosClave[0].push_back(string("GRADUADOS"));
    atributosClave[1].push_back(Settings::GRADUADOS_FILE_PATH);
    atributosClave[0].push_back(string("INSCRITOS"));
    atributosClave[1].push_back(Settings::INSCRITOS_FILE_PATH);
    atributosClave[0].push_back(string("MATRICULADOS"));
    atributosClave[1].push_back(Settings::MATRICULADOS_FILE_PATH);
    atributosClave[0].push_back(string("PRIMER CURSO"));
    atributosClave[1].push_back(Settings::PRIMER_CURSO_FILE_PATH);

    for (int )
}

void SNIESController::seleccionarEtiquetas(int filaMin, int filaMax, vector<string>& etiquetasParaLeer)
{
    etiquetasParaLeer.clear();
    for (int fila = filaMin; fila <= filaMax; fila++)
    {
        for (int columna = 0; columna < matrizEtiquetas[fila].size(); columna++)
        {
            etiquetasParaLeer.push_back(matrizEtiquetas[fila][columna]);
        }
    }
}

void SNIESController::crearProgramas(vector<vector<string>>& matrizArchivo, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
{
    string etiquetaCorrespondiente;
    ProgramaAcademico *programaNuevo;
    Consolidado *consolidadoNuevo;
    string datoString;
    int datoInt;
    string strCodigoSNIES = "CÓDIGO SNIES DEL PROGRAMA";
    string strSexo = "SEXO";
    string sexoActual;
    string strAno = "AÑO";
    int anoActual;
    string strSemestre = "SEMESTRE";
    int semestreActual;
    int filaCorrespondiente;
    bool programaNuevoCreado;
    //Nos saltamos la primera fila (0) porque son las etiquetas y esas no se guardan sino que se utilizan para mapear
    for (int fila = 1; fila < matrizArchivo.size(); fila++)
    {
        programaNuevo = new ProgramaAcademico();
        consolidadoNuevo = new Consolidado();
        for (int columna = 0; columna < matrizArchivo[fila].size(); columna++)
        {
            etiquetaCorrespondiente = matrizArchivo[0][columna];
            datoString = matrizArchivo[fila][columna];

            //Buscamos a que tipo de atributo pertenece para agregarlo correspondientemente
            filaCorrespondiente = verificarFilaEtiqueta(etiquetaCorrespondiente, fAtrStrProg, fAtrIntProg, fAtrStrCon, fAtrIntCon);
            if (filaCorrespondiente == fAtrStrProg)
            {
                programaNuevo->agregarElementoTipoString(etiquetaCorrespondiente, datoString);
            }
            else if (filaCorrespondiente == fAtrIntProg)
            {
                datoInt = stoi(datoString);
                programaNuevo->agregarElementoTipoInt(etiquetaCorrespondiente, datoInt);
            }
            else if (filaCorrespondiente == fAtrIntCon)
            {
                datoInt = stoi(datoString);
                consolidadoNuevo->agregarDatoInt(etiquetaCorrespondiente, datoInt);
            }
            else
            {
                consolidadoNuevo->agregarDatoString(etiquetaCorrespondiente, datoString);
            }
        }

        //Intentaremos agregar el programa al mapa
        auto resultadoInsert = programasAcademicos.insert(make_pair(programaNuevo->consultarDatoInt(strCodigoSNIES), programaNuevo));
        programaNuevoCreado = resultadoInsert.second;
        sexoActual = consolidadoNuevo->obtenerDatoString(strSexo);
        anoActual = consolidadoNuevo->obtenerDatoInt(strAno);
        semestreActual = consolidadoNuevo->obtenerDatoInt(strSemestre);
        if (!programaNuevoCreado)
        {
            //Borramos el programa nuevo que intentamos crear
            programaNuevo->~ProgramaAcademico();
            delete programaNuevo;

            //Asignamos el consolidado al programa que ya existía
            ((resultadoInsert.first)->second)->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
        }
        else
        {
            //Asignamos el consolidado al programa nuevo
            programaNuevo->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
        }
    }
}

int SNIESController::verificarFilaEtiqueta(string& etiquetaCorrespondiente, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
{
    int filaCorrespondiente = -1;
    vector<string>::iterator itFilaMatriz;
    itFilaMatriz = find(matrizEtiquetas[fAtrStrProg].begin(), matrizEtiquetas[fAtrStrProg].end(), etiquetaCorrespondiente);
    if (itFilaMatriz != matrizEtiquetas[fAtrStrProg].end())
    {
        filaCorrespondiente = fAtrStrProg;
    }
    else
    {
        itFilaMatriz = find(matrizEtiquetas[fAtrIntProg].begin(), matrizEtiquetas[fAtrIntProg].end(), etiquetaCorrespondiente);
        if (itFilaMatriz != matrizEtiquetas[fAtrIntProg].end())
        {
            filaCorrespondiente = fAtrIntProg;
        }
        else
        {
            itFilaMatriz = find(matrizEtiquetas[fAtrIntCon].begin(), matrizEtiquetas[fAtrIntCon].end(), etiquetaCorrespondiente);
            if (itFilaMatriz != matrizEtiquetas[fAtrIntCon].end())
            {
                filaCorrespondiente = fAtrIntCon;
            }
            else
            {
                filaCorrespondiente = fAtrStrCon;
            }
        }
    }

    return filaCorrespondiente;
}


void SNIESController::procesarDatosCsv(string &ano1, string &ano2)
{
    vector<int> codigosSnies;
    vector<vector<string>> programasAcademicosVector;
    int posicion;
    int columna;
    // cout << "antes leer programas csv" << endl;
    codigosSnies = gestorCsvObj.leerProgramasCsv(rutaProgramasCSV);
    // cout << "despues leer programas csv" << endl;
    programasAcademicosVector = gestorCsvObj.leerArchivoPrimera(rutaAdmitidos, ano1, codigosSnies);
    // cout << "despues leer archivos Primera" << endl;
    etiquetasColumnas = programasAcademicosVector[0];

    for (int i = 1; i < programasAcademicosVector.size(); i += 4)
    {
        ProgramaAcademico *programaAcademico = new ProgramaAcademico();
        programaAcademico->setCodigoDeLaInstitucion(stoi(programasAcademicosVector[i][0]));          // CÓDIGO DE LA INSTITUCIÓN
        programaAcademico->setIesPadre(stoi(programasAcademicosVector[i][1]));                       // IES_PADRE
        programaAcademico->setInstitucionDeEducacionSuperiorIes(programasAcademicosVector[i][2]);    // INSTITUCIÓN DE EDUCACIÓN SUPERIOR (IES)
        programaAcademico->setPrincipalOSeccional(programasAcademicosVector[i][3]);                  // PRINCIPAL O SECCIONAL
        programaAcademico->setIdSectorIes(stoi(programasAcademicosVector[i][4]));                    // ID SECTOR IES
        programaAcademico->setSectorIes(programasAcademicosVector[i][5]);                            // SECTOR IES
        programaAcademico->setIdCaracter(stoi(programasAcademicosVector[i][6]));                     // ID CARÁCTER
        programaAcademico->setCaracterIes(programasAcademicosVector[i][7]);                          // CARACTER IES
        programaAcademico->setCodigoDelDepartamentoIes(stoi(programasAcademicosVector[i][8]));       // CÓDIGO DEL DEPARTAMENTO (IES)
        programaAcademico->setDepartamentoDeDomicilioDeLaIes(programasAcademicosVector[i][9]);       // DEPARTAMENTO DE DOMICILIO DE LA IES
        programaAcademico->setCodigoDelMunicipioIes(stoi(programasAcademicosVector[i][10]));         // CÓDIGO DEL MUNICIPIO IES
        programaAcademico->setMunicipioDeDomicilioDeLaIes(programasAcademicosVector[i][11]);         // MUNICIPIO DE DOMICILIO DE LA IES
        programaAcademico->setCodigoSniesDelPrograma(stoi(programasAcademicosVector[i][12]));        // CÓDIGO SNIES DEL PROGRAMA
        programaAcademico->setProgramaAcademico(programasAcademicosVector[i][13]);                   // PROGRAMA ACADÉMICO
        programaAcademico->setIdNivelAcademico(stoi(programasAcademicosVector[i][14]));              // ID NIVEL ACADÉMICO
        programaAcademico->setNivelAcademico(programasAcademicosVector[i][15]);                      // NIVEL ACADÉMICO
        programaAcademico->setIdNivelDeFormacion(stoi(programasAcademicosVector[i][16]));            // ID NIVEL DE FORMACIÓN
        programaAcademico->setNivelDeFormacion(programasAcademicosVector[i][17]);                    // NIVEL DE FORMACIÓN
        programaAcademico->setIdMetodologia(stoi(programasAcademicosVector[i][18]));                 // ID METODOLOGÍA
        programaAcademico->setMetodologia(programasAcademicosVector[i][19]);                         // METODOLOGÍA
        programaAcademico->setIdArea(stoi(programasAcademicosVector[i][20]));                        // ID ÁREA
        programaAcademico->setAreaDeConocimiento(programasAcademicosVector[i][21]);                  // ÁREA DE CONOCIMIENTO
        programaAcademico->setIdNucleo(stoi(programasAcademicosVector[i][22]));                      // ID NÚCLEO
        programaAcademico->setNucleoBasicoDelConocimientoNbc(programasAcademicosVector[i][23]);      // NÚCLEO BÁSICO DEL CONOCIMIENTO (NBC)
        programaAcademico->setIdCineCampoAmplio(stoi(programasAcademicosVector[i][24]));             // ID CINE CAMPO AMPLIO
        programaAcademico->setDescCineCampoAmplio(programasAcademicosVector[i][25]);                 // DESC CINE CAMPO AMPLIO
        programaAcademico->setIdCineCampoEspecifico(stoi(programasAcademicosVector[i][26]));         // ID CINE CAMPO ESPECÍFICO
        programaAcademico->setDescCineCampoEspecifico(programasAcademicosVector[i][27]);             // DESC CINE CAMPO ESPECÍFICO
        programaAcademico->setIdCineCodigoDetallado(stoi(programasAcademicosVector[i][28]));         // ID CINE CÓDIGO DETALLADO
        programaAcademico->setDescCineCodigoDetallado(programasAcademicosVector[i][29]);             // DESC CINE CÓDIGO DETALLADO
        programaAcademico->setCodigoDelDepartamentoPrograma(stoi(programasAcademicosVector[i][30])); // CÓDIGO DEL DEPARTAMENTO (PROGRAMA)
        programaAcademico->setDepartamentoDeOfertaDelPrograma(programasAcademicosVector[i][31]);     // DEPARTAMENTO DE OFERTA DEL PROGRAMA
        programaAcademico->setCodigoDelMunicipioPrograma(stoi(programasAcademicosVector[i][32]));    // CÓDIGO DEL MUNICIPIO (PROGRAMA)
        programaAcademico->setMunicipioDeOfertaDelPrograma(programasAcademicosVector[i][33]);        // MUNICIPIO DE OFERTA DEL PROGRAMA
        Consolidado *consolidado[4];
        for (int m = 0; m < 4; ++m)
        {
            consolidado[m] = new Consolidado();
            consolidado[m]->setIdSexo(stoi(programasAcademicosVector[i + m][34]));
            consolidado[m]->setSexo(programasAcademicosVector[i + m][35]);
            consolidado[m]->setAno(stoi(programasAcademicosVector[i + m][36]));
            consolidado[m]->setSemestre(stoi(programasAcademicosVector[i + m][37]));
            consolidado[m]->setAdmitidos(stoi(programasAcademicosVector[i + m][38]));
            programaAcademico->setConsolidado(consolidado[m], m);
        }
        programasAcademicos.emplace(programaAcademico->getCodigoSniesDelPrograma(), programaAcademico);
    }
    // cout << "despues crear programas academicos" << endl;
    programasAcademicosVector = gestorCsvObj.leerArchivoSegunda(rutaAdmitidos, ano2, codigosSnies);
    // cout << "despues leer archivos segunda" << endl;
    for (int j = 0; j < programasAcademicosVector.size(); j += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[j][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            Consolidado *consolidado[4];
            for (int m = 0; m < 4; ++m)
            {
                consolidado[m] = new Consolidado();
                consolidado[m]->setIdSexo(stoi(programasAcademicosVector[j + m][1]));
                consolidado[m]->setSexo(programasAcademicosVector[j + m][2]);
                consolidado[m]->setAno(stoi(programasAcademicosVector[j + m][3]));
                consolidado[m]->setSemestre(stoi(programasAcademicosVector[j + m][4]));
                consolidado[m]->setAdmitidos(stoi(programasAcademicosVector[j + m][5]));
                programa->setConsolidado(consolidado[m], m + 4);
            }
        }
    }
    // cout << "despues crear todos los consolidados" << endl;
    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaGraduados, ano1, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m);
                consolidado->setGraduados(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaGraduados, ano2, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m + 4);
                consolidado->setGraduados(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaInscritos, ano1, codigosSnies, 12);
    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m);
                consolidado->setInscritos(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    if (ano2 == "2022")
    {
        columna = 12;
    }
    else
    {
        columna = 13;
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaInscritos, ano2, codigosSnies, columna);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m + 4);
                consolidado->setInscritos(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaMatriculados, ano1, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m);
                consolidado->setMatriculados(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaMatriculados, ano2, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m + 4);
                consolidado->setMatriculados(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaMatriculadosPrimerSemestre, ano1, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m);
                consolidado->setMatriculadosPrimerSemestre(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    programasAcademicosVector = gestorCsvObj.leerArchivo(rutaMatriculadosPrimerSemestre, ano2, codigosSnies, 13);

    for (int k = 0; k < programasAcademicosVector.size(); k += 4)
    {
        map<int, ProgramaAcademico *>::iterator it = programasAcademicos.find(stoi(programasAcademicosVector[k][0]));
        if (it != programasAcademicos.end())
        {
            ProgramaAcademico *programa = it->second;

            for (int m = 0; m < 4; ++m)
            {
                Consolidado *consolidado = programa->getConsolidado(m + 4);
                consolidado->setMatriculadosPrimerSemestre(stoi(programasAcademicosVector[k + m][1]));
            }
        }
    }

    bool archivoCreado;
    archivoCreado = gestorCsvObj.crearArchivo(rutaOutput, programasAcademicos, etiquetasColumnas);
    // cout << archivoCreado << endl;
}

//FIXME: Adaptar al nuevo diseño
void SNIESController::buscarProgramas(bool flag, string &palabraClave, int idComparacion)
{
    list<ProgramaAcademico *> listaProgramas;
    string llaveNombrePrograma = string("PROGRAMA ACADÉMICO");
    string llaveId = string("ID NIVEL DE FORMACIÓN");
    string codigoSNIES = string("CÓDIGO SNIES DEL PROGRAMA");
    string codigoInstitucion = string("CÓDIGO DE LA INSTITUCIÓN");
    string institucionEducacionSuperior = string("INSTITUCIÓN DE EDUCACIÓN SUPERIOR (IES)");
    string METODOLOGIA =string("METODOLOGÍA");
    string rutaOutput = Settings::OUTPUTS_PATH;

    for (map<int, ProgramaAcademico *>::iterator it = programasAcademicos.begin(); it != programasAcademicos.end(); ++it)
    {
        ProgramaAcademico *programa = it->second;
        string nombre = programa->consultarDatoString(llaveNombrePrograma);
        int id = stoi(programa->consultarDatoString(llaveId));
        //nombre = hecho123 palabraClave = hecho
        
        if (nombre.find(palabraClave) != string::npos && id == idComparacion)
        {
            listaProgramas.push_back(programa);
            // codigo SNIES, nombre del programa, codigo de la institucion, nombre de la institucion y metodología
            cout << programa->consultarDatoInt(codigoSNIES) << ";" << nombre << ";" << programa->consultarDatoInt(codigoInstitucion) << ";"; 
            cout << programa->consultarDatoString(institucionEducacionSuperior) << ";" << programa->consultarDatoString(METODOLOGIA) << endl;
        }
    }

    if (flag)
    {


        for(int gestor = 0; gestor < gestoresArchivos.size(); gestor++)
        {
            try
            {
                gestoresArchivos[gestor]->crearArchivoBuscados(rutaOutput,listaProgramas,matrizEtiquetas);
            }catch(out_of_range& e)
            {
                cout << "No se pudo crear el archivo con los buscados" << endl;
            }
        }
    }
}

//FIXME: Adaptar al nuevo diseño
void SNIESController::calcularDatosExtra(bool exportarArchivo)
{
    vector<vector<string>> matrizFinal;
    vector<vector<string>> matrizEtiquetas1;
    vector<vector<string>> matrizEtiquetas2;
    vector<vector<string>> matrizEtiquetas3;
    vector<string> etiquetas1;
    vector<string> sumaMatriculados;
    string ano1 = matrizEtiquetas[5][matrizEtiquetas.size() - 2];
    string ano2 = matrizEtiquetas[5][matrizEtiquetas.size() - 1];
    string Matriculados = "Matriculados";
    string Neos = "PRIMER CURSO";
    string Metodologia = "ID METODOLOGÍA";
    string CodigoSnies = "CÓDIGO SNIES DEL PROGRAMA";
    string NombrePrograma = "PROGRAMA ACADÉMICO";

    for (string ano : matrizEtiquetas[5])
    {
        string etiqueta1 = "Suma Estudiantes Matriculados de Programas Seleccionados (Presencial o Virtual) año " + ano;
        etiquetas1.push_back(etiqueta1);
    }
    matrizEtiquetas1.push_back(etiquetas1);

    vector<string> etiquetas2 = {
        "Codigo Snies", "Nombre del Programa", "Nombre del Institucion", "Diferencial porcentual anual de NEOS de los años" + ano2 + " y " + ano1};
    matrizEtiquetas2.push_back(etiquetas2);

    vector<string> etiquetas3 = {
        "Codigo Snies", "Nombre del Programa sin NEOS en los ultimos 3 semestres"};
    matrizEtiquetas3.push_back(etiquetas3);

    for (auto &it : programasAcademicos)
    {
        int SumaNeosPrimerSemestre;
        int SumaNeosSegundoSemestre;
        int SumaNeosTercerSemestre;
        int SumaNeosCuartoSemestre;
        int SumaNeosPrimerAno;
        int SumaNeosSegundoAno;
        int suma;
        ProgramaAcademico *programa = it.second;

        // Acceso a los datos del programa académico desde los mapas
        int idMetodologiaBuscada = programa->consultarDatoInt(Metodologia);

        if (idMetodologiaBuscada == 1 || idMetodologiaBuscada == 3)
        {
            // Sumar estudiantes matriculados del primer y segundo año
            for (string ano : matrizEtiquetas[5])
            {
                for (string sexo : matrizEtiquetas[4])
                {
                    Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(ano), 1);
                    suma += stoi(consolidado1->obtenerDatoString(Matriculados));
                    Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(ano), 2);
                    suma += stoi(consolidado2->obtenerDatoString(Matriculados));
                }
                sumaMatriculados.push_back(to_string(suma));
            }
        }

        for (string sexo : matrizEtiquetas[4])
        {
            Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(ano1), 1);
            SumaNeosPrimerSemestre += stoi(consolidado1->obtenerDatoString(Neos));
            Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(ano1), 2);
            SumaNeosSegundoSemestre += stoi(consolidado2->obtenerDatoString(Neos));

            Consolidado *consolidado3 = programa->buscarConsolidado(sexo, stoi(ano2), 1);
            SumaNeosTercerSemestre += stoi(consolidado3->obtenerDatoString(Neos));
            Consolidado *consolidado4 = programa->buscarConsolidado(sexo, stoi(ano2), 2);
            SumaNeosCuartoSemestre += stoi(consolidado4->obtenerDatoString(Neos));
        }

        if ((SumaNeosPrimerSemestre == 0 && SumaNeosSegundoSemestre == 0 && SumaNeosTercerSemestre == 0) || (SumaNeosSegundoSemestre == 0 && SumaNeosTercerSemestre == 0 && SumaNeosCuartoSemestre == 0))
        {
            etiquetas3 = {to_string(programa->consultarDatoInt(CodigoSnies)), programa->consultarDatoString(NombrePrograma)};
        }
// esta parte que es la de calcular la diferencia entre dos anos toca revisarla para ver si se calcula solo la de los utlimos dos o de dos en dos y asi hasta calcualar todos los anos.
        for (string sexo : matrizEtiquetas[4])
        {
            Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(ano1), 1);
            SumaNeosPrimerAno += stoi(consolidado1->obtenerDatoString(Neos));
            Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(ano1), 2);
            SumaNeosPrimerAno += stoi(consolidado2->obtenerDatoString(Neos));

            Consolidado *consolidado3 = programa->buscarConsolidado(sexo, stoi(ano2), 1);
            SumaNeosSegundoAno += stoi(consolidado3->obtenerDatoString(Neos));
            Consolidado *consolidado4 = programa->buscarConsolidado(sexo, stoi(ano2), 2);
            SumaNeosSegundoAno+= stoi(consolidado4->obtenerDatoString(Neos));
        }

        for (int i = 0; i < 4; ++i)
        {
            Consolidado *consolidado = programa->getConsolidado(i);
            int numNeos = consolidado->getMatriculadosPrimerSemestre();
            neosPrimerAno += numNeos;
        }

        for (int i = 0; i < 4; ++i)
        {
            Consolidado *consolidado = programa->getConsolidado(i + 4);
            int numNeos = consolidado->getMatriculadosPrimerSemestre();
            neosSegundoAno += numNeos;
        }

        if (neosPrimerAno != 0)
        {
            diferenciaNeos = ((neosSegundoAno - neosPrimerAno) * 100) / neosPrimerAno;
        }
        else
        {
            diferenciaNeos = 0;
        }
        datosEtiquetas2 = {to_string(programa->getCodigoSniesDelPrograma()), programa->getProgramaAcademico(), programa->getInstitucionDeEducacionSuperiorIes(), to_string(diferenciaNeos)};
        matrizEtiquetas2.push_back(datosEtiquetas2);

        etiquetas1 = {to_string(sumaPrimerAno), to_string(sumaSegundoAno)};
        matrizEtiquetas1.push_back(etiquetas1);
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas1.begin(), matrizEtiquetas1.end());
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas2.begin(), matrizEtiquetas2.end());
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas3.begin(), matrizEtiquetas3.end());

        for (const auto &fila : matrizFinal)
        {
            for (size_t i = 0; i < fila.size(); ++i)
            {
                cout << fila[i];
                if (i < fila.size() - 1)
                {
                    cout << ";";
                }
            }
            cout << endl;
        }

        if (flag)
        {
            bool creado;
            creado = gestorCsvObj.crearArchivoExtra(rutaOutput, matrizFinal);
        }
    }
}