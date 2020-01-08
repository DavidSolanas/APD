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

class Sufijo
{
private:
    int indice;
    string sufijo;

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

/**
 * Crea los grupos de sufijos S0, S1 y S2 dado el texto t
 */
void crear_grupos_de_sufijos(const string &t, vector<Sufijo> &s0, vector<Sufijo> &s1, vector<Sufijo> &s2)
{
    const int len = t.size();
    for (int q = 0; 3 * q < len; q++)
    {
        s0.push_back(Sufijo(3 * q, t.substr(3 * q, len)));
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

#endif // !VECTOR_SUFIJOS_HPP