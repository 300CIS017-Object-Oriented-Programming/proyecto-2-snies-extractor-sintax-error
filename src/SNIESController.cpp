#include "SNIESController.h"

using namespace std;

SNIESController::SNIESController()
{
    try
    {
        // Inicialización de los gestores necesarios para el controlador
        inicializarGestores();

        // Inicialización de las etiquetas que se utilizarán en el procesamiento de los datos
        inicializarEtiquetas();
    }
    catch (out_of_range &e)
    {
        // Si se produce una excepción out_of_range, se captura y se lanza una nueva excepción con un mensaje más detallado
        string errorMsg = string("No se pudo inicializar el Controlador. ") + e.what();

        // Lanzar la excepción con el mensaje modificado
        throw out_of_range(errorMsg);
    }
}

void SNIESController::inicializarGestores()
{
    // Agregar instancias de diferentes gestores al vector gestoresArchivos
    gestoresArchivos.push_back(new GestorCsv());  // Agrega un gestor para manejar archivos CSV
    gestoresArchivos.push_back(new GestorTxt());  // Agrega un gestor para manejar archivos TXT
    gestoresArchivos.push_back(new GestorJSON()); // Agrega un gestor para manejar archivos JSON
}

void SNIESController::inicializarEtiquetas()
{
    matrizEtiquetas = vector<vector<string>>(6);
    // Leer el archivo de configuracion para obtener la información necesaria
    leerArchivoConfiguracion(matrizEtiquetas);
}

void SNIESController::leerArchivoConfiguracion(vector<vector<string>> &matriz)
{
    // Ruta al archivo de configuración que contiene las etiquetas
    string rutaConfig = Settings::ETIQUETAS_CONFIG_PATH;

    // Abrir el archivo de configuración
    ifstream archivoConfig(rutaConfig);
    bool archivoAbierto = archivoConfig.is_open();
    if (!archivoAbierto)
    {
        // Lanzar una excepción si no se puede abrir el archivo
        throw out_of_range("Error al abrir el archivo de configuracion de atributos");
    }

    string fila;
    // Inicializamos la posición de fila en -1, para que cuando leamos la primera categoría, comience en la fila 0 de la matriz
    int posFilaMatriz = -1;

    // Leer el archivo línea por línea
    while (getline(archivoConfig, fila))
    {
        /* Las diferentes categorías de etiquetas comienzan con una fila que contiene el nombre de la categoría.
         * El primer carácter del nombre de la categoría siempre comienza con '-'.
         * Esto se utiliza para saber cuándo cambiamos de categoría, lo que implica cambiar de fila en la matriz de etiquetas.
         */
        if (fila[0] == '-')
        {
            // Incrementar el índice de la fila de la matriz cuando cambiamos de categoría
            ++posFilaMatriz;
        }
        else
        {
            // Agregar la etiqueta a la fila actual de la matriz
            matriz[posFilaMatriz].push_back(fila);
        }
    }

    // Cerrar el archivo una vez hemos terminado de leer
    archivoConfig.close();
}

SNIESController::~SNIESController()
{
    // Iteramos sobre el mapa de programas académicos
    for (auto &pair : programasAcademicos)
    {
        // Llamamos al destructor del objeto ProgramaAcademico de forma explícita
        ((pair).second)->~ProgramaAcademico();
        // Liberamos la memoria del objeto ProgramaAcademico
        delete pair.second;
    }

    // Iteramos sobre el vector de gestores de archivos para liberar memoria
    for (int posGestor = 0; posGestor < gestoresArchivos.size(); posGestor++)
    {
        // Liberamos la memoria de cada gestor de archivos
        delete gestoresArchivos[posGestor];
    }
}

