#include "mapa.h"

Mapa::Mapa(){

    this->cantidad_filas = 0;
    this->cantidad_columnas = 0;
    this->mapa = 0;
    this->cantidad_edificios = 0;
    this->edificios_posibles = 0;
    this->usuario_inventario = 0;
    this -> vector_casilleros_lluvia = 0;
    this -> total_casilleros = 0;
    this -> mapa_bien_cargado = true;
    this -> ubicaciones_bien_cargadas = true;
}

bool Mapa::carga_incorrecta_archivos(){
    bool carga_incorrecta = ( ( ! mapa_bien_cargado ) || ( ! ubicaciones_bien_cargadas ) || (obtener_cantidad_edificios() == -1) 
    || (usuario_inventario->obtener_cantidad_de_materiales() == -1) );

    return carga_incorrecta;

}

void Mapa::ingreso_datos_mapa(){

    this->usuario_inventario = new Inventario;
    usuario_inventario->cargar_materiales();

    cargar_edificios();
    procesar_archivo_mapa();
    procesar_archivo_ubicaciones();
}

void Mapa::procesar_archivo_mapa(){

    ifstream arch;
    arch.open(ARCHIVO_MAPA);
    if(arch.is_open()){
    
        string filas, columnas;
        for ( int i = 0 ; i < 1 ; i++){
            arch >> filas; 
            arch >> columnas;

            cantidad_filas = stoi(filas);
            cantidad_columnas = stoi(columnas);
        }
        string nombre;

        generar_matriz();

        for ( int i = 0; i < cantidad_filas; i++){
            for (int j = 0; j < cantidad_columnas; j++){
                arch >> nombre ;
                if ( nombre == "T") {
                    this->mapa[i][j] = new Casillero_construible(i, j);
                } 
                if (nombre == "C") {
                    this->mapa[i][j] = new Casillero_transitable(i,j);
                }
                if (nombre == "L") {
                    this->mapa[i][j] = new Casillero_inaccesible(i,j);
                }
            }
        }
        arch.close();
        
    }else{
        mapa_bien_cargado = false;
    }
}

void Mapa::generar_matriz(){
    this->mapa = new Casillero ** [ cantidad_filas ];
    for ( int i = 0; i < cantidad_filas; i++){
        mapa[i] = new Casillero * [ cantidad_columnas ];
    }

}

void Mapa::procesar_archivo_ubicaciones(){

    ifstream archivo;
    archivo.open(ARCHIVO_UBICACIONES);

    if(archivo.is_open() && mapa_bien_cargado){
        string nombre,segundo_nombre, barra, fila, columna;

        while( archivo >> nombre ){
            if ( nombre == "planta"){
                archivo >> segundo_nombre;
                getline(archivo, barra, '(');
                getline(archivo, fila, ',');
                getline(archivo, barra, ' ');
                getline(archivo, columna, ')');

                nombre += " " + segundo_nombre;
            } else {
                getline(archivo, barra, '(');
                getline(archivo, fila, ',');
                getline(archivo, barra, ' ');
                getline(archivo, columna, ')');
            }

            if (nombre == "piedra" || nombre == "madera" || nombre == "metal"){
                mapa[stoi(fila)][stoi(columna)]->agregar_material(nombre,1);
            }

            int madera, piedra, metal, maximo;

            for ( int i = 0; i < obtener_cantidad_edificios(); i++){
                if ( obtener_edificio(i)->obtener_nombre() == nombre){
                    piedra = obtener_edificio(i)-> obtener_cantidad_piedra();
                    madera = obtener_edificio(i)->obtener_cantidad_madera();
                    metal = obtener_edificio(i)->obtener_cantidad_metal();
                    maximo = obtener_edificio(i)->obtener_maximo_construir();

                    mapa[stoi(fila)][stoi(columna)]->agregar_edificio(nombre, piedra, madera, metal, maximo);

                    obtener_edificio(i) ->sumar_cantidad();

                }
            }


        }

        archivo.close();
    }else{
        ubicaciones_bien_cargadas = false;
    }

}

