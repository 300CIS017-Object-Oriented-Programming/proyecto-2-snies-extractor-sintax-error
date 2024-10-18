#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include "ProgramaAcademico.h"
#include "Consolidado.h"

using namespace std;

class GestorArchivos
{
public:
    GestorArchivos() = default;
    virtual ~GestorArchivos();
    virtual vector<int> leerProgramas(string &ruta) = 0;
    virtual vector<vector<string>> leerArchivo(string &rutaBase, vector<string> &etiquetasColumnas, vector<int> &codigosSnies) = 0;

    virtual bool crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<string> etiquetasProgramaString, vector<string> etiquetasProgramaInt, vector<string> etiquetasConsolidadoString, vector<string> etiquetasConsolidadoInt) = 0;
    virtual bool crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<string> etiquetasProgramaString, vector<string> etiquetasProgramaInt, vector<string> etiquetasConsolidadoString, vector<string> etiquetasConsolidadoInt) = 0;
    virtual bool crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) = 0;
};

#endif
