#include <iostream>
#include <fstream>
#include <cstring>
#include "huffman.cpp"
using namespace std;

//--------------------------FUNCIONES HUFFMAN:--------------------------



//----------------------Fin de las FUNCIONES HUFFMAN----------------------

/**
 * Devuelve cierto si el par (x1,x2) es menor que el par (y1,y2)
 * comparando x1 con y1 y x2 con y2.
 */
bool cmp_par(int x1, int x2, int y1, int y2)
{
    return (x1 < y1 || (x1 == y1 && x2 <= y2));
}

/**
 * Devuelve cierto si la tripleta (x1,x2,x3) es menor que la tripleta (y1,y2,y3)
 * comparando x1 con y1, x2 con y2 y x3 con y3.
 */
bool cmp_tripleta(int x1, int x2, int x3, int y1, int y2, int y3)
{
    return (x1 < y1 || (x1 == y1 && cmp_par(x2, x3, y2, y3)));
}

/**
 * Devuelve el valor máximo del vector de enteros v de tamaño n
 * se considera que todos los valores de v son enteros positivos.
 */
int max(unsigned char *v, int n)
{
    int max = -1;
    for (int i = 0; i < n; i++)
    {
        int val = (int)v[i];
        max = val > max ? val : max;
    }
    return max;
}

/**
 * Aplica el algoritmo de radix sort para ordenar el vector src con los valores del
 * vector v (de 0 a K).
 */
void radix_sort(int *src, int *dst, int *v, int offset,
                int n, int K, int start)
{
    int cnt[K + 2];
    // Inicializar contador
    for (int i = 0; i < K + 2; i++)
    {
        cnt[i] = 0;
    }

    // Contar ocurrencias de los caracteres
    for (int i = 0; i < n; i++)
    {
        int caracter = v[start + offset + src[i]];
        cnt[caracter]++;
    }

    // Contar el número de elementos menores o iguales que el i-ésimo
    for (int i = 0, suma = 0; i <= K; i++)
    {
        int t = cnt[i];
        cnt[i] = suma;
        suma += t;
    }

    // Ordenar el vector src almacenando el resultado en dst
    for (int i = 0; i < n; i++)
    {
        //Carácter
        int caracter = v[start + offset + src[i]];
        //Nueva posición en el vector del sufijo src[i]
        int j = cnt[caracter]++;
        dst[j] = src[i];
    }
}

/**
 * Calcula el vector de sufijos dado el vector de enteros (caracteres) s.
 */
void vector_sufijo(int *s, int *SA, int n, int K, int start)
{
    int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
    int *s12 = new int[n02 + 3];
    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;

    int *SA12 = new int[n02 + 3];
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;

    int *s0 = new int[n0];
    int *SA0 = new int[n0];

    /*
    * Genera las posiciones no múltiplos de 3 de los sufijos de la entrada s
    */
    for (int i = 0, j = 0; i < n + (n0 - n1); i++)
    {
        if ((i % 3) != 0)
            s12[j++] = i;
    }

    // Aplica radix sort a las tripletas de los sufijos de s
    radix_sort(s12, SA12, s, +2, n02, K, start);
    radix_sort(SA12, s12, s, +1, n02, K, start);
    radix_sort(s12, SA12, s, +0, n02, K, start);

    // Calcular el rango (rank) de las tripletas ordenadas
    // c0, c1 y c2 se usan para comparar dos tripletas y ver que son
    // distintas, si son distintas, se incrementa el rango, si son iguales
    // se le pone el mismo rango y habrá que aplicar recursividad
    int rank = 0, c0_ant = -1, c1_ant = -1, c2_ant = -1;
    for (int i = 0; i < n02; i++)
    {
        if (s[start + SA12[i]] != c0_ant || s[start + SA12[i] + 1] != c1_ant || s[start + SA12[i] + 2] != c2_ant)
        { // Tripletas distintas
            rank++;
            c0_ant = s[start + SA12[i]];
            c1_ant = s[start + SA12[i] + 1];
            c2_ant = s[start + SA12[i] + 2];
        }
        if ((SA12[i] % 3) == 1)
        {
            // Mitad izquierda
            s12[SA12[i] / 3] = rank;
        }
        else
        {
            // Mitad derecha
            s12[SA12[i] / 3 + n0] = rank;
        }
    }

    // Si rank < n02 implica que había alguna tripleta repetida,
    // sino hubieran sido todas distintas y sería igual a n02.
    // Hay que aplicar recursividad para eliminar las repetidas
    if (rank < n02)
    {
        vector_sufijo(s12, SA12, n02, rank, start);
        // Guardar los valores únicos en s12 usando el vector de sufijos
        for (int i = 0; i < n02; i++)
            s12[SA12[i]] = i + 1;
    }
    else
    {
        // Generar vector de sufijos
        for (int i = 0; i < n02; i++)
            SA12[s12[i] - 1] = i;
    }
    // Ordenar los sufijos de posición múltiplo de 3 de SA12 por su primer carácter
    for (int i = 0, j = 0; i < n02; i++)
    {
        if (SA12[i] < n0)
            s0[j++] = 3 * SA12[i];
    }
    radix_sort(s0, SA0, s, 0, n0, K, start);

    // Mezclar SA0 y SA12
    for (int p = 0, t = n0 - n1, k = 0; k < n; k++)
    {
        // Posición del sufijo t-ésimo de SA12
        int i = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
        // Posición del sufijo p-ésimo de SA0
        int j = SA0[p];

        if (SA12[t] < n0 ? cmp_par(s[start + i], s12[SA12[t] + n0], s[start + j],
                                   s12[j / 3])
                         : cmp_tripleta(s[start + i], s[start + i + 1], s12[SA12[t] - n0 + 1],
                                        s[start + j], s[start + j + 1], s12[j / 3 + n0]))
        {
            // El sufijo de SA12 es menor que el de SA0
            SA[k] = i;
            t++;
            if (t == n02)
            {
                // Solo quedan los sufijos de SA0
                for (k++; p < n0; p++, k++)
                    SA[k] = SA0[p];
            }
        }
        else
        {
            SA[k] = j;
            p++;
            if (p == n0)
            {
                // Solo quedan los sufijos de SA12
                for (k++; t < n02; t++, k++)
                {
                    SA[k] = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
                }
            }
        }
    }
}

