#include <iostream>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::string namefile = argv[1];
    int n_datos = atoi(argv[2]);
    std::vector<std::vector<bool>> M(n_datos, std::vector<bool>(n_datos));
    std::uniform_real_distribution<float> dist_precio(10, 2000);
    std::uniform_real_distribution<float> dist_unidades(100, 50000);

    // Volcar datos en un fichero
    std::ofstream f(namefile);
    if (f.is_open())
    {
        int precio, unidades;
        for (int i = 0; i < n_datos; i++)
        {
            precio = dist_precio(mt);
            unidades = dist_unidades(mt);
            f << "Producto_" << i + 1 << "\t" << precio << "\t" << unidades << "\n";
        }
        f.close();
    }
    else
    {
        std::cerr << "Fichero no creado" << std::endl;
    }
    return 0;
}