void Mapa::validar_coordenada(int &fila, int &columna){
    cout << "\n Ingrese las coordenadas : \n" << endl;
    cout << "\n - Ingrese la fila -> ";
    cin >> fila;
    cout << " - Ingrese la columna -> ";
    cin >> columna;
    cout << "\n";

    while ( fila > cantidad_filas || columna > cantidad_columnas ){
        cout << "\n### Se ingresaron coordenadas fuera del limite : ";
        cout << "filas <= " << cantidad_filas << " - columnas <= " << cantidad_columnas << "###\n" << endl;

        cout << "\n Ingrese la fila -> ";
        cin >> fila;
        cout << " Ingrese la columna -> ";
        cin >> columna;
        cout << "\n";
    }
}

bool Mapa::aceptar_condiciones(){
    bool acepto = false;
    char opcion;
    cout << "\n Desea realizar la operacion ? ( s/n ) : ";
    cin >> opcion;
    cout << "\n";

    if ( opcion == 's'){
        acepto = true;
    }

    return acepto;
}

// --------------- EDIFICIOS POSIBLES --------------------------------------------------
void Mapa::cargar_edificios(){
ifstream nuevo_archivo;
    nuevo_archivo.open(ARCHIVO_EDIFICIO);

    if(nuevo_archivo.is_open()){


        string nombre_edificio, segundo_nombre, cantidad_piedra, cantidad_madera, cantidad_metal, maximo;

        int piedra, madera, metal, maximo_construir;

        Edificio * nuevo_edificio;

        while (nuevo_archivo >> nombre_edificio){
            if ( nombre_edificio == "planta"){
                nuevo_archivo >> segundo_nombre;
                nuevo_archivo >> cantidad_piedra;
                nuevo_archivo >> cantidad_madera;
                nuevo_archivo >> cantidad_metal;
                nuevo_archivo >> maximo;

                nombre_edificio += " " + segundo_nombre;

            } else {
                nuevo_archivo >> cantidad_piedra;
                nuevo_archivo >> cantidad_madera;
                nuevo_archivo >> cantidad_metal;
                nuevo_archivo >> maximo;

            }

            piedra = stoi(cantidad_piedra);
            madera = stoi(cantidad_madera);
            metal = stoi(cantidad_metal);
            maximo_construir = stoi(maximo);

            if (nombre_edificio == ASERRADERO){

                nuevo_edificio = new Aserradero(piedra, madera, metal, maximo_construir);

            }
            else if ( nombre_edificio == ESCUELA){

                nuevo_edificio = new Escuela( piedra, madera, metal, maximo_construir);

            }
            else if ( nombre_edificio == FABRICA){

                nuevo_edificio = new Fabrica( piedra, madera, metal, maximo_construir);

            }
            else if ( nombre_edificio == MINA){

                nuevo_edificio = new Mina( piedra, madera, metal, maximo_construir);

            }
            else if ( nombre_edificio == OBELISCO){

                nuevo_edificio = new Obelisco( piedra, madera, metal, maximo_construir);

            }
            else if ( nombre_edificio == PLANTA_ELECTRICA){

                nuevo_edificio = new Planta_electrica( piedra, madera, metal, maximo_construir);

            }
            agregar_edificio(nuevo_edificio);

        }

        nuevo_archivo.close();
    
    }else{
        cantidad_edificios= ERROR;
    }
}

void Mapa::agregar_edificio(Edificio * nuevo_edificio){
    int cantidad_vieja = cantidad_edificios;

    Edificio ** vector_edificio = new Edificio * [ cantidad_vieja + 1];     

    for ( int i = 0; i < cantidad_edificios; i++){
        vector_edificio[i] = edificios_posibles[i];
    }

    vector_edificio[ cantidad_vieja ] = nuevo_edificio;

    if ( cantidad_edificios != 0){
        delete [] edificios_posibles; 
    }

    edificios_posibles = vector_edificio;
    cantidad_edificios++;
}

