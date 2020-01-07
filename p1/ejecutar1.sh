#!/bin/bash
g++ -std=c++11 generar_datos.cpp -o generar_datos
g++ -std=c++11 generar_productos.cpp -o generar_productos
g++ -std=c++11 pract1.cpp -o pract1

echo "¿Quiere utilizar la opción verbose? (0 ó 1)"
echo "  1 = Sí"
echo "  0 = No"
read verbose

echo "¿Qué algoritmo quiere utilizar? (0 ó 1)"
echo "  Karger = 0"
echo "  Karger-Stein = 1"
read algoritmo


if [ $verbose -eq 0 ] && [ $algoritmo -eq 0 ]
then
    echo "Calculando problema para un dos Cliques de 8 vértices..."
    ./pract1 datos/datos_clique.txt datos/productos_8.txt
    echo
    echo "Calculando problema para un grafo no conexo..."
    ./pract1 datos/datos_desconectado.txt datos/productos_10.txt
    echo
    echo "Calculando problema para un grafo de 57 vértices..."
    ./pract1 datos/datos_57_075.txt datos/productos_57.txt
    echo
    echo "Calculando problema para un grafo de 100 vértices..."
    ./pract1 datos/datos_100_06.txt datos/productos_100.txt
    echo
    echo "Calculando problema para un grafo de 300 vértices..."
    ./pract1 datos/datos_300_05.txt datos/productos_300.txt
fi

if [ $verbose -eq 0 ] && [ $algoritmo -eq 1 ]
then
    echo "Calculando problema para un dos Cliques de 8 vértices..."
    ./pract1 datos/datos_clique.txt datos/productos_8.txt -a karger-stein
    echo
    echo "Calculando problema para un grafo no conexo..."
    ./pract1 datos/datos_desconectado.txt datos/productos_10.txt -a karger-stein
    echo
    echo "Calculando problema para un grafo de 57 vértices..."
    ./pract1 datos/datos_57_075.txt datos/productos_57.txt -a karger-stein
    echo
    echo "Calculando problema para un grafo de 100 vértices..."
    ./pract1 datos/datos_100_06.txt datos/productos_100.txt -a karger-stein
    echo
    echo "Calculando problema para un grafo de 300 vértices..."
    ./pract1 datos/datos_300_05.txt datos/productos_300.txt -a karger-stein
fi

if [ $verbose -eq 1 ] && [ $algoritmo -eq 0 ]
then
    echo "Calculando problema para un dos Cliques de 8 vértices..."
    ./pract1 datos/datos_clique.txt datos/productos_8.txt -v
    echo
    echo "Calculando problema para un grafo no conexo..."
    ./pract1 datos/datos_desconectado.txt datos/productos_10.txt -v
    echo
    echo "Calculando problema para un grafo de 57 vértices..."
    ./pract1 datos/datos_57_075.txt datos/productos_57.txt -v
    echo
    echo "Calculando problema para un grafo de 100 vértices..."
    ./pract1 datos/datos_100_06.txt datos/productos_100.txt -v
    echo
    echo "Calculando problema para un grafo de 300 vértices..."
    ./pract1 datos/datos_300_05.txt datos/productos_300.txt -v
fi

if [ $verbose -eq 1 ] && [ $algoritmo -eq 1 ]
then
    echo "Calculando problema para un dos Cliques de 8 vértices..."
    ./pract1 datos/datos_clique.txt datos/productos_8.txt -v -a karger-stein
    echo
    echo "Calculando problema para un grafo no conexo..."
    ./pract1 datos/datos_desconectado.txt datos/productos_10.txt -v -a karger-stein
    echo
    echo "Calculando problema para un grafo de 57 vértices..."
    ./pract1 datos/datos_57_075.txt datos/productos_57.txt -v -a karger-stein
    echo
    echo "Calculando problema para un grafo de 100 vértices..."
    ./pract1 datos/datos_100_06.txt datos/productos_100.txt -v -a karger-stein
    echo
    echo "Calculando problema para un grafo de 300 vértices..."
    ./pract1 datos/datos_300_05.txt datos/productos_300.txt -v -a karger-stein
fi
