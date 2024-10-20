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
public:
    GestorJSON() = default;
    void escribirEtiquetasJson(json& etiquetasJson, string& strCodigoSNIES, string& strNombrePrograma, vector<vector<string>>& matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas);

    //funciones que agregue en base a lo hecho en GestorTxt

    bool crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas) override;
    bool crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas) override;
    bool crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
};

#endif