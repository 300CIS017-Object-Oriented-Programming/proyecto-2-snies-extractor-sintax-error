#include "ProgramaAcademico.h"

using namespace std;

// Constructor por defecto de la clase ProgramaAcademico
ProgramaAcademico::ProgramaAcademico()
{
    // consolidados = vector<Consolidado *>(8);
}

// Método para agregar un dato de tipo string al mapa de datos string
// nombreDato: referencia a la clave asociada al dato
// dato: referencia al valor string que se va a almacenar
void ProgramaAcademico::agregarElementoTipoString(string& nombreDato, string& dato)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(nombreDato);
    // Almacena el valor string en el mapa mapaDatosString
    mapaDatosString[llave] = dato;
}

// Método para agregar un dato de tipo entero al mapa de datos enteros
// nombreDato: referencia a la clave asociada al dato
// dato: valor entero que se va a almacenar
void ProgramaAcademico::agregarElementoTipoInt(string& nombreDato, int dato)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(nombreDato);
    // Almacena el valor entero en el mapa mapaDatosEnteros
    mapaDatosEnteros[llave] = dato;
}

// Método para consultar un dato string en el mapa de datos string
// clave: referencia a la clave del dato a consultar
// return: el valor string asociado a la clave
string ProgramaAcademico::consultarDatoString(string& clave)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Verifica si la clave existe en el mapa, de lo contrario lanza una excepción
    if (mapaDatosString.find(llave) == mapaDatosString.end())
    {
        // Genera un mensaje de error con la clave no encontrada
        string msg = string("Llave No Encontrada: '") + llave + string("'");
        throw invalid_argument(msg);  // Lanza excepción si la clave no existe
    }
    // Retorna el valor string asociado a la clave
    return mapaDatosString.at(llave);
}

// Método para consultar un dato entero en el mapa de datos enteros
// clave: referencia a la clave del dato a consultar
// return: el valor entero asociado a la clave
int ProgramaAcademico::consultarDatoInt(string& clave)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(clave);
    // Verifica si la clave existe en el mapa, de lo contrario lanza una excepción
    if (mapaDatosEnteros.find(llave) == mapaDatosEnteros.end())
    {
        // Genera un mensaje de error con la clave no encontrada
        string msg = string("Llave No Encontrada: '") + llave + string("'");
        throw invalid_argument(msg);  // Lanza excepción si la clave no existe
    }
    // Retorna el valor entero asociado a la clave
    return mapaDatosEnteros.at(llave);
}

// Método para establecer un consolidado en el mapa de consolidados
// infoSexo: referencia a la información del sexo
// infoAno: ano del consolidado
// infoSemestre: semestre del consolidado
// nuevoConsolidado: puntero al consolidado que se va a almacenar
void ProgramaAcademico::setConsolidado(string& infoSexo, int infoAno , int infoSemestre, Consolidado *nuevoConsolidado)
{
    // Construye una clave combinando sexo, año y semestre, y la normaliza
    string claveConsolidado = infoSexo + string("-") + to_string(infoAno) + string("-") + to_string(infoSemestre);
    claveConsolidado = utilidadObj.minusculasSinEspacios(claveConsolidado);
    // Almacena el consolidado en el mapa de consolidados usando la clave generada
    consolidados[claveConsolidado] = nuevoConsolidado;
}

// Método para obtener un consolidado del mapa usando su clave
// claveConsolidado: referencia a la clave del consolidado a consultar
// return: puntero al consolidado asociado a la clave
Consolidado *ProgramaAcademico::getConsolidado(string& claveConsolidado)
{
    // Normaliza la clave usando el método minusculasSinEspacios
    string llave = utilidadObj.minusculasSinEspacios(claveConsolidado);
    // Retorna el consolidado asociado a la clave
    return consolidados[llave];
}

// Destructor de la clase ProgramaAcademico
// Libera la memoria asociada a los consolidados almacenados
ProgramaAcademico::~ProgramaAcademico()
{
    // Itera sobre el mapa de consolidados y libera la memoria de cada uno
    for (auto& pair : consolidados)
    {
        delete pair.second;
    }
    // Limpia el mapa de consolidados
    consolidados.clear();
}

// Método para buscar un consolidado en el mapa utilizando sexo, año y semestre
// infoSexo: referencia a la información del sexo
// infoAno: año del consolidado
// infoSemestre: semestre del consolidado
// return: puntero al consolidado si se encuentra
Consolidado* ProgramaAcademico::buscarConsolidado(string& infoSexo, int infoAno , int infoSemestre)
{
    // Construye una clave combinando sexo, año y semestre, y la normaliza
    string claveConsolidado = infoSexo + string("-") + to_string(infoAno) + string("-") + to_string(infoSemestre);
    claveConsolidado = utilidadObj.minusculasSinEspacios(claveConsolidado);
    // Verifica si la clave existe en el mapa, de lo contrario lanza una excepción
    if (consolidados.find(claveConsolidado) == consolidados.end())
    {
        // Genera un mensaje de error con la clave no encontrada
        string msg = string("Llave No Encontrada: '") + claveConsolidado + string("'");
        throw invalid_argument(msg);  // Lanza excepción si la clave no existe
    }
    // Retorna el consolidado asociado a la clave
    return consolidados[claveConsolidado];
}
