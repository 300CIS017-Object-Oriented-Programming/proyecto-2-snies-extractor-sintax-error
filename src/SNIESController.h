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
#include "GestorCsv.h"

using namespace std;

class SNIESController
{

private:
    map<int, ProgramaAcademico *> programasAcademicos;
    vector<GestorArchivos*> gestoresArchivos;
    vector<vector<string>> matrizEtiquetas;
    /*
     Matriz Etiquetas[0] -> Etiquetas Atributos de Tipo String Programa Academico
     Matriz Etiquetas[1] -> Etiquetas Atributos de Tipo Int Programa Academico
     Matriz Etiquetas[2] -> Etiquetas Atributos de Tipo String Consolidado
     Matriz Etiquetas[3] -> Etiquetas Atributos de Tipo Int Consolidado
     Matriz Etiquetas[4] -> Sexos Disponibles
     Matriz Etiquetas[5] -> Años a considerar (Se obtienen de los parametros de procesarDatosCsv)
    */

public:
    SNIESController() = default;
    SNIESController(string &, string &, string &, string &, string &, string &, string &);
    ~SNIESController();
    void procesarDatos(vector<string> anos);
    void calcularDatosExtra(bool exportarArchivo);
    void buscarProgramas(bool exportarArchivo, string& palabraClave, int idComparacion);
};

#endif