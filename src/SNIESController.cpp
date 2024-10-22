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

void SNIESController::procesarDatos(vector<string> anos)
{
    try {
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
        string strAno = "AÑO";
        string strSexo = "SEXO";
        string strSemestre = "SEMESTRE";
        seleccionarEtiquetas(FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, etiquetasParaLeer);
        etiquetasParaLeer.push_back(strAdmitidos);
        etiquetasParaLeer.push_back(strAno);
        etiquetasParaLeer.push_back(strSexo);
        etiquetasParaLeer.push_back(strSemestre);

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

        string etiquetaClave;
        //Como el primer año de admitidos ya lo leímos, usaremos esta bandera para saltarnos su lectura
        bool admitidosFlag = true;
        string strCodigoSNIES = "CÓDIGO SNIES DEL PROGRAMA";
        string strAnoActual;
        for(int posAtributosClave = 0; posAtributosClave < atributosClave[0].size(); posAtributosClave++)
        {
            //Cargo en las etiquetas a leer las que necesito del archivo
            etiquetaClave = atributosClave[0][posAtributosClave];
            etiquetasParaLeer.clear();
            etiquetasParaLeer.push_back(strCodigoSNIES);
            etiquetasParaLeer.push_back(etiquetaClave);
            etiquetasParaLeer.push_back(strAno);
            etiquetasParaLeer.push_back(strSexo);
            etiquetasParaLeer.push_back(strSemestre);

            //Leo los archivos de cada año para cada atributo clave
            for (int columna = 0; columna < matrizEtiquetas[FILA_ANOS_DISPONIBLES].size(); columna++)
            {
                if (!admitidosFlag)
                {
                    rutaActual = atributosClave[1][posAtributosClave] + matrizEtiquetas[FILA_ANOS_DISPONIBLES][columna];
                    matrizArchivo = gestoresArchivos[0]->leerArchivo(rutaActual, etiquetasParaLeer, codigosSNIES);
                    //Proceso la información resultante para asignar datos a sus respectivos consolidados y programas
                    asignarConsolidados(matrizArchivo, etiquetaClave, FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, FILA_ATRIBUTOS_STRING_CONSOLIDADO, FILA_ATRIBUTOS_INT_CONSOLIDADO);
                }
                else
                {
                    admitidosFlag = false;
                }
            }
        }
        //Para este punto ya tenemos todos los programas y consolidados

        //Una vez tenemos todos los programas y consolidados, creamos el archivo de resultados con cada gestor de archivos
        string rutaOutput = Settings::OUTPUTS_PATH;
        for(int gestor = 0; gestor < gestoresArchivos.size(); gestor++)
        {
            try
            {
                gestoresArchivos[gestor]->crearArchivo(rutaOutput, programasAcademicos, matrizEtiquetas);
            }catch(out_of_range& e)
            {
                cout << "No se pudo crear el archivo de resultados. " << e.what() << endl;
            }
        }
    } catch (out_of_range& e)
    {
        string errorMsg = string("Error FATAL al abrir un archivo. ") + e.what();
        throw out_of_range(errorMsg);
    }
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
    int filaCorrespondiente;
    vector<string>::iterator itFilaMatriz1;
    vector<string>::iterator itFilaMatriz2;
    vector<string>::iterator itFilaMatriz3;
    itFilaMatriz1 = find(matrizEtiquetas[fAtrStrProg].begin(), matrizEtiquetas[fAtrStrProg].end(), etiquetaCorrespondiente);
    itFilaMatriz2 = find(matrizEtiquetas[fAtrIntProg].begin(), matrizEtiquetas[fAtrIntProg].end(), etiquetaCorrespondiente);
    itFilaMatriz3 = find(matrizEtiquetas[fAtrIntCon].begin(), matrizEtiquetas[fAtrIntCon].end(), etiquetaCorrespondiente);
    if (itFilaMatriz1 != matrizEtiquetas[fAtrStrProg].end())
    {
        filaCorrespondiente = fAtrStrProg;
    }
    else if (itFilaMatriz2 != matrizEtiquetas[fAtrIntProg].end())
    {
        filaCorrespondiente = fAtrIntProg;
    }
    else if (itFilaMatriz3 != matrizEtiquetas[fAtrIntCon].end())
    {
        filaCorrespondiente = fAtrIntCon;
    }

    return filaCorrespondiente;
}

