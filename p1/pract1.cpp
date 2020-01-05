#include <iostream>
#include <random>
#include <fstream>
#include <list>
#include <vector>
#include "Productos.hpp"
#include <cmath>
#include <ctime>

using namespace std;

class Vertice
{
public:
    int raiz;
    list<int> vertices;

    Vertice() { raiz = -1; }
    Vertice(const int v) : raiz(v) { vertices.push_back(v); }
    Vertice(const int v, list<int> _Vertices) : raiz(v), vertices(_Vertices) {}

    void union_vertices(Vertice &n)
    {
        raiz = n.raiz < raiz ? n.raiz : raiz;
        vertices.splice(vertices.end(), n.vertices);
    }

    const bool operator==(const Vertice &n) const
    {
        return this->raiz == n.raiz;
    }
};

// Estructura de datos para representar cada una de las aristas del grafo
class Arista
{
public:
    // Cada arista se representa por los 2 vertices (v1 < v2 siempre)
    int v1, v2;
    // El peso de la arista representara el numero de veces que los objetos v1 y v2
    // hayan sido comprados juntos, en caso de que los datos de entrada asi lo expresen
    // En caso de que los datos de entrada sean 0 o 1, el peso tendra el valor 1
    int peso;

    //Constructor, siempre se asigna a v1 el menor de los valores de los parámetros
    Arista(const int _v1, const int _v2, const int _peso)
    {
        peso = _peso;
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
        return peso < a.peso;
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
    int n_vertices; // Numero de vertices
    int n_aristas;  // Numero de aristas
    bool conexo;
    list<Arista> aristas;     // Grafo representado como un conjunto de aristas
    vector<Vertice> vertices; //Conjunto de vertices del grafo

    //Constructor
    Grafo() {}
    Grafo(const int n_v, const int n_a,
          list<Arista> a, vector<Vertice> v) : n_vertices(n_v), n_aristas(n_a), aristas(a), vertices(v) {}

    const bool operator<(const Grafo &G) const
    {
        return n_aristas < G.n_aristas;
    }
};

//Obtiene directamente del fichero el grafo correspondiente
// Leemos solo triangulo superior de la matriz
bool cargar_grafo(ifstream &f, Grafo &grafo)
{
    int num_productos;
    int valorArista;
    bool conexo;
    bool esMatrizBinaria = true; //Si encuenta uno o mas numeros >1, esMatrizBinaria=false

    f >> num_productos;
    grafo.n_vertices = num_productos;
    grafo.vertices.resize(num_productos);
    grafo.conexo = true;

    for (int i = 0; i < num_productos; i++)
    {
        bool desconectado = true;
        for (int j = 0; j < num_productos; j++)
        {
            f >> valorArista;
            desconectado = desconectado & (valorArista == 0);
            if (j < i && valorArista > 0) //Si es arista se añade al grafo
            {
                if (valorArista != 1)
                {
                    esMatrizBinaria = false;
                }
                grafo.aristas.push_back(Arista(i, j, valorArista));
                grafo.vertices[i] = Vertice(i);
                grafo.vertices[j] = Vertice(j);
            }
        }
        if (grafo.conexo && desconectado)
            grafo.conexo = false;
    }
    grafo.n_aristas = grafo.aristas.size();
    if (esMatrizBinaria)
    {
        grafo.aristas.sort();
    }

    return esMatrizBinaria;
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
    G.vertices[new_v].union_vertices(G.vertices[v]);
    // Borra la arista a del grafo G
    G.aristas.erase(e);
    auto it = G.aristas.begin();
    while (it != G.aristas.end())
    {
        (*it).v1 = (*it).v1 == v ? new_v : (*it).v1;
        (*it).v2 = (*it).v2 == v ? new_v : (*it).v2;

        //Self loop
        if ((*it).v1 == (*it).v2)
        {
            it = G.aristas.erase(it);
            continue;
        }

        if ((*it).v2 < (*it).v1)
            swap((*it).v1, (*it).v2);

        it++;
    }
    G.n_aristas = G.aristas.size();
}

Grafo karger(const Grafo &G, const bool esMatrizBinaria)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    Grafo _G(G.n_vertices, G.n_aristas, G.aristas, G.vertices);
    int i;
    while (_G.n_vertices > 2)
    {
        // seleccionar arista aletoriamente
        if (esMatrizBinaria)
        {
            std::uniform_int_distribution<int> dist(0, min(_G.n_aristas - 1, 5)); //¿Como favorecemos que se elijan las aristas mas pesadas?
            i = dist(mt);                                                         //De momento esta puesto que coja una de las 5 primeras, ya que la lista esta ordenada, pero no se
        }
        else
        {
            std::uniform_int_distribution<int> dist(0, _G.n_aristas - 1);
            i = dist(mt);
        }
        merge(_G, i);
    }
    return _G;
}

