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
#include "Utilidad.h"
#include "Settings.h"

using namespace std;

class GestorArchivos
{
protected:
    Utilidad utilidadObj;

public:
    GestorArchivos() = default;
    virtual ~GestorArchivos();
    vector<int> leerProgramas(string &ruta);
    vector<vector<string>> leerArchivo(string &rutaBase, vector<string> &etiquetasColumnas, vector<int> &codigosSnies);

    virtual bool crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas) = 0;
    virtual bool crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas) = 0;
    virtual bool crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) = 0;
};

#endif
