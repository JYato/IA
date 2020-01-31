#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <cmath>
#include <climits>

using namespace std;

int totalpob = 100;
double probmutacion = 0.05;
int numgeneraciones = 1000;


struct Ciudad{
    double x, y;
    Ciudad(){}
    Ciudad(double _x, double _y): x(_x), y(_y) {}
    double dist(Ciudad p){
        return hypot(p.x - x, p.y - y );
    }
};

/*float distancia_euclediana(Ciudad* A, Ciudad* B){
    return sqrt(pow(abs(B->x - A->x), 2) + pow(abs(B->y - A->y), 2));
}

float distancia_euclediana(Ciudad A, Ciudad B)
{
	return sqrt(pow(abs(B.x - A.x), 2) + pow(abs(B.y - A.y), 2));
}
*/


int ELITE = 1;
class Poblacion{
    vector<Ciudad> &Ciudads;
    vector<vector<int> > cromosomas;
    vector<double> prob;
    int n;

public:
    Poblacion(vector<Ciudad> &_Ciudads): Ciudads(_Ciudads){}
    Poblacion(int cromosomasize, vector<Ciudad> &_Ciudads): n(cromosomasize), Ciudads(_Ciudads){}
    Poblacion(int popuSize, int cromosomasize, vector<Ciudad> &_Ciudads): Ciudads(_Ciudads) {
        n = cromosomasize;
        cromosomas.clear();
        vector<int> ind;
        for(int i = 0; i < n; i++) ind.push_back(i);

        for(int i = 0; i < popuSize; i++){
            random_shuffle ( ind.begin(), ind.end() );
            vector<int> newRoute = ind;
            cromosomas.push_back( newRoute );
        }
    }

    vector<vector<int> > getcromosomas() const{
        return cromosomas;
    }

    int getcromosomasize() const{
        return n;
    }

    vector<double> getProb() const{
        return prob;
    }

    void operator=(const Poblacion& poblac){
        cromosomas = poblac.getcromosomas();
        n = poblac.getcromosomasize();
        prob = poblac.getProb();
    }

    void cruzamiento(vector<int> &p1, vector<int> &p2, vector<int> &h1, vector<int> &h2){
        h1.clear();
        h2.clear();
        int a = rand()%(n-1), b = rand()%(n-1) ;
        if(a > b){
            swap(a,b);
        }

        bool oc1[1000];
        bool oc2[1000];
        memset(oc1, 0, sizeof(oc1));
        memset(oc2, 0, sizeof(oc2));

        for(int i = 0; i < n; i++){
            h1.push_back( -1 );
            h2.push_back( -1 );
        }

        for(int i = a+1; i <= b; i++){
            h1[i] = p1[i];
            oc1[ p1[i] ] = 1;
            h2[i] = p2[i];
            oc2[ p2[i] ] = 1;
        }
        int i1 = (b+1)%n, i2 = (b+1)%n;
        for(int i = 0; i < n; i++){
            if( !oc1[ p2[ (b+1+i)%n ] ] ){
                h1[i1] = p2[ (b+1+i)%n ];
                i1 = (i1+1)%n;
            }
            if( !oc2[ p1[ (b+1+i)%n ] ] ){
                h2[i2] = p1[ (b+1+i)%n ];
                i2 = (i2+1)%n;
            }
        }

        double r;
        r = (double)rand()/(double)RAND_MAX;
        if( r <= probmutacion ){
            a = rand()%(n);
            b = rand()%(n);
            swap(h1[a], h1[b]);
        }

        r = (double)rand()/(double)RAND_MAX;
        if( r <= probmutacion ){
            a = rand()%(n);
            b = rand()%(n);
            swap(h2[a], h2[b]);
        }
    }

    Poblacion evolucionar(){
        Poblacion pSiguiente = Poblacion(n, Ciudads);
        order();
        int cant;
        for(cant = 0; cant < ELITE && cant < (int)cromosomas.size(); cant++){
            vector<int> nRuta = cromosomas[cant];
            pSiguiente.addRoute( nRuta );
        }

        while(ELITE < (int)cromosomas.size()){
            vector<int> h1,h2;
            double r;

            r = (double)rand()/(double)RAND_MAX;
            int ind1 = lower_bound( prob.begin(), prob.end(), r) - prob.begin();

            int ind2 = ind1;
            while( ind2 == ind1){
                r = (double)rand()/(double)RAND_MAX;
                ind2 = lower_bound( prob.begin(), prob.end(), r) - prob.begin();
            }

            cruzamiento( cromosomas[ind1], cromosomas[ind2], h1, h2 );
            pSiguiente.addRoute( h1 );
            if( pSiguiente.getSize() == (int)cromosomas.size() ) break;
            pSiguiente.addRoute( h2 );
            if( pSiguiente.getSize() == (int)cromosomas.size() ) break;
        }
        return pSiguiente;
    }

    double getNoaptitud(int i){
        if( i >= 0 && i < cromosomas.size() ){
            double noaptitud = 0;
            for(int j = 0; j < n; j++){
                noaptitud += Ciudads[cromosomas[i][j]].dist( Ciudads[cromosomas[i][(j+1)%n] ] )  ;
            }
            return noaptitud;
        }
        else{
            return -1.0;
        }
    }

