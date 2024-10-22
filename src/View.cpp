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
    validezRespuesta = respuestaValida(respuestaEsperada, userAnswer);
    if (validezRespuesta)
    {
        parametrizacionBool = true;
        cout << "A continuacion se procesaran los datos de los programas academicos seleccionados en /programas.csv..." << endl;

        int anio1;
        int anio2;
        solicitarAniosBusqueda(anio1, anio2);

        cout << "Procesando datos ..." << endl;
        controlador.procesarDatosCsv(anio1, ano2);
        cout << "Datos procesados con exito!" << endl;
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
    cout << "Recuerde revisar la carpeta de outputs para los archivos .csv exportados" << endl;
    cout << "Programa Cerrado con exito!" << endl;
}

void View::mostrarDatosExtra()
{
    char opcionYN;
    cout << "A continuacion vamos a mostrar datos relevantes de los programas academicos seleccionados" << "\n"
         << endl;
    cout << "Desea Convertir los datos a un archivo CSV?(Y/N): " << endl;
    cin >> opcionYN;
    opcionYN = tolower(opcionYN);
    cout << "\n";
    // FIXME verificar que el usuario ingrese un valor igual al esperado, return true si es Y, false si es N, y no sale si no retorna un valor válido
    // Simplificar el código de acuerdo a ese ajuste
    if (opcionYN == 'y')
    {
        controlador.calcularDatosExtra(true);
    }

    else
    {
        controlador.calcularDatosExtra(false);
    }
}

void View::buscarPorPalabraClaveYFormacion()
{
    char opcionYN = 'y', opcionCSV;
    string palabraClave;
    bool convertirCSV;
    int idFormacionAcademica;

    while (opcionYN == 'y')
    {
        cout << "Desea hacer una busqueda por palabra clave del nombre del programa(Y/N): " << endl;
        cin >> opcionYN;
        cout << "\n";
        opcionYN = tolower(opcionYN);

        if (opcionYN == 'y')
        {
            cout << "Deseas convertir convertir los datos del programa academico a un CSV?(Y/N): " << endl;
            cin >> opcionCSV;
            cout << "\n";
            opcionCSV = tolower(opcionCSV);

            if (opcionCSV == 'y')
            {
                convertirCSV = true;
            }

            else
            {
                convertirCSV = false;
            }

            cout << "Escriba la palabra clave para buscar los programas por nombre:" << endl;
            cin >> palabraClave;
            cout << endl;

            cout << "Seleccione un nivel de formacion para filtrar: \n 1->Especializacion Universitaria\n 2->Maestria\n 3->Doctorado\n 4->Formacion Tecnica Profesional \n 5->Tecnologico\n 6->Universitario\n 7->Especializacion Tecnico Profesional\n 8->Especializacion Tecnologica\n 10->Especializacion Medico Quirurgica\n " << endl;
            cin >> idFormacionAcademica;
            cout << "\n";
            while ((idFormacionAcademica > 10) || (idFormacionAcademica == 9) || (idFormacionAcademica < 1))
            {
                cout << "Seleccione una opcion entre 1-10 excluyendo el 9\n"
                     << endl;
                cin >> idFormacionAcademica;
            }

            controlador.buscarProgramas(convertirCSV, palabraClave, idFormacionAcademica);
        }
    }
}

bool View::respuestaValida(string& respuestaEsperada, string& respuestaActual)
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