int *crear_vector_sufijos(unsigned char *input, int start, int length)
{
    int alphabetSize = max(input, length);
    int *SA = new int[length + 3];
    int *_in = new int[length + 3];

    //Añadir a la entrada 3 carácteres nulos para preservar los 3-gramas
    for (int i = 0; i < length; i++)
    {
        _in[i] = (int)input[i];
    }
    _in[length] = _in[length + 1] = _in[length + 2] = 0;

    vector_sufijo(_in, SA, length, alphabetSize, start);

    return SA;
}

/**
 * Devuelve el tamaño del fichero f en Bytes.
 */
int tamanyo_fichero(ifstream &f)
{
    f.clear();
    f.seekg(0, f.end);
    int tam = f.tellg();
    f.seekg(0, f.beg);
    return tam;
}

/**
 * Devuelve el indice que marca la posicion del caracter c en el diccionario
 */
int buscarIndice(unsigned char c, unsigned char *diccionario, const int dimDiccionario, int indiceC)
{
    int i;
    for (i = 0; i < dimDiccionario; i++)
    {
        if (diccionario[i] == c)
        {
            return i;
        }
    }
    if (i == dimDiccionario)
    {
        cerr << "ERROR: No se encuentra el caracter " << c << endl;
        exit(1);
    }
    return -1;
}
/**
 * Copia los primeros dim caracteres del string src al string dst
 */
inline void strcpy(unsigned char *dst, const unsigned char *src, const int dim)
{
    for (int i = 0; i < dim; i++)
    {
        dst[i] = src[i];
    }
}

/**
 * Mueve el caracter que ocupa la posicion indice en la cadena diccionario
 * a la posicion 0 de esta
 */
void moverDelante (int indice, unsigned char *diccionario, const int dimDiccionario){
    unsigned char *aux = (unsigned char *)malloc(dimDiccionario * sizeof(unsigned char));
    strcpy(aux, diccionario, dimDiccionario); //Copia todos los caracteres de diccionario en aux
    unsigned char c = diccionario[indice];
    strcpy(diccionario + 1, aux, indice); //Copia todos los caracteres hasta el indice encontrado
    diccionario[0]= c;
}

/**
 * Transforma input en una cadena de enteros, segun el algoritmo Move To Front
 */
unsigned char *moveToFront(unsigned char *input, const int tam, unsigned char *diccionario, const int dimDiccionario)
{   
    unsigned char *seqIndices = (unsigned char *)malloc(tam * sizeof(unsigned char));
    //int *seqIndices = (int *)malloc(tam * sizeof(int));
    
    int indice;
    for (int i = 0; i < tam; i++)
    { //Para cada caracter de la cadena input
        indice = buscarIndice(input[i], diccionario, dimDiccionario, i);
        seqIndices[i]= (char)indice;
        moverDelante(indice, diccionario, dimDiccionario);
    }
    return seqIndices;
}

/**
 * 
 */
unsigned char *moveToFrontInverso (unsigned char *seqIndices, const int tam, unsigned char *diccionario, const int dimDiccionario){
    unsigned char *mtfInverso = (unsigned char *)malloc(tam * sizeof(unsigned char));

//    cout << seqIndices << endl;

    for (int i=0; i < tam; i++){
        int indice= seqIndices[i];
//        cout << indice << endl;
        mtfInverso[i]= diccionario[indice];

        moverDelante(indice, diccionario, dimDiccionario);
    }
    return mtfInverso;
}

/**
 * Calcula la transformada de Burrows-Wheeler
 */
