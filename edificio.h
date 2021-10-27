#ifndef EDIFICIO_H
#define EDIFICIO_H

#include <iostream>

using namespace std;

class Edificio
{
private:
    string nombre;
    int cantidad_piedra;
    int canitdad_madera;
    int cantidad_metal;
    int maximo_construir;
    int fila;
    int columna;

public:
    Edificio(std::string nombre , int cantidad_piedra, int cantidad_madera, int cantidad_metal, int maximo_construir);
    string mostrar_nombre();

    int mostrar_cantidad_piedra();

    int mostrar_canitdad_madera();

    int mostrar_cantidad_metal();

    int mostrar_maximo_construir();

    void cambiar_fila_columna(int i, int j);

};




#endif // EDIFICIO_H