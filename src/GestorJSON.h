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

using namespace std;

class GestorJSON : public GestorArchivos
{
public:
    GestorJSON() = default;
    vector<int> leerProgramas(string &ruta) override;
    vector<vector<string>> leerArchivo(string &rutaBase, vector<string> &etiquetasColumnas, vector<int> &codigosSnies) override;

    bool crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<string> etiquetasColumnas) override;
    bool crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<string> etiquetasColumnas) override;
    bool crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
};

#endif