int Mapa::obtener_cantidad_edificios(){
    return cantidad_edificios;
}

int Mapa::obtener_posicion_edificio(string nombre){
    int pos;
    for ( int i = 0; i < cantidad_edificios ; i++){
        if ( nombre == edificios_posibles[i]->obtener_nombre()){
            pos = i;
        }
    }return pos;
}

Edificio * Mapa::obtener_edificio(int posicion){
    return edificios_posibles[posicion];
}

bool Mapa::existe_el_edificio(string nombre){
    bool existe = false;
    for (int i = 0; i < cantidad_edificios; i++){
        string nombre_buscado = edificios_posibles[i]->obtener_nombre();
        if ( nombre_buscado == nombre ){
            existe = true;
        }
    }

    return existe;
}

bool Mapa::supera_maximo(string nombre){
    bool supera_max = true;
    int maximo , construidos, restantes;
    for ( int i = 0; i < cantidad_edificios ; i++){

        Edificio * edificio_buscado = obtener_edificio(i);
        string nombre_edificio = edificio_buscado->obtener_nombre();

        if ( nombre_edificio == nombre ){

            maximo = edificio_buscado->obtener_maximo_construir();
            construidos = edificio_buscado->obtener_cantidad_construidos();

            restantes = maximo - construidos;

            if ( restantes > 0 ){
                supera_max = false;
            } 
        }
        
    }

    return supera_max;
}

// -------------- DIVISION PUNTO POR PUNTO : MENU -------------------------------

void Mapa::construir_edificio_nombre(){

    string nombre_nuevo;
    cout << "\n -> Ingrese el nombre del nuevo edificio que desea construir : ";
    cin.ignore();
    getline(cin , nombre_nuevo);

    bool existe_edificio = existe_el_edificio(nombre_nuevo);

    if ( existe_edificio ){

        realizar_construccion(nombre_nuevo);

    } else {
        cout << "\n El edificio buscado NO existe . \n" << endl;
    }
}

void Mapa::realizar_construccion(string nombre_nuevo){

        int pos_edificio = obtener_posicion_edificio(nombre_nuevo);

        int piedra_necesaria = obtener_edificio(pos_edificio)->obtener_cantidad_piedra();
        int madera_necesaria = obtener_edificio(pos_edificio)->obtener_cantidad_madera();
        int metal_necesario = obtener_edificio(pos_edificio)->obtener_cantidad_metal();
        int maximo = obtener_edificio(pos_edificio)->obtener_maximo_construir();

        bool supera_max = supera_maximo(nombre_nuevo);
        bool alcanzan_materiales = usuario_inventario->alcanzan_materiales(piedra_necesaria, madera_necesaria, metal_necesario);

        if ( !supera_max){
            if (alcanzan_materiales){
                if ( aceptar_condiciones() ){

                    int fila , columna;
                    cout << "\n ### En esta seccion podra CONSTRUIR un EDIFICIO : ###\n" << endl;
                    validar_coordenada( fila, columna);

                    if ( mapa[fila][columna]->obtener_nombre() == "T"){
                        
                        bool existe_edificio_construido = mapa[fila][columna]->existe_edificio();
                        if ( ! existe_edificio_construido ){
                            mapa[fila][columna]->agregar_edificio(nombre_nuevo, piedra_necesaria, madera_necesaria, metal_necesario, maximo);
                            obtener_edificio(pos_edificio)->sumar_cantidad();
                            usuario_inventario->utilizar_materiales(piedra_necesaria, madera_necesaria, metal_necesario);
                            cout << "\n ¡ FELICITACIONES : El edificio " << nombre_nuevo << " fue creado exitosamente ! \n" << endl;
                        }else{
                            cout << "\n El casillero ya contiene un edificio .\n" << endl;
                        }
                    }else{
                        cout <<"\nEste no es un terreno por lo que no se pueden construir edificios en el\n"<<endl;
                        mapa[fila][columna]->mostrar_casillero();
                        cout<<endl;
                    }
                }
            }
            else{
                cout<<"\n No se puede construir el edificio de tipo " << nombre_nuevo << " ya que no alcanzan los materiales para construirlo.\n"<<endl;
            }
        }else {
            cout << "\n No se pueden construir mas " << nombre_nuevo << " ya que supera el maximo permitido. \n" << endl; 
        }

}

