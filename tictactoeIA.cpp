#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Nodo{
public:
    int valor, profundidad;
    bool maxmini;///false = max, true = min
    vector<string> tablero;
    Nodo* padre;
    vector<Nodo*> hijos;
    int pos_modificada;
    Nodo(int prof = 1, bool maxmini_ = false){
        valor = 0;
        maxmini = maxmini_;
        profundidad = prof;
        padre = NULL;
        /*if(profundidad > 0){
            crear_hijos();
        }
        else{
            funcion_estocastica();
        }*/
    }
    Nodo(vector<string> tablerito, int prof, bool maxmini_){
        valor = 0;
        maxmini = maxmini_;
        profundidad = prof;
        padre = NULL;
        tablero = tablerito;
    }
    void actuar(){

            //cout << "aqui" << endl;
            //print_tablero();
            crear_hijos();
            //funcion_estocastica();
    }

    void contar_espacios_libres(vector<int>& libres){
        //vector<int> libres;
        for(int i = 0; i < 9; i++){
            if(tablero[i] == "_"){
                libres.push_back(i);
            }
        }
        //cout << "tamanio: " << libres.size() << endl;
    }

    void crear_hijos(){
        vector<int> libres; //= contar_espacios_libres();
        contar_espacios_libres(libres);
        //cout << "aqui" << endl;}
        //cout << profundidad << endl;
        for(int i = 0; i < libres.size() && profundidad > 1; i++){
            Nodo* nuevo = new Nodo(profundidad-1, !maxmini);
            nuevo->padre = this;
            nuevo->tablero = this->tablero;
            //nuevo->print_tablero();
            //cout << maxmini;
            //cout << nuevo->maxmini;
            //cout << libres.size();
            if(!maxmini){///si el actual es maximo, entonces el hijo debe escribir un circulo de humano
                //cout << "aqui";
                ///cout << libres[i] << endl;
                nuevo->tablero[libres[i]] = "o";
                nuevo->pos_modificada = libres[i];
            }
            else{
                //cout << "aqui";
                nuevo->tablero[libres[i]] = "x";
            }
            //cout << "aqui";
            //nuevo->print_tablero();
            hijos.push_back(nuevo);
            hijos[i]->actuar();
        }
    }

    int maximo(){
        int temp = -1000;
        int pos;
        for(int i = 0; i < hijos.size(); i++){
            if(temp < hijos[i]->valor){
                temp = hijos[i]->valor;
                pos = hijos[i]->pos_modificada;
                //cout << hijos[i]->valor << " ";
            }
        }
        //cout << endl;
        this->valor = temp;
        return pos;
    }

    int minimo(){
        int temp = 1000;
        for(int i = 0; i < hijos.size(); i++){
            if(temp > hijos[i]->valor){
                temp = hijos[i]->valor;
            }
        }
        this->valor = temp;
        return temp;
    }

    void posicion_poner_IA(){
        if(hijos.size() == 0){
            this->funcion_estocastica();
            //cout << valor << endl;
            return;
        }
        for(int i = 0; i < hijos.size(); i++){
            hijos[i]->posicion_poner_IA();
        }
        if(!maxmini){///=false
            maximo();
        }
        else{
             minimo();
        }
        //return pos_modificada;
    }


    int funcion_estocastica(){//solo aplicar a las hojas
        for(int i = 0; i < 3; i++){
            int j = i * 3;
            if((tablero[j] == "x" || tablero[j] == "_") && (tablero[j+1] == "x" || tablero[j+1] == "_") && (tablero[j+2] == "x" || tablero[j+2] == "_")){
                valor++;
            }
            else if((tablero[j] == "o" || tablero[j] == "_") && (tablero[j+1] == "o" || tablero[j+1] == "_") && (tablero[j+2] == "o" || tablero[j+2] == "_")){
                valor--;
            }
            if((tablero[i] == "x" || tablero[i] == "_") && (tablero[i+3] == "x" || tablero[i+3] == "_") && (tablero[i+6] == "x" || tablero[i+6] == "_")){
                valor++;
            }
            else if((tablero[i] == "o" || tablero[i] == "_") && (tablero[i+3] == "o" || tablero[i+3] == "_") && (tablero[i+6] == "o" || tablero[i+6] == "_")){
                valor--;
            }
        }
        if((tablero[0] == "x" || tablero[0] == "_") && (tablero[4] == "x" || tablero[4] == "_") && (tablero[8] == "x" || tablero[8] == "_")){
            valor++;
        }
        else if((tablero[0] == "o" || tablero[0] == "_") && (tablero[4] == "x" || tablero[4] == "_") && (tablero[8] == "x" || tablero[8] == "_")){
            valor--;
        }
        if((tablero[2] == "x" || tablero[2] == "_") && (tablero[4] == "x" || tablero[4] == "_") && (tablero[6] == "x" || tablero[6] == "_")){
            valor++;
        }
        else if((tablero[2] == "o" || tablero[2] == "_") && (tablero[4] == "x" || tablero[4] == "_") && (tablero[6] == "x" || tablero[6] == "_")){
            valor--;
        }
    }

    void print_tablero(){
        cout << endl;
        for(int i = 0; i < 9; i += 3){
            cout <<  tablero[i] << " | " << tablero[i+1] << " | " << tablero[i+2] << endl;
        }
    }
};
///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// return 0 = nadie, 1 = ia , 2 = humano
int evaluar(vector<string> tablero, int pos_eval){
    switch(pos_eval){
    case 0:
        if((tablero[0] == "x" && tablero[1] == "x" && tablero[2] == "x") || (tablero[0] == "x" && tablero[3] == "x" && tablero[3] == "x") || (tablero[0] == "x" && tablero[4] == "x" && tablero[8] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[1] == "o" && tablero[2] == "o") || (tablero[0] == "o" && tablero[3] == "o" && tablero[3] == "o") || (tablero[0] == "o" && tablero[4] == "o" && tablero[8] == "o")){
            return 2;
        }
        break;

    case 1:
        if((tablero[0] == "x" && tablero[1] == "x" && tablero[2] == "x") || (tablero[1] == "x" && tablero[4] == "x" && tablero[7] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[1] == "o" && tablero[2] == "o") || (tablero[1] == "o" && tablero[4] == "o" && tablero[7] == "o")){
            return 2;
        }
        break;

    case 2:
        if((tablero[0] == "x" && tablero[1] == "x" && tablero[2] == "x") || (tablero[2] == "x" && tablero[4] == "x" && tablero[6] == "x") || (tablero[2] == "x" && tablero[5] == "x" && tablero[8] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[1] == "o" && tablero[2] == "o") || (tablero[2] == "o" && tablero[4] == "o" && tablero[6] == "o") || (tablero[2] == "o" && tablero[5] == "o" && tablero[8] == "o")){
            return 2;
        }
        break;
    case 3:
        if((tablero[0] == "x" && tablero[3] == "x" && tablero[6] == "x") || (tablero[3] == "x" && tablero[4] == "x" && tablero[5] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[3] == "o" && tablero[6] == "o") || (tablero[3] == "o" && tablero[4] == "o" && tablero[5] == "o")){
            return 2;
        }
        break;
    case 4:
        if((tablero[0] == "x" && tablero[4] == "x" && tablero[8] == "x") || (tablero[2] == "x" && tablero[4] == "x" && tablero[6] == "x") || (tablero[3] == "x" && tablero[4] == "x" && tablero[5] == "x") || (tablero[1] == "x" && tablero[4] == "x" && tablero[7] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[4] == "o" && tablero[8] == "o") || (tablero[2] == "o" && tablero[4] == "o" && tablero[6] == "o") || (tablero[3] == "o" && tablero[4] == "o" && tablero[5] == "o") || (tablero[1] == "o" && tablero[4] == "o" && tablero[7] == "o")){
            return 2;
        }
        break;
    case 5:
        if((tablero[2] == "x" && tablero[5] == "x" && tablero[8] == "x") || (tablero[3] == "x" && tablero[4] == "x" && tablero[5] == "x")){
            return 1;
        }
        else if((tablero[2] == "o" && tablero[5] == "o" && tablero[8] == "o") || (tablero[3] == "o" && tablero[4] == "o" && tablero[5] == "o")){
            return 2;
        }
        break;
    case 6:
        if((tablero[0] == "x" && tablero[3] == "x" && tablero[6] == "x") || (tablero[2] == "x" && tablero[4] == "x" && tablero[6] == "x") || (tablero[6] == "x" && tablero[7] == "x" && tablero[8] == "x")){
            return 1;
        }
        else if((tablero[0] == "o" && tablero[3] == "o" && tablero[6] == "o") || (tablero[2] == "o" && tablero[4] == "o" && tablero[6] == "o") || (tablero[6] == "o" && tablero[7] == "o" && tablero[8] == "o")){
            return 2;
        }
        break;
    case 7:
        if((tablero[6] == "x" && tablero[7] == "x" && tablero[8] == "x") || (tablero[1] == "x" && tablero[4] == "x" && tablero[7] == "x")){
            return 1;
        }
        else if((tablero[6] == "o" && tablero[7] == "o" && tablero[8] == "o") || (tablero[1] == "o" && tablero[4] == "o" && tablero[7] == "o")){
            return 2;
        }
        break;
    case 8:
        if((tablero[6] == "x" && tablero[7] == "x" && tablero[8] == "x") || (tablero[2] == "x" && tablero[5] == "x" && tablero[8] == "x") || (tablero[0] == "x" && tablero[4] == "x" && tablero[8] == "x")){
            return 1;
        }
        else if((tablero[6] == "o" && tablero[7] == "o" && tablero[8] == "o") || (tablero[2] == "o" && tablero[5] == "o" && tablero[8] == "o") || (tablero[0] == "o" && tablero[4] == "o" && tablero[8] == "o")){
            return 2;
        }
        break;
    }
    return 0;
}

