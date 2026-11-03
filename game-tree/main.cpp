#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>

using namespace std;

// Структура узла игрового дерева
struct Node {
    int value;                  // Оценка листа 
    vector<Node*> children;     // Дочерние узлы
    string name;                // Имя 
    Node(string name, int value = 0) : name(name), value(value) {}
};

// Функция для добавления дочерних узлов
void addChildren(Node* parent, const vector<Node*>& children) {
    parent->children = children;
}

// Алгоритм минимакс
int minimax(Node* node, bool player, bool analysis) {
    // Проверка является ли узел листом, если да, то выводится имя и оценка
    if (node->children.empty()) {
        cout << "Лист " << node->name << ": " << node->value << endl;
        return node->value;
    }

    // Переменная для хранения лучшей оценки узла в зависимости от хода игрока
    int bestValue = player ? INT_MIN : INT_MAX;

    // Сортировка дочерних узлов в зависимости от порядка анализа
    if (!analysis) {
        reverse(node->children.begin(), node->children.end());
    }

    // Обход всех дочерних узлов текущего
    for (auto child : node->children) {
        // Вызов алгоритма 
        int childValue = minimax(child, !player, analysis);
        // Выбор лучшей оценки узла в зависимости от хода игрока
        if (player) {
            bestValue = max(bestValue, childValue);
        }
        else {
            bestValue = min(bestValue, childValue);
        }
    }

    // Вывод промежуточной оценки вершины
    cout << "Вершина " << node->name << " возвращает: " << bestValue << endl;
    return bestValue;
}

// Функция для вывода дерева
void tree(Node* node, int depth = 0) {
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << node->name << " (" << node->value << ")" << endl;
    for (auto child : node->children) {
        tree(child, depth + 1);
    }
}

// Функция для изменение значений листьев
void editValues(Node* node) {
    if (node->children.empty()) {
        cout << "Введите новое значение для листа " << node->name << " (текущее: " << node->value << "): ";
        cin >> node->value;
        return;
    }
    for (auto child : node->children) {
        editValues(child);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Создание игрового дерево
    Node* Z = new Node("Z");
    Node* A = new Node("A");
    Node* B = new Node("B");
    Node* C = new Node("C");
    Node* D = new Node("D");
    Node* E = new Node("E");
    Node* F = new Node("F");
    Node* G = new Node("G");
    Node* H = new Node("H");
    Node* I = new Node("I");
    Node* J = new Node("J");
    Node* K = new Node("K");
    Node* L = new Node("L");
    Node* M = new Node("M");
    Node* N = new Node("N");
    Node* O = new Node("O");
    Node* P = new Node("P");
    Node* Q = new Node("Q");
    Node* R = new Node("R");
    Node* S = new Node("S");
    Node* T = new Node("T");
    Node* U = new Node("U");
    Node* V = new Node("V");
    Node* W = new Node("W");
    Node* X = new Node("X");
    Node* Y = new Node("Y");

    Node* K1 = new Node("K1", 3);
    Node* K2 = new Node("K2", 2);

    Node* L1 = new Node("L1", 7);
    Node* L2 = new Node("L2", 6);

    Node* M1 = new Node("M1", 8);
    Node* M2 = new Node("M2", 4);
    Node* M3 = new Node("M3", 1);

    Node* N1 = new Node("N1", 6);
    Node* N2 = new Node("N2", 5);

    Node* O1 = new Node("O1", 8);
    Node* O2 = new Node("O2", 7);
    Node* O3 = new Node("O3", 9);

    Node* P1 = new Node("P1", 4);
    Node* P2 = new Node("P2", 3);

    Node* Q1 = new Node("Q1", 5);
    Node* Q2 = new Node("Q2", 4);
    Node* Q3 = new Node("Q3", 6);

    Node* R1 = new Node("R1", 3);
    Node* R2 = new Node("R2", 1);

    Node* S1 = new Node("S1", 4);
    Node* S2 = new Node("S2", 2);

    Node* T1 = new Node("T1", 5);
    Node* T2 = new Node("T2", 1);
    Node* T3 = new Node("T3", 4);

    Node* U1 = new Node("U1", 3);
    Node* U2 = new Node("U2", 1);

    Node* V1 = new Node("V1", 5);
    Node* V2 = new Node("V2", 0);

    Node* W1 = new Node("W1", 2);
    Node* W2 = new Node("W2", 3);

    Node* X1 = new Node("X1", 0);
    Node* X2 = new Node("X2", 4);
    Node* X3 = new Node("X3", 3);

    Node* Y1 = new Node("Y1", 7);
    Node* Y2 = new Node("Y2", 6);
    Node* Y3 = new Node("Y3", 7);

    addChildren(Z, { A, B, C });
    addChildren(A, { D, E });
    addChildren(D, { K, L, M });
    addChildren(K, { K1, K2 });
    addChildren(L, { L1, L2 });
    addChildren(M, { M1, M2, M3 });
    addChildren(E, { N, O, P });
    addChildren(N, { N1, N2 });
    addChildren(O, { O1, O2, O3 });
    addChildren(P, { P1, P2 });
    addChildren(B, { F, G, H });
    addChildren(F, { Q });
    addChildren(Q, { Q1, Q2, Q3 });
    addChildren(G, { R, S, T });
    addChildren(R, { R1, R2 });
    addChildren(S, { S1, S2 });
    addChildren(T, { T1, T2, T3 });
    addChildren(H, { U, V });
    addChildren(U, { U1, U2 });
    addChildren(V, { V1, V2 });
    addChildren(C, { I, J });
    addChildren(I, { W, X });
    addChildren(W, { W1, W2 });
    addChildren(X, { X1, X2, X3 });
    addChildren(J, { Y });
    addChildren(Y, { Y1, Y2, Y3 });

    // Ввод параметров
    cout << "Начальная структура дерева:" << endl;
    tree(Z);

    cout << "Хотите изменить оценки листьев? 1 - да, 2 - нет: ";
    int edit;
    cin >> edit;
    if (edit == 1) {
        editValues(Z);
        cout << "\nОбновленная структура дерева:" << endl;
        tree(Z);
        cout  << endl;
    }

    cout << "Выберите порядок анализа: 1 - слева-направо, 2 - справа-налево: ";
    int analysisChoice;
    cin >> analysisChoice;
    bool analysis = (analysisChoice == 1);

    cout << "Кто начинает? 1 - MAX, 2 - MIN: ";
    int playerChoice;
    cin >> playerChoice;
    bool player = (playerChoice == 1);

    cout << "\nЗапуск алгоритма минимакс" << endl;
    int result = minimax(Z, player, analysis);
    cout << "\nРезультат работы алгоритма минимакс: " << result << endl;

    return 0;
}