    void order(){
        vector<pair<double,int> > ind;
        double totalaptitud = 0;
        for(int i = 0; i < (int)cromosomas.size(); i++){
            double noaptitud = getNoaptitud(i);
            double aptitud = 1.0/(double)noaptitud;
            totalaptitud += aptitud;
            ind.push_back( make_pair(-aptitud, i) );
        }
        sort( ind.begin(), ind.end() );
        vector<vector<int> > rutas2;
        double ac = 0;
        prob.clear();
        for(int i = 0; i < (int)cromosomas.size(); i++){
            rutas2.push_back(cromosomas[ ind[i].second ]);
            ac+= -ind[i].first;
            prob.push_back( ac/totalaptitud );
        }
        cromosomas = rutas2;
    }

    void addRoute(vector<int> &nRuta){
        cromosomas.push_back(nRuta);
    }

    int getSize(){
        return (int)cromosomas.size();
    }

    vector<Ciudad> getprim(){
        vector<Ciudad> res;
        for(int i = 0; i < n; i++){
            res.push_back(Ciudads[cromosomas[0][i]]);
        }
        return res;
    }

    vector<int> getprimIndexes(){
        return cromosomas[0];
    }

    Ciudad getCiudad(int i){
        return Ciudads[i];
    }

};


/**class Cromosoma{
public:
    vector<int> ciudades;///guardo los ids de las ciudades
    Cromosoma(){
        vector<int>cities(cant_ciudades);
        for(int i = 0; i < cant_ciudades; i++)cities[i] = i;
        ciudades = cities;
    }
    Cromosoma(vector<int> cities){
        ciudades = cities;
    }
    float aptitud(){
        float apt = 0;
        for(int i = 1; i < cities_global.size(); i++){
            float temp = distancia_euclediana(cities_global[ciudades[i-1]], cities_global[ciudades[i]]);
            apt += temp;
        }
        return apt;
    }

    //por inversion
    void mutacion(int pos, int cant){
        reverse(ciudades.begin()+pos, ciudades.begin()+pos+cant);
    }
    Cromosoma operator =(const Cromosoma& cr){
        ciudades = cr.ciudades;
        return *this;
    }
};*/

/*Cromosoma* cruzamiento_ciclos1(Cromosoma padre1, Cromosoma padre2){
    int id1, id2, id3, posmid;
    posmid = cant_ciudades/2;
    id1 = padre1.ciudades[0];
    id2 = padre2.ciudades[0];
    int posmod;
    for(posmod = 0; id2 != padre1.ciudades[posmod]; posmod++){}
    id3 = padre1.ciudades[posmod];
    vector<int> dehijo1(cant_ciudades);
    for(int i = 1, j = 1; i < cant_ciudades-1; i++, j++){//i del hijo, j del padre2
        if(i == posmid){
            i++;
        }

    }
    Cromosoma* hijo1 = new Cromosoma();
}*/

//algoritmo genetico
vector<Ciudad> genetico_viajero(vector<Ciudad> &Ciudads){
    int n = (int)Ciudads.size();

    Poblacion ACTUAL(totalpob, n, Ciudads);

    ACTUAL.order();
    vector<Ciudad> result = ACTUAL.getprim();
    vector<int> rIndexes = ACTUAL.getprimIndexes();

    //cout << "Ruta con más aptitud en Poblacion inicial - distancia:"<< ACTUAL.getNoaptitud(0)<< endl;
    cout << "Nodos:" << endl;
    for(int i = 0; i < result.size(); i++) {
        cout<< rIndexes[i] << " : "<< result[i].x << " " << result[i].y << endl;
    }

    cout << endl << "\nGeneracion" << "\t"<<"distancia acumulada" << endl;

    for(int iter = 0; iter < numgeneraciones; iter++){
        Poblacion siguiente = ACTUAL.evolucionar();
        ACTUAL = siguiente;
        ACTUAL.order();
        if(iter%100 == 0 || iter == numgeneraciones - 1) cout << (iter+1)/100 <<"\t"<< (int)ACTUAL.getNoaptitud(0) << endl;
    }
    ACTUAL.order();
    return ACTUAL.getprim();
}

vector<Ciudad> Ciudads;

void crear_puntos(int cant)
{
    srand (unsigned(time(NULL)));
    for(int i = 0; i < cant; i++){
        float x = rand()%100 + 1;
        float y = rand()%100 + 1;
        Ciudads.push_back(Ciudad(x,y));
    }
}

int main(){
    srand (unsigned(time(NULL)));
    //vector<Ciudad> Ciudads;
    fstream inFile("pruebasica3.txt",std::ios_base::in);

    double x, y, z;//z esta de mas
    int cant = 1000;
    crear_puntos(cant);
    /*while(inFile){
        inFile >> x >> y >> z;
        Ciudads.push_back(Ciudad(x,y));
	}*/
    vector<Ciudad> resultado = genetico_viajero(Ciudads);

    cout << endl << "Ruta con mejor aptitud:"<< endl;
    for(int i = 0; i < resultado.size(); i++) {
        cout<< resultado[i].x << " " << resultado[i].y << endl;
    }
    return 0;
}

