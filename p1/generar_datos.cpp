#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <vector>

int get_n_related(const std::vector<std::vector<bool>> &M, const int n)
{
    int cont = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (M[i][j])
            {
                cont++;
            }
        }
    }
    return cont / 2;
}

int main(int argc, char const *argv[])
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<float> dist(10, 100);
    int n_datos = dist(mt);
    const float prob_0 = 0.25f;
    std::vector<std::vector<bool>> M(n_datos, std::vector<bool>(n_datos));
    std::uniform_real_distribution<float> dist_val(0.0f, 1.0f);
    for (int i = 0; i < n_datos; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            bool data = dist_val(mt) >= prob_0 ? 1 : 0;
            M[i][j] = data;
            M[j][i] = data;
        }
    }

    // Volcar datos en un fichero
    std::ofstream f("./datos.txt");
    if (f.is_open())
    {
        f << n_datos << std::endl;
        for (int i = 0; i < n_datos; i++)
        {
            for (int j = 0; j < n_datos; j++)
            {
                f << M[i][j] << " ";
            }
            f << "\n";
        }
        f.close();
    }
    else
    {
        std::cerr << "Fichero no creado" << std::endl;
    }
    int c = get_n_related(M, n_datos);
    std::cout << c << std::endl;
    return 0;
}
