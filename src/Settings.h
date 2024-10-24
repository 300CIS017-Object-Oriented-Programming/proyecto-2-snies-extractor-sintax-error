/**
 * @file Settings.h
 * @brief Este archivo de encabezado define la clase Settings, que contiene variables de ruta de archivo constantes estáticas.
 *
 * La clase Settings se utiliza para almacenar rutas de archivos como constantes estáticas. Estas rutas están destinadas para su uso
 * en toda la aplicación, permitiendo un acceso fácil a las ubicaciones de varios archivos de entrada.
 *
 * Las siguientes constantes estáticas están definidas:
 * - ADMITIDOS_FILE_PATH: Ruta al archivo de entrada "admitidos".
 * - MATRICULADOS_FILE_PATH: Ruta al archivo de entrada "matriculados".
 * - INSCRITOS_FILE_PATH: Ruta al archivo de entrada "inscritos".
 * - PROGRAMAS_FILTRAR_FILE_PATH: Ruta al archivo de entrada "programas filtrar".
 *
 * Nota: Las constantes estáticas deben definirse fuera de la clase para asignar memoria a ellas.
 *
 *
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
class Settings
{
public:
    // En la clase Settings, las variables ADMITIDOS_FILE_PATH, MATRICULADOS_FILE_PATH y ETC se declaran como static const std::string. Esto significa que son constantes de tipo string que pertenecen a la clase en lugar de a instancias individuales de la clase.
    inline static const std::string ADMITIDOS_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/admitidos";
    inline static const std::string GRADUADOS_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/graduados";
    inline static const std::string INSCRITOS_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/inscritos";
    inline static const std::string MATRICULADOS_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/matriculados";
    inline static const std::string PRIMER_CURSO_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/matriculadosPrimerSemestre";
    inline static const std::string PROGRAMAS_FILTRAR_FILE_PATH = "C:/SNIES_EXTRACTOR/inputs/programas.csv";
    inline static const std::string BASE_PATH = "C:/SNIES_EXTRACTOR/inputs/";
    inline static const std::string DELIMITADOR = ";";
    inline static const std::string ETIQUETAS_CONFIG_PATH = "C:/SNIES_EXTRACTOR/inputs/etiquetasAtributos.txt";
    inline static const std::string OUTPUTS_PATH = "C:/SNIES_EXTRACTOR/outputs/";

    // FIXME completar para el resto de constantes
};

// Declaración:
// Inicialización: La inicialización de estas variables se realiza fuera de la clase. Esto es necesario porque las variables estáticas deben ser definidas en un solo lugar en el archivo de implementación (o en el mismo archivo, pero fuera de la clase) para que el compilador sepa cuánto espacio reservar en memoria.
//const std::string Settings::BASE_PATH = "C:/SNIES_EXTRACTOR/inputs/";
//const std::string Settings::PROGRAMAS_FILTRAR_FILE_PATH = BASE_PATH + "programas.csv";
//const std::string Settings::ADMITIDOS_FILE_PATH = BASE_PATH + "admitidos";
//const std::string Settings::MATRICULADOS_FILE_PATH = BASE_PATH + "matriculados";
//const std::string Settings::INSCRITOS_FILE_PATH = BASE_PATH + "inscritos";
//const std::string Settings::DELIMITADOR = ";";
//const std::string Settings::ETIQUETAS_CONFIG_PATH = "etiquetasAtributos.txt";
//const std::string Settings::OUTPUTS_PATH = "C:/SNIES_EXTRACTOR/outputs/";
//const std::string Settings::PRIMER_CURSO_FILE_PATH = "matriculadosPrimerSemestre";
//const std::string Settings::GRADUADOS_FILE_PATH = "graduados";

#endif // SETTINGS_H