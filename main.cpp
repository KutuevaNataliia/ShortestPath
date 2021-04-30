#include <iostream>
#include <fstream>

typedef std::pair<int,int> E;

int min(int x, int y) {
    if (x < y) {
        return x;
    }
    return y;
}

void floydInit(int n, int m, int** C, int* weight, E* edges, int INFINITE) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            C[i][j] = INFINITE;
        }
    }
    for (int i = 0; i < n; i++) {
        C[i][i] = 0;
    }
    for (int i = 0; i < m; i++) {
        C[edges[i].first][edges[i].second] = min(C[edges[i].first][edges[i].second], weight[i]);
    }
}

int** createQMatrix(int n, int m, E* edges) {
    int** Q = new int* [n];
    for (int i = 0; i < n; i++) {
        Q[i] = new int[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Q[i][j] = -1;
        }
    }
    for (int i = 0; i < m; i++) {
        Q[edges[i].first][edges[i].second] = edges[i].second;
    }
    return Q;
}

void floyd(int n, int** C, int** Q) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(C[i][k] + C[k][j] < C[i][j]) {
                    C[i][j] = C[i][k] + C[k][j];
                    Q[i][j] = Q[i][k];
                }
            }
        }
    }
}

void restorePaths(int n, int** C, int** Q, int INFINITE, std::ofstream &output) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (C[i][j] > 0 && C[i][j] < INFINITE) {
                int s = i;
                int t = j;
                while (s != t) {
                    output << (char)('a' + s) << ' ';
                    s = Q[s][t];
                }
                output << (char)('a' + t) << '(' << C[i][j] << ")\n";
            }
        }
    }
}

int main() {
    std::ifstream input("input.txt");
    if (!input) {
        exit(1);
    }
    int n, m;
    input >> n >> m;

    E *edges = new E[m];
    int *weight = new int[m];
    char u, v;
    int maxWeight = 0;
    for (int i = 0; i < m; i++) {
        input >> u >> v >> weight[i];
        if (weight[i] > maxWeight) {
            maxWeight = weight[i];
        }
        edges[i].first = u - 'a';
        edges[i].second = v - 'a';
    }
    input.close();

    int **C = new int* [n];
    for (int i = 0; i < n; i++) {
        C[i] = new int[n];
    }
    int infinite = maxWeight * n + 1;
    floydInit(n, m, C, weight,edges, infinite);
    int** Q = createQMatrix(n, m, edges);
    floyd(n, C, Q);

    std::ofstream output("output.txt");
    if (!output) {
        exit(2);
    }
    restorePaths(n, C, Q, infinite, output);
    output.close();

    return 0;
}
