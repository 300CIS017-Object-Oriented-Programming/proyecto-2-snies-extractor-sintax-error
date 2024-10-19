#ifndef GESTOR_TXT_H
#define GESTOR_TXT_H

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

class GestorTxt : public GestorArchivos
{
private:
    //Metodos privados auxiliares
    void escribirEtiquetas(string& strCodigoSNIES, string& strNombrePrograma, string& fila, string& delimitador, vector<vector<string>>& matrizEtiquetas, int minPosEtiquetas, int maxPosEtiquetas);
    void escribirPrograma(string& strCodigoSNIES, string& strNombrePrograma, string& fila, string& delimitador , vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual);
    void imprimirConsolidados(string& fila, ofstream& archivoResultados, string& delimitador, vector<vector<string>>& matrizEtiquetas, ProgramaAcademico* programaActual);
public:
    GestorTxt() = default;

    bool crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>>& matrizEtiquetas) override;
    bool crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>>& matrizEtiquetas) override;
    bool crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
};

#endif