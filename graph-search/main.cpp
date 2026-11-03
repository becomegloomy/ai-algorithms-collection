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

const int INF = numeric_limits<int>::max(); // Определение бесконечности
const double PHEROMONE = 1.0;   // Влияние феромонов
const double HEURISTICS = 2.0;  // Влияние эвристики
const double EVAPORATION = 0.5; // Скорость испарения феромонов
const double F = 100.0;  // Кол-во феромонов, добавляемое за один обход

// Структура ребра
struct Edge {
    int index; // Индекс
    int weight; // Вес 
    double pheromone; // Кол-во феромона
};

// Класс граф
class Graph {
private:
    int vertices; // Переменная для вершин
    vector<vector<Edge>> adjList; // Список смежности
    vector<int> h; // Эвристика

public:
    Graph(int v) : vertices(v), adjList(v), h(v, 0) {}

    // Метод для добавления ребра граф
    void addEdge(int start, int finish, int weight) {
        adjList[start - 1].push_back({ finish - 1, weight, 1.0 });
    }
    // Метод для добавления эвристической оценки вершине
    void setHeuristic(int vertex, int heuristic) {
        h[vertex - 1] = heuristic;
    }

    // Алгоритм А*
    vector<int> algorithmA(int start, int finish) {
        vector<int> g(vertices, INF); // Вектор для хранения минимального расстояния от начальной вершины до каждой другой
        vector<int> prev(vertices, -1);  // Для хранения предшественников каждой вершины
        set<pair<int, int>> opened;        // Множество нераскрытых вершин
        vector<int> openOut; // Список нераскрытых вершины для вывода в протокол поиска
        vector<int> closed;  // Список раскрытых вершины
        vector<int> closedOrder; // Последовательность раскрываемых вершин
        set<int> openedCheck; // Множество для проверки наличия вершин в списке нераскрытых

        int step = 0; // Счётчик шагов
        g[start - 1] = 0;  // Расстояние начальной вершины до самой себя
        // Добавление начальной вершины в нераскрытые
        opened.insert({ 0, start - 1 });
        openOut.push_back(start - 1);
        openedCheck.insert(start - 1);

        cout << endl << "Запуск алгоритма А* " << endl << endl;

        // Сам алгоритм
        while (!opened.empty()) {   // Пока есть нераскрытые вершины
            // Извлечение первой вершины из нераскрытых и перенос в раскрытые
            int current = opened.begin()->second; 
            opened.erase(opened.begin());
            closed.push_back(current);
            closedOrder.push_back(current);
            openOut.erase(remove(openOut.begin(), openOut.end(), current), openOut.end());
            openedCheck.erase(current);

            // Вывод протокола поиска по шагам
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

            // Проверка текущей вершины на целевую 
            if (current == finish - 1) break;

            // Обработка соседних вершин
            for (const auto& edge : adjList[current]) { // Перебор исходящих из вершины ребер
                int neighbor = edge.index;  // Извлечение индекса соседней вершины 
                int distNeighbor = g[current] + edge.weight; // Вычисление расстояния до соседней вершины
                // Проверка на кратчайший путь до соседней вершины
                if (distNeighbor < g[neighbor]) { 
                    // Обновление данных пути с учетом эвристики
                    opened.erase({ g[neighbor] + h[neighbor], neighbor });
                    g[neighbor] = distNeighbor;
                    prev[neighbor] = current;
                    opened.insert({ g[neighbor] + h[neighbor], neighbor });
                    // Проверка на добавление соседней вершины в список нераскрытых для избежания дублирования
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

        // Восстановление кратчайшего пути по предшествующим вершинам
        vector<int> path;
        for (int at = finish - 1; at != -1; at = prev[at])
            path.push_back(at);
        reverse(path.begin(), path.end()); // "Переворот" пути

        cout << "Длина кратчайшего пути: " << g[finish - 1] << endl;

        cout << "Кратчайший путь: ";
        for (int v : path) {
            cout << v + 1 << " ";
        }
        cout << endl;

        return path;
    }


    // Муравьиный алгоритм
    vector<int> antAlgorithm(int start, int finish, int colonySize) {
        srand(time(0)); // Генератор случайных чисел для выбора муравьем начального пути
        vector<int> bestPath; // Вектор для хранения кратчайшего пути
        double bestPathDist = INF; // Длина кратчайшего пути
        vector<vector<int>> antPaths(colonySize); // Пути муравьев

        cout << endl << "Запуск муравьиного алгоритма " << endl << endl;

        // Сам алгоритм по циклам
        for (int cycle = 0; cycle < 3; ++cycle) { 
            cout << "Цикл " << cycle + 1 << ":" << endl;

            antPaths.clear(); // Очистка путей муравьев
            antPaths.resize(colonySize); // Корректировка вектора под кол-во муравьев

            for (int ant = 0; ant < colonySize; ++ant) { // Для каждого муравья
                vector<int> path; // Путь муравья
                vector<bool> visited(vertices, false); // Массив посещённых вершин
                int current = start - 1;  // Текущая вершина - начальная
                path.push_back(current);  // Добавление вершины в путь
                visited[current] = true;  // Пометка вершины посещённой

                while (current != finish - 1) { // Пока не достигнута целевая вершина
                    // Вычисление вероятности перехода для каждого ребра, исходящего из текущей вершины
                    vector<pair<int, double>> probabilities;  // Вектор для хранения возможных путей
                    double sumProbabilites = 0.0;  // Переменная для хранения суммы всех вероятностей переходов для нормализации и генерации случайного числа для выбора пути
                    for (const auto& edge : adjList[current]) { // Для каждого ребра текущей вершины
                        if (!visited[edge.index]) {  
                            double pheromone = edge.pheromone;  // Получение значения феромона на ребре
                            double heuristic = 1.0 / (edge.weight + 1);  // Эвристики (обратная значение от веса ребра)
                            double probability = pow(pheromone, PHEROMONE) * pow(heuristic, HEURISTICS);  // Вычисление вероятности перехода по ребру
                            probabilities.push_back({ edge.index, probability });  // Добавляем ребра в возможные пути
                            sumProbabilites += probability;  // Обновление общей вероятности
                        }
                    }

                    // Выбор пути из возможных на основе вероятностного распределения с использованием случайного числа
                    double random = rand() / (double)RAND_MAX * sumProbabilites;  // Генерация случайного числа для выбора пути муравьем
                    double accumulatedProbability = 0.0;  // Переменная для накопления вероятностей и принятия решения по каждому конкретному ребру 
                    for (const auto& prob : probabilities) {  // Для каждой возможной вероятности
                        accumulatedProbability += prob.second;  // Вычисление накопленной вероятности
                        if (accumulatedProbability >= random) {  // Сравнение накопленной вероятности с случайным числом
                            current = prob.first;    // Переход по выбранному ребру
                            path.push_back(current); // Добавление вершины в путь
                            visited[current] = true; // Пометка вершины посещённой
                            break;  
                        }
                    }
                }

                antPaths[ant] = path;  // Сохранение пути муравья
                double pathDist = 0; // Переменная для хранения длины пути
                // Вычисление длины пути
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
                // Проверка является ли найденный путь кратчайшим
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

            // Обновление кол-ва феромонов на ребрах
            // Испарение 
            for (auto& row : adjList) {
                for (auto& edge : row) {
                    edge.pheromone *= (1 - EVAPORATION);
                }
            }
            // Влияние муравьев на кол-во феромонов
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
    
    // Ввод исходных данных
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
