/*
  Nombre: Jose Hugo Jaita Aguilar
  Lo simulacion se hizo con un grafo no dirigido, pero la estructura tambien soporta
  grafos dirigidos.
  Todos los pesos los pusimos a 1, tambien funciona con pesos reales.
  Encuentra el camino mas corto entre un nodo origen y los demas
  
  Parte visual.
  Permite reacomodar los nodos con clik izquierdo
  Permite borrar nodos con click derecho

  se necesita instalar la libreria SFML
  sudo apt-get install libsfml-dev
  
  Compilacion
  g++ -g -std=c++11 -c main.cc && g++ -g -std=c++11 main.o -o main -lsfml-graphics -lsfml-window -lsfml-system && ./main

*/

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
  int i, v, w;
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
      }
  }
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

  /*****************Dijkstra****************/
  /*hay que cambiar esto a un vector*/
  int node_src = 0, node_dst = (int)RandomFloat(0,my_nodes.size()-1);
  int D[1000],route[1000];
  Graph G(my_nodes.size(),node_src);
  
  for (auto it = my_edges.begin(); it != my_edges.end(); ++it) {
    G.setEdge(it->node_o,it->node_d,1);
  }
  cout << "La ruta sera de: "<<node_src<<" -> "<<node_dst << "\n";
  dijkstra(&G,D,route,0);

  cout << "Los costos para cada nodo son:" << "\n";
  for (int i = 0; i < my_nodes.size(); i++) {
    cout << D[i] << " ";
  }
  cout<< "\n La ruta es:\n";

  int a = node_dst,b;
  do{
    b = route[a];
    cout << a << " <- ";
    a = b;
  } while (a != node_src);
  
  cout<<node_src<<"\n";
  /*****************************************/
  
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Search");
  bool is_pressed = false;
  int node_index;
  
  while (window.isOpen()){
    sf::Event event;
    while (window.pollEvent(event)){
      switch (event.type) {
      case sf::Event::Closed: {
	window.close();
	break;
      }
      case sf::Event::MouseButtonPressed: {
	if (event.mouseButton.button == sf::Mouse::Left){
	  Node<int> n;
	  int x, y;
	  int x_ = event.mouseButton.x, y_ = event.mouseButton.y;
	  for (int i = 0; i < my_nodes.size(); i++) {
	    n = my_nodes[i];
	    x = n.x, y = n.y;
	    if (x_>x && x_<x+10 && y_>y && y_<y+10){
	      cout <<"Node: "<<i<<endl;
	      node_index = i;
	      is_pressed = true;
	    }
	  }
	}
	if (event.mouseButton.button == sf::Mouse::Right){
	  Node<int> n;
	  int x, y;
	  int x_ = event.mouseButton.x, y_ = event.mouseButton.y;
	  for (int i = 0; i < my_nodes.size(); i++) {
	    n = my_nodes[i];
	    x = n.x, y = n.y;
	    if (x_>x && x_<x+10 && y_>y && y_<y+10){
	      cout <<"Nodo eliminado: "<<i<<endl;
	      /*remover el nodo seleccionado*/
	      my_nodes[i].id = -1;
	      /*remover todas los edges asociados con dicho node*/
	      auto it = my_edges.begin();
	      int tam = my_edges.size();
	      for (int j = 0; j < tam; j++) {
		if (it->node_o == i || it->node_d == i)
		  my_edges.erase(it);
		else ++it;
	      }
	    }
	  }	  
	}
	break;
      }
      case sf::Event::MouseButtonReleased: {
	is_pressed = false;
	break;
	}
      case sf::Event::MouseMoved: {
	if (is_pressed){
	  int i = node_index;
	  Node<int>* pn = &my_nodes[i];
	  int x_ = event.mouseMove.x, y_ = event.mouseMove.y;
	  pn->x = x_, pn->y = y_;
	  }
	break;
      }
  
	/*add more events*/ 
      }
    }

    
    window.clear();
    
    /*para nodos*/
    for(auto p = my_nodes.begin(); p != my_nodes.end(); p++) {
      if (p->id != -1){
	sf::RectangleShape rs = sf::RectangleShape(sf::Vector2f(10, 10));
	rs.setPosition(p->x, p->y);
	window.draw(rs);
      }
    }
    
    /*dibujar edges*/
    for (auto it = my_edges.begin(); it != my_edges.end(); ++it) {
      Node<int> n1 = my_nodes[it->node_o];
      Node<int> n2 = my_nodes[it->node_d];
      sf::Vertex line[] = {sf::Vertex(sf::Vector2f(n1.x,n1.y)),
			   sf::Vertex(sf::Vector2f(n2.x,n2.y))};
      window.draw(line,2,sf::Lines);
    }
    /*pintamos la ruta*/
    int n1 = node_dst;
    int n2;
    
    do{
      n2 = route[n1];
      Node<int> node1 = my_nodes[n1];
      Node<int> node2 = my_nodes[n2];
      sf::Vertex line[] = {sf::Vertex(sf::Vector2f(node1.x,node1.y),sf::Color::Red),
			   sf::Vertex(sf::Vector2f(node2.x,node2.y),sf::Color::Red)};
      window.draw(line,2,sf::Lines);
      n1=n2;	
    } while (n2 != node_src);
      
      
      window.display();
  }
  
  return 0;
}
