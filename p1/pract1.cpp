#include <iostream>
#include <random>
#include <fstream>
#include <list>
#include <map>

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
    int n_vertices;       // Numero de vertices
    int n_aristas;        // Numero de aristas
    list<Arista> aristas; // Grafo representado como un conjunto de aristas

    //Constructor
    Grafo() {}
    Grafo(const int n_v, const int n_a, list<Arista> a) : n_vertices(n_v), n_aristas(n_a), aristas(a) {}
};

//Obtiene directamente del fichero el grafo correspondiente
// Leemos solo triangulo superior de la matriz
void cargar_grafo(ifstream &f, Grafo &grafo, int num_productos)
{
    grafo.n_vertices = num_productos;

    bool esArista;
    for (int i = 0; i < num_productos; i++)
    {
        for (int j = 0; j < num_productos; j++)
        {
            f >> esArista;
            if (j < i && esArista)
            {
                grafo.aristas.push_back(Arista(i, j));
            }
        }
    }
    grafo.n_aristas = grafo.aristas.size();
}

/**
 * Junta los dos vértices de la arista a en el nodo de menor índice de a
 * y actualiza las aristas del grafo G que apuntaban a esos dos vértices.
 */
void merge(Grafo &G, const int index)
{
    auto e = G.aristas.begin();
    std::advance(e, index);

    const int new_v = (*e).v1;
    const int v = (*e).v2;

    // Actualiza el número de vértices del grafo G
    G.n_vertices--;
    // Borra la arista a del grafo G
    G.aristas.erase(e);

    for (auto it = G.aristas.begin(); it != G.aristas.end(); it++)
    {
        //Self loop
        if ((*it).v1 == (*it).v2)
        {
            G.aristas.erase(it);
            continue;
        }
        (*it).v1 = (*it).v1 == v ? new_v : (*it).v1;
        (*it).v2 = (*it).v2 == v ? new_v : (*it).v2;

        if ((*it).v2 < (*it).v1)
        {
            cout << (*it).v1 << "  " << (*it).v2 << endl;
            swap((*it).v1, (*it).v2);
            cout << (*it).v1 << "  " << (*it).v2 << endl;
        }
    }
    G.n_aristas = G.aristas.size();
}

void cargar_productos(ifstream &f, map<int, string> &productos, int num_productos)
{
    int clave;
    string valor, aux;
    for (int i = 0; i < num_productos; i++)
    {
        f >> clave; //ID del producto
        f >> aux;   //nombre
        valor = aux;
        f >> aux; //precio
        valor = valor + "-" + aux;
        f >> aux; //unidades disponibles
        valor = valor + "-" + aux;
        productos.insert(pair<int, string>(clave, valor));
    }
}

int main(int argc, char const *argv[])
{
    string usage = "Uso: ./pract1 <fichero_datos> <fichero_productos>";
    Grafo grafo;
    if (argc != 3)
    {
        cerr << "Número incorrecto de parámetros" << std::endl;
        cerr << usage << std::endl;
        exit(1);
    }

    string filenameDatos = argv[1];
    string filenameProductos = argv[2];
    ifstream f(filenameDatos);
    if (f.is_open())
    {
        //cargar_grafo(f, grafo);
        grafo.n_vertices = 4;
        grafo.n_aristas = 5;
        grafo.aristas.push_back(Arista(0, 1));
        grafo.aristas.push_back(Arista(0, 2));
        grafo.aristas.push_back(Arista(0, 3));
        grafo.aristas.push_back(Arista(1, 3));
        grafo.aristas.push_back(Arista(2, 3));
        for (auto &&i : grafo.aristas)
        {
            cout << i.v1 << "  " << i.v2 << endl;
        }
        cout << endl;
        merge(grafo, 0);
        for (auto &&i : grafo.aristas)
        {
            cout << i.v1 << "  " << i.v2 << endl;
        }
        f.close();
    }
    else
    {
        cerr << "No se ha podido abrir el fichero de datos\n";
    }
    return 0;
}
