#ifndef GESTOR_JSON_H
#define GESTOR_JSON_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include "ProgramaAcademico.h"
#include "Consolidado.h"
#include "GestorArchivos.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class GestorJSON : public GestorArchivos
{
private:
    void escribirEtiquetasJson(json &etiquetasJson, string &strCodigoSNIES, string &strNombrePrograma, vector<vector<string>> &matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas);
    void escribirProgramaJson(json &jsonData, string &nombrePrograma, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual, string &strCodigoSNIES, vector<string> &vectorAtributosPrograma);
    void imprimirConsolidadosJson(json &jsonData, ProgramaAcademico *programaActual, vector<vector<string>> &matrizEtiquetas, vector<string> &vectorAtributosPrograma);
    void escribirConsolidadoJson(json &jsonData, Consolidado *consolidadoActual, vector<vector<string>> &matrizEtiquetas, string &llaveConsolidado);
public:
    GestorJSON() = default;
    ~GestorJSON() override = default;

    void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) override;
    void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) override;
    void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
};

#endif