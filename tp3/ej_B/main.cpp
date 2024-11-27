
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

//Tp3 - ejB //FabianGL 522/20 TDA

#define cotaMaxN 10000000

//***Implementación FK cp--algorithms***
//src: cp-algorithms.com/graph/edmonds_karp.html
int n, m;
vector<vector<int>> capacidades;
vector<vector<int>> adj_original;
vector<vector<int>> adj_a_modificarse;

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, cotaMaxN});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj_a_modificarse[cur]) {
            if (parent[next] == -1 && capacidades[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacidades[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(n); //n ó n+1??????
    int new_flow;

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacidades[prev][cur] -= new_flow;
            capacidades[cur][prev] += new_flow;
            cur = prev;
        }
    }
    return flow;
}
//***fin cita***

struct Arista {
    int desde;
    int hacia;
};

int main() {
    cin >> n;
    cin >> m;

    int suma_total_A = 0, suma_total_B = 0;
    //vector<vector<int>> movements(n, vector<int>(n, 0));
    int s = 0, t = 2*n + 1;
    capacidades.resize(n*2 + 2, vector<int>(n*2 + 2, 0)); //n*n capacidades,

    int inicio_primer_columna = 1;
    int inicio_segunda_columna = n+1;

    adj_original.resize(2*n+2);
    adj_a_modificarse.resize(2*n+2);


    //agrego los A de Source a primer columna
    for (int i = 0; i < n; i++) {
        int a_i; cin >> a_i;
        //conecto de nodo 0 a los 1 a n (primer columna de modelo)
        adj_original[s].push_back(inicio_primer_columna + i);
        adj_a_modificarse[s].push_back(inicio_primer_columna + i);
        //guardo capacidad
        capacidades[s][inicio_primer_columna + i] = a_i;
        suma_total_A += a_i;
    }

    //agrego los B de segunda columna a sumidero
    for (int i = 0; i < n; i++) {
        int b_i; cin >> b_i;
        //conecto de nodo 0 a los 1 a n (ultima columna de modelo)
        adj_original[inicio_segunda_columna + i].push_back(t);
        adj_a_modificarse[inicio_segunda_columna + i].push_back(t);
        //guardo capacidad
        capacidades[inicio_segunda_columna + i][t] = b_i;
        suma_total_B += b_i;
    }

    //caso borde
    if (suma_total_A != suma_total_B) {
        cout << "NO" << endl;
        return 0;
    }

    //aristas 1-1 de primer columna hacia segunda
    for (int i = 0; i < n; i++) {
        adj_original[inicio_primer_columna + i].push_back(inicio_segunda_columna + i);
        adj_a_modificarse[inicio_primer_columna + i].push_back(inicio_segunda_columna + i);
        capacidades[inicio_primer_columna + i][inicio_segunda_columna + i] = cotaMaxN;
    }

    //aristas de aulas intercambiables
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        adj_original[inicio_primer_columna + x].push_back(inicio_segunda_columna + y);
        adj_a_modificarse[inicio_primer_columna + x].push_back(inicio_segunda_columna + y);
        capacidades[inicio_primer_columna + x][inicio_segunda_columna + y] = cotaMaxN;

        adj_original[inicio_primer_columna + y].push_back(inicio_segunda_columna + x);
        adj_a_modificarse[inicio_primer_columna + y].push_back(inicio_segunda_columna + x);
        capacidades[inicio_primer_columna + y][inicio_segunda_columna + x] = cotaMaxN;
    }

    const int max_flow = maxflow(s, t);

    //res puesta es hacer grafo original - grafo modificado?
    if (max_flow == suma_total_B) {
        cout << "YES" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << adj_original[i][j] - adj_a_modificarse[i][j]<< " ";
            }
            cout << endl;
        }
    } else {
        cout << "NO" << endl;
    }
    return 0;
}
