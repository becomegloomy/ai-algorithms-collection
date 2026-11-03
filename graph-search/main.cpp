#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;

const int INF = numeric_limits<int>::max(); 
const double PHEROMONE = 1.0;
const double HEURISTICS = 2.0;
const double EVAPORATION = 0.5;
const double F = 100.0; 

struct Edge {
    int index; 
    int weight; 
    double pheromone;
};

class Graph {
private:
    int vertices; 
    vector<vector<Edge>> adjList;
    vector<int> h;

public:
    Graph(int v) : vertices(v), adjList(v), h(v, 0) {}

    void addEdge(int start, int finish, int weight) {
        adjList[start - 1].push_back({ finish - 1, weight, 1.0 });
    }
    void setHeuristic(int vertex, int heuristic) {
        h[vertex - 1] = heuristic;
    }

    vector<int> algorithmA(int start, int finish) {
        vector<int> g(vertices, INF);
        vector<int> prev(vertices, -1);
        set<pair<int, int>> opened; 
        vector<int> openOut;
        vector<int> closed;
        vector<int> closedOrder;
        set<int> openedCheck;

        int step = 0;
        g[start - 1] = 0;
        opened.insert({ 0, start - 1 });
        openOut.push_back(start - 1);
        openedCheck.insert(start - 1);

        cout << endl << "Запуск алгоритма А* " << endl << endl;

        while (!opened.empty()) { 
            int current = opened.begin()->second; 
            opened.erase(opened.begin());
            closed.push_back(current);
            closedOrder.push_back(current);
            openOut.erase(remove(openOut.begin(), openOut.end(), current), openOut.end());
            openedCheck.erase(current);

            cout << "Шаг поиска " << ++step << ":" << endl;
            cout << "Раскрытые вершины: ";
            for (int v : closed) {
                cout << v + 1 << " ";
            }
            cout << endl;

            cout << "Нераскрытые вершины: ";
            for (int v : openOut) {
                cout << v + 1 << " ";
            }
            cout << endl;
            cout << "----------------------------" << endl;

            if (current == finish - 1) break;

            for (const auto& edge : adjList[current]) { 
                int neighbor = edge.index;
                int distNeighbor = g[current] + edge.weight;
                if (distNeighbor < g[neighbor]) { 
                    opened.erase({ g[neighbor] + h[neighbor], neighbor });
                    g[neighbor] = distNeighbor;
                    prev[neighbor] = current;
                    opened.insert({ g[neighbor] + h[neighbor], neighbor });
                    if (openedCheck.find(neighbor) == openedCheck.end()) {
                        openOut.push_back(neighbor);
                        openedCheck.insert(neighbor);
                    }
                }
            }
        }

        cout << "Последовательность раскрываемых вершин: ";
        for (int v : closedOrder) {
            cout << v + 1 << " ";
        }
        cout << endl;

        vector<int> path;
        for (int at = finish - 1; at != -1; at = prev[at])
            path.push_back(at);
        reverse(path.begin(), path.end());

        cout << "Длина кратчайшего пути: " << g[finish - 1] << endl;

        cout << "Кратчайший путь: ";
        for (int v : path) {
            cout << v + 1 << " ";
        }
        cout << endl;

        return path;
    }


