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
    vector<string> R;
    R.push_back("roc");
    R.push_back("a");
    R.push_back("aa");
    R.push_back("aab");
    R.push_back("ess");
    R.push_back("ing");
    R.push_back("oce");
    R.push_back("ssi");
    R.push_back("ng");
    radix_sort(R);

    for (auto &&i : R)
    {
        cout << i << endl;
    }

    return 0;
}
