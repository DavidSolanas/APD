#!/bin/bash
g++ -std=c++11 p2.cpp -o bzip2

echo "Comprimiendo quijote.txt..."
./bzip2 -c datos/quijote.txt
echo "Listo!"
mv datos/quijote.txt.bz2 datos/quijote_comprimido.txt.bz2
echo "Fichero generado: quijote_comprimido.txt.bz2"

echo "Descomprimiendo quijote_comprimido.txt.bz2..."
./bzip2 -d datos/quijote_comprimido.txt.bz2
echo "Listo!"
mv datos/quijote_comprimido.txt datos/quijote_descomprimido.txt
echo "Fichero generado: quijote_descomprimido.txt"