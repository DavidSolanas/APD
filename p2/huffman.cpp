/**
 * Fichero p1.cpp
 * Práctica 1 Algoritmia Básica
 * Autores: Diego Martínez Baselga      735969
 *          David Solanas Sanz          738630
 */

#include <iostream>
#include <fstream>
#include "Monticulo_arboles.h"
#include <bitset>
#include <cstring>

using namespace std;

/**
 * Devuelve el tamaño del fichero f en Bytes.
 */
int tamanno_fichero(ifstream &f)
{
    f.clear();
    f.seekg(0, f.end);
    int tam = f.tellg();
    f.seekg(0, f.beg);
    return tam;
}

/**
 * Lee el contenido del vector de caracteres datos y almacena
 * en el vector frecuencias el número de apariciones de 
 * cada carácter en dicho vector. La posición de un carácter
 * en el vector frecuencias vendrá determinado por su código ASCII.
 * n es el tamaño en Bytes del fichero
 */
void calcular_frecuencias(unsigned char *datos, unsigned int *frecuencias, int n)
{
    for (int i = 0; i < n; i++)
    {
        frecuencias[datos[i]]++;
    }
}

/**
 * monticulo: Monticulo de arboles vacio
 * frecuencias: Vector en el que cada componente de posición i almacena
 *              el número de apariciones del caracter con código ASCII i
 * Devuelve en monticulo el montículo de árboles formado por los árboles de
 * Huffman de un solo caracter ordenado por número de apariciones
 */
void inicializar_monticulo(Monticulo_arboles *monticulo, unsigned int *frecuencias)
{
    for (int i = 0; i < 256; i++)
    {
        if (frecuencias[i] != 0)
        {
            monticulo->insertar(new Arbol_caracteres(i, frecuencias[i]));
        }
    }
}
/**
 * frecuencias: Vector en el que cada componente de posición i almacena
 *              el número de apariciones del caracter con código ASCII i
 * Devuelve el árbol Huffman  correspondiente al
 * vector de frecuencias frecuencias
 */
Arbol_caracteres *huffman(unsigned int *frecuencias)
{
    Monticulo_arboles *monticulo = new Monticulo_arboles();
    inicializar_monticulo(monticulo, frecuencias);
    if (monticulo->terminado())
    {
        //Un solo tipo de byte en todo el fichero
        Arbol_caracteres *a = monticulo->borrar_primero();
        monticulo->insertar(new Arbol_caracteres(a));
    }
    if (monticulo->vacio())
    {
        //Fichero vacío
        return nullptr;
    }
    else
    {
        while (!monticulo->terminado())
        {
            Arbol_caracteres *a = monticulo->borrar_primero();
            Arbol_caracteres *b = monticulo->borrar_primero();
            monticulo->insertar(new Arbol_caracteres(a, b));
        }
        return monticulo->borrar_primero();
    }
}
/**
 * a: árbol de Huffman
 * cadena: Código correspondiente a la raíz de a
 * Devuelve en cada posicion i del vector codigos el codigo Huffman correspondiente
 * al byte i, dado el arbol a
 */
void clacular_codigos(Arbol_caracteres *a, string codigos[], string cadena)
{
    if (a != nullptr)
    {
        if (a->getIzq() == nullptr)
        {
            codigos[a->getCaracter()] = cadena;
        }
        else if (a->getDer() == nullptr)
        {
            //Caso en el que el árbol está compuesto por solo una letra (un nodo y una hoja)
            clacular_codigos(a->getIzq(), codigos, cadena + '0');
        }
        else
        {
            clacular_codigos(a->getIzq(), codigos, cadena + '0');
            clacular_codigos(a->getDer(), codigos, cadena + '1');
        }
    }
}
/**
 * contenido: contenido en bytes de un fichero
 * tam: tamaño del fichero en bytes
 * codigos: códigos Huffman correspondientes a cada Byte perteneciente al fichero
 * f_salida: fichero de salida en el que escribir el código resultante de codificar
 *            contenido
 * Escribe en f_salida el código correspondiente a codificar los primeros tam bytes
 * de contenido utilizando los codigos del vector codigos
 */