void Mapa::listar_edificios_construidos(){
    cout << "\n";
    cout << "\t\t###   Listado de los edificio construidos :   ### " << endl;
    cout << "\nOrden de los elementos : " << endl;
    cout << " -> nombre : cantidad construidos " << endl;
    cout << " - coordenada \n - coordenada\n ..." << endl;
    cout << "\n" ;
    cout << "___________________________________________________" << endl;
    cout << "\n" ;
    for (int i = 0; i < obtener_cantidad_edificios(); i++){

        int cantidad_constuidos = obtener_edificio(i)->obtener_cantidad_construidos();
        string nombre_edificio = obtener_edificio(i)->obtener_nombre();

        if ( cantidad_constuidos > 0 ){

            cout << " -> "<< nombre_edificio << " : " << cantidad_constuidos << endl;
            mostrar_coordenadas(nombre_edificio);

        }

    }
    cout << "\n" ;
    cout << "___________________________________________________" << endl;
    cout << "\n" ;
}

void Mapa::listar_todos_edificios(){
    cout << "\n";
    cout << "\t\t###   Listado de todos los edificios :   ###" << endl;
    cout << "\nOrden de los elementos :  " << endl;
    cout << "\n -> nombre / piedra / madera / metal / cuantos puedo construir " << endl;
    cout << "_________________________________________________________________" << endl;
    for ( int i = 0; i < cantidad_edificios; i++){
        cout << "\n";
        cout << " -> " << edificios_posibles[i]->obtener_nombre() << " "
        << edificios_posibles[i]->obtener_cantidad_piedra() << " "
        << edificios_posibles[i]->obtener_cantidad_madera() << " "
        << edificios_posibles[i]->obtener_cantidad_metal() << " "
        << edificios_posibles[i]->obtener_cuantos_puedo_construir() << endl;
        cout << "\n";
        cout << "_________________________________________________________________" << endl;
    }
    cout << "\n";
}

void Mapa::mostrar_coordenadas(string nombre){
    for ( int i = 0; i < cantidad_filas; i++){
        for ( int j = 0 ; j < cantidad_columnas; j++){
            
            bool es_construible = "T" == mapa[i][j]->obtener_nombre();
            if ( es_construible ){
                mapa[i][j]->mostrar_coordenadas_edificio(nombre);
            }
        }
    }
}

void Mapa::demoler_edificio(){

    cout << "\n\t\t ###   En esta seccion podra DEMOLER un EDIFICIO :   ###" << endl;
    
    cout << "\n";
    int fila, columna;
    cout << " Ingrese las coordenadas del edificio a demoler : \n" << endl;

    validar_coordenada( fila, columna);

    string nombre_edificio = mapa[fila][columna]->obtener_nombre_edificio();

    if ( nombre_edificio != ""){

        if ( aceptar_condiciones() ){

            obtengo_materiales_elimino_edificio(nombre_edificio, fila, columna);
            cout << "\n\t\t ###   El edificio : " << nombre_edificio << ", ha sido DEMOLIDO exitosamente !   ###\n" << endl;

        }

    } else {
        cout << "\n En la coordenada ingresada no existe ningun edificio ...\n" << endl;
    }


}

