#include <iostream>
#include <bits/stdc++.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define INF 100000

using namespace std;

class Punto;

class Arista{
public:
    Punto* A;
	Punto* B;
	Arista(Punto* A_, Punto* B_)
	{
		A = A_;
		B = B_;
	};
};

class Punto{
public:
	vector<Arista*>Aristas;
	float x,y,costo;
	bool visitado;
	bool agregado;
	int id;
	Punto(){
		x = 0;
		y = 0;
		visitado = false;
		agregado = false;
	}
	Punto(float x_, float y_)
	{
		x = x_;
		y = y_;
		visitado = false;
		agregado = false;
	}
	Punto operator = (const Punto& Point)
	{
		x = Point.x;
		y = Point.y;
		return *this;
	}
	bool operator == (const Punto& Point) {
		if (x == Point.x && y == Point.y) {
			return true;
		}
		return false;
	}
};

float distancia_puntos(Punto *A, Punto *B)
{
	return sqrt(pow(abs(B->x - A->x), 2) + pow(abs(B->y - A->y), 2));
}
float distancia_puntos(Punto A, Punto B)
{
	return sqrt(pow(abs(B.x - A.x), 2) + pow(abs(B.y - A.y), 2));
}

vector<Punto> Puntos;

void CrearPuntos()
{
    //srand(time(NULL));
	for (int i = 0; i < 200; i++)
	{
		float x = rand() % 100 + 1;
		float y = rand() % 100 + 1;
		Puntos.push_back(Punto(x, y));
		Puntos[i].id = i;
	}
}

int ext_der_inf(vector<Punto> points)
{
	int posicion = 0;
	float max_x = points[0].x;
	float max_y = points[0].y;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].x > max_x && points[i].y < max_y)
		{
			max_x = points[i].x;
			max_y = points[i].y;
			posicion = i;
		}
	}
	return posicion;
}

vector<Arista*> Edges;

void KNN()
{
	float temp = 11;
	for (int i = 0; i < Puntos.size(); i++)
	{
		for(int j = 0; Puntos[i].Aristas.size() < 5; j++)
		{
			for (int h = 0; h < Puntos.size(); h++)
			{
				float dis = distancia_puntos(&Puntos[i], &Puntos[h]);
				if (dis < temp && dis != 0 && Puntos[i].Aristas.size() < 5 && Puntos[h].Aristas.size() < 5)
				{
					Arista* nuevo1 = new Arista(&Puntos[i], &Puntos[h]);
					Arista* nuevo2 = new Arista(&Puntos[h], &Puntos[i]);
					Puntos[i].Aristas.push_back(nuevo1);
					Puntos[h].Aristas.push_back(nuevo2);
					Edges.push_back(nuevo1);
					Edges.push_back(nuevo2);
				}
			}
			if (Puntos[i].Aristas.size() < 5)temp = temp + 5;
		}
	}
}

///------------------------------------------------------------------------------------------------------------------------------------------------
struct Comparer{
    bool operator()(const pair<int, float> &t1, const pair<int, float> &t2) const{
        return t1.second > t2.second;
    }
};

struct Edged{
    Punto *v, *w;
    float peso;

    Edged(Punto *v_, Punto *w_){
        v = v_;
        w = w_;
        peso = distancia_puntos(v_,w_);
    }
    Edged(Punto v_, Punto w_){
        v = new Punto(v_.x, v_.y);
        w = new Punto(w_.x, w_.y);
        peso = distancia_puntos(v,w);
    }
};