void SNIESController::procesarDatos(int anio1, int anio2)
{
    try
    {
        // Definimos las filas de la matriz de etiquetas donde se encuentran los atributos
        int FILA_ATRIBUTOS_STRING_PROGRAMA = 0;
        int FILA_ATRIBUTOS_INT_PROGRAMA = 1;
        int FILA_ATRIBUTOS_STRING_CONSOLIDADO = 2;
        int FILA_ATRIBUTOS_INT_CONSOLIDADO = 3;
        int FILA_SEXOS_DISPONIBLES = 4;
        int FILA_ANOS_DISPONIBLES = 5;

        // Generamos la lista de años a procesar y la almacenamos en la matriz de etiquetas
        vector<string> anos = generarAniosBusqueda(anio1, anio2);
        matrizEtiquetas[FILA_ANOS_DISPONIBLES] = anos;

        vector<int> codigosSNIES;
        vector<string> etiquetasParaLeer;
        vector<vector<string>> matrizArchivo;
        string rutaActual;

        // Leemos los códigos SNIES de los programas que se desean buscar
        rutaActual = Settings::PROGRAMAS_FILTRAR_FILE_PATH;
        // Utilizamos el primer gestor (0) para leer los programas
        codigosSNIES = gestoresArchivos[0]->leerProgramas(rutaActual);

        /*
         * Leemos el primer archivo con la información base de los programas
         * y los consolidados para este archivo en particular.
         */

        // Seleccionamos las etiquetas que nos interesan para la lectura
        string strAdmitidos = "ADMITIDOS";
        string strAno = "AÑO";
        string strSexo = "SEXO";
        string strSemestre = "SEMESTRE";
        seleccionarEtiquetas(FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, etiquetasParaLeer);
        etiquetasParaLeer.push_back(strAdmitidos);
        etiquetasParaLeer.push_back(strAno);
        etiquetasParaLeer.push_back(strSexo);
        etiquetasParaLeer.push_back(strSemestre);
        // Hacemos que las etiquetas para leer se pongan en minusculas, sin espacios y sin tildes
        corregirEtiquetas(etiquetasParaLeer);

        // Leemos el archivo de admitidos del primer año disponible
        rutaActual = Settings::ADMITIDOS_FILE_PATH + matrizEtiquetas[FILA_ANOS_DISPONIBLES][0];
        // FIXME: Esta devolviendo vacio
        matrizArchivo = gestoresArchivos[0]->leerArchivo(rutaActual, etiquetasParaLeer, codigosSNIES);

        // Procesamos los datos obtenidos para crear los programas académicos
        crearProgramas(matrizArchivo, FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, FILA_ATRIBUTOS_STRING_CONSOLIDADO, FILA_ATRIBUTOS_INT_CONSOLIDADO);

        /*
         * Ahora leemos el resto de los archivos para obtener los consolidados
         * y asignarlos al programa académico correspondiente según el código SNIES.
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
        // Bandera para saltar la lectura del primer archivo de admitidos
        bool admitidosFlag = true;
        string strCodigoSNIES = "CÓDIGO SNIES DEL PROGRAMA";
        string strAnoActual;

        // Iteramos sobre los atributos clave y los años disponibles para leer los archivos correspondientes
        for (int posAtributosClave = 0; posAtributosClave < atributosClave[0].size(); posAtributosClave++)
        {
            etiquetaClave = atributosClave[0][posAtributosClave];
            etiquetasParaLeer.clear();
            etiquetasParaLeer.push_back(strCodigoSNIES);
            etiquetasParaLeer.push_back(etiquetaClave);
            etiquetasParaLeer.push_back(strAno);
            etiquetasParaLeer.push_back(strSexo);
            etiquetasParaLeer.push_back(strSemestre);
            // Quitamos tildes espacios y ponemos en minusculas
            corregirEtiquetas(etiquetasParaLeer);

            // Leemos los archivos de cada año para cada atributo clave
            for (int columna = 0; columna < matrizEtiquetas[FILA_ANOS_DISPONIBLES].size(); columna++)
            {
                if (!admitidosFlag)
                {
                    rutaActual = atributosClave[1][posAtributosClave] + matrizEtiquetas[FILA_ANOS_DISPONIBLES][columna];
                    matrizArchivo = gestoresArchivos[0]->leerArchivo(rutaActual, etiquetasParaLeer, codigosSNIES);
                    // Asignamos los datos leídos a los consolidados correspondientes
                    asignarConsolidados(matrizArchivo, etiquetaClave, FILA_ATRIBUTOS_STRING_PROGRAMA, FILA_ATRIBUTOS_INT_PROGRAMA, FILA_ATRIBUTOS_STRING_CONSOLIDADO, FILA_ATRIBUTOS_INT_CONSOLIDADO);
                }
                else
                {
                    admitidosFlag = false;
                }
            }
        }
        // En este punto ya hemos procesado todos los programas y consolidados

        // Creamos el archivo de resultados con cada gestor de archivos
        string rutaOutput = Settings::OUTPUTS_PATH;
        for (int gestor = 0; gestor < gestoresArchivos.size(); gestor++)
        {
            try
            {
                gestoresArchivos[gestor]->crearArchivo(rutaOutput, programasAcademicos, matrizEtiquetas);
            }
            catch (out_of_range &e)
            {
                // Mostramos el error si no se pudo crear el archivo de resultados
                cout << "No se pudo crear el archivo de resultados. " << e.what() << endl;
            }
        }
    }
    catch (out_of_range &e)
    {
        // Lanzamos una excepción si ocurre un error al abrir un archivo
        string errorMsg = string("Error FATAL al abrir un archivo. ") + e.what();
        throw out_of_range(errorMsg);
    }
}

void SNIESController::seleccionarEtiquetas(int filaMin, int filaMax, vector<string> &etiquetasParaLeer)
{
    // Limpiamos el vector de etiquetas para asegurarnos de que esté vacío antes de agregar nuevas etiquetas
    etiquetasParaLeer.clear();

    // Iteramos sobre las filas especificadas (de filaMin a filaMax inclusive)
    for (int fila = filaMin; fila <= filaMax; fila++)
    {
        // Para cada fila, iteramos sobre las columnas para agregar cada etiqueta al vector
        for (int columna = 0; columna < matrizEtiquetas[fila].size(); columna++)
        {
            // Agregamos la etiqueta correspondiente a la lista de etiquetas a leer
            etiquetasParaLeer.push_back(matrizEtiquetas[fila][columna]);
        }
    }
}

void SNIESController::corregirEtiquetas(vector<string> &etiquetasParaLeer)
{
    // Eliminamos tildes y espacios, y ponemos en minusculas las etiquetas
    for (int posVector = 0; posVector < etiquetasParaLeer.size(); posVector++)
    {
        etiquetasParaLeer[posVector] = utilidadObj.minusculasSinEspacios(etiquetasParaLeer[posVector]);
    }
}

void SNIESController::crearProgramas(vector<vector<string>> &matrizArchivo, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
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

    // Iteramos sobre las filas del archivo, comenzando desde la fila 1, ya que la fila 0 contiene las etiquetas
    for (int fila = 1; fila < matrizArchivo.size(); fila++)
    {
        programaNuevo = new ProgramaAcademico(); // Creamos un nuevo programa
        consolidadoNuevo = new Consolidado();    // Creamos un nuevo consolidado

        // Iteramos sobre las columnas de la fila actual
        for (int columna = 0; columna < matrizArchivo[fila].size(); columna++)
        {
            // Obtenemos la etiqueta y el dato correspondiente de la matriz
            etiquetaCorrespondiente = matrizArchivo[0][columna];
            datoString = matrizArchivo[fila][columna];

            // Verificamos a qué tipo de atributo pertenece la etiqueta y agregamos los datos
            filaCorrespondiente = verificarFilaEtiqueta(etiquetaCorrespondiente, fAtrStrProg, fAtrIntProg, fAtrStrCon, fAtrIntCon);

            // Si la etiqueta pertenece a un atributo de tipo string del programa, lo agregamos
            if (filaCorrespondiente == fAtrStrProg)
            {
                programaNuevo->agregarElementoTipoString(etiquetaCorrespondiente, datoString);
            }
            // Si la etiqueta pertenece a un atributo de tipo int del programa, lo agregamos
            else if (filaCorrespondiente == fAtrIntProg)
            {
                datoInt = stoi(datoString); // Convertimos el string a int
                programaNuevo->agregarElementoTipoInt(etiquetaCorrespondiente, datoInt);
            }
            // Si la etiqueta pertenece a un atributo de tipo int del consolidado, lo agregamos
            else if (filaCorrespondiente == fAtrIntCon)
            {
                datoInt = stoi(datoString); // Convertimos el string a int
                consolidadoNuevo->agregarDatoInt(etiquetaCorrespondiente, datoInt);
            }
            // Si la etiqueta pertenece a un atributo de tipo string del consolidado, lo agregamos
            else
            {
                consolidadoNuevo->agregarDatoString(etiquetaCorrespondiente, datoString);
            }
        }

        // Intentamos agregar el nuevo programa al mapa de programas académicos
        auto resultadoInsert = programasAcademicos.insert(make_pair(programaNuevo->consultarDatoInt(strCodigoSNIES), programaNuevo));
        programaNuevoCreado = resultadoInsert.second; // Verificamos si el programa fue creado exitosamente

        // Obtenemos los datos clave del consolidado (sexo, año, semestre)
        sexoActual = consolidadoNuevo->obtenerDatoString(strSexo);
        anoActual = consolidadoNuevo->obtenerDatoInt(strAno);
        semestreActual = consolidadoNuevo->obtenerDatoInt(strSemestre);

        // Si el programa ya existía, asignamos el consolidado al programa existente
        if (!programaNuevoCreado)
        {
            // Eliminamos el programa recién creado ya que no se insertó
            delete programaNuevo;

            // Asignamos el consolidado al programa existente en el mapa
            ((resultadoInsert.first)->second)->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
        }
        else
        {
            // Si el programa es nuevo, le asignamos el consolidado correspondiente
            programaNuevo->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
        }
    }
}

int SNIESController::verificarFilaEtiqueta(string &etiquetaCorrespondiente, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
{
    int filaCorrespondiente = -1;
    vector<string>::iterator itFilaMatriz1;
    vector<string>::iterator itFilaMatriz2;
    vector<string>::iterator itFilaMatriz3;

    vector<string> filaAtrStrProg = matrizEtiquetas[fAtrStrProg];
    vector<string> filaAtrIntProg = matrizEtiquetas[fAtrIntProg];
    vector<string> filaAtrIntCon = matrizEtiquetas[fAtrIntCon];

    corregirEtiquetas(filaAtrStrProg);
    corregirEtiquetas(filaAtrIntProg);
    corregirEtiquetas(filaAtrIntCon);

    // Buscamos la etiqueta en la fila de atributos string del programa
    itFilaMatriz1 = find(filaAtrStrProg.begin(), filaAtrStrProg.end(), etiquetaCorrespondiente);

    // Buscamos la etiqueta en la fila de atributos int del programa
    itFilaMatriz2 = find(filaAtrIntProg.begin(), filaAtrIntProg.end(), etiquetaCorrespondiente);

    // Buscamos la etiqueta en la fila de atributos int del consolidado
    itFilaMatriz3 = find(filaAtrIntCon.begin(), filaAtrIntCon.end(), etiquetaCorrespondiente);

    // Si encontramos la etiqueta en la fila de atributos string del programa
    if (itFilaMatriz1 != filaAtrStrProg.end())
    {
        filaCorrespondiente = fAtrStrProg;
    }
    // Si encontramos la etiqueta en la fila de atributos int del programa
    else if (itFilaMatriz2 != filaAtrIntProg.end())
    {
        filaCorrespondiente = fAtrIntProg;
    }
    // Si encontramos la etiqueta en la fila de atributos int del consolidado
    else if (itFilaMatriz3 != filaAtrIntCon.end())
    {
        filaCorrespondiente = fAtrIntCon;
    }

    // Devolvemos el valor de la fila correspondiente a la etiqueta
    return filaCorrespondiente;
}

void SNIESController::asignarConsolidados(vector<vector<string>> &matrizArchivo, string &etiquetaClave, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon)
{
    Consolidado *consolidadoNuevo;
    Consolidado *consolidadoViejo;
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
    ProgramaAcademico *programaAsociado;
    map<int, ProgramaAcademico *>::iterator itPrograma;

    // Nos saltamos la primera fila porque son las etiquetas y no necesitamos guardarlas
    for (int fila = 1; fila < matrizArchivo.size(); fila++)
    {
        consolidadoNuevo = new Consolidado();

        // Iteramos por las columnas para leer los datos correspondientes al consolidado
        for (int columna = 0; columna < matrizArchivo[fila].size(); columna++)
        {
            etiquetaCorrespondiente = matrizArchivo[0][columna]; // Etiqueta de la columna actual
            datoString = matrizArchivo[fila][columna];           // Dato leído de la fila actual

            // Verificamos a qué tipo de atributo pertenece para asignarlo correctamente
            filaCorrespondiente = verificarFilaEtiqueta(etiquetaCorrespondiente, fAtrStrProg, fAtrIntProg, fAtrStrCon, fAtrIntCon);

            if (filaCorrespondiente == fAtrIntCon) // Atributo numérico del consolidado
            {
                datoInt = stoi(datoString);
                consolidadoNuevo->agregarDatoInt(etiquetaCorrespondiente, datoInt); // Guardamos el dato en el consolidado
            }
            else if (filaCorrespondiente == fAtrIntProg) // Atributo numérico del programa (solo el código SNIES)
            {
                datoInt = stoi(datoString);
                codigoSNIES = datoInt; // Guardamos el código SNIES para asignar el consolidado al programa correcto
            }
            else // Atributo de tipo string
            {
                consolidadoNuevo->agregarDatoString(etiquetaCorrespondiente, datoString);
            }
        }

        // Buscamos el programa académico correspondiente al código SNIES
        itPrograma = programasAcademicos.find(codigoSNIES);
        if (itPrograma != programasAcademicos.end()) // Si encontramos el programa
        {
            programaAsociado = itPrograma->second; // Obtenemos el programa
            sexoActual = consolidadoNuevo->obtenerDatoString(strSexo);
            anoActual = consolidadoNuevo->obtenerDatoInt(strAno);
            semestreActual = consolidadoNuevo->obtenerDatoInt(strSemestre);

            // Verificamos si ya existe un consolidado con el mismo sexo, año y semestre
            try
            {
                consolidadoViejo = programaAsociado->buscarConsolidado(sexoActual, anoActual, semestreActual);
                // Si ya existe, añadimos el dato clave al consolidado existente
                datoClave = consolidadoNuevo->obtenerDatoInt(etiquetaClave);
                consolidadoViejo->agregarDatoInt(etiquetaClave, datoClave);
                delete consolidadoNuevo; // Eliminamos el consolidado nuevo ya que el dato fue agregado al existente
            }
            catch (invalid_argument &e) // Si no existe el consolidado, lo creamos
            {
                programaAsociado->setConsolidado(sexoActual, anoActual, semestreActual, consolidadoNuevo);
            }
        }
        else // Si no encontramos el programa, eliminamos el consolidado nuevo
        {
            delete consolidadoNuevo;
        }
    }
}

vector<string> SNIESController::generarAniosBusqueda(int anio1, int anio2)
{
    // Calculamos el tamaño del vector que contendrá los años de búsqueda
    int tamanioVector = (anio2 - anio1) + 1;

    // Inicializamos el vector con el tamaño calculado
    vector<string> anosBusqueda = vector<string>(tamanioVector);

    int anioActual = anio1; // Iniciamos con el año inicial
    for (int posVector = 0; posVector < anosBusqueda.size(); posVector++)
    {
        // Convertimos el año actual a string y lo asignamos a la posición correspondiente en el vector
        anosBusqueda[posVector] = to_string(anioActual);
        anioActual++; // Pasamos al siguiente año
    }

    return anosBusqueda; // Devolvemos el vector con los años generados
}

void SNIESController::buscarProgramas(bool exportarArchivo, string &palabraClave, int idComparacion)
{
    list<ProgramaAcademico *> listaProgramas; // Lista para almacenar los programas que coinciden con la búsqueda
    // Definición de las llaves para acceder a los datos del programa
    string llaveNombrePrograma = string("PROGRAMA ACADÉMICO");
    string llaveId = string("ID NIVEL DE FORMACIÓN");
    string codigoSNIES = string("CÓDIGO SNIES DEL PROGRAMA");
    string codigoInstitucion = string("CÓDIGO DE LA INSTITUCIÓN");
    string institucionEducacionSuperior = string("INSTITUCIÓN DE EDUCACIÓN SUPERIOR (IES)");
    string METODOLOGIA = string("METODOLOGÍA");
    string rutaOutput = Settings::OUTPUTS_PATH; // Ruta de salida para los archivos exportados
    string delimitador = Settings::DELIMITADOR; // Delimitador para la salida

    // Imprimimos en terminal las etiquetas que vamos a mostrar de los programas
    cout << codigoSNIES << delimitador;
    cout << llaveNombrePrograma << delimitador;
    cout << codigoInstitucion << delimitador;
    cout << institucionEducacionSuperior << delimitador;
    cout << METODOLOGIA << endl;

    // Iteramos sobre todos los programas académicos
    for (map<int, ProgramaAcademico *>::iterator it = programasAcademicos.begin(); it != programasAcademicos.end(); ++it)
    {
        ProgramaAcademico *programa = it->second;                           // Obtenemos el programa actual
        string nombre = programa->consultarDatoString(llaveNombrePrograma); // Consultamos el nombre del programa
        int id = stoi(programa->consultarDatoString(llaveId));              // Convertimos el ID a entero

        // Verificamos si el nombre contiene la palabra clave y si el ID coincide
        if (nombre.find(palabraClave) != string::npos && id == idComparacion)
        {
            listaProgramas.push_back(programa); // Agregamos el programa a la lista de resultados
            // Imprimimos los datos del programa en la terminal
            cout << programa->consultarDatoInt(codigoSNIES) << delimitador;
            cout << nombre << delimitador;
            cout << programa->consultarDatoInt(codigoInstitucion) << delimitador;
            cout << programa->consultarDatoString(institucionEducacionSuperior) << delimitador;
            cout << programa->consultarDatoString(METODOLOGIA) << endl;
        }
    }

    // Exportamos los resultados a archivos de salida si se solicita
    if (exportarArchivo)
    {
        for (int gestor = 0; gestor < gestoresArchivos.size(); gestor++)
        {
            try
            {
                // Llamamos al método para crear archivos con los programas buscados
                gestoresArchivos[gestor]->crearArchivoBuscados(rutaOutput, listaProgramas, matrizEtiquetas);
            }
            catch (out_of_range &e) // Manejo de errores en caso de que no se pueda crear el archivo
            {
                cout << "No se pudo crear el archivo con los buscados. " << e.what() << endl;
            }
        }
    }
}

void SNIESController::calcularDatosExtra(bool exportarArchivo)
{
    // Inicializamos las matrices y variables para almacenar los resultados
    vector<vector<string>> matrizFinal;
    vector<vector<string>> matrizEtiquetas1;
    vector<vector<string>> matrizEtiquetas2;
    vector<vector<string>> matrizEtiquetas3;
    vector<string> etiquetas1;
    string Matriculados = "Matriculados";                                 // Etiqueta para matriculados
    string Neos = "PRIMER CURSO";                                         // Etiqueta para estudiantes de primer curso
    string Metodologia = "ID METODOLOGÍA";                                // Etiqueta para metodología
    string CodigoSnies = "CÓDIGO SNIES DEL PROGRAMA";                     // Etiqueta para el código SNIES
    string NombrePrograma = "PROGRAMA ACADÉMICO";                         // Etiqueta para el nombre del programa
    string NombreInstitucion = "INSTITUCIÓN DE EDUCACIÓN SUPERIOR (IES)"; // Etiqueta para el nombre de la institución

    // Generamos etiquetas para los años de matrícula
    for (string ano : matrizEtiquetas[5])
    {
        string etiqueta1 = "Suma Estudiantes Matriculados de Programas Seleccionados (Presencial o Virtual) año " + ano;
        etiquetas1.push_back(etiqueta1);
    }
    matrizEtiquetas1.push_back(etiquetas1); // Agregamos las etiquetas de matrícula a la matriz

    // Generamos etiquetas para la diferencia porcentual anual de NEOS
    vector<string> etiquetas2 = {
        "Codigo Snies",
        "Nombre del Programa",
        "Nombre del Institucion"};
    for (size_t i = 0; i < matrizEtiquetas[5].size() - 1; ++i)
    {
        std::string ano1 = matrizEtiquetas[5][i];
        std::string ano2 = matrizEtiquetas[5][i + 1];
        etiquetas2.push_back("Diferencial porcentual anual de NEOS de los años " + ano1 + " y " + ano2);
    }
    matrizEtiquetas2.push_back(etiquetas2); // Agregamos las etiquetas de diferencias a la matriz

    // Generamos etiquetas para programas sin NEOS en los últimos 3 semestres
    vector<string> etiquetas3 = {
        "Codigo Snies", "Nombre del Programa sin NEOS en los ultimos 3 semestres"};
    matrizEtiquetas3.push_back(etiquetas3); // Agregamos estas etiquetas a la matriz

    // Iteramos sobre todos los programas académicos
    for (auto &it : programasAcademicos)
    {
        // Variables para almacenar sumas y diferencias
        int SumaNeosPrimerSemestre = 0;
        int SumaNeosSegundoSemestre = 0;
        int SumaNeosTercerSemestre = 0;
        int SumaNeosCuartoSemestre = 0;
        int SumaNeosPrimerAno = 0;
        int SumaNeosSegundoAno = 0;
        int suma = 0;
        int diferenciaNeos = 0;
        vector<string> sumaMatriculados;         // Vector para almacenar las sumas de matriculados
        ProgramaAcademico *programa = it.second; // Obtenemos el programa actual

        // Acceso a los datos del programa académico desde los mapas
        int idMetodologiaBuscada = programa->consultarDatoInt(Metodologia);

        // Solo consideramos programas con ciertas metodologías
        if (idMetodologiaBuscada == 1 || idMetodologiaBuscada == 3)
        {
            // Sumar estudiantes matriculados del primer y segundo año
            for (string ano : matrizEtiquetas[5])
            {
                for (string sexo : matrizEtiquetas[4])
                {
                    // Sumar matriculados por sexo y año
                    Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(ano), 1);
                    suma += stoi(consolidado1->obtenerDatoString(Matriculados));
                    Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(ano), 2);
                    suma += stoi(consolidado2->obtenerDatoString(Matriculados));
                }
                sumaMatriculados.push_back(to_string(suma)); // Agregamos la suma a la lista
            }
            matrizEtiquetas2.push_back(sumaMatriculados); // Agregamos la suma de matriculados a la matriz
        }

        // Generamos la lista de diferencias para los NEOS
        vector<string> diferenciasVariosAnosNeos = {to_string(programa->consultarDatoInt(CodigoSnies)), programa->consultarDatoString(NombrePrograma), programa->consultarDatoString(NombreInstitucion)};

        // Calculamos las diferencias porcentuales entre años
        for (int i = 0; i < matrizEtiquetas[5].size() - 1; ++i)
        {
            string ano1 = matrizEtiquetas[5][i];
            string ano2 = matrizEtiquetas[5][i + 1];

            for (string sexo : matrizEtiquetas[4])
            {
                Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(ano1), 1);
                SumaNeosPrimerAno += stoi(consolidado1->obtenerDatoString(Neos));
                Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(ano1), 2);
                SumaNeosPrimerAno += stoi(consolidado2->obtenerDatoString(Neos));

                Consolidado *consolidado3 = programa->buscarConsolidado(sexo, stoi(ano2), 1);
                SumaNeosSegundoAno += stoi(consolidado3->obtenerDatoString(Neos));
                Consolidado *consolidado4 = programa->buscarConsolidado(sexo, stoi(ano2), 2);
                SumaNeosSegundoAno += stoi(consolidado4->obtenerDatoString(Neos));
            }

            // Calculamos la diferencia porcentual de NEOS
            if (SumaNeosPrimerAno != 0)
            {
                diferenciaNeos = ((SumaNeosSegundoAno - SumaNeosPrimerAno) * 100) / SumaNeosPrimerAno;
            }
            else
            {
                diferenciaNeos = 0; // Evitamos división por cero
            }
            diferenciasVariosAnosNeos.push_back(to_string(diferenciaNeos)); // Agregamos la diferencia a la lista
        }
        matrizEtiquetas2.push_back(diferenciasVariosAnosNeos); // Agregamos las diferencias a la matriz

        // Calculamos las sumas de NEOS para el penúltimo y último año
        string penultimoAno = matrizEtiquetas[5][matrizEtiquetas.size() - 2];
        string ultimoAno = matrizEtiquetas[5][matrizEtiquetas.size() - 1];
        for (string sexo : matrizEtiquetas[4])
        {
            Consolidado *consolidado1 = programa->buscarConsolidado(sexo, stoi(penultimoAno), 1);
            SumaNeosPrimerSemestre += stoi(consolidado1->obtenerDatoString(Neos));
            Consolidado *consolidado2 = programa->buscarConsolidado(sexo, stoi(penultimoAno), 2);
            SumaNeosSegundoSemestre += stoi(consolidado2->obtenerDatoString(Neos));

            Consolidado *consolidado3 = programa->buscarConsolidado(sexo, stoi(ultimoAno), 1);
            SumaNeosTercerSemestre += stoi(consolidado3->obtenerDatoString(Neos));
            Consolidado *consolidado4 = programa->buscarConsolidado(sexo, stoi(ultimoAno), 2);
            SumaNeosCuartoSemestre += stoi(consolidado4->obtenerDatoString(Neos));
        }

        // Si no hay NEOS en los últimos 3 semestres, guardamos el programa
        if ((SumaNeosPrimerSemestre == 0 && SumaNeosSegundoSemestre == 0 && SumaNeosTercerSemestre == 0) || (SumaNeosSegundoSemestre == 0 && SumaNeosTercerSemestre == 0 && SumaNeosCuartoSemestre == 0))
        {
            etiquetas3 = {to_string(programa->consultarDatoInt(CodigoSnies)), programa->consultarDatoString(NombrePrograma)};
            matrizEtiquetas3.push_back(etiquetas3); // Agregamos a la matriz
        }

        // Combinamos todas las matrices de resultados
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas1.begin(), matrizEtiquetas1.end());
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas2.begin(), matrizEtiquetas2.end());
        matrizFinal.insert(matrizFinal.end(), matrizEtiquetas3.begin(), matrizEtiquetas3.end());

        // Imprimimos los resultados finales en la consola
        for (const auto &fila : matrizFinal)
        {
            for (size_t i = 0; i < fila.size(); ++i)
            {
                cout << fila[i]; // Imprimimos cada dato
                if (i < fila.size() - 1)
                {
                    cout << ";"; // Agregamos un delimitador entre datos
                }
            }
            cout << endl; // Nueva línea al final de cada fila
        }

        // Si se solicita, exportamos los resultados a archivos
        if (exportarArchivo)
        {
            string ruta = Settings::OUTPUTS_PATH;
            for (GestorArchivos *gestor : gestoresArchivos)
            {
                try
                {
                    // Llamamos al método para crear archivos con los datos extra
                    gestor->crearArchivoExtra(ruta, matrizFinal);
                }
                catch (out_of_range &e) // Manejo de errores en caso de que no se pueda crear el archivo
                {
                    cout << "No se pudo crear el archivo con los datos extra: " << e.what() << endl;
                }
            }
        }
    }
}
