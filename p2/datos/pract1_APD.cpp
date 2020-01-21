/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: pract1.cpp                       *
 * Práctica 1 - APD                             *
 ************************************************/

#include <iostream>
#include <random>
#include <fstream>
#include <list>
#include <vector>
#include "Productos.hpp"
#include <cmath>
#include <ctime>
#include <cstring>

using namespace std;

/*
 * Clase para representar cada uno de los vertices del grafo
 */
class Vertice
{
public:
    int raiz;
    list<int> vertices;

    //Constructores:
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
 
/*
 * Clase para representar cada una de las aristas del grafo
 */
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

    const bool operator==(const Arista &a) const
    {
        return this->v1 == a.v1 && this->v2 == a.v2;
    }

    const bool operator!=(const Arista &a) const
    {
        return this->v1 != a.v1 || this->v2 != a.v2;
    }
};

/*
 *  Clase con la que se va a representar el grafo sobre el que 
 *  se va a aplicaer el algoritmo de Karger para encontrar el corte minimo
 */
class Grafo
{
public:
    int n_vertices; // Numero de vertices
    int n_aristas;  // Numero de aristas
    bool conexo;    // Es true sii el grafo no contiene ningun vertice desconectado
    list<Arista> aristas;     //Conjunto de aristas del grafo
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

/*
 * Obtiene del fichero de entrada asociado al flujo f el grafo
 * sobre el que se va a aplicar el algoritmo de Karger
 */
void cargar_grafo(ifstream &f, Grafo &grafo)
{
    int num_productos;
    int valorArista;
    //La primera linea especifica el numero de productos, por tanto, de vertices:
    f >> num_productos; 
    grafo.n_vertices = num_productos;
    grafo.vertices.resize(num_productos);
    //El grafo es conexo mientras no se encuentre ningun vertice desconectado
    grafo.conexo = true;
    //Recorre la matriz del fichero de entrada
    for (int i = 0; i < num_productos; i++)
    {
        bool desconectado = true;
        for (int j = 0; j < num_productos; j++)
        {
            f >> valorArista;
            desconectado = desconectado & (valorArista == 0);
            // condicion j<i para solo tener en cuenta el triangulo inferior de la matriz
            if (j < i && valorArista > 0) //Si es arista se añade al grafo
            {
                Arista a(i, j);
                //Añade la arista tántas veces como se ha comprado con el producto j (Karger extendido)
                for (int i = 0; i < valorArista; i++)
                {
                    grafo.aristas.push_back(a);
                }
                grafo.vertices[i] = Vertice(i);
                grafo.vertices[j] = Vertice(j);
            }
        }
        if (grafo.conexo && desconectado)
            grafo.conexo = false;
    }
    grafo.n_aristas = grafo.aristas.size();
}

/*
 * Junta los dos vértices de la arista a en el nodo de menor índice de a
 * y actualiza las aristas del grafo G que apuntaban a esos dos vértices.
 */
void merge(Grafo &G, const int index, const bool verbose)
{
    auto e = G.aristas.begin();
    std::advance(e, index);

    const int new_v = (*e).v1;
    const int v = (*e).v2;

    if (verbose)
        cout << "Contrayendo arista: <" << new_v << "---" << v << ">" << endl;

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

        //Self loop (borra las aristas reflexivas)
        if ((*it).v1 == (*it).v2)
        {
            it = G.aristas.erase(it);
            continue;
        }

        if ((*it).v2 < (*it).v1)
            swap((*it).v1, (*it).v2);

        it++;
    }
    G.n_aristas = G.aristas.size(); //Actualiza al nuevo numero de aristas
}

/*
 * Algoritmo de Karger: Elige aleatoriamente una de las aristas del grafo,
 * fusiona (merge) los dos vertices que la componen, y se eliminan las
 * aristas reflexivas que puedan aparecer. El proceso se repite hasta que
 * solo queden 2 vertices en el grafo
 */
Grafo karger(const Grafo &G, const bool verbose)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    Grafo _G(G.n_vertices, G.n_aristas, G.aristas, G.vertices);
    int i;
    while (_G.n_vertices > 2) //Mientras queden mas de 2 vertices en el grafo
    {
        std::uniform_int_distribution<int> dist(0, _G.n_aristas - 1);
        i = dist(mt); //Elige la arista aleatoriamente
        merge(_G, i, verbose); //Fusiona los dos vertices de la arista elegida
    }
    return _G;
}

/*
 * Dado que el algoritmo de Karger es probabilista (Monte Carlo), esta funcion
 * lo ejecuta n veces, quedandose siempre con el corte minimo de las ejecuciones.
 * De esta forma se reduce la probabilidad de error.
 */
Grafo rep_karger(const Grafo &G, const int n, const bool verbose)
{
    Grafo best_G;
    best_G.n_aristas = G.n_aristas;
    for (int i = 0; i < n; i++)
    {
        Grafo _G = karger(G, verbose);
        if (_G.n_aristas < best_G.n_aristas)
        {
            best_G = _G;
        }
    }
    return best_G;
}

/*
 * Aplica la contracción de aristas hasta que el garfo G tiene t vértices
 */
