/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: Vector_sufijos.hpp                  *
 * Práctica 2 - APD                             *
 ************************************************/

#ifndef VECTOR_SUFIJOS_HPP
#define VECTOR_SUFIJOS_HPP

#include <iostream>
#include <vector>

using namespace std;

/**
 * Clase que representa la estructura de datos de un sufijo
 */
class Sufijo
{
private:
    int indice;     //Posicion del sufijo en el vector
    string sufijo;  //Contenido del sufijo

public:
    Sufijo(const int i, const string &s);
    int get_indice() const;
    string get_sufijo() const;
    void set_indice(const int i);
    void set_sufijo(const string &s);
};

Sufijo::Sufijo(const int i, const string &s) : indice(i), sufijo(s) {}

int Sufijo::get_indice() const
{
    return indice;
}

string Sufijo::get_sufijo() const
{
    return sufijo;
}

void Sufijo::set_indice(const int i)
{
    indice = i;
}

void Sufijo::set_sufijo(const string &s)
{
    sufijo = s;
}

/**
 * Clase que representa la estructura de datos de un vector de sufijos
 */
class Vector_sufijos
{
private:
    int size; // Tamaño del vector de sufijos
    int *A;   // Vector de sufijos

public:
    Vector_sufijos(const int size);
};

Vector_sufijos::Vector_sufijos(const int size)
{
    this->size = size;
    A = new int[size];
    //Inicializar vector de sufijos con índices no válidos
    for (int i = 0; i < size; i++)
    {
        A[i] = -1;
    }
}

int getSize (const Vector_sufijos &vs){
    return vs.size;
}

/**
 * Crea los grupos de sufijos S0, S1 y S2 dado el texto t
 */
void crear_grupos_de_sufijos(const string &t, vector<Sufijo> &s0, vector<Sufijo> &s1, vector<Sufijo> &s2)
{
    const int len = t.size();
    for (int q = 0; 3 * q < len; q++)
    {
        s0.push_back(Sufijo(3 * q, t.substr(3 * q, len))); //substr(inicio,numCaracteres a incluir)
        if (3 * q + 1 < len)
            s1.push_back(Sufijo(3 * q + 1, t.substr(3 * q + 1, len)));
        if (3 * q + 2 < len)
            s2.push_back(Sufijo(3 * q + 2, t.substr(3 * q + 2, len)));
    }
}

void mostrar_grupos(vector<Sufijo> &s0, vector<Sufijo> &s1, vector<Sufijo> &s2)
{
    cout << "S0" << endl;
    for (auto &&i : s0)
    {
        cout << i.get_indice() << ": " << i.get_sufijo() << endl;
    }

    cout << "\nS1" << endl;
    for (auto &&i : s1)
    {
        cout << i.get_indice() << ": " << i.get_sufijo() << endl;
    }

    cout << "\nS2" << endl;
    for (auto &&i : s2)
    {
        cout << i.get_indice() << ": " << i.get_sufijo() << endl;
    }
}

vector<int> counting_sort(vector<string> &R, const int i)
{
    const int k = 256; //Número de letras del alfabeto (tabla ASCII)
    // Inicializar contador
    int C[k] = {0};
    vector<int> B(R.size());
    //Contar frecuencias (num de elementos de cada tipo)
    for (int j = 0; j < R.size(); j++)
    {
        //Comprobación de rango del string
        int v = i >= R[j].size() ? 0 : R[j].at(i);
        C[v]++;
    }
    //Contar el número de elementos menores o iguales que el j-ésimo
    for (int j = 1; j < k; j++)
    {
        C[j] = C[j] + C[j - 1];
    }

    //Calcular la posición de cada elemento en el vector ordenado
    for (int j = R.size() - 1; j >= 0; j--)
    {
        int v = i >= R[j].size() ? 0 : R[j].at(i);
        B[C[v] - 1] = j;
        C[v]--;
    }
    return B;
}

/**
 * Algoritmo Radix Sort para 3-gramas
 */
void radix_sort(vector<string> &R)
{
    for (int i = 2; i >= 0; i--)
    {
        vector<int> B = counting_sort(R, i);
        vector<string> aux = R;
        for (int j = 0; j < B.size(); j++)
        {
            R[j] = aux[B[j]];
        }
    }
}

#endif // !VECTOR_SUFIJOS_HPP