#include <iostream>
#define UNVISITED 0
#define VISITED 1
//#define INF 9999999
using namespace std;

class Graph {
  int numVertex, numEdge;
  int **matrix;
  // Pointer to adjacency matrix
  int *mark;
  bool type;
public:
  Graph(int numVert, bool t): type(t){ Init(numVert); }
  ~Graph() {
    delete [] mark;
    for (int i=0; i<numVertex; i++)
      delete [] matrix[i];
    delete [] matrix;
  }
  void Init(int n) {
    int i;
    numVertex = n;
    numEdge = 0;
    mark = new int[n];
    // Initialize mark array
    for (i=0; i<numVertex; i++)
      mark[i] = UNVISITED;
    matrix = (int**) new int*[numVertex]; // Make matrix
    for (i=0; i<numVertex; i++)
      matrix[i] = new int[numVertex];
    for (i=0; i< numVertex; i++) // Initialize to 0 weights
      for (int j=0; j<numVertex; j++)
	matrix[i][j] = 0;
  }
  int n() { return numVertex; } // Number of vertices
  int e() { return numEdge; }
  // Number of edges
  // Return first neighbor of "v"
  int first(int v) {
    for (int i=0; i<numVertex; i++)
      if (matrix[v][i] != 0) return i;
    return numVertex;
    // Return n if none
  }
  // Return v’s next neighbor after w
  int next(int v, int w) {
    for(int i=w+1; i<numVertex; i++)
      if (matrix[v][i] != 0)
	return i;
    return numVertex;
    // Return n if none
  }
  // Set edge (v1, v2) to "wt"
  void setEdge(int v1, int v2, int wt) {
    if (wt<0) cout << "Error" << "\n";
    if (matrix[v1][v2] == 0) numEdge++;
    matrix[v1][v2] = wt;
    if (!type) matrix[v2][v1] = wt;
  }
  void delEdge(int v1, int v2) { // Delete edge (v1, v2)
    if (matrix[v1][v2] != 0) numEdge--;
    matrix[v1][v2] = 0;
    if (!type) matrix[v2][v1] = 0;
  }
  bool isEdge(int i, int j) // Is (i, j) an edge?
  { return matrix[i][j] != 0; }
  int weight(int v1, int v2) { return matrix[v1][v2]; }
  int getMark(int v) { return mark[v]; }
  void setMark(int v, int val) { mark[v] = val; }
};

/*
int minVertex(Graph* G, int* D) {
  int i, v = -1;
  for (i=0; i<G->n(); i++)
    if (G->getMark(i) == UNVISITED) { v = i; break; }
  for (i++; i<G->n(); i++) 
    if ((G->getMark(i) == UNVISITED) && (D[i] < D[v]))
      v = i;
  return v;
}

void Dijkstra(Graph* G, int* D, int s) {
  int i, v, w;
  for (int i=0; i<G->n(); i++)
    D[i] = INFINITY;
  D[s] = 0;
  for (i=0; i<G->n(); i++) {
    v = minVertex(G, D);
    if (D[v] == INFINITY) return;
    G->setMark(v, VISITED);
    for (w=G->first(v); w<G->n(); w = G->next(v,w))
      if (D[w] > (D[v] + G->weight(v, w)))
	D[w] = D[v] + G->weight(v, w);
  }
}

int main(int argc, char *argv[])
{
  int N = 4;
  int D[N];
  Graph G(N);
  G.setEdge(1,2,5);
  G.setEdge(1,3,2);
  G.setEdge(2,0,2);
  G.setEdge(3,0,3);
  G.setEdge(0,1,6);
  
  Dijkstra(&G,D,2);

  for (int i = 0; i < N; i++) {
    cout << D[i] << " ";
  }
  cout<< "\n";
  
  return 0;
}
*/
