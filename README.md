[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QApazJy0)

## SNIES Extractor
Gracias al grupo que me presetó el código fuente de este proyecto

## Presentación general
En este proyecto se utilizarán 5 clases para cumplir el propósito de extraer la información del Sistema Nacional de Información de Educación Superior. Para el usuario, este tendrá una interfaz por terminal que le indicará los pasos a seguir y la información que necesita digitar.

## Cumplimiento de Requerimientos

1. **Solicitar rango de años a analizar**  
   En el menú, se le pregunta al usuario cuál es el primer año de búsqueda, tomando en cuenta que los rangos de años siempre son consecutivos, el segundo año de búsqueda se calcula a partir del primero.

2. **Lectura de archivos .csv correspondientes**  
   A partir de los años ingresados por el usuario, el controlador (clase `SNIESController`) leerá la información de los archivos apropiados uno por uno.

3. **Calcular admitidos, inscritos, graduados, matriculados y matriculados de primer semestre por programa y por año**  
   El SNIES-Extractor, mediante su controlador, calculará estos datos por cada programa y año a partir de los datos en bruto que extrae de los archivos .csv apropiados.

4. **Generación de Archivos de Salida**  
   Al terminar de extraer la información y procesarla, el SNIES-Extractor generará un archivo de resultados en la siguiente carpeta que ya debería estar parametrizada en el disco duro:  
   `C:\SNIES_EXTRACTOR\outputs\resultados.csv`.

5. **Visualización de Datos**  
   El SNIES-Extractor muestra al usuario el consolidado de estudiantes por año en programas presenciales o virtuales, la diferencia porcentual anual entre la cantidad de nuevos matriculados durante los años de búsqueda por programa y la lista de programas sin nuevos matriculados en 3 semestres consecutivos. Además, permitirá al usuario exportar esta información en un archivo ubicado en la carpeta ya parametrizada:  
   `C:\SNIES_EXTRACTOR\outputs\extras.csv`.

6. **Filtrado de Programas**  
   Se le pregunta al usuario si desea hacer una búsqueda con 2 filtros entre los programas solicitados: una palabra clave para buscar según los nombres de programas y el nivel de formación de los programas.

7. **Validación de Datos**  
   El SNIES-Extractor es un programa de C++ robusto que puede detectar errores a la hora de recibir información fuera de rangos esperados, tanto de parte del usuario como de la lectura de los archivos .csv.

Más abajo podrán encontrar el diagrama UML de las clases.


# Diagrama Mermaid

```mermaid
classDiagram
direction BT
    class ProgramaAcademico {
      -map<string, string> mapaDatosString
      -map<string, int> mapaDatosEnteros;
      -map<string, Consolidado *> consolidados;
      -Utilidad utilidadObj;
      +ProgramaAcademico();
      +void agregarElementoTipoString(string &, string &);
      +void agregarElementoTipoInt(string &, int);
      +void setConsolidado(string &, int, int, Consolidado *);
      +string consultarDatoString(string &);
      +int consultarDatoInt(string &);
      +Consolidado *buscarConsolidado(string &, int, int);
      +Consolidado *getConsolidado(string &);
      +~ProgramaAcademico();
    }
    class Consolidado {
      -map<string, int> datosIntConsolidados;
      -map<string, string> datosStringConsolidados;
      -Utilidad utilidadObj;
      +Consolidado();
      +void agregarDatoInt(string &, int);
      +void agregarDatoString(string &, string &);
      +int obtenerDatoInt(string &);
      +string obtenerDatoString(string &);
    }
    class GestorArchivos {
      #Utilidad utilidadObj;
      +GestorArchivos() = default;
      +virtual ~GestorArchivos() = default;
      +vector<int> leerProgramas(string &ruta);
      +vector<vector<string>> leerArchivo(string &rutaBase, vector<string> &etiquetasColumnas, vector<int> &codigosSnies);
      +virtual void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) = 0;
      +virtual void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) = 0;
      +virtual void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) = 0;
    }


    class GestorCsv {
      +GestorCsv() = default;
      +~GestorCsv() override = default;
      +void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
    }
    class GestorTxt {
      +GestorTxt() = default;
      +~GestorTxt() override = default;
      +void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
    }
    class GestorJSON {
      +GestorJSON() = default;
      +~GestorJSON() override = default;
      +void crearArchivo(string &ruta, map<int, ProgramaAcademico *> &mapadeProgramasAcademicos, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoBuscados(string &ruta, list<ProgramaAcademico *> &programasBuscados, vector<vector<string>> &matrizEtiquetas) override;
      +void crearArchivoExtra(string &ruta, vector<vector<string>> datosAImprimir) override;
    }
    class SNIESController {
         -map<int, ProgramaAcademico *> programasAcademicos;
         -vector<GestorArchivos *> gestoresArchivos;
         -vector<vector<string>> matrizEtiquetas;
         -Utilidad utilidadObj;
         +SNIESController();
         +~SNIESController();
         +void procesarDatos(int anio1, int anio2);
         +void calcularDatosExtra(bool exportarArchivo);
         +void buscarProgramas(bool exportarArchivo, string& palabraClave, int idComparacion);
    }
    class View {
        -SNIESController controlador;
        -Utilidad utilidadObj;
        +View() = default;
        +~View();
        +bool mostrarPantallaBienvenido();
        +void mostrarDatosExtra();
        +void buscarPorPalabraClaveYFormacion();
        +void salir();
    }
    class Main {
        + int main()
    }

ProgramaAcademico o-- Consolidado : tiene varios
View <.. Main : usa
View --> SNIESController : tiene un
SNIESController --> GestorArchivos: tiene un
SNIESController o-- Consolidado
Consolidado <.. GestorArchivos: usa
```
