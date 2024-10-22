#ifndef SNIES_CONTROLLER_H
#define SNIES_CONTROLLER_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <list>
#include "ProgramaAcademico.h"
#include "Consolidado.h"
#include "GestorArchivos.h"
#include "GestorCsv.h"
#include "GestorTxt.h"
#include "GestorJSON.h"

using namespace std;

class SNIESController
{

private:
    map<int, ProgramaAcademico *> programasAcademicos;
    vector<GestorArchivos *> gestoresArchivos;
    vector<vector<string>> matrizEtiquetas;
    /*
     Matriz Etiquetas[0] -> Etiquetas Atributos de Tipo String Programa Academico
     Matriz Etiquetas[1] -> Etiquetas Atributos de Tipo Int Programa Academico
     Matriz Etiquetas[2] -> Etiquetas Atributos de Tipo String Consolidado
     Matriz Etiquetas[3] -> Etiquetas Atributos de Tipo Int Consolidado
     Matriz Etiquetas[4] -> Sexos Disponibles
     Matriz Etiquetas[5] -> Años a considerar (Se obtienen de los parametros de procesarDatosCsv)
    */
    Utilidad utilidadObj;

    void inicializarGestores();
    void inicializarEtiquetas();
    void leerArchivoConfiguracion(vector<vector<string>>& matriz);
    void seleccionarEtiquetas(int filaMin, int filaMax, vector<string>& etiquetasParaLeer);
    void crearProgramas(vector<vector<string>>& matrizArchivo, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon);
    int verificarFilaEtiqueta(string& etiquetaCorrespondiente, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon);
    void asignarConsolidados(vector<vector<string>>& matrizArchivo, string& etiquetaClave, int fAtrStrProg, int fAtrIntProg, int fAtrStrCon, int fAtrIntCon);
    vector<string> generarAniosBusqueda(int anio1, int anio2);

public:
    SNIESController();
    ~SNIESController();
    void procesarDatos(int anio1, int anio2);
    void calcularDatosExtra(bool exportarArchivo);
    void buscarProgramas(bool exportarArchivo, string& palabraClave, int idComparacion);
};

#endif