class Lista_Adyacencia{
    int Vcnt, Ecnt;
    bool digraph;
public:
    vector<vector<Edged> >adj;
    int cont;
    int V() const{
        return this->Vcnt;
    }
    int E() const{
        return this->Ecnt;
    }
    bool directed() const{return digraph;}
    Lista_Adyacencia(){}
    Lista_Adyacencia(int V, bool digraph = false): Vcnt(V), Ecnt(0), digraph(digraph), adj(V){}
    void Insert(Edged edge){
        adj[edge.v->id].push_back(edge);
        Ecnt++;
    }
    void dijkstra(int partida, int destino){
        vector<int> previo(V(), -1);
	    vector<float> distancia(V(), INF);
		vector<bool> visitado(V(), false);
		priority_queue<pair<int, float>, vector<pair<int,float> >, Comparer> Q;
		//vector<Punto*> corrida_puntos; corrida_puntos.push_back(adj[partida][0].v);
		distancia[partida] = 0;
		Q.push(make_pair(partida,0));
		while(!Q.empty()){
            int temp = Q.top().first, j;
            Q.pop();
            if(temp == destino)break;
            visitado[temp] = true;
            for(int i = 0; i < adj[temp].size(); i++){
                float peso = adj[temp][i].peso;
                if(!visitado[adj[temp][i].w->id] && distancia[temp] + peso < distancia[adj[temp][i].w->id]){
                    distancia[adj[temp][i].w->id] = distancia[temp] + adj[temp][i].peso;
                    previo[adj[temp][i].w->id] = temp;
                    //corrida_puntos.push_back(adj[temp][i].w);
					Q.push(make_pair(adj[temp][i].w->id, distancia[adj[temp][i].w->id]));
                }
            }
		}
		int temp = destino;
		cout << "Este busqueda ciega(Dijkstra) imprime desde el destino hacia atras." << endl;
		cout << "Numeros de nodos: " << endl << destino << " ";
		while(previo[temp] != previo[partida]){
            cout << previo[temp] << " ";

            temp = previo[temp];
		}
		temp = destino;
		cout << endl << "Coordenadas:"  << endl;
		while(previo[temp] != previo[partida]){
            cout << adj[temp][0].v->x << "," << adj[temp][0].v->y << " | ";
            temp = previo[temp];
		}
		//temp = destino;
        cout << endl << "Distancia dijkstra: " << distancia[destino] << endl;
		//cout << destino << endl;
		/*while(previo[temp] != previo[partida]){
            cout << previo[temp] << endl;
            temp = previo[temp];
		}*/
		previo.clear();
    }
};
///------------------------------------------------------------------------------------------------------------------------------------------------
int ext_izq_sup(vector<Punto> points)
{
	int posicion = 0;
	float max_x = points[0].x;
	float max_y = points[0].y;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].x < max_x&& points[i].y > max_y)
		{
			max_x = points[i].x;
			max_y = points[i].y;
			posicion = i;
		}
	}
	return posicion;
}

class Asterisco
{
public:
	Asterisco(){}
	void buscar(Punto& partida, Punto& destino){
	    cout << endl << "---------------------------------------------------------------------------------" << endl;
		cout << "A asterisco imprime desde el punto de partida hasta el destino." << endl;
		Punto* temp;
		Q.push(&partida);
		float temp1, temp2;
		cout << "Coordenadas:" << endl;
		while (!Q.empty())
		{
			temp = Q.top();
			temp->visitado = true;
			camino.push_back(temp);
			cout << temp->x << "," << temp->y << " | ";
			Q = priority_queue<Punto*, vector<Punto* >, compare>();
			if (*temp == destino)
			{
				break;
			}
            //float temp1,temp2;
            for (int i = 0; i < temp->Aristas.size(); i++)
            {
                if (temp->Aristas[i]->B->visitado == false)
                {
                    temp1 = distancia_puntos(*temp->Aristas[i]->A, *temp->Aristas[i]->B);
                    temp2 = distancia_puntos(*temp->Aristas[i]->B, destino);
                    temp->Aristas[i]->B->costo = temp1 + temp2;
                    Q.push(temp->Aristas[i]->B);
                }
            }

		}
		float res = 0;
		cout << endl << "Puntos: " << endl;
        for (int i = 0; i < camino.size(); i++)
        {
            cout << camino[i]->id << "  ";

            if (i+1 != camino.size())
            {
                res = res + distancia_puntos(camino[i],camino[i+1]);
            }
        }
        /*cout << endl << "Coordenadas: " << endl;
        for(int i = 0; i < camino.size(); i++){
            cout << cout << camino[i]->x << "," << camino[i]->y << " | ";
        }*/
        cout << endl<< "Distancia total A_asterisco: " << res << endl;

	}
	struct compare
	{
		bool operator()(Punto*& C, Punto*& D)
		{
			return C->costo > D->costo;
		}
	};
	priority_queue<Punto*, vector<Punto* >, compare> Q;
	vector<Punto*> camino;
};


int main()
{
    Lista_Adyacencia grafo(200);
    Asterisco asterisk;
	CrearPuntos();
	//cout << "AQUI" << endl;
	KNN();
    //cout << "AQUI" << endl;
    for(int i = 0; i < Puntos.size(); i++){
        for(int j = 0; j < Puntos[i].Aristas.size(); j++){
            Edged nuevo(Puntos[i].Aristas[j]->A, Puntos[i].Aristas[j]->B);
            grafo.Insert(nuevo);
        }
    }
    int pos1, pos2;
    Punto* extr1;
    Punto* extr2;
    pos1 = ext_izq_sup(Puntos);
    pos2 = ext_der_inf(Puntos);
    ///------------------------------
    grafo.dijkstra(pos1,pos2);
    ///------------------------------
	extr1 = &Puntos[pos1];
	extr2 = &Puntos[pos2];

	//cout << "AQUI" << endl;
	asterisk.buscar(*extr1, *extr2);

	return 0;
}

