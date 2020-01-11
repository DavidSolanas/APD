/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: p2.cpp                              *
 * Práctica 2 - APD                             *
 ************************************************/

#include "Vector_sufijos.hpp"


void transformadaBW (const Vector_sufijos &vs, const string input){
    int len = vs.getSize();
    string transformada="";

    int i;
    for (int i=0; i < len; i++){
        //Pilla el ultimo caracter
    }


} 


int main(int argc, char const *argv[])
{
    string filenameIn = argv[1];
    string filenameOut = argv[2];

    

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
