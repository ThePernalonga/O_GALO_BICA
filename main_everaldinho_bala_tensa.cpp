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

// Estrutura que armazena os dados de um ponto.
class Point
{
    float x;
    float y; // Coordenadas do ponto
    int parent; // Índice do vértice pai
    float minDist; // Menor distância para o próximo vértice do grafo
    bool visited; // Controle para os nós visitados

public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    
    bool operator==(const Point &p) // Sobrecarga de operador para checar se o ponto inserido em uma das extremidades da aresta é igual ao ponto procurado
    {
        return x == p.x && y == p.y;
    }

   /*
   / getters e setters para as variáveis privadas do ponto
   */
    float getX() const { return this->x; }
    float getY() const { return this->y; }

    void setVisited(bool visited) { this->visited = visited; }
    bool getVisited() const { return this->visited; }

    void setParent(int parent) { this->parent = parent; }
    int getParent() const { return this->parent; }

    void setMinDist(float minDist) { this->minDist = minDist; }
    float getMinDist() const { return this->minDist; }

};

// Estrutura que armazena os dados de uma aresta.
class Edge
{
    Point u, v; // Extremidades da aresta
    float weight; // Peso de uma aresta

	public:
	
	/*
    / getters e setters para as variáveis privadas da aresta
    */
    Point getU() const { return this->u; }
    void setU(Point u) { this->u = u; }

    Point getV() const { return this->v; }
    void setV(Point v) { this->v = v; }
    
    float getWeight() const { return this->weight; }
    void setWeight(float weight) { this->weight = weight; }

};

// Calcula a distância euclidiana entre 2 pontos
float distanceBtwPoints(Point p1, Point p2)
{
    return sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
}

// Função auxiliar que salva a AGM em um arquivo chamado tree.txt
void saveAGM(vector<Edge> edges)
{
    ofstream file;
    file.open("tree.txt");
    for (int i = 0; i < edges.size(); i++)
    {
        // Verifica se a edge não tem loop em si mesma
        if (edges[i].getU().getX() != edges[i].getV().getX() || edges[i].getU().getY() != edges[i].getV().getY())
        {
            file << edges[i].getU().getX() << " " << edges[i].getU().getY() << "\n"; // Salva a aresta no arquivo
            file << edges[i].getV().getX() << " " << edges[i].getV().getY() << "\n";
        }
    }
    file.close();
}

// Realiza o algoritmo Prim dentro do vetor de points
vector<Edge> prims(vector<Point> points) {
    vector<Edge> edges;

	// Inicializa os edges com dados vazios
    for (int i = 0; i < points.size(); i++) // O(n)
    {
        points[i].setMinDist(numeric_limits<float>::max());
        points[i].setParent(-1);
        points[i].setVisited(false);
    }

	// ponto inicial
    points[0].setMinDist(0);

	// Enquanto não tiver visitado todos os pontos
    for (int i = 0; i < points.size(); i++) // O(n)
    {
        int minIndex = -1;

        // Encontra o ponto com menor distância
        for (int j = 0; j < points.size(); j++) // O(n^2)
        {
            // Verifica se o ponto já foi visitado e se ponto j é o ponto com menor distância
            if (points[j].getVisited() == false && (minIndex == -1 || points[j].getMinDist() < points[minIndex].getMinDist()))
                minIndex = j;
        }

        points[minIndex].setVisited(true);

        // minIndex é o parent dos seguintes pontos
        for (int j = 0; j < points.size(); j++) // O(n^2)
        {
            // Verifica se o ponto já foi visitado senão verifica se a distância do ponto j é menor que a distância do ponto minIndex
            if (points[j].getVisited() == false && distanceBtwPoints(points[minIndex], points[j]) < points[j].getMinDist())
            {
                points[j].setMinDist(distanceBtwPoints(points[minIndex], points[j]));
                points[j].setParent(minIndex);
            }
        }
    }

	// Adiciona as arestas ao vetor de edges
    for (int i = 0; i < points.size(); i++) // O(n)
    {
        if (points[i].getParent() != -1)
        {
            Edge e;
            e.setU(points[points[i].getParent()]); 
            e.setV(points[i]);
            e.setWeight(distanceBtwPoints(e.getU(), e.getV()));
            edges.push_back(e);
        }
    }

    // Inverte as arestas para que o grafo seja direcionado
    reverse(edges.begin(), edges.end());

    return edges;
}

