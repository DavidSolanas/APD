#include <iostream>
#include <random>
#include <fstream>
#include <set>

using namespace std;

// Estructura de datos para representar cada una de las aristas del grafo
class Arista
{
public:
    // Cada arista se representa por los 2 vertices (v1 < v2 siempre)
    int v1, v2;

    //Constructor, siempre se asigna a v1 el menor de los valores de los parámetros
    Arista(const int _v1, const int _v2)
    {
        if (_v1 < _v2)
        {
            v1 = _v1;
            v2 = _v2;
        }
        else
        {
            v1 = _v2;
            v2 = _v1;
        }
    }
    const bool operator<(const Arista &a) const
    {
        int _a = v2 - v1;
        int _b = a.v2 - a.v1;
        return v1 < a.v1 || v2 < a.v2 || _a < _b;
    }

    const bool operator==(const Arista &a) const
    {
        return this->v1 == a.v1 && this->v2 == a.v2;
    }

    const bool operator!=(const Arista &a) const
    {
        return this->v1 != a.v1 || this->v2 != a.v2;
    }
};



// Estructura de datos para representar el grafo sobre el que aplicamos Karger
class Grafo
{
public:
    int n_vertices;      // Numero de vertices
    int n_aristas;       // Numero de aristas
    set<Arista> aristas; // Grafo representado como un conjunto de aristas

    //Constructor
    Grafo() {}
    Grafo(const int n_v, const int n_a, set<Arista> a) : n_vertices(n_v), n_aristas(n_a), aristas(a) {}
};


//Obtiene directamente del fichero el grafo correspondiente
// Leemos solo triangulo superior de la matriz
void cargar_grafo(ifstream &f, Grafo &grafo){
    int num_productos;
    f >> num_productos;
    grafo.n_vertices = num_productos;

    int ultimaColumna=1; //ultimaColumna in [1..num_productos] para descartar el triangulo inferior
    int esArista=0;
    for (int i=1; i <= num_productos; i++){
        for (int j=1; j <= num_productos; j++){
            f >> esArista;
            if (j > ultimaColumna && esArista==1 && i!=j){  //La ultima comprobacion por si acaso
                grafo.aristas.insert(Arista(i, j));
            }
        }
        ultimaColumna++;
    }
    grafo.n_aristas = grafo.aristas.size();
}



int main(int argc, char const *argv[])
{
    string usage = "Uso: pract1 <fichero_datos>";
    Grafo grafo;
    if (argc != 2){
        cerr << "Número incorrecto de parámetros" << std::endl;
        cerr << usage << std::endl;
        exit(1);
    }

    string filename = argv[1];
    ifstream f(filename);
    if (f.is_open())
    {
        cargar_grafo(f, grafo);


        f.close();
    }
    return 0;
}