unsigned char *transformadaBW(int *suffix_array, unsigned char *input, const int tam)
{
    unsigned char *transformada = (unsigned char *)malloc(tam * sizeof(unsigned char));
    int i;
    for (i = 0; i < tam; i++)
    {
        transformada[i] = input[(suffix_array[i] - 1 + tam) % tam]; //Calcula el ultimo caracter
    }
    transformada[i] = '\0'; //Finaliza la cadena con el caracter nulo
    return transformada;
}

/**
 * 
 */
unsigned char *inversaBW(unsigned char *input, const int tam, const int dimDiccionario){
    // input: cadena codificada con BW
    // inversa: donde se va a almacenar la inversa BW de input
    unsigned char *inversa = (unsigned char *)malloc(tam * sizeof(unsigned char));
    //c[ti] almacena el numero de caracteres menores a ti
    int *c = (int *)malloc(tam * sizeof(int));
    //cMenores[]

    for (int i=0; i < tam; i++){
        inversa[i]= '\0';
    }

    // Bucle para rellenar c[] con la cuenta de menores a cada caracter
    for (int i=0; i < tam; i++){
        c[i]= 0;
        //unsigned char ti= input[i];
        for (int j=0; j < tam; j++){
            if (input[j] < input[i]){
                c[i]++;
            }
        }
    }

    //Para almacenar el numero de los caracteres que vamos detectando:
    int *index_acumulado = (int *)malloc(dimDiccionario * sizeof(int));
    for (int i=0; i < dimDiccionario; i++){
        index_acumulado[i]= 0;
    }
    //index almacena el numero de la ocurrencia del caracter
    int *index = (int *)malloc(tam * sizeof(int));
    for (int i=0; i < tam; i++){
        unsigned char ti = input[i];
        index[i]= index_acumulado[ti];
        index_acumulado[ti] = index_acumulado[ti] + 1;
    }


    //LF = C + i
    int *LF = (int *)malloc(tam * sizeof(int));
    for (int i=0; i < tam; i++){
        LF[i]= c[i] + index[i];
    }

    //DECODIFICAMOS:
    int r= 0;
    unsigned char cd = input[r];
    int i;
    for (i=0; i < tam; i++){
        //inversa = cd + inversa;
        inversa[i]= cd;
        r = LF[r];
        cd = input[r];
        
    }
    inversa[i]= '\0';

    return inversa;
}


int main(int argc, char const *argv[])
{
    string usage = "Uso: ./bzip2 [-c|-d] <fichero>";
    if (argc < 3)
    {
        cerr << "Número incorrecto de parámetros" << std::endl;
        cerr << usage << std::endl;
        exit(1);
    }
    ifstream f_entrada(argv[2]);
    if (!f_entrada.is_open())
    {
        cerr << "Imposible abrir el fichero de entrada, cancelando..." << endl;
        exit(1);
    }

    int tam = tamanyo_fichero(f_entrada);
    unsigned char *contenido = (unsigned char *)malloc(tam);
    f_entrada.read((char *)contenido, tam);
    f_entrada.close();

    const int dimDiccionario= 257;
    //Genera el diccionario a utilizar: los caracteres de la tabla ASCII
    unsigned char *diccionario = (unsigned char *)malloc(dimDiccionario * sizeof(unsigned char));
    unsigned char *diccionarioOriginal = (unsigned char *)malloc(dimDiccionario * sizeof(unsigned char));
    int i;
    for (i = 1; i < 256; i++){ //Empieza desde 1 para no pillar el caracter nulo ('\0')
        diccionario[i] = (unsigned char)i;
    }
    diccionario[i] = '\0';
    strcpy(diccionarioOriginal, diccionario, dimDiccionario);

    if (string(argv[1])== "-c"){    //COMPRIMIR
        int *suffix_array = crear_vector_sufijos(contenido, 0, tam);
        unsigned char *transformada = transformadaBW(suffix_array, contenido, tam);        
        unsigned char *mtf = moveToFront(transformada, tam, diccionario, dimDiccionario);      

        comprimir(argv[2], contenido, tam);

    }
    else if (string(argv[1])== "-d"){   //DESCOMPRIMIR
        string nom_fichero= argv[2];
        string nom_salida = nom_fichero.substr(0, nom_fichero.length()-4);
        int tam_original;
        unsigned char *descomprimido= descomprimir(nom_fichero, tam_original);
        
        unsigned char *mtfInverso= moveToFrontInverso(descomprimido, tam, diccionarioOriginal, dimDiccionario);

        unsigned char *inversaBurrows = inversaBW(mtfInverso, tam, dimDiccionario);

        ofstream f_salida;
        f_salida.open(nom_salida, ios::binary);
        f_salida.write((char*)inversaBurrows, tam_original);
        f_salida.close();

    }
    else{
        cout << "Opcion invalida: huf [-c|-d] <nombre_fichero>"<<endl;
    }

    return 0;
}