    vector<int> antAlgorithm(int start, int finish, int colonySize) {
        srand(time(0));
        vector<int> bestPath;
        double bestPathDist = INF;
        vector<vector<int>> antPaths(colonySize);

        cout << endl << "Запуск муравьиного алгоритма " << endl << endl;

        for (int cycle = 0; cycle < 3; ++cycle) { 
            cout << "Цикл " << cycle + 1 << ":" << endl;

            antPaths.clear();
            antPaths.resize(colonySize);

            for (int ant = 0; ant < colonySize; ++ant) {
                vector<int> path;
                vector<bool> visited(vertices, false); 
                int current = start - 1;
                path.push_back(current); 
                visited[current] = true;

                while (current != finish - 1) { 
                    vector<pair<int, double>> probabilities;  
                    double sumProbabilites = 0.0; 
                    for (const auto& edge : adjList[current]) { 
                        if (!visited[edge.index]) {  
                            double pheromone = edge.pheromone; 
                            double heuristic = 1.0 / (edge.weight + 1);
                            double probability = pow(pheromone, PHEROMONE) * pow(heuristic, HEURISTICS);
                            probabilities.push_back({ edge.index, probability });
                            sumProbabilites += probability;
                        }
                    }

                    double random = rand() / (double)RAND_MAX * sumProbabilites;
                    double accumulatedProbability = 0.0;
                    for (const auto& prob : probabilities) {
                        accumulatedProbability += prob.second;
                        if (accumulatedProbability >= random) {
                            current = prob.first;   
                            path.push_back(current);
                            visited[current] = true;
                            break;  
                        }
                    }
                }

                antPaths[ant] = path; 
                double pathDist = 0;
                for (int i = 0; i < path.size() - 1; ++i) { 
                    int u = path[i];
                    int v = path[i + 1];
                    for (const auto& edge : adjList[u]) {  
                        if (edge.index == v) {  
                            pathDist += edge.weight;
                            break;
                        }
                    }
                }
                if (pathDist < bestPathDist) {
                    bestPathDist = pathDist;
                    bestPath = path;  
                }
            }

            cout << "Пути муравьев: " << endl;
            for (int ant = 0; ant < colonySize; ++ant) {
                cout << "Муравей " << ant + 1 << ": ";
                for (int v : antPaths[ant]) {
                    cout << v + 1 << " ";
                }
                cout << endl;
            }

            for (auto& row : adjList) {
                for (auto& edge : row) {
                    edge.pheromone *= (1 - EVAPORATION);
                }
            }
            for (const auto& path : antPaths) {  
                double affectAnt = F / path.size();
                for (int i = 0; i < path.size() - 1; ++i) {  
                    int u = path[i];
                    int v = path[i + 1];
                    for (auto& edge : adjList[u]) {  
                        if (edge.index == v) {  
                            edge.pheromone += affectAnt;
                            break;
                        }
                    }
                }
            }

            cout << "Состояние феромонов: " << endl;
            for (int u = 0; u < adjList.size(); ++u) {
                for (const auto& edge : adjList[u]) {
                    cout << "Ребро (" << u + 1 << " -> " << edge.index + 1
                        << "): Феромон = " << edge.pheromone << endl;
                }
            }
            cout << "----------------------------" << endl;
        }

        cout << "Длина кратчайшего пути: " << bestPathDist << endl;

        cout << "Кратчайший путь: ";
        for (int v : bestPath) {
            cout << v + 1 << " ";
        }
        cout << endl;

        return bestPath;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    int vertices, edges;
    cout << "Введите количество вершин: ";
    cin >> vertices;
    Graph graph(vertices);

    cout << "Введите количество ребер: ";
    cin >> edges;
    cout << "Введите каждое ребро (начало конец вес):" << endl;
    for (int i = 0; i < edges; ++i) {
        int start, finish, weight;
        cin >> start >> finish >> weight;
        graph.addEdge(start, finish, weight);
    }

    int start, finish;
    cout << "Введите начальную вершину: ";
    cin >> start;
    cout << "Введите целевую вершину: ";
    cin >> finish;

    cout << "Выберите алгоритм (1 - А*, 2 - Муравьиный): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Введите эвристические значения для каждой вершины:" << endl;
        for (int i = 1; i <= vertices; ++i) {
            int heuristic;
            cout << "Эвристика для вершины " << i << ": ";
            cin >> heuristic;
            graph.setHeuristic(i, heuristic);
        }
        vector<int> path = graph.algorithmA(start, finish);
        cout << endl;
    }
    else if (choice == 2) {
        int colonySize;
        cout << "Введите размер колонии: ";
        cin >> colonySize;
        vector<int> path = graph.antAlgorithm(start, finish, colonySize);
        cout << endl;
    }
    else {
        cout << "Некорректный выбор алгоритма!" << endl;
    }

    return 0;
}
