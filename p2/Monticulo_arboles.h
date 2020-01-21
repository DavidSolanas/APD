/**
 * Fichero Monticulo_arboles.h
 * Práctica 1 Algoritmia Básica
 * Autores: Diego Martínez Baselga      735969
 *          David Solanas Sanz          738630
 */
#include "Arbol_caracteres.h"

//Monticulo de árboles de Huffman
class Monticulo_arboles
{
private:
    //Posicion del vector en donde está el último puntero a árbol
    int ultimo;
    //Vector que almacena punteros a árboles desde las posiciones
    //0 a último. Está ordenado como un montículo por la frecuencia de
    //aparición del árbol
    Arbol_caracteres* vector[256];
    
public:
    /**
     * Crea un montículo vacío
     */
    Monticulo_arboles();

    /**
     * Añade el árbol a al montículo y lo reordena
     */
    void insertar(Arbol_caracteres* a);

    /**
     * Devuelve el elemento de la cima del montículo y lo devuelve
     */
    Arbol_caracteres* borrar_primero();

    /**
     * Devuelve si el montículo contiene un solo elemento
     */
    bool terminado();

    /**
     * Devuelve si el montículo está vacío
     */
    bool vacio();
};

Monticulo_arboles::Monticulo_arboles()
{
    ultimo = -1;
    for (int i = 0; i<256; i++){
        vector[i]=nullptr;
    }
}


void Monticulo_arboles:: insertar(Arbol_caracteres* a){
    int i = ++ultimo;
    while (i!=0 && vector [(i-1)/2] -> getFrecuencia() > a -> getFrecuencia()){
        vector[i] = vector [(i-1)/2];
        i=(i-1)/2;
    }
    vector[i] = a;
}

Arbol_caracteres* Monticulo_arboles::borrar_primero(){
    if (ultimo == -1){
        return nullptr;
    }
    else{
        Arbol_caracteres* a = vector[0];
        Arbol_caracteres* b = vector[ultimo--];
        int i = 0;
        bool finalizar = false;
        //Se toma el último árbol de vector de árboles y se comprueba, empezando en la
        //raíz, dónde se puede colocar permutándolo cada vez con un hijo de la posición
        //a comprobar correspondiente.  
        while (ultimo >= 2*i +1 && !finalizar)
        {
            //Su hijo izquierdo es el último elemento del vector y la frecuencia de este
            //es menor
            if (ultimo == 2*i + 1 && vector [ultimo]->getFrecuencia() < b->getFrecuencia()){
                vector[i] = vector[ultimo];
                i = ultimo;
            }
            //Su hijo izquierdo tiene una frecuencia menor que la propia y la del hijo
            //derecho
            else if (vector[2*i+1]->getFrecuencia() < vector[2*i+2]->getFrecuencia() && 
                        vector[2*i+1]->getFrecuencia() < b->getFrecuencia()){
                vector[i] = vector[2*i+1];
                i = 2*i+1;
            }
            //Su hijo derecho tiene una frecuencia menor que la propia
            else if (vector[2*i+2]->getFrecuencia() < b->getFrecuencia()){
                vector[i] = vector[2*i+2];
                i = 2*i+2;
            }
            //No hay que permutar más, frecuencia menor que la de los hijos
            else{
                finalizar = true;
            }
        }   
        vector[i]=b;     
        return a;
    }
}

bool Monticulo_arboles:: terminado(){
    return ultimo==0;
}

bool Monticulo_arboles:: vacio(){
    return ultimo==-1;
}