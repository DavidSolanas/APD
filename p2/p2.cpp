/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: p2.cpp                              *
 * Práctica 2 - APD                             *
 ************************************************/

#include "Vector_sufijos.hpp"

int main(int argc, char const *argv[])
{
    vector<Sufijo> s0, s1, s2;

    crear_grupos_de_sufijos("processing", s0, s1, s2);
    mostrar_grupos(s0, s1, s2);

    return 0;
}
