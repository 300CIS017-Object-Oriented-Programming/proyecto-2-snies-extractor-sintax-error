#ifndef GESTOR_CSV_H
#define GESTOR_CSV_H

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
#include "Utilidad.h"

using namespace std;

class GestorCsv : public GestorArchivos
{
private:
    // Metodos privados auxiliares
    void escribirEtiquetas(string &strCodigoSNIES, string &strNombrePrograma, string &fila, string &delimitador, vector<vector<string>> &matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas);
    void escribirPrograma(string &strCodigoSNIES, string &strNombrePrograma, string &fila, string &delimitador, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual);
    void imprimirConsolidados(string &fila, ofstream &archivoResultados, string &delimitador, vector<vector<string>> &matrizEtiquetas, ProgramaAcademico *programaActual);
    void escribirConsolidado(string &fila, string &delimitador, Consolidado *consolidadoActual, vector<vector<string>> &matrizEtiquetas);

public:
    GestorCsv() = default;
    ~GestorCsv() override = default;
    void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) override;
    void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) override;
    void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
};

#endif