/************************************************
 * Autores: Santiago Buey, David Solanas        *
 * Fichero: Productos.hpp                       *
 * Práctica 1 - APD                             *
 ************************************************/

#ifndef PRODUCTOS_HPP
#define PRODUCTOS_HPP

#include <iostream>

/*
 * Clase que representa los productos de Amazon 
 * con los que se va a trabajar
 */
class Producto
{
private:
    std::string nombre; //Nombre del producto
    int unidades; //Unidades restantes del producto
    double precio; //Precio del producto

public:
    Producto() {}
    Producto(const std::string &_nombre, const int _unidades, const double _precio) : nombre(_nombre),
                                                                                      unidades(_unidades), precio(_precio) {}
    void set_nombre(const std::string &nombre);
    void set_unidades(const int unidades);
    void set_precio(const double precio);
    std::string get_nombre() const;
    int get_unidades() const;
    double get_precio() const;
};

void Producto::set_nombre(const std::string &nombre)
{
    this->nombre = nombre;
}

void Producto::set_unidades(const int unidades)
{
    this->unidades = unidades;
}

void Producto::set_precio(const double precio)
{
    this->precio = precio;
}

std::string Producto::get_nombre() const
{
    return nombre;
}

int Producto::get_unidades() const
{
    return unidades;
}

double Producto::get_precio() const
{
    return precio;
}

#endif // !PRODUCTOS_HPP