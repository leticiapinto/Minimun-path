#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <stdlib.h>  
#include <array>

#include <SFML/Graphics.hpp>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"
#include "search.h"

#define INF 9999999
#define WIDTH 1000
#define HEIGHT 650 

using namespace std;


struct My_edge{
  int node_o, node_d, w;
  My_edge(int o,int d):node_o(o),node_d(d){}
};

template <class T>
struct Node{
  int x, y;
  T id;
  Node(){}
  Node(int x_, int y_, T i):x(x_),y(y_), id(i){}
};

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int minVertex(Graph* G, int* D) {
  int i, v = -1;
  for (i=0; i<G->n(); i++)
    if (G->getMark(i) == UNVISITED) { v = i; break; }
  for (i++; i<G->n(); i++) 
    if ((G->getMark(i) == UNVISITED) && (D[i] < D[v]))
      v = i;
  return v;
}

void dijkstra(Graph* G, int* D, int* route, int s) {
  int i, v, w,cont=0;
  for (int i=0; i<G->n(); i++)
    D[i] = INF;
  D[s] = 0;
  for (i=0; i<G->n(); i++) {
    v = minVertex(G, D);
    if (D[v] == INFINITY) return;
    G->setMark(v, VISITED);
    for (w=G->first(v); w<G->n(); w = G->next(v,w))
      if (D[w] > (D[v] + G->weight(v, w))){
	D[w] = D[v] + G->weight(v, w);
	route[w] = v;
	//cont++;
      }
  }
  //return cont;
}


std::vector<std::vector<int>> prepocesamiento(vector<int> L, vector<Node<int> > my_nodes, vector<My_edge> my_edges){


	std::vector<std::vector<int>> preproc;

	for (int i = 0; i < L.size(); i++) {
		std::vector<int> temp_vec(my_nodes.size());
		std::fill(temp_vec.begin(), temp_vec.end(), 0);
		preproc.push_back(temp_vec);
	}


	for (int i = 0; i < L.size(); i++) {
		for (int j = 0; j < my_nodes.size(); j++) {
		    cout<<preproc[i][j]<<" ";
		}
		cout<<"\n";
	}

	for(auto i = 0; i < L.size(); i++){
	  	for (int j=0; j< my_nodes.size(); j++){
	  		int dist =0;
	  		if(L[i]!=j){
	  			Graph G(my_nodes.size(),0);
			int D[1000],route[1000];
			for (auto it = my_edges.begin(); it != my_edges.end(); ++it) {
			    G.setEdge(it->node_o,it->node_d,1);
			}
	  		dijkstra(&G,D,route,L[i]);

	  		int a = j,b;
	  		
		  	do{
			    b = route[a];
			    //cout << a << " <- ";
			    dist++;
			    a = b;
			  } while (a != L[i]);
	  		}
		  	//cout<<node_src<<"\n";
	  		cout<<"ruta de i =  '"<< L[i] <<"' a j = '"<< j<< "' : "<<dist<<"\n";
	  		preproc[i][j] = dist;
	  	}
	  	cout<<"\n";
	}

	return preproc;

}

int main(){

	  srand (time(NULL));
	  float numberPoints = roundf(RandomFloat(20, 100));

	  vector<Vector2<float>> points;
	  vector<My_edge> my_edges;
	  vector<Node<int>> my_nodes;
	  
	  for(int i = 0; i < numberPoints; i++) {
	    float x = RandomFloat(0,WIDTH-1);
	    float y = RandomFloat(0,HEIGHT-1);
	    points.push_back(Vector2<float>(x,y));
	    my_nodes.push_back(Node<int>((int)x,(int)y,i));
	  }
	  
	  Delaunay<float> triangulation;
	  std::vector<Triangle<float>> triangles = triangulation.triangulate(points);
	  std::vector<Edge<float>> edges = triangulation.getEdges();
	  
	  for(auto &e : edges){
	    /*convertimo a otro formaro los edges
	     en este momento no tienen pesos*/
	    /*buscar que nodo tiene las coordenadas de e*/
	    int i,j;
	    for (i = 0; i < points.size(); i++) {
	      if (points[i] == e.p1) break;
	    }
	    for (j = 0; j < points.size(); j++) {
	      if (points[j] == e.p2) break;
	    }
	    My_edge me(i,j);
	    my_edges.push_back(me);
	  }

	  cout << "Nodes :"  <<my_nodes.size() <<"\n";
	  cout << "Edges :" <<my_edges.size() <<"\n";

	  //tengo l pertenece a L
	  //tengo v pertenece a G


	  vector<int> L;

	  L.push_back(1);
	  L.push_back(4);
	  L.push_back(8);
	  L.push_back(16);

	  //vector<vector<int> > preproc;

	std::vector<std::vector<int>> preproc;
	
	preproc = prepocesamiento(L, my_nodes, my_edges);

	//despues del preprocesamiento tenemos :

	cout<<"\n";
	for (int i = 0; i < L.size(); i++) {
		for (int j = 0; j < my_nodes.size(); j++) {
		    cout<<preproc[i][j]<<" ";
		}
		cout<<"\n";
	}

  return 0;


}