Grafo rep_karger(const Grafo &G, const bool esMatrizBinaria, const int n)
{
    Grafo best_G;
    best_G.n_aristas = G.n_aristas;
    for (int i = 0; i < n; i++)
    {
        Grafo _G = karger(G, esMatrizBinaria);
        if (_G.n_aristas < best_G.n_aristas)
        {
            best_G = _G;
        }
    }
    return best_G;
}

/**
 * Aplica la contracción de aristas hasta que el garfo G tiene t vértices
 */
Grafo contract(const Grafo &G, const int t)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    Grafo _G(G.n_vertices, G.n_aristas, G.aristas, G.vertices);
    while (_G.n_vertices > t)
    {
        // seleccionar arista aletoriamente
        std::uniform_int_distribution<int> dist(0, _G.n_aristas - 1);
        int i = dist(mt);
        merge(_G, i);
    }
    return _G;
}

Grafo karger_stein(const Grafo &G, const bool esMatrizBinaria)
{
    if (G.n_vertices <= 6)
    {
        return karger(G, esMatrizBinaria);
    }
    else
    {
        int t = ceil(1 + G.n_vertices / M_SQRT2);
        Grafo G1 = contract(G, t);
        Grafo G2 = contract(G, t);
        return min(karger_stein(G1, esMatrizBinaria), karger_stein(G2, esMatrizBinaria));
    }
}

void cargar_productos(ifstream &f, vector<Producto> &productos)
{
    std::string nombre;
    int unidades;
    double precio;
    int i = 0;
    while (!f.eof())
    {
        f >> nombre;   //nombre
        f >> precio;   //precio
        f >> unidades; //unidades disponibles
        productos[i++] = Producto(nombre, unidades, precio);
    }
}

void calcular_desconectado(const Grafo &G, list<int> &v1, list<int> &v2)
{
    for (int i = 0; i < G.vertices.size(); i++)
    {
        if (G.vertices[i].raiz != -1)
        {
            //Vértice conectado
            v1.push_back(i);
        }
        else
        {
            v2.push_back(i);
        }
    }
}

void mostrar_conjuntos(list<int> v1, list<int> v2, vector<Producto> &productos)
{
    cout << "Conjunto productos Amazon:    {  ";
    for (auto &&i : v1)
    {
        cout << productos[i].get_nombre() << "  ";
    }
    cout << "}\n";
    cout << "Conjunto productos Amazonymas: {  ";
    for (auto &&i : v2)
    {
        cout << productos[i].get_nombre() << " ";
    }
    cout << "}\n";
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
        ifstream fp(filenameProductos);
        if (fp.is_open())
        {
            bool esMatrizBinaria = cargar_grafo(f, grafo);
            vector<Producto> productos(grafo.n_vertices);
            cargar_productos(fp, productos);
            Grafo _g;

            //Medir tiempo algoritmo
            struct timespec start, finish;
            clock_gettime(CLOCK_REALTIME, &start);

            if (grafo.conexo)
            {
                //_g = karger_stein(grafo);
                _g = karger(grafo, esMatrizBinaria);
                int _v1 = _g.aristas.front().v1;
                int _v2 = _g.aristas.front().v2;
                list<int> v1 = _g.vertices[_v1].vertices;
                list<int> v2 = _g.vertices[_v2].vertices;
                mostrar_conjuntos(v1, v2, productos);
            }
            else
            {
                //Calcular solución manualmente
                list<int> v1; //Conjunto de vértices 1 (conectados)
                list<int> v2; //Conjunto de vértices 2 (desconectados)
                calcular_desconectado(grafo, v1, v2);
                mostrar_conjuntos(v1, v2, productos);
            }

            // Obtener tiempo de ejecución
            clock_gettime(CLOCK_REALTIME, &finish);
            double elapsed = (finish.tv_sec - start.tv_sec);
            elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
            cout << "Corte mínimo calculado: " << _g.n_aristas << endl;
            std::cout << "Tiempo transcurrido del algoritmo: " << elapsed << " segundos." << std::endl;
            fp.close();
        }
        else
        {
            cerr << "No se ha podido abrir el fichero de productos\n";
        }
        f.close();
    }
    else
    {
        cerr << "No se ha podido abrir el fichero de datos\n";
    }
    return 0;
}