uint8_t escribir_codigo(unsigned char *contenido, int tam, string codigos[],
                        ofstream &f_salida)
{
    string codigo = "";
    for (int i = 0; i < tam; i++)
    {
        codigo += codigos[contenido[i]];
        if (codigo.length() >= 8)
        {
            char byte;
            for (int j = 0; j < 8; j++)
            {
                if (codigo[8 - j - 1] == '0')
                {
                    byte &= ~(1 << j);
                }
                else
                {
                    byte |= 1 << j;
                }
            }
            f_salida.write(&byte, sizeof(byte));
            codigo = codigo.substr(8);
        }
    }
    //El último byte del código a escribir puede tener un tamaño menor de 8.
    //Se completa con bits 0 ese byte para poderlo escribir correctamente
    uint8_t resto = 0;
    if (codigo.length() > 0)
    {
        char byte = 0;
        for (int j = 0; j < codigo.length(); j++)
        {
            if (codigo[j] == '0')
            {
                byte &= ~(1 << (8 - j - 1));
            }
            else
            {
                byte |= 1 << (8 - j - 1);
            }
        }
        f_salida.write(&byte, sizeof(byte));
        resto = codigo.length();
    }
    return resto;
}

/**
 * f: Fichero que almacena un arbol Huffman. En el fichero
 *    se almacena de cada nodo el byte al que corresponde y si es hoja. Los nodos se
 *    almacenan por niveles y de izquierda a derecha, es decir, primero la raíz,
 *    después sus hijos (izquierdo y derecho), después los hijos del izquierdo, 
 *    los del derecho y así sucesivamente.
 * num_nodos: Número de nodos del árbol
 * Devuelve el árbol almacenado en el fichero f
 */
Arbol_caracteres *leer_arbol(ifstream &f, unsigned int num_nodos)
{
    Arbol_caracteres *arboles = (Arbol_caracteres *)malloc(num_nodos * sizeof(Arbol_caracteres));
    unsigned char car;
    bool esHoja[num_nodos];
    queue<Arbol_caracteres *> *cola = new queue<Arbol_caracteres *>;
    for (int i = 0; i < num_nodos; i++)
    {
        f.read(reinterpret_cast<char *>(&car), sizeof(unsigned char));
        f.read(reinterpret_cast<char *>(&esHoja[i]), sizeof(bool));
        arboles[i].setCaracter(car);
        cola->push(&arboles[i]);
    }
    cola->pop();
    for (int i = 0; i < num_nodos; i++)
    {
        if (!cola->empty() && !esHoja[i])
        {
            arboles[i].setIzq(cola->front());
            cola->pop();
            if (!cola->empty())
            {
                arboles[i].setDer(cola->front());
                cola->pop();
            }
            else
            {
                arboles[i].setDer(nullptr);
            }
        }
        else
        {
            arboles[i].setIzq(nullptr);
            arboles[i].setDer(nullptr);
        }
    }
    return arboles;
}
/**
 * arbol: Árbol de Huffman que descifra el código almacenado en codigo
 * codigo: código Huffman almacenado a nivel de bits
 * n: tamaño en bytes de codigo
 * bits_extra: bits añadidos al final de codigo para los bits finales formaran un
 *             byte
 * tam_original: tamaño del fichero una vez descifrado
 * Devuelve un vector de bytes que almacena el contenido de codigo una vez
 * descifrado
 */
unsigned char *descifrar(Arbol_caracteres *arbol, unsigned char *codigo, int n,
                         uint8_t bits_extra, int tam_original)
{
    Arbol_caracteres *aux = arbol;
    unsigned char *texto = (unsigned char *)malloc(tam_original);
    int k = 0;
    for (int i = 0; i < n - 1; i++)
    {
        char c = codigo[i];
        for (int j = 7; j >= 0; j--)
        {
            if ((c >> j) & 1)
            {
                aux = aux->getDer();
            }
            else
            {
                aux = aux->getIzq();
            }
            if (aux->getIzq() == nullptr)
            {
                texto[k++] = aux->getCaracter();
                aux = arbol;
            }
        }
    }
    char c = codigo[n - 1];
    for (int j = 7; j > 7 - bits_extra; j--)
    {
        if ((c >> j) & 1)
        {
            aux = aux->getDer();
        }
        else
        {
            aux = aux->getIzq();
        }
        if (aux->getIzq() == nullptr)
        {
            texto[k++] = aux->getCaracter();
            aux = arbol;
        }
    }
    return texto;
}
/**
 * nom_fichero: nombre del fichero a comprimir
 * Escribe en <nom_fichero>.huf una compresión utilizando códigos Huffman
 * del fichero de manera que la función descomprimir lo pueda recuperar.
 * El fichero resultante almacena en el siguiente orden:
 *     - bits extra añadidos al código para formar el último byte
 *     - número de nodos del árbol de Huffman
 *     - tamaño de <nom_fichero>
 *     - árbol de Huffman, por niveles (empezando por la raíz) y de izquierda a 
 *       derecha entre nodos del mismo nivel, almacenando de cada nodo el byte al que
 *       corresponde y si es hoja
 *     - código Huffman correspondiente al fichero
 */
