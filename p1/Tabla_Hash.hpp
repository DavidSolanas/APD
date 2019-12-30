/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: Tabla_Hash.hpp                      *
 * Práctica 1 - APD                             *
 ************************************************/

#ifndef TABLA_HASH_HPP
#define TABLA_HASH_HPP

#include "Productos.hpp"

class Tabla_Hash
{
private:
    Producto **productos;
    int N;

public:
    Tabla_Hash(const int n)
    {
        N = n;
        productos = new Producto *[n];
        for (int i = 0; i < n; i++)
        {
            productos[i] = nullptr;
        }
    }

    int h(const std::string &s);
    void insertar(Producto *p);
    Producto *get(const std::string &s);
};

int Tabla_Hash::h(const std::string &s)
{
    int hash = 11;
    for (int i = 0; i < s.size(); i++)
    {
        hash = hash * 31 + s[i];
    }
    return hash % N;
}

void Tabla_Hash::insertar(Producto *p)
{
    int index = h(p->get_nombre());
    int c = 0;
    while (productos[index] != nullptr &&
           productos[index]->get_nombre() != p->get_nombre())
    {
        if (c++ > N)
            return;
        index = (index + 1) % N;
    }
    productos[index] = p;
}

Producto *Tabla_Hash::get(const std::string &s)
{
    int index = h(s);
    int fin = index - 1;
    while (productos[index] != nullptr)
    {
        if (productos[index]->get_nombre() == s)
        {
            return productos[index];
        }
        if (index == fin)
        {
            return nullptr;
        }

        index = (index + 1) % N;
    }
    return nullptr;
}

#endif // !TABLA_HASH_HPP