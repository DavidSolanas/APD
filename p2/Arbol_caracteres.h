/**
 * Fichero Arbol_caracteres.h
 * Práctica 1 Algoritmia Básica
 * Autores: Diego Martínez Baselga      735969
 *          David Solanas Sanz          738630
 */
#include <iostream>
#include <queue>
#include <bitset>

using namespace std;

//Árbol de Huffman
class Arbol_caracteres
{
private:
    // byte correspondiente con el nodo del árbol (indefinido si no es hoja)
    unsigned char caracter;
    // frecuencia de aparición de los caracteres del árbol
    unsigned int frecuencia;
    // hijo derecho
    Arbol_caracteres* der;
    // hijo izquierdo
    Arbol_caracteres* izq;
public:
    /**
     * Crea el árbol correspondiente al byte a con frecuencia f
     */
    Arbol_caracteres(unsigned char a, unsigned int f);

    /**
     * Crea el árbol correspondiente a la fusión del árbol i con el d.
     * i pasa a ser el hijo izquierdo y d el derecho
     */
    Arbol_caracteres(Arbol_caracteres* i, Arbol_caracteres* d);

    /**
     * Devuelve el árbol de caracteres formado por un árbol con hijo i.
     * Utilizado en ficheros con solo un byte o bytes iguales
     */
    Arbol_caracteres(Arbol_caracteres* i);

    /**
     * Libera el espacio del arbol de caracteres
     */
    ~Arbol_caracteres();

    /**
     * Devuelve la suma de frecuencias de aparición de los caracteres 
     * pertenecientes al arbol
     */
    unsigned int getFrecuencia();

    /**
     * Devuelve el byte correspondiente a la raíz del árbol (indefinido si no es hoja)
     */
    unsigned char getCaracter();

    /**
     * Devuelve el hijo izquierdo de la raíz del árbol
     */
    Arbol_caracteres* getIzq();

    /**
     * Devuelve el hijo derecho de la raíz del árbol
     */
    Arbol_caracteres* getDer();

    /**
     * Asigna c como byte correspondiente con la raíz del árbol
     */
    void setCaracter (unsigned char c);

    /**
     * Asigna f como frecuencia correspondiente con la raíz del árbol
     */
    void setFrecuencia (unsigned int f);

    /**
     * Asigna d como hijo derecho de la raíz del árbol
     */
    void setDer (Arbol_caracteres* d);

    /**
     * Asigna i como hijo izquierdo de la raíz del árbol
     */
    void setIzq (Arbol_caracteres* i);

    /**
     * Escribe en f una estructura que permite recuperar el árbol.
     * Recorre el árbol en anchura de izquierda a derecha, empezando por
     * la raíz descendiendo por niveles. De cada nodo, escribe el caracter
     * al que corresponde y si es un nodo hoja
     */
    unsigned int escribir_arbol_fichero (ofstream& f);
};

Arbol_caracteres::Arbol_caracteres(unsigned char a, unsigned int f)
{
    der=nullptr;
    izq=nullptr;
    caracter=a;
    frecuencia=f;
}

Arbol_caracteres::Arbol_caracteres(Arbol_caracteres* i, Arbol_caracteres* d){
    der=d;
    izq=i;
    caracter=0;
    frecuencia= d->frecuencia + i->frecuencia;
}

Arbol_caracteres::Arbol_caracteres(Arbol_caracteres* i){
    der=nullptr;
    izq=i;
    caracter=0;
    frecuencia= i->frecuencia;
}

Arbol_caracteres::~Arbol_caracteres()
{
    if (izq!=nullptr){
        delete(izq);
        delete(der);
    }
}

unsigned int Arbol_caracteres:: getFrecuencia(){
    return frecuencia;
}

unsigned char Arbol_caracteres:: getCaracter(){
    return caracter;
}

Arbol_caracteres* Arbol_caracteres::getIzq(){
    return izq;
}

Arbol_caracteres* Arbol_caracteres::getDer(){
    return der;
}

void Arbol_caracteres::setCaracter (unsigned char c){
    this->caracter=c;
}

void Arbol_caracteres::setFrecuencia (unsigned int f){
    this->frecuencia=f;
}

void Arbol_caracteres::setDer (Arbol_caracteres* d){
    this->der=d;
}
    
void Arbol_caracteres::setIzq (Arbol_caracteres* i){
    this->izq=i;
}

unsigned int Arbol_caracteres::escribir_arbol_fichero (ofstream& f){
    queue<Arbol_caracteres*>* cola = new queue<Arbol_caracteres*>();
    cola->push(this);
    unsigned int i = 0;
    while(!cola->empty()){
        Arbol_caracteres* a = cola->front();
        cola->pop();
        unsigned char caracter = a->getCaracter();
        bool esHoja = a->izq == nullptr;
        f.write(reinterpret_cast<char*>(&caracter), sizeof(unsigned char));
        f.write(reinterpret_cast<char*>(&esHoja), sizeof(bool));
        if (a->izq != nullptr){
            cola->push(a->getIzq());
        }
        if (a->der != nullptr){
            cola->push(a->getDer());
        }
        i++;
    }
    return i;
}