#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

// Estructura de datos para representar cada una de las aristas del grafo
struct Arista{
    // Cada arista se representa por los 2 vertices entre los que se encuentra:
    int v1, v2; 
};



// Estructura de datos para representar el grafo sobre el que aplicamos Karger
struct Grafo{
    int V;  // Numero de vertices
    int A;  // Numero de aristas
    vector<Arista> aristas; // Grafo representado como un vector de aristas
};

Grafo newGrafo(){
    Grafo g;
    g.V=0;
    g.A=0;
    //g.aristas= new ;
    return g;
}

void 

//Obtiene directamente del fichero el grafo correspondiente
// Leemos solo triangulo superior de la matriz
Grafo grafoCompras(ifstream &f, const int numProductos){

    Grafo g = newGrafo();

    int ultimaColumna=1;
    int esArista=0;
    for (int i=1; i <= numProductos; i++){
        for (int j=1; j <= numProductos; j++){
            f >> esArista;
            if (j > ultimaColumna && esArista==1 && i!=j){  //La ultima comprobacion por si acaso
                
            }
        }
        ultimaColumna++;
    }
    


}



int main(int argc, char const *argv[]){
    string usage = "Usage: Path_tracer -n ppp";
    if (argc != 2){
        cerr << "Incorrect number of parameters" << std::endl;
        cerr << usage << std::endl;
        exit(1);
    }
    else{
        string filename = argv[1];
        ifstream f(filename);
        if (f.is_open()){
            int numProductos;
            f >> numProductos;
            cout << numProductos << endl;

            Grafo g=grafoCompras(f,numProductos);
            f.close();
        }
    }
    return 0;
}