void Mapa::obtengo_materiales_elimino_edificio(string nombre_edificio, int fila, int columna){

    int mitad_piedra, mitad_madera, mitad_metal;

    for (int i = 0; i < obtener_cantidad_edificios(); i++){

        string edificio_en_lista = obtener_edificio(i)->obtener_nombre();

        if (edificio_en_lista == nombre_edificio){
            obtener_edificio(i)->restar_cantidad();

            mitad_piedra = obtener_edificio(i)->obtener_mitad_piedra();
            mitad_madera = obtener_edificio(i)->obtener_mitad_madera();
            mitad_metal = obtener_edificio(i)->obtener_mitad_metal();

        }

    }

    devolver_materiales(mitad_piedra, mitad_madera, mitad_metal);

    mapa[fila][columna]->eliminar_edificio();
}

void Mapa::devolver_materiales(int piedra_obtenida, int madera_obtenida, int metal_obtenida){

    cout << "\n------------------------------\n" << endl;
    cout << "\nMateriales obtenidos \n" << endl;
    cout << PIEDRA << " : " << piedra_obtenida << endl;
    cout << MADERA <<" : " << madera_obtenida << endl;
    cout << METAL << " : " << metal_obtenida << endl;
    cout << "\n------------------------------\n" << endl;

    usuario_inventario->devolver_materiales(piedra_obtenida, madera_obtenida, metal_obtenida);

}

void Mapa::mostrar_mapa(){
    cout << " -------------------------------------------------------------------------- " << endl;
    cout << "\n";
    for (int i = 0; i < cantidad_filas ; i++){
        for ( int j = 0; j < cantidad_columnas; j++){
        cout << "  ";
           cout << mapa[i][j]->obtener_nombre()
                << mapa[i][j]->obtener_diminutivo_edificio()
                << mapa[i][j]->obtener_diminutivo_material()
                << "\t";
        }
        cout << "\n" << endl;
    }
    cout << "\n";
    cout << " -------------------------------------------------------------------------- " << endl;
    cout << "\n";

}

void Mapa::consultar_coordenada(){
    int fila , columna;

    validar_coordenada( fila, columna);

    mapa[fila][columna]->mostrar_casillero() ;
    cout << "\n";
}

void Mapa::mostrar_inv(){
    usuario_inventario->mostrar_inventario();
}

void Mapa::recolectar_recursos_producidos(){
    int piedra = 0;
    int madera = 0;
    int metal = 0;
    int cantidad_edificios, total_brindado , cantidad_construidos, cantidad_a_brindar;
    string nombre_edificio;

    cantidad_edificios = obtener_cantidad_edificios();

    for ( int i = 0; i < cantidad_edificios; i++){

        Edificio * edificio_solicitado = obtener_edificio(i); 

        nombre_edificio = edificio_solicitado->obtener_nombre();
        cantidad_construidos = edificio_solicitado->obtener_cantidad_construidos();
        cantidad_a_brindar = edificio_solicitado->obtener_cantidad_brindada();

        total_brindado = cantidad_construidos * cantidad_a_brindar;

        if ( nombre_edificio == MINA){

            piedra += total_brindado;

        } else if ( nombre_edificio == ASERRADERO){
            
            madera += total_brindado;

        } else if ( nombre_edificio == FABRICA){

            metal += total_brindado;
        }
    }

    devolver_materiales(piedra, madera, metal);
}

int Mapa::generar_numero_random(int min, int max){
    int range = max + 1  - min;  
    return min + ( rand() % range);

}

void Mapa::consultar_material_a_colocar(int &cant_gen_piedras, int &cant_gen_maderas, int &cant_gen_metales, string &material_a_colocar ){
    if (cant_gen_piedras){
        material_a_colocar = "piedra";
        cant_gen_piedras --;

    } else if (cant_gen_maderas){
        material_a_colocar = "madera";
        cant_gen_maderas --;

    } else{
        material_a_colocar = "metal";
        cant_gen_metales --;
    }
}