void SNIESController::asignarConsolidados(vector<vector<string>>& matrizArchivo, string& etiquetaClave, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
{
    Consolidado* consolidadoNuevo;
    Consolidado* consolidadoViejo;
    string etiquetaCorrespondiente;
    string datoString;
    int datoInt;
    int datoClave;
    int filaCorrespondiente;
    string strCodigoSNIES = "CÓDIGO SNIES DEL PROGRAMA";
    string strAno = "AÑO";
    string strSexo = "SEXO";
    string strSemestre = "SEMESTRE";
    int codigoSNIES;
    int anoActual;
    string sexoActual;
    int semestreActual;
    ProgramaAcademico* programaAsociado;
    map<int, ProgramaAcademico*>::iterator itPrograma;
    //Nos saltamos la primera fila (0) porque son las etiquetas y esas no se guardan sino que se utilizan para mapear
    for (int fila = 1; fila < matrizArchivo.size(); fila++)
    {
        consolidadoNuevo = new Consolidado();
        for (int columna = 0; columna < matrizArchivo[fila].size(); columna++)
        {
            etiquetaCorrespondiente = matrizArchivo[0][columna];
            datoString = matrizArchivo[fila][columna];

            //Buscamos a que tipo de atributo pertenece para agregarlo correspondientemente
            filaCorrespondiente = verificarFilaEtiqueta(etiquetaCorrespondiente, fAtrStrProg, fAtrIntProg, fAtrStrCon, fAtrIntCon);
            if (filaCorrespondiente == fAtrIntCon)
            {
                datoInt = stoi(datoString);
                consolidadoNuevo->agregarDatoInt(etiquetaCorrespondiente, datoInt);
            }
            //El unico atributo int de un programa que deberiamos conseguir es el codigo SNIES
            else if (filaCorrespondiente == fAtrIntProg)
            {
                datoInt = stoi(datoString);
                codigoSNIES = datoInt;
            }
            else
            {
                consolidadoNuevo->agregarDatoString(etiquetaCorrespondiente, datoString);
            }
        }

        //Busco a que programa pertenece para asignarle el consolidado que acabo de crear
        itPrograma = programasAcademicos.find(codigoSNIES);
        if (itPrograma != programasAcademicos.end())
        {
            programaAsociado = itPrograma->second;
            sexoActual = consolidadoNuevo->obtenerDatoString(strSexo);
            anoActual = consolidadoNuevo->obtenerDatoInt(strAno);
            semestreActual = consolidadoNuevo->obtenerDatoInt(strSemestre);
            //Verificamos si ya teniamos este consolidado (sexo-año-semestre)
            try
            {
                //Si ya existia le agregamos el dato clave del archivo
                consolidadoViejo = programaAsociado->buscarConsolidado(sexoActual, anoActual, semestreActual);
                datoClave = consolidadoNuevo->obtenerDatoInt(etiquetaClave);
                consolidadoViejo->agregarDatoInt(etiquetaClave, datoClave);
                delete consolidadoNuevo;
            } catch (invalid_argument& e)
            {
                //Creamos un nuevo consolidado
                programaAsociado->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
            }
        }
        else
        {
            delete consolidadoNuevo;
        }
    }
}

void SNIESController::buscarProgramas(bool exportarArchivo, string &palabraClave, int idComparacion)
{
    list<ProgramaAcademico *> listaProgramas;
    string llaveNombrePrograma = string("PROGRAMA ACADÉMICO");
    string llaveId = string("ID NIVEL DE FORMACIÓN");
    string codigoSNIES = string("CÓDIGO SNIES DEL PROGRAMA");
    string codigoInstitucion = string("CÓDIGO DE LA INSTITUCIÓN");
    string institucionEducacionSuperior = string("INSTITUCIÓN DE EDUCACIÓN SUPERIOR (IES)");
    string METODOLOGIA =string("METODOLOGÍA");
    string rutaOutput = Settings::OUTPUTS_PATH;
    string delimitador = Settings::DELIMITADOR;

    //Imprimimos en terminal las etiquetas que vamos a mostrar de los programas
    cout << codigoSNIES << delimitador;
    cout << llaveNombrePrograma << delimitador;
    cout << codigoInstitucion << delimitador;
    cout << institucionEducacionSuperior << delimitador;
    cout << METODOLOGIA << endl;

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
            cout << programa->consultarDatoInt(codigoSNIES) << delimitador;
            cout << nombre << delimitador;
            cout << programa->consultarDatoInt(codigoInstitucion) << delimitador;
            cout << programa->consultarDatoString(institucionEducacionSuperior) << delimitador;
            cout << programa->consultarDatoString(METODOLOGIA) << endl;
        }
    }

    //Exportamos los resultados a archivos de salida si nos lo han pedido
    if (exportarArchivo)
    {
        for(int gestor = 0; gestor < gestoresArchivos.size(); gestor++)
        {
            try
            {
                gestoresArchivos[gestor]->crearArchivoBuscados(rutaOutput,listaProgramas,matrizEtiquetas);
            }catch(out_of_range& e)
            {
                cout << "No se pudo crear el archivo con los buscados. " << e.what() << endl;
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