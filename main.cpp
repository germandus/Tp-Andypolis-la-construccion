#include "inventario.h"
#include <iostream>

int main (){
    Inventario *inventario = new Inventario;
    inventario -> cargar_materiales();
    //cout << "hola ";

    
    // for (int i = 0; i < inventario->obtener_cantidad_de_materiales(); i++ ){

    //     cout << inventario -> obtener_material_de_lista_materiales(i) -> obtener_nombre()<< " "
    //     <<inventario -> obtener_material_de_lista_materiales(i) -> obtener_cantidad_disponible()<<endl;
    //     }
    
    //Restar materiales (se debe saber cuanto consume el edificio)

    //agregar material (se debe saber cuanto consume el edificio) ya devuelve la mitad
    // inventario -> obtener_material_de_lista_materiales(0) -> restar_material(100);
    //inventario ->lista_materiales[1] -> sumar_material(900);
    
    //mostrar inventario
    //inventario -> mostrar_inventario();
    
    if (inventario ->alcanzan_materiales(100, 200,300)){
        cout<<" alcazaron ";
        inventario ->utilizar_materiales(100,200,300);
    }else{
        cout<<" no alcanzaron xd ";
        };

    inventario -> mostrar_inventario();
    //cout <<"aaaaaaaaaaaaaaaaaa";

    
    //cout <<" hola ";

    //NO PUEDO INSTANCIAR CLASES HIJAS DE MATERIAL!!!
    // Material cosa("holis ",1) ;
    // Material cosa_2("hola ",2);

    //cosa.saludar();

    delete inventario;


    return 0;
}