void Mapa::mostrar_alerta_materiales_no_colocados(int materiales_restantes, int cant_gen_piedras, int cant_gen_maderas, int cant_gen_metales){
    cout <<endl<<"No se pudieron colocar los siguientes " <<materiales_restantes 
    << " materiales porque los casilleros transitables ya estan todos ocupados: "<<endl;
    if (cant_gen_piedras > 0){
        cout<<cant_gen_piedras <<" unidades de piedra"<<endl;
    }
    if (cant_gen_maderas > 0){
        cout<<cant_gen_maderas <<" unidades de madera" <<endl;
    }
    if (cant_gen_metales > 0){
        cout<<cant_gen_metales <<" unidades de metal " <<endl;
    }
}


void Mapa::colocar_materiales_llovidos(int tot_materiales_gen, int cant_gen_piedras, int cant_gen_maderas, int cant_gen_metales){

    string material_a_colocar = "";
    int materiales_restantes = tot_materiales_gen;

    while (materiales_restantes >0 && total_casilleros >0){

        consultar_material_a_colocar(cant_gen_piedras, cant_gen_maderas, cant_gen_metales, material_a_colocar);

        int pos_coordenada =  generar_numero_random( 0, total_casilleros - 1);
        
        int fila =  obtener_casillero_vector_casilleros_lluvia(pos_coordenada) ->obtener_fila() ;
        int columna =  obtener_casillero_vector_casilleros_lluvia(pos_coordenada) ->obtener_columna() ;

        mapa[fila][columna] -> agregar_material(material_a_colocar, 1);

        cout <<"1 unidad de " << material_a_colocar << " en " <<"("<< fila << ","  << columna <<")" << endl;

        sacar_casillero(pos_coordenada);

        materiales_restantes --;
    }

    if ( materiales_restantes != 0 ){
        mostrar_alerta_materiales_no_colocados(materiales_restantes, cant_gen_piedras, cant_gen_maderas, cant_gen_metales);
    }

}



Casillero_transitable* Mapa :: obtener_casillero_vector_casilleros_lluvia ( int pos) {
	return vector_casilleros_lluvia[pos];
}


void Mapa::sacar_casillero(int posicion_numero_a_sacar){
    
    if (total_casilleros > 1) {

        //mando el q quiero elimnar a la ult pos y lo intercambio con ese
        swap_casillero(total_casilleros - 1, posicion_numero_a_sacar);
        
        Casillero_transitable **vector_aux_casilleros_lluvia = new Casillero_transitable*[total_casilleros - 1];

        for(int i = 0; i < total_casilleros - 1; i++){
            vector_aux_casilleros_lluvia[i] = vector_casilleros_lluvia[i];
            //Como esta en la ult pos nunca lo copio!
        }
        
        delete vector_casilleros_lluvia[total_casilleros - 1];
        delete[] vector_casilleros_lluvia;


        vector_casilleros_lluvia = vector_aux_casilleros_lluvia;

    }else{
        delete vector_casilleros_lluvia[total_casilleros - 1];
        delete[] vector_casilleros_lluvia;
    }

    total_casilleros --;
}

void Mapa::swap_casillero(int posicion_1, int posicion_2){
    Casillero_transitable *aux = vector_casilleros_lluvia[posicion_1];
    vector_casilleros_lluvia[posicion_1] = vector_casilleros_lluvia[posicion_2]; //mando al final (pos_1) el quiero eliminar(pos_2)
    vector_casilleros_lluvia[posicion_2] = aux; //el ultimo lo pongo en donde estaba el que quiero eliminar
}

void Mapa :: agregar_casillero_a_vector_casilleros_lluvia (Casillero_transitable *casillero, int tam_nuevo, int pos ) {
    
    Casillero_transitable** vector_aux = new Casillero_transitable*[tam_nuevo];

    for (int i = 0; i<pos; i++){
        vector_aux[i] = vector_casilleros_lluvia[i];
    }

    vector_aux[pos] = casillero;

    if (total_casilleros != 0){
        delete [] vector_casilleros_lluvia;
        }

    total_casilleros = tam_nuevo;
    vector_casilleros_lluvia = vector_aux;
}

