//  Trabalho 2 PAA
//  Andre Luiz RA: 2090279
//  Everaldo Junior   RA: 1798200

#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

/*
 * TODO
 * 1. Mudar nome de funções
 * 2. Isolar código
 * 3. Mudar ordem das funções
 *
*/


// Estrutura de armazenamento dos Pontos.
class Point{
  public:
    
    // Coordenadas x, y e suas respectivas características
    float x;
    float y;
    int parent; // Vértice pai
    float dist; // Menor distancia para o próximo vértice
    bool visited; // Indicador se foi visitado
    
    // Override no operador ==
    bool operator==(const Point &p) {
        return x == p.x && y == p.y;
    }
};

// Estrutura de armazenamento das Aresta.
class Edge{
	public:
    // Arestas do vértice
    Point right;
    Point left;
    float weight; // Custo/Peso da aresta
};

// Calculo da distância euclidiana entre 2 pontos
float euclidianDistance(Point a, Point b){
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Salva o ciclo do grafo em um arquivo cycle.txt
void makeCycleFile(vector<Point> points){
  ofstream file;
  file.open("cycle.txt");
  for (int i = 0; i < points.size(); i++){
    file << points[i].x << " " << points[i].y << "\n";
  }
  file.close();
}

// Função responsavel pelo armazenamento em output da AGM, gerando o arquivo tree.txt
void makeTreeFile(vector<Edge> edges){
  // Abre e sobrescreve o arquivo
  ofstream file;
  file.open("tree.txt");
  for (int i = 0; i < edges.size(); i++){
    // Verifica se a edge não tem loop infinito
    if (edges[i].left.x != edges[i].right.x || edges[i].left.y != edges[i].right.y){
      // Se entrar salva a aresta no arquivo
      file << edges[i].left.x << " " << edges[i].left.y << "\n";
      file << edges[i].right.x << " " << edges[i].right.y << "\n";
    }
  }
  file.close();
}

// Inicializa points para o Algoritmo Prim
void initializePoint(Point &point) {
    point.visited = false;
    point.dist = numeric_limits<float>::max();
    point.parent = -1;
}

// Realiza o Algoritmo Prim dentro de points
vector<Edge> prim(vector<Point> points) {
  vector<Edge> edges;

	// Percorre os edges
  for (int i = 0; i < points.size(); i++) { // O(n)
    initializePoint(points[i]);
  }

	// Inicializa o primeiro ponto com distancia 0
  points[0].dist = 0;

	// Percorre até todos os vértices serem visitados
  for (int i = 0; i < points.size(); i++) { // Complexidade O(2n^2)
    int min = -1;

    // Procura o ponto com menor distância
    for (int j = 0; j < points.size(); j++) { // O(n)
      // Verifica se já foi visitado e se points[j] está com a menor distância
      if (points[j].visited == false && (min == -1 || points[j].dist < points[min].dist))
        // Se entrar a menor distancia passa a ser points[j]
        min = j;
    }

    points[min].visited = true;

    // min é o parent dos seguintes pontos
    for (int j = 0; j < points.size(); j++) { // O(n)
      // Verifica se o ponto já foi visitado senão verifica se a distância do ponto j é menor que a distância do ponto min
      if (points[j].visited == false && (euclidianDistance(points[min], points[j]) < points[j].dist)) {
        points[j].dist = euclidianDistance(points[min], points[j]);
        points[j].parent = min;
      }
    }
  }

	// Adiciona as arestas ao vetor de edges
  for (int i = 0; i < points.size(); i++) { // O(n)
    if (points[i].parent != -1) {
      Edge currentEdge;
      currentEdge.left = points[points[i].parent]; 
      currentEdge.right = points[i];
      currentEdge.weight = euclidianDistance(currentEdge.left, currentEdge.right);
      edges.push_back(currentEdge);
    }
  }

  // Inverte as arestas para que o grafo seja direcionado
  reverse(edges.begin(), edges.end());

  return edges;
}


// calcula o peso do ciclo.
float getCycleWeight(vector<Point> cycle){
    float weight = 0;

    // Percorre o vetor de pontos do ciclo e calcula o peso de cada aresta
    for (int i = 0; i < cycle.size() - 1; i++){
        weight = weight + euclidianDistance(cycle[i], cycle[i + 1]);
    }

    // Retorna o valor de peso do ciclo (soma dos pesos das arestas)
    return weight * 100;
}


// Realiza uma busca em profundidade a partir das edges obtidas pela função prim(), a partir de um ponto de início "start"
vector<Point> dfs(vector<Edge> edges, Point start) {
  vector<Point> stack; // pilha de pontos para realizar a BP
  vector<Point> cycle; // ciclo a ser retornado ao final do algoritmo
  vector<Point> visited; // pontos que foram visitados pelo algoritmo
    
  // Adiciona o primeiro ponto na pilha
  stack.push_back(start);

    // Enquanto a pilha não estiver vazia
  while (!stack.empty()){
    // Pega o topo da pilha
    Point p = stack.back();
    // Remove o topo da pilha
    stack.pop_back();

    // Veinta se o ponto procurado (retirado da pilha) já foi visitado
    if (find(visited.begin(), visited.end(), p) == visited.end()){

      // Adiciona o ponto ao vetor de visitados
      visited.push_back(p);

      // Adiciona os vizinhos do ponto ao vetor de pilha

      // x == p.x && y == p.y
      // Adiciona os vizinhos do ponto ao vetor de pilha
      for (int i = 0; i < edges.size(); i++){
        Edge currentEdge = edges[i];

        if (currentEdge.left.x == p.x && currentEdge.left.y == p.y){
          stack.push_back(currentEdge.right);
        }
        else if (currentEdge.right.x == p.x && currentEdge.right.y == p.y){
          stack.push_back(currentEdge.left);
        }
      }
    }
  }

  // Insere os pontos visitados no vetor do ciclo
  for (int i = 0; i < visited.size(); i++){
    cycle.push_back(visited[i]);
  }

  // Insere o primeiro ponto no vetor do ciclo (para fechar o ciclo)
  cycle.push_back(start);
  return cycle;
}

void addPointToVector(vector<Point> &points, string line) {
        size_t index = line.find(' ');
        float x = stof(line.substr(0, index));
        float y = stof(line.substr(index + 1));
        Point p({x, y});
        points.push_back(p);
}

int main(int argc, char *argv[]){
    clock_t start;
    clock_t end;
    ifstream inFile(argv[1]);


    vector<Point> points;

    string line;
    getline(inFile, line);

    // Leitura dos pontos do arquivo
    while (getline(inFile, line)){
      addPointToVector(points,line);
    }

    // Inicia o cronômetro
    start = clock();

    vector<Edge> edges = prim(points);
    makeTreeFile(edges);
    
    // Ordena as arestas pelo peso
    sort(edges.begin(), edges.end(), [](Edge e1, Edge e2) { return e1.weight < e2.weight; });

	vector<Point> cycle = dfs(edges, points[0]);
    makeCycleFile(cycle);
    float weight = getCycleWeight(cycle);

    // Finaliza o cronômetro
    end = clock();

    // Imprime o tempo de execução e o peso do ciclo
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << fixed << setprecision(6) << time_taken << " " << weight << "\n";
    
	inFile.close();
	return 0;
}