Grafo contract(const Grafo &G, const int t, const bool verbose)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    Grafo _G(G.n_vertices, G.n_aristas, G.aristas, G.vertices);
    while (_G.n_vertices > t)
    {
        // seleccionar arista aletoriamente
        std::uniform_int_distribution<int> dist(0, _G.n_aristas - 1);
        int i = dist(mt);
        merge(_G, i, verbose); //Fusiona los dos vertices de la arista elegida
    }
    return _G;
}

/*
 * Mejora del algoritmo de Karger, en vez de contraer hasta llegar a 2 vertices
 * contrae hasta llegar a un valor t calculado
 */
Grafo karger_stein(const Grafo &G, const bool verbose)
{
    if (G.n_vertices <= 6)
    {
        return karger(G, verbose); //En los casos pequeños devuelve karger normal
    }
    else
    {
        int t = ceil(1 + G.n_vertices / M_SQRT2); //Calculo de t
        Grafo G1 = contract(G, t, verbose);
        Grafo G2 = contract(G, t, verbose);
        //Se elige el minimo de las dos llamadas recursivas (con G1 y G2)
        return min(karger_stein(G1, verbose), karger_stein(G2, verbose));
    }
}

/*
 * Obtiene del fichero de entrada asociado al flujo f el listado
 * de todos los  productos con el que se va a trabajar, y los almacena 
 * en el vector productos
 */
void cargar_productos(ifstream &f, vector<Producto> &productos, const int num_productos)
{
    std::string nombre;
    int unidades;
    double precio;
    //int i = 0;
    //while (!f.eof())
    for (int i=0; i < num_productos; i++)
    {
        f >> nombre;   //nombre
        f >> precio;   //precio
        f >> unidades; //unidades disponibles
        //productos[i++] = Producto(nombre, unidades, precio);
        productos[i] = Producto(nombre, unidades, precio);
    }
}

/*
 * En caso de que el grafo no sea conexo, no hace falta aplicar el algoritmo,
 * por lo que calcular_desconectado calcula manualmente los dos grupos de 
 * vertices y los devuelve en las listas enlazadas v1 y v2. Introduce en v1
 * aquellos vertices que tengan al menos una arista con otro vertice, y en v2
 * los vertices desconectados del grafo.
 */
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

/*
 * Procedimiento que escribe por la salida estándar la solucion obtenida
 * por el algoritmo, indicando el valor del minimo corte ademas de qué productos 
 * deberan estar en Amazon y cuales en Amazonymas
 */
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
    string usage = "Uso: ./pract1 <fichero_datos> <fichero_productos> [-v] [-a {karger, karger-stein, karger-extendido}]";
    Grafo grafo;
    if (argc < 3)
    {
        cerr << "Número incorrecto de parámetros" << std::endl;
        cerr << usage << std::endl;
        exit(1);
    }

    string filenameDatos = argv[1];
    string filenameProductos = argv[2];
    int algortimo = 0; //karger normal (0), karger-stein (1), karger-extendido(2)
    bool verbose = false;

    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
            verbose = true;
        if (strcmp(argv[i], "-a") == 0)
        {
            if (i + 1 == argc)
            {
                cerr << "Debe introducir los parámetros correctamente" << std::endl;
                cerr << usage << std::endl;
                exit(1);
            }
            else
            {
                if (strcmp(argv[i + 1], "karger-stein") == 0)
                {
                    algortimo = 1;
                }
                if (strcmp(argv[i + 1], "karger-extendido") == 0)
                {
                    algortimo = 2;
                }
                i++;
            }
        }
    }

    ifstream f(filenameDatos);
    if (f.is_open())
    {
        ifstream fp(filenameProductos);
        if (fp.is_open())
        {
            cargar_grafo(f, grafo); //Obtiene el grafo a partir del fichero
            vector<Producto> productos(grafo.n_vertices);
            cargar_productos(fp, productos, grafo.n_vertices); //Obtiene el vector de productos
            Grafo _g;

            //Medir tiempo algoritmo
            struct timespec start, finish;
            clock_gettime(CLOCK_REALTIME, &start);

            if (grafo.conexo)
            {
                switch (algortimo)
                {
                case 1:
                    _g = karger_stein(grafo, verbose);
                    break;
                default:
                    _g = karger(grafo, verbose);
                    break;
                }
                int _v1 = _g.aristas.front().v1;
                int _v2 = _g.aristas.front().v2;
                list<int> v1 = _g.vertices[_v1].vertices;
                list<int> v2 = _g.vertices[_v2].vertices;
                mostrar_conjuntos(v1, v2, productos);
                cout << "Corte mínimo calculado: " << _g.n_aristas << endl;
            }
            else
            {
                //Calcular solución manualmente
                list<int> v1; //Conjunto de vértices 1 (conectados)
                list<int> v2; //Conjunto de vértices 2 (desconectados)
                calcular_desconectado(grafo, v1, v2);
                mostrar_conjuntos(v1, v2, productos);
                cout << "Corte mínimo calculado: 0 (grafo NO conexo)"   << endl;
            }

            // Obtener tiempo de ejecución
            clock_gettime(CLOCK_REALTIME, &finish);
            double elapsed = (finish.tv_sec - start.tv_sec);
            elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
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