void print_tablero(vector<string> tablero){
        for(int i = 0; i < 9; i += 3){
            cout <<  tablero[i] << " | " << tablero[i+1] << " | " << tablero[i+2] << endl;
        }
}

///x = ia
///o = humano

int main()
{
    string opcion;
    int profundidad;
    cout << "Que profundidad desea? : "; cin >> profundidad;
    cout << "Quien empezara, la IA o usted?: Escribir 'a' para la IA; 'b', usted: "; cin >> opcion;
    vector<string> tablero(9,"_");
    int win = 0;
    if(opcion == "a"){
        for(int i = 0; i < 5 && win == 0; i++){
            Nodo* game = new Nodo(tablero,profundidad,false);
            //game->tablero = tablero;
            game->posicion_poner_IA();
            int pos = game->maximo();
            tablero[pos] = "x";
            cout << "Movimiento IA: " << endl;
            print_tablero(tablero);
            int pos_humano;
            cout << "En que posicion pondra su ficha?: "; cin >> pos_humano;
            tablero[pos_humano] = "o";
            cout << "Movimiento HUMANO: " << endl;
            print_tablero(tablero);
        }
    }
    else if(opcion == "b"){
        for(int i = 0; i < 4 && win == 0; i++){
            int pos_humano;
            cout << "En que posicion pondra su ficha?: "; cin >> pos_humano;
            tablero[pos_humano] = "o";
            cout << "Movimiento HUMANO: " << endl;
            print_tablero(tablero);
            win = evaluar(tablero,pos_humano); if(win)break;

            Nodo* game = new Nodo(tablero,profundidad,false);
            game->actuar();
            //game->tablero = tablero;
            game->posicion_poner_IA();
            int pos = game->maximo();
            //cout << pos << endl;
            tablero[pos] = "x";
            cout << "Movimiento IA: " << endl;
            print_tablero(tablero);
            win = evaluar(tablero,pos); if(win)break;
        }
    }
    if(win == 1){
        cout << "Gano la IA" << endl;
    }
    else{
        cout << "Gano usted" << endl;
    }

    ///crear un nodo()
    ///posicion 0,1,2,3,4,5,6,7,8
    /**
      0 | 1 | 2
      3 | 4 | 5
      6 | 7 | 8

      _ | _ | _
      _ | _ | _
      _ | _ | _
    */

    return 0;
}