void Mapa::cargar_vector_casilleros_lluvia_con_casileros_permitidos(){
    
    int pos = 0;
    Casillero_transitable *casillero_aux;

    for ( int i = 0; i < cantidad_filas; i++){
        for ( int j = 0; j < cantidad_columnas ; j++){
            if (mapa[i][j] -> obtener_nombre() =="C" && !( mapa[i][j] -> existe_material() ) ){    
                
                casillero_aux  = new Casillero_transitable(i, j);

                agregar_casillero_a_vector_casilleros_lluvia(casillero_aux,pos+1, pos);
                
                pos+=1;
            }
        }
    }
}

void Mapa::ejecutar_lluvia(int tot_materiales_gen, int cant_gen_piedras, int cant_gen_maderas, int cant_gen_metales){

    cargar_vector_casilleros_lluvia_con_casileros_permitidos();

    colocar_materiales_llovidos(tot_materiales_gen, cant_gen_piedras, cant_gen_maderas, cant_gen_metales);
    
    int total_cas = total_casilleros;

    for ( int i = 0; i < total_cas; i++){
        delete vector_casilleros_lluvia[i];
        total_casilleros--;
    }
    if (total_cas !=0){
        delete [] vector_casilleros_lluvia;
        vector_casilleros_lluvia = nullptr;
    }
}

void Mapa :: lluvia_recursos(){

    srand( (unsigned)time(0) );

    int cant_gen_piedras = generar_numero_random(1,2); 
    int cant_gen_maderas = generar_numero_random(0,1);
    int cant_gen_metales = generar_numero_random(2,4);
    
    int tot_materiales_gen = cant_gen_piedras + cant_gen_maderas + cant_gen_metales;
    
        
    cout << "Han llovido en el mapa " << tot_materiales_gen << " unidades de materiales: " <<endl
    <<cant_gen_piedras <<" unidades de piedra"<<endl
    <<cant_gen_maderas <<" unidades de madera" <<endl
    <<cant_gen_metales <<" unidades de metal " <<endl<<endl
    <<"en las siguientes posiciones: "<< endl;

    ejecutar_lluvia(tot_materiales_gen,cant_gen_piedras, cant_gen_maderas, cant_gen_metales);

    cout <<endl;
}

// -------------- FINALIZA PUNTOS DEL MENU -------------------------------

Mapa::~Mapa(){

    if (mapa_bien_cargado && ubicaciones_bien_cargadas){
        ofstream archivo_ubicaciones(ARCHIVO_UBICACIONES);
        for ( int i = 0; i < cantidad_filas; i++){
            for ( int j = 0; j < cantidad_columnas ; j++){
                if (mapa[i][j] ->existe_edificio() ){
                    archivo_ubicaciones << mapa[i][j] ->obtener_nombre_edificio() << " ("
                    << i << ", " << j << ")" << endl;
                }
                if ( mapa[i][j] -> existe_material() ){ 
                    archivo_ubicaciones << mapa[i][j] -> obtener_nombre_material() <<" ("
                    << i << ", " << j << ")" << endl;
                }
            }
        }
    }

    if (mapa_bien_cargado){
        for ( int i = 0; i < cantidad_filas; i++){
            for ( int j = 0; j < cantidad_columnas ; j++){
                
                delete mapa[i][j];
            }
            delete [] mapa[i];
        }
        delete [] mapa;
        this->mapa = 0;
    }

    int total = cantidad_edificios;
    for ( int i = 0; i < total; i++){

        delete edificios_posibles[i];
        cantidad_edificios--;
    }
    delete [] edificios_posibles;
    edificios_posibles = nullptr;

    delete usuario_inventario;
    usuario_inventario = 0;
    
}