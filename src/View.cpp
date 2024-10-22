#include "View.h"

View::~View()
{
    controlador.~SNIESController();
}

bool View::mostrarPantallaBienvenido()
{
    bool parametrizacionBool = false;
    bool validezRespuesta;

    cout << "Bienvenido al SNIES-Extractor!" << endl;
    cout << "=========================================" << endl;
    cout << "Recuerde que para el correcto funcionamiento del programa tuvo que haber parametrizado" << endl;
    cout << "antes la carpeta SNIES_EXTRACTOR en el disco duro C:, con sus respectivas carpetas inputs y outputs" << endl;
    cout << "y todos los archivo CSV del SNIES." << endl;
    cout << "Si ya hizo esto, escriba 'Y' y Enter: " << endl;

    string userAnswer;
    cin >> userAnswer;
    cout << endl;

    string respuestaEsperada = string("Y");
    validezRespuesta = esRespuestaValida(respuestaEsperada, userAnswer);
    if (validezRespuesta)
    {
        parametrizacionBool = true;
        cout << "A continuacion se procesaran los datos de los programas academicos seleccionados en /programas.csv..." << endl;

        int anio1;
        int anio2;
        solicitarAniosBusqueda(anio1, anio2);

        try
        {
            cout << "Procesando datos ..." << endl;
            controlador.procesarDatos(anio1, anio2);
            cout << "Datos procesados con exito!" << endl;
        } catch (out_of_range& e)
        {
            cout << e.what() << endl;
            parametrizacionBool = false;
        }
    }
    else
    {
        parametrizacionBool = false;
    }

    return parametrizacionBool;
}

void View::salir()
{
    cout << "Cerrando programa..." << endl;
    cout << "Recuerde revisar la carpeta de outputs para los archivos exportados" << endl;
    cout << "Programa Cerrado con exito!" << endl;
}

void View::mostrarDatosExtra()
{
    string opcionYN;
    cout << "A continuacion vamos a mostrar datos relevantes de los programas academicos seleccionados" << "\n"
         << endl;
    cout << "Desea Convertir los datos a unos archivos de resultados?(Y/N): " << endl;
    cin >> opcionYN;
    cout << endl;

    string respuestaEsperada = string("Y");
    bool exportarArchivo = esRespuestaValida(respuestaEsperada, opcionYN);
    controlador.calcularDatosExtra(exportarArchivo);
}

void View::buscarPorPalabraClaveYFormacion()
{
    string opcionYN = string("Y");
    string opcionCSV;
    string palabraClave;
    bool convertirCSV;
    int idFormacionAcademica = 0;
    string respuestaEsperada = string("Y");
    bool hacerBusqueda = esRespuestaValida(respuestaEsperada, opcionYN);
    bool nivelFormacionInvalido;

    while (hacerBusqueda)
    {
        cout << "Desea hacer una busqueda por palabra clave del nombre del programa(Y/N): " << endl;
        cin >> opcionYN;
        cout << endl;

        hacerBusqueda = esRespuestaValida(respuestaEsperada, opcionYN);
        if (hacerBusqueda)
        {
            cout << "Deseas convertir los datos del programa academico a un archivo de salida?(Y/N): " << endl;
            cin >> opcionCSV;
            cout << endl;

            convertirCSV = esRespuestaValida(respuestaEsperada, opcionCSV);

            cout << "Escriba la palabra clave para buscar los programas por nombre:" << endl;
            cin >> palabraClave;
            cout << endl;

            string nivelesFormacion = string("Seleccione un nivel de formacion para filtrar: \n 1->Especializacion Universitaria\n 2->Maestria\n 3->Doctorado\n 4->Formacion Tecnica Profesional \n 5->Tecnologico\n 6->Universitario\n 7->Especializacion Tecnico Profesional\n 8->Especializacion Tecnologica\n 10->Especializacion Medico Quirurgica\n");
            string mensajeError = string("Ingrese un nivel de formacion valido");
            nivelFormacionInvalido = false;
            while ((idFormacionAcademica > 10) || (idFormacionAcademica == 9) || (idFormacionAcademica < 1))
            {
                if (nivelFormacionInvalido)
                {
                    cout << "Seleccione una opcion entre 1-10 excluyendo el 9" << endl;
                }
                else
                {
                    nivelFormacionInvalido = true;
                }
                idFormacionAcademica = solicitarIntValido(nivelesFormacion, mensajeError);
            }

            controlador.buscarProgramas(convertirCSV, palabraClave, idFormacionAcademica);
        }
    }
}

bool View::esRespuestaValida(string& respuestaEsperada, string& respuestaActual)
{
    bool validez = false;
    if (utilidadObj.minusculasSinEspacios(respuestaActual) == utilidadObj.minusculasSinEspacios(respuestaEsperada))
    {
        validez = true;
    }
    else
    {
        validez = false;
    }
    return validez;
}

int View::solicitarIntValido(string& mensajeInput, string& mensajeError)
{
    string userInput = '';
    bool condicion = false;
    bool intInvalido = false;

    while (!condicion)
    {
        if (intInvalido)
        {
            //Imprimimos un mensaje de error si no se puso un int válido
            cout << mensajeError << endl;
        }
        else
        {
            intInvalido = true;
        }

        //Imprimimos el mensaje para el input y leemos la respuesta del usuario
        cout << mensajeInput << endl;
        cin >> userInput;
        cout << endl;

        //Verificamos que el string se pueda convertir a int para salir del bucle
        condicion = utilidadObj.isConvertibleToInt(userInput);
    }

    return stoi(userInput);
}

void View::solicitarAniosBusqueda(int& anio1, int& anio2)
{
    string anoAux;

    //Leemos el primer anio de busqueda
    string mensajeInput = string("Ingrese el primer anio de busqueda:");
    string mensajeError = string("Por favor introduzca un anio valido.");
    anio1 = solicitarIntValido(mensajeInput, mensajeError);

    //Leemos el segundo anio de busqueda
    bool anioInvalido = false;
    anio2 = anio1;
    mensajeInput = string("Ingrese el ultimo anio de busqueda:");
    mensajeError = string("Por favor introduzca un anio valido.");
    while (anio1 == anio2)
    {
        if (anioInvalido)
        {
            cout << "El segundo anio de busqueda debe ser distinto al primero: " << anio1 << endl;
        }
        else
        {
            anioInvalido = true;
        }
        anio2 = solicitarIntValido(mensajeInput, mensajeError);
    }

    // Organizo los años de menor a mayor
    organizarAnios(anio1, anio2);
}

void View::organizarAnios(int& anio1, int& anio2)
{
    int anioAux;
    if (anio1 > anio2)
    {
        anioAux = anio1;
        anio1 = anio2;
        anio2 = anioAux;
    }
}
