#include <iostream>
#include <float.h>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <vector>
#include <climits>

class Point
{
    float x, y;
    int key;
    int parent;
    int minDist;
    bool visited;

public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    Point &operator=(const Point &p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
    void setXY(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    float getX() const { return this->x; }
    float getY() const { return this->y; }
    void setVisited(bool visited) { this->visited = visited; }
    bool getVisited() const { return this->visited; }
    void setKey(int key) { this->key = key; }
    int getKey() const { return this->key; }
    void setParent(int parent) { this->parent = parent; }
    int getParent() const { return this->parent; }
    void setMinDist(int minDist) { this->minDist = minDist; }
    int getMinDist() const { return this->minDist; }
};

float distanceBtwPoints(Point p1, Point p2)
{
    return sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
}

int minKey(std::vector<Point> points)
{
    // Heap ou Priority Queue
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < points.size(); i++)
    {
        if ((points[i].getVisited() == false && min_index == -1) || (points[i].getVisited() == false && points[i].getKey() < min))
        {
            min = points[i].getKey();
            min_index = i;
        }
    }
    return min_index;
}
void saveAGM(std::vector<Point> agm){
  	std::ofstream fp("tree.txt");
	
	int i = 0;
    while (i < agm.size())
	{
        fp << agm[i].getX() << " "<< agm[i].getY() << std::endl;
        i++;
	}
  	fp.close();
}

std::vector<Point> primsAlgorithm(std::vector<Point> points)
{	
    std::vector<Point> mst;
    for (int i = 0; i < points.size(); i++)
    {
        points[i].setKey(INT_MAX);
		points[i].setVisited(false);
    }
    points[2].setKey(0);
    points[2].setParent(-1);
    for (int i = 0; i < points.size(); i++)
    {
        int min = minKey(points);
        points[min].setVisited(true);
        
		mst.push_back(points[min]);
        points[min].setKey(INT_MAX);
    
        for (int j = 0; j < points.size(); j++)
        {
			float dist = distanceBtwPoints(points[min], points[j]);
            if (min != j && points[j].getVisited() == false && dist < points[j].getKey())
            {
                points[j].setParent(min);
                points[j].setKey(dist);
            }
        }
    }

	std::cout << "MST" << std::endl;
	for(int i = 0; i < mst.size(); i++) {
		std::cout << mst[i].getX() << " " << mst[i].getY() << std::endl;
	}
    saveAGM(mst);
    return mst;
}

int main(int argc, char *argv[])
{
    clock_t start;
    clock_t end;

    std::ifstream inFile(argv[1]);
    std::string line;
    std::getline(inFile, line);

    std::vector<Point> points;

    while (std::getline(inFile, line))
    {
        std::size_t index = line.find(' ');
        float x = stof(line.substr(0, index));
        float y = stof(line.substr(index + 1));
        Point p(x, y);
        points.push_back(p);
    }
    start = clock();

    primsAlgorithm(points);

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    
    return 0;
}
/*
    1. computacao de uma grafo completo G = (V = P, E), tal que o custo de uma aresta
    (u, v) ∈ E ´e dado pela distˆancia Euclidiana entre os v´ertices u, v ∈ V ;
    
    2. computacao de uma Arvore Gerado Minima (AGM) ´ T = (V, E0 ⊂ E), para o grafo
    G, usando o algoritmo de Prim iniciando em um v´ertice s ∈ V ;
    
    3. computar uma Busca em Profundidade (BP) na ´arvore T, a partir do mesmo
    vertice s usado para iniciar o algoritmo de Prim, para criar a sequˆencia de v´ertices
    s, v1, v2, . . . , v|V |−1, s, vi 6= s para i = 1, 2, . . . , |V | − 1, que ser´a a solu¸c˜ao para o
    problema do caixeiro viajante (note que o primeiro v´ertice ´e repetido no final da
    sequˆencia para formar o ciclo.).
*/

/*
    tree.txt
    0 1
    2 1

    2 1
    3 0

    2 1
    2 2

    2 2
    3 3

    2 2
    1 3
    
    Saída
    0 1
    2 1
    2 1
    3 0
    2 1
    2 2
    2 2
    3 3
    2 2
    1 3

*/