// Salva o ciclo do grafo em um arquivo cycle.txt
void saveCycle(vector<Point> points)
{
    ofstream file;
    file.open("cycle.txt");
    for (int i = 0; i < points.size(); i++)
    {
        file << points[i].getX() << " " << points[i].getY() << "\n";
    }
    file.close();
}

// Realiza uma busca em profundidade a partir das edges obtidas pela função prims(), a partir de um ponto de início "start"
vector<Point> dfs(vector<Edge> edges, Point start)
{
    vector<Point> cycle; // ciclo a ser retornado ao final do algoritmo
    vector<Point> visited; // pontos que foram visitados pelo algoritmo
    vector<Point> stack; // pilha de pontos para realizar a BP

	// Adiciona o primeiro ponto na pilha
    stack.push_back(start);

    // Enquanto a pilha não estiver vazia
    while (!stack.empty())
    {
        // Pega o topo da pilha
        Point p = stack.back();
        // Remove o topo da pilha
        stack.pop_back();

        // Verifica se o ponto procurado (retirado da pilha) já foi visitado
		if (find(visited.begin(), visited.end(), p) == visited.end())
        {
            // Adiciona o ponto ao vetor de visitados
            visited.push_back(p);

            // Adiciona os vizinhos do ponto ao vetor de pilha
            for (int i = 0; i < edges.size(); i++)
            {
                if (edges[i].getU() == p)
                {
                    stack.push_back(edges[i].getV());
                }
                else if (edges[i].getV() == p)
                {
                    stack.push_back(edges[i].getU());
                }
            }
        }
    }

    // Insere os pontos visitados no vetor do ciclo
    for (int i = 0; i < visited.size(); i++)
    {
        cycle.push_back(visited[i]);
    }
	
    // Insere o primeiro ponto no vetor do ciclo (para fechar o ciclo)
    cycle.push_back(start);
    return cycle;
}

// calcula o peso do ciclo.
float cycleWeight(vector<Point> cycle)
{
    float weight = 0;

    // Percorre o vetor de pontos do ciclo e calcula o peso de cada aresta
    for (int i = 0; i < cycle.size() - 1; i++)
    {
        weight += distanceBtwPoints(cycle[i], cycle[i + 1]);
    }

    // Retorna o valor de peso do ciclo (soma dos pesos das arestas)
    return weight*100;
}


int main(int argc, char *argv[])
{
    clock_t start;
    clock_t end;

    ifstream inFile(argv[1]);
    string line;
    getline(inFile, line);

    vector<Point> points;

    // Leitura dos pontos do arquivo
    while (getline(inFile, line))
    {
        size_t index = line.find(' ');
        float x = stof(line.substr(0, index));
        float y = stof(line.substr(index + 1));
        Point p(x, y);
        points.push_back(p);
    }

    // Inicia o cronômetro
    start = clock();

    vector<Edge> edges = prims(points);
    saveAGM(edges);
    
    // Ordena as arestas pelo peso
    sort(edges.begin(), edges.end(), [](Edge e1, Edge e2) { return e1.getWeight() < e2.getWeight(); });

	vector<Point> cycle = dfs(edges, points[0]);
    saveCycle(cycle);
    float weight = cycleWeight(cycle);

    // Finaliza o cronômetro
    end = clock();

    // Imprime o tempo de execução e o peso do ciclo
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << fixed << setprecision(6) << time_taken << " " << weight << "\n";
    
	inFile.close();
	return 0;
}