void comprimir(const char *nom_fichero, unsigned char *input, int tam)
{
    /*        
        //Lectura del fichero y almacenamiento del contenido en un
        //vector de una manera eficiente
        ifstream f_entrada;
        f_entrada.open(nom_fichero, ios::binary);
        int tam = tamanno_fichero(f_entrada);
        unsigned char *contenido = (unsigned char *)malloc(tam);
        f_entrada.read((char*)contenido, tam);
        f_entrada.close();
*/

    // Creación del array de frecuencias e inicialización a 0
    unsigned int frecuencias[256] = {0};
    calcular_frecuencias(input, frecuencias, tam);
    // Creación del árbol Huffman del fichero
    Arbol_caracteres *arbol = huffman(frecuencias);
    // Cálculo de código Huffman para cada byte distinto perteneciente al fichero
    string codigos[256];
    clacular_codigos(arbol, codigos, "");
    //Escritura del fichero <nom_fichero.huf>
    ofstream f_salida;
    f_salida.open(string(nom_fichero) + ".bz2", ios::binary);
    //Espacio para el número de bits añadidos al final que no forman parte
    //de la cadena original (que completan el ultimo byte),
    //para el número de nodos del árbol y para el tamaño del fichero original
    f_salida.seekp(sizeof(uint8_t) + sizeof(unsigned int) + sizeof(int));
    unsigned int num_nodos = 0;
    //Puede ser un fichero vacío
    if (arbol != nullptr)
    {
        //Se escribe el árbol
        num_nodos = arbol->escribir_arbol_fichero(f_salida);
    }
    //Se escribe el código
    uint8_t bits_extra = escribir_codigo(input, tam, codigos, f_salida);
    //Se escribe el número de bits extra, número de nodos y tamaño del
    //fichero original al principio del fichero
    f_salida.seekp(0);
    f_salida.write(reinterpret_cast<char *>(&bits_extra), sizeof(uint8_t));
    f_salida.write(reinterpret_cast<char *>(&num_nodos), sizeof(unsigned int));
    f_salida.write(reinterpret_cast<char *>(&tam), sizeof(int));
    f_salida.close();
}

/**
 * nom_fichero: en formato <fichero_original>.huf. Almacena el árbol de Huffman
 *              y el código correspondiente de un fichero.
 * El fichero nom_fichero contiene la siguiente estructura:
 *     - bits extra añadidos al código para formar el último byte
 *     - número de nodos del árbol de Huffman
 *     - tamaño de <nom_fichero>
 *     - árbol de Huffman, por niveles (empezando por la raíz) y de izquierda a 
 *       derecha entre nodos del mismo nivel, almacenando de cada nodo el byte al que
 *       corresponde y si es hoja
 *     - código Huffman correspondiente al fichero
 * La función descomprime el fichero nom_fichero y lo escribe en el 
 * fichero <fichero_original>. 
 */
unsigned char *descomprimir(string nom_fichero, int &tam_original)
{
    string nom_salida = nom_fichero.substr(0, nom_fichero.length() - 4);
    ifstream f;
    f.open(nom_fichero, ios::binary);
    int tam = tamanno_fichero(f);
    uint8_t bits_extra;
    f.read(reinterpret_cast<char *>(&bits_extra), sizeof(uint8_t));
    unsigned int num_nodos;
    f.read(reinterpret_cast<char *>(&num_nodos), sizeof(unsigned int));
    //    int tam_original;
    f.read(reinterpret_cast<char *>(&tam_original), sizeof(int));
    Arbol_caracteres *arbol = leer_arbol(f, num_nodos);
    int location = f.tellg();
    unsigned char *codigo = (unsigned char *)malloc(tam - location);
    f.read((char *)codigo, tam - location);
    f.close();
    unsigned char *contenido = descifrar(arbol, codigo, tam - location, bits_extra, tam_original);

    return contenido;
    /*
    ofstream f_salida;
    f_salida.open(nom_salida, ios::binary);
    f_salida.write((char*)contenido, tam_original);
    f_salida.